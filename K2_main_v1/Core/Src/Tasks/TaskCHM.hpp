/**********************************************************************************/
/**
*  @file
*  TaskCHM.hpp - declaration of class TTaskCHM.
*
*  Target:  ...
*/

/**********************************************************************************/
#ifndef __TaskCHM_H
#define __TaskCHM_H

/**********************************************************************************/
#include <HalMotChambers.hpp>
// #include "../Sys/OsTask.hpp"
#include "HalDcFans.hpp"
#include "HalHeaters.hpp"
#include "HalMotChambers.hpp"
#include "Interfaces/InterfaceVIP.hpp"
#include "TaskSYS.hpp"
#include "TaskAI.hpp"


/**********************************************************************************/
#define TASK_CHM_CMD_START        (1<<0)

#define TASK_CHM_EVENT_START_COMPOSTING  (1<<2)
#define TASK_CHM_EVENT_START_COLLECTING  (1<<3)
#define TASK_CHM_EVENT_STOP_PROCESS      (1<<4)

#define TASK_CHM_EVENT_TICK_PROCESS      (1<<5)
#define TASK_CHM_EVENT_MIXING_LEFT       (1<<6)
#define TASK_CHM_EVENT_MIXING_RIGHT      (1<<7)

// #define TASK_CHM_FLAG_NOT_PRESENT    (1<<7)
// #define TASK_CHM_FLAG_PTC_HEATER_ON  (1<<8)


// #define TASK_CHM_EVENT_NEW_STATE      (1<<19)

// #define TASK_CHM_EVENT_ERROR  (1<<20)


/**********************************************************************************/
// #define TASK_CHM_PAD_LOW_LEVEL_T   50  // 50C
// #define TASK_CHM_PAD_HIGH_LEVEL_T  55  // 55C
// #define TASK_CHM_PTC_LOW_LEVEL_T   40  // 40C
// #define TASK_CHM_PTC_HIGH_LEVEL_T  45  // 45C

#define TASK_CHM_REPEAT_TIME_MIXING  (20 * 60)  // (120 * 60)  // 120 Minutes


#define TASK_CHM_REPEAT_TIME_MIXING_MODE1  (30 * TASK_SYS_1_MINUTE)  // 30 minutes
#define TASK_CHM_TIME_MIXING_MODE1         (3 * TASK_SYS_1_HOUR)     // 3 hours
#define TASK_CHM_REPEAT_TIME_MIXING_MODE2  (5 * TASK_SYS_1_HOUR)     // 5 hours

#define TASK_CHM_SIZE_TABLE_COMPOST_PROCESS       13
#define TASK_CHM_NUM_PARAMETERS_COMPOST_PROCESS   5
#define TASK_CHM_INDEX_TIME_COMPOST_PROCESS     0
#define TASK_CHM_INDEX_PAD_H_COMPOST_PROCESS    1
#define TASK_CHM_INDEX_MIXING_COMPOST_PROCESS   2
#define TASK_CHM_INDEX_PTC_H_COMPOST_PROCESS    3
#define TASK_CHM_INDEX_PTC_F_COMPOST_PROCESS    4

/**********************************************************************************/
#define TASK_CHM_NUM_PREDICTIONS 3


#define TASK_CHM_LOW_HUMIDITY	4500
#define TASK_CHM_MED_HUMIDITY	5000
#define TASK_CHM_HIGH_HUMIDITY	5500

#define TASK_CHM_LOW_TEMP		4500
#define TASK_CHM_MED_TEMP 		4600
#define TASK_CHM_HIGH_TEMP 		4900

#define TASK_CHM_LOW_MOISTURE      40
#define TASK_CHM_MEDIUM_MOISTURE   55
#define TASK_CHM_HIGH_MOISTURE     70


#define TASK_CHM_MAX_GAS		10.2e7
#define TASK_CHM_MIN_GAS		4.0e7

#define TASK_CHM_MIX_LONG 		120
#define TASK_CHM_MIX_SHORT		20

/**********************************************************************************/
////// Dynamic Algorithm //////
typedef enum
{
	PtcDutyCycleMode_0,
	PtcDutyCycleMode_1,
	PtcDutyCycleMode_2,
	PtcDutyCycleMode_3,
	PtcDutyCycleMode_4,
	PtcDutyCycleMode_5,
	PtcDutyCycleMode_6,
	PtcDutyCycleMode_7,

	PtcDutyCycleMode_99,

} EDutyCycleMode;
////// end Dynamic Algorithm //////



typedef enum
{
	TaskChamber_Left,
	TaskChamber_Right,
} ETaskChamber;

