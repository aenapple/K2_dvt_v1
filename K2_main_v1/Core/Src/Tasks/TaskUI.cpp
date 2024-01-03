/**********************************************************************************/
/**
*  @file
*  Todo: TaskUI.cpp.
*
*/
/**********************************************************************************/
#include "TaskUI.hpp"
#include "TaskSYS.hpp"


TTaskUI TaskUI;
extern TTaskSYS TaskSYS;
/**********************************************************************************/
#ifdef __DEBUG_UI_OUTPUT_ENABLED
	#include "../Debug/TaskCLI.hpp"
	extern TTaskConsole TaskConsole;
	#define DiagPrintf(...) TaskConsole.DebugPrintf(__VA_ARGS__)
	#define DiagNotice(fmt, ...) TaskConsole.DebugNotice("NET: " fmt "\r\n", ##__VA_ARGS__)
#else
	#define DiagPrintf(...)
	#define DiagNotice(...)
#endif

/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskUI::Run(void)
{
	u32 resultBits;
//	EOsResult result;


	this->Delay(20);

	while(true)
	{
		if(this->EventGroup.WaitAndBits(TASK_UI_CMD_START, 1000) == OsResult_Ok)
		{
			break;
		}
	}

	this->Delay(10);



	while(true)
	{
        if(this->EventGroup.WaitOrBits(
        			TASK_UI_EVENT_ERROR	|
					TASK_UI_EVENT_INIT	|
					TASK_UI_NO_STATE    |
					TASK_UI_EVENT_IDLE  |
					TASK_UI_EVENT_TOP_REMOVED,
					&resultBits,
					1000
					) == OsResult_Timeout)
        {
            

            continue;
        }

        this->ClearEvents(TASK_UI_EVENT_NEW_SET_STATE);
      
      
        if((resultBits & TASK_UI_EVENT_ERROR) > 0)
        {
       		this->FlashError();
   		}
        
        if((resultBits & TASK_UI_EVENT_INIT) > 0)
        {
           	this->LedAllOff();
           	this->LedRedOn();
        }
        
        if((resultBits & TASK_UI_EVENT_IDLE) > 0)
        {
        	this->FlashIdle();
        }

        if((resultBits & TASK_UI_NO_STATE) > 0)
        {
           	this->LedAllOff();
        }

        if((resultBits & TASK_UI_EVENT_TOP_REMOVED) > 0)
        {
        	this->FlashTopRemoved();
        }
        
        


	}  // end while(true)


}
//=== end Run ======================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskUI::FlashError(void)
{
	u16 numberFlashes;


	numberFlashes = this->GetErrorNumberFlashes();
	while(true)
	{
		this->LedAllOff();
		if(this->EventGroup.WaitAndBits(TASK_UI_EVENT_NEW_SET_STATE, 500) == OsResult_Ok)
		{
			return;
		}

		for(u16 i = 0; i < numberFlashes; i++)
		{
			this->LedAllOff();
			if(this->EventGroup.WaitAndBits(TASK_UI_EVENT_NEW_SET_STATE, 400) == OsResult_Ok)
			{
				return;
			}

			this->LedRedOn();
			if(this->EventGroup.WaitAndBits(TASK_UI_EVENT_NEW_SET_STATE, 400) == OsResult_Ok)
			{
				this->LedAllOff();
				return;
			}

		}

		this->LedAllOff();
		if(this->EventGroup.WaitAndBits(TASK_UI_EVENT_NEW_SET_STATE, 1600) == OsResult_Ok)
		{
			return;
		}

	}

}
//=== end FlashError ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskUI::FlashIdle(void)
{
	while(true)
	{
		this->LedAllOff();
		if(this->EventGroup.WaitAndBits(TASK_UI_EVENT_NEW_SET_STATE, 2000) == OsResult_Ok)
		{
			return;
		}

		this->LedRedOn();
		if(this->EventGroup.WaitAndBits(TASK_UI_EVENT_NEW_SET_STATE, 100) == OsResult_Ok)
		{
			this->LedAllOff();
			return;
		}

	}

}
//=== end FlashIdle ================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskUI::FlashTopRemoved(void)
{
	while(true)
	{
		this->LedAllOff();
		if(this->EventGroup.WaitAndBits(TASK_UI_EVENT_NEW_SET_STATE, 200) == OsResult_Ok)
		{
			return;
		}

		this->LedRedOn();
		if(this->EventGroup.WaitAndBits(TASK_UI_EVENT_NEW_SET_STATE, 200) == OsResult_Ok)
		{
			this->LedAllOff();
			return;
		}

	}

}
//=== end FlashTopRemoved ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskUI::LedRedOn(void)
{
	HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
}
//=== end LedRedOn =================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskUI::LedAllOff(void)
{
	HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
}
//=== end LedAllOff ================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskUI::SetState(u32 event)
{
	this->SetEvents(event | TASK_UI_EVENT_NEW_SET_STATE);
}
//=== end SetState =================================================================

//==================================================================================
u8 TTaskUI::GetErrorNumberFlashes()
{
	return(TaskSYS.GetErrorCode());
}
//=== end GetNumberFlashes =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskUI::Init(void)
{
	//EOsResult result;




	return(OsResult_Ok);
}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskUI::CreateOsResources(void)
{
//	EOsResult result;



	return(OsResult_Ok);
}
//=== end CreateOsResources ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskUI::CreateTask(void)
{
	((TOsTask*)this)->CreateTask("UI TASK", OS_TASK_UI_SIZE_STACK, OS_TASK_UI_PRIORITY);
}
//=== end CreateTask ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskUI::CreateTaskStatic(void)
{
	((TOsTask*)this)->CreateTaskStatic("UI TASK", OS_TASK_UI_SIZE_STACK, OS_TASK_UI_PRIORITY, xStackBuffer, &xTaskBuffer);
}
//=== end CreateTaskStatic =========================================================

/**********************************************************************************/
