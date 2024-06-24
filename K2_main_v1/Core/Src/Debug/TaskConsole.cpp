/**********************************************************************************/
/**
*  @file
*  Todo: TaskConsole.cpp.
*
*/
/**********************************************************************************/
#include "TaskConsole.hpp"
#include "TaskSYS.hpp"

#ifndef __RELEASE
TTaskConsole TaskConsole;
#endif
extern TTaskSYS TaskSYS;
/**********************************************************************************/
extern UART_HandleTypeDef huart1;


//==================================================================================


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskConsole::Run(void)
{
	u32 resultBits;
	EOsResult result;


	this->Delay(10);



	while(true)
	{
        if(this->EventGroup.WaitOrBits(
        		TASK_CONSOLE_EVENT_TX_START,
					&resultBits,
					1000
					) == OsResult_Timeout)
        {
            

            continue;
        }
      
      
        if((resultBits & TASK_CONSOLE_EVENT_TX_START) > 0)
        {
   			result= this->UartDataFrameSent();
   		}

        
        
        
        
        
        this->Delay(50);  // mSec
		
      
      //this->DebugPrint("Cycles - %06d\r\n", counter);



	}  // end while(true)


}
//=== end Run ======================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskConsole::DebugVprintf(const char * format, va_list args)
{
	u16 size;
	char debugPrintfBuffer[TASK_CONSOLE_BUFFER_TX_SIZE];

	size = vsnprintf(debugPrintfBuffer, sizeof(debugPrintfBuffer), format, args);

	return(this->SendData((u8*)debugPrintfBuffer, size));
}
//=== end DebugVprintf =============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskConsole::DebugPrintf(const char* format, ...)
{
	EOsResult result;
	va_list args;


	va_start(args, format);
	result = DebugVprintf(format, args);
	va_end(args);


	return(result);
}
//=== end DebugPrintf ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskConsole::DebugNotice(const char* format, ...)
{
	EOsResult result;
	TTaskSYS::TimeSystem timeSystem;
	va_list args;


	timeSystem = TaskSYS.GetTimeSystem();
	result = this->DebugPrintf("%02d:%02d:%02d.%03d - ", timeSystem.h, timeSystem.m, timeSystem.s, timeSystem.ms);

    va_start(args, format);
    result = this->DebugVprintf(format, args);
    va_end(args);


    return(result);
}
//=== end DebugNotice ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskConsole::SendData(u8* pBuffer, u16 numberBytes)
{
	if(numberBytes == this->SendToUart(pBuffer, numberBytes, 100)) return OsResult_Ok;
	else return(OsResult_ErrorUartTransmit);
}
//=== end SendData =================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskConsole::SendString(std::string* str)
{
	return(this->SendData((u8*)str->c_str(), str->size()));
}
//=== end SendString ===============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskConsole::UartDataFrameSent(void)
{
	// EOsResult result;
	static u8 tmpBuffer[256];
	u32 numberBytesAvailable;
	u32 numberBytesReceived;


	while(true)
	{
		numberBytesAvailable= this->StreamBufferTx.GetBytesAvailable();
		if(numberBytesAvailable < sizeof(tmpBuffer)) break;

		numberBytesReceived= this->StreamBufferTx.Receive(tmpBuffer, sizeof(tmpBuffer), 10);

		HAL_UART_Transmit_DMA(&huart1, tmpBuffer, sizeof(tmpBuffer));

		if(this->EventGroup.WaitAndBits(TASK_CONSOLE_EVENT_TX_DMA_CMPL, 100) == OsResult_Timeout)
		{
			// todo - DMAError - !!!
			return(OsResult_Timeout);
		}

	}

	if(numberBytesAvailable > 0)
	{
		numberBytesReceived= this->StreamBufferTx.Receive(tmpBuffer, numberBytesAvailable, 10);

		HAL_UART_Transmit_DMA(&huart1, tmpBuffer, numberBytesAvailable);

		if(this->EventGroup.WaitAndBits(TASK_CONSOLE_EVENT_TX_DMA_CMPL, 100) == OsResult_Timeout)
		{
			// todo - DMAError - !!!
			return(OsResult_Timeout);
		}
	}


	return(OsResult_Ok);
}
//=== end UartDataFrameSent =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u32 TTaskConsole::SendToUart(u8* inputData, u32 numberBytes, u32 timeout)
{
	u32 result;


	if(this->SemaphoreTx.Take(timeout) == OsResult_Timeout) return(0);

	result= this->StreamBufferTx.Send(inputData, numberBytes, timeout);

	this->SetEvents(TASK_CONSOLE_EVENT_TX_START);
	this->SemaphoreTx.Give();


	return(result);
}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskConsole::SetEventUartDmaCpltFromISR(void)
{
	this->SetEventsFromISR(TASK_CONSOLE_EVENT_TX_DMA_CMPL);
}
//=== end SetEventUartDmaCpltFromISR ===============================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskConsole::Init(void)
{
	//EOsResult result;


	this->SemaphoreTx.Give();


	return(OsResult_Ok);
}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskConsole::CreateOsResources(void)
{
	EOsResult result;


	result = this->StreamBufferTx.CreateStatic(TASK_CONSOLE_SIZE_BUFFER_TX, TASK_CONSOLE_DEFAULT_TRIGGER_LEVEL, this->bufferUartTx);
	if(result != OsResult_Ok) return(result);

	result= this->SemaphoreTx.CreateStatic();
	if(result != OsResult_Ok) return(result);


	return(OsResult_Ok);
}
//=== end CreateOsResources ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskConsole::CreateTask(void)
{
	((TOsTask*)this)->CreateTask("CONSOLE TASK", OS_TASK_CLI_SIZE_STACK, OS_TASK_CLI_PRIORITY);
}
//=== end CreateTask ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskConsole::CreateTaskStatic(void)
{
	((TOsTask*)this)->CreateTaskStatic("SYSTEM TASK", OS_TASK_CLI_SIZE_STACK, OS_TASK_CLI_PRIORITY, xStackBuffer, &xTaskBuffer);
}
//=== end CreateTaskStatic =========================================================

/**********************************************************************************/
