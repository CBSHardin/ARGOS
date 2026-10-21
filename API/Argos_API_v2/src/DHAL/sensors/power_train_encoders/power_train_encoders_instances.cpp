
#include "../../../config.h"



#ifdef USE_DFROBOT_CHASIS

  #include "implementations/dfrobot_chasis/dfrobot_chasis_power_train_encoders.hpp"
  #include "interface_power_train_encoders.hpp"

  namespace Argos
  {

    DFRobotChasisPowerTrainEncoders chasisPowerTrainEncoders;

    InterfacePowerTrainEncoders<DFRobotChasisPowerTrainEncoders>& PowerTrainEncoders = chasisPowerTrainEncoders;

  }

#endif
