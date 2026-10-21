#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import struct
import sys
import os
import threading
import time

from enum import Enum
import binascii
from tqdm import tqdm
from time import sleep


class serverStates(Enum):
	waitID = 0
	sendProgramToWrite = 1
	sendProgramToVerify = 2
	serverShutdown = 3


def get_local_ip():
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	s.connect(("1.0.0.1", 80))
	local_ip = s.getsockname()[0]  # Getting the local IP address
	s.close()
	return local_ip


def createBeaconMessage(server_ip, server_port):
	msg = (42).to_bytes(1, "big", signed=False)

	#Convertimos la IP del servidor a una tupla
	ip = struct.unpack('BBBB', socket.inet_aton(server_ip))

	msg = msg + ip[0].to_bytes(1, "big", signed=False)
	msg = msg + ip[1].to_bytes(1, "big", signed=False)
	msg = msg + ip[2].to_bytes(1, "big", signed=False)
	msg = msg + ip[3].to_bytes(1, "big", signed=False)

	#Incluimos el puerto
	msg = msg + server_port.to_bytes(2, "big", signed=False)

	return msg


def checkHexIntegrity(hexFile):

	hexFile.seek(0)

	for (c, l) in enumerate(hexFile):
		#Para cada linea, comprobamos que el formato sea el de un archivo
		#hexadecimal de Intel
		if(l[0] != ':'):
			return -1

		#Comprobamos que el tipo de datos es correcto
		recordType = int(l[7:9])
		if(recordType > 5 or recordType < 0):
			return -1

		recordLength = int(l[1:3], base=16)

		#Comprobamos el checksum
		checksum = 0
		for i in range(1, recordLength*2+9, 2):
			checksum  = checksum  + int(l[i:i+2], 16)

		#Calculamos el checksum en hexadecimal
		checksum = ((~checksum)+1) & 0xff

		if(int(l[recordLength*2+9:recordLength*2+11],16) != checksum):
			return -1

	return 0




