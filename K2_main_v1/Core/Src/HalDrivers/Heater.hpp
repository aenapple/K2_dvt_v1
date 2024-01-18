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
#include "Os.hpp"


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
	void Init(EHeater heater);
	void TurnOn(EHeaterPwm heaterPwm);
	void TurnOff(void);
	void PulseOn(void);
	void PulseOff(void);
	s8 GetTemperature(void);
	void SetTemperature(s8 t);
	u8 GetPwm(void);


protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////
	s8 temperature;
//	EHeater heater;
	GPIO_TypeDef *GPIOx;
	u16 GPIO_Pin;
	u8 pwm;


    ////// constants //////


    ////// functions //////


};
//=== end class THeater ============================================================

/**********************************************************************************/
#endif
