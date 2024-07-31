/**********************************************************************************/
/**
*  @file
*  OsTask.hpp - declaration of class TOsTask.
*
*/

/**********************************************************************************/
#ifndef __OsTask_H
#define __OsTask_H

/**********************************************************************************/
#include "Os/Os.hpp"
#include "Os/OsEventGroup.hpp"
#include "Os/OsQueue.hpp"
#include "Os/OsSemaphore.hpp"
#include "Os/OsStreamBuffer.hpp"
#include "ConfigApplication.h"
#include "SysState.h"
// #include "SysError.h"

/**********************************************************************************/
#define OS_TASK_SYS_SIZE_STACK      (configMINIMAL_STACK_SIZE * 16)  // 16 * 64 * 4 = 4096 bytes
#define OS_TASK_CLI_SIZE_STACK      (configMINIMAL_STACK_SIZE * 8)  // 8 * 64 * 4 = 2048 bytes
#define OS_TASK_UI_SIZE_STACK       (configMINIMAL_STACK_SIZE * 8)  // 8 * 64 * 4 = 2048 bytes
#define OS_TASK_HAL_SIZE_STACK      (configMINIMAL_STACK_SIZE * 8)  // 8 * 64 * 4 = 2048 bytes
#define OS_TASK_CHM_SIZE_STACK      (configMINIMAL_STACK_SIZE * 8)  // 8 * 64 * 4 = 2048 bytes
#define OS_TASK_AI_SIZE_STACK       (configMINIMAL_STACK_SIZE * 8)  // 8 * 64 * 4 = 2048 bytes



////// configMAX_PRIORITIES = 32. //////
/*
  1. Low priority numbers denote low priority tasks. The idle task has priority zero (tskIDLE_PRIORITY).
  2. currently - configMAX_PRIORITIES = 12.
*/

// there is timer_tick_priority option in bsp that selects deferred function call priority
// currently set to this (configMAX_PRIORITIES - 1)
// it is used by Event service for setting events from Interrupt
// (configMAX_PRIORITIES - 1) is occupied by software timers
#define OS_TASK_SYS_PRIORITY       (configMAX_PRIORITIES - 2) // higher priority
#define OS_TASK_HAL_PRIORITY       (configMAX_PRIORITIES - 3)
#define OS_TASK_CHM_PRIORITY       (configMAX_PRIORITIES - 4)
#define OS_TASK_CLI_PRIORITY       (configMAX_PRIORITIES - 5)
#define OS_TASK_UI_PRIORITY	       (configMAX_PRIORITIES - 6)
#define OS_TASK_AI_PRIORITY	       (configMAX_PRIORITIES - 7)

#define OS_TASK_NET_QUEUE_SIZE 16

//#define OS_TASK_SYS_MAX_NUMBER_ERRORS  10
//#define OS_TASK_SYS_SIZE_PARAM_ERRORS  (sizeof(EResult))  // error code - 2 bytes

// #define OS_QUEUE_SYSTEM_MESSAGES               4
// #define OS_QUEUE_SYSTEM_MESSAGES_SIZE_ELEMENT  OS_QUEUE_MAX_SIZE_ELEMENT


/**********************************************************************************/
//==================================================================================
class TOsTask : public TOs
{
public:
	////// variables //////


	////// constants //////

	////// functions //////
	virtual EOsResult Init(void)=0;
	void SetEvents(u32);
	void SetEventsFromISR(u32);
	EventGroupHandle_t GetEventHandle(void);
	void ClearEvents(u32);
//#if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )
	void ClearEventsFromISR(u32);

	u32 GetBits();
//#endif
	u32 GetTickCount(void);
	void CreateTaskStatic(const char * const, u32,	UBaseType_t, StackType_t * const, StaticTask_t * const);
	void CreateTask(const char * const, u32, UBaseType_t);

	UBaseType_t GetTaskPriority(void);
	void SetTaskPriority(UBaseType_t uxNewPriority);

	void Suspend();

protected:
	////// variables //////
	StaticTask_t xTaskBuffer;
	TaskHandle_t xHandle;

	TOsEventGroup EventGroup;


	////// constants //////

	////// functions //////
	static void Exec(void*);
	virtual void Run(void)=0;
	virtual EOsResult CreateOsResources(void)=0;

private:
	////// variables //////

	////// constants //////

	////// functions //////



};
//=== end class TOsTask ============================================================

/**********************************************************************************/
#endif
