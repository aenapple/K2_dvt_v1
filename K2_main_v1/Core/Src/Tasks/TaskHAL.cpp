/**********************************************************************************/
/**
*  @file
*  Todo: TaskHAL.cpp.
*
*/
/**********************************************************************************/
#include "TaskHAL.hpp"
#include "TaskSYS.hpp"
#include "TaskCHM.hpp"


TTaskHAL TaskHAL;
extern TTaskSYS TaskSYS;
extern TTaskCHM TaskChmLeft;
extern TTaskCHM TaskChmRight;
/**********************************************************************************/
#ifdef __DEBUG_HAL_OUTPUT_ENABLED
	#include "TaskCLI.hpp"
	extern TTaskCLI TaskCLI;
	#define DiagPrintf(...) TaskCLI.DebugPrintf(__VA_ARGS__)
	#define DiagNotice(fmt, ...) TaskCLI.DebugNotice("HAL: " fmt "\r\n", ##__VA_ARGS__)
#else
	#define DiagPrintf(...)
	#define DiagNotice(...)
#endif

/**********************************************************************************/
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;


/**********************************************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hAdc)
{
	TaskHAL.SetEventAdcConvCpltFromISR(hAdc);
}
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef* hAdc)
{
	TaskHAL.SetEventAdcErrorFromISR(hAdc);
}

/* void HAL_GPIO_EXTI_Rising_Callback(u16 gpioPin)
{
	TaskHAL.HandlerGpioInterrupt(gpioPin);
} */

void HAL_GPIO_EXTI_Falling_Callback(u16 gpioPin)
{
	TaskHAL.HandlerGpioInterrupt(gpioPin);
}




