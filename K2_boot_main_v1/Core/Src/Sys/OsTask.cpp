/**********************************************************************************/
/**********************************************************************************/
/**
*  @file
*  Todo: OsTask.cpp.
*
*/
/**********************************************************************************/
#include "../Sys/OsTask.hpp"


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
void TOsTask::SetTaskPriority(UBaseType_t uxNewPriority)
{
	vTaskPrioritySet(this->xHandle, uxNewPriority);
}
//=== end SeTOsTaskPriority ==========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
UBaseType_t TOsTask::GetTaskPriority(void)
{
	return uxTaskPriorityGet(this->xHandle);
}
//=== end GeTOsTaskPriority ==========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
void TOsTask::Suspend()
{
	vTaskSuspend(this->xHandle);
}
//=== end SeTOsTaskPriority ==========================================================

//==================================================================================
/**
*  @brief Static wrapper function for starting task.
*  @param task - pointer to the task object
*/
void TOsTask::Exec(void* task)
{
	((TOsTask *)task)->Run();
}
//=== end Exec =====================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
void TOsTask::CreateTask(
		 	const char * const pcName,
			u32 ulStackDepth,
			UBaseType_t uxPriority)
{
	if(xTaskCreate(Exec, pcName, ulStackDepth, this, uxPriority, &xHandle) == pdPASS)
    {
		// The task has been created successfully.
  		if(this->CreateOsResources() == OsResult_Ok)
      	{
  			// OS resources have been created successfully.
  			return;
      	}
    }

    // the task could not be created.
    while(true)
    {
    	TOsTask::Delay(1000);
    }

}
//=== end CreateTask ===============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
void TOsTask::CreateTaskStatic(
			 const char * const pcName,
		 	 u32 ulStackDepth,
			 UBaseType_t uxPriority,
			 StackType_t * const puxStackBuffer,
			 StaticTask_t * const pxTaskBuffer)
{
	xHandle = xTaskCreateStatic(Exec, pcName, ulStackDepth, this, uxPriority, puxStackBuffer, pxTaskBuffer);
	if( xHandle != NULL)
    {
		// The task has been created successfully.
		if(this->CreateOsResources() == OsResult_Ok)
    	{
			if( this->EventGroup.CreateStatic() == OsResult_Ok)
			{
				// OS resources have been created successfully.
				return;
			}
    	}
    }

    // the task could not be created.
    while(true)
    {
    	TOsTask::Delay(1000);
    }

}
//=== end CreateTaskFunction =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
void TOsTask::SetEvents(u32 events)
{
	this->EventGroup.SetBits(events);
}
//=== end SetEvents ================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
void TOsTask::SetEventsFromISR(u32 events)
{
	this->EventGroup.SetBitsFromISR(events);
}
//=== end SetEventsFromISR =========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
EventGroupHandle_t TOsTask::GetEventHandle(void)
{
	return this->EventGroup.GetHandle();
}
//=== end GetEventHandle =========================================================


//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
void TOsTask::ClearEvents(u32 events)
{
	this->EventGroup.ClearBits(events);
}
//=== end ClearEvents ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
#if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )
void TOsTask::ClearEventsFromISR(u32 events)
{
	this->EventGroup.ClearBitsFromISR(events);
}
#endif
//=== end ClearEventsFromISR =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
u32 TOsTask::GetTickCount(void)
{
	return(xTaskGetTickCount());
}
//=== end GetTickCount =============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
/*EOsResult TOsTask::CreateOsResources(void)
{
	//virtual function default implementation because there is always 1 event group exist
	EOsResult result;
	result = this->EventGroup.CreateStatic();
	return(result);
}*/
//=== end CreateOsResources ========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return ... .
*/
u32  TOsTask::GetBits()
{
	return this->EventGroup.GetBits();
}
//=== end ClearEvents ==============================================================

/**********************************************************************************/
