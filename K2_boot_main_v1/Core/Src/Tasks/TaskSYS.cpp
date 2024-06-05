/**********************************************************************************/
/**
*  @file
*  Todo: TaskSYS.cpp.
*
*/
/**********************************************************************************/
#include "TaskUI.hpp"
#include <stdio.h>
#include <stdarg.h>
#include "TaskSYS.hpp"


TTaskSYS TaskSYS;
extern TTaskUI TaskUI;
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
	'L','L','0','1','-','B','B','E','-','0','1','.','0','0',' ',' ',  // version - 16 bytes
	'C','R','C','_',                                                  // crc32 - 4 bytes
	'S','I','Z','E',                                                  // firmware size - 4 bytes
	'L','L','1','-','0','0','0','0','0','0','0','0','0','0','0','0',  // serial number - 16 bytes
	'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0',  // encryption key - 16 bytes
};

#define SYS_STATE_TABLE(enumState, interfaceVipCode, errorCode) \
		errorCode

const u8 TTaskSYS::errorCode[Number_ESysState] =
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
void CreateApplicationTasks(void)
{
	TaskUI.CreateTaskStatic();

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


	if(this->Init() != OsResult_Ok)
	{
		this->InitProcessError();
	}


	this->InterfaceVIP.ReInit();
	this->Delay(2);
	this->StartRxData();

//	DiagNotice("TaskSYS started!");

//	this->SetSysState(SysState_Idle);

	while(true)
	{
		if(this->EventGroup.WaitOrBits(
					TASK_SYS_EVENT_UART_RX_CPLT	    |
					TASK_SYS_EVENT_UART_ERROR	    |
					TASK_SYS_EVENT_JUMP_APPLICATION |
					TASK_SYS_EVENT_RESET            |
					TASK_SYS_ERROR,
					&resultBits,
					1000  // 1 Sec
					) == OsResult_Timeout)
        {
			continue;
        }

        if((resultBits & TASK_SYS_ERROR) > 0)
        {
        	this->ProcessError();
        }

        if((resultBits & TASK_SYS_EVENT_UART_RX_CPLT) > 0)
        {
        	this->ProcessRxData();
        }

 		if(((resultBits & TASK_SYS_EVENT_UART_ERROR) > 0)
 			|| ((resultBits & TASK_SYS_EVENT_UART_RX_TIMEOUT) > 0))
       	{
       		this->InterfaceVIP.ReInit();
       		this->Delay(2);
       		this->StartRxData();

       		continue;
       	}

 		if((resultBits & TASK_SYS_EVENT_JUMP_APPLICATION) > 0)
 		{
 		   	this->JumpToApplication();
 		}

 		if((resultBits & TASK_SYS_EVENT_RESET) > 0)
 		{
 		   	this->Reset();
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
void TTaskSYS::InitProcessError(void)
{
	this->ProcessError();
}
//=== end InitProcessError =========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
void TTaskSYS::ProcessError(void)
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
void TTaskSYS::ProcessRxData(void)
{
	EOsResult result;
	u8 data[IFC_VIP_UART_SIZE_PACKET - 2];
	u8* pData;
//	u16 tempU16;
	u32 tempU32;
	EIfcVipCommand command;
	u8 byte;
	u8 packet[IFC_VIP_MEMORY_PACKET_SIZE];


//	this->timeoutInterfaceVIP = TaskSys.GetSystemCounter();

	pData = this->InterfaceVIP.GetPointerDataRx();
	command = this->InterfaceVIP.GetCommand();
	data[IFC_VIP_MEMORY_TYPE_INDEX] = pData[IFC_VIP_MEMORY_TYPE_INDEX];
	switch(command)
	{
		case IfcVipCommand_GetState:
			data[IFC_VIP_STATE_INDEX] = 0;
			data[IFC_VIP_ERROR_INDEX] = 0;
			data[IFC_VIP_SUB_STATE_INDEX] = (u8)IfcVipSubState_Bootloader;
			break;

		case IfcVipCommand_WriteByte:
			if(data[IFC_VIP_MEMORY_TYPE_INDEX] == IfcVipMemory_Cpu1)
			{
				memcpy((void*)&tempU32, (void*)&pData[IFC_VIP_MEMORY_ADR_INDEX], sizeof(tempU32));
				byte = pData[IFC_VIP_MEMORY_WR_DATA_INDEX];
				result = this->WriteByteToFlash(tempU32, byte);
				if(result != OsResult_Ok)
				{
					command = IfcVipCommand_nAck;
				}
			}
			else
			{
				command = IfcVipCommand_nAck;
			}
			break;

		case IfcVipCommand_ReadByte:
			if(data[IFC_VIP_MEMORY_TYPE_INDEX] == IfcVipMemory_Cpu1)
			{
				memcpy((void*)&tempU32, (void*)&pData[IFC_VIP_MEMORY_ADR_INDEX], sizeof(tempU32));
				result = this->ReadByteToFlash(tempU32, &byte);
				if(result != OsResult_Ok)
				{
					command = IfcVipCommand_nAck;
				}
				else
				{
					data[IFC_VIP_MEMORY_RD_DATA_INDEX] = byte;
				}
			}
			else
			{
				command = IfcVipCommand_nAck;
			}
			break;

		case IfcVipCommand_WritePacket:
			if(data[IFC_VIP_MEMORY_TYPE_INDEX] == IfcVipMemory_Cpu1)
			{
				memcpy((void*)&tempU32, (void*)&pData[IFC_VIP_MEMORY_ADR_INDEX], sizeof(tempU32));
				memcpy((void*)packet, (void*)&pData[IFC_VIP_MEMORY_WR_DATA_INDEX], sizeof(packet));
				result = this->WritePacketToFlash(tempU32, packet);
				if(result != OsResult_Ok)
				{
					command = IfcVipCommand_nAck;
				}
			}
			else
			{
				command = IfcVipCommand_nAck;
			}
			break;

		case IfcVipCommand_ReadPacket:
			if(data[IFC_VIP_MEMORY_TYPE_INDEX] == IfcVipMemory_Cpu1)
			{
				memcpy((void*)&tempU32, (void*)&pData[IFC_VIP_MEMORY_ADR_INDEX], sizeof(tempU32));
				result = this->ReadPacketToFlash(tempU32, packet);
				if(result != OsResult_Ok)
				{
					command = IfcVipCommand_nAck;
				}
				else
				{
					memcpy((void*)&pData[IFC_VIP_MEMORY_RD_DATA_INDEX], (void*)packet, sizeof(packet));
				}
			}
			else
			{
				command = IfcVipCommand_nAck;
			}
			break;

		case IfcVipCommand_JumpApplication:
			this->SetEvents(TASK_SYS_EVENT_JUMP_APPLICATION);
			break;

		case IfcVipCommand_Reset:
			this->SetEvents(TASK_SYS_EVENT_RESET);
			break;


		default:
			command = IfcVipCommand_nAck;
			break;
	}

	this->InterfaceVIP.StartRxData();
	this->InterfaceVIP.StartTxData(command, data);

	// DEBUG
	//this->Delay(20);
	//this->Delay(2);
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
EOsResult TTaskSYS::WriteByteToFlash(u32 address, u8 data)
{
	return(OsResult_Ok);
}
//=== end WriteByteToFlash ===============-=========================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::ReadByteToFlash(u32 address, u8* data)
{
	return(OsResult_Ok);
}
//=== end ReadByteToFlash ==========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::WritePacketToFlash(u32 address, u8* data)
{
	return(OsResult_Ok);
}
//=== end WritePacketToFlash =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskSYS::ReadPacketToFlash(u32 address, u8* data)
{
	return(OsResult_Ok);
}
//=== end ReadPacketToFlash ========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
void TTaskSYS::JumpToApplication(void)
{
//	EOsResult result;
	u8 versionApplication[16];
	u8* tempAddress;
	typedef void(*pFunction)(void);
	pFunction Jump_To_Application;


	tempAddress = (u8*)(TASK_SYS_ADDRESS_APPLICATION + TASK_SYS_ADDRESS_IDENT);
	memcpy((void*)versionApplication, (void*)tempAddress, TASK_SYS_SIZE_VERSION);
	if((versionApplication[TASK_SYS_ADDRESS_VERSION] != 'L')
		|| (versionApplication[TASK_SYS_ADDRESS_VERSION + 1] != 'L')
		|| (versionApplication[TASK_SYS_ADDRESS_VERSION + 2] != '0')
		|| (versionApplication[TASK_SYS_ADDRESS_VERSION + 3] != '1')
		)
	{
		this->SetError(SysError_ApplicationNone);
		return;
	}


	__disable_irq();
/*	for(u16 i = 0; i < 8; i++)
	{
		NVIC->ICER[i] = 0xFFFFFFFF;
		NVIC->ICPR[i] = 0xFFFFFFFF;
	} */

	HAL_DeInit();

	SysTick->CTRL = 0;  // &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
	__set_CONTROL(0);

	__set_MSP(*(__IO uint32_t*)TASK_SYS_ADDRESS_APPLICATION);
	u32 app_jump_address = *( uint32_t*) (TASK_SYS_ADDRESS_APPLICATION + 4);

	__ISB();
	__DSB();

	Jump_To_Application = (pFunction)app_jump_address;
	Jump_To_Application();

	while(true)
	{
	   this->Delay(100);
	}

	HAL_StatusTypeDef halResult;
	FLASH_EraseInitTypeDef eraseInit = {0};
	u32 pageError;


	HAL_FLASH_Unlock();

	eraseInit.Page = 24;
	eraseInit.NbPages = FLASH_PAGE_NB;
	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
//	eraseInit.Banks = FLASH_BANK_BOTH;
	halResult = HAL_FLASHEx_Erase(&eraseInit, &pageError);

	HAL_FLASH_Lock();

}
//=== end JumpToApplication ========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
void TTaskSYS::Reset(void)
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
	this->InterfaceVIP.StartRxData();
}
//=== end StartRxData ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskSYS::SetError(ESysState error)
{
	this->error = error;
	this->SetEvents(TASK_SYS_ERROR);

	this->sysState = error;
}
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
	return(this->errorCode[this->error]);
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
*  Todo: function description..
*
*  @return ... .
*/
/* void TTaskSYS::SetSysState(ESysState sysState)
{
	u32 uiEvent;

	// DEBUG
	//return;
	// DEBUG

	this->sysState = sysState;

	switch(this->sysState)
	{
		case SysState_Idle:
			uiEvent = TASK_UI_EVENT_IDLE;
			break;

		case SysState_SelfTest:
			uiEvent = TASK_UI_EVENT_INIT;
			break;

		case SysState_CriticalOzoneLevel:
			uiEvent = TASK_UI_EVENT_OZONE_LEVEL;
			break;

		case SysState_Composting:
			uiEvent = TASK_UI_EVENT_COMPOSTING;
			break;

		default:
		// SysState_NoState:
		// SysState_SelfTest:
			uiEvent = TASK_UI_EVENT_INIT;
	}

	TaskUI.SetEvents(uiEvent);

} */
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
	if(this->InterfaceVIP.ParsingRxPacket() == OsResult_Ok)
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
EOsResult TTaskSYS::Init(void)
{
	EOsResult result;

	this->systemCounter = 0;

	TaskUI.Init();
	TaskUI.SetEvents(TASK_UI_CMD_START);
//	this->SetSysState(SysState_NoState);

	this->Delay(10);


	this->enableTickHook = true;
	this->Delay(100);



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
