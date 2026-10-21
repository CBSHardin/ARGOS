
#include "../../../DHAL/sensors/imu/imuHAL.h"
#include "../../../DHAL/sensors/imu/interfaceImuHAL.h"

#include "lsm6dsr/implDHAL.h"

#include "../../../utils/RTOSconfig/tasksPriorities.h"


implDHAL_lsm6dsr implIMU(IMU_TASK_PRIORITY);

imuHAL imu(&implIMU);
