/**********************************************************************************/
/**
 *  @file Fan.hpp
 *  @brief Header file for class TFan
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __Fan_H
#define __Fan_H

/**********************************************************************************/
#include "Os.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
class TFan
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void Init(void);
	bool GetFault(void);
	u8 GetPwm(void);
	void IncrementCounterRpm(void);
	void ResetCounterRpm(void);
	u16 GetCounterRpm(void);

	void Start(void);
	void Start(u8 pwm);
	void Stop(void);



protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////
	bool error;
	u16 counterRpm;
	u8 pwm;
	bool pwmStarted;


    ////// constants //////


    ////// functions //////
	void InitPwm(void);
	void StartPwm(u8 pwm);
	void StopPwm(void);



};
//=== end class TFan ===============================================================

/**********************************************************************************/
#endif
