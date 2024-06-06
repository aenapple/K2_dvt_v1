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
// const u8 TTaskCHM::tableCompostProcess[TASK_CHM_SIZE_TABLE_COMPOST_PROCESS][3] =
// {
//	{ 0x00, 35, (5 * 6) /* x10 */ },
//	{ 0x07, 40, (4 * 6) /* x10 */ },
//	{ 0x11, 45, (2 * 6) /* x10 */ },
//	{ 0x14, 50, 6 /* x10 */ },
//	{ 0x17, 55, 3 /* x10 */ },
//	{ 0x22, 45, (2 * 6) /* x10 */ },
//	{ 0x23, 40, (2 * 6) /* x10 */ },
// };

const u8 TTaskCHM::tableCompostProcess[TASK_CHM_SIZE_TABLE_COMPOST_PROCESS][TASK_CHM_NUM_PARAMETERS_COMPOST_PROCESS] =
{
	{ CycleStep_0m,		PadHeaterT_55degC,	TypeMixing_5m,		ModePtcHeater_Off,	ModePtcFan_On},
	{ CycleStep_30m,	PadHeaterT_Na,		TypeMixing_5m,		ModePtcHeater_Na,	ModePtcFan_Na},
	{ CycleStep_60m,	PadHeaterT_Na,		TypeMixing_5m,		ModePtcHeater_10m,	ModePtcFan_Na},
	{ CycleStep_80m,	PadHeaterT_Na,		TypeMixing_Na,		ModePtcHeater_10m,	ModePtcFan_Na},
	{ CycleStep_90m,	PadHeaterT_50degC,	TypeMixing_25s,		ModePtcHeater_Na,	ModePtcFan_Na},
	{ CycleStep_105m,	PadHeaterT_Na,		TypeMixing_25s,		ModePtcHeater_Na,	ModePtcFan_Na},
	{ CycleStep_120m,	PadHeaterT_Na,		TypeMixing_5m,		ModePtcHeater_10m,	ModePtcFan_Na},
	{ CycleStep_150m,	PadHeaterT_45degC,	TypeMixing_Na,		ModePtcHeater_Na,	ModePtcFan_Na},
	{ CycleStep_165m,	PadHeaterT_Na,		TypeMixing_25s,		ModePtcHeater_Na,	ModePtcFan_Na},
	{ CycleStep_180m,	PadHeaterT_Na,		TypeMixing_Na,		ModePtcHeater_5m,	ModePtcFan_Na},
	{ CycleStep_210m,	PadHeaterT_40degC,	TypeMixing_Na,		ModePtcHeater_Na,	ModePtcFan_Na},
	{ CycleStep_240m,	PadHeaterT_Na,		TypeMixing_5m_5h,	TypeMixing_5m_5h,	ModePtcFan_Na},
	{ CycleStep_360m,	PadHeaterT_Na,		TypeMixing_Na, 		ModePtcHeater_Na,	ModePtcFan_1h_2h},
};

void TTaskCHM::MaxHumidityDifference(){




//	def GetMaxHumidityVariation(humidity_collection_time, curr_humidity):
/*
 *     global maxHumidityDiff
    global curr_min_humidity
    global curr_max_humidity

    if humidity_collection_time > 0:
        humidity_collection_time = humidity_collection_time - increment




        match mix_phase:
            case 0:
                curr_min_humidity = min(curr_min_humidity, curr_humidity)
            case _:
                curr_max_humidity = max(curr_max_humidity, curr_humidity)
    else:
        maxHumidityDiff  = curr_max_humidity - curr_min_humidity
        curr_min_humidity = 100
        curr_max_humidity = 0
        humidity_collection_time = mixing_interval + mixing_counter


    return humidity_collection_time
 *
 */
}


