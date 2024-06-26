/**********************************************************************************/
/**
 *  @file HeaterPadLeft.cpp
 *  @brief Driver for Pad left heaters
 *
 *  @copyright (c) 2024 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "HeaterPadLeft.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THeaterPadLeft::PulseOn()
{
	HAL_GPIO_WritePin(PAD1_ON_GPIO_Port, PAD1_ON_Pin, GPIO_PIN_RESET);
}
//=== end PulseOn ==================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void THeaterPadLeft::PulseOff()
{
	HAL_GPIO_WritePin(PAD1_ON_GPIO_Port, PAD1_ON_Pin, GPIO_PIN_SET);
}
//=== end PulseOff =================================================================

/**********************************************************************************/
