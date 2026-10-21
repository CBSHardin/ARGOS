
#ifndef __ARGOS_API_DFROBOT_CHASIS_POWER_TRAIN_ENCODERS_H__
#define __ARGOS_API_DFROBOT_CHASIS_POWER_TRAIN_ENCODERS_H__

#include <stdlib.h>
#include <stdint.h>

#include "../../interface_power_train_encoders.hpp"
#include <delegate.h>

#include "../../../../../DDL/quadEncoder/quadEncoder.hpp"

namespace Argos
{

//Utilizaremos el patrón CRTP para implementar las clases

class DFRobotChasisPowerTrainEncoders : public InterfacePowerTrainEncoders<DFRobotChasisPowerTrainEncoders>
{

  public:

    DFRobotChasisPowerTrainEncoders() :
    mState(false),
    mEncoderM1(11, 26, 38, 25, 100, 1.0),
    mEncoderM2(12, 27, 38, 25, 100, 1.0, true),
    mEncoderM3(10, 28, 38, 25, 100, 1.0),
    mEncoderM4(13, 29, 38, 25, 100, 1.0, true)
    {};

    void begin();

    bool getStatus();

    float getRPM(uint8_t motor);

    uint16_t getEncoderCount(uint8_t motor);

    const uint8_t getNumberOfEncoders();

    uint8_t getEncoderStepsPerRevolution(uint8_t motor);

    void clearEncoderCount(uint8_t motor);

    void end();

  private:

    bool mState;

    quadEncoder mEncoderM1;
    quadEncoder mEncoderM2;
    quadEncoder mEncoderM3;
    quadEncoder mEncoderM4;

    void encoderM1Callback(float rpm, uint16_t count);
    void encoderM2Callback(float rpm, uint16_t count);
    void encoderM3Callback(float rpm, uint16_t count);
    void encoderM4Callback(float rpm, uint16_t count);

};

}

#endif
