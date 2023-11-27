/**********************************************************************************/
/**
 *  @file Adc.hpp
 *  @brief Header file for Adc.cpp
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __Adc_H
#define __Adc_H

/**********************************************************************************/
#include "Os.hpp"


/**********************************************************************************/
#define ADC_VREF        2.5   // 2.5V
#define ADC_RESOLUTION  4096  // 12-bits
// #define ADC_VREF_INT    1.21  // 3.0V

////// ADC1. //////
#define ADC1_IN0_AC_CURRENT   0
#define ADC1_IN1_T_PAD_LEFT   1
#define ADC1_IN2_T_PAD_RIGHT  2
#define ADC1_IN3_T_PTC_LEFT   3
#define ADC1_IN4_T_PTC_RIGHT  4
#define ADC1_T_CPU_CHANNEL    5
#define ADC1_VREF_CHANNEL     6
#define ADC1_VBAT_CHANNEL     7
#define ADC1_MAX_NUMBER_CHANNEL  8

// #define ADC_VREF_ADDRESS      (u32)0x1FFF75AA  // Raw data acquired at a temperature of 30 degrees VREF = 3.0V
// #define ADC_VREF_CALIBRATION  (float)3.0
// #define ADC_VREF_WORKING      (float)2.5

// #define ADC_TS_CAL1_ADDRESS  0x1FFF75A8  // TS ADC raw data acquired at a temperature of 30 degrees VREF = 3.0V
// #define ADC_TS_CAL2_ADDRESS  0x1FFF75CA  // TS ADC raw data acquired at a temperature of 130 degrees VREF = 3.0V


/**********************************************************************************/
enum EAdcChannel
{
	AdcChannel_AcCurrent = 1,
	AdcChannel_tPadLeft,
	AdcChannel_tPadRight,
	AdcChannel_tPtcLeft,
	AdcChannel_tPtcRight,
	AdcChannel_TCpu,
	AdcChannel_Vref,
	AdcChannel_Vbat,
};


/**********************************************************************************/
//==================================================================================
class TAdc
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void StartAdc1(void);
	void StopAdc1(void);
	void StopDmaAdc1();
	u16* GetPtrBufferAdc1(void);
	float GetVoltage(u16);


protected:
	////// variables //////


	////// constants //////


	////// functions //////

private:
	////// variables //////
	u16 dmaBufferAdc1[ADC1_MAX_NUMBER_CHANNEL];


	////// constants //////


	////// functions //////

};
//=== end class TAdc ===============================================================

/**********************************************************************************/
#endif
