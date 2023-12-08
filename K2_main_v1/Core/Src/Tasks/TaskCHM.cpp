/**********************************************************************************/
/**
*  @file
*  Todo: TaskCHM.cpp.
*
*/
/**********************************************************************************/
#include "TaskCHM.hpp"
#include "TaskHAL.hpp"
#include "TaskSYS.hpp"

TTaskCHM TaskChmLeft;
TTaskCHM TaskChmRight;
extern TTaskHAL TaskHAL;
extern TTaskSYS TaskSYS;
/**********************************************************************************/
#ifdef __DEBUG_UID_OUTPUT_ENABLED
	#include "../Debug/TaskCLI.hpp"
	extern TTaskCLI TaskCLI;
	#define DiagPrintf(...) printf(__VA_ARGS__)  // TaskCLI.DebugPrintf(__VA_ARGS__)
	#define DiagNotice(fmt, ...) TaskCLI.DebugNotice("UI: " fmt "\r\n", ##__VA_ARGS__)
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
void TTaskCHM::Run(void)
{
	u32 resultBits;
//	EOsResult result;


	this->Delay(20);  // must be minimum 4 mSec delay before call WaitAndBits/WaitOrBits !!!

	while(true)
	{
		if(this->EventGroup.WaitAndBits(TASK_CHM_CMD_START, 1000) == OsResult_Ok)
		{
			break;
		}
	}

	this->Delay(10);


	while(true)
	{
        if(this->EventGroup.WaitOrBits(
        			TASK_CHM_EVENT_START_COMPOSTING |
					TASK_CHM_EVENT_START_COLLECTING |
					TASK_CHM_EVENT_ERROR,
					&resultBits,
					1000
					) == OsResult_Timeout)
        {
//        	DiagNotice("Working");

            continue;
        }
      
        this->ClearEvents(TASK_CHM_EVENT_NEW_STATE);
      
        if((resultBits & TASK_CHM_EVENT_ERROR) > 0)
        {
//   			this->FlashError();
   		}

        if((resultBits & TASK_CHM_EVENT_START_COMPOSTING) > 0)
        {
        	this->Process(TaskChmState_Composting);
        }

        if((resultBits & TASK_CHM_EVENT_START_COLLECTING) > 0)
        {
        	this->Process(TaskChmState_Composting);
        }
        
        
        
        
        
        // this->Delay(50);  // mSec
		
      
      //this->DebugPrint("Cycles - %06d\r\n", counter);



	}  // end while(true)


}
//=== end Run ======================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::Process(ETaskChmState taskChmState)
{
	u32 resultBits;
	EOsResult result;


	this->Delay(10);

	this->taskChmState = taskChmState;

	while(true)
	{
		if(this->EventGroup.WaitOrBits(
					TASK_CHM_EVENT_TICK_PROCESS |
					TASK_CHM_EVENT_MIXING       |
					TASK_CHM_EVENT_STOP_PROCESS |
					TASK_CHM_EVENT_ERROR,
					&resultBits,
					1000
					) == OsResult_Timeout)
       {
//        	DiagNotice("Working");

            continue;
        }


        if((resultBits & TASK_CHM_EVENT_ERROR) > 0)
        {
   			// todo: set error
        	return;
   		}

        if((resultBits & TASK_CHM_EVENT_TICK_PROCESS) > 0)
        {
        	this->TickProcess();
        }

        if((resultBits & TASK_CHM_EVENT_MIXING) > 0)
        {
        	this->Mixing();
        }

        if((resultBits & TASK_CHM_EVENT_STOP_PROCESS) > 0)
        {
        	this->StopProcess();
        }




	        // this->Delay(50);  // mSec


	      //this->DebugPrint("Cycles - %06d\r\n", counter);



		}  // end while(true)
}
//=== end ProcessComposting ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::TickProcess()
{
	// todo:
	// close loop PTC heater
	// close loop Pad heater
	// close loop Humidity
	// close loop level sensor
	// close loop gas sensor - ???
}
//=== end TickProcess ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::Mixing()
{
	EOsResult result;


	result = this->StartMotorForward(100);  // 100% PWM
	if(result != OsResult_Ok)
	{
		TaskSYS.SetSysState(SysError_ApplicationError);
		return;
	}

	// todo:
	// Start motor forward
	// wait 2 minutes
	// stop motor
	// wait 100 mSec
	// Start motor backward
	// wait 30 seconds
	// stop motor
	// wait 100 mSec
	// Start motor forward
	// wait 2 minutes
	// stop motor
	// wait 100 mSec
	// Start motor backward
	// wait 30 seconds
	// stop motor

}
//=== end Mixing ===================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskCHM::StartMotorForward(u8 pwm)
{
/*	TSysCommand SysCommand;


	SysCommand.command = SysCommand_ControlMotor;
	if(this->taskChmIndex == TaskChmIndex_Left)
	{
		SysCommand.parameters[0] = IfcVipMotor_Chamber1;
	}
	else
	{
		SysCommand.parameters[0] = IfcVipMotor_Chamber2;
	}
	SysCommand.parameters[1] = pwm;
	SysCommand.parameters[2] = IfcVipMotorDirection_Forward;

	return(TaskHAL.SendSysCommand(&SysCommand)); */

	return(OsResult_Ok);
}
//=== end StartMotorForward ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskCHM::StartMotorBackward(u8 pwm)
{
	TSysCommand SysCommand;


	SysCommand.command = SysCommand_ControlMotor;
	if(this->taskChmIndex == TaskChmIndex_Left)
	{
		SysCommand.parameters[0] = IfcVipMotor_Chamber1;
	}
	else
	{
		SysCommand.parameters[0] = IfcVipMotor_Chamber2;
	}
	SysCommand.parameters[1] = pwm;
	SysCommand.parameters[2] = IfcVipMotorDirection_Backward;

	return(TaskHAL.SendSysCommand(&SysCommand));

//	return(OsResult_Ok);
}
//=== end StartMotorBackward =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskCHM::StopMotor()
{
	return(this->StartMotorBackward(0));
}
//=== end StopMotor ================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StopProcess()
{
	// todo:
	// stop chamber motor
	// stop PTC heater
	// stop Pad heater
}
//=== end StopProcess ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskCHM::Delay_IT(u32 time)
{
	u32 resultBits;


	for(u32 i = 0; i < time; i += 10)
	{
		this->Delay(10);

		resultBits = this->EventGroup.GetBits();

		if((resultBits & TASK_CHM_EVENT_ERROR) > 0)
		{
			return(OsResult_Error);
		}

		if((resultBits & TASK_CHM_EVENT_STOP_PROCESS) > 0)
		{
			return(OsResult_StopProcess);
		}

		if((resultBits & TASK_CHM_EVENT_NEW_STATE) > 0)
		{
			return(OsResult_Ok);
		}

	}


	return(OsResult_Ok);
}
//=== end Delay_IT =================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::SetState(u32 event)
{
	this->SetEvents(event | TASK_CHM_EVENT_NEW_STATE);
}
//=== end SetState =================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskCHM::Init(ETaskChmIndex taskChmIndex)
{

	this->taskChmIndex = taskChmIndex;
	this->taskChmState = TaskChmState_Idle;



	return(OsResult_Ok);
}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskCHM::CreateOsResources(void)
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
void TTaskCHM::CreateTask(void)
{
	((TOsTask*)this)->CreateTask("UID TASK", OS_TASK_CHM_SIZE_STACK, OS_TASK_CHM_PRIORITY);
}
//=== end CreateTask ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::CreateTaskStatic(void)
{
	((TOsTask*)this)->CreateTaskStatic("UI TASK", OS_TASK_CHM_SIZE_STACK, OS_TASK_CHM_PRIORITY, xStackBuffer, &xTaskBuffer);
}
//=== end CreateTaskStatic =========================================================

/**********************************************************************************/
