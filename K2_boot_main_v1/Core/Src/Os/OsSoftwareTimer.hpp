/**********************************************************************************/
/**
*  @file
*  OsSoftwareTimer.hpp - declaration of the class TOsSoftwareTimer.
*
*/
/**********************************************************************************/
#ifndef __OsSoftwareTimer_H
#define __OsSoftwareTimer_H

/**********************************************************************************/
#include "Os.hpp"

/**********************************************************************************/
typedef void (*TSoftwareTimerCallbackFunction)(void*);

/**********************************************************************************/
//==================================================================================
class TOsSoftwareTimer : public TOs
{
public:
	/// variables ///

	/// constants ///

	/// functions ///
	TOsSoftwareTimer(void);
	EOsResult CreateStatic(
						const char * const,
						u32,
						const UBaseType_t,
						TSoftwareTimerCallbackFunction,
						void* _this
						);
	TimerHandle_t GetHandle(void);
	EOsResult Stop(u32);
	EOsResult Start(u32);


protected:
	/// variables ///

	/// constants ///

	/// functions ///


private:
	/// variables ///
	TimerHandle_t TimerHandle;
	StaticTimer_t StaticTimer;
	void* context;
	TSoftwareTimerCallbackFunction ClassCallbackFunction;
	
	/// constants ///

	/// functions ///
	static void TimerCallbackAdapter(TimerHandle_t);


};
//=== end class TOsSoftwareTimer ===================================================

/**********************************************************************************/
#endif
