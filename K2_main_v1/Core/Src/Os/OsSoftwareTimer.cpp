/**********************************************************************************/
/**
*  @file
*  Todo: OsSoftwareTimer.cpp.
*
*/
/**********************************************************************************/
#include "OsSoftwareTimer.hpp"

/**********************************************************************************/
//==================================================================================
/**
*  Constructor.
*
*  @return ... .
*/
TOsSoftwareTimer::TOsSoftwareTimer(void)
{
	this->TimerHandle = NULL;
	this->context = NULL;
	this->ClassCallbackFunction = NULL;
}
//=== end TOsSoftwareTimer =========================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
EOsResult TOsSoftwareTimer::CreateStatic(
				const char * const pc_timer_name,
				u32 period_ms,
				const UBaseType_t uxAutoReload,
				TSoftwareTimerCallbackFunction classCallbackFunction,
				void* _this
				)
{
	if(this->TimerHandle != NULL)
	{
		return(OsResult_ErrorCreateResource);
	}

	this->context = _this;
	this->ClassCallbackFunction = classCallbackFunction;
	this->TimerHandle= xTimerCreateStatic(
				pc_timer_name,
				period_ms * portTICK_PERIOD_MS,
				uxAutoReload,
				this,
				TimerCallbackAdapter,
				&StaticTimer
				);
	
	if(this->TimerHandle == NULL)
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
EOsResult TOsSoftwareTimer::Stop(u32 time_to_wait_ms)
{
	if (pdPASS == xTimerStop(GetHandle(), time_to_wait_ms * portTICK_PERIOD_MS))
	{
		return(OsResult_Ok);
	}

	return(OsResult_Timeout);
}
//=== end Stop =====================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
EOsResult TOsSoftwareTimer::Start(u32 time_to_wait_ms)
{
	if (pdPASS == xTimerStart(GetHandle(), time_to_wait_ms * portTICK_PERIOD_MS))
	{
		return(OsResult_Ok);
	}

	return(OsResult_Timeout);
}
//=== end Start ====================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
TimerHandle_t TOsSoftwareTimer::GetHandle(void)
{
	return(this->TimerHandle);
}
//=== end GetHandle ================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
void TOsSoftwareTimer::TimerCallbackAdapter(TimerHandle_t timerHandle)
{
	TOsSoftwareTimer* _this = (TOsSoftwareTimer*)pvTimerGetTimerID(timerHandle);
	_this->ClassCallbackFunction(_this->context);
}
//=== end TimerCallbackAdapter =====================================================

/**********************************************************************************/