class OAPworkerThread(threading.Thread):
	def __init__(self, conn, address, filePath):
		super(OAPworkerThread, self).__init__()

		self.conn = conn
		self.address = address
		self.filePath = filePath

		self.f = open(self.filePath, "r")    #Fichero que contiene el hexadecimal

		#Variables de la funcion de lectura del archivo hexadecimal
		self.lastAddress = 0
		self.lastLine = ""
		self.lastLineNextByteIndex = -1

	def computeProgramSize(self, hexFile):
		totalSize = 0

		hexFile.seek(0)

		for (c, l) in enumerate(hexFile):
			totalSize = totalSize + int(l[1:3], base=16)

		return totalSize


	def sendMessage(self, payload):
		length = len(payload).to_bytes(2, "big", signed=False)
		return length+payload


	def recieveMessage(self, data):
		messageLength = int.from_bytes(data[0:2], "big", signed=False)
		return data[2:messageLength+2]


	def showByteArray(self, byteArray):
		str = ""
		for i in byteArray:
			str = str+ hex(i) +" "
		return str


	def getAddress(self, line):
		return int(line[3:7], base=16)


	def getRecordLength(self, line):
		return int(line[1:3], base=16)


	def getRecordType(self, line):
		return int(line[7:10], base=16)


	def getRecord(self, line):

		recordLength = self.getRecordLength(line)
		return binascii.unhexlify(line[9:recordLength*2+9])


	def getProgramChunk(self, f, pageSize):

		if len(self.lastLine) == 0:
			f.seek(0)

		chunkAddress = -1

		data = bytes()
		line = ""

		sendedBytes = 0

		lastPos = 0

		while(sendedBytes < pageSize):

			#Si la última linea se procesó completamente, cargamos una nueva linea
			if(self.lastLineNextByteIndex == -1):

				lastPos = f.tell()
				line = f.readline()

				#Si no hemos llegado al final del archivo
				if(len(line) !=0 ):

					address = self.getAddress(line)

					#Si es la primera linea que transmitimos de un nuevo trozo
					#del programa, la direccion de la linea sera la direccion del trozo
					if(chunkAddress == -1):
						chunkAddress = address

					#Leemos la dirección y comprobamos si la direccion es consecutiva
					#a la direccion actual, si es así, procesamos los bytes, sino,
					#cerramos la pagina y devolvemos los bytes actuales
					if(self.lastAddress == address):
						if(pageSize - sendedBytes >= self.getRecordLength(line)):
							data = data + self.getRecord(line)
							self.lastAddress = self.getRecordLength(line)+ address
							sendedBytes = sendedBytes + self.getRecordLength(line)
							self.lastLineNextByteIndex = -1

						#Debemos añadir una cantidad parcial de bytes
						else:
							data  = data + self.getRecord(line)[0: pageSize-sendedBytes]
							self.lastAddress = address + pageSize-sendedBytes
							self.lastLineNextByteIndex = pageSize-sendedBytes
							sendedBytes = sendedBytes + pageSize - sendedBytes

						self.lastLine = line

					#Si las direcciones no son consecutivas, finalizamos
					else:
						self.lastAddress = address
						f.seek(lastPos)
						break

			#Si la última linea no se procesó completamente, la terminamos de procesar
			else:
				if(chunkAddress==-1):
					chunkAddress = self.lastAddress

				if(pageSize-sendedBytes >= self.getRecordLength(self.lastLine) - self.lastLineNextByteIndex ):
					data = data + self.getRecord(self.lastLine)[self.lastLineNextByteIndex:self.getRecordLength(self.lastLine) + 1]
					self.lastAddress = self.lastAddress + self.getRecordLength(self.lastLine) - self.lastLineNextByteIndex
					sendedBytes = sendedBytes + self.getRecordLength(self.lastLine) - self.lastLineNextByteIndex
					self.lastLineNextByteIndex = -1

				#Si aún quedan bytes en la linea pero la longitud máxima del chunk se ha alcanzado:
				else:
					data = data + self.getRecord(self.lastLine)[self.lastLineNextByteIndex : self.lastLineNextByteIndex + pageSize-sendedBytes]
					self.lastAddress = self.lastAddress + pageSize-sendedBytes
					self.lastLineNextByteIndex = self.lastLineNextByteIndex + pageSize-sendedBytes
					sendedBytes = sendedBytes + pageSize-sendedBytes

		#Convertimos la direccion y los datos a un array de bytes para su transmision
		return (chunkAddress.to_bytes(4, "big", signed = False) + data)


	def resetProgramChunkPos(self, f):
		self.lastAddress = 0
		self.lastLine = ""
		self.lastLineNextByteIndex = -1
		f.seek(0)


	###### RUN #####
	def run(self):

		serverState = serverStates.waitID

		with self.conn:

			totalSendedBytes = 0

			while(serverState != serverStates.serverShutdown):

				data = self.conn.recv(1024);

				if( not data):
					continue

				#print("	>> Debug, rcv data: " + data.decode().format())

				if(serverState == serverStates.waitID):

					if(data == bytes("argos", "utf-8")):
						#Enviamos el numero de bytes de programa
						programSize = self.computeProgramSize(self.f)
						print("Tamaño del programa: "+str(programSize)+" bytes")

						self.conn.sendall(self.sendMessage(programSize.to_bytes(4, "big", signed=False)))

						serverState = serverStates.sendProgramToWrite

						#LLevamos al inicio el lector de trozos de programa
						self.resetProgramChunkPos(self.f)
						totalSendedBytes = 0


						#writeBar = FillingCirclesBar('Escribiendo programa', max=programSize, suffix=" %(remaining) - %(max) Bytes")

						writeBar = tqdm(desc='Escribiendo programa', total=programSize, ncols=80, unit="Bytes", bar_format="{l_bar}{bar}|{n_fmt}/{total_fmt}, {rate_fmt}")


				elif(serverState == serverStates.sendProgramToWrite):

					#print("Datos recibidos: "+self.showByteArray(self.recieveMessage(data)))
					cmd = self.recieveMessage(data)
					if(cmd == bytes("\x06", "utf-8")):	#Si recibimos un ACK, enviamos el siguiente chunck

						if(totalSendedBytes < programSize):
							#Las paginas tendrán un tamaño de 128 bytes
							chunk = self.getProgramChunk(self.f, 128)
							self.conn.sendall(self.sendMessage(chunk))

							#print("Bytes enviados: "+str(totalSendedBytes)+" Chunk address: "+ str(int.from_bytes(chunk[0:4], "big", signed=False) )+" Chuenk enviado...")

							totalSendedBytes = totalSendedBytes + len(chunk) - 4

							writeBar.update(len(chunk) - 4)
							writeBar.refresh()

						#Programa enviado
						else:
							writeBar.close()
							sleep(0.1)

							print("Escritura del programa completada")
							serverState = serverStates.sendProgramToVerify

							# LLevamos al inicio el lector de trozos de programa
							self.resetProgramChunkPos(self.f)
							totalSendedBytes = 0

							verifyBar = tqdm(desc='Verificando programa', total=programSize, ncols=80, unit="Bytes", bar_format="{l_bar}{bar}|{n_fmt}/{total_fmt}, {rate_fmt}")

					else:
						writeBar.close()
						sleep(0.1)

						print("# Error al recibir confirmacion de envio de programa")
						print(" -> Error: "+cmd.decode("utf-8"))

						self.conn.close()
						serverState = serverStates.serverShutdown



				elif(serverState == serverStates.sendProgramToVerify):

					cmd = self.recieveMessage(data)
					if(cmd == bytes("\x06", "utf-8")):	#Si recibimos un ACK, enviamos el siguiente chunck

						if(totalSendedBytes < programSize):
							#Las paginas tendrán un tamaño de 128 bytes
							chunk = self.getProgramChunk(self.f, 128)
							self.conn.sendall(self.sendMessage(chunk))

							#print("Bytes enviados: "+str(totalSendedBytes)+" Chuenk enviado...")

							totalSendedBytes = totalSendedBytes + len(chunk) - 4

							verifyBar.update( len(chunk) - 4)
							verifyBar.refresh()

						#Programa enviado
						else:
							verifyBar.close()
							sleep(0.1)

							print("Verificacion del programa completada")
							print("\n¡Programa cargado con exito!")
							self.conn.close()
							serverState = serverStates.serverShutdown

					else:
						verifyBar.close()
						sleep(0.1)

						print("# Error al recibir confirmacion de envio de programa en verifiacion")
						print(" -> Error: " + cmd.decode("utf-8"))
						self.conn.close()
						f.self.close()
						serverState = serverStates.serverShutdown





