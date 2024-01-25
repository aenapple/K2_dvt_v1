/**********************************************************************************/
/**
 *  @file Fan.cpp
 *  @brief Driver for fan
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "Fan.hpp"


/**********************************************************************************/
extern TIM_HandleTypeDef htim17;


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TFan::Init()
{
	this->pwm = 50;
	this->pwmStarted = true;

	this->Stop();
}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
bool TFan::GetFault()
{
	return(this->error);
}
//=== end GetFault =================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u8 TFan::GetPwm()
{
	return(this->pwm);
}
//=== end GetPwm ===================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TFan::Start()
{
	this->StartPwm(99);  // forward with max speed
}
//=== end Start ====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TFan::Start(u8 pwm)
{
	this->StartPwm(pwm);
}
//=== end Start ====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TFan::Stop(void)
{
//	this->StopPwm();
	this->StartPwm(0);
}
//=== end Stop =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TFan::IncrementCounterRpm(void)
{
	this->counterRpm++;
}
//=== end IncrementCounterRpm ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TFan::ResetCounterRpm(void)
{
	this->counterRpm = 0;
}
//=== end IncrementCounterRpm ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u16 TFan::GetCounterRpm(void)
{
	return this->counterRpm;
}
//=== end IncrementCounterRpm ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TFan::StartPwm(u8 pwm)
{
	TIM_OC_InitTypeDef sConfigOC;
	u8 tempPwm;


	if(pwm > 99) tempPwm = 99;
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

	HAL_TIM_PWM_ConfigChannel(&htim17, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);

	this->pwmStarted = true;

}
//=== end Start ====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TFan::StopPwm(void)
{
	if(this->pwmStarted)
	{
		HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1);
		this->pwm = 0;
		this->pwmStarted = false;
	}
}
//=== end Stop =====================================================================

/**********************************************************************************/
