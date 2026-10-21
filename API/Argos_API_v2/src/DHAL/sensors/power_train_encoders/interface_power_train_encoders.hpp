#ifndef __ARGOS_API_INTERFACE_POWER_TRAIN_ENCODERS_H__
#define __ARGOS_API_INTERFACE_POWER_TRAIN_ENCODERS_H__


#include <stdlib.h>
#include <stdint.h>

#include "../../../utils/onUpdateCallbackManager.h"
#include <delegate.h>

namespace Argos
{

//Usaremos el patrón CRTP para hacer que sean independiente de la platafomra,
//es decir, que podamos usar otra plataforma en el futuro si es necesario
//Se asumirá que, el orden de los encoders siempre es el mismo
//Ruedas delanteras número más bajo, ruedas traseras, número más alto
//Ruedas izquierdas, números impares; ruedas derechas, números pares

template<class T>
class InterfacePowerTrainEncoders
{

  public:

    void begin();

    bool getStatus();

    float getRPM(uint8_t motor);

    uint16_t getEncoderCount(uint8_t motor);

    const uint8_t getNumberOfEncoders();

    uint8_t getEncoderStepsPerRevolution(uint8_t motor);

    void clearEncoderCount(uint8_t motor);

    bool registerOnSpeedSampleUpdateCallbackD(delegate<void, uint8_t, float, uint16_t> callbackDelegate);

    void unregisterOnSpeedSampleUpdateCallbackD(delegate<void, uint8_t, float, uint16_t> callbackDelegate);

    void end();

  protected:
    onUpdateCallbackManager<delegate<void, uint8_t, float, uint16_t>, 5> updatesPublisher;

};

#include "interface_power_train_encoders.ipp"

}



#ifdef USE_DFROBOT_CHASIS

#include "implementations/dfrobot_chasis/dfrobot_chasis_power_train_encoders.hpp"

namespace Argos
{

  class DFRobotChasisPowerTrainEncoders;

  extern InterfacePowerTrainEncoders<DFRobotChasisPowerTrainEncoders>& PowerTrainEncoders;

}

#endif


#endif
