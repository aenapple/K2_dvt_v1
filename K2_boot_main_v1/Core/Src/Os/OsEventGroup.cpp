/**********************************************************************************/
/**
*  @file
*  Todo: OsEventGroup.cpp.
*
*/
/**********************************************************************************/
#include "OsEventGroup.hpp"

/**********************************************************************************/
//==================================================================================
/**
*  Constructor.
*
*  @return ... .
*/
TOsEventGroup::TOsEventGroup(void)
{
	this->EventGroupHandle= NULL;
}
//=== end TOsEventGroup ============================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
EOsResult TOsEventGroup::Create(void)
{
	if(this->EventGroupHandle != NULL)
	{
		return(OsResult_ErrorCreateResource);
	}

	this->EventGroupHandle= xEventGroupCreate();
	if(this->EventGroupHandle == NULL)
	{
		return(OsResult_ErrorCreateResource);
	}

	return(OsResult_Ok);
}
//=== end Create ===================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
EOsResult TOsEventGroup::CreateStatic(void)
{
	if(this->EventGroupHandle != NULL)
	{
		return(OsResult_ErrorCreateResource);
	}

	this->EventGroupHandle= xEventGroupCreateStatic(&StaticEventGroup);
	if(this->EventGroupHandle == NULL)
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
*  	@return
*/
EventGroupHandle_t TOsEventGroup::GetHandle(void)
{
	return(this->EventGroupHandle);
}
//=== end GetHandle ================================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
EOsResult TOsEventGroup::WaitAndBits(u32 bits, TickType_t timeout)
{
	EventBits_t resultBits;

	resultBits= xEventGroupWaitBits(this->EventGroupHandle, bits, pdTRUE, pdTRUE, timeout);

	if((resultBits & bits) == bits)
	{
		return(OsResult_Ok);
	}

	return(OsResult_Timeout);
}
//=== end WaitAndBits ==============================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
EOsResult TOsEventGroup::WaitAndBitsNoClear(u32 bits, TickType_t timeout)
{
	EventBits_t resultBits;

	resultBits= xEventGroupWaitBits(this->EventGroupHandle, bits, pdFALSE, pdTRUE, timeout);

	if((resultBits & bits) == bits)
	{
		return(OsResult_Ok);
	}

	return(OsResult_Timeout);
}
//=== end WaitAndBitsNoClear =======================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
EOsResult TOsEventGroup::WaitOrBits(u32 bits, u32* returnBits, TickType_t timeout)
{
	EventBits_t resultBits;

	resultBits= xEventGroupWaitBits(this->EventGroupHandle, bits, pdTRUE, pdFALSE, timeout);

	if((resultBits & bits) == 0)
	{
		return(OsResult_Timeout);
	}

	(*returnBits)= resultBits;

	return(OsResult_Ok);
}
//=== end WaitOrBits ===============================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
EOsResult TOsEventGroup::WaitOrBitsNoClear(u32 bits, u32* returnBits, TickType_t timeout)
{
	EventBits_t resultBits;

	resultBits= xEventGroupWaitBits(this->EventGroupHandle, bits, pdFALSE, pdFALSE, timeout);

	if((resultBits & bits) == 0)
	{
		return(OsResult_Timeout);
	}

	(*returnBits)= resultBits;

	return(OsResult_Ok);
}
//=== end WaitOrBitsNoClear ========================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
u32 TOsEventGroup::ClearBits(u32 bits)
{
	return(xEventGroupClearBits(this->EventGroupHandle, bits));
}
//=== end ClearBits ================================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
#if((configUSE_TRACE_FACILITY == 1) && (INCLUDE_xTimerPendFunctionCall == 1) && (configUSE_TIMERS == 1))
EOsResult TOsEventGroup::ClearBitsFromISR(u32 bits)
{
	if(xEventGroupClearBitsFromISR(this->EventGroupHandle, bits) == pdPASS)
	{
		return(OsResult_Ok);
	}
	else
	{
		return(OsResult_ResourceBusy);
	}
}
#endif
//=== end ClearBitsFromISR =========================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
void TOsEventGroup::SetBits(u32 bits)
{
	xEventGroupSetBits(this->EventGroupHandle, bits);
}
//=== end SetBits ==================================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
EOsResult TOsEventGroup::SetBitsFromISR(u32 bits)
{
	BaseType_t result;
	BaseType_t highPriorityTaskWoken;

	highPriorityTaskWoken = pdFALSE;
	result= xEventGroupSetBitsFromISR(this->EventGroupHandle, bits, &highPriorityTaskWoken);

	if(result == pdPASS)
	{
		portYIELD_FROM_ISR(highPriorityTaskWoken);
		return(OsResult_Ok);
	}

	return(OsResult_ResourceBusy);
}
//=== end SetBitsFromISR ===========================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
u32 TOsEventGroup::GetBits(void)
{
	return(xEventGroupGetBits(this->EventGroupHandle));
}
//=== end GetBits ==================================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
u32 TOsEventGroup::GetBitsFromISR(void)
{
	return(xEventGroupGetBitsFromISR(this->EventGroupHandle));
}
//=== end GetBitsFromISR ===========================================================

/**********************************************************************************/
