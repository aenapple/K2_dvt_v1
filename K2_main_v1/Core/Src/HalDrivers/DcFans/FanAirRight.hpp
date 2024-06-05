/**********************************************************************************/
/**
 *  @file FanAirRight.hpp
 *  @brief Header file for class TFanAirRight
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __FanAirRight_H
#define __FanAirRight_H

/**********************************************************************************/
#include "DcFan.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
class TFanAirRight : public TDcFan
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void StartPwm(u8 pwm);
	void StopPwm(void);


protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////


    ////// constants //////


    ////// functions //////




};
//=== end class TFanAirRight =======================================================

/**********************************************************************************/
#endif