void TTaskCHM::BmeControlParams(u16 temperature, u16 rHumidity)
{


	if (this->avgHumidity == 0)
			rHumidity = this->avgHumidity;
		else
			rHumidity = this->maxRelativeHumidity;


		if (temperature <= this->bmeLowTemp){

			if (rHumidity >= this->highHumidity)
			{
				this->mixIntervalTime = TASK_SYS_2_MINUTES;
				this->ptcHeaterPwm    = 100;
				this->ptcFanPwm       = 8;
				this->exhaustFanPwm   = 10;

			} else if (rHumidity >= this->medHumidity) {
				this->mixIntervalTime = TASK_SYS_4_MINUTES;
				this->ptcHeaterPwm    = 100;
				this->ptcFanPwm       = 5;
				this->exhaustFanPwm   = 0;


			} else {
				this->mixIntervalTime = TASK_SYS_12_MINUTES;
				this->ptcHeaterPwm    = 50;
				this->ptcFanPwm       = 2;
				this->exhaustFanPwm   = 0;
			}

		} else if (temperature  <= this->bmeMedTemp) {
			this->dutyCycle =   DutyCycleMode_0;

			if (rHumidity >=        this->highHumidity)
			{
				this->mixIntervalTime = TASK_SYS_3_MINUTES;
				this->ptcHeaterPwm    = 100;
				this->ptcFanPwm       = 5;
				this->exhaustFanPwm   = 10;

			} else if (rHumidity >= this->medHumidity) {
				this->mixIntervalTime = TASK_SYS_10_MINUTES;
				this->ptcHeaterPwm    = 100;
				this->ptcFanPwm       = 4;
				this->exhaustFanPwm   = 0;

			} else if (rHumidity >= this->lowHumidity) {
				this->mixIntervalTime = TASK_SYS_15_MINUTES;
				this->ptcHeaterPwm    = 50;
				this->ptcFanPwm       = 2;
				this->exhaustFanPwm   = 0;

			} else {
				this->dutyCycle =   DutyCycleMode_1;


			}
		} else if (temperature  <= this->bmeHighTemp) {
			this->dutyCycle =   DutyCycleMode_0;

			if (rHumidity >=        this->highHumidity)
			{
				this->mixIntervalTime = TASK_SYS_4_MINUTES;
				this->ptcHeaterPwm    = 100;
				this->ptcFanPwm       = 5;
				this->exhaustFanPwm   = 10;

			} else if (rHumidity >= this->medHumidity) {
				this->mixIntervalTime = TASK_SYS_10_MINUTES;
				this->ptcHeaterPwm    = 100;
				this->ptcFanPwm       = 1;
				this->exhaustFanPwm   = 0;
			} else if (rHumidity >= this->lowHumidity) {
				this->dutyCycle =   DutyCycleMode_2;

			} else {
				this->dutyCycle =   DutyCycleMode_3;

			}
		} else {
			this->dutyCycle =   DutyCycleMode_3;
		}

}