class multicastBeaconThread(threading.Thread):
	def __init__(self, multicast_group, period, numberOfTries, eventOnBeaconFinish, message):
		super(multicastBeaconThread, self).__init__()

		self.multicast_group = multicast_group
		self.period = period
		self.numberOfTries = numberOfTries
		self.eventOnBeaconFinish = eventOnBeaconFinish
		self.message = message

	def run(self):

		try:

			sockBeacon = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

			ttl = struct.pack('b', 1)
			sockBeacon.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, ttl)

			for x in range(self.numberOfTries):
				sockBeacon.sendto(message, multicast_group)
				time.sleep(self.period)

		finally:
			sockBeacon.close()
			self.eventOnBeaconFinish.set()  #Informamos a la tarea principal que se ha terminado




##Server config
HOST = get_local_ip()
SERVER_PORT = 65434

##Comprobaciones preliminares

if (len(sys.argv) != 2):
	print("# Error: numero de argumentos introducidos inválido")
	print("python OAP_server.py <intel_hex_file_to_load> \n")
	sys.exit()

# Abrimos el archivo proporcionado por el usuario a través de la linea de comandos
try:
	f = open(sys.argv[1], "r")

	#Comprobamos que la extension es correcta
	if( os.path.splitext(f.name)[1] != ".hex"):
		print("# Error: extension de archivo incorrecta")
		sys.exit();

	#Verificamos la integridad del archivo
	if(checkHexIntegrity(f)!=0):
		print("# Error: Verificacion de la integridad del fichero fallida.")
		print("  Compruebe que es un archivo hexadecimal en formato Intel.\n")
		sys.exit()

except FileNotFoundError as exc:
	print("# Error: Fichero no encontrado")
	sys.exit()

f.close()

#message = b'very important data'

message = createBeaconMessage(HOST, SERVER_PORT)

multicast_group = ('239.0.0.42', 50042)

#print(message);

#Beacon config
period = 1
numberOfTries = 5   #5 segundos hasta terminar de emitir la baliza

eventOnBeaconFinish = threading.Event()

multicastBeacon = multicastBeaconThread(multicast_group, period, numberOfTries, eventOnBeaconFinish, message )

multicastBeacon.start()


print("┌".ljust(79, '─')+'┐')
print("│"+" Over Air Programming - ARGOS ".center(78, " ")+"│")
print("└".ljust(79, '─') + '┘')

print("programado por Carlos Berbell Sánchez\n".ljust(80))

print("Esperando conexión de plataformas ARGOS..")

OAPworkersList = []


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:

	#Esperamos conexiones entrantes
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

	s.bind((HOST, SERVER_PORT))

	s.listen()

	s.settimeout(0.5)

	keepListening = True

	while(keepListening):

		#print("Iteracion "+str(iteracion))
		#iteracion = 1+iteracion

		try:

			conn, address = s.accept()

			#with conn:

			print("ARGOS conectado: " + str(address[0]) + ":"+str(address[1]))

			#Desplegamos cada uno de los hilos
			worker = OAPworkerThread(conn, address, sys.argv[1])

			OAPworkersList.append(worker)

			worker.start()


				#print("Robot conectado en " + str(address[0]) + ":"+str(address[1]))

				#conn.close()

				#print("Robot conectado en " + str(address[0])+"."+str(address[1])+"."+str(address[2])+"."+str(address[3])+":"+str(address[4]))

		except socket.timeout:
			#Comprobamos si la tarea del beacon ha terminado
			if(eventOnBeaconFinish.is_set()):
				keepListening = False

	#Una vez ha terminado el proceso de escucha y despliegue de los hilos,
	#esperamos a que termine cada hilo.
	s.close()

	for worker in OAPworkersList:
		worker.join()

	print("\nPrograma actualizado en todos los robots")