/**********************************************************************************/
//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::SetEventTickFromISR(void)
{
	this->Adc.StartAdc1();

	if(this->counterTimeAcMeasurement < TASK_HAL_AC_TIME_MEASUREMENT)
	{
		this->counterTimeAcMeasurement++;
	}
	else
	{
		////// Input each 100 mSec. //////
//		this->CheckLidOpenFromISR();
//		this->CheckTopRemovedFromISR();

		this->counterTimeAcMeasurement = 0;

		if(this->counterAcMain < TASK_HAL_AC_PULSE_NUMBERS)
		{
			this->flagAcMainPresent = false;
		}
		else
		{
			this->flagAcMainPresent = true;
		}
		this->counterAcMain = 0;

		if(this->counterAcMotor < TASK_HAL_AC_PULSE_NUMBERS)
		{
			this->flagAcMotorPresent = false;
		}
		else
		{
			this->flagAcMotorPresent = true;
		}
		this->counterAcMotor = 0;

	}

}
//=== end SetEventTickFromISR ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskHAL::Run(void)
{
	u32 resultBits;
//	EOsResult result;


	this->Delay(20);

	while(true)
	{
		if(this->EventGroup.WaitAndBits(TASK_HAL_CMD_START, 1000) == OsResult_Ok)
		{
			break;
		}
	}

//	this->Gpio.SetLevelTopResetPin(GpioLevel_Low);  // Clear Reset Top CPU
	this->Delay(200);
	this->InterfaceMasterVIP.ReInit();


	while(true)
	{
        if(this->EventGroup.WaitOrBits(
//        			TASK_HAL_EVENT_GET_BME688	|
					TASK_HAL_EVENT_UART_RX_CPLT	|
					TASK_HAL_EVENT_UART_ERROR	|
					TASK_HAL_EVENT_SYS_COMMAND	|
					TASK_HAL_EVENT_T_READY      |
					TASK_HAL_CMD_SELF_TEST,
					&resultBits,
					100
					) == OsResult_Timeout)
        {
            this->GetStateTopCpu();
            this->CheckTopRemoved();
            this->CheckLidOpen();
//            this->GetSensorBme688();

        	continue;
        }
      

        if((resultBits & TASK_HAL_EVENT_T_READY) > 0)
        {
        	this->CalculatingTSensors();
//        	TaskCLI.DebugPrintf("Heater");
        }
        
        if((resultBits & TASK_HAL_EVENT_UART_ERROR) > 0)
        {
            this->InterfaceMasterVIP.ReInit();
            this->Delay(2);
        }

        if((resultBits & TASK_HAL_EVENT_SYS_COMMAND) > 0)
        {
            this->ProcessSysCommand();
        }

        if((resultBits & TASK_HAL_CMD_SELF_TEST) > 0)
        {
        	this->ProcessSelfTest();
        }

        
        
        
        // this->Delay(50);  // mSec
		
      
      //this->DebugPrint("Cycles - %06d\r\n", counter);



	}  // end while(true)


}
//=== end Run ======================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::GetStateTopCpu(void)
{
	EOsResult result;
	u8* pData;


	result = this->SendCommand(IfcVipCommand_GetState, 0);
	// DEBUG
//	return;
	// DEBUG
	if(result != OsResult_Ok)
	{
		this->InterfaceMasterVIP.ReInit();
		this->Delay(10);

		result = this->SendCommand(IfcVipCommand_GetState, 0);
		if(result != OsResult_Ok)
		{
			TaskSYS.SetSysState(SysError_InterfaceVipM);
			return;
		}
	}

	pData = this->InterfaceMasterVIP.GetPointerDataRx();

	TaskSYS.UpdateTopCpuState(pData);

}
//=== end GetStateTopCpu ===========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::GetSensorBme688(void)
{
	EOsResult result;
	u8* pData;


	result = this->SendCommand(IfcVipCommand_GetBme688_Part1, 0);
	if(result != OsResult_Ok)
	{
		this->InterfaceMasterVIP.ReInit();
		this->Delay(10);

		result = this->SendCommand(IfcVipCommand_GetBme688_Part1, 0);
		if(result != OsResult_Ok)
		{
			TaskSYS.SetSysState(SysError_InterfaceVipM);
			return;
		}
	}

	pData = this->InterfaceMasterVIP.GetPointerDataRx();

	TaskSYS.UpdateSensorBme688(pData + IFC_VIP_BME688_TEMPERATURE);

}
//=== end GetSensorBme688 ==========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::ProcessSysCommand(void)
{
	EOsResult result;
	TSysCommand sysCommand;


	while(true)
	{
		result = this->OsQueue.Receive((void*)&sysCommand, 0);
		if(result == OsResult_QueueEmpty)
		{
			return;
		}

		switch(sysCommand.command)
		{
			case SysCommand_ControlHeater:
				this->ControlHeater(sysCommand.parameters);
				break;

			case SysCommand_ControlMotor:
				this->ControlMotor(sysCommand.parameters);
				break;

			case SysCommand_ControlLamp:
				this->ControlLamp(sysCommand.parameters);
				break;

			case SysCommand_SetPosition:
				break;


			default:
				break;
		}

/*		result = this->SendCommand(sysCommand.command, sysCommand.parameters);
		if(result != OsResult_Ok)
		{
			this->InterfaceMasterVIP.ReInit();
			this->Delay(10);

			result = this->SendCommand(sysCommand.command, sysCommand.parameters);
			if(result != OsResult_Ok)
			{
				TaskSYS.SetSysState(SysError_InterfaceVipM);
				return;
			}
		} */

	}

}
//=== end ProcessSysCommand ========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::ProcessSelfTest(void)
{
	// todo: turn off all - ???

	this->AcPowerOff();
	this->Delay(200);
	if(this->flagAcMainPresent)
	{
		TaskSYS.SetSysState(SysError_MainAcNotPresent);  // Error - AC Main is present
		return;
	}

/*	if(this->CheckTopRemoved())
	{
		return;
	}

	if(this->CheckLidOpen())
	{
		return;
	} */


	// todo:
	// check present chamber left
	// check present chamber right
	// check present tank


	this->AcPowerOn();
	this->Delay(200);
	if(!this->flagAcMainPresent)
	{
		this->AcPowerOff();
		TaskSYS.SetSysState(SysError_MainAcNotPresent);  // Error - AC Main is not present
		return;
	}

	this->Delay(100);
	this->Gpio.SetLevelTopResetPin(GpioLevel_Low);  // Clear Reset Top CPU

	// todo:
	// check connection with Top CPU
	// check unlock Top part
	// check level sensors
	// check Bme688 sensor
	// check lamps
	// check dam
	// test Main Fan
	// test Pad heaters
	// test PTC heaters and fans
	// test Main motor
	// test Chamber motors - ???


	TaskSYS.SetEvents(TASK_SYS_EVENT_OK);
}
//=== end ProcessSelfTest ==========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
bool TTaskHAL::CheckTopRemoved()
{
	if((this->Gpio.ReadTopRemoved() == GpioLevel_High) || (this->flagTopUnlocked))
	{
		if(!this->flagSentEventTopRemoved)
		{
			this->flagSentEventTopRemoved = true;
			TaskSYS.SetEvents(TASK_SYS_EVENT_TOP_REMOVED);
		}

		this->flagSentEventTopPresent = false;

		return(true);
	}
	else
	{
		if(!this->flagSentEventTopPresent)
		{
			this->flagSentEventTopPresent = true;
			TaskSYS.SetEvents(TASK_SYS_EVENT_TOP_PRESENT);
		}

		this->flagSentEventTopRemoved = false;

		return(false);
	}

}
//=== end CheckTopRemoved ==========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
bool TTaskHAL::CheckTopRemovedFromISR()
{
	if((this->Gpio.ReadTopRemoved() == GpioLevel_High) || (this->flagTopUnlocked))
	{
		if(!this->flagSentEventTopRemoved)
		{
			this->flagSentEventTopRemoved = true;
			TaskSYS.SetEventsFromISR(TASK_SYS_EVENT_TOP_REMOVED);
		}

		this->flagSentEventTopPresent = false;

		return(true);
	}
	else
	{
		if(!this->flagSentEventTopPresent)
		{
			this->flagSentEventTopPresent = true;
			TaskSYS.SetEventsFromISR(TASK_SYS_EVENT_TOP_PRESENT);
		}

		this->flagSentEventTopRemoved = false;

		return(false);
	}

}
//=== end CheckTopRemovedFromISR ===================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
bool TTaskHAL::CheckLidOpen()
{
	// DEBUG
	if(this->Gpio.ReadPresentTank() == GpioLevel_High)
	{
		this->flagPresentTank = false;
	}
	else
	{
		this->flagPresentTank = true;
	}

	if(this->Gpio.ReadPresentChamberLeft() == GpioLevel_High)
	{
		this->flagPresentChamberLeft = false;
	}
	else
	{
		this->flagPresentChamberLeft = true;
	}

	if(this->Gpio.ReadPresentChamberRight() == GpioLevel_High)
	{
		this->flagPresentChamberRight = false;
	}
	else
	{
		this->flagPresentChamberRight = true;
	}
	// DEBUG


	if(this->Gpio.ReadLidOpen() == GpioLevel_High)
	{
		if(!this->flagSentEventLidOpen)
		{
			this->flagSentEventLidOpen = true;
			TaskSYS.SetEvents(TASK_SYS_EVENT_LID_OPEN);
		}

		this->flagSentEventLidClosed = false;

		return(true);
	}
	else
	{
		if(!this->flagSentEventLidClosed)
		{
			this->flagSentEventLidClosed = true;
			TaskSYS.SetEvents(TASK_SYS_EVENT_LID_CLOSED);
		}

		this->flagSentEventLidOpen = false;

		return(false);
	}

}
//=== end CheckLidOpen =============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
bool TTaskHAL::CheckLidOpenFromISR()
{
	if(this->Gpio.ReadLidOpen() == GpioLevel_High)
	{
		if(!this->flagSentEventLidOpen)
		{
			this->flagSentEventLidOpen = true;
			TaskSYS.SetEventsFromISR(TASK_SYS_EVENT_LID_OPEN);
		}

		this->flagSentEventLidClosed = false;

		return(true);
	}
	else
	{
		if(!this->flagSentEventLidClosed)
		{
			this->flagSentEventLidClosed = true;
			TaskSYS.SetEventsFromISR(TASK_SYS_EVENT_LID_CLOSED);
		}

		this->flagSentEventLidOpen = false;

		return(false);
	}

}
//=== end CheckLidOpenFromISR ======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EOsResult TTaskHAL::ControlMotor(u8* parameters)
{

/*	if(parameters[IFC_VIP_MOTOR_NUMBER_INDEX] == IfcVipMotor_Main)
	{
		return(OsResult_Ok);
	}


	if(parameters[IFC_VIP_MOTOR_NUMBER_INDEX] == IfcVipMotor_Chamber1)
	{
		if(parameters[IFC_VIP_MOTOR_CONTROL_INDEX] < 100)
		{
			this->MotorChamberLeft.Stop();
		}
		else
		{
			if(parameters[IFC_VIP_MOTOR_DIRECTION_INDEX] == DirMotorChamber_Forward)
			{
				this->MotorChamberLeft.StartForward();
			}
			else
			{
				this->MotorChamberLeft.StartBackward();
			}
		}

		return(OsResult_Ok);
	}


	if(parameters[IFC_VIP_MOTOR_NUMBER_INDEX] == IfcVipMotor_Chamber2)
	{
		if(parameters[IFC_VIP_MOTOR_CONTROL_INDEX] < 100)
		{
			this->MotorChamberRight.Stop();
		}
		else
		{
			if(parameters[IFC_VIP_MOTOR_DIRECTION_INDEX] == DirMotorChamber_Forward)
			{
				this->MotorChamberRight.StartForward();
			}
			else
			{
				this->MotorChamberRight.StartBackward();
			}
		}

	} */


	return(OsResult_Ok);
}
//=== end ControlMotor =============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EOsResult TTaskHAL::ControlHeater(u8* parameters)
{
	EHeaterPwm heaterPwm;


	heaterPwm = (EHeaterPwm)parameters[IFC_VIP_HEATER_CONTROL_INDEX];
	switch(parameters[IFC_VIP_HEATER_NUMBER_INDEX])
	{
		case IfcVipHeater_Ptc1:
/*			if(heaterPwm < HeaterPwm_10)
			{
				this->PtcFanLeft.Stop();
			}
			else
			{
				if(heaterPwm < HeaterPwm_60)
				{
					this->PtcFanLeft.Start(PtcFanPwm_50, PtcFanMaxPwm_50);
				}
				else
				{
					if(heaterPwm < HeaterPwm_80)
					{
						this->PtcFanLeft.Start(PtcFanPwm_66, PtcFanMaxPwm_66_100);
					}
					else
					{
						this->PtcFanLeft.Start(PtcFanPwm_100, PtcFanMaxPwm_66_100);
					}
				}
			}
			this->PtcHeaterLeft.TurnOn(heaterPwm); */
			break;

		case IfcVipHeater_Ptc2:
/*			if(heaterPwm < HeaterPwm_10)
			{
				this->PtcFanRight.Stop();
			}
			else
			{
				if(heaterPwm < HeaterPwm_60)
				{
					this->PtcFanRight.Start(PtcFanPwm_50, PtcFanMaxPwm_50);
				}
				else
				{
					if(heaterPwm < HeaterPwm_80)
					{
						this->PtcFanRight.Start(PtcFanPwm_66, PtcFanMaxPwm_66_100);
					}
					else
					{
						this->PtcFanRight.Start(PtcFanPwm_100, PtcFanMaxPwm_66_100);
					}
				}
			}
			this->PtcHeaterRight.TurnOn(heaterPwm); */
			break;

		case IfcVipHeater_Pad1:
//			this->PadHeaterLeft.TurnOn(heaterPwm);
			break;

		default:  // IfcVipHeater_Pad2
//			this->PadHeaterRight.TurnOn(heaterPwm);
			break;

	}


	return(OsResult_Ok);
}
//=== end ControlHeater ============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EOsResult TTaskHAL::ControlLamp(u8* parameters)
{


	return(OsResult_Ok);
}
//=== end ControlLamp ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::AcPowerOn()
{
	this->Gpio.AcPowerOn();
	this->flagAcMainTurnedOn = true;
}
//=== end AcPowerOn ================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::AcPowerOff()
{
	this->Gpio.AcPowerOff();
	this->flagAcMainTurnedOn = false;
}
//=== end AcPowerOff ===============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::StartMainMotorCW()
{
	if(this->flagAcMainPresent)
	{
		HAL_GPIO_WritePin(SW_STATOR2_GPIO_Port, SW_STATOR2_Pin, GPIO_PIN_RESET);
		this->Delay(100);
		this->flagStartMainMotor = true;
	}
}
//=== end StartMainMotorCW =========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::StartMainMotorCCW()
{
	if(this->flagAcMainPresent)
	{
		HAL_GPIO_WritePin(SW_STATOR2_GPIO_Port, SW_STATOR2_Pin, GPIO_PIN_SET);
		this->Delay(100);
		this->flagStartMainMotor = true;
	}
}
//=== end StartMainMotorCCW ========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::StopMainMotor()
{
	this->flagStartMainMotor = false;
}
//=== end StopMainMotor ============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::BrakeOnMainMotor()
{
//	HAL_GPIO_WritePin(BREAK_ON_GPIO_Port, BREAK_ON_Pin, GPIO_PIN_SET);
	HAL_GPIO_TogglePin(SW_STATOR2_GPIO_Port, SW_STATOR2_Pin);
}
//=== end BrakeOnMainMotor =========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::BrakeOffMainMotor()
{
	HAL_GPIO_WritePin(BREAK_ON_GPIO_Port, BREAK_ON_Pin, GPIO_PIN_RESET);
}
//=== end BrakeOffMainMotor =========================================================

