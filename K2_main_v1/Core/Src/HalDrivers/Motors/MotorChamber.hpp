/**********************************************************************************/
/**
 *  @file MotorChamber.hpp
 *  @brief Header file for class TMotorChamber
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __MotorChamber_H
#define __MotorChamber_H

/**********************************************************************************/
#include "Os/Os.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
class TMotorChamber
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void Init(void);
	u8 GetPwm(void);
	bool GetDirection(void);

	virtual void StartForward(void) = 0;
	virtual void StartBackward(void) = 0;
	virtual void Stop(void) = 0;


protected:
    ////// variables //////
	u8 pwm;
	bool direction;


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////


    ////// constants //////


    ////// functions //////


};
//=== end class TMotorChamber ======================================================

/**********************************************************************************/
#endif
