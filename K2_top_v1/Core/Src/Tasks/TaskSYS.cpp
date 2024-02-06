/**********************************************************************************/
/**
*  @file
*  Todo: TaskSYS.cpp.
*
*/
/**********************************************************************************/
#include "TaskUI.hpp"
#include "TaskHAL.hpp"
#include <stdio.h>
#include <stdarg.h>
#include "TaskSYS.hpp"


TTaskSYS TaskSYS;
extern TTaskUI TaskUI;
extern TTaskHAL TaskHAL;
/**********************************************************************************/
#ifdef __DEBUG_SYS_OUTPUT_ENABLED
	#include "../Debug/TaskCLI.hpp"
	extern TTaskCLI TaskCLI;
	#define DiagPrintf(...) TaskCLI.DebugPrintf(__VA_ARGS__)
	#define DiagNotice(fmt, ...) TaskCLI.DebugNotice("SYS: " fmt "\r\n", ##__VA_ARGS__)
#else
	#define DiagPrintf(...)
	#define DiagNotice(...)
#endif

/**********************************************************************************/

extern "C" void CreateApplicationTasks(void);

//==================================================================================
extern "C" void vApplicationTickHook(void)
{
	TaskSYS.SetEventTickFromISR();
}

/**********************************************************************************/
const u8 __attribute__((section (".module_ident"))) TTaskSYS::module_ident[] =
{
	'L','L','0','1','-','A','B','E','-','0','1','.','0','0',' ',' ',  // version - 16 bytes
	'C','R','C','_',                                                  // crc32 - 4 bytes
	'S','I','Z','E',                                                  // firmware size - 4 bytes
	'L','L','1','-','0','0','0','0','0','0','0','0','0','0','0','0',  // serial number - 16 bytes
};

#define SYS_STATE_TABLE(enumState, interfaceVipCode, errorCode) \
		errorCode

const u8 TTaskSYS::errorCode[Number_ESysState] =
{
#include "TableSysState.h"
};

#undef SYS_STATE_TABLE

#define SYS_STATE_TABLE(enumState, interfaceVipCode, errorCode) \
		interfaceVipCode

const u8 TTaskSYS::interfaceVipCode[Number_ESysState] =
{
#include "TableSysState.h"
};

#undef SYS_STATE_TABLE

/**********************************************************************************/
extern UART_HandleTypeDef huart1;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		TaskSYS.SetEventUartRxCpltFromISR();
	}

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		TaskSYS.SetEventUartTxCpltFromISR();
	}

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		TaskSYS.SetEventUartErrorFromISR();
	}

}

