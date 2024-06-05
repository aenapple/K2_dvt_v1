/**********************************************************************************/
/**
*  @file
*  Todo: Os.cpp.
*
*/
/**********************************************************************************/
#include "Os.hpp"

/**********************************************************************************/


//==================================================================================
/**
*  Todo: function description
*
*  @return void .
*/
void TOs::Delay(u64 time)
{
	if (time > portTICK_PERIOD_MS) vTaskDelay(time / portTICK_PERIOD_MS);
	else vTaskDelay(1);
}
//=== end Delay ====================================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
u32 TOs::GetTimeStamp(void)
{
	return(xTaskGetTickCount());
}
//=== end GetTimeStamp =============================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
bool TOs::IsTimeExpired(u32 timeStamp)
{
	s32 timeout = xTaskGetTickCount() - timeStamp;
	if (timeout >= 0) return(true);

	return(false);
}
//=== end IsTimeExpired ============================================================

/**********************************************************************************/
