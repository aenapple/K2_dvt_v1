/**********************************************************************************/
/**
 *  @file Adc.cpp
 *  @brief ...
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "Adc.hpp"


/**********************************************************************************/
extern ADC_HandleTypeDef hadc1;


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TAdc::StartAdc1()
{
	// DEBUG
/*	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP(); */
	// DEBUG

//	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

  	HAL_ADC_Start_DMA(&hadc1, (u32*)this->dmaBufferAdc1, ADC1_MAX_NUMBER_CHANNEL);
}
//=== end StartAdc1 ================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TAdc::StopAdc1()
{
  	HAL_ADC_Stop(&hadc1);
}
//=== end StopAdc1 =================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
void TAdc::StopDmaAdc1()
{
    HAL_ADC_Stop_DMA(&hadc1);
}
//=== end StopDmaAdc1 ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
u16* TAdc::GetPtrBufferAdc1()
{
  	return this->dmaBufferAdc1;
}
//=== end GetPtrBufferAdc1 =========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
float TAdc::GetVoltage(u16 adcValue)
{
    float result;


    result = (float)adcValue * (ADC_VREF / ADC_RESOLUTION);


    return(result);
}
//=== end GetVoltage ===============================================================

/**********************************************************************************/