//typedef enum
//{
//	TypeMixing_Na,
//	TypeMixing_Off,
//	TypeMixing_25s,
//	TypeMixing_5m,
//	TypeMixing_5m_5h,
//} ETypeMixing;

//typedef enum
//{
//	ModePtcHeater_Na,
//	ModePtcHeater_Off,
//	ModePtcHeater_10m,
//	ModePtcHeater_5m,
//	ModePtcHeater_5m_5h,
//} EModePtcHeater;

//typedef enum
//{
//	ModePtcFan_Na,
//	ModePtcFan_Off,
//	ModePtcFan_On,    // 66% power
//	ModePtcFan_1h_2h,
//} EModePtcFan;

//typedef enum
//{
//	PadHeaterT_Na,
//	PadHeaterT_55degC,
//	PadHeaterT_50degC,
//	PadHeaterT_45degC,
//	PadHeaterT_40degC,
//} EPadHeaterT;

//typedef enum
//{
//	CycleStep_0m,
//	CycleStep_30m,
//	CycleStep_60m,
//	CycleStep_80m,
//	CycleStep_90m,
//	CycleStep_105m,
//	CycleStep_120m,
//	CycleStep_150m,
//	CycleStep_165m,
//	CycleStep_180m,
//	CycleStep_210m,
//	CycleStep_240m,
//	CycleStep_360m,
//
//} ECycleStep;

typedef enum
{
	TaskChmState_Idle = 0,
	TaskChmState_Composting = 1,
	TaskChmState_Collecting = 2,
} ETaskChmState;

//typedef enum
//{
//	MixingMode_5m,  //
//	MixingMode_25s,
//	MixingMode_5m_5h,
//} EMixingMode;

//typedef enum
//{
//	TimeCompostProcess_00 = 0,  // time 00:00
//	TimeCompostProcess_07 = 1,  // time 07:00
//	TimeCompostProcess_11 = 2,  // time 11:00
//	TimeCompostProcess_14 = 3,  // time 14:00
//	TimeCompostProcess_17 = 4,  // time 17:00
//	TimeCompostProcess_22 = 5,  // time 22:00
//	TimeCompostProcess_23 = 6,  // time 23:00
//
//	TimeCompostProcess_Last = 7
//
//} ETimeCompostProcess;

/**********************************************************************************/
//==================================================================================
class TTaskCHM : public TOsTask
{
public:
	////// variables //////
	// DEBUG
//	TMotorChamber MotorChamber;
	// DEBUG


	////// constants //////


	////// functions //////
	EOsResult Init(ETaskChamber taskChamber);
	EOsResult Init(void)  { return(OsResult_Ok); }
	ETaskChmState GetState(void);
	void UpdateSensorBme688(TBme688Sensor* leftBme688Sensor, TBme688Sensor* rightBme688Sensor);
//	void SetState(u32 event);

	void SetError(void);

	EHeaterPwm GetPwmHeaterPtcLeft(void);
	EHeaterPwm GetPwmHeaterPtcRight(void);

	void PulseOnHeaterPtcLeft(void);
	void PulseOnHeaterPtcRight(void);

	void PulseOffHeaterPtcLeft(void);
	void PulseOffHeaterPtcRight(void);

	EHeaterPwm GetPwmHeaterPadLeft(void);
	EHeaterPwm GetPwmHeaterPadRight(void);

	void PulseOnHeaterPadLeft(void);
	void PulseOnHeaterPadRight(void);

	void PulseOffHeaterPadLeft(void);
	void PulseOffHeaterPadRight(void);

//	void StartCycleCompostProcess(void);
//	void SetConfigCompostProcess(u8 hours);
//	void SetTimeCompostProcess(ETimeCompostProcess timeCompostProcess);

	void SetPtcTemperatureLeft(s8 temperature);
	void SetPtcTemperatureRight(s8 temperature);

	s8 GetPtcTemperatureLeft(void);
	s8 GetPtcTemperatureRight(void);

	void SetPtcTemperatureLevels(s8 lowLevel, s8 highLevel);
	void SetPtcTime(u32 repeatTime, u32 workTime);

	void SetPadTemperatureLeft(s8 temperature);
	void SetPadTemperatureRight(s8 temperature);

	s8 GetPadTemperatureLeft(void);
	s8 GetPadTemperatureRight(void);

	void SetPadTemperatureLevels(s8 lowLevel, s8 highLevel);
	void SetPadTime(u16 repeatTime, u16 workTime);
//	void StartMixingTimeMode1(void);




//	void SetEventTickProcessFromISR(void);


