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
#include "TaskAI.hpp"


TTaskCHM TaskChm;
extern TTaskHAL TaskHAL;
extern TTaskSYS TaskSYS;

extern TTaskAI TaskAILeft;
//extern TTaskAI  TaskAIRight;

#ifndef __RELEASE
	#include "TaskConsole.hpp"
	extern TTaskConsole TaskConsole;
#endif
/**********************************************************************************/
#ifdef __DEBUG_CHM_OUTPUT_ENABLED
	#define DiagPrintf(...) printf(__VA_ARGS__)  // TaskConsole.DebugPrintf(__VA_ARGS__)
	#define DiagNotice(fmt, ...) TaskConsole.DebugNotice("CHM: " fmt "\r\n", ##__VA_ARGS__)
#else
	#define DiagPrintf(...)
	#define DiagNotice(...)
#endif

/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
void TTaskCHM::Process(ETaskChmState taskChmState)
{
	u32 resultBits;
//	EOsResult result;

	this->Delay(10);

	this->incrementTime  = TASK_SYS_30_SECONDS;
	this->incrementCounter = this->incrementCounter;

	this->heatPurgeInterval = TASK_SYS_10_HOURS;
	this->heatPurgeCounter = TASK_SYS_10_HOURS;
	this->isHeatPurge      = false;

	this->lowTemp   = TASK_CHM_LOW_TEMP;
	this->highTemp  = TASK_CHM_HIGH_TEMP;

	this->lastLowTemp = this->lowTemp;



	this->ptcIntervalTime    = 0;
	this->ptcCounterWorkTime = 0;
	this->ptcDutyCycle       = PtcDutyCycleMode_0;
	this->ptcDutyCycleOnFlag = false;

	this->ptcFanPwm          = 60;
	this->ptcHeaterPwm       = 100;

	this->taskChmState = taskChmState;



	//  Include Instances of AI Class;
	//  TODO Add neural networks, and maybe increment size thingie
		TaskAILeft.Init(this->incrementTime);
	//	TaskAIRight.Init(this->incrementTime);

	this->mixingPhase = MixingPhase_0;
	this->mixIntervalTime = TASK_SYS_2_MINUTES;
//	this->mixCounterIntervalTime = 0;DeterminePtcDutyCycle

	this->ClearEvents(TASK_CHM_EVENT_TICK_PROCESS);
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
			// DEBUG
			if(this->taskChamber == TaskChamber_Left)
			{
				DiagNotice("TaskChmLeft composting!!");
			}
			else
			{
				DiagNotice("TaskChmRight composting!!");
			}
			// DEBUG

			continue;
		}


		if((resultBits & TASK_CHM_EVENT_TICK_PROCESS) > 0)
		{
			this->TickProcess();
		}

		if((resultBits & TASK_CHM_EVENT_MIXING) > 0)
		{
			// DEBUG
			if(this->taskChamber == TaskChamber_Left)
			{
				DiagNotice("TaskChmLeft start mixing!!");
			}
			else
			{
				DiagNotice("TaskChmRight start mixing!!");
			}
			// DEBUG

			this->Mixing();
			this->ClearEvents(TASK_CHM_EVENT_MIXING);

			// DEBUG
			if(this->taskChamber == TaskChamber_Left)
			{
				DiagNotice("TaskChmLeft stop mixing!!");
			}
			else
			{
				DiagNotice("TaskChmRight stop mixing!!");
			}
			// DEBUG
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
	//=== end Process ==================================================================

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
        	// DEBUG
        	if(this->taskChamber == TaskChamber_Left)
        	{
        		DiagNotice("TaskChmLeft composting!!");
        	}
        	else
        	{
        		DiagNotice("TaskChmRight composting!!");
        	}
        	// DEBUG

            continue;
        }
      
