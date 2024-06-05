/**********************************************************************************/
/**
*  @file
*  OsEventGroup.hpp - declaration of the class TOsEventGroup.
*
*/
/**********************************************************************************/
#ifndef __OsEventGroup_H
#define __OsEventGroup_H

/**********************************************************************************/
#include "Os.hpp"

/**********************************************************************************/
//==================================================================================
/**
*
*/
class TOsEventGroup : public TOs
{
public:
	/// variables ///

	/// constants ///

	/// functions ///
	TOsEventGroup(void);
	EOsResult Create(void);
	EOsResult CreateStatic(void);
	EventGroupHandle_t GetHandle(void);
	EOsResult Receive(void*, u16);
	EOsResult Send(void*, u16);
	void SendFromISR(void*);

	EOsResult WaitAndBits(u32, TickType_t);
	EOsResult WaitAndBitsNoClear(u32, TickType_t);
	EOsResult WaitOrBits(u32, u32*, TickType_t);
	EOsResult WaitOrBitsNoClear(u32, u32*, TickType_t);
	u32 ClearBits(u32);
#if ((configUSE_TRACE_FACILITY == 1) && (INCLUDE_xTimerPendFunctionCall == 1) && (configUSE_TIMERS == 1))
	EResult ClearBitsFromISR(u32);
#endif
	void SetBits(u32);
	EOsResult SetBitsFromISR(u32);
	u32 GetBits(void);
	u32 GetBitsFromISR(void);


protected:
	/// variables ///

	/// constants ///

	/// functions ///


private:
	/// variables ///
	EventGroupHandle_t EventGroupHandle;
	StaticEventGroup_t StaticEventGroup;

	/// constants ///

	/// functions ///


};
//=== end class TOsEventGroup ======================================================

/**********************************************************************************/
#endif
