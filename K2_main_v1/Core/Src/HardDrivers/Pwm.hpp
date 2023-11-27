/**********************************************************************************/
/**
 *  @file Pwm.hpp
 *  @brief Header file for Pwm.hpp
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __Pwm_H
#define __Pwm_H

/**********************************************************************************/
#include "Os.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
class TPwm
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	bool GetStarted(void)
	{
		return(this->pwmStarted);
	}
	u8 GetPwm()
	{
		return(this->pwm);
	}



protected:
    ////// variables //////
	bool pwmStarted;
	TIM_HandleTypeDef* pTimer;
	u32 channel;

    ////// constants //////


    ////// functions //////
	void InitPwm(TIM_HandleTypeDef* pTimer, u32 channel);
	void StartPwm(u8 pwm);
	void StopPwm(void);


private:
    ////// variables //////
	u8 pwm;



    ////// constants //////


    ////// functions //////




};
//=== end class TPwm ===============================================================

/**********************************************************************************/
#endif
