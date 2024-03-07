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
const u8 TTaskCHM::tableCompostProcess[TASK_CHM_SIZE_TABLE_COMPOST_PROCESS][3] =
{
	{ 0x00, 35, (5 * 6) /* x10 */ },
	{ 0x07, 40, (4 * 6) /* x10 */ },
	{ 0x11, 45, (2 * 6) /* x10 */ },
	{ 0x14, 50, 6 /* x10 */ },
	{ 0x17, 55, 3 /* x10 */ },
	{ 0x22, 45, (2 * 6) /* x10 */ },
	{ 0x23, 40, (2 * 6) /* x10 */ },
};


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
					TASK_CHM_EVENT_START_COLLECTING,
					&resultBits,
					1000
					) == OsResult_Timeout)
        {
//        	DiagNotice("Working");

            continue;
        }
      
//        this->ClearEvents(TASK_CHM_EVENT_NEW_STATE);
      

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
	this->SetEvents(TASK_CHM_EVENT_MIXING);
	this->ptcCounterRepeatTime = 0;
	this->ptcCounterWorkTime = this->ptcWorkTime;
	this->flagPtcOn = true;
	while(true)
	{
		if(this->EventGroup.WaitOrBits(
					TASK_CHM_EVENT_TICK_PROCESS |
					TASK_CHM_EVENT_MIXING       |
					TASK_CHM_EVENT_STOP_PROCESS,
					&resultBits,
					1000
					) == OsResult_Timeout)
       {
//        	DiagNotice("Working");

            continue;
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
        	return;
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

	if(this->mixingCounterRepeatTime < this->mixingRepeatTime)
	{
		this->mixingCounterRepeatTime++;
	}
	else
	{
		this->mixingCounterRepeatTime = 0;
		this->SetEvents(TASK_CHM_EVENT_MIXING);
	}

/*	if(this->mixingCounterTimeMode1 > 0)
	{
		this->mixingCounterTimeMode1--;
	}
	else
	{
		this->mixingCounterRepeatTime = TASK_CHM_REPEAT_TIME_MIXING_MODE2;
		this->SetEvents(TASK_CHM_EVENT_MIXING);
	} */

	if(this->ptcCounterRepeatTime < this->ptcRepeatTime)
	{
		this->ptcCounterRepeatTime++;
	}
	else
	{
		this->ptcCounterRepeatTime = 0;
		this->ptcCounterWorkTime = this->ptcWorkTime;
		this->flagPtcOn = true;
	}

	if(this->flagPtcOn)
	{
		if(this->ptcCounterWorkTime > 0)
		{
			this->ptcCounterWorkTime--;
		}
		else
		{
			this->flagPtcOn = false;
		}
	}

	///// Pad Heater. //////
	if(this->padTemperature < this->padLowLevel_T)
	{
		this->PadHeater.TurnOn(HeaterPwm_100);
	}

	if(this->padTemperature > this->padHighLevel_T)
	{
		this->PadHeater.TurnOff();
	}

	///// Ptc Heater. //////
	if((this->ptcTemperature < this->ptcLowLevel_T) && this->flagPtcOn)
	{
		this->PtcFan.Start(PtcFanPwm_66, PtcFanMaxPwm_66_100);
		this->PtcHeater.TurnOn(HeaterPwm_50);
	}

	if((this->ptcTemperature > this->ptcHighLevel_T) || (!this->flagPtcOn))
	{
//		this->PtcFan.Stop(); // .Start(PtcFanPwm_50, PtcFanMaxPwm_50);
		this->PtcFan.Start(PtcFanPwm_66, PtcFanMaxPwm_66_100);
		this->PtcHeater.TurnOff();
	}


	// todo:
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
/* void TTaskCHM::TickProcess()
{

	if(this->ptcCounterRepeatTime < this->ptcRepeatTime)
	{
		this->ptcCounterRepeatTime++;
	}
	else
	{
		this->ptcCounterRepeatTime = 0;
		this->ptcCounterWorkTime = this->ptcWorkTime;
		this->flagPtcOn = true;
	}

	if(this->flagPtcOn)
	{
		if(this->ptcCounterWorkTime > 0)
		{
			this->ptcCounterWorkTime--;
		}
		else
		{
			this->flagPtcOn = false;
		}
	}

	///// Pad Heater. //////
	if(this->padTemperature < this->padLowLevel_T)
	{
		TaskHAL.TurnOnHeater(this->padHeater, HeaterPwm_100);
	}

	if(this->padTemperature > this->padHighLevel_T)
	{
		TaskHAL.TurnOffHeater(this->padHeater);
	}

	///// Ptc Heater. //////
	if((this->ptcTemperature < this->ptcLowLevel_T) && this->flagPtcOn)
	{
		TaskHAL.TurnOnHeater(this->ptcHeater, HeaterPwm_30);
	}

	if((this->ptcTemperature > this->ptcHighLevel_T) || (!this->flagPtcOn))
	{
		TaskHAL.TurnOffHeater(this->ptcHeater);
	}


	// todo:
	// close loop Humidity
	// close loop level sensor
	// close loop gas sensor - ???
}*/
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


	this->MotorChamber.StartForward();

	result = this->DelaySecond(120);  // 2 minutes
	this->MotorChamber.Stop();
	if(result == OsResult_StopProcess)
	{
		return;
	}

	this->Delay(100);
	this->MotorChamber.StartBackward();

	result = this->DelaySecond(30);  // 30 Seconds
	this->MotorChamber.Stop();
	if(result == OsResult_StopProcess)
	{
		return;
	}

	this->Delay(100);
	this->MotorChamber.StartForward();

	result = this->DelaySecond(120);  // 2 minutes
	this->MotorChamber.Stop();
	if(result == OsResult_StopProcess)
	{
		return;
	}

	this->Delay(100);
	this->MotorChamber.StartBackward();

	this->DelaySecond(30);  // 30 Seconds
	this->MotorChamber.Stop();

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
	this->MotorChamber.StartForward();


	return(OsResult_Ok);
}
//=== end StartMotorForward ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
/* EOsResult TTaskCHM::StartMotorForward(u8 pwm)
{
	if(this->taskChmIndex == TaskChmIndex_Left)
	{
		TaskHAL.TurnOnMotorChamber(MotorChamber_Left, DirMotorChamber_Forward, pwm);
	}
	else
	{
		TaskHAL.TurnOnMotorChamber(MotorChamber_Right, DirMotorChamber_Forward, pwm);
	}


	return(OsResult_Ok);
} */
//=== end StartMotorForward ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskCHM::StartMotorBackward(u8 pwm)
{
	this->MotorChamber.StartBackward();


	return(OsResult_Ok);
}
//=== end StartMotorBackward =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
/* EOsResult TTaskCHM::StartMotorBackward(u8 pwm)
{
	if(this->taskChmIndex == TaskChmIndex_Left)
	{
		TaskHAL.TurnOnMotorChamber(MotorChamber_Left, DirMotorChamber_Backward, pwm);
	}
	else
	{
		TaskHAL.TurnOnMotorChamber(MotorChamber_Right, DirMotorChamber_Backward, pwm);
	}


	return(OsResult_Ok);
} */
//=== end StartMotorBackward =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskCHM::StopMotor()
{
	this->MotorChamber.Stop();


	return(OsResult_Ok);
}
//=== end StopMotor ================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
/* EOsResult TTaskCHM::StopMotor()
{
	return(this->StartMotorBackward(0));
} */
//=== end StopMotor ================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StopProcess()
{
	this->MotorChamber.Stop();
	this->PtcHeater.TurnOff();
	this->PadHeater.TurnOff();
	this->PtcFan.Stop();
}
//=== end StopProcess ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskCHM::DelaySecond(u16 seconds)
{
	u32 resultBits;
	u64 tempSystemTime;
	u32 timeout;


	timeout = seconds * 1000;
	tempSystemTime = TaskSYS.GetSystemCounter();
	while(true)
	{
		if((TaskSYS.GetSystemCounter() - tempSystemTime) > timeout)
		{
			break;  // timeout passed
		}

		if(this->EventGroup.WaitOrBits(
	       		TASK_CHM_EVENT_STOP_PROCESS |
				TASK_CHM_EVENT_TICK_PROCESS,
				&resultBits,
				1000) == OsResult_Timeout)
	    {
	       	continue;
	    }

	    if((resultBits & TASK_CHM_EVENT_TICK_PROCESS) > 0)
	    {
	      	this->TickProcess();
	    }
	    else
	    {
	       	return(OsResult_StopProcess);  // received TASK_CHM_EVENT_STOP_PROCESS
	    }

	}  // end while(true)


	return(OsResult_Ok);
}
//=== end DelaySecond ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::SetConfigCompostProcess(u8 hours)
{
	u8 timeStart;
	u8 timeStop;



	if(hours == this->tableCompostProcess[TimeCompostProcess_23][0])
	{
		this->SetTimeCompostProcess(TimeCompostProcess_23);
		return;
	}

	if(hours == this->tableCompostProcess[TimeCompostProcess_22][0])
	{
		this->SetTimeCompostProcess(TimeCompostProcess_22);
		return;
	}


	for(u16 i = 0; i < TimeCompostProcess_22; i++)
	{
		timeStart = this->tableCompostProcess[i][0];
		timeStop = this->tableCompostProcess[i + 1][0];

		if((hours >= timeStart) && (hours < timeStop))
		{
			this->SetTimeCompostProcess((ETimeCompostProcess)i);
			return;
		}

	}

}
//=== end SetConfigCompostProcess ==================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::SetTimeCompostProcess(ETimeCompostProcess timeCompostProcess)
{
	u16 newTime;


	if(this->padHighLevel_T != this->tableCompostProcess[timeCompostProcess][1])
	{
		this->padLowLevel_T = this->tableCompostProcess[timeCompostProcess][1] - 2;
		this->padHighLevel_T = this->tableCompostProcess[timeCompostProcess][1];
	}

	newTime = (u16)this->tableCompostProcess[timeCompostProcess][2] * TASK_SYS_10_MINUTES;
#ifdef __DEBUG_TEST_MIXING_MECHANISM
	newTime = TASK_SYS_10_MINUTES;
#endif
	if(this->mixingRepeatTime != newTime)
	{
		this->mixingRepeatTime = newTime;
		this->mixingCounterRepeatTime = 0;
	}

}
//=== end SetTimeCompostProcess ====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::SetPtcTemperature(s8 temperature)
{
	this->ptcTemperature = temperature;
}
//=== end SetPtcTemperature ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
s8 TTaskCHM::GetPtcTemperature(void)
{
	return(this->ptcTemperature);
}
//=== end GetPtcTemperature ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::SetPtcTemperatureLevels(s8 lowLevel, s8 highLevel)
{
	this->ptcLowLevel_T = lowLevel;
	this->ptcHighLevel_T = highLevel;
}
//=== end SetPtcTemperatureLevels ==================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::SetPtcTime(u32 repeatTime, u32 workTime)
{
	this->ptcRepeatTime = repeatTime;
	this->ptcWorkTime = workTime;
}
//=== end SetPtcTime ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::SetPadTemperature(s8 temperature)
{
	this->padTemperature = temperature;
}
//=== end SetPadTemperature ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
s8 TTaskCHM::GetPadTemperature(void)
{
	return(this->padTemperature);
}
//=== end GetPadTemperature ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::SetPadTemperatureLevels(s8 lowLevel, s8 highLevel)
{
	this->padLowLevel_T = lowLevel;
	this->padHighLevel_T = highLevel;
}
//=== end SetPadTemperatureLevels ==================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::SetPadTime(u16 repeatTime, u16 workTime)
{
	this->padRepeatTime = repeatTime;
	this->padWorkTime = workTime;
}
//=== end SetPadTime ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StartMixingTimeMode1()
{
	this->mixingCounterTimeMode1 = TASK_CHM_TIME_MIXING_MODE1;
	this->mixingCounterRepeatTime = TASK_CHM_REPEAT_TIME_MIXING_MODE1;
}
//=== end StartMixingTimeMode1 =====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
ETaskChmState TTaskCHM::GetState(void)
{
	return(this->taskChmState);
}
//=== end GetState =================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
/* void TTaskCHM::SetState(u32 event)
{
	this->SetEvents(event | TASK_CHM_EVENT_NEW_STATE);
} */
//=== end SetState =================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskCHM::Init(ETaskChamber taskChamber)
{

//	this->taskChamber = taskChamber;
	this->taskChmState = TaskChmState_Idle;

	if(taskChamber == TaskChamber_Left)
	{
//		this->ptcHeater = Heater_PtcHeaterLeft;
//		this->padHeater = Heater_PadHeaterLeft;
		this->MotorChamber.Init(MotorChamber_Left);
		this->PtcHeater.Init(Heater_PtcHeaterLeft);
		this->PadHeater.Init(Heater_PadHeaterLeft);
		this->PtcFan.Init(PtcFan_Left);
	}
	else
	{
//		this->ptcHeater = Heater_PtcHeaterRight;
//		this->padHeater = Heater_PadHeaterRight;
		this->MotorChamber.Init(MotorChamber_Right);
		this->PtcHeater.Init(Heater_PtcHeaterRight);
		this->PadHeater.Init(Heater_PadHeaterRight);
		this->PtcFan.Init(PtcFan_Right);
	}

//	this->SetPadTemperature(TASK_CHM_PAD_LOW_LEVEL_T, TASK_CHM_PAD_HIGH_LEVEL_T);
//	this->SetPtcTemperature(TASK_CHM_PTC_LOW_LEVEL_T, TASK_CHM_PTC_HIGH_LEVEL_T);

	this->mixingRepeatTime = TASK_CHM_REPEAT_TIME_MIXING;
	this->mixingCounterRepeatTime = 0;


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
