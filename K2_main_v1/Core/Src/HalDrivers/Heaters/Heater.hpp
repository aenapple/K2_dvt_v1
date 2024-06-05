/**********************************************************************************/
/**
 *  @file Heater.hpp
 *  @brief Header file for class THeater
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __Heater_H
#define __Heater_H

/**********************************************************************************/
#include "Os/Os.hpp"


/**********************************************************************************/
enum EHeater
{
	Heater_PtcHeaterLeft,
	Heater_PtcHeaterRight,
	Heater_PadHeaterLeft,
	Heater_PadHeaterRight,
};

enum EHeaterPwm
{
	HeaterPwm_0 = 0,
	HeaterPwm_10 = 1,
	HeaterPwm_20 = 2,
	HeaterPwm_30 = 3,
	HeaterPwm_40 = 4,
	HeaterPwm_50 = 5,
	HeaterPwm_60 = 6,
	HeaterPwm_70 = 7,
	HeaterPwm_80 = 8,
	HeaterPwm_90 = 9,
	HeaterPwm_100 = 10,

};


/**********************************************************************************/
//==================================================================================
class THeater
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void Init(void);
	void TurnOn(EHeaterPwm heaterPwm);
	void TurnOff(void);
	s8 GetTemperature(void);
	void SetTemperature(s8 t);
	EHeaterPwm GetPwm(void);

	virtual void PulseOn(void) = 0;
	virtual void PulseOff(void) = 0;


protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////
	s8 temperature;
	EHeaterPwm pwm;


    ////// constants //////


    ////// functions //////


};
//=== end class THeater ============================================================

/**********************************************************************************/
#endif
