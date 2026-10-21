

#include "WireBased/implPHAL.h"

#include "../../PHAL/i2c/interfaceI2cHAL.h"
#include "../../PHAL/i2c/i2cHAL.h"

#include "../../utils/RTOSconfig/tasksPriorities.h"


implPHAL_i2c_Wire_PDL implI2C(I2C_GATEKEEPER_PRIORITY);

i2cHAL i2c(&implI2C);
