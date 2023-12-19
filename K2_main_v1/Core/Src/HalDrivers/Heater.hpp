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
	HeaterPwm_10 = 10,
	HeaterPwm_20 = 20,
	HeaterPwm_30 = 30,
	HeaterPwm_40 = 40,
	HeaterPwm_50 = 50,
	HeaterPwm_60 = 60,
	HeaterPwm_70 = 70,
	HeaterPwm_80 = 80,
	HeaterPwm_90 = 90,
	HeaterPwm_100 = 100,

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
