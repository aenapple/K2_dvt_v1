/**********************************************************************************/
/**
 *  @file Gpio.hpp
 *  @brief Header file for Gpio.cpp
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __Gpio_H
#define __Gpio_H

/**********************************************************************************/
#include "Os.hpp"


/**********************************************************************************/
// #define ADC_VREF        2.5   // 2.5V



/**********************************************************************************/
enum EGpioLevel
{
	GpioLevel_Low,
	GpioLevel_High,
};


/**********************************************************************************/
//==================================================================================
class TGpio
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void Init(void);
	void AcPowerOn(void);
	void AcPowerOff(void);
	EGpioLevel ReadTopRemoved(void);
	EGpioLevel ReadLidOpen(void);


	void MainMotorOn(void);
	void MainMotorOff(void);
	void MainMotorBrakingOn(void);
	void MainMotorBrakingOff(void);
	void MainMotorPulseOn(void);
	void MainMotorPulseOff(void);
	void MainMotorResetPulseOn(void);
	void MainMotorResetPulseOff(void);
	void MainMotorStator1On(void);
	void MainMotorStator1Off(void);
	void MainMotorStator2On(void);
	void MainMotorStator2Off(void);
	void ChamberLeftMotorOn(void);
	void ChamberLeftMotorOff(void);
	void ChamberLeftMotorSetForward(void);
	void ChamberLeftMotorSetBackwardward(void);
	void ChamberRightMotorOn(void);
	void ChamberRightMotorOff(void);
	void ChamberRightMotorSetForward(void);
	void ChamberRightMotorSetBackwardward(void);
	void PadHeaterLeftOn(void);
	void PadHeaterLeftOff(void);
	void PadHeaterRightOn(void);
	void PadHeaterRightOff(void);
	void PtcHeaterLeftOn(void);
	void PtcHeaterLeftOff(void);
	void PtcHeaterRightOn(void);
	void PtcHeaterRightOff(void);


protected:
	////// variables //////


	////// constants //////


	////// functions //////

private:
	////// variables //////



	////// constants //////


	////// functions //////




};
//=== end class TGpio ==============================================================

/**********************************************************************************/
#endif