/* void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart3)
	{
		TaskSYS.SetEventUartRxHalfCpltFromISR();
	}
} */


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void CreateApplicationTasks()
{
	TaskUI.CreateTaskStatic();
	TaskHAL.CreateTaskStatic();

	TaskSYS.CreateTaskStatic();

}
//=== end CreateApplicationTasks ===================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskSYS::SetEventTickFromISR(void)
{
	this->systemCounter++;
	if(!this->enableTickHook) return;

	TaskHAL.SetEventTickFromISR();



}
//=== end SetEventTickFromISR ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::Run(void)
{
	u32 resultBits;
	EOsResult result;


	result = this->Init();
	if(result != OsResult_Ok)
	{
		this->InitProcessError(result);
	}
	else
	{
		this->SetSysState(SysState_Idle);
	}

	this->ReInitUart();
	this->StartRxData();

	while(true)
	{
		if(this->EventGroup.WaitOrBits(
					TASK_SYS_EVENT_UART_RX_CPLT     |
					TASK_SYS_EVENT_UART_ERROR	    |
					TASK_SYS_EVENT_RESET            |
					TASK_SYS_EVENT_SET_DAM_POSITION |
					TASK_SYS_EVENT_ERROR,
					&resultBits,
					1000  // 1 Sec
					) == OsResult_Timeout)
        {
			continue;
        }

        if((resultBits & TASK_SYS_EVENT_ERROR) > 0)
        {
        	this->ProcessError();
        }

        if((resultBits & TASK_SYS_EVENT_UART_RX_CPLT) > 0)
        {
        	this->ProcessRxData();
        }

 		if((resultBits & TASK_SYS_EVENT_UART_ERROR) > 0)
       	{
       		this->ReInitUart();
       		this->Delay(2);
       		this->StartRxData();

       		continue;
       	}

 		if((resultBits & TASK_SYS_EVENT_RESET) > 0)
 		{
 		   	this->Reset();
 		}

 		if((resultBits & TASK_SYS_EVENT_SET_DAM_POSITION) > 0)
 		{
 		   	this->SetDamPosition();
 		}



	}  // end while(true)


}
//=== end Run ======================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
void TTaskSYS::InitProcessError(EOsResult error)
{
	switch(error)
	{
		case OsResult_ErrorI2c1:
			this->SetSysState(SysError_I2cErrorChannel1);
			break;

		case OsResult_ErrorI2c2:
			this->SetSysState(SysError_I2cErrorChannel2);
			break;

		default:
			this->SetSysState(SysError_ApplicationError);
	}

}
//=== end InitProcessError =========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
void TTaskSYS::ProcessError()
{
	u32 resultBits;


	TaskUI.SetState(TASK_UI_EVENT_ERROR);

	while(true)
	{

		if(this->EventGroup.WaitOrBits(
					TASK_SYS_EVENT_UART_RX_CPLT     |
					TASK_SYS_EVENT_UART_ERROR	    |
					TASK_SYS_EVENT_RESET,
					&resultBits,
					1000  // 1 Sec
					) == OsResult_Timeout)
		{
			continue;
		}

		if((resultBits & TASK_SYS_EVENT_UART_RX_CPLT) > 0)
		{
			this->ProcessRxData();
		}

		if((resultBits & TASK_SYS_EVENT_UART_ERROR) > 0)
		{
			this->ReInitUart();
			this->Delay(2);
			this->StartRxData();

			continue;
		}

		if((resultBits & TASK_SYS_EVENT_RESET) > 0)
		{
		   	this->Reset();
		}

	}

}
//=== end ProcessError =============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
void TTaskSYS::ProcessRxData()
{
//	EOsResult result;
	u8 data[IFC_VIP_UART_SIZE_DATA];
	u8* pData;
	u16 tempU16;
//	u32 tempU32;
	EIfcVipCommand command;
//	u8 byte;
//	u8 packet[IFC_VIP_MEMORY_PACKET_SIZE];


	pData = this->InterfaceSlaveVIP.GetPointerDataRx();
	command = this->InterfaceSlaveVIP.GetCommand();
	data[IFC_VIP_NUMBER_OF_ITEM] = pData[IFC_VIP_NUMBER_OF_ITEM];
	switch(command)
	{
		case IfcVipCommand_GetState:
			TIfcSystemState* IfcSystemState;
			IfcSystemState = TaskHAL.GetPointerIfcSystemState();
			if(this->sysState < SysError_Start)
			{
				IfcSystemState->sysState = this->interfaceVipCode[this->sysState];
				IfcSystemState->error = 0;
			}
			else
			{
				IfcSystemState->sysState = IfcVipState_Error;
				IfcSystemState->error = this->interfaceVipCode[this->sysState];
			}
			IfcSystemState->subSysState = IfcVipSubState_Application;
			memcpy((void*)data, (void*)IfcSystemState, sizeof(TIfcSystemState) - 2);
			break;

		case IfcVipCommand_GetBme688_Part1:
			memcpy((void*)&data[IFC_VIP_NUMBER_OF_ITEM + 1],
					(void*)TaskHAL.GetPointerBme688Sensors((EIfcBme688Sensor)pData[IFC_VIP_NUMBER_OF_ITEM]),
					sizeof(TBme688Sensors) - sizeof(u16));
			break;

//		case IfcVipCommand_GetBme688_Part2:
//			break;

		case IfcVipCommand_SetFanSpeed:
			TaskHAL.StartFan(pData[IFC_VIP_FAN_PWM_INDEX]);
			break;

		case IfcVipCommand_GetFanSpeed:
			data[IFC_VIP_FAN_PWM_INDEX] = TaskHAL.GetFanPwm();
			tempU16 = TaskHAL.GetFanRpm();
			memcpy((void*)&data[IFC_VIP_FAN_RPM_INDEX], (void*)&tempU16, sizeof(u16));
			break;

//		case IfcVipCommand_GetAdcChannel:
//			break;

//		case IfcVipCommand_GetAcParameters:
//			break;

		case IfcVipCommand_GetTemperature:
			break;

//		case IfcVipCommand_SetHeaterParameters:
//			break;

//		case IfcVipCommand_SetMotorParameters:
//			break;

//		case IfcVipCommand_SetLampParameters:
//			break;

//		case IfcVipCommand_GetGasSensor:
//			break;

//		case IfcVipCommand_GetMotorParameters:
//			break;

//		case IfcVipCommand_GetLampParameters:
//			break;

//		case IfcVipCommand_GetHeaterParameters:
//			break;

		case IfcVipCommand_StartTest:
			break;

//		case IfcVipCommand_ContinueProcess:
//			break;

//		case IfcVipCommand_StopProcess:
//			break;

//		case IfcVipCommand_StartProcess:
//			break;

//		case IfcVipCommand_WriteByte:
//			break;

//		case IfcVipCommand_ReadByte:
//			break;

//		case IfcVipCommand_WritePacket:
//			break;

//		case IfcVipCommand_ReadPacket:
//			break;

//		case IfcVipCommand_JumpApplication:
//			break;

		case IfcVipCommand_Reset:
			if(this->sysState == SysState_Idle)
			{
				this->SetEvents(TASK_SYS_EVENT_RESET);
			}
			else
			{
				command = IfcVipCommand_nAck;
			}
			break;

		case IfcVipCommand_ControlLamp:
			TaskHAL.ControlLamp((EHalLamp)pData[IFC_VIP_LAMP_NUMBER_INDEX], (EHalTurn)pData[IFC_VIP_LAMP_CONTROL_INDEX]);
			break;

		case IfcVipCommand_GetStateLamp:
			data[IFC_VIP_LAMP_NUMBER_INDEX] = TaskHAL.GetLampState((EHalLamp)pData[IFC_VIP_LAMP_NUMBER_INDEX]);
			break;

		case IfcVipCommand_SetPosition:
			this->setDamPosition = (EDamPosition)pData[IFC_VIP_POSITION_INDEX];
			if(this->currentDamPosition != this->setDamPosition)
			{
				this->SetEvents(TASK_SYS_EVENT_SET_DAM_POSITION);
			}
			break;

		case IfcVipCommand_GetLevelSensor:
			break;


		default:
			command = IfcVipCommand_nAck;
			break;
	}

	this->InterfaceSlaveVIP.StartRxData(&huart1);
	this->InterfaceSlaveVIP.StartTxData(&huart1, command, data);

	// DEBUG
	//this->Delay(20);
	//this->Delay(2);
	// DEBUG
}
//=== end ProcessRxData ============================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskSYS::SetDamPosition()
{
	EOsResult result;


	if(this->setDamPosition == DamPosition_LeftOpen)
	{
		if(TaskHAL.ReadDamSensorRight() == GpioLevel_Low)
		{
			this->currentDamPosition = DamPosition_LeftOpen;
			return;
		}

		TaskHAL.DamMotorStartForward();
	}
	else
	{
		if(TaskHAL.ReadDamSensorLeft() == GpioLevel_Low)
		{
			this->currentDamPosition = DamPosition_RightOpen;
			return;
		}

		TaskHAL.DamMotorStartBackward();
	}

	this->SetSysState(SysState_Busy);

	result = this->WaitDamPosition(4000); // timeout - 4 Sec
	this->Delay(140);  // todo- ???
	TaskHAL.DamMotorStop();

	if(result != OsResult_Ok)
	{
		if(result == OsResult_Timeout)
		{
			this->SetSysState(SysError_DamMotor);
		}

		return;
	}

	this->SetSysState(SysState_Idle);

}
//=== end SetDamPosition ===========================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
EDamPosition TTaskSYS::GetDamPosition()
{
	return(this->currentDamPosition);
}
//=== end GetDamPosition ===========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
void TTaskSYS::Reset()
{

}
//=== end Reset ====================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
void TTaskSYS::StartRxData(void)
{
//	this->ClearEvents(TASK_SYS_EVENT_UART_RX_TIMEOUT);
	this->InterfaceSlaveVIP.StartRxData(&huart1);
}
//=== end StartRxData ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
/* void TTaskSYS::SetError(ESysState error)
{
	this->error = error;
	this->SetEvents(TASK_SYS_ERROR);

	this->sysState = error;
} */
//=== end SetError =================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
 u8 TTaskSYS::GetErrorCode()
{
	return(this->errorCode[this->sysState]);
}
//=== end GetErrorCode =============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
ESysState TTaskSYS::GetSysState()
{
	return(this->sysState);
}
//=== end GetSysState ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
void TTaskSYS::UpdateSensorBme688(u8* pBufferState)
{

/*	taskENTER_CRITICAL();

	memcpy((void*)&this->bme688Sensor, (void*)pBufferState, sizeof(TBme688Sensor));

	taskEXIT_CRITICAL(); */

}
//=== end UpdateSensorBme688 =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::SetSysState(ESysState sysState)
{
	u32 uiEvent;

	// DEBUG
	//return;
	// DEBUG

	this->sysState = sysState;

	if(this->sysState > SysError_Start)
	{
		this->SetEvents(TASK_SYS_EVENT_ERROR);
		return;
	}


	switch(this->sysState)
	{
		case SysState_Idle:
			uiEvent = TASK_UI_EVENT_IDLE;
			break;

		case SysState_Init:
		case SysState_Busy:
		case SysState_SelsfTest:
			uiEvent = TASK_UI_EVENT_INIT;
			break;


		default: // SysState_NoState:
			uiEvent = TASK_UI_NO_STATE;
	}

	TaskUI.SetState(uiEvent);

}
//=== end SetSysState ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskSYS::Delay_IT(u16 timeDelay)
{
	EOsResult result;
	u32 resultBits;
	u64 startTime;


	startTime = this->systemCounter;

	while(true)
	{
		if((this->systemCounter - startTime) > timeDelay)
		{
			break;  // OsResult_Ok
		}

		if(this->EventGroup.WaitOrBits(
					TASK_SYS_EVENT_UART_RX_CPLT |
					TASK_SYS_EVENT_UART_ERROR	|
					TASK_SYS_EVENT_ERROR,
					&resultBits,
					10  // 10 mSec
					) == OsResult_Timeout)
		{
			continue;
	    }

	    if((resultBits & TASK_SYS_EVENT_ERROR) > 0)
	    {
	        this->SetEvents(TASK_SYS_EVENT_ERROR);
	        return(OsResult_Error);
	    }

	    if((resultBits & TASK_SYS_EVENT_UART_RX_CPLT) > 0)
	    {
//	        this->DelayProcessRxData();
	    }

 		if((resultBits & TASK_SYS_EVENT_UART_ERROR) > 0)
       	{
       		this->ReInitUart();
       		this->Delay(2);
       		this->StartRxData();

       		continue;
       	}




	}  // end while(true)


	return(OsResult_Ok);
}
//=== end Delay_IT =================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskSYS::WaitDamPosition(u16 waitTime)
{
//	EOsResult result;
	u32 resultBits;
	u64 startTime;


	startTime = this->systemCounter;

	while(true)
	{
		if((this->systemCounter - startTime) > waitTime)
		{
			break;  // ERROR - timeout
		}

		if(this->setDamPosition == DamPosition_LeftOpen)
		{
			if(TaskHAL.ReadDamSensorRight() == GpioLevel_Low)
			{
				this->currentDamPosition = DamPosition_LeftOpen;
				return(OsResult_Ok);
			}

		}
		else
		{
			if(TaskHAL.ReadDamSensorLeft() == GpioLevel_Low)
			{
				this->currentDamPosition = DamPosition_RightOpen;
				return(OsResult_Ok);
			}

		}

		if(this->EventGroup.WaitOrBits(
					TASK_SYS_EVENT_UART_RX_CPLT |
					TASK_SYS_EVENT_UART_ERROR	|
					TASK_SYS_EVENT_ERROR,
					&resultBits,
					10  // 10 mSec
					) == OsResult_Timeout)
		{
			continue;
	    }

	    if((resultBits & TASK_SYS_EVENT_ERROR) > 0)
	    {
	        this->SetEvents(TASK_SYS_EVENT_ERROR);
	        return(OsResult_Error);
	    }

	    if((resultBits & TASK_SYS_EVENT_UART_RX_CPLT) > 0)
	    {
	        this->ProcessRxData();
	    }

 		if((resultBits & TASK_SYS_EVENT_UART_ERROR) > 0)
       	{
       		this->ReInitUart();
       		this->Delay(2);
       		this->StartRxData();

       		continue;
       	}




	}  // end while(true)


	return(OsResult_Timeout);
}
//=== end WaitDamPosition ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::SetEventUartTxCpltFromISR(void)
{

}
//=== end SetEventUartTxCpltFromISR ================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::SetEventUartRxCpltFromISR(void)
{
	if(this->InterfaceSlaveVIP.ParsingRxPacket() == OsResult_Ok)
	{
		this->SetEventsFromISR(TASK_SYS_EVENT_UART_RX_CPLT);
	}
	else
	{
		this->SetEventsFromISR(TASK_SYS_EVENT_UART_ERROR);
	}

//	this->flagUartRxTimeout = false;
}
//=== end SetEventUartRxCpltFromISR ================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::ReInitUart()
{
	HAL_UART_DeInit(&huart1);
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 9600;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_TXINVERT_INIT;
	huart1.AdvancedInit.TxPinLevelInvert = UART_ADVFEATURE_TXINV_ENABLE;
	HAL_UART_Init(&huart1);

}
//=== end ReInitUart ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::SetEventUartErrorFromISR(void)
{
	this->SetEventsFromISR(TASK_SYS_EVENT_UART_ERROR);
//	this->flagUartRxTimeout = false;
}
//=== end SetEventUartErrorFromISR =================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
/* void TTaskSYS::SetEventUartRxHalfCpltFromISR(void)
{
	this->uartRxTimeout = 20;  // 20 mSec
	this->flagUartRxTimeout = true;
} */
//=== end SetEventUartRxHalfCpltFromISR ============================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
TTaskSYS::TimeSystem TTaskSYS::GetTimeSystem(void)
{
	TTaskSYS::TimeSystem timeSystem;


	timeSystem.ms = this->systemCounter % 1000;
    u32 sec = this->systemCounter / 1000;
    timeSystem.s = sec % 60;
    timeSystem.m = (sec / 60) % 60;
    timeSystem.h = (sec / 3600) % 24;
    timeSystem.d = (sec / 86400);


    return(timeSystem);
}
//=== end GetTimeSystem ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u64 TTaskSYS::GetSystemCounter(void)
{
	return(this->systemCounter);
}
//=== end GetSystemCounter =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskSYS::Init(void)
{
	EOsResult result;
//	u32 resultBits;


	this->systemCounter = 0;
	this->counterTimeTickProcess = 0;
	this->currentDamPosition = DamPosition_NoPosition;

	TaskUI.Init();
	TaskUI.SetEvents(TASK_UI_CMD_START);
	this->SetSysState(SysState_NoState);

	this->Delay(10);

	this->SetSysState(SysState_Init);
//	this->InterfaceSlaveVIP.ReInit();
// 	this->StartRxData();

	// DEBUG
//	   	while(true)
//	   	{
/*	   		this->setDamPosition = DamPosition_LeftOpen;

	   		this->SetDamPosition();

	   		this->Delay(100);
//	   		this->Delay(100);

//	   		this->setDamPosition = DamPosition_RightOpen;
//	   		this->SetDamPosition();

	   		TaskHAL.StopFan();

	   	while(true)
	   	{
	   		this->Delay(100);
	   		this->Delay(100);
	   	} */


/*	   		TaskHAL.StartFan(100);
	   		this->Delay(5000);

	   		TaskHAL.StartFan(50);
	   		this->Delay(5000);

	   		TaskHAL.StartFan(10);
	   		this->Delay(5000);

	   		TaskHAL.StopFan();
	   		this->Delay(5000); */

//	   	}
	   	// DEBUG

   	result = TaskHAL.Init();
   	if(result != OsResult_Ok)
   	{
   		return(result);
   	}


   	TaskHAL.SetEvents(TASK_HAL_CMD_START);
   	this->enableTickHook = true;





	return(OsResult_Ok);
}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskSYS::CreateOsResources(void)
{
//	EOsResult result;



	return(OsResult_Ok);
}
//=== end CreateOsResources ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::CreateTask(void)
{
	((TOsTask*)this)->CreateTask("SYSTEM TASK", OS_TASK_SYS_SIZE_STACK, OS_TASK_SYS_PRIORITY);
}
//=== end CreateTask ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::CreateTaskStatic(void)
{
	((TOsTask*)this)->CreateTaskStatic("SYSTEM TASK", OS_TASK_SYS_SIZE_STACK, OS_TASK_SYS_PRIORITY, xStackBuffer, &xTaskBuffer);
}
//=== end CreateTaskStatic =========================================================

/**********************************************************************************/
