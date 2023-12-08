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


/**********************************************************************************/
//==================================================================================
class THeater
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	EOsResult Init(EHeater heater);
	void TurnOn(u8 pwm);
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
	EHeater heater;
	GPIO_TypeDef *GPIOx;
	u16 GPIO_Pin;
	u8 pwm;


    ////// constants //////


    ////// functions //////


};
//=== end class THeater ============================================================

/**********************************************************************************/
#endif
