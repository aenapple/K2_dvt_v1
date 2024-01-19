/**********************************************************************************/
/**
 *  @file Ch101.hpp
 *  @brief Header file for class TCh101
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __Ch101_H
#define __Ch101_H

/**********************************************************************************/
#include "Os.hpp"
#include "app_config.h"


/**********************************************************************************/
#define CH101_RTC_CAL_PULSE_MS	100



enum ECh101Sensor
{
	Ch101Sensor_Left = 0,
	Ch101Sensor_Right = 1,
	Ch101Sensor_Tank = 2
};

/**********************************************************************************/
//==================================================================================
class TCh101 : public TOs
{
public:
	////// variables //////


	////// constants //////
	static const u8 i2cAddresses[];
	static const u8 i2c_buses[];


	////// functions //////
	void Init(void);
	EOsResult FindSensor(ECh101Sensor ch101Sensor);
	EOsResult ReadRegisterI2c1(u8 devAddress, u8 regAddress, u16 regLength, u8* regValue);
	EOsResult ReadRawI2c1(u8 devAddress, u16 dataLength, u8* readData);
	EOsResult WriteRegisterI2c1(u8 devAddress, u8 regAddress, u16 regLength, u8* regValue);
	EOsResult WriteRawRegisterI2c1(u8 devAddress, u16 regLength, u8* regValue);
	EOsResult ReadRegisterI2c2(u8 devAddress, u8 regAddress, u16 regLength, u8* regValue);
	EOsResult ReadRawRegisterI2c2(u8 devAddress, u16 regLength, u8* regValue);
	EOsResult WriteRegisterI2c2(u8 devAddress, u8 regAddress, u16 regLength, u8* regValue);
	EOsResult WriteRawRegisterI2c2(u8 devAddress, u16 regLength, u8* regValue);



protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////
	ch_group_t chirp_group;
	ch_dev_t chirp_devices[CHIRP_MAX_NUM_SENSORS];
	bool presentLeftSensor;
	bool presentRightSensor;
	bool presentTankSensor;


    ////// constants //////


    ////// functions //////




};
//=== end class TCh101 =============================================================

/**********************************************************************************/
#endif