	void CreateTask(void);
	void CreateTaskStatic(void);



  
protected:
	////// variables //////


	////// constants //////


	////// functions //////


  
private:
	////// variables //////
	StackType_t xStackBuffer[OS_TASK_CHM_SIZE_STACK];
	
	ETaskChamber taskChamber;
	ETaskChmState taskChmState;

	THalDcFans* HalDcFans;
	THalHeaters* HalHeaters;
	THalMotChambers* HalMotChambers;

	bool error;

	s8 ptcLowLevel_T;
	s8 ptcHighLevel_T;
	s8 ptcTemperature;
	u32 ptcRepeatTime;
	u32 ptcCounterRepeatTime;
	u32 ptcWorkTime;


//	EModePtcHeater modePtcHeater;
//	EModePtcFan modePtcFan;


//
//	s8 padLowLevel_T;
//	s8 padHighLevel_T;

//	u16 padRepeatTime;
//	u16 padCounterRepeatTime;
//	u16 padWorkTime;
//	u16 padCounterWorkTime;
//	bool flagPadOn;

//	u16 mixingCounterRepeatTime;
//	u16 mixingRepeatTime;
//	u16 mixingCounterTimeMode1;
//	u16 counterCycleCompostProcess;
//	EMixingMode mixingMode;

	////// Dynamic Algorithm variables //////
	u16 incrementCounter;
	u16 incrementTime;

	u64 heatPurgeInterval;
	u64 heatPurgeCounter;
	bool isHeatPurge;

	u32 lowTemp;
	u32 highTemp;

	u32 lastLowTemp;

    u32 ptcIntervalTime;
    u32 ptcCounterWorkTime;
    bool ptcDutyCycleOnFlag;
    EDutyCycleMode ptcDutyCycle;

    u8 ptcFanPwm;
    u8 ptcHeaterPwm;

    // Fix for P50 TODO FIX
    s8 ptcTemperatureRight;
    s8 ptcTemperatureLeft;
    s8 padTemperatureRight;
    s8 padTemperatureLeft;



	u16 timeCw;
	u16 timeCcw;

//	float rightMoisturePredictions[TASK_CHM_NUM_PREDICTIONS] = {0,0,0}; //short time intervals
//	float leftMoisturePredictions[TASK_CHM_NUM_PREDICTIONS]  = {0,0,0};

	float leftMoisturePredictions;
	float rightMoisturePredictions;

    TBme688Sensor leftBmeSensorChamber;
    TBme688Sensor rightBmeSensorChamber;
    TBme688Sensor bmeSensorFan;



//	u16 mixCounterWorkTime;
    // Might need larger counting intervals depending on how large we make it.
	u16 mixCounterIntervalTime;
	u16 mixIntervalTime;

	EMixingPhase rightMixingPhase;
	EMixingPhase leftMixingPhase;


	////// end Dynamic Algorithm variables //////


	////// constants //////


	////// functions //////
	void Process(ETaskChmState taskChmState);
	void TickProcess(void);
	void Mixing(EMixingPhase mixingPhase);
	void StopProcess(void);
	void StartFanAir(u8 pwm);
	void StopFanAir(void);
	void StartFanPtc(u8 pwm);
	void StopFanPtc(void);
	void StartHeaterPtc(u8 pwm);
	void StopHeaterPtc(void);
	void StartHeaterPad(u8 pwm);
	void StopHeaterPad(void);

	void StartForwardMotorChamberLeft(void);
	void StartBackwardMotorChamberLeft(void);
	void StopMotorChamberLeft(void);


	void StartForwardMotorChamberRight(void);
	void StartBackwardMotorChamberRight(void);
	void StopMotorChamberRight(void);

	EOsResult DelaySecond(u16 seconds);///	void SetStepCompostProcess(ECycleStep cycleStep);///	void SetStepCompostProcess(u16 timeStep);

	////// Dynamic Algorithm Logic //////
//	void GetSensorBme688(void);

	void BmeControlParams(u16 temperature, u16 rHumidity);

	void ActuatorPWMCheck(void);

	void MixingLeft(void);
	void MixingRight(void);


	/// Modulate Duty Cycles
	void SetPtcDutyCycles(void);

	////// CHeck Chamber Conditions (change naming convention after)

	void DeterminePtcDutyCycle(float moistureChamberA, float moistureChamberB);
	void ModulatModulateChamberTemperatures(u16 leftChamberHumidity, u16 rightChamberHumidity);
	////// end Dynamic Algorithm Logic //////


	void Run(void);
	EOsResult CreateOsResources(void);


};
//=== end class TTaskCHM ===========================================================

/**********************************************************************************/
#endif
