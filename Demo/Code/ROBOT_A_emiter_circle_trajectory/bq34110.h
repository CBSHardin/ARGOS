///////////////////////////////////////////////
//
//  BQ34110 - Battery Fuel Gauge Library
//
//////////////////////////////////////////////
// ARGOS - SWARM ROBOTICS PLATFORM
//////////////////////////////////////////////
// by Carlos Berbell Sánchez
//////////////////////////////////////////////
/// \file 
/// This file contents the definitions of
/// commands, registers addresses, etc.
/// \author Carlos Berbell Sánchez


/////////////// MAC SUBCOMMANDS ////////////////
#define ALERT1STATUS  0x005E
#define ALERT2STATUS  0x005F
#define RESET         0x0041
#define ACCUM_RESET   0x004B

/////////////// REGISTERS ADDRESSES DEFINITION ////////////////
#define ALERT1_0_CONFIG   0x413E
#define ALERT1_1_CONFIG   0x413F
#define ALERT1_2_CONFIG   0x4140
#define ALERT1_3_CONFIG   0x4141
#define ALERT1_4_CONFIG   0x4142
#define ALERT1_5_CONFIG   0x4143
#define ALERT1_6_CONFIG   0x4143

#define ALERT2_0_CONFIG   0x4145
#define ALERT2_1_CONFIG   0x4146
#define ALERT2_2_CONFIG   0x4147
#define ALERT2_3_CONFIG   0x4148
#define ALERT2_4_CONFIG   0x4149
#define ALERT2_5_CONFIG   0x414A
#define ALERT2_6_CONFIG   0x414B

#define NUMBER_OF_SERIES_CELLS        0x4155
#define VOLTAGE_DIVIDER               0x4010
#define PIN_CONTROL_CONFIG            0x413D
#define FLASH_UPDATE_OK_VOLTAGE       0x4157
#define CEDV_GAUGING_CONFIGURATION    0x424B
#define DESIGN_CAPACITY_mAh           0x41F5
#define LEARNED_FULL_CHARGE_CAPACITY  0x40C0
#define VOLTAGE_0_DOD                 0x4263
#define OVERLOAD_CURRENT              0x4291
#define FIXED_EDV0                    0x425A
#define FIXED_EDV1                    0x425D
#define FIXED_EDV2                    0x4260
#define PIN_CONTROL_CONFIG            0x413D
#define ACCUM_CHARGE_POSSITVE_THRESHOLD 0x416C
#define MFG_STATUS_INIT               0x40D7
#define BATTERY_LOW_SET_THRESHOLD     0x4184
#define BATTERY_LOW_TIME              0x4186
#define BATTERY_LOW_CLEAR_THRESHOLD   0x4187


///////////////// ALERT STATUS REGISTER ID //////////////////
#define ALERT1_0   0
#define ALERT1_1   1
#define ALERT1_2   2
#define ALERT1_3   3
#define ALERT1_4   4
#define ALERT1_5   5
#define ALERT1_6   6
#define ALERT2_0   7
#define ALERT2_1   8
#define ALERT2_2   9
#define ALERT2_3   10
#define ALERT2_4   11
#define ALERT2_5   12
#define ALERT2_6   13

//////////////////// ALERT STATUS FLAGS /////////////////////
#define SLEEP     0x80  //AlertX_0 register flags
#define CHGINH    0x40
#define FD        0x20
#define FC        0x10
#define TCA       0x08
#define TDA       0x04
#define CHG       0x02
#define DSG       0x01

#define SOCLOW    0x40  //AlertX_1 register flags  
#define UTC       0x20
#define UTD       0x10
#define OTC       0x08
#define OTD       0x04
#define BATHIGH   0x02
#define BATLOW    0x01

#define LCTO      0x80  //AlertX_2 register flags
#define LFAULT    0x40
#define LABRT     0x20
#define LCMD      0x10
#define LPER      0x08
#define LRLX      0x04
#define LCHG      0x02
#define LDSG      0x01

#define LDONE     0x80  //AlertX_3 register flags
#define LRES      0x40
#define LRSTOR    0x20
#define LCTLEDGE  0x10
#define LUCD      0x08
#define LDPAM     0x04
#define LDPAT     0x02
#define LDPAI     0x01

#define SRRL      0x80  //AlertX_4 register flags
#define SRCL      0x40
#define LTI       0x10
#define RSDLI     0x08
#define RCELLR    0x04
#define IRRCOMP   0x02
#define IRCOMP    0x01

#define RSDLWARN  0x40  //AlertX_5 register flags
#define RSDWARN   0x20
#define DRDWARN   0x10
#define RSDLALERT 0x04
#define RSDALERT  0x02
#define DRDALERT  0x01

#define ACTHR     0x02  //AlertX_6 register flags
#define BLT       0x01