void TTaskCHM::SetStepDutyCycles()
{

		switch(this->dutyCycle)
		{
			case DutyCycleMode_0:
				if (this->ptcDutyCycleOnFlag) {
					ptcIntervalTime = 0;
					ptcCounterWorkTime = 0;
				}
				break;

			case DutyCycleMode_99:

				if (this->ptcDutyCycleOnFlag) {
					this->ptcDutyCycleOnFlag = false;

					this->ptcHeaterPwm  = 0;
					this->ptcFanPwm     = 0;
					this->filterFanPwm  = 100;
					this->exhaustFanPwm = 50;

					this->ptcIntervalTime = TASK_SYS_2_MINUTES;
					this->ptcCounterWorkTime = this->ptcIntervalTime;
				} else {
					this->ptcDutyCycleOnFlag = true;

					this->ptcHeaterPwm  = 0;
					this->ptcFanPwm     = 0;
					this->filterFanPwm  = 100;
					this->exhaustFanPwm = 50;

					this->ptcIntervalTime = TASK_SYS_2_MINUTES;
					this->ptcCounterWorkTime = this->ptcIntervalTime;
				}
				break;


			case DutyCycleMode_1:
				if (this->ptcDutyCycleOnFlag) {
					this->ptcDutyCycleOnFlag = false;

					this->ptcHeaterPwm  = 0;
					this->ptcFanPwm     = 0;
					this->filterFanPwm  = 100;
					this->exhaustFanPwm = 50;

					this->ptcIntervalTime = TASK_SYS_2_MINUTES;
					this->ptcCounterWorkTime = this->ptcIntervalTime;
				} else {
					this->ptcDutyCycleOnFlag = true;

					this->ptcHeaterPwm  = 0;
					this->ptcFanPwm     = 0;
					this->filterFanPwm  = 100;
					this->exhaustFanPwm = 50;

					this->ptcIntervalTime = TASK_SYS_2_MINUTES;
					this->ptcCounterWorkTime = this->ptcIntervalTime;
				}
				break;

			case DutyCycleMode_2:
				if (this->ptcDutyCycleOnFlag) {
					this->ptcDutyCycleOnFlag = false;

					this->ptcHeaterPwm  = 0;
					this->ptcFanPwm     = 0;
					this->filterFanPwm  = 100;
					this->exhaustFanPwm = 50;

					this->ptcIntervalTime = TASK_SYS_2_MINUTES;
					this->ptcCounterWorkTime = this->ptcIntervalTime;
				} else {
					this->ptcDutyCycleOnFlag = true;

					this->ptcHeaterPwm  = 0;
					this->ptcFanPwm     = 0;
					this->filterFanPwm  = 100;
					this->exhaustFanPwm = 50;

					this->ptcIntervalTime = TASK_SYS_2_MINUTES;
					this->ptcCounterWorkTime = this->ptcIntervalTime;
				}
				break;

			case DutyCycleMode_3:
				if (this->ptcDutyCycleOnFlag) {
					this->ptcDutyCycleOnFlag = false;

					this->ptcHeaterPwm  = 0;
					this->ptcFanPwm     = 0;
					this->filterFanPwm  = 100;
					this->exhaustFanPwm = 50;

					this->ptcIntervalTime = TASK_SYS_2_MINUTES;
					this->ptcCounterWorkTime = this->ptcIntervalTime;
				} else {
					this->ptcDutyCycleOnFlag = true;

					this->ptcHeaterPwm  = 0;
					this->ptcFanPwm     = 0;
					this->filterFanPwm  = 100;
					this->exhaustFanPwm = 50;

					this->ptcIntervalTime = TASK_SYS_2_MINUTES;
					this->ptcCounterWorkTime = this->ptcIntervalTime;
				}
				break;

		}


}

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

	if (this->filterFanPwm >= 100)
		this->filterFanPwm = 100;
	 else if (this->filterFanPwm <= 0)
		this->filterFanPwm = 0;

	if (this->exhaustFanPwm >= 100)
		this->exhaustFanPwm = 100;
	 else if (this->exhaustFanPwm <= 0)
		this->exhaustFanPwm = 0;

	if (this->filterFanPwm >= 100)
		this->filterFanPwm = 100;
	 else if (this->filterFanPwm <= 0)
		this->filterFanPwm = 0;
}


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
//	EOsResult result;


	this->Delay(10);

	this->taskChmState = taskChmState;
