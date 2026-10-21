
#include "../../../config.h"



#ifdef USE_DFROBOT_CHASIS

  #include "implementations/dfrobot_chasis/dfrobot_chasis_power_train.hpp"
  #include "interface_power_train.hpp"

  namespace Argos
  {

    DFRobotChasisPowerTrain chasisPowerTrain(182, 3000);

    InterfacePowerTrain<DFRobotChasisPowerTrain>& PowerTrain = chasisPowerTrain;

  }

#endif
