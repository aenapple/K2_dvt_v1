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
#include "Os/Os.hpp"
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


	////// functions //////
	static TCh101& GetInstance(void);
	EOsResult Init(void);
	EOsResult StartMeasurement(ECh101Sensor ch101Sensor);
	EOsResult FindSensor(ECh101Sensor ch101Sensor);
	EOsResult Read(ch_dev_t *dev_ptr, uint8_t *data, uint16_t num_bytes);
	EOsResult ReadMemory(ch_dev_t *dev_ptr, u16 mem_addr, u8 *data, u16 num_bytes);
	EOsResult Write(ch_dev_t *dev_ptr, uint8_t *data, uint16_t num_byte);
	EOsResult WriteMemory(ch_dev_t *dev_ptr, u16 mem_addr, u8 *data, u16 num_bytes);
	EOsResult GetInfo(u8 dev_num, ch_i2c_info_t *info_ptr);
	void SetPinProgram(u8 io_index);
	void ResetPinProgram(u8 io_index);
	void SetPinIntOutput(u8 io_index);
	void SetPinIntInputEXTI(u8 io_index);
	void SetGroupPinIntOutput(void);
	void SetGroupPinIntInputEXTI(void);
	void ClearPinInt(u8 io_index);
	void SetPinInt(u8 io_index);
	void ClearGroupPinInt(void);
	void SetGroupPinInt(void);
	void InitAllPins(void);
	void InterruptEnable(u8 io_index);
	void InterruptGroupEnable(void);
	void InterruptDisable(u8 io_index);
	void InterruptGroupDisable(void);
	bool GetInterruptEnable(ECh101Sensor ch101Sensor);






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
	bool leftInterruptEnable;
	bool rightInterruptEnable;
	bool tankInterruptEnable;


    ////// constants //////
	static const u8 i2cAddresses[];
	static const u8 i2c_buses[];


	////// functions //////


	TCh101(void);
	TCh101(TCh101 const&);
	TCh101& operator= (TCh101 const&);


};
//=== end class TCh101 =============================================================

/**********************************************************************************/
#endif