//        this->ClearEvents(TASK_CHM_EVENT_NEW_STATE);
      

        if((resultBits & TASK_CHM_EVENT_START_COMPOSTING) > 0)
        {
        	// DEBUG
        	if(this->taskChamber == TaskChamber_Left)
        	{
        	    DiagNotice("TaskChmLeft started composting!!");
        	}
        	else
        	{
        	    DiagNotice("TaskChmRight started composting!!");
        	}
        	// DEBUG

        	this->Process(TaskChmState_Composting);

        	// DEBUG
        	if(this->taskChamber == TaskChamber_Left)
        	{
        	    DiagNotice("TaskChmLeft started composting!!");
        	}
        	else
        	{
        	    DiagNotice("TaskChmRight started composting!!");
        	}
        	// DEBUG
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
void TTaskCHM::TickProcess()
{
	if (this->incrementCounter > 0) {
		this->incrementCounter--;
		return;
	} else {
		this->incrementCounter = this->incrementTime;
	}

	// DEBUG
//	if(this->taskChamber == TaskChamber_Left)
//	{
//	    DiagNotice("TaskChmLeft TickProcess!!");
//	}
//	else
//	{
//	    DiagNotice("TaskChmRight TickProcess!!");
//	}
	// DEBUG

	// Collect Values from Temperature Sensor
	this->padTemperatureLeft = TaskHAL.GetTemperaturePadLeft();
	this->padTemperatureRight = TaskHAL.GetTemperaturePadRight();

	this->ptcTemperatureLeft = TaskHAL.GetTemperaturePtcLeft();
	this->ptcTemperatureRight = TaskHAL.GetTemperaturePtcRight();
	// End Collect Values From Temperature Sensor


	// Get Moisture Predictions
	//	this->rightMoisturePredictions = TaskAIRight.GetPredictions();
	//	this->leftMoisturePredictions  = TaskAILeft.GetPredictions();

	// End Get Moisture Predictions

	////// PTC Fan & Heater counter Using Generated Moistures //////
	// TODO check if statement; or we could do without the array (array might be good for getting an average)
	this->ptcCounterWorkTime -= this->incrementTime;
	if (this->ptcCounterWorkTime <= 0) {
		if (rightMoisturePredictions[0] != 0 && rightMoisturePredictions[1] != 0 && rightMoisturePredictions[2] != 0 &&
				leftMoisturePredictions[0] != 0 && leftMoisturePredictions[1] != 0 && leftMoisturePredictions[2] != 0) {
			// Get Average of the predictions
			// this->DeterminePtcDutyCycle()
		}
	}

	////// End PTC Fan & Heater counter Using Generated Moistures //////


	//////// Mixing Chambers. Make sure they're not mixing at the same time     //////
//	u16 mixingCounter = TaskAILeft.GetMixingCounter();
//	if ( mixingCounter <= 0) {
//		// if right.mixpahse == 0
//			 // begin mixing cycle for left chamber
//	} else {
//		// decrement left mixing counter
//	}
//
//	mixingCounter = TaskAIRight.GetMixingCounter();
//	if (mixingCounter <= 0) {
//		// if left.mixpahse == 0
//		 // begin mixing cycle for right chamber
//	} else {
//		// decrement right mixing counter
//	}

//	////// Mixing counter. //////
//	if(this->mixCounterIntervalTime < this->mixIntervalTime)
//	{
//		this->mixCounterIntervalTime++;
//	}
//	else
//	{
//		this->mixCounterIntervalTime = 0;
//		this->SetEvents(TASK_CHM_EVENT_MIXING);
//	}
//	////// Mixing counter. //////
	//////// End Mixing Chambers   //////


	////// Run AI Chambers  //////

	/////// End Run AI Chambers   //////

	this->ActuatorPWMCheck();

	if (this->ptcHeaterPwm > 0) {
		this->StartHeaterPtc(this->ptcHeaterPwm);
	} else {
		this->StopHeaterPtc();
	}

	if (this->ptcFanPwm > 0) {
		this->StartFanPtc(this->ptcFanPwm);
	} else {
		this->StopFanPtc();
	}


//	EOsResult result;
//	result = OsResult_Ok;

//	if(this->counterCycleCompostProcess < TASK_SYS_360_MINUTES)
//	{
//		this->counterCycleCompostProcess++;
//
//		if((this->counterCycleCompostProcess == TASK_SYS_30_MINUTES)
//			|| (this->counterCycleCompostProcess == TASK_SYS_60_MINUTES)
//			|| (this->counterCycleCompostProcess == TASK_SYS_80_MINUTES)
//			|| (this->counterCycleCompostProcess == TASK_SYS_90_MINUTES)
//			|| (this->counterCycleCompostProcess == TASK_SYS_105_MINUTES)
//			|| (this->counterCycleCompostProcess == TASK_SYS_120_MINUTES)
//			|| (this->counterCycleCompostProcess == TASK_SYS_150_MINUTES)
//			|| (this->counterCycleCompostProcess == TASK_SYS_165_MINUTES)
//			|| (this->counterCycleCompostProcess == TASK_SYS_180_MINUTES)
//			|| (this->counterCycleCompostProcess == TASK_SYS_210_MINUTES)
//			|| (this->counterCycleCompostProcess == TASK_SYS_240_MINUTES)
//			|| (this->counterCycleCompostProcess == TASK_SYS_360_MINUTES)
//			)
//		{
//			this->SetStepCompostProcess(this->counterCycleCompostProcess);
//		}
//
//	}

}
//=== end TickProcess ==============================================================


//==================================================================================
void TTaskCHM::DeterminePtcDutyCycle(u16 moistureChamberA, u16 moistureChamberB){
	if (moistureChamberA >= TASK_CHM_HIGH_MOISTURE && moistureChamberB >= TASK_CHM_HIGH_MOISTURE)
	{

		this->ptcDutyCycle = PtcDutyCycleMode_1;
	}

	else if ((moistureChamberA >= TASK_CHM_HIGH_MOISTURE && moistureChamberB >= TASK_CHM_MEDIUM_MOISTURE) ||
			(moistureChamberB >= TASK_CHM_HIGH_MOISTURE && moistureChamberA >= TASK_CHM_MEDIUM_MOISTURE))
	{

		this->ptcDutyCycle = PtcDutyCycleMode_2;
	}

	else if ((moistureChamberA >= TASK_CHM_HIGH_MOISTURE && moistureChamberB >= TASK_CHM_LOW_MOISTURE) ||
			(moistureChamberB >= TASK_CHM_HIGH_MOISTURE && moistureChamberA >= TASK_CHM_LOW_MOISTURE))
	{

		this->ptcDutyCycle = PtcDutyCycleMode_3;
	}
	else if (moistureChamberA >= TASK_CHM_MEDIUM_MOISTURE && moistureChamberB >= TASK_CHM_MEDIUM_MOISTURE)
	{

		this->ptcDutyCycle = PtcDutyCycleMode_4;
	}
	else if ((moistureChamberA >= TASK_CHM_MEDIUM_MOISTURE && moistureChamberB >= TASK_CHM_LOW_MOISTURE) ||
			(moistureChamberB >= TASK_CHM_MEDIUM_MOISTURE && moistureChamberA >= TASK_CHM_LOW_MOISTURE))
	{

		this->ptcDutyCycle = PtcDutyCycleMode_5;
	}
	else if (moistureChamberA >= TASK_CHM_LOW_MOISTURE and moistureChamberB >= TASK_CHM_LOW_MOISTURE)
	{

		this->ptcDutyCycle = PtcDutyCycleMode_6;

	}
	else
	{
		this->ptcDutyCycle = PtcDutyCycleMode_7;
	}

}
//=== end DeterminePtcDutyCycle =========================================================
void TTaskCHM::ModulatModulateChamberTemperatures(u16 leftChamberHumidity, u16 rightChamberHumidity)
{
    if (leftChamberHumidity >= TASK_CHM_HIGH_HUMIDITY && rightChamberHumidity >= TASK_CHM_HIGH_HUMIDITY)
    {
        this->lowTemp = TASK_CHM_LOW_TEMP + 500;
    }
    else if  (leftChamberHumidity >= TASK_CHM_HIGH_HUMIDITY || rightChamberHumidity >= TASK_CHM_HIGH_HUMIDITY)
    {
    	this->lowTemp = TASK_CHM_LOW_TEMP + 300;
    }
    else
    {
    	this->lowTemp = TASK_CHM_LOW_TEMP;
    }
}
//=== end ModulateChamberTemperatures =========================================================
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::SetPtcDutyCycles()
{

	switch(this->ptcDutyCycle)
	{
    case PtcDutyCycleMode_0:  // Staring Up
        if (this->ptcDutyCycleOnFlag) {

            this->ptcFanPwm  = 10;
            this->ptcHeaterPwm = 0;

            this->ptcIntervalTime = TASK_SYS_1_MINUTE + TASK_SYS_30_SECONDS;
            this->ptcCounterWorkTime = ptcIntervalTime;
            this->ptcDutyCycleOnFlag = false;
        }
        else {

            this->ptcFanPwm    = 50;
            this->ptcHeaterPwm = 100;

            this->ptcCounterWorkTime = TASK_SYS_30_SECONDS;
            this->ptcDutyCycleOnFlag = true;
        }
    case PtcDutyCycleMode_1:  // High High
        if (this->ptcDutyCycleOnFlag) {
            ptcDutyCycleOnFlag = false;

			this->ptcFanPwm  = 10;
			this->ptcHeaterPwm = 0;

			this->ptcIntervalTime = TASK_SYS_1_MINUTE;
			this->ptcCounterWorkTime = ptcIntervalTime;
        }
		else {

            this->ptcFanPwm    = 80;
            this->ptcHeaterPwm = 100;

            this->ptcCounterWorkTime = TASK_SYS_1_MINUTE + TASK_SYS_30_SECONDS;
            this->ptcDutyCycleOnFlag = true;

		}
    case PtcDutyCycleMode_2: // High Medium
        if (this->ptcDutyCycleOnFlag) {

			this->ptcFanPwm  = 10;
			this->ptcHeaterPwm = 0;

			this->ptcIntervalTime = TASK_SYS_2_MINUTES;
			this->ptcCounterWorkTime = ptcIntervalTime;
			this->ptcDutyCycleOnFlag = false;
        }
        else {

            this->ptcFanPwm    = 70;
            this->ptcHeaterPwm = 100;

            this->ptcCounterWorkTime = TASK_SYS_2_MINUTES + TASK_SYS_30_SECONDS;
            this->ptcDutyCycleOnFlag = true;

        }

    case PtcDutyCycleMode_3: // High Low
        if (this->ptcDutyCycleOnFlag) {

			this->ptcFanPwm  = 10;
			this->ptcHeaterPwm = 0;

			this->ptcIntervalTime = TASK_SYS_3_MINUTES;
			this->ptcCounterWorkTime = ptcIntervalTime;
			this->ptcDutyCycleOnFlag = false;

        }
        else {

            this->ptcFanPwm    = 60;
            this->ptcHeaterPwm = 50;

            this->ptcCounterWorkTime = TASK_SYS_3_MINUTES + TASK_SYS_30_SECONDS;
            this->ptcDutyCycleOnFlag = true;

        }
    case PtcDutyCycleMode_4: // Medium Medium
        if (this->ptcDutyCycleOnFlag) {

			this->ptcFanPwm  = 10;
			this->ptcHeaterPwm = 0;

			this->ptcIntervalTime = TASK_SYS_4_MINUTES;
			this->ptcCounterWorkTime = ptcIntervalTime;
			this->ptcDutyCycleOnFlag = false;
        }
        else {

            this->ptcFanPwm    = 40;
            this->ptcHeaterPwm = 100;

            this->ptcCounterWorkTime = TASK_SYS_4_MINUTES + TASK_SYS_30_SECONDS;
            this->ptcDutyCycleOnFlag = true;

        }

    case PtcDutyCycleMode_5: // Medium Low
        if (this->ptcDutyCycleOnFlag) {

			this->ptcFanPwm  = 10;
			this->ptcHeaterPwm = 0;

			this->ptcIntervalTime = TASK_SYS_5_MINUTES;
			this->ptcCounterWorkTime = ptcIntervalTime;
			this->ptcDutyCycleOnFlag = false;
        }
		else {

            this->ptcFanPwm    = 30;
            this->ptcHeaterPwm = 50;

            this->ptcCounterWorkTime = TASK_SYS_5_MINUTES + TASK_SYS_30_SECONDS;
            this->ptcDutyCycleOnFlag = true;
		}

    case PtcDutyCycleMode_6: // Low Low
        if (this->ptcDutyCycleOnFlag) {

			this->ptcFanPwm  = 10;
			this->ptcHeaterPwm = 0;

			this->ptcIntervalTime = TASK_SYS_6_MINUTES;
			this->ptcCounterWorkTime = ptcIntervalTime;
			this->ptcDutyCycleOnFlag = false;
        }
        else {
            this->ptcFanPwm    = 20;
            this->ptcHeaterPwm = 30;

            this->ptcCounterWorkTime = TASK_SYS_6_MINUTES + TASK_SYS_30_SECONDS;
            this->ptcDutyCycleOnFlag = true;
        }

    case PtcDutyCycleMode_7:
        if (this->ptcDutyCycleOnFlag) {

			this->ptcFanPwm  = 10;
			this->ptcHeaterPwm = 0;

			this->ptcIntervalTime = TASK_SYS_7_MINUTES;
			this->ptcCounterWorkTime = ptcIntervalTime;
			this->ptcDutyCycleOnFlag = false;
        }
        else {
        	this->ptcCounterWorkTime = TASK_SYS_7_MINUTES + TASK_SYS_30_SECONDS;
        	this->ptcDutyCycleOnFlag = true;

        }

    case PtcDutyCycleMode_99:
        if (this->ptcDutyCycleOnFlag) {

			this->ptcFanPwm  = 10;
			this->ptcHeaterPwm = 0;

			this->ptcIntervalTime    = TASK_SYS_15_MINUTES;
			this->ptcCounterWorkTime = ptcIntervalTime;
            this->ptcDutyCycleOnFlag = false;
        }
        else {
//            this->AiLeftChamber.mixIntervalTime = TASK_SYS_10_MINUTES;
//            this->AiRightChamber.mixIntervalTime = TASK_SYS_10_MINUTES;

			this->ptcFanPwm  =   80;
			this->ptcHeaterPwm = 10;

            ptcCounterWorkTime = TASK_SYS_2_MINUTES;
            ptcDutyCycleOnFlag = true;
        }
        break;




	}
}
//=== end SetStepDutyCycles ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::ActuatorPWMCheck()
{
	if (this->ptcHeaterPwm >= 100)
		this->ptcHeaterPwm = 100;
	 else if (this->ptcHeaterPwm)
		this->ptcHeaterPwm = 0;

	if (this->ptcFanPwm >= 100)
		this->ptcFanPwm = 100;
	 else if (this->ptcFanPwm <= 0)
		this->ptcFanPwm = 0;

}
//=== end ActuatorPWMCheck =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/


// Might have to do a left and right sensor or something
void TTaskCHM::UpdateSensorBme688(TBme688Sensor* leftBme688Sensor, TBme688Sensor* rightBme688Sensor)
{
	memcpy((void*)&this->leftBmeSensorChamber, (void*)leftBme688Sensor, sizeof(TBme688Sensor));
	memcpy((void*)&this->rightBmeSensorChamber, (void*)rightBme688Sensor, sizeof(TBme688Sensor));
}
//=== end UpdateSensorBme688 =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
//=== end GetSensorBme688 ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/

// TODO make MixingRight, MixingLeft
void TTaskCHM::Mixing()
{
	EOsResult result;


	this->mixingPhase = MixingPhase_1;
	this->StartForwardMotorChamber();
	result = this->DelaySecond(this->timeCw);
	this->StopMotorChamber();
	if(result == OsResult_StopProcess)
	{
		return;
	}

	this->mixingPhase = MixingPhase_2;
	this->StartBackwardMotorChamber();
	result = this->DelaySecond(this->timeCcw);
	this->StopMotorChamber();
	if(result == OsResult_StopProcess)
	{
		return;
	}

	this->mixingPhase = MixingPhase_3;
	this->StartForwardMotorChamber();
	result = this->DelaySecond(this->timeCw);
	this->StopMotorChamber();
	if(result == OsResult_StopProcess)
	{
		return;
	}

	this->mixingPhase = MixingPhase_4;
	this->StartBackwardMotorChamber();
	result = this->DelaySecond(this->timeCcw);
	this->StopMotorChamber();
	if(result == OsResult_StopProcess)
	{
		return;
	}

	this->mixingPhase = MixingPhase_0;

}
//=== end Mixing ===================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StartFanAir(u8 pwm)
{
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalDcFans->StartFanAirLeft(pwm);
	}
	else
	{
		this->HalDcFans->StartFanAirRight(pwm);
	}
}
//=== end StartFanAir ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StopFanAir()
{
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalDcFans->StopFanAirLeft();
	}
	else
	{
		this->HalDcFans->StopFanAirRight();
	}
}
//=== end StopFanAir ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StartFanPtc(u8 pwm)
{
	this->HalDcFans->StartFanPtcLeft(pwm);
	this->HalDcFans->StartFanPtcRight(pwm);

}
//=== end StartFanPtc ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StopFanPtc()
{
    this->HalDcFans->StopFanPtcLeft();

	this->HalDcFans->StopFanPtcRight();

}
//=== end StopFanPtc ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StartHeaterPtc(u8 pwm)
{
	this->HalHeaters->StartHeaterPtcLeft(pwm);

	this->HalHeaters->StartHeaterPtcRight(pwm);

}
//=== end StartHeaterPtc ===========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StopHeaterPtc()
{

	this->HalHeaters->StopHeaterPtcLeft();
	this->HalHeaters->StopHeaterPtcRight();

}
//=== end StopHeaterPtc ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StartHeaterPad(u8 pwm)
{
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalHeaters->StartHeaterPadLeft(pwm);
	}
	else
	{
		this->HalHeaters->StartHeaterPadRight(pwm);
	}
}
//=== end StartHeaterPad ===========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StopHeaterPad()
{
	this->HalHeaters->StopHeaterPadLeft();
	this->HalHeaters->StopHeaterPadRight();

}
//=== end StopHeaterPad ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EHeaterPwm TTaskCHM::GetPwmHeaterPtcLeft(void)
{
	this->HalHeaters->GetPwmHeaterPtcLeft();

}
EHeaterPwm TTaskCHM::GetPwmHeaterPtcRight(void)
{
	this->HalHeaters->GetPwmHeaterPtcRight();
}