#ifndef __DEBUG_CONTROL_HEATER
//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::TurnOnHeater(EHeater heater, EHeaterPwm heaterPwm)
{
	TSysCommand SysCommand;
	EOsResult result;


	switch(heater)
	{
		case Heater_PtcHeaterLeft:
			SysCommand.parameters[IFC_VIP_HEATER_NUMBER_INDEX] = IfcVipHeater_Ptc1;
			break;

		case Heater_PtcHeaterRight:
			SysCommand.parameters[IFC_VIP_HEATER_NUMBER_INDEX] = IfcVipHeater_Ptc2;
			break;

		case Heater_PadHeaterLeft:
			SysCommand.parameters[IFC_VIP_HEATER_NUMBER_INDEX] = IfcVipHeater_Pad1;
			break;

		case Heater_PadHeaterRight:
			SysCommand.parameters[IFC_VIP_HEATER_NUMBER_INDEX] = IfcVipHeater_Pad2;
			break;
	}


	SysCommand.command = SysCommand_ControlHeater;
	SysCommand.parameters[IFC_VIP_HEATER_CONTROL_INDEX] = heaterPwm;

	result = this->SendSysCommand(&SysCommand);
	if(result != OsResult_Ok)
	{
		// todo: set ERROR
	}


}

//=== end TurnOnHeater =============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::TurnOffHeater(EHeater heater)
{
	this->TurnOnHeater(heater, HeaterPwm_0);
}
//=== end TurnOffHeater ============================================================

