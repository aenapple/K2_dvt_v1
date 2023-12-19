/**********************************************************************************/
/**
*  @file
*  TaskUI.hpp - declaration of class TTaskUI.
*
*  Target:  ...
*/

/**********************************************************************************/
#ifndef __TaskUI_H
#define __TaskUI_H

/**********************************************************************************/
#include "OsTask.hpp"


/**********************************************************************************/
#define TASK_UI_CMD_START           (1<<0)

#define TASK_UI_EVENT_ERROR          (1<<1)
#define TASK_UI_EVENT_NEW_SET_STATE  (1<<2)
#define TASK_UI_EVENT_INIT           (1<<3)
#define TASK_UI_EVENT_IDLE           (1<<4)
#define TASK_UI_NO_STATE             (1<<5)
#define TASK_UI_EVENT_TOP_REMOVED    (1<<6)



/**********************************************************************************/
//==================================================================================
class TTaskUI : public TOsTask
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	EOsResult Init(void);
	void SetState(u32 event);

	// DEBUG
//	void LedRedOn(void);
//	void LedAllOff(void);
	// DEBUG


	void CreateTask(void);
	void CreateTaskStatic(void);


  
protected:
	////// variables //////


	////// constants //////


	////// functions //////


  
private:
	////// variables //////
	StaticTask_t xTaskBuffer;
	StackType_t xStackBuffer[OS_TASK_UI_SIZE_STACK];

	
	////// constants //////


	////// functions //////
	void FlashError(void);
	void FlashIdle(void);
	void FlashTopRemoved(void);
	void LedRedOn(void);
	void LedAllOff(void);
	u8 GetErrorNumberFlashes(void);


	void Run(void);
	EOsResult CreateOsResources(void);


};
//=== end class TTaskUI ============================================================

/**********************************************************************************/
#endif
