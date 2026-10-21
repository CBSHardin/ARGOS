<img width="4624" height="2084" alt="20230123_102339" src="https://github.com/user-attachments/assets/97ab5acf-4325-4eb9-b05d-8b1eeff1477f" />

# ARGOS
For my Bachelor's Thesis I developed a robotic platform called ARGOS, oriented to the research in multi-robot systems and swarm robotics. This repo contains a small showoff of the platform's capabilities. 

## Why do you need this? Motivation of the project
Well, at the time of the project, there weren't robotics platforms designed for multi-robot system and swarm robotics easily accessible. Nonetheless, there were a few that could have been used, but some of this platforms were too tailored to a specific set of problems or too expensive to be considerate. 

Therefore, the ARGOS robotic platform was born. It incorporates the basics elements you'd need for research in swarm robotics and multi-robot systems:
+ Mobile capabilities: four wheeled motors with encoders to allow close loop speed control and precise movement control. 
+ Battery Management Unit: to supervise the state of charge of the battery pack (LiPO chemistry) as well as to protect the user and the robot.
+ 9 axis IMU: accelerometer, gyroscope and magnetometer to implement close loop pose control and waypoint based navigation.
+ Range and bearing sensor: this sensor allows the robots locate the relative distance and angle of others robots to be able to cooperate with them. Also, it could work as reflectivity LIDAR to detect near obstacles.
+ Ultrasonic sensor to detect obstacles locates at the robot's front.
+ Reflectivity floor sensor to detect different zones of operation.
+ A communication system based on ESP-Now.
+ OTA capability: all the robots can be program directly over-the-air.
+ On-board user interface.

It must be noted that all the hardware for this project was developed during the 2020 semiconductor crisis and COVID-19 lockdowns. These circumstances imposed severe restrictions on component availability and, therefore, heavily conditioned the solutions developed. Nevertheless, it was a hell of a lot of fun to fight through it.

<details>

<summary>See the virtual assembly and the PCBs renders</summary>

### Virtual assembly
<img width="1500" height="1200" alt="ensamblado_virtual_perspectiva_difuso" src="https://github.com/user-attachments/assets/7ccf516c-332e-41e2-ae31-d906e1482d76" />



### Electronic boards

  <img  width="480" height="480" alt="power_board" src="https://github.com/user-attachments/assets/227ff322-3065-4dfe-9cf6-6f3406ca768d" /> 
  <p text-align="center">Powerboard</p>

  <img  width="480" height="480" alt="motherboard_virtual_top" src="https://github.com/user-attachments/assets/17da5e61-6dee-4bb6-b457-339b1a78e310" />
  <p text-align="center">Motherboard</p>
  
<img width="480" height="438" alt="R B_sensor_tx" src="https://github.com/user-attachments/assets/f3eb691d-d085-4313-826a-7a647c0d7559" />
<p text-align="center">R&B sensor tx board</p>

<img width="480" height="403" alt="placa_sensor_rx" src="https://github.com/user-attachments/assets/5a463972-1f1d-4792-ba43-d04d5791e8c4" />
<p text-align="center">R&B sensor rx board</p>

<img width="480" height="480" alt="user_interface_board" src="https://github.com/user-attachments/assets/fab2674a-6bf0-40da-b233-f327f88e8dd6" />
<p text-align="center">User interface board</p>

<img width="383" height="450" alt="encoder_virtual_assembly" src="https://github.com/user-attachments/assets/3b35d884-4f61-4c9a-80a8-6138448bd0f7" />
<p text-align="center">Encoder's virtual assembly</p>

</details>

## Check the demo of the Range and Bearing Sensor
The design of the range and bearing sensor required a considerable amount of time, but it was paramount to the project. It was based on the work of Jim Pugh, Alcherio Martinoli, et al [1], as well as the concepts developed by James F. Roberts et al [2].

https://github.com/user-attachments/assets/1695c092-c0f4-4aa4-9918-c9a1ab5e772d

It should be noted that in order to guarantee high precision (measurement repeatability) and an operational range of 4 meters, a thorough theoretical analysis was conducted regarding both the maximum allowable noise level and the gain of the amplification stages on the RX board. If you are working on a similar problem, I hope this provides some inspiration, as I found very little literature available on the subject.

## Not just hardware
The full scope of the project wasn't only "design a robot", the full scope of the project was design a toolkit that could be used to research in swarm robotics. Usually, research or just try out a new idea requires invest effort and time on the boilerplate part of the idea. This'd lead to waste resources to just get to the starting point. As consequence, to extract the maximum value, this platform aiming to be a fast-prototyping platform for mobile robots using alike ecosystems. 

For to accomplish this goal, it was mandatory to hide all the complexity involved in control systems, sensors, pose estimation, communications, etc and the real time constrains imposed by all of them. To allow the researcher to focus on the algorithm instead the platform, an API based in FreeRTOS was designed. This allows to hide all the complexity of the systems control; and at the same time, it satisfy the real time constrains. 

Moreover and following the goal of fast-prototyping, Arduino IDE was selected as the IDE for the toolkit. A new core was designed to incorporate a port of FreeRTOS and to adapt some core's code. In addition, the tools for OTA (over-the-air programming) were integrated seamless in Arduino IDE though the new core. This simplify the tedious process of deploying a new program to simple click in the Arduino IDE's programming button.

## More info
You could find more info of the project at the bachelor's dissertation, available in the digital archive of the Universidad Politécnica of Madrid (UPM): here.