#else
//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::TurnOnHeater(EHeater heater, EHeaterPwm heaterPwm)
{
	switch(heater)
	{
		case Heater_PtcHeaterLeft:
			if(heaterPwm < HeaterPwm_60)
			{
				this->PtcFanLeft.Start(PtcFanPwm_50, PtcFanMaxPwm_50);
			}
			else
			{
				if(heaterPwm < HeaterPwm_80)
				{
					this->PtcFanLeft.Start(PtcFanPwm_66, PtcFanMaxPwm_66_100);
				}
				else
				{
					this->PtcFanLeft.Start(PtcFanPwm_100, PtcFanMaxPwm_66_100);
				}
			}
			this->PtcHeaterLeft.TurnOn(heaterPwm);
			break;

		case Heater_PtcHeaterRight:
			if(heaterPwm < HeaterPwm_60)
			{
				this->PtcFanRight.Start(PtcFanPwm_50, PtcFanMaxPwm_50);
			}
			else
			{
				if(heaterPwm < HeaterPwm_80)
				{
					this->PtcFanRight.Start(PtcFanPwm_66, PtcFanMaxPwm_66_100);
				}
				else
				{
					this->PtcFanRight.Start(PtcFanPwm_100, PtcFanMaxPwm_66_100);
				}
			}
			this->PtcHeaterRight.TurnOn(heaterPwm);
			break;

		case Heater_PadHeaterLeft:
			this->PadHeaterLeft.TurnOn(heaterPwm);
			break;

		default:  // Heater_PadHeaterRight
			this->PadHeaterRight.TurnOn(heaterPwm);

	}
}
//=== end TurnOnHeater =============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::TurnOffHeater(EHeater heater)
{
	switch(heater)
	{
		case Heater_PtcHeaterLeft:
			this->PtcHeaterLeft.TurnOff();
			this->PtcFanLeft.Stop();
			break;

		case Heater_PtcHeaterRight:
			this->PtcHeaterRight.TurnOff();
			this->PtcFanRight.Stop();
			break;

		case Heater_PadHeaterLeft:
			this->PadHeaterLeft.TurnOff();
			break;

		default:  // Heater_PadHeaterRight
			this->PadHeaterRight.TurnOff();

	}
}
//=== end TurnOffHeater ============================================================
#endif

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
/* void TTaskHAL::TurnOnMotorChamber(EMotorChamber motorChamber, EDirMotorChamber dirMotorChamber, u8 pwm)
{
	TSysCommand SysCommand;
	EOsResult result;


	SysCommand.command = SysCommand_ControlMotor;
	if(motorChamber == MotorChamber_Left)
	{
		SysCommand.parameters[IFC_VIP_MOTOR_NUMBER_INDEX] = IfcVipMotor_Chamber1;
	}
	else
	{
		SysCommand.parameters[IFC_VIP_MOTOR_NUMBER_INDEX] = IfcVipMotor_Chamber2;
	}

	if(dirMotorChamber == DirMotorChamber_Forward)
	{
		SysCommand.parameters[IFC_VIP_MOTOR_DIRECTION_INDEX] = IfcVipMotorDirection_Forward;
	}
	else
	{
		SysCommand.parameters[IFC_VIP_MOTOR_DIRECTION_INDEX] = IfcVipMotorDirection_Backward;
	}

	SysCommand.parameters[IFC_VIP_MOTOR_CONTROL_INDEX] = pwm;


	result = TaskHAL.SendSysCommand(&SysCommand);
	if(result != OsResult_Ok)
	{
		// todo: set ERROR
	}


} */
//=== end TurnOnMotorChamber =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
/* void TTaskHAL::TurnOffMotorChamber(EMotorChamber motorChamber)
{
	this->TurnOnMotorChamber(motorChamber, DirMotorChamber_Forward, 0);
} */
//=== end TurnOffMotorChamber ======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::CalculatingTSensors()
{
	s8 temperature;


	for(u16 i = 0; i < TASK_HAL_NUMBER_T_SENSORS; i++)
	{
		switch(i)
		{
			case 0:
				temperature = this->CalculatingTPtcSensor(IfcVipTemperature_PtcHeater1);
//				if(temperature >= 50) temperature++;
//				if(temperature >= 60) temperature++;
				TaskChmLeft.SetPtcTemperature(temperature);
				break;

			case 1:
				temperature = this->CalculatingTPtcSensor(IfcVipTemperature_PtcHeater2);
//				if(temperature >= 50) temperature++;
//				if(temperature >= 60) temperature++;
				TaskChmRight.SetPtcTemperature(temperature);
				break;

			case 2:
				temperature = this->CalculatingTSensor(IfcVipTemperature_PadHeater1);
				if(temperature >= 40) temperature++;
				if(temperature >= 50) temperature++;
				if(temperature >= 60) temperature++;
				TaskChmLeft.SetPadTemperature(temperature);
				break;

			case 3:
				temperature = this->CalculatingTSensor(IfcVipTemperature_PadHeater2);
				if(temperature >= 40) temperature++;
				if(temperature >= 50) temperature++;
				if(temperature >= 60) temperature++;
				TaskChmRight.SetPadTemperature(temperature);
				break;

			default:
				break;
		}

	}

//	DiagPrintf("PTC temperature - %d", this->tPtc);

}
//=== end CalculatingTSensors ======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
s8 TTaskHAL::CalculatingTSensor(EIfcVipTemperature ifcVipTemperature)
{
	u16 adcTSensor;


	taskENTER_CRITICAL();
	switch(ifcVipTemperature)
	{
/*		case IfcVipTemperature_PtcHeater1:
			adcTSensor = this->adcTPtcLeft;
			break;

		case IfcVipTemperature_PtcHeater2:
			adcTSensor = this->adcTPtcRight;
			break; */

		case IfcVipTemperature_PadHeater1:
			adcTSensor = this->adcTPadLeft;
			break;

		case IfcVipTemperature_PadHeater2:
			adcTSensor = this->adcTPadRight;
			break;

		default:
			return(-5);
	}
	taskEXIT_CRITICAL();


	if(adcTSensor > TASK_HAL_TH_0_DEG_C)
	{
		return(-1);
	}

	if(adcTSensor < TASK_HAL_TH_75_DEG_C)
	{
		return(80);
	}


	if(adcTSensor > TASK_HAL_TH_5_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_5_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_10_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_10_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_15_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_15_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_20_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_20_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_25_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_25_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_30_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_30_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_35_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_35_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_40_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_40_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_45_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_45_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_50_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_50_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_55_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_55_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_60_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_60_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_65_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_65_DEG_C));
	}


	return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_70_DEG_C));
}
//=== end CalculatingTSensor =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
s8 TTaskHAL::CalculatingTSensorDigC(u16 localAdcTHeater, u16 adcLowLevel)
{
	u16 rangeAdc;
	u16 adcHighLevel;
	s8 lowLevelT;


	switch(adcLowLevel)
	{
		case TASK_HAL_TH_5_DEG_C:
			adcHighLevel = TASK_HAL_TH_0_DEG_C;
			lowLevelT = 0;
			break;

		case TASK_HAL_TH_10_DEG_C:
			adcHighLevel = TASK_HAL_TH_5_DEG_C;
			lowLevelT = 5;
			break;

		case TASK_HAL_TH_15_DEG_C:
			adcHighLevel = TASK_HAL_TH_10_DEG_C;
			lowLevelT = 10;
			break;

		case TASK_HAL_TH_20_DEG_C:
			adcHighLevel = TASK_HAL_TH_15_DEG_C;
			lowLevelT = 15;
			break;

		case TASK_HAL_TH_25_DEG_C:
			adcHighLevel = TASK_HAL_TH_20_DEG_C;
			lowLevelT = 20;
			break;

		case TASK_HAL_TH_30_DEG_C:
			adcHighLevel = TASK_HAL_TH_25_DEG_C;
			lowLevelT = 25;
			break;

		case TASK_HAL_TH_35_DEG_C:
			adcHighLevel = TASK_HAL_TH_30_DEG_C;
			lowLevelT = 30;
			break;

		case TASK_HAL_TH_40_DEG_C:
			adcHighLevel = TASK_HAL_TH_35_DEG_C;
			lowLevelT = 35;
			break;

		case TASK_HAL_TH_45_DEG_C:
			adcHighLevel = TASK_HAL_TH_40_DEG_C;
			lowLevelT = 40;
			break;

		case TASK_HAL_TH_50_DEG_C:
			adcHighLevel = TASK_HAL_TH_45_DEG_C;
			lowLevelT = 45;
			break;

		case TASK_HAL_TH_55_DEG_C:
			adcHighLevel = TASK_HAL_TH_50_DEG_C;
			lowLevelT = 50;
			break;

		case TASK_HAL_TH_60_DEG_C:
			adcHighLevel = TASK_HAL_TH_55_DEG_C;
			lowLevelT = 55;
			break;

		case TASK_HAL_TH_65_DEG_C:
			adcHighLevel = TASK_HAL_TH_60_DEG_C;
			lowLevelT = 60;
			break;

		case TASK_HAL_TH_70_DEG_C:
			adcHighLevel = TASK_HAL_TH_65_DEG_C;
			lowLevelT = 65;
			break;


		default:  // TASK_HAL_TS_75_DEG_C
			adcHighLevel = TASK_HAL_TH_70_DEG_C;
			lowLevelT = 70;
			break;
	}

	rangeAdc = adcHighLevel - adcLowLevel;
	rangeAdc /= 5;

	if(localAdcTHeater >= (adcLowLevel + rangeAdc * 4))
	{
		return(lowLevelT);
	}

	if(localAdcTHeater >= (adcLowLevel + rangeAdc * 3))
	{
		return(lowLevelT + 1);
	}

	if(localAdcTHeater >= (adcLowLevel + rangeAdc * 2))
	{
		return(lowLevelT + 2);
	}

	if(localAdcTHeater >= (adcLowLevel + rangeAdc))
	{
		return(lowLevelT + 3);
	}


	return(lowLevelT + 4);
}
//=== end CalculatingTSensorDigC ===================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
s8 TTaskHAL::CalculatingTPtcSensor(EIfcVipTemperature ifcVipTemperature)
{
	u16 adcTSensor;


	taskENTER_CRITICAL();
	if(ifcVipTemperature == IfcVipTemperature_PtcHeater1)
	{
		adcTSensor = this->adcTPtcLeft;
	}
	else
	{
		adcTSensor = this->adcTPtcRight;
	}
	taskEXIT_CRITICAL();


	if(adcTSensor > TASK_HAL_TP_0_DEG_C)
	{
		return(-1);
	}

	if(adcTSensor < TASK_HAL_TP_75_DEG_C)
	{
		return(80);
	}


	if(adcTSensor > TASK_HAL_TP_5_DEG_C)
	{
		return(this->CalculatingTPtcSensorDigC(adcTSensor, TASK_HAL_TP_5_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TP_10_DEG_C)
	{
		return(this->CalculatingTPtcSensorDigC(adcTSensor, TASK_HAL_TP_10_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TP_15_DEG_C)
	{
		return(this->CalculatingTPtcSensorDigC(adcTSensor, TASK_HAL_TP_15_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TP_20_DEG_C)
	{
		return(this->CalculatingTPtcSensorDigC(adcTSensor, TASK_HAL_TP_20_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TP_25_DEG_C)
	{
		return(this->CalculatingTPtcSensorDigC(adcTSensor, TASK_HAL_TP_25_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TP_30_DEG_C)
	{
		return(this->CalculatingTPtcSensorDigC(adcTSensor, TASK_HAL_TP_30_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TP_35_DEG_C)
	{
		return(this->CalculatingTPtcSensorDigC(adcTSensor, TASK_HAL_TP_35_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TP_40_DEG_C)
	{
		return(this->CalculatingTPtcSensorDigC(adcTSensor, TASK_HAL_TP_40_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TP_45_DEG_C)
	{
		return(this->CalculatingTPtcSensorDigC(adcTSensor, TASK_HAL_TP_45_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TP_50_DEG_C)
	{
		return(this->CalculatingTPtcSensorDigC(adcTSensor, TASK_HAL_TP_50_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TP_55_DEG_C)
	{
		return(this->CalculatingTPtcSensorDigC(adcTSensor, TASK_HAL_TP_55_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TP_60_DEG_C)
	{
		return(this->CalculatingTPtcSensorDigC(adcTSensor, TASK_HAL_TP_60_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TP_65_DEG_C)
	{
		return(this->CalculatingTPtcSensorDigC(adcTSensor, TASK_HAL_TP_65_DEG_C));
	}


	return(this->CalculatingTPtcSensorDigC(adcTSensor, TASK_HAL_TP_70_DEG_C));
}
//=== end CalculatingTPtcSensor ====================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
s8 TTaskHAL::CalculatingTPtcSensorDigC(u16 localAdcTHeater, u16 adcLowLevel)
{
	u16 rangeAdc;
	u16 adcHighLevel;
	s8 lowLevelT;


	switch(adcLowLevel)
	{
		case TASK_HAL_TP_5_DEG_C:
			adcHighLevel = TASK_HAL_TP_0_DEG_C;
			lowLevelT = 0;
			break;

		case TASK_HAL_TP_10_DEG_C:
			adcHighLevel = TASK_HAL_TP_5_DEG_C;
			lowLevelT = 5;
			break;

		case TASK_HAL_TP_15_DEG_C:
			adcHighLevel = TASK_HAL_TP_10_DEG_C;
			lowLevelT = 10;
			break;

		case TASK_HAL_TP_20_DEG_C:
			adcHighLevel = TASK_HAL_TP_15_DEG_C;
			lowLevelT = 15;
			break;

		case TASK_HAL_TP_25_DEG_C:
			adcHighLevel = TASK_HAL_TP_20_DEG_C;
			lowLevelT = 20;
			break;

		case TASK_HAL_TP_30_DEG_C:
			adcHighLevel = TASK_HAL_TP_25_DEG_C;
			lowLevelT = 25;
			break;

		case TASK_HAL_TP_35_DEG_C:
			adcHighLevel = TASK_HAL_TP_30_DEG_C;
			lowLevelT = 30;
			break;

		case TASK_HAL_TP_40_DEG_C:
			adcHighLevel = TASK_HAL_TP_35_DEG_C;
			lowLevelT = 35;
			break;

		case TASK_HAL_TP_45_DEG_C:
			adcHighLevel = TASK_HAL_TP_40_DEG_C;
			lowLevelT = 40;
			break;

		case TASK_HAL_TP_50_DEG_C:
			adcHighLevel = TASK_HAL_TP_45_DEG_C;
			lowLevelT = 45;
			break;

		case TASK_HAL_TP_55_DEG_C:
			adcHighLevel = TASK_HAL_TP_50_DEG_C;
			lowLevelT = 50;
			break;

		case TASK_HAL_TP_60_DEG_C:
			adcHighLevel = TASK_HAL_TP_55_DEG_C;
			lowLevelT = 55;
			break;

		case TASK_HAL_TP_65_DEG_C:
			adcHighLevel = TASK_HAL_TP_60_DEG_C;
			lowLevelT = 60;
			break;

		case TASK_HAL_TP_70_DEG_C:
			adcHighLevel = TASK_HAL_TP_65_DEG_C;
			lowLevelT = 65;
			break;


		default:  // TASK_HAL_TP_75_DEG_C
			adcHighLevel = TASK_HAL_TP_70_DEG_C;
			lowLevelT = 70;
			break;
	}

	rangeAdc = adcHighLevel - adcLowLevel;
	rangeAdc /= 5;

	if(localAdcTHeater >= (adcLowLevel + rangeAdc * 4))
	{
		return(lowLevelT);
	}

	if(localAdcTHeater >= (adcLowLevel + rangeAdc * 3))
	{
		return(lowLevelT + 1);
	}

	if(localAdcTHeater >= (adcLowLevel + rangeAdc * 2))
	{
		return(lowLevelT + 2);
	}

	if(localAdcTHeater >= (adcLowLevel + rangeAdc))
	{
		return(lowLevelT + 3);
	}


	return(lowLevelT + 4);
}
//=== end CalculatingTPtcSensorDigC ================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
u16 TTaskHAL::GetAdcValue(EAdcChannel adcChanel)
{
	u16 adcValue;


	switch(adcChanel)
	{
/*		case AdcChannel_Input6: adcValue = this->ArraySensors.SensorOxygen1.GetState(); break;
		case AdcChannel_Input8: adcValue = this->ArraySensors.SensorOxygen2.GetState(); break;
		case AdcChannel_Input9: adcValue = this->ArraySensors.SensorOxygen3.GetState(); break;
		case AdcChannel_THeater: adcValue = this->ArraySensors.SensorMethane1.GetState(); break;
		case AdcChannel_OzoneLamp: adcValue = this->ArraySensors.SensorMethane2.GetState(); break;
		case AdcChannel_UvLamp: adcValue = this->ArraySensors.SensorMethane3.GetState(); break;
		case AdcChannel_TCpu: adcValue = 0; break;
		case AdcChannel_Vref: adcValue = 0; break; */

		default:
			adcValue = 0;
	}


	return(adcValue);
}
//=== end GetAdcValue ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
s8 TTaskHAL::GetTemperature(EIfcVipTemperature ifcVipTemperature)
{
	s8 temperature;

	switch(ifcVipTemperature)
	{
		case IfcVipTemperature_PadHeater1:
			temperature = this->GetTemperaturePadLeft();
			break;

		case IfcVipTemperature_PadHeater2:
			temperature = this->GetTemperaturePadRight();
			break;

		case IfcVipTemperature_Mcu2:
			temperature = this->GetTemperatureCpu2();
			break;

		case IfcVipTemperature_Mcu3:
			temperature = this->GetTemperatureCpu3();
			break;

		case IfcVipTemperature_PtcHeater1:
			temperature = this->GetTemperaturePtcLeft();
			break;

		case IfcVipTemperature_PtcHeater2:
			temperature = this->GetTemperaturePtcRight();
			break;


		default:
			temperature = 0;
	}


	return(temperature);
}
//=== end GetTemperature ===========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
s8 TTaskHAL::GetTemperatureCpu2(void)
{
	return(this->tCpu2);
}
//=== end GetTemperatureCpu2 =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
s8 TTaskHAL::GetTemperatureCpu3(void)
{
	return(this->tCpu3);
}
//=== end GetTemperatureCpu3 =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
s8 TTaskHAL::GetTemperaturePtcLeft(void)
{
	return(TaskChmLeft.GetPtcTemperature());
}
//=== end GetTemperaturePtcLeft ====================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
s8 TTaskHAL::GetTemperaturePtcRight(void)
{
	return(TaskChmRight.GetPtcTemperature());
}
//=== end GetTemperaturePtcRight ===================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
s8 TTaskHAL::GetTemperaturePadLeft(void)
{
	return(TaskChmLeft.GetPadTemperature());
}
//=== end GetTemperaturePadLeft =====================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
s8 TTaskHAL::GetTemperaturePadRight(void)
{
	return(TaskChmRight.GetPadTemperature());
}
//=== end GetTemperaturePadRight ===================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
/* void TTaskHAL::ReadZ1911()
{
	u32 resultBits;
	u8 data[IFC_Z1911_UART_SIZE_PACKET];


	this->InterfaceZ1911.StartRxData();

	memset((void*)data, 0, IFC_Z1911_UART_SIZE_PACKET);
	data[IFC_Z1911_INDEX_START_BYTE] = IFC_Z1911_START_BYTE;
	data[IFC_Z1911_SENSOR_NUM] = IFC_Z1911_SENSOR_NUM;
	data[IFC_Z1911_INDEX_CMD_TX] = IFC_Z1911_CMD_READ;
	this->InterfaceZ1911.StartTxData(data);

	if(this->EventGroup.WaitOrBits(
	        			TASK_HAL_EVENT_UART2_ERROR	|
						TASK_HAL_EVENT_UART2_RX_CPLT,
						&resultBits,
						100
						) == OsResult_Timeout)
	{
		this->SetEvents(TASK_HAL_EVENT_UART2_ERROR);
		return;
	}

	////// if result == Ok. //////
	if((resultBits & TASK_HAL_EVENT_UART2_ERROR) > 0)
	{
		this->SetEvents(TASK_HAL_EVENT_UART2_ERROR);
		return;
	}

	////// if(resultBits & TASK_HAL_EVENT_UART2_RX_CPLT) > 0) //////
} */
//=== end ReadZ1911 ================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::AdcConversionComplete()
{
	u16* ptrBuffer;


	this->Adc.StopDmaAdc1();

	ptrBuffer = this->Adc.GetPtrBufferAdc1();
	for(u16 i = 0; i < ADC1_MAX_NUMBER_CHANNEL; i++)
	{
		this->calculationResultAdc1[i] += ptrBuffer[i];
	}

	this->adcIndexConversion++;

	if(this->adcIndexConversion >= HAL_ADC_NUMBER_CONVERSION)
	{
#ifdef __DEBUG_ADC_CALIBRATION_ENABLED
		u16* adrAdcVrefCalibration;
		u16 adcVrefCalibration;
		u16 tmpU16;
		float kVref;


		adrAdcVrefCalibration = (u16*)ADC_VREF_ADDRESS;
		adcVrefCalibration = (*adrAdcVrefCalibration);
		tmpU16 = this->calculationResultAdc1[ADC_VREF_CHANNEL] / HAL_ADC_NUMBER_CONVERSION;
		kVref = ADC_VREF_CALIBRATION * adcVrefCalibration / tmpU16;
		kVref = kVref / ADC_VREF_WORKING;
#else
		u16 kVref;
		u16 tmpU16;



		kVref = 1;
#endif

		tmpU16 = (u16)(this->calculationResultAdc1[ADC1_IN0_AC_CURRENT] / HAL_ADC_NUMBER_CONVERSION * kVref);

		this->indexAccumulativeT++;
		if(this->indexAccumulativeT >= HAL_T_NUMBER_CONVERSION)
		{
			this->adcTPadLeft = (u16)(this->accumulativeTPadLeft / HAL_T_NUMBER_CONVERSION);
			this->adcTPadRight = (u16)(this->accumulativeTPadRight / HAL_T_NUMBER_CONVERSION);
			this->adcTPtcLeft = (u16)(this->accumulativeTPtcLeft / HAL_T_NUMBER_CONVERSION);
			this->adcTPtcRight = (u16)(this->accumulativeTPtcRight / HAL_T_NUMBER_CONVERSION);

			this->indexAccumulativeT = 0;
			this->accumulativeTPadLeft = 0;
			this->accumulativeTPadRight = 0;
			this->accumulativeTPtcLeft = 0;
			this->accumulativeTPtcRight = 0;
			this->SetEventsFromISR(TASK_HAL_EVENT_T_READY);
		}
		else
		{
			u16 calcT;

			tmpU16 = (u16)(this->calculationResultAdc1[ADC1_IN1_T_PAD_LEFT] / HAL_ADC_NUMBER_CONVERSION * kVref);
			calcT = (this->lastTPadLeft[0] + this->lastTPadLeft[1] +  tmpU16) / 3;
			this->lastTPadLeft[0] = this->lastTPadLeft[1];
			this->lastTPadLeft[1] =  tmpU16;
			this->accumulativeTPadLeft += calcT;

			tmpU16 = (u16)(this->calculationResultAdc1[ADC1_IN2_T_PAD_RIGHT] / HAL_ADC_NUMBER_CONVERSION * kVref);
			calcT = (this->lastTPadRight[0] + this->lastTPadRight[1] +  tmpU16) / 3;
			this->lastTPadRight[0] = this->lastTPadRight[1];
			this->lastTPadRight[1] =  tmpU16;
			this->accumulativeTPadRight += calcT;

			tmpU16 = (u16)(this->calculationResultAdc1[ADC1_IN3_T_PTC_LEFT] / HAL_ADC_NUMBER_CONVERSION * kVref);
			calcT = (this->lastTPtcLeft[0] + this->lastTPtcLeft[1] +  tmpU16) / 3;
			this->lastTPtcLeft[0] = this->lastTPtcLeft[1];
			this->lastTPtcLeft[1] =  tmpU16;
			this->accumulativeTPtcLeft += calcT;

			tmpU16 = (u16)(this->calculationResultAdc1[ADC1_IN4_T_PTC_RIGHT] / HAL_ADC_NUMBER_CONVERSION * kVref);
			calcT = (this->lastTPtcRight[0] + this->lastTPtcRight[1] +  tmpU16) / 3;
			this->lastTPtcRight[0] = this->lastTPtcRight[1];
			this->lastTPtcRight[1] =  tmpU16;
			this->accumulativeTPtcRight += calcT;


		}



		this->adcIndexConversion = 0;
		memset((void*)calculationResultAdc1, 0, ADC1_MAX_NUMBER_CHANNEL * sizeof(u16));

	}



}
//=== end AdcConversionComplete ====================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::SetEventAdcConvCpltFromISR(ADC_HandleTypeDef* hAdc)
{
	this->AdcConversionComplete();
}
//=== end SetEventAdcConvCpltFromISR ===============================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::SetEventAdcErrorFromISR(ADC_HandleTypeDef* hAdc)
{
	// DEBUG
	// this->DebugGpio.ResetPin1();
	// DEBUG
}
//=== end SetEventAdcErrorFromISR ==================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::HandlerGpioInterrupt(u16 gpioPin)
{
	if(gpioPin == AC_MAIN_Pin)
	{
		this->counterAcMain++;

		this->ProcessHeater();

		if(this->acPhase)
		{
			this->ProcessAcPhase();
		}
		this->acPhase = !this->acPhase;

		if(this->flagStartMainMotor)
		{
			HAL_GPIO_WritePin(MOTOR_ON_GPIO_Port, MOTOR_ON_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(MOTOR_ON_GPIO_Port, MOTOR_ON_Pin, GPIO_PIN_RESET);
		}

	}

	if(gpioPin == AC_MOTOR_Pin)
	{
		this->counterAcMotor++;
	}






}
//=== end HandlerGpioInterrupt =====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskHAL::ProcessAcPhase(void)
{
	this->counterPwmHeater++;
	if(this->counterPwmHeater >= TASK_HAL_AC_MAX_PWM_HEATER)
	{
		this->counterPwmHeater = 0;
	}

	TaskChmLeft.PtcFan.IncrementCounterPwm();
	if(TaskChmLeft.PtcFan.GetCounterPwm() >= TaskChmLeft.PtcFan.GetMaxPwm())
	{
		TaskChmLeft.PtcFan.ClearCounterPwm();
	}

	TaskChmRight.PtcFan.IncrementCounterPwm();
	if(TaskChmRight.PtcFan.GetCounterPwm() >= TaskChmRight.PtcFan.GetMaxPwm())
	{
		TaskChmRight.PtcFan.ClearCounterPwm();
	}

}
//=== end ProcessAcPhase ===========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskHAL::ProcessHeater()
{
	////// Pad Heaters //////
	if(TaskChmLeft.PadHeater.GetPwm() > this->counterPwmHeater)
	{
		TaskChmLeft.PadHeater.PulseOn();
	}
	else
	{
		TaskChmLeft.PadHeater.PulseOff();
	}

	if(TaskChmRight.PadHeater.GetPwm() > this->counterPwmHeater)
	{
		TaskChmRight.PadHeater.PulseOn();
	}
	else
	{
		TaskChmRight.PadHeater.PulseOff();
	}

	////// PTC Heaters //////
	if(TaskChmLeft.PtcHeater.GetPwm() > this->counterPwmHeater)
	{
		TaskChmLeft.PtcHeater.PulseOn();
	}
	else
	{
		TaskChmLeft.PtcHeater.PulseOff();
	}

	if(TaskChmRight.PtcHeater.GetPwm() > this->counterPwmHeater)
	{
		TaskChmRight.PtcHeater.PulseOn();
	}
	else
	{
		TaskChmRight.PtcHeater.PulseOff();
	}

	////// PTC Fans //////
	if(TaskChmLeft.PtcFan.GetPwm() > TaskChmLeft.PtcFan.GetCounterPwm())
	{
		TaskChmLeft.PtcFan.PulseOn();
	}
	else
	{
		TaskChmLeft.PtcFan.PulseOff();
	}

	if(TaskChmRight.PtcFan.GetPwm() > TaskChmRight.PtcFan.GetCounterPwm())
	{
		TaskChmRight.PtcFan.PulseOn();
	}
	else
	{
		TaskChmRight.PtcFan.PulseOff();
	}


}
//=== end ProcessHeater ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskHAL::SetEventUart2_TxCpltFromISR(void)
{

}
//=== end SetEventUart5_TxCpltFromISR ==============================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskHAL::SetEventUart2_RxCpltFromISR(void)
{
	if(this->InterfaceMasterVIP.ParsingRxPacket() == OsResult_Ok)
	{
		this->SetEventsFromISR(TASK_HAL_EVENT_UART_RX_CPLT);
	}
	else
	{
		this->SetEventsFromISR(TASK_HAL_EVENT_UART_ERROR);
	}

}
//=== end SetEventUart2_RxCpltFromISR ==============================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskHAL::SetEventUart2_ErrorFromISR(void)
{
	this->SetEventsFromISR(TASK_HAL_EVENT_UART_ERROR);
}
//=== end SetEventUart2_ErrorFromISR ===============================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskHAL::SendSysCommand(TSysCommand* sysCommand)
{
	EOsResult result;


	result= this->OsQueue.Send(sysCommand, 20); // timeout 20 mSec
	if(result != OsResult_Ok)
	{
		return(result);
	}

	this->EventGroup.SetBits(TASK_HAL_EVENT_SYS_COMMAND);

	this->Delay(4);

	return(OsResult_Ok);
}
//=== end SendSysCommand ===========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskHAL::SendCommand(EIfcVipCommand command, u8* pBuffer)
{
	u32 resultBits;
	EOsResult result;


	this->InterfaceMasterVIP.StartRxData();
	this->InterfaceMasterVIP.StartTxData(command, pBuffer);

	result = this->EventGroup.WaitOrBits(
						TASK_HAL_EVENT_UART_RX_CPLT |
						TASK_HAL_EVENT_UART_ERROR   |
						TASK_HAL_EVENT_UART_RX_TIMEOUT,
						&resultBits,
						100);

	if(result == OsResult_Timeout)
	{
		return(OsResult_Timeout);
	}

	if((resultBits & TASK_HAL_EVENT_UART_RX_CPLT) > 0)
	{
		this->SetEvents(TASK_HAL_EVENT_UART_RX_CPLT);
		return(OsResult_Ok);
	}

	if((resultBits & TASK_HAL_EVENT_UART_ERROR) > 0)
	{
		return(OsResult_ErrorI2cReceive);
	}



	return(OsResult_Timeout);
}
//=== end SendCommand ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskHAL::Init(void)
{
	EOsResult result;


	this->AcPowerOff();
	this->Gpio.SetLevelTopResetPin(GpioLevel_High);  // Set Reset Top CPU

	this->flagSentEventTopRemoved = false;
	this->flagSentEventTopPresent = true;
	this->flagSentEventLidOpen = false;
	this->flagSentEventLidClosed = true;
	this->flagTopUnlocked = false;

	this->adcIndexConversion = 0;
	this->accumulativeTPadLeft = 0;
	this->accumulativeTPadRight = 0;
	this->accumulativeTPtcLeft = 0;
	this->accumulativeTPtcRight = 0;
	this->indexAccumulativeT = 0;
	this->lastTPadLeft[0] = this->lastTPadLeft[1] = 2000;
	this->lastTPadRight[0] = this->lastTPadRight[1] = 2000;
	this->lastTPtcLeft[0] = this->lastTPtcLeft[1] = 2000;
	this->lastTPtcRight[0] = this->lastTPtcRight[1] = 2000;
	
	this->InterfaceMasterVIP.Init(IfcUart_2);

	this->counterPwmHeater = 0;


	this->acPhase = false;
	this->flagAcMainPresent = false;
	this->flagAcMotorPresent = false;
	this->flagStartMainMotor = false;


//	this->PtcHeaterLeft.Init(Heater_PtcHeaterLeft);
//	this->PtcHeaterRight.Init(Heater_PtcHeaterRight);
//	this->PadHeaterLeft.Init(Heater_PadHeaterLeft);
//	this->PadHeaterRight.Init(Heater_PadHeaterRight);

//	this->PtcFanLeft.Init(PtcFan_Left);
//	this->PtcFanRight.Init(PtcFan_Right);

//	this->MotorChamberLeft.Init(MotorChamber_Left);
//	this->MotorChamberRight.Init(MotorChamber_Right);

	result = this->Eeprom.Init();
	if(result != OsResult_Ok)
	{
		return(result);
	}


	return(OsResult_Ok);
}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskHAL::CreateOsResources(void)
{
	EOsResult result;

	result= this->OsQueue.Create(TASK_HAL_MAX_NUMBER_COMMANDS, TASK_HAL_SIZE_PARAM_COMMANDS);
	if(result != OsResult_Ok)
	{
		return(result);
	}

	return(OsResult_Ok);
}
//=== end CreateOsResources ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskHAL::CreateTask(void)
{
	((TOsTask*)this)->CreateTask("HAL TASK", OS_TASK_HAL_SIZE_STACK, OS_TASK_HAL_PRIORITY);
}
//=== end CreateTask ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskHAL::CreateTaskStatic(void)
{
	((TOsTask*)this)->CreateTaskStatic("HAL TASK", OS_TASK_HAL_SIZE_STACK, OS_TASK_HAL_PRIORITY, xStackBuffer, &xTaskBuffer);
}
//=== end CreateTaskStatic =========================================================
