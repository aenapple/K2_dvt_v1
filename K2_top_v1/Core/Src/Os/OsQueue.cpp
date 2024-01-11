/**********************************************************************************/
/**
*  @file
*  Todo: OsQueue.cpp.
*
*/
/**********************************************************************************/
#include "OsQueue.hpp"

/**********************************************************************************/
//==================================================================================
/**
*  Todo: Constructor.
*
*  @return ... .
*/
TOsQueue::TOsQueue(void)
{
	this->QueueHandle= NULL;
}
//=== end TOsQueue ==============================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
EOsResult TOsQueue::Create(u8 lenght, u8 size_element)
{
	if(this->QueueHandle != NULL)
	{
		return(OsResult_ErrorCreateResource);
	}
	if((lenght > OS_QUEUE_MAX_LENGHT) || (size_element > OS_QUEUE_MAX_SIZE_ELEMENT))
	{
		return(OsResult_ErrorCreateResource);
	}

	this->QueueHandle= xQueueCreate(lenght, size_element);
	if(this->QueueHandle == NULL)
	{
		return(OsResult_ErrorCreateResource);
	}

	size = 0;
	return(OsResult_Ok);
}
//=== end Create ===================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
EOsResult TOsQueue::CreateStatic(u8 lenght, u8 size_element, u8* buffer)
{
	if(this->QueueHandle != NULL)
	{
		return(OsResult_ErrorCreateResource);
	}
	if((lenght > OS_QUEUE_MAX_LENGHT) || (size_element > OS_QUEUE_MAX_SIZE_ELEMENT))
	{
		return(OsResult_ErrorCreateResource);
	}

	this->QueueHandle= xQueueCreateStatic(lenght, size_element, buffer, &QueueBuffer);
	if(this->QueueHandle == NULL)
	{
		return(OsResult_ErrorCreateResource);
	}

	size = 0;
	return(OsResult_Ok);
}
//=== end Create ===================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
void TOsQueue::Reset(void)
{
	xQueueReset(this->QueueHandle);
}
//=== end Reset ====================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
QueueHandle_t TOsQueue::GetHandle(void)
{
	return(this->QueueHandle);
}
//=== end GetHandle ================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
EOsResult TOsQueue::Receive(void* p_buffer, TickType_t timeout)
{
	if(xQueueReceive(this->QueueHandle, p_buffer, timeout) == errQUEUE_EMPTY)
	{
		return(OsResult_QueueEmpty);
	}

	size--;
	return(OsResult_Ok);
}
//=== end Receive ==================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
EOsResult TOsQueue::Send(void* p_buffer, TickType_t timeout)
{
	if(xQueueSend(this->QueueHandle, p_buffer, timeout) == errQUEUE_FULL)
	{
		return(OsResult_QueueFull);
	}

	size++;
	return(OsResult_Ok);
}
//=== end Send =====================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
void TOsQueue::SendFromISR(void* p_buffer)
{
  BaseType_t xHigherPriorityTaskWoken;
  
  xQueueSendFromISR(this->QueueHandle, p_buffer, &xHigherPriorityTaskWoken);
  
}
//=== end SendFromISR ==============================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
int TOsQueue::Size()
{
	return size;
}
//=== end Size =====================================================================
/**********************************************************************************/
