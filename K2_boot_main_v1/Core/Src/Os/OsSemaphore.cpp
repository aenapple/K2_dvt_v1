/**********************************************************************************/
/**
*  @file
*  Todo: OsSemaphore.cpp.
*
*/
/**********************************************************************************/
#include "OsSemaphore.hpp"

/**********************************************************************************/
//==================================================================================
/**
*  The function ... .
*
*  @return ... .
*/
TOsSemaphore::TOsSemaphore(void)
{
	this->SemaphoreHandle= NULL;
}
//=== end TOsSemaphore =============================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
EOsResult TOsSemaphore::CreateStatic(void)
{
	if(this->SemaphoreHandle != NULL)
	{
		return(OsResult_ErrorCreateResource);
	}

	this->SemaphoreHandle = xSemaphoreCreateBinaryStatic(&this->StaticSemaphore);
	if(this->SemaphoreHandle == NULL)
	{
		return(OsResult_ErrorCreateResource);
	}

	return(OsResult_Ok);
}
//=== end CreateStatic =============================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
SemaphoreHandle_t TOsSemaphore::GetHandle(void)
{
	return(this->SemaphoreHandle);
}
//=== end GetHandle ================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
EOsResult TOsSemaphore::Take(u32 timeout)
{
	if(xSemaphoreTake(this->SemaphoreHandle, timeout) == pdTRUE)
	{
		return(OsResult_Ok);
	}
  
	return(OsResult_Timeout);
}
//=== end Take =====================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
/*EOsResult TOsSemaphore::TakeRecursive(u32 timeout)
{
	if(xSemaphoreTakeRecursive(this->SemaphoreHandle, timeout) == pdTRUE)
	{
		return(OsResult_Ok);
	}

	return(OsResult_Timeout);
}*/
//=== end TakeRecursive ============================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
EOsResult TOsSemaphore::Reset(void)
{
	return(this->Take(0));
}
//=== end Reset ====================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
void TOsSemaphore::Give(void)
{
	xSemaphoreGive(this->SemaphoreHandle);
}
//=== end Give =====================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
/*void TOsSemaphore::GiveRecursive(void)
{
	xSemaphoreGiveRecursive(this->SemaphoreHandle);
}*/
//=== end GiveRecursive ============================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
void TOsSemaphore::GiveFromIsr(void)
{
	BaseType_t xHigherPriorityTaskWoken;

	xHigherPriorityTaskWoken= pdFALSE;
	xSemaphoreGiveFromISR(this->SemaphoreHandle, &xHigherPriorityTaskWoken );
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
//=== end GiveFromIsr ==============================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
void TOsSemaphore::TakeFromIsr(void)
{
	BaseType_t xHigherPriorityTaskWoken;

	xHigherPriorityTaskWoken= pdFALSE;
	xSemaphoreTakeFromISR(this->SemaphoreHandle, &xHigherPriorityTaskWoken );
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
//=== end TakeFromIsr ==============================================================

/**********************************************************************************/
