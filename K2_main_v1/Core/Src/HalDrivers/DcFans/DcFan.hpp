/**********************************************************************************/
/**
 *  @file DcFan.hpp
 *  @brief Header file for class TDcFan
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __DcFan_H
#define __DcFan_H

/**********************************************************************************/
#include "Os/Os.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
class TDcFan
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void Init(void);
	bool GetFault(void);
	u8 GetPwm(void);
	u16 GetRpm(void);
	void SetRpm(u16 rpm);
	void IncrementCounterRpm(void);
	void ResetCounterRpm(void);
	u16 GetCounterRpm(void);

	void Start(void);
	void Start(u8 pwm);
	void Stop(void);

	virtual void StartPwm(u8 pwm) = 0;
	virtual void StopPwm(void) = 0;



protected:
    ////// variables //////
	bool error;
	u16 counterRpm;
	u8 pwm;
	u16 rpm;
	bool pwmStarted;


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////



    ////// constants //////


    ////// functions //////


};
//=== end class TDcFan =============================================================

/**********************************************************************************/
#endif
