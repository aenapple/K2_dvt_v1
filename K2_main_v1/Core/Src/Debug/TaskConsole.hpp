/**********************************************************************************/
/**
*  @file
*  TaskConsole.hpp - declaration of class TTaskConsole.
*
*  Target:  ...
*/

/**********************************************************************************/
#ifndef __TaskConsole_H
#define __TaskConsole_H

/**********************************************************************************/
#include "OsTask.hpp"
#include "Os\OsSemaphore.hpp"
#include "StreamBuffer.hpp"


/**********************************************************************************/
#define TASK_CONSOLE_CMD_START           (1<<0)
#define TASK_CONSOLE_EVENT_TX_START      (1<<1)
#define TASK_CONSOLE_EVENT_TX_DMA_CMPL   (1<<2)


#define TASK_CONSOLE_SIZE_BUFFER_TX         2048
#define TASK_CONSOLE_DEFAULT_TRIGGER_LEVEL  1

#define TASK_CONSOLE_BUFFER_TX_SIZE  256
#define TASK_CONSOLE_BUFFER_RX_SIZE  256

/**********************************************************************************/
//==================================================================================
class TTaskConsole : public TOsTask
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	EOsResult DebugPrintf(const char *, ...);
	EOsResult DebugNotice(const char *, ...);

	EOsResult SendData(u8*, u16);
	EOsResult SendString(std::string*);
	EOsResult UartDataFrameSent(void);
	u32 SendToUart(u8* inputData, u32 numberBytes, u32 timeout);

	void SetEventUartDmaCpltFromISR(void);

	EOsResult Init(void);
	void CreateTask(void);
	void CreateTaskStatic(void);

  
protected:
	////// variables //////


	////// constants //////


	////// functions //////


  
private:
	////// variables //////
	StaticTask_t xTaskBuffer;
	StackType_t xStackBuffer[OS_TASK_CLI_SIZE_STACK];

	u8 bufferUartTx[TASK_CONSOLE_SIZE_BUFFER_TX];
	TOsSemaphore SemaphoreTx;
	TStreamBuffer StreamBufferTx;

    // DEBUG
    //bool toggleLed;
    // DEBUG

	
	////// constants //////


	////// functions //////
	EOsResult DebugVprintf(const char *, va_list);


	void Run(void);
    EOsResult CreateOsResources(void);


};
//=== end class TTaskConsole =======================================================

/**********************************************************************************/
#endif