//=== end GetPwmHeaterPtc ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::PulseOnHeaterPtcLeft(void)
{
	this->HalHeaters->PulseOnHeaterPtcLeft();

}
void TTaskCHM::PulseOnHeaterPtcRight(void)
{
	this->HalHeaters->PulseOnHeaterPtcRight();

}

//=== end PulseOnHeaterPtc =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::PulseOffHeaterPtcLeft(void)
{
	this->HalHeaters->PulseOffHeaterPtcLeft();
}
void TTaskCHM::PulseOffHeaterPtcRight(void)
{
	this->HalHeaters->PulseOffHeaterPtcRight();
}
//=== end PulseOffHeaterPtc ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EHeaterPwm TTaskCHM::GetPwmHeaterPadLeft(void)
{
	this->HalHeaters->GetPwmHeaterPadLeft();
}
EHeaterPwm TTaskCHM::GetPwmHeaterPadRight(void)
{
	this->HalHeaters->GetPwmHeaterPadRight();
}

//=== end GetPwmHeaterPad ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::PulseOnHeaterPadLeft(void)
{
	this->HalHeaters->PulseOnHeaterPadLeft();
}
void TTaskCHM::PulseOnHeaterPadRight(void)
{
	this->HalHeaters->PulseOnHeaterPadRight();
}
//=== end PulseOnHeaterPad =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::PulseOffHeaterPadLeft(void)
{
	this->HalHeaters->PulseOffHeaterPadLeft();

}
void TTaskCHM::PulseOffHeaterPadRight(void)
{
	this->HalHeaters->PulseOffHeaterPadRight();
}
//=== end PulseOffHeaterPad ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/

