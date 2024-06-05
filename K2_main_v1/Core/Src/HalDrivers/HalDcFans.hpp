/**********************************************************************************/
/**
 *  @file HalDcFans.hpp
 *  @brief Header file for class THalDcFans
 *
 *  @copyright (c) 2024 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __HalDcFans_H
#define __HalDcFans_H

/**********************************************************************************/
#include "Os/Os.hpp"
#include "FanPtcLeft.hpp"
#include "FanPtcRight.hpp"
#include "FanAirLeft.hpp"
#include "FanAirRight.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
class THalDcFans
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	static THalDcFans& GetInstance(void);
	void StartFanPtcLeft(u8 pwm);
	void StopFanPtcLeft(void);
	u8 GetPwmFanPtcLeft(void);
	u16 GetRpmFanPtcLeft(void);
	void StartFanPtcRight(u8 pwm);
	void StopFanPtcRight(void);
	u8 GetPwmFanPtcRight(void);
	u16 GetRpmFanPtcRight(void);
	void StartFanAirLeft(u8 pwm);
	void StopFanAirLeft(void);
	u8 GetPwmFanAirLeft(void);
	u16 GetRpmFanAirLeft(void);
	void StartFanAirRight(u8 pwm);
	void StopFanAirRight(void);
	u8 GetPwmFanAirRight(void);
	u16 GetRpmFanAirRight(void);



protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////
	TFanPtcLeft FanPtcLeft;
	TFanPtcRight FanPtcRight;
	TFanAirLeft FanAirLeft;
	TFanAirRight FanAirRight;


    ////// constants //////


    ////// functions //////
	THalDcFans(void);
	//~THalDcFans(void);


	THalDcFans(THalDcFans const&);
	THalDcFans& operator= (THalDcFans const&);


};
//=== end class THalDcFans =========================================================

/**********************************************************************************/
#endif
