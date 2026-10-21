
#ifndef __ARGOS_API_PUBLISHER_H__
#define __ARGOS_API_PUBLISHER_H__

#include <stdlib.h>
#include <stdint.h>

namespace Argos
{

//Esta clase implementa el publicador o agente de notificación del patron
//observer, operando con delegados.
//El objetivo es poder implementar forma sencilla las notificaciones a todos
//aquellos módulos que deben acoplarse los unos a los otros, y deben
//compartir el acceso a la información con varios modulos. Por ejemplo,
//el navSensor puede acceder al InterfacePowerTrainEncoders, y al mismo tiempo,
//el sensor de velocidad puede necesitar saber cual es la velocidad.

template<class T>
class Publisher
{

  public:
/*
    bool registerSubscriber(delegate )

    bool unregisterSubscriber(delegate )

    bool notifyAll()
*/
};


}

#endif
