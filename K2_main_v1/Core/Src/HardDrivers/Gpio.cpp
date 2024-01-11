/**********************************************************************************/
/**
 *  @file Gpio.cpp
 *  @brief ...
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "Gpio.hpp"


/**********************************************************************************/



/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::Init()
{

}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::AcPowerOn()
{
	HAL_GPIO_WritePin(AC_ON_GPIO_Port, AC_ON_Pin, GPIO_PIN_SET);
}
//=== end AcPowerOn ================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::AcPowerOff()
{
	HAL_GPIO_WritePin(AC_ON_GPIO_Port, AC_ON_Pin, GPIO_PIN_RESET);
}
//=== end AcPowerOff ===============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EGpioLevel TGpio::ReadTopRemoved(void)
{
	if(HAL_GPIO_ReadPin(TOP_REMOVED_GPIO_Port, TOP_REMOVED_Pin) == GPIO_PIN_RESET)
	{
		return(GpioLevel_Low);
	}
	else
	{
		return(GpioLevel_High);
	}
}
//=== end ReadTopRemoved ===========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EGpioLevel TGpio::ReadTopPresent(void)
{
	if(HAL_GPIO_ReadPin(TOP_PRESENT_GPIO_Port, TOP_PRESENT_Pin) == GPIO_PIN_RESET)
	{
		return(GpioLevel_Low);
	}
	else
	{
		return(GpioLevel_High);
	}
}
//=== end ReadTopPresent ===========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EGpioLevel TGpio::ReadLidOpen(void)
{
	if(HAL_GPIO_ReadPin(LID_OPEN_GPIO_Port, LID_OPEN_Pin) == GPIO_PIN_RESET)
	{
		return(GpioLevel_Low);
	}
	else
	{
		return(GpioLevel_High);
	}
}
//=== end ReadLidOpen ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EGpioLevel TGpio::ReadPresentTank(void)
{
	if(HAL_GPIO_ReadPin(PRESENT_TANK_GPIO_Port, PRESENT_TANK_Pin) == GPIO_PIN_RESET)
	{
		return(GpioLevel_Low);
	}
	else
	{
		return(GpioLevel_High);
	}
}
//=== end ReadPresentTank ==========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EGpioLevel TGpio::ReadPresentChamberLeft(void)
{
	if(HAL_GPIO_ReadPin(PRESENT_CHM1_GPIO_Port, PRESENT_CHM1_Pin) == GPIO_PIN_RESET)
	{
		return(GpioLevel_Low);
	}
	else
	{
		return(GpioLevel_High);
	}
}
//=== end ReadPresentChamberLeft ===================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EGpioLevel TGpio::ReadPresentChamberRight(void)
{
	if(HAL_GPIO_ReadPin(PRESENT_CHM2_GPIO_Port, PRESENT_CHM2_Pin) == GPIO_PIN_RESET)
	{
		return(GpioLevel_Low);
	}
	else
	{
		return(GpioLevel_High);
	}
}
//=== end ReadPresentChamberRight ==================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::SetLevelTopResetPin(EGpioLevel gpioLevel)
{
	if(gpioLevel == GpioLevel_Low)
	{
		HAL_GPIO_WritePin(TOP_RST_GPIO_Port, TOP_RST_Pin, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(TOP_RST_GPIO_Port, TOP_RST_Pin, GPIO_PIN_SET);
	}
}
//=== end SetLevelTopResetPin ======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::MainMotorOn()
{
	HAL_GPIO_WritePin(MOTOR_ON_GPIO_Port, MOTOR_ON_Pin, GPIO_PIN_SET);
}
//=== end MainMotorOn ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::MainMotorOff()
{
	HAL_GPIO_WritePin(MOTOR_ON_GPIO_Port, MOTOR_ON_Pin, GPIO_PIN_RESET);
}
//=== end MainMotorOff =============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::MainMotorBrakingOn()
{
	HAL_GPIO_WritePin(BREAK_ON_GPIO_Port, BREAK_ON_Pin, GPIO_PIN_SET);
}
//=== end MainMotorBrakingOn =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::MainMotorBrakingOff()
{
	HAL_GPIO_WritePin(BREAK_ON_GPIO_Port, BREAK_ON_Pin, GPIO_PIN_RESET);
}
//=== end MainMotorBrakingOff ======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::MainMotorPulseOn()
{
	HAL_GPIO_WritePin(PULSE_ON_GPIO_Port, PULSE_ON_Pin, GPIO_PIN_SET);
}
//=== end MainMotorPulseOn =========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::MainMotorPulseOff()
{
	HAL_GPIO_WritePin(PULSE_ON_GPIO_Port, PULSE_ON_Pin, GPIO_PIN_RESET);
}
//=== end MainMotorPulseOff ========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::MainMotorResetPulseOn()
{
	HAL_GPIO_WritePin(PULSE_RST_GPIO_Port, PULSE_RST_Pin, GPIO_PIN_SET);
}
//=== end MainMotorResetPulseOn ====================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::MainMotorResetPulseOff()
{
	HAL_GPIO_WritePin(PULSE_RST_GPIO_Port, PULSE_RST_Pin, GPIO_PIN_RESET);
}
//=== end MainMotorResetPulseOff ===================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::MainMotorStator1On()
{
	HAL_GPIO_WritePin(SW_STATOR1_GPIO_Port, SW_STATOR1_Pin, GPIO_PIN_SET);
}
//=== end MainMotorStator1On =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::MainMotorStator1Off()
{
	HAL_GPIO_WritePin(SW_STATOR1_GPIO_Port, SW_STATOR1_Pin, GPIO_PIN_RESET);
}
//=== end MainMotorStator1Off ======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::MainMotorStator2On()
{
	HAL_GPIO_WritePin(SW_STATOR2_GPIO_Port, SW_STATOR2_Pin, GPIO_PIN_SET);
}
//=== end MainMotorStator2On =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::MainMotorStator2Off()
{
	HAL_GPIO_WritePin(SW_STATOR2_GPIO_Port, SW_STATOR2_Pin, GPIO_PIN_RESET);
}
//=== end MainMotorStator2Off ======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::ChamberLeftMotorOn()
{
	HAL_GPIO_WritePin(ON_CHM1_MOT_GPIO_Port, ON_CHM1_MOT_Pin, GPIO_PIN_SET);
}
//=== end ChamberLeftMotorOn =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::ChamberLeftMotorOff()
{
	HAL_GPIO_WritePin(ON_CHM1_MOT_GPIO_Port, ON_CHM1_MOT_Pin, GPIO_PIN_RESET);
}
//=== end ChamberLeftMotorOff ======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::ChamberLeftMotorSetForward()
{
	HAL_GPIO_WritePin(DIR_CHM1_MOT_GPIO_Port, DIR_CHM1_MOT_Pin, GPIO_PIN_SET);
}
//=== end ChamberLeftMotorSetForward ===============================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::ChamberLeftMotorSetBackwardward()
{
	HAL_GPIO_WritePin(DIR_CHM1_MOT_GPIO_Port, DIR_CHM1_MOT_Pin, GPIO_PIN_RESET);
}
//=== end ChamberLeftMotorSetBackwardward ==========================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::ChamberRightMotorOn()
{
	HAL_GPIO_WritePin(ON_CHM2_MOT_GPIO_Port, ON_CHM2_MOT_Pin, GPIO_PIN_SET);
}
//=== end ChamberRightMotorOn ======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::ChamberRightMotorOff()
{
	HAL_GPIO_WritePin(ON_CHM2_MOT_GPIO_Port, ON_CHM2_MOT_Pin, GPIO_PIN_RESET);
}
//=== end ChamberRightMotorOff =====================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::ChamberRightMotorSetForward()
{
	HAL_GPIO_WritePin(DIR_CHM2_MOT_GPIO_Port, DIR_CHM2_MOT_Pin, GPIO_PIN_SET);
}
//=== end ChamberRightMotorSetForward ==============================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::ChamberRightMotorSetBackwardward()
{
	HAL_GPIO_WritePin(DIR_CHM2_MOT_GPIO_Port, DIR_CHM2_MOT_Pin, GPIO_PIN_RESET);
}
//=== end ChamberRightMotorSetBackwardward =========================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::PadHeaterLeftOn()
{
	HAL_GPIO_WritePin(PAD1_ON_GPIO_Port, PAD1_ON_Pin, GPIO_PIN_RESET);
}
//=== end PadHeaterLeftOn ==========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::PadHeaterLeftOff()
{
	HAL_GPIO_WritePin(PAD1_ON_GPIO_Port, PAD1_ON_Pin, GPIO_PIN_SET);
}
//=== end PadHeaterLeftOff =========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::PadHeaterRightOn()
{
	HAL_GPIO_WritePin(PAD2_ON_GPIO_Port, PAD2_ON_Pin, GPIO_PIN_RESET);
}
//=== end PadHeaterRightOn =========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::PadHeaterRightOff()
{
	HAL_GPIO_WritePin(PAD2_ON_GPIO_Port, PAD2_ON_Pin, GPIO_PIN_SET);
}
//=== end PadHeaterRightOff ========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::PtcHeaterLeftOn()
{
	HAL_GPIO_WritePin(PTC1_ON_GPIO_Port, PTC1_ON_Pin, GPIO_PIN_RESET);
}
//=== end PtcHeaterLeftOn ==========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::PtcHeaterLeftOff()
{
	HAL_GPIO_WritePin(PTC1_ON_GPIO_Port, PTC1_ON_Pin, GPIO_PIN_SET);
}
//=== end PtcHeaterLeftOff =========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::PtcHeaterRightOn()
{
	HAL_GPIO_WritePin(PTC2_ON_GPIO_Port, PTC2_ON_Pin, GPIO_PIN_RESET);
}
//=== end PtcHeaterRightOn ==========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TGpio::PtcHeaterRightOff()
{
	HAL_GPIO_WritePin(PTC2_ON_GPIO_Port, PTC2_ON_Pin, GPIO_PIN_SET);
}
//=== end PtcHeaterRightOff ========================================================

/**********************************************************************************/
