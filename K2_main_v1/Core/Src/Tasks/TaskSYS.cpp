/**********************************************************************************/
/**
*  @file
*  Todo: TaskSYS.cpp.
*
*/
/**********************************************************************************/
#include "TaskUI.hpp"
#include "TaskHAL.hpp"
#include "TaskCHM.hpp"
#include <stdio.h>
#include <stdarg.h>
#include "TaskSYS.hpp"



TTaskSYS TaskSYS;
extern TTaskUI TaskUI;
extern TTaskHAL TaskHAL;
extern TTaskCHM TaskChmLeft;
extern TTaskCHM TaskChmRight;
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
	'L','L','0','1','-','0','0','0','0','0','0','0','0','0','0','1',  // serial number - 16 bytes
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
extern UART_HandleTypeDef huart2;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		TaskSYS.SetEventUartRxCpltFromISR();
	}

	if(huart == &huart2)
	{
		TaskHAL.SetEventUart2_RxCpltFromISR();
	}

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		TaskSYS.SetEventUartTxCpltFromISR();
	}

	if(huart == &huart2)
	{
		TaskHAL.SetEventUart2_TxCpltFromISR();
	}

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		TaskSYS.SetEventUartErrorFromISR();
	}

	if(huart == &huart2)
	{
		TaskHAL.SetEventUart2_ErrorFromISR();
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
extern "C" void CreateApplicationTasks()
{
	TaskUI.CreateTaskStatic();
	TaskHAL.CreateTaskStatic();
	TaskChmLeft.CreateTaskStatic();
	TaskChmRight.CreateTaskStatic();

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

	if(this->counterTimeTickProcess < TASK_SYS_TIME_TICK_PROCESS)
	{
		this->counterTimeTickProcess++;
	}
	else
	{
		this->SetEventsFromISR(TASK_SYS_EVENT_TICK_PROCESS);

		this->counterTimeTickProcess = 0;
	}

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
	// u16 tmpCounter;


	result = this->Init();
	if(result != OsResult_Ok)
	{
		this->InitProcessError(result);
	}


/*	resultBits = this->EventGroup.GetBits();
	if((resultBits & TASK_SYS_EVENT_ERROR) == 0)
	{
		TEeprom* pEeprom;
		TRtc Rtc;

		pEeprom = TaskHAL.GetPointerEeprom();
		result = pEeprom->ReadRtc(&Rtc);
		if(result == OsResult_Ok)
		{
			this->counterBetaTestLog = TASK_SYS_5_MINUTES;

//			if(__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) == RESET)
//			{
//				result = pEeprom->WriteTimestamp(&Rtc);
//			}

			result = pEeprom->WriteTimestamp(&Rtc);
			if(result == OsResult_Ok)
			{
//				TaskChmLeft.SetConfigCompostProcess(Rtc.hours);
//				TaskChmRight.SetConfigCompostProcess(Rtc.hours);
				this->prevHours = Rtc.hours;

				this->SetSysState(SysState_Idle);


//				TaskChmRight.SetPtcTemperatureLevels(50, 55);
//				TaskChmRight.SetPtcTime(6 * 60 * 60, 1 * 60 * 60);
				TaskChmRight.SetEvents(TASK_CHM_EVENT_START_COMPOSTING);

//				TaskChmLeft.SetPtcTemperatureLevels(50, 55);
//				TaskChmLeft.SetPtcTime(6 * 60 * 60, 1 * 60 * 60);
				TaskChmLeft.SetEvents(TASK_CHM_EVENT_START_COMPOSTING);
			}
			else
			{
				this->SetSysState(SysError_I2cErrorChannel1);
			}

		}
		else
		{
			this->SetSysState(SysError_I2cErrorChannel2);
		}

	} */

//	DiagNotice("TaskSYS started!");

	this->SetSysState(SysState_Idle);

	// DEBUG
	TaskHAL.AcPowerOn();
	this->Delay(400);
//	this->SetEvents(TASK_SYS_EVENT_START_TEST);
//	this->SetEvents(TASK_SYS_CMD_SET_RIGHT_OPEN);
	// DEBUG


	while(true)
	{
/*		TEeprom* pEeprom;
		TRtc Rtc;

		pEeprom = TaskHAL.GetPointerEeprom();
		result = pEeprom->ReadRtc(&Rtc);
		if(result != OsResult_Ok)
		{
			this->SetSysState(SysError_I2cErrorChannel2);
		} */


		if(this->EventGroup.WaitOrBits(
					TASK_SYS_EVENT_UART_RX_CPLT |
					TASK_SYS_EVENT_UART_ERROR	|
					TASK_SYS_EVENT_RESET        |
					TASK_SYS_EVENT_TOP_REMOVED  |
					TASK_SYS_EVENT_LID_OPEN     |
					TASK_SYS_EVENT_TICK_PROCESS |
					TASK_SYS_EVENT_START_TEST   |
//					TASK_SYS_EVENT_END_GRINDING |
					TASK_SYS_CMD_SET_LEFT_OPEN  |
					TASK_SYS_CMD_SET_RIGHT_OPEN |
					TASK_SYS_EVENT_ERROR,
					&resultBits,
					1000  // 1 Sec
					) == OsResult_Timeout)
        {
			continue;
        }

#ifndef __DEBUG_EXTERNAL_PC_CONTROL

        if((resultBits & TASK_SYS_EVENT_ERROR) > 0)
        {
        	this->ProcessError();
        }

#endif

        if((resultBits & TASK_SYS_EVENT_UART_RX_CPLT) > 0)
        {
        	this->ProcessRxData();
        }

 		if((resultBits & TASK_SYS_EVENT_UART_ERROR) > 0)
       	{
       		this->ReInitUart();
       		this->Delay(2);
       		this->StartRxData();
       	}

 		if((resultBits & TASK_SYS_EVENT_RESET) > 0)
 		{
 		   	this->Reset();
 		}

#ifndef __DEBUG_EXTERNAL_PC_CONTROL

 		if((resultBits & TASK_SYS_EVENT_TOP_REMOVED) > 0)
 		{
 			this->SetSysState(SysState_TopRemoved);
 			this->SelfTest();
 		}

 		if((resultBits & TASK_SYS_EVENT_LID_OPEN) > 0)
 		{
 		   	this->ProcessLidOpen();
 		}

 		if((resultBits & TASK_SYS_EVENT_TICK_PROCESS) > 0)
 		{
 		   	this->ProcessTick();
 		}
#endif

 		if((resultBits & TASK_SYS_EVENT_START_TEST) > 0)
 		{
 		   	this->ProcessTest();
 		}

 		if((resultBits & TASK_SYS_CMD_SET_LEFT_OPEN) > 0)
 		{
 		   	this->ProcessSetPosition(1);  // left open
 		}

 		if((resultBits & TASK_SYS_CMD_SET_RIGHT_OPEN) > 0)
 		{
 		   	this->ProcessSetPosition(2);  // right open
 		}


/* 		if((resultBits & TASK_SYS_EVENT_END_GRINDING) > 0)
 		{
 		   	this->SetSysState(SysState_Idle);
 		} */


	}  // end while(true)


}
//=== end Run ======================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::SelfTest()
{
	u32 resultBits;
	ESysState inputSysState;


	this->ClearEvents(TASK_SYS_EVENT_TOP_PRESENT | TASK_SYS_EVENT_LID_CLOSED);
	TaskHAL.SetEvents(TASK_HAL_CMD_SELF_TEST);

	inputSysState = this->sysState;
	if(inputSysState != SysState_TopRemoved)
	{
		this->SetSysState(SysState_SelsfTest);
	}
	while(true)
	{

		if(this->EventGroup.WaitOrBits(
					TASK_SYS_EVENT_UART_RX_CPLT |
					TASK_SYS_EVENT_UART_ERROR	|
					TASK_SYS_EVENT_OK           |
					TASK_SYS_EVENT_TOP_REMOVED  |
					TASK_SYS_EVENT_TOP_PRESENT  |
					TASK_SYS_EVENT_LID_OPEN     |
					TASK_SYS_EVENT_LID_CLOSED   |
					TASK_SYS_EVENT_ERROR,
					&resultBits,
					100
					) == OsResult_Timeout)
	    {
			continue;
	    }

		if((resultBits & TASK_SYS_EVENT_OK) > 0)
		{
			if(inputSysState == SysState_TopRemoved)
			{
				this->SetSysState(SysState_Idle);
			}

			break;  // Self Test - OK
		}

		if((resultBits & TASK_SYS_EVENT_TOP_REMOVED) > 0)
		{
		   	this->SetSysState(SysState_TopRemoved);
		}

		if((resultBits & TASK_SYS_EVENT_TOP_PRESENT) > 0)
		{
//			TaskHAL.SetEvents(TASK_HAL_CMD_SELF_TEST);
			this->SetSysState(SysState_SelsfTest);
		}

		if((resultBits & TASK_SYS_EVENT_LID_OPEN) > 0)
		{
		   	this->SetSysState(SysState_LidOpen);
		}

		if((resultBits & TASK_SYS_EVENT_LID_CLOSED) > 0)
		{
//			TaskHAL.SetEvents(TASK_HAL_CMD_SELF_TEST);
			this->SetSysState(SysState_SelsfTest);
		}

	    if((resultBits & TASK_SYS_EVENT_ERROR) > 0)
	    {
	       	this->SetEvents(TASK_SYS_EVENT_ERROR);
	    	return;
	    }

	    if((resultBits & TASK_SYS_EVENT_UART_RX_CPLT) > 0)
	    {
	    	this->ProcessRxData();
	    }

	    if((resultBits & TASK_SYS_EVENT_UART_ERROR) > 0)
	    {
	    	this->ReInitUart();
	    	this->StartRxData();
       	}



	}

}
//=== end SelfTest =================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::ProcessTest()
{
	u32 resultBits;
	ESysState inputSysState;


	inputSysState = this->sysState;
	this->SetSysState(SysState_Busy);
	TaskHAL.SetEvents(TASK_HAL_CMD_SELF_TEST_TOP);

	while(true)
	{
		if(this->EventGroup.WaitOrBits(
					TASK_SYS_EVENT_UART_RX_CPLT |
					TASK_SYS_EVENT_UART_ERROR	|
					TASK_SYS_EVENT_TICK_PROCESS |
					TASK_SYS_EVENT_ERROR        |
					TASK_SYS_EVENT_OK,
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
	    	this->StartRxData();

	    	continue;
	    }

	    if((resultBits & TASK_SYS_EVENT_OK) > 0)
	    {
	    	break;
	    }
	}

	this->SetSysState(inputSysState);

}
//=== end ProcessTest ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::ProcessSetPosition(u8 position)
{
	u32 resultBits;
	ESysState inputSysState;


	inputSysState = this->sysState;
	this->SetSysState(SysState_Busy);

	if(position == 1)
	{
		TaskHAL.SetEvents(TASK_HAL_CMD_SET_LEFT_OPEN);
	}
	else
	{
		TaskHAL.SetEvents(TASK_HAL_CMD_SET_RIGHT_OPEN);
	}

	while(true)
	{
		if(this->EventGroup.WaitOrBits(
					TASK_SYS_EVENT_UART_RX_CPLT |
					TASK_SYS_EVENT_UART_ERROR	|
					TASK_SYS_EVENT_TICK_PROCESS |
					TASK_SYS_EVENT_ERROR        |
					TASK_SYS_EVENT_OK,
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
	    	this->StartRxData();

	    	continue;
	    }

	    if((resultBits & TASK_SYS_EVENT_OK) > 0)
	    {
	    	break;
	    }
	}

	this->SetSysState(inputSysState);

}
//=== end ProcessSetPosition =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::ProcessLidOpen()
{
	EOsResult result;


	this->SetSysState(SysState_LidOpen);
	this->ClearEvents(TASK_SYS_EVENT_LID_CLOSED);
	result = this->WaitEvent(TASK_SYS_EVENT_LID_CLOSED);
	if(result == OsResult_Continue)
	{
		this->SetSysState(SysState_Idle);

		return;
	}

	////// if(result == OsResult_Ok) //////
	this->SetSysState(SysState_Grinding);
	this->ClearEvents(TASK_SYS_EVENT_END_GRINDING);
	TaskHAL.SetEvents(TASK_HAL_CMD_START_GRINDING);
	result = this->WaitEvent(TASK_SYS_EVENT_END_GRINDING);
	if(result != OsResult_Ok)
	{
		TaskHAL.SetEvents(TASK_HAL_CMD_STOP_GRINDING);
	}
	if(result == OsResult_Continue)
	{
		return;
	}

	TaskChmLeft.StartCycleCompostProcess();
	TaskChmRight.StartCycleCompostProcess();

	if(result == OsResult_LidOpen)
	{
		TaskHAL.SetEvents(TASK_HAL_CMD_STOP_GRINDING);
		this->SetSysState(SysState_LidOpen);
		this->WaitEvent(TASK_SYS_EVENT_LID_CLOSED);
		this->SetSysState(SysState_Idle);

		return;
	}

//	TaskChmLeft.SetEvents(TASK_CHM_EVENT_MIXING);
//	TaskChmRight.SetEvents(TASK_CHM_EVENT_MIXING);

	this->SetSysState(SysState_Idle);

}
//=== end ProcessLidOpen ===========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
void TTaskSYS::InitProcessError(EOsResult result)
{
	// DEBUG
	this->SetSysState(SysError_ApplicationError);
	this->ProcessError();
	// DEBUG
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


#ifdef __DEBUG_BETA_TEST
/*	if(this->sysState == SysError_InterfaceVipM)
	{
		this->SetSysState(SysState_TopRemoved);
		this->SelfTest();
		return;
	} */
#endif

	TaskUI.SetState(TASK_UI_EVENT_ERROR);

	while(true)
	{

		if(this->EventGroup.WaitOrBits(
					TASK_SYS_EVENT_UART_RX_CPLT |
					TASK_SYS_EVENT_UART_ERROR	|
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
	    }

	 	if((resultBits & TASK_SYS_EVENT_RESET) > 0)
		{
			this->SetEvents(TASK_SYS_EVENT_RESET);
			return;
		}

		// DEBUG
		// DiagNotice("ERROR !!!");
/*		if(HAL_GPIO_ReadPin(TOP_PRESENT_GPIO_Port, TOP_PRESENT_Pin) == GPIO_PIN_RESET)
		{
			this->Delay(100);
			{
				if(HAL_GPIO_ReadPin(TOP_PRESENT_GPIO_Port, TOP_PRESENT_Pin) == GPIO_PIN_RESET)
				{
					// this->SetSysState(SysState_TopRemoved);
					// this->SelfTest();
					HAL_NVIC_SystemReset();
					return;
				}
			}

			// HAL_NVIC_SystemReset();
		} */
		// DEBUG


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
	EOsResult result;
	u8 data[IFC_VIP_UART_SIZE_DATA];
	u8* pData;
//	u16 tempU16;
//	u32 tempU32;
	EIfcVipCommand command;
	u8 byte;
	u8 packet[IFC_VIP_MEMORY_PACKET_SIZE];
	TRtc Rtc;
	TEeprom* pEeprom;
	TBme688Sensor* bme688Sensor;


//	this->timeoutInterfaceVIP = TaskSys.GetSystemCounter();

	pData = this->InterfaceSlaveVIP.GetPointerDataRx();
	command = this->InterfaceSlaveVIP.GetCommand();
	data[IFC_VIP_MEMORY_TYPE_INDEX] = pData[IFC_VIP_MEMORY_TYPE_INDEX];
	switch(command)
	{
		case IfcVipCommand_GetState:

			if(this->sysState < SysError_Start)
			{
				this->ifcSystemState.sysState = this->interfaceVipCode[this->sysState];
				this->ifcSystemState.error = 0;
			}
			else
			{
				this->ifcSystemState.sysState = IfcVipState_Error;
				this->ifcSystemState.error = this->interfaceVipCode[this->sysState];
			}
			this->ifcSystemState.subSysState = IfcVipSubState_Application;
			memcpy((void*)data, (void*)&this->ifcSystemState, sizeof(TIfcSystemState) - 2);
			break;

		case IfcVipCommand_GetBme688_Part1:
			if((EIfcBme688Sensor)pData[IFC_VIP_NUMBER_OF_ITEM] == IfcBme688Sensor_Fan)
			{
				bme688Sensor = &this->bme688SensorFan;
			}
			else
			{
				if((EIfcBme688Sensor)pData[IFC_VIP_NUMBER_OF_ITEM] == IfcBme688Sensor_Left)
				{
					bme688Sensor = &this->bme688SensorLeft;
				}
				else  // right
				{
					bme688Sensor = &this->bme688SensorRight;
				}
			}
			memcpy((void*)&data[IFC_VIP_NUMBER_OF_ITEM + 1], (void*)bme688Sensor, sizeof(TBme688Sensor));
			break;

//		case IfcVipCommand_GetBme688_Part2:
//			break;

		case IfcVipCommand_ControlFan:
			this->ControlFan(pData);
			break;

		case IfcVipCommand_GetStateFan:
			this->GetStateFan(pData);
			memcpy((void*)data, (void*)pData, sizeof(data));
			break;

//		case IfcVipCommand_GetAdcChannel:
//			break;

		case IfcVipCommand_GetAcParameters:
			break;

		case IfcVipCommand_GetTemperature:
			data[IFC_VIP_T_SENSOR_INDEX] = TaskHAL.GetTemperature((EIfcVipTemperature)pData[IFC_VIP_NUMBER_OF_ITEM]);
			break;

//		case IfcVipCommand_SetHeaterParameters:
//			break;

//		case IfcVipCommand_SetMotorParameters:
//			break;

//		case IfcVipCommand_SetLampParameters:
//			break;

		case IfcVipCommand_GetGasSensor:
			break;

//		case IfcVipCommand_GetMotorParameters:
//			break;

//		case IfcVipCommand_GetLampParameters:
//			break;

//		case IfcVipCommand_GetHeaterParameters:
//			break;

		case IfcVipCommand_StartTest:
			this->ifcVipComponent = (EIfcVipComponent)pData[IFC_VIP_NUMBER_OF_ITEM];
			this->SetEvents(TASK_SYS_EVENT_START_TEST);
			break;

//		case IfcVipCommand_ContinueProcess:
//			break;

//		case IfcVipCommand_StopProcess:
//			break;

//		case IfcVipCommand_StartProcess:
//			break;


		case IfcVipCommand_WriteByte:
			result = this->WriteByteToMemory(pData);
			if(result != OsResult_Ok)
			{
				command = IfcVipCommand_nAck;
			}
			break;

		case IfcVipCommand_ReadByte:
			result = this->ReadByteFromMemory(pData, &byte);
			if(result != OsResult_Ok)
			{
				command = IfcVipCommand_nAck;
			}
			data[IFC_VIP_MEMORY_RD_DATA_INDEX] = byte;
			break;

		case IfcVipCommand_WritePacket:
			memcpy((void*)packet, (void*)&pData[IFC_VIP_MEMORY_WR_DATA_INDEX], sizeof(packet));
			result = this->WritePacketToMemory(pData, packet);
			if(result != OsResult_Ok)
			{
				command = IfcVipCommand_nAck;
			}
			break;

		case IfcVipCommand_ReadPacket:
			result = this->ReadPacketFromMemory(pData, packet);
			if(result != OsResult_Ok)
			{
				command = IfcVipCommand_nAck;
			}
			memcpy((void*)&data[IFC_VIP_MEMORY_RD_DATA_INDEX], (void*)packet, sizeof(packet));
			break;

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
			this->ControlLamp(pData);
			break;

		case IfcVipCommand_GetStateLamp:
			this->GetStateLamp(command, pData);
			break;

		case IfcVipCommand_ControlMotor:
			this->ControlMotor(pData);
			break;

		case IfcVipCommand_GetStateMotor:
			this->GetStateMotor(pData);
			memcpy((void*)data, (void*)pData, sizeof(data));
			break;

		case IfcVipCommand_ControlHeater:
			this->ControlHeater(pData);
			break;

		case IfcVipCommand_GetStateHeater:
			this->GetStateHeater(pData);
			memcpy((void*)data, (void*)pData, sizeof(data));
			break;

		case IfcVipCommand_SetPosition:
/*			TSysCommand sysCommand;
			sysCommand.command = SysCommand_SetPosition;
			sysCommand.parameters[IFC_VIP_POSITION_INDEX] = pData[IFC_VIP_POSITION_INDEX];
			result = TaskHAL.SendSysCommand(&sysCommand); */
			if(pData[IFC_VIP_POSITION_INDEX] == 1)
			{
				this->SetEvents(TASK_SYS_CMD_SET_LEFT_OPEN);
			}
			else
			{
				this->SetEvents(TASK_SYS_CMD_SET_RIGHT_OPEN);
			}
			break;

		case IfcVipCommand_SetRTC:
			memcpy((void*)&Rtc, (void*)&pData[IFC_VIP_DATA_START], sizeof(TRtc));
			pEeprom = TaskHAL.GetPointerEeprom();
			result = pEeprom->WriteRtc(&Rtc);
			if(result != OsResult_Ok)
			{
				this->SetSysState(SysError_I2cErrorChannel2);
			}
			break;

		case IfcVipCommand_GetRTC:
			pEeprom = TaskHAL.GetPointerEeprom();
			result = pEeprom->ReadRtc(&Rtc);
			if(result != OsResult_Ok)
			{
				this->SetSysState(SysError_I2cErrorChannel2);
				memset((void*)&data[IFC_VIP_DATA_START], 0, sizeof(TRtc));
			}
			else
			{
				memcpy((void*)&data[IFC_VIP_DATA_START], (void*)&Rtc, sizeof(TRtc));
			}
			break;


		default:
			command = IfcVipCommand_nAck;
			break;
	}

	this->InterfaceSlaveVIP.StartRxData(&huart1);
	this->InterfaceSlaveVIP.StartTxData(&huart1, command, data);

	// DEBUG
//	this->Delay(20);
//	this->Delay(2);
	// DEBUG
}
//=== end ProcessRxData ============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::WriteByteToMemory(u8* pReceivedPacket)
{
	u32 address;
	u8 byte;
	EIfcVipMemory typeMemory;


	typeMemory = (EIfcVipMemory)pReceivedPacket[IFC_VIP_MEMORY_TYPE_INDEX];
	if(typeMemory == IfcVipMemory_Cpu1)
	{
		return(OsResult_ErrorNAck);
	}

	memcpy((void*)&address, (void*)&pReceivedPacket[IFC_VIP_MEMORY_ADR_INDEX], sizeof(address));
	byte = pReceivedPacket[IFC_VIP_MEMORY_WR_DATA_INDEX];

	if(typeMemory == IfcVipMemory_Cpu2)
	{
		return(this->WriteByteToSlaveCpu(address, byte));
	}

	////// if(typeMemory == IfcVipMemory_Eeprom) //////
	return(this->WriteByteToEeprom(address, byte));
}
//=== end WriteByteToMemory ========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::ReadByteFromMemory(u8* pReceivedPacket, u8* readByte)
{
	u32 address;
	EIfcVipMemory typeMemory;


	typeMemory = (EIfcVipMemory)pReceivedPacket[IFC_VIP_MEMORY_TYPE_INDEX];
	if(typeMemory == IfcVipMemory_Cpu1)
	{
		*readByte = 0;
		return(OsResult_ErrorNAck);
	}

	memcpy((void*)&address, (void*)&pReceivedPacket[IFC_VIP_MEMORY_ADR_INDEX], sizeof(address));

	if(typeMemory == IfcVipMemory_Cpu2)
	{
		return(this->ReadByteFromSlaveCpu(address, readByte));
	}

	////// if(typeMemory == IfcVipMemory_Eeprom) //////
	return(this->ReadByteFromEeprom(address, readByte));
}
//=== end ReadByteFromMemory =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::WritePacketToMemory(u8* pReceivedPacket, u8* writePacket)
{
	u32 address;
	EIfcVipMemory typeMemory;


	typeMemory = (EIfcVipMemory)pReceivedPacket[IFC_VIP_MEMORY_TYPE_INDEX];
	if(typeMemory == IfcVipMemory_Cpu1)
	{
		return(OsResult_ErrorNAck);
	}

	memcpy((void*)&address, (void*)&pReceivedPacket[IFC_VIP_MEMORY_ADR_INDEX], sizeof(address));

	if(typeMemory == IfcVipMemory_Cpu2)
	{
		return(this->WritePacketToSlaveCpu(address, writePacket));
	}

	////// if(typeMemory == IfcVipMemory_Eeprom) //////
	return(this->WritePacketToEeprom(address, writePacket));
}
//=== end WritePacketToMemory ======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::ReadPacketFromMemory(u8* pReceivedPacket, u8* readPacket)
{
	u32 address;
	EIfcVipMemory typeMemory;


	typeMemory = (EIfcVipMemory)pReceivedPacket[IFC_VIP_MEMORY_TYPE_INDEX];
	if(typeMemory == IfcVipMemory_Cpu1)
	{
		memset((void*)readPacket, 0, IFC_VIP_MEMORY_PACKET_SIZE);
		return(OsResult_ErrorNAck);
	}

	memcpy((void*)&address, (void*)&pReceivedPacket[IFC_VIP_MEMORY_ADR_INDEX], sizeof(address));

	if(typeMemory == IfcVipMemory_Cpu2)
	{
		return(this->ReadPacketFromSlaveCpu(address, readPacket));
	}

	////// if(typeMemory == IfcVipMemory_Eeprom) //////
	return(this->ReadPacketFromEeprom(address, readPacket));
}
//=== end ReadPacketFromMemory =====================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::WriteByteToSlaveCpu(u32 address, u8 data)
{
	return(OsResult_Ok);
}
//=== end WriteByteToSlaveCpu ======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::ReadByteFromSlaveCpu(u32 address, u8* data)
{
	return(OsResult_Ok);
}
//=== end ReadByteFromSlaveCpu =====================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::WritePacketToSlaveCpu(u32 address, u8* data)
{
	return(OsResult_Ok);
}
//=== end WritePacketToSlaveCpu ====================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::ReadPacketFromSlaveCpu(u32 address, u8* data)
{
	return(OsResult_Ok);
}
//=== end ReadPacketFromSlaveCpu ===================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::WriteByteToEeprom(u32 address, u8 data)
{
	return(OsResult_Ok);
}
//=== end WriteByteToEeprom ========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::ReadByteFromEeprom(u32 address, u8* data)
{
	return(OsResult_Ok);
}
//=== end ReadByteFromEeprom =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::WritePacketToEeprom(u32 address, u8* data)
{
	TEeprom* pEeprom;


	pEeprom = TaskHAL.GetPointerEeprom();


	return(pEeprom->WritePacket(address, data));
}
//=== end WritePacketToEeprom ======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::ReadPacketFromEeprom(u32 address, u8* data)
{
	TEeprom* pEeprom;


	pEeprom = TaskHAL.GetPointerEeprom();


	return(pEeprom->ReadPacket(address, data));
}
//=== end ReadPacketFromEeprom =====================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::ControlLamp(u8* pData)
{
	EOsResult result;
	TSysCommand sysCommand;


	sysCommand.command = SysCommand_ControlLamp;
	sysCommand.parameters[IFC_VIP_LAMP_CONTROL_INDEX] = pData[IFC_VIP_FAN_PWM_INDEX];
	result = TaskHAL.SendSysCommand(&sysCommand);


	return(result);
}
//=== end ControlLamp ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::GetStateLamp(EIfcVipCommand ifcVipCommand, u8* pData)
{
	return(OsResult_Ok);
}
//=== end GetStateLamp =============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::ControlFan(u8* pData)
{
	THalDcFans* HalDcFans;
	EOsResult result;


	static THalDcFans& halF = THalDcFans::GetInstance();
	HalDcFans= &halF;

	result = OsResult_Ok;
	switch((EIfcVipFan)pData[IFC_VIP_NUMBER_OF_ITEM])
	{
		case IfcVipFan_Main:
			TSysCommand sysCommand;
			sysCommand.command = SysCommand_ControlFan;
			sysCommand.parameters[IFC_VIP_FAN_PWM_INDEX] = pData[IFC_VIP_FAN_PWM_INDEX];
			result = TaskHAL.SendSysCommand(&sysCommand);
			break;

		case IfcVipFan_Ptc1:
			HalDcFans->StartFanPtcLeft(pData[IFC_VIP_FAN_PWM_INDEX]);
			break;

		case IfcVipFan_Ptc2:
			HalDcFans->StartFanPtcRight(pData[IFC_VIP_FAN_PWM_INDEX]);
			break;

		case IfcVipFan_Air1:
			HalDcFans->StartFanAirLeft(pData[IFC_VIP_FAN_PWM_INDEX]);
			break;

		case IfcVipFan_Air2:
			HalDcFans->StartFanAirRight(pData[IFC_VIP_FAN_PWM_INDEX]);
			break;
	}


	return(result);
}
//=== end ControlFan ===============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::GetStateFan(u8* pData)
{
	THalDcFans* HalDcFans;
	u16 readRpm;


	static THalDcFans& halF = THalDcFans::GetInstance();
	HalDcFans= &halF;

	switch((EIfcVipFan)pData[IFC_VIP_NUMBER_OF_ITEM])
	{
		case IfcVipFan_Main:
			pData[IFC_VIP_FAN_PWM_INDEX] = TaskHAL.GetPwmFanMain();
			readRpm = TaskHAL.GetRpmFanMain();
			break;

		case IfcVipFan_Ptc1:
			pData[IFC_VIP_FAN_PWM_INDEX] = HalDcFans->GetPwmFanPtcLeft();
			readRpm = HalDcFans->GetRpmFanPtcLeft();
			break;

		case IfcVipFan_Ptc2:
			pData[IFC_VIP_FAN_PWM_INDEX] = HalDcFans->GetPwmFanPtcRight();
			readRpm = HalDcFans->GetRpmFanPtcRight();
			break;

		case IfcVipFan_Air1:
			pData[IFC_VIP_FAN_PWM_INDEX] = HalDcFans->GetPwmFanAirLeft();
			readRpm = HalDcFans->GetRpmFanAirLeft();
			break;

		case IfcVipFan_Air2:
			pData[IFC_VIP_FAN_PWM_INDEX] = HalDcFans->GetPwmFanAirRight();
			readRpm = HalDcFans->GetRpmFanAirRight();
			break;
	}

	memcpy((void*)&pData[IFC_VIP_FAN_RPM_INDEX], (void*)&readRpm, sizeof(readRpm));
	memset((void*)&pData[IFC_VIP_FAN_RPM_INDEX + sizeof(readRpm)], 0, (IFC_VIP_UART_SIZE_DATA - 3));


	return(OsResult_Ok);
}
//=== end GetStateFan ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::ControlMotor(u8* pData)
{
	THalMotChambers* HalMotChambers;


	static THalMotChambers& halMC = THalMotChambers::GetInstance();
	HalMotChambers= &halMC;

	switch((EIfcVipMotor)pData[IFC_VIP_NUMBER_OF_ITEM])
	{
		case IfcVipMotor_Main:
			if(pData[IFC_VIP_MOTOR_PWM_INDEX] > 0)
			{
				// added resistor
				HAL_GPIO_WritePin(SW_STATOR1_GPIO_Port, SW_STATOR1_Pin, GPIO_PIN_RESET);

				if((EIfcVipMotorDirection)pData[IFC_VIP_MOTOR_DIR_INDEX] == IfcVipMotorDirection_CW)
				{
					TaskHAL.SetEvents(TASK_HAL_CMD_START_MOTOR_CW);
				}
				else
				{
					TaskHAL.SetEvents(TASK_HAL_CMD_START_MOTOR_CCW);
				}
			}
			else
			{
				TaskHAL.SetEvents(TASK_HAL_CMD_STOP_MOTOR);
			}
			break;


		case IfcVipMotor_Chamber1:
			if(pData[IFC_VIP_MOTOR_PWM_INDEX] > 0)
			{
				if((EIfcVipMotorDirection)pData[IFC_VIP_MOTOR_DIR_INDEX] == IfcVipMotorDirection_CW)
				{
					HalMotChambers->StartForwardMotorLeft();
				}
				else
				{
					HalMotChambers->StartBackwardMotorLeft();
				}
			}
			else
			{
				HalMotChambers->StopMotorLeft();
			}
			break;

		case IfcVipMotor_Chamber2:
			if(pData[IFC_VIP_MOTOR_PWM_INDEX] > 0)
			{
				if((EIfcVipMotorDirection)pData[IFC_VIP_MOTOR_DIR_INDEX] == IfcVipMotorDirection_CW)
				{
					HalMotChambers->StartForwardMotorRight();
				}
				else
				{
					HalMotChambers->StartBackwardMotorRight();
				}
			}
			else
			{
				HalMotChambers->StopMotorRight();
			}
			break;

		default:
			return(OsResult_Ok);
	}


	return(OsResult_Ok);
}
//=== end ControlMotor =============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::GetStateMotor(u8* pData)
{
	THalMotChambers* HalMotChambers;


	static THalMotChambers& halMC = THalMotChambers::GetInstance();
	HalMotChambers= &halMC;

	switch((EIfcVipMotor)pData[IFC_VIP_NUMBER_OF_ITEM])
	{
		case IfcVipMotor_Chamber1:
			pData[IFC_VIP_MOTOR_PWM_INDEX] = HalMotChambers->GetPwmMotorLeft();
			if(HalMotChambers->GetDirMotorLeft())
			{
				pData[IFC_VIP_MOTOR_DIR_INDEX] = (u8)IfcVipMotorDirection_CW;
			}
			else
			{
				pData[IFC_VIP_MOTOR_DIR_INDEX] = (u8)IfcVipMotorDirection_CCW;
			}
			break;

		case IfcVipMotor_Chamber2:
			pData[IFC_VIP_MOTOR_PWM_INDEX] = HalMotChambers->GetPwmMotorRight();
			if(HalMotChambers->GetDirMotorRight())
			{
				pData[IFC_VIP_MOTOR_DIR_INDEX] = (u8)IfcVipMotorDirection_CW;
			}
			else
			{
				pData[IFC_VIP_MOTOR_DIR_INDEX] = (u8)IfcVipMotorDirection_CCW;
			}
			break;

		default:
			pData[IFC_VIP_MOTOR_PWM_INDEX] = 0;
			pData[IFC_VIP_MOTOR_DIR_INDEX] = 0;

	}

	memset((void*)&pData[IFC_VIP_MOTOR_DIR_INDEX + 1], 0, (IFC_VIP_UART_SIZE_DATA - 2));


	return(OsResult_Ok);
}
//=== end GetStateMotor ============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::ControlHeater(u8* pData)
{
	THalHeaters* HalHeaters;


	static THalHeaters& halH = THalHeaters::GetInstance();
	HalHeaters= &halH;

	switch((EIfcVipHeater)pData[IFC_VIP_NUMBER_OF_ITEM])
	{
		case IfcVipHeater_Pad1:
			HalHeaters->StartHeaterPadLeft(pData[IFC_VIP_HEATER_PWM_INDEX]);
			break;

		case IfcVipHeater_Pad2:
			HalHeaters->StartHeaterPadRight(pData[IFC_VIP_HEATER_PWM_INDEX]);
			break;

		case IfcVipHeater_Ptc1:
			HalHeaters->StartHeaterPtcLeft(pData[IFC_VIP_HEATER_PWM_INDEX]);
			break;

		case IfcVipHeater_Ptc2:
			HalHeaters->StartHeaterPtcRight(pData[IFC_VIP_HEATER_PWM_INDEX]);
			break;
	}


	return(OsResult_Ok);
}
//=== end ControlHeater ============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::GetStateHeater(u8* pData)
{
	THalHeaters* HalHeaters;


	static THalHeaters& halH = THalHeaters::GetInstance();
	HalHeaters= &halH;

	switch((EIfcVipHeater)pData[IFC_VIP_NUMBER_OF_ITEM])
	{
		case IfcVipHeater_Ptc1:
			pData[IFC_VIP_HEATER_PWM_INDEX] = HalHeaters->GetPwmHeaterPtcLeft();
			break;

		case IfcVipHeater_Ptc2:
			pData[IFC_VIP_HEATER_PWM_INDEX] = HalHeaters->GetPwmHeaterPtcRight();
			break;

		case IfcVipHeater_Pad1:
			pData[IFC_VIP_HEATER_PWM_INDEX] = HalHeaters->GetPwmHeaterPadLeft();
			break;

		case IfcVipHeater_Pad2:
			pData[IFC_VIP_HEATER_PWM_INDEX] = HalHeaters->GetPwmHeaterPadRight();
			break;
	}

	memset((void*)&pData[IFC_VIP_HEATER_PWM_INDEX + 1], 0, (IFC_VIP_UART_SIZE_DATA - 1));


	return(OsResult_Ok);
}
//=== end GetStateHeater ===========================================================

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
void TTaskSYS::ProcessTick()
{
	EOsResult result;
	TEeprom* pEeprom;
	TRtc Rtc;
	TBetaTestRecord BetaTestRecord;


	TaskChmLeft.SetEvents(TASK_CHM_EVENT_TICK_PROCESS);
	TaskChmRight.SetEvents(TASK_CHM_EVENT_TICK_PROCESS);

	// DEBUG
/*	BetaTestRecord.bme688SensorFan = this->bme688SensorFan;
	BetaTestRecord.bme688SensorLeft = this->bme688SensorLeft;
	BetaTestRecord.bme688SensorRight = this->bme688SensorRight; */

/*	BetaTestRecord.bme688SensorFan.gasResistance = this->counterMinute;
	BetaTestRecord.bme688SensorFan.humidity = this->counterMinute;
	BetaTestRecord.bme688SensorFan.pressure = this->counterMinute;
	BetaTestRecord.bme688SensorFan.temperature = this->counterMinute;

	BetaTestRecord.bme688SensorLeft.gasResistance = this->counterMinute;
	BetaTestRecord.bme688SensorLeft.humidity = this->counterMinute;
	BetaTestRecord.bme688SensorLeft.pressure = this->counterMinute;
	BetaTestRecord.bme688SensorLeft.temperature = this->counterMinute;

	BetaTestRecord.bme688SensorRight.gasResistance = this->counterMinute;
	BetaTestRecord.bme688SensorRight.humidity = this->counterMinute;
	BetaTestRecord.bme688SensorRight.pressure = this->counterMinute;
	BetaTestRecord.bme688SensorRight.temperature = this->counterMinute; */

/*	BetaTestRecord.tPadLeft = this->counterMinute;  // TaskHAL.GetTemperaturePadLeft();
	BetaTestRecord.tPadRight = this->counterMinute + 1;  // TaskHAL.GetTemperaturePadRight();
	BetaTestRecord.tPtcLeft = this->counterMinute + 2;  // TaskHAL.GetTemperaturePtcLeft();
	BetaTestRecord.tPtcRight = this->counterMinute + 3;  // TaskHAL.GetTemperaturePtcRight();
	BetaTestRecord.timestamp = this->GetTimeSystem();
	BetaTestRecord.timestamp.reserved = 0; */

/*	BetaTestRecord.timestamp.d = this->counterMinute;
	BetaTestRecord.timestamp.h = this->counterMinute;
	BetaTestRecord.timestamp.m = this->counterMinute;
	BetaTestRecord.timestamp.ms = this->counterMinute;
	BetaTestRecord.timestamp.reserved = this->counterMinute;
	BetaTestRecord.timestamp.s = this->counterMinute; */


/*	this->Delay(20);
	pEeprom = TaskHAL.GetPointerEeprom();
	result = pEeprom->WriteRecord(&BetaTestRecord);
	if((result != OsResult_Ok) && (result != OsResult_EepromFull))
	{
		this->SetSysState(SysError_I2cErrorChannel1);
		return;
	} */
	// DEBUG

/*	if(this->counterMinute < TASK_SYS_1_MINUTE)
	{
		this->counterMinute++;
	}
	else
	{
		this->counterMinute = 0;

		pEeprom = TaskHAL.GetPointerEeprom();
		result = pEeprom->ReadRtc(&Rtc);
		if(result != OsResult_Ok)
		{
			this->SetSysState(SysError_I2cErrorChannel2);
			return;
		}

		TaskChmLeft.SetConfigCompostProcess(Rtc.hours);
		TaskChmRight.SetConfigCompostProcess(Rtc.hours);
	} */

	if(this->counterBetaTestLog > 0)
	{
		this->counterBetaTestLog--;
	}
	else
	{
		BetaTestRecord.bme688SensorFan = this->bme688SensorFan;
		BetaTestRecord.bme688SensorLeft = this->bme688SensorLeft;
		BetaTestRecord.bme688SensorRight = this->bme688SensorRight;
		BetaTestRecord.tPadLeft = TaskHAL.GetTemperaturePadLeft();
		BetaTestRecord.tPadRight = TaskHAL.GetTemperaturePadRight();
		BetaTestRecord.tPtcLeft = TaskHAL.GetTemperaturePtcLeft();
		BetaTestRecord.tPtcRight = TaskHAL.GetTemperaturePtcRight();
		BetaTestRecord.timestamp = this->GetTimeSystem();
		BetaTestRecord.timestamp.reserved = 0;

/*		pEeprom = TaskHAL.GetPointerEeprom();
		result = pEeprom->WriteRecord(&BetaTestRecord);
		if((result != OsResult_Ok) && (result != OsResult_EepromFull))
		{
			this->SetSysState(SysError_I2cErrorChannel1);
			return;
		} */

		this->counterBetaTestLog = TASK_SYS_5_MINUTES;
	}



//	if(this->bme688SensorLeft.humidity)
}
//=== end ProcessTick ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
/* EOsResult TTaskSYS::SelfTest()
{
//	EOsResult result;



	return(OsResult_Ok);
} */
//=== end SelfTest =================================================================

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
void TTaskSYS::UpdateTopCpuState(TIfcSystemState* IfcSystemState)
{


	taskENTER_CRITICAL();

	this->ifcSystemState.levelChamberLeft = IfcSystemState->levelChamberLeft;
	this->ifcSystemState.levelChamberRight = IfcSystemState->levelChamberRight;

	taskEXIT_CRITICAL();

/*	if(this->getState.state == IfcVipState_Error)
	{
		this->SetSysState(this->getState.error);
	}
	*/
}
//=== end UpdateTopCpuState ========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
void TTaskSYS::UpdateSensorBme688(EIfcBme688Sensor ifcBme688Sensor, u8* pBufferState)
{

	taskENTER_CRITICAL();

	if(ifcBme688Sensor == IfcBme688Sensor_Fan)
	{
		memcpy((void*)&this->bme688SensorFan, (void*)pBufferState, sizeof(TBme688Sensor));
	}
	else
	{
		if(ifcBme688Sensor == IfcBme688Sensor_Left)
		{
			memcpy((void*)&this->bme688SensorLeft, (void*)pBufferState, sizeof(TBme688Sensor));
		}
		else  // IfcBme688Sensor_Right
		{
			memcpy((void*)&this->bme688SensorRight, (void*)pBufferState, sizeof(TBme688Sensor));
		}
	}

	taskEXIT_CRITICAL();

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

		case SysState_TopRemoved:
		case SysState_LidOpen:
			uiEvent = TASK_UI_EVENT_TOP_REMOVED;
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
void TTaskSYS::ReInitUart()
{
	HAL_UART_DeInit(&huart1);
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	HAL_UART_Init(&huart1);

}
//=== end ReInitUart ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
TimeSystem TTaskSYS::GetTimeSystem(void)
{
	TimeSystem timeSystem;


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
u64 TTaskSYS::GetSystemCounter()
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
void TTaskSYS::TestChamberMotors()
{
	THalMotChambers* HalMotChambers;


	static THalMotChambers& halMC = THalMotChambers::GetInstance();
	HalMotChambers= &halMC;

	TaskHAL.AcPowerOn();
	this->Delay(1000);

	while(true)
	{
		HalMotChambers->StartForwardMotorLeft();
		HalMotChambers->StartForwardMotorRight();
		this->Delay(10 * 60 * 1000);  // 10 minutes
		HalMotChambers->StopMotorLeft();
		HalMotChambers->StopMotorRight();

		this->Delay(10 * 60 * 1000);  // 10 minutes

		HalMotChambers->StartBackwardMotorLeft();
		HalMotChambers->StartBackwardMotorRight();
		this->Delay(10 * 60 * 1000);  // 10 minutes
		HalMotChambers->StopMotorLeft();
		HalMotChambers->StopMotorRight();

		this->Delay(10 * 60 * 1000);  // 10 minutes
	}


}
//=== end TestChamberMotors ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::TestMainMotor()
{
//	while(true)
//	{
		TaskHAL.AcPowerOn();
		this->Delay(1000);

		TaskUI.SetState(TASK_UI_EVENT_TOP_REMOVED);

		// added resistor
		HAL_GPIO_WritePin(SW_STATOR1_GPIO_Port, SW_STATOR1_Pin, GPIO_PIN_RESET);

		TaskHAL.StartMainMotorCW();

		this->Delay(200);
		// short cut resistor
		HAL_GPIO_WritePin(SW_STATOR1_GPIO_Port, SW_STATOR1_Pin, GPIO_PIN_SET);

		this->Delay(5800);

		// added resistor
		HAL_GPIO_WritePin(SW_STATOR1_GPIO_Port, SW_STATOR1_Pin, GPIO_PIN_RESET);
		this->Delay(200);
		TaskHAL.StopMainMotor();

		this->Delay(200);
		TaskHAL.BrakeOnMainMotor();
		this->Delay(2000);

		TaskUI.SetState(TASK_UI_EVENT_IDLE);



		// added resistor
		HAL_GPIO_WritePin(SW_STATOR1_GPIO_Port, SW_STATOR1_Pin, GPIO_PIN_RESET);

		TaskHAL.StartMainMotorCCW();

		this->Delay(1000);
		// short cut resistor
		HAL_GPIO_WritePin(SW_STATOR1_GPIO_Port, SW_STATOR1_Pin, GPIO_PIN_SET);

		this->Delay(4800);

		// added resistor
		HAL_GPIO_WritePin(SW_STATOR1_GPIO_Port, SW_STATOR1_Pin, GPIO_PIN_RESET);
		this->Delay(200);
		TaskHAL.StopMainMotor();


		this->Delay(200);
		TaskHAL.BrakeOnMainMotor();
		this->Delay(2000);

		TaskUI.SetState(TASK_UI_EVENT_INIT);


		TaskHAL.AcPowerOff();
		this->Delay(1000);
//	}

}
//=== end TestMainMotor ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::TestPtcFans()
{
	THalDcFans* HalDcFans;

	static THalDcFans& halF = THalDcFans::GetInstance();
	HalDcFans= &halF;

	TaskHAL.AcPowerOn();
	this->Delay(1000);

	while(true)
	{
		HalDcFans->StartFanPtcLeft(10);
		this->Delay(5000);
		HalDcFans->StartFanPtcLeft(20);
		this->Delay(5000);
		HalDcFans->StartFanPtcLeft(40);
		this->Delay(5000);

		HalDcFans->StopFanPtcLeft();

		this->Delay(2000);

		HalDcFans->StartFanPtcRight(10);
		this->Delay(5000);
		HalDcFans->StartFanPtcRight(20);
		this->Delay(5000);
		HalDcFans->StartFanPtcRight(40);
		this->Delay(5000);

		HalDcFans->StopFanPtcRight();

		this->Delay(2000);

	}
}
//=== end TestPtcFans ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::TestPtcHeaters()
{
	THalHeaters* HalHeaters;

	static THalHeaters& halH = THalHeaters::GetInstance();
	HalHeaters= &halH;

	TaskHAL.AcPowerOn();
	this->Delay(1000);

	while(true)
	{
		HalHeaters->StartHeaterPtcLeft(10);
		this->Delay(5000);
		HalHeaters->StartHeaterPtcLeft(20);
		this->Delay(5000);
		HalHeaters->StartHeaterPtcLeft(40);
		this->Delay(5000);

		HalHeaters->StopHeaterPtcLeft();

		this->Delay(2000);

		HalHeaters->StartHeaterPtcRight(10);
		this->Delay(5000);
		HalHeaters->StartHeaterPtcRight(20);
		this->Delay(5000);
		HalHeaters->StartHeaterPtcRight(40);
		this->Delay(5000);

		HalHeaters->StopHeaterPtcRight();

		this->Delay(2000);

	}

}
//=== end TestPtcHeaters ===========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::TestPadHeaters()
{
	THalHeaters* HalHeaters;

	static THalHeaters& halH = THalHeaters::GetInstance();
	HalHeaters= &halH;

	TaskHAL.AcPowerOn();
	this->Delay(1000);

	while(true)
	{
		HalHeaters->StartHeaterPadLeft(10);
		this->Delay(5000);
		HalHeaters->StartHeaterPadLeft(20);
		this->Delay(5000);
		HalHeaters->StartHeaterPadLeft(40);
		this->Delay(5000);

		HalHeaters->StopHeaterPadLeft();

		this->Delay(2000);

		HalHeaters->StartHeaterPadRight(10);
		this->Delay(5000);
		HalHeaters->StartHeaterPadRight(20);
		this->Delay(5000);
		HalHeaters->StartHeaterPadRight(40);
		this->Delay(5000);

		HalHeaters->StopHeaterPadRight();

		this->Delay(2000);

	}

}
//=== end TestPadHeaters ===========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskSYS::WaitEvent(u32 event)
{
	u32 resultBits;


	while(true)
	{
		if(this->EventGroup.WaitOrBits(
					TASK_SYS_EVENT_UART_RX_CPLT |
					TASK_SYS_EVENT_UART_ERROR	|
					TASK_SYS_EVENT_RESET        |
					TASK_SYS_EVENT_TOP_REMOVED  |
					TASK_SYS_EVENT_LID_OPEN     |
					TASK_SYS_EVENT_TICK_PROCESS |
					TASK_SYS_EVENT_START_TEST   |
					TASK_SYS_EVENT_ERROR        |
					event,
					&resultBits,
					1000  // 1 Sec
					) == OsResult_Timeout)
		{
			continue;
	    }

	    if((resultBits & TASK_SYS_EVENT_ERROR) > 0)
	    {
	    	this->SetEvents(TASK_SYS_EVENT_ERROR);
	    	return(OsResult_Continue);
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

	    }

	 	if((resultBits & TASK_SYS_EVENT_RESET) > 0)
	 	{
	 		this->SetEvents(TASK_SYS_EVENT_RESET);
	 		return(OsResult_Continue);
	 	}

	 	if((resultBits & TASK_SYS_EVENT_TOP_REMOVED) > 0)
	 	{
	 		this->SetEvents(TASK_SYS_EVENT_TOP_REMOVED);
	 		return(OsResult_Continue);
	 	}

	 	if((resultBits & TASK_SYS_EVENT_LID_OPEN) > 0)
	 	{
//	 		this->SetEvents(TASK_SYS_EVENT_LID_OPEN);
	 		return(OsResult_LidOpen);
	 	}

	 	if((resultBits & event) > 0)
	 	{
	 	   	break;
	 	}

	 	if((resultBits & TASK_SYS_EVENT_TICK_PROCESS) > 0)
	 	{
	 	   	this->ProcessTick();
	 	}

	 	if((resultBits & TASK_SYS_EVENT_START_TEST) > 0)
	 	{
	 		this->SetEvents(TASK_SYS_EVENT_START_TEST);
	 	   	return(OsResult_Continue);
	 	}


	}  // end while(true)


	return(OsResult_Ok);
}
//=== end WaitEvent ================================================================

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
	this->counterMinute = 0;

	TaskUI.Init();
	TaskUI.SetEvents(TASK_UI_CMD_START);
	this->SetSysState(SysState_NoState);

	this->Delay(10);


	this->SetSysState(SysState_Init);
	this->ReInitUart();
   	this->StartRxData();

   	result = TaskHAL.Init();
   	if(result != OsResult_Ok)
   	{
   		return(result);
   	}

   	TaskChmLeft.Init(TaskChamber_Left);
   	TaskChmRight.Init(TaskChamber_Right);

   	TaskHAL.SetEvents(TASK_HAL_CMD_START);
   	TaskChmLeft.SetEvents(TASK_CHM_CMD_START);
   	TaskChmRight.SetEvents(TASK_CHM_CMD_START);
   	this->enableTickHook = true;


   	HAL_GPIO_WritePin(TOP_RST_GPIO_Port, TOP_RST_Pin, GPIO_PIN_SET);
   	this->Delay(200);
   	HAL_GPIO_WritePin(TOP_RST_GPIO_Port, TOP_RST_Pin, GPIO_PIN_RESET);

// 	this->TestChamberMotors();
// 	this->TestMainMotor();
// 	this->TestPtcFans();
// 	this->TestPtcHeaters();
//  this->TestPadHeaters();
// 	this->TestChambers();
   	// DEBUG

 /*  	TaskHAL.AcPowerOn();
   	TaskChmRight.SetPtcTemperatureLevels(50, 55);
   					TaskChmRight.SetPtcTime(6 * 60 * 60, 1 * 60 * 60);
   					TaskChmRight.SetEvents(TASK_CHM_EVENT_START_COMPOSTING);

   					TaskChmLeft.SetPtcTemperatureLevels(50, 55);
   					TaskChmLeft.SetPtcTime(6 * 60 * 60, 1 * 60 * 60);
   					TaskChmLeft.SetEvents(TASK_CHM_EVENT_START_COMPOSTING);
   	while(true)
   	{
   		this->Delay(10000);
   	} */

#ifndef __DEBUG_EXTERNAL_PC_CONTROL

   	this->SelfTest();

#endif




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
