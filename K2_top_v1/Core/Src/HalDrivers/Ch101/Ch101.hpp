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
#include "soniclib.h"


/**********************************************************************************/
#define CH101_RTC_CAL_PULSE_MS	100



/**********************************************************************************/
//==================================================================================
class TCh101
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void Init(void);
	EOsResult ReadRegisterI2c1(u8 devAddress, u8 regAddress, u16 regLength, u8* regValue);
	EOsResult ReadRawRegisterI2c1(u8 devAddress, u16 regLength, u8* regValue);
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
	ch_group_t 	chirp_group;


    ////// constants //////


    ////// functions //////




};
//=== end class TCh101 =============================================================

/**********************************************************************************/
#endif
