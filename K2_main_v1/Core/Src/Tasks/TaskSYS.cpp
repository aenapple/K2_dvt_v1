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
extern UART_HandleTypeDef huart2;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		TaskSYS.SetEventUartRxCpltFromISR();
	}

	if(huart == &huart2)
	{
//		TaskHAL.SetEventUart2_RxCpltFromISR();
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
//		TaskHAL.SetEventUart2_TxCpltFromISR();
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
//		TaskHAL.SetEventUart2_ErrorFromISR();
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


/*	this->InterfaceSlaveVIP.ReInit();
	this->Delay(2);
	this->StartRxData(); */

//	DiagNotice("TaskSYS started!");

	this->SetSysState(SysState_Idle);

	while(true)
	{
		if(this->EventGroup.WaitOrBits(
					TASK_SYS_EVENT_UART_RX_CPLT |
					TASK_SYS_EVENT_UART_ERROR	|
					TASK_SYS_EVENT_RESET        |
					TASK_SYS_EVENT_TOP_REMOVED  |
					TASK_SYS_EVENT_LID_OPEN     |
					TASK_SYS_EVENT_TICK_PROCESS |
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
       		this->InterfaceSlaveVIP.ReInit();
       		this->Delay(2);
       		this->StartRxData();

       		continue;
       	}

 		if((resultBits & TASK_SYS_EVENT_RESET) > 0)
 		{
 		   	this->Reset();
 		}

 		if((resultBits & TASK_SYS_EVENT_TOP_REMOVED) > 0)
 		{
 		   	this->SelfTest();
 		}

 		if((resultBits & TASK_SYS_EVENT_LID_OPEN) > 0)
 		{
 		   	this->ProcessLidOpen();
 		}

 		if((resultBits & TASK_SYS_EVENT_TICK_PROCESS) > 0)
 		{
 		   	this->TickProcess();
 		}


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


	this->ClearEvents(TASK_SYS_EVENT_TOP_PRESENT | TASK_SYS_EVENT_LID_CLOSED);
	TaskHAL.SetEvents(TASK_HAL_CMD_SELF_TEST);
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
		   	break;  // Self Test - OK
		}

		if((resultBits & TASK_SYS_EVENT_TOP_REMOVED) > 0)
		{
		   	this->SetSysState(SysState_TopRemoved);
		}

		if((resultBits & TASK_SYS_EVENT_TOP_PRESENT) > 0)
		{
			TaskHAL.SetEvents(TASK_HAL_CMD_SELF_TEST);
			this->SetSysState(SysState_SelsfTest);
		}

		if((resultBits & TASK_SYS_EVENT_LID_OPEN) > 0)
		{
		   	this->SetSysState(SysState_LidOpen);
		}

		if((resultBits & TASK_SYS_EVENT_LID_CLOSED) > 0)
		{
			TaskHAL.SetEvents(TASK_HAL_CMD_SELF_TEST);
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
	    	this->InterfaceSlaveVIP.ReInit();
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
void TTaskSYS::ProcessLidOpen()
{

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
	TaskUI.SetState(TASK_UI_EVENT_ERROR);

	while(true)
	{

		this->Delay(1000);
		// DEBUG
		// DiagNotice("ERROR !!!");
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


//	this->timeoutInterfaceVIP = TaskSys.GetSystemCounter();

	pData = this->InterfaceSlaveVIP.GetPointerDataRx();
	command = this->InterfaceSlaveVIP.GetCommand();
	data[IFC_VIP_MEMORY_TYPE_INDEX] = pData[IFC_VIP_MEMORY_TYPE_INDEX];
	switch(command)
	{
		case IfcVipCommand_GetState:
			memcpy((void*)data, (void*)&this->getState, sizeof(TGetState));
			if(this->sysState < SysError_Start)
			{
				data[IFC_VIP_STATE_INDEX] = this->interfaceVipCode[this->sysState];
				data[IFC_VIP_ERROR_INDEX] = 0;
			}
			else
			{
				data[IFC_VIP_STATE_INDEX] = IfcVipState_Error;
				data[IFC_VIP_ERROR_INDEX] = this->interfaceVipCode[this->sysState];
			}
			data[IFC_VIP_SUB_STATE_INDEX] = IfcVipSubState_Application;
			break;

		case IfcVipCommand_GetBme688_Part1:
			break;

//		case IfcVipCommand_GetBme688_Part2:
//			break;

		case IfcVipCommand_SetFanSpeed:
			break;

		case IfcVipCommand_GetFanSpeed:
			break;

//		case IfcVipCommand_GetAdcChannel:
//			break;

		case IfcVipCommand_GetAcParameters:
			break;

		case IfcVipCommand_GetTemperature:
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

		case IfcVipCommand_Test:
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
			this->ControlLamp(command, pData);
			break;

		case IfcVipCommand_GetStateLamp:
			this->GetStateLamp(command, pData);
			break;

		case IfcVipCommand_ControlMotor:
			this->ControlMotor(command, pData);
			break;

		case IfcVipCommand_GetStateMotor:
			this->GetStateMotor(command, pData);
			break;

		case IfcVipCommand_ControlHeater:
			this->ControlHeater(command, pData);
			break;

		case IfcVipCommand_GetStateHeater:
			this->GetStateHeater(command, pData);
			break;


		default:
			command = IfcVipCommand_nAck;
			break;
	}

	this->InterfaceSlaveVIP.StartRxData();
	this->InterfaceSlaveVIP.StartTxData(command, data);

	// DEBUG
	this->Delay(20);
	this->Delay(2);
	// DEBUG
}
//=== end ProcessEvseRxData ========================================================

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
	return(OsResult_Ok);
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
	return(OsResult_Ok);
}
//=== end ReadPacketFromEeprom =====================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::ControlLamp(EIfcVipCommand ifcVipCommand, u8* pData)
{
/*	EIfcVipMemory typeMemory;


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


*/
	return(OsResult_Ok);
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
EOsResult TTaskSYS::ControlMotor(EIfcVipCommand ifcVipCommand, u8* pData)
{
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
EOsResult TTaskSYS::GetStateMotor(EIfcVipCommand ifcVipCommand, u8* pData)
{
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
EOsResult TTaskSYS::ControlHeater(EIfcVipCommand ifcVipCommand, u8* pData)
{
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
EOsResult TTaskSYS::GetStateHeater(EIfcVipCommand ifcVipCommand, u8* pData)
{
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
void TTaskSYS::TickProcess()
{
	TaskChmLeft.SetEvents(TASK_CHM_EVENT_TICK_PROCESS);
	TaskChmRight.SetEvents(TASK_CHM_EVENT_TICK_PROCESS);
}
//=== end TickProcess ==============================================================

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
	this->InterfaceSlaveVIP.StartRxData();
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
void TTaskSYS::UpdateTopCpuState(u8* pBufferState)
{
	TGetState readGetState;


	taskENTER_CRITICAL();

	memcpy((void*)&readGetState, (void*)pBufferState, sizeof(TGetState));

	this->getState.acCurrent = readGetState.acCurrent;
	this->getState.levelChamverLeft = readGetState.levelChamverLeft;
	this->getState.levelChamverRight = readGetState.levelChamverRight;
	this->getState.levelTank = readGetState.levelTank;
	this->getState.stateLamps = readGetState.stateLamps;
	this->getState.stateDamShutter = readGetState.stateDamShutter;

	this->getState.error = readGetState.error;
	this->getState.state = readGetState.state;

	this->getState.stateMotors &= (0xFF - (TASK_SYS_MOTOR_DAM | TASK_SYS_SENSOR_DAM1 | TASK_SYS_SENSOR_DAM2));
	readGetState.stateMotors &= (TASK_SYS_MOTOR_DAM | TASK_SYS_SENSOR_DAM1 | TASK_SYS_SENSOR_DAM2);
	this->getState.stateMotors |= readGetState.stateMotors;


	this->getState.stateSensors &= (0xFF - (TASK_SYS_SENSOR_TOP_LOCK1 | TASK_SYS_SENSOR_TOP_LOCK2));
	readGetState.stateSensors |= (TASK_SYS_SENSOR_TOP_LOCK1 | TASK_SYS_SENSOR_TOP_LOCK2);
	this->getState.stateSensors |= readGetState.stateSensors;

	taskEXIT_CRITICAL();

	if(this->getState.state == IfcVipState_Error)
	{
		this->SetSysState(this->getState.error);
	}

}
//=== end UpdateTopCpuState ========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
void TTaskSYS::UpdateSensorBme688(u8* pBufferState)
{

	taskENTER_CRITICAL();

	memcpy((void*)&this->bme688Sensor, (void*)pBufferState, sizeof(TBme688Sensor));

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
EOsResult TTaskSYS::Init(void)
{
	EOsResult result;
//	u32 resultBits;


	this->systemCounter = 0;
	this->counterTimeTickProcess = 0;

	TaskUI.Init();
	TaskUI.SetEvents(TASK_UI_CMD_START);
	this->SetSysState(SysState_NoState);

	this->Delay(10);


	this->SetSysState(SysState_Init);
	this->InterfaceSlaveVIP.Init(&huart1, USART1);
	this->InterfaceSlaveVIP.ReInit();
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

   	// DEBUG
   	TaskHAL.AcPowerOn();
   	this->Delay(1000);
   	while(true)
   	{
   		TaskChmLeft.MotorChamber.StartForward();
   		this->Delay(10000);
   		TaskChmLeft.MotorChamber.Stop();

   		this->Delay(1000);

   		TaskChmLeft.MotorChamber.StartBackward();
   		this->Delay(10000);
   		TaskChmLeft.MotorChamber.Stop();

   		this->Delay(10000);

   		TaskChmRight.MotorChamber.StartForward();
   		this->Delay(10000);
   		TaskChmRight.MotorChamber.Stop();

   		this->Delay(1000);

   		TaskChmRight.MotorChamber.StartBackward();
   		this->Delay(10000);
   		TaskChmRight.MotorChamber.Stop();

   		this->Delay(10000);

   	}
   	// DEBUG

   	this->SelfTest();

	// DEBUG
/*	while(true)
	{
		TaskHAL.AcPowerOn();
		this->Delay(1000);


		TaskUI.SetState(TASK_UI_EVENT_TOP_REMOVED);

		// added resistor
		HAL_GPIO_WritePin(SW_STATOR1_GPIO_Port, SW_STATOR1_Pin, GPIO_PIN_RESET);

		TaskHAL.StartMainMotorCW();

		this->Delay(1000);
		// short cut resistor
		HAL_GPIO_WritePin(SW_STATOR1_GPIO_Port, SW_STATOR1_Pin, GPIO_PIN_SET);

		this->Delay(9000);

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

		this->Delay(2000);

		TaskHAL.StopMainMotor();


		this->Delay(200);
		TaskHAL.BrakeOnMainMotor();
		this->Delay(2000);

		TaskUI.SetState(TASK_UI_EVENT_INIT);

		// added resistor
		HAL_GPIO_WritePin(SW_STATOR1_GPIO_Port, SW_STATOR1_Pin, GPIO_PIN_RESET);



		TaskHAL.AcPowerOff();
		this->Delay(30000);  // 30 Sec
	} */
	// DEBUG














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