// TODO Seperate into two functions, or just use parameters
void TTaskCHM::StartForwardMotorChamber()
{
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalMotChambers->StartForwardMotorLeft();
	}
	else
	{
		this->HalMotChambers->StartForwardMotorRight();
	}
}
//=== end StartForwardMotorChamber =================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StartBackwardMotorChamber()
{
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalMotChambers->StartBackwardMotorLeft();
	}
	else
	{
		this->HalMotChambers->StartBackwardMotorRight();
	}
}
//=== end StartBackwardMotorChamber ================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StopMotorChamber()
{
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalMotChambers->StopMotorLeft();
	}
	else
	{
		this->HalMotChambers->StopMotorRight();
	}
}
//=== end StopMotorChamber =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StopProcess()
{
	this->StopMotorChamber();
	this->StopHeaterPtc();
	this->StopHeaterPad();
	this->StopFanPtc();
	this->StopFanAir();
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
	    	this->SetEvents(TASK_CHM_EVENT_STOP_PROCESS);
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

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
//==================================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::SetPtcTemperatureLeft(s8 temperature)
{
	this->ptcTemperature = temperature;
}
void TTaskCHM::SetPtcTemperatureRight(s8 temperature)
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
s8 TTaskCHM::GetPtcTemperatureLeft(void)
{
	return(this->ptcTemperature);
}

s8 TTaskCHM::GetPtcTemperatureRight(void)
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
//void TTaskCHM::SetPtcTime(u32 repeatTime, u32 workTime)
//{
//	this->ptcRepeatTime = repeatTime;
//	this->ptcWorkTime = workTime;
//}
//=== end SetPtcTime ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::SetPadTemperatureLeft(s8 temperature)
{
	this->padTemperatureLeft = temperature;
}
void TTaskCHM::SetPadTemperatureRight(s8 temperature)
{
	this->padTemperatureRight = temperature;
}

//=== end SetPadTemperature ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
s8 TTaskCHM::GetPadTemperatureLeft(void)
{
	return(this->padTemperatureLeft);
}
s8 TTaskCHM::GetPadTemperatureRight(void)
{
	return(this->padTemperatureRight);
}

//=== end GetPadTemperature ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
//void TTaskCHM::SetPadTemperatureLevels(s8 lowLevel, s8 highLevel)
//{
//	this->padLowLevel_T = lowLevel;
//	this->padHighLevel_T = highLevel;
//}
//=== end SetPadTemperatureLevels ==================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
//void TTaskCHM::SetPadTime(u16 repeatTime, u16 workTime)
//{
//	this->padRepeatTime = repeatTime;
//	this->padWorkTime = workTime;
//}
//=== end SetPadTime ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
//void TTaskCHM::StartMixingTimeMode1()
//{
//	this->mixingCounterTimeMode1 = TASK_CHM_TIME_MIXING_MODE1;
//	this->mixingCounterRepeatTime = TASK_CHM_REPEAT_TIME_MIXING_MODE1;
//}
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
void TTaskCHM::SetError()
{
	this->error = true;
}
//=== end SetError =================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskCHM::Init(ETaskChamber taskChamber)
{
	this->taskChmState = TaskChmState_Idle;

	static THalDcFans& halF = THalDcFans::GetInstance();
	this->HalDcFans= &halF;

	static THalHeaters& halH = THalHeaters::GetInstance();
	this->HalHeaters= &halH;

	static THalMotChambers& halMC = THalMotChambers::GetInstance();
	this->HalMotChambers= &halMC;

	this->mixCounterIntervalTime = 0;
	this->mixIntervalTime = TASK_SYS_15_MINUTES;
	this->mixingPhase = MixingPhase_0;
	this->error = false;


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
