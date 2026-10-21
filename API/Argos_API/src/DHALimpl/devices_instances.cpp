
#include "../config.h"

namespace Argos
{


#ifdef USE_DFROBOT_CHASIS

  #include "actuators/power_train/dfrobot_chasis/dfrobot_chasis_power_train.hpp"
  #include "../DHAL/actuators/power_train/power_train.hpp"

  DFRobotChasisPowerTrain chasisPowerTrain;

  class PowerTrain<DFRobotChasisPowerTrain> PowerTrain = chasisPowerTrain;

#endif



}
