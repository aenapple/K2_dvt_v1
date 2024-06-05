/**********************************************************************************/
/**
 *  @file FanAirLeft.hpp
 *  @brief Header file for class TFanAirLeft
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __FanAirLeft_H
#define __FanAirLeft_H

/**********************************************************************************/
#include "DcFan.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
class TFanAirLeft : public TDcFan
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
//=== end class TFanAirLeft ========================================================

/**********************************************************************************/
#endif
