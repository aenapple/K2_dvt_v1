/**********************************************************************************/
/**
 *  @file FanPtcLeft.cpp
 *  @brief Driver for fan
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include <FanPtcLeft.hpp>


/**********************************************************************************/
extern TIM_HandleTypeDef htim16;


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TFanPtcLeft::StartPwm(u8 pwm)
{
	TIM_OC_InitTypeDef sConfigOC;
	u8 tempPwm;


	if(pwm > 100) tempPwm = 100;
	else tempPwm = pwm;

	if(this->pwmStarted)
	{
		if(this->pwm == tempPwm)
		{
			return;
		}

		this->StopPwm();

	}

	this->pwm = tempPwm;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = (u32)this->pwm;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	HAL_TIM_PWM_ConfigChannel(&htim16, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);

	this->pwmStarted = true;

}
//=== end Start ====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TFanPtcLeft::StopPwm(void)
{
	if(this->pwmStarted)
	{
		HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
		this->pwm = 0;
		this->pwmStarted = false;
	}
}
//=== end Stop =====================================================================

/**********************************************************************************/
