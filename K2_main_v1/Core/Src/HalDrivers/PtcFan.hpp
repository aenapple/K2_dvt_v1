/**********************************************************************************/
/**
 *  @file PtcFan.hpp
 *  @brief Header file for class TPtcFan
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __PtcFan_H
#define __PtcFan_H

/**********************************************************************************/
#include "Os.hpp"


/**********************************************************************************/
enum EPtcFan
{
	PtcFan_Left,
	PtcFan_Right,
};

enum EPtcFanPwm
{
	PtcFanPwm_0 = 0,    // 0% speed
	PtcFanPwm_50 = 1,   // 50% spedd
	PtcFanPwm_66 = 2,   // 66% speed
	PtcFanPwm_100 = 3,  // 100% speed
};

enum EPtcFanMaxPwm
{
	PtcFanMaxPwm_50 = 2,     // for 0% and 50% speed
	PtcFanMaxPwm_66_100 = 3, // for 66% and 100% speed
};


/**********************************************************************************/
//==================================================================================
class TPtcFan
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void Init(EPtcFan ptcFan);
	void Start(EPtcFanPwm ptcFanPwm, EPtcFanMaxPwm ptcFanMaxPwm);
	void Stop(void);
	void PulseOn(void);
	void PulseOff(void);
	u8 GetPwm(void);
	u8 GetMaxPwm(void);
	u8 GetCounterPwm(void);
	void IncrementCounterPwm(void);
	void ClearCounterPwm(void);



protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////
	u8 pwm;
	u8 maxPwm;
	u8 counterPwm;
	GPIO_TypeDef *GPIOx;
	u16 GPIO_Pin;


    ////// constants //////


    ////// functions //////



};
//=== end class TPtcFan ============================================================

/**********************************************************************************/
#endif