//	this->SetEvents(TASK_CHM_EVENT_MIXING);
//	this->ptcCounterRepeatTime = 0;
//	this->ptcCounterWorkTime = this->ptcWorkTime;
//	this->flagPtcOn = true;
//		this->SetPtcTemperatureLevels(50, 55);
//		this->SetPadTemperatureLevels(53, 55);


	this->samplingCounter = TASK_SYS_20_MINUTES;

	this->mixingPhase = MixingPhase_0;
	this->mixIntervalTime = TASK_SYS_2_MINUTES;
	this->mixCounterWorkTime =  TASK_SYS_20_SECONDS;

	this->SetEvents(TASK_CHM_EVENT_MIXING);

	this->modePtcFan = ModePtcFan_1h_2h;
	this->StartFanPtc(50);
	this->ptcFanCounterRepeatTime = TASK_SYS_2_HOURS;
	this->ptcFanCounterWorkTime = TASK_SYS_1_HOUR;

	this->lowHumidity  = TASK_CHM_LOW_HUMIDITY;
	this->medHumidity  = TASK_CHM_MED_HUMIDITY;
	this->highHumidity = TASK_CHM_HIGH_HUMIDITY;

	this-> humiditySampleCounter =	this->samplingCounter;

	this->maxGas = TASK_CHM_MAX_GAS;
	this->minGas = TASK_CHM_MIN_GAS;

	this->bmeLowTemp   = TASK_CHM_LOW_TEMP;
	this->bmeMedTemp   = TASK_CHM_MED_TEMP;
	this->bmeHighTemp  = TASK_CHM_HIGH_TEMP;

	this->ptcDutyCycleOnFlag = false;

	this->dutyCycle = DutyCycleMode_0;

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
//        	DiagNotice("Working");

            continue;
        }


        if((resultBits & TASK_CHM_EVENT_TICK_PROCESS) > 0)
        {
        	this->TickProcess();
        }

        if((resultBits & TASK_CHM_EVENT_MIXING) > 0)
        {
//        	this->Mixing();
        	this->ClearEvents(TASK_CHM_EVENT_MIXING);
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

void TTaskCHM::GetSensorBme688(EIfcBme688Sensor ifcBme688Sensor)
{
	u8* pData;
	pData = this->InterfaceMasterVIP.GetPointerDataRx();
	taskENTER_CRITICAL();
	memcpy((void*)&this->bmeSensorFan, (void*)(pData + IFC_VIP_BME688_TEMPERATURE), sizeof(TBme688Sensor));
	taskEXIT_CRITICAL();
}
void TTaskCHM::GetSensorBme688()
{
	u8* pData;
	pData = this->InterfaceMasterVIP.GetPointerDataRx();
	taskENTER_CRITICAL();
	memcpy((void*)&this->bmeSensorChamber, (void*)((pData + IFC_VIP_BME688_TEMPERATURE)), sizeof(TBme688Sensor));
	taskEXIT_CRITICAL();
}


void TTaskCHM::TickProcess()
{
//	s8 padTemp;
//	s8 ptcTemp;
//
//	padTemp = this->GetPadTemperature();
//	ptcTemp = this->GetPtcTemperature();


	this->GetSensorBme688(IfcBme688Sensor_Fan);
	this->GetSensorBme688();

	this->BmeControlParams(bmeSensorChamber.temperature, bmeSensorChamber.humidity);


	////// Mixing counter. //////
	if (this->mixCounterWorkTime > 0){
			this->mixCounterWorkTime--; // decrement according to time

	} else {
		this->Mixing();
	}

	////// PTC Fan & Heater counter. For Duty Cycle	 //////
	if (this->ptcCounterWorkTime > 0) {
		this->ptcCounterWorkTime--; // decrement according to time

	} else {
		this->SetStepDutyCycles();
	}


	if (this->humiditySampleCounter > 0) {
		this->humiditySampleCounter--;

		switch (this->mixingPhase) {
			case MixingPhase_0:
//	            curr_min_humidity = min(curr_min_humidity, curr_humidity)
				break;

			case MixingPhase_1:
			case MixingPhase_2:
			case MixingPhase_3:
			case MixingPhase_4:

				break;
//				curr_max_humidity = max(curr_max_humidity, curr_humidity)
//	            case _:


		}

	} else {

	}



	this->ActuatorPWMCheck();


	if (this->ptcFanPwm > 0) {
		this->StartFanPtc(this->ptcFanPwm);
	} else {
		this->StopFanPtc();
	}

	if (this->ptcHeaterPwm > 0) {
		this->StartHeaterPtc(this->ptcHeaterPwm);
	} else {
		this->StopHeaterPtc();
	}

	if (this->exhaustFanPwm > 0) {
		TaskHAL.StartMainFan(exhaustFanPwm);
 	} else {
		TaskHAL.StopMainFan();
 	}

	if (this->padHeaterPwm) {
		this->StartHeaterPad(padHeaterPwm);
	} else {
		this->StopHeaterPad();
	}

	// Activate filter fan



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
	u16 timeCw;
	u16 timeCcw;

	switch(mixingPhase) {

		case MixingPhase_0:
			this->StartForwardMotorChamber();
			this->mixCounterWorkTime = this->timeCw;

			this->mixingPhase = MixingPhase_1;
			break;

		case MixingPhase_1:
			result = this->DelaySecond(this->timeCw);
			this->StopMotorChamber();
			if(result == OsResult_StopProcess)
			{
				return;
			}

			this->StartBackwardMotorChamber();
			this->mixCounterWorkTime = this->timeCcw;
			this->mixingPhase = MixingPhase_2;
			break;


		case MixingPhase_2:
			result = this->DelaySecond(this->timeCcw);
			this->StopMotorChamber();
			if(result == OsResult_StopProcess)
			{
				return;
			}
			this->StartForwardMotorChamber();
			this->mixCounterWorkTime = this->timeCw;

			this->mixingPhase = MixingPhase_3;
			break;

		case MixingPhase_3:
			result = this->DelaySecond(this->timeCw);
			this->StopMotorChamber();
			if(result == OsResult_StopProcess)
			{
				return;
			}

			this->StartBackwardMotorChamber();
			this->mixCounterWorkTime = this->timeCcw;


			this->mixingPhase = MixingPhase_4;
			break;

		case MixingPhase_4:
			result = this->DelaySecond(this->timeCcw);
			this->StopMotorChamber();
			if(result == OsResult_StopProcess)
			{
				return;
			}

			this->mixingPhase = MixingPhase_0;
			break;

	}



}
//=== end Mixing ===================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::StartFanPtc(u8 pwm)
{
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalDcFans->StartFanPtcLeft(pwm);
	}
	else
	{
		this->HalDcFans->StartFanPtcRight(pwm);
	}
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
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalDcFans->StopFanPtcLeft();
	}
	else
	{
		this->HalDcFans->StopFanPtcRight();
	}
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
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalHeaters->StartHeaterPtcLeft(pwm);
	}
	else
	{
		this->HalHeaters->StartHeaterPtcRight(pwm);
	}
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
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalHeaters->StopHeaterPtcLeft();
	}
	else
	{
		this->HalHeaters->StopHeaterPtcRight();
	}
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
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalHeaters->StopHeaterPadLeft();
	}
	else
	{
		this->HalHeaters->StopHeaterPadRight();
	}
}
//=== end StopHeaterPad ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EHeaterPwm TTaskCHM::GetPwmHeaterPtc(void)
{
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalHeaters->GetPwmHeaterPtcLeft();
	}
	else
	{
		this->HalHeaters->GetPwmHeaterPtcRight();
	}
}
//=== end GetPwmHeaterPtc ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::PulseOnHeaterPtc(void)
{
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalHeaters->PulseOnHeaterPtcLeft();
	}
	else
	{
		this->HalHeaters->PulseOnHeaterPtcRight();
	}
}
//=== end PulseOnHeaterPtc =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::PulseOffHeaterPtc(void)
{
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalHeaters->PulseOffHeaterPtcLeft();
	}
	else
	{
		this->HalHeaters->PulseOffHeaterPtcRight();
	}
}
//=== end PulseOffHeaterPtc ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EHeaterPwm TTaskCHM::GetPwmHeaterPad(void)
{
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalHeaters->GetPwmHeaterPadLeft();
	}
	else
	{
		this->HalHeaters->GetPwmHeaterPadRight();
	}
}
//=== end GetPwmHeaterPad ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::PulseOnHeaterPad(void)
{
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalHeaters->PulseOnHeaterPadLeft();
	}
	else
	{
		this->HalHeaters->PulseOnHeaterPadRight();
	}
}
//=== end PulseOnHeaterPad =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::PulseOffHeaterPad(void)
{
	if(this->taskChamber == TaskChamber_Left)
	{
		this->HalHeaters->PulseOffHeaterPadLeft();
	}
	else
	{
		this->HalHeaters->PulseOffHeaterPadRight();
	}
}
//=== end PulseOffHeaterPad ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
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
//void TTaskCHM::StartCycleCompostProcess(void)
//{
//	this->SetStepCompostProcess(TASK_SYS_0_MINUTES);
//}
//=== end StartCycleCompostProcess =================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
//void TTaskCHM::SetStepCompostProcess(ECycleStep cycleStep)
//{
//	switch(cycleStep)
//	{
////		case CycleStep_0m:
////			break;
//
//		case CycleStep_30m:
//			break;
//
//		case CycleStep_60m:
//			break;
//
//		case CycleStep_80m:
//			break;
//
//		case CycleStep_90m:
//			break;
//
//		case CycleStep_105m:
//			break;
//
//		case CycleStep_120m:
//			break;
//
//		case CycleStep_150m:
//			break;
//
//		case CycleStep_165m:
//			break;
//
//		case CycleStep_180m:
//			break;
//
//		case CycleStep_210m:
//			break;
//
//		case CycleStep_240m:
//			break;
//
//		case CycleStep_360m:
//			break;
//
//		default: // case CycleStep_0m
//			this->counterCycleCompostProcess = TASK_SYS_0_MINUTES;
//	}
//
//}
//=== end SetStepCompostProcess ====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskCHM::SetStepCompostProcess()
{


}
//=== end SetStepCompostProcess ====================================================

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
//void TTaskCHM::SetTimeCompostProcess(ETimeCompostProcess timeCompostProcess)
//{
//	u16 newTime;
//
//
//	if(this->padHighLevel_T != this->tableCompostProcess[timeCompostProcess][1])
//	{
//		this->padLowLevel_T = this->tableCompostProcess[timeCompostProcess][1] - 2;
//		this->padHighLevel_T = this->tableCompostProcess[timeCompostProcess][1];
//	}
//
//	newTime = (u16)this->tableCompostProcess[timeCompostProcess][2] * TASK_SYS_10_MINUTES;
//#ifdef __DEBUG_TEST_MIXING_MECHANISM
//	newTime = TASK_SYS_10_MINUTES;
//#endif
//	if(this->mixingRepeatTime != newTime)
//	{
//		this->mixingRepeatTime = newTime;
//		this->mixingCounterRepeatTime = 0;
//	}
//
//}
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

	this->taskChamber = taskChamber;
	this->taskChmState = TaskChmState_Idle;

	static THalDcFans& halF = THalDcFans::GetInstance();
	this->HalDcFans= &halF;

	static THalHeaters& halH = THalHeaters::GetInstance();
	this->HalHeaters= &halH;

	static THalMotChambers& halMC = THalMotChambers::GetInstance();
	this->HalMotChambers= &halMC;


	if(taskChamber == TaskChamber_Left)
		{
	//		this->ptcHeater = Heater_PtcHeaterLeft;
	//		this->padHeater = Heater_PadHeaterLeft;
//			this->MotorChamber.Init(MotorChamber_Left);
//			this->PtcHeater.Init(Heater_PtcHeaterLeft);
//			this->PadHeater.Init(Heater_PadHeaterLeft);
//			this->PtcFan.Init(PtcFan_Left);

			this->timeCw = TASK_CHM_MIX_LONG;
			this->timeCcw = TASK_CHM_MIX_SHORT;

		}
		else
		{
	//		this->ptcHeater = Heater_PtcHeaterRight;
	//		this->padHeater = Heater_PadHeaterRight;
//			this->MotorChamber.Init(MotorChamber_Right);
//			this->PtcHeater.Init(Heater_PtcHeaterRight);
//			this->PadHeater.Init(Heater_PadHeaterRight);
//			this->PtcFan.Init(PtcFan_Right);

			this->timeCw =  TASK_CHM_MIX_SHORT;
			this->timeCcw = TASK_CHM_MIX_LONG;

		}

//	this->SetPadTemperature(TASK_CHM_PAD_LOW_LEVEL_T, TASK_CHM_PAD_HIGH_LEVEL_T);
//	this->SetPtcTemperature(TASK_CHM_PTC_LOW_LEVEL_T, TASK_CHM_PTC_HIGH_LEVEL_T);

//	this->mixingRepeatTime = TASK_CHM_REPEAT_TIME_MIXING;
//	this->mixingCounterRepeatTime = TASK_SYS_5_HOURS;

//	this->mixingMode = MixingMode_5m;

//	this->counterCycleCompostProcess = TASK_SYS_360_MINUTES;


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
