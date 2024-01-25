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
#include "../Sys/OsTask.hpp"
#include "Heater.hpp"
#include "PtcFan.hpp"
#include "MotorChamber.hpp"


/**********************************************************************************/
#define TASK_CHM_CMD_START        (1<<0)

#define TASK_CHM_EVENT_START_COMPOSTING  (1<<2)
#define TASK_CHM_EVENT_START_COLLECTING  (1<<3)
#define TASK_CHM_EVENT_STOP_PROCESS      (1<<4)

#define TASK_CHM_EVENT_TICK_PROCESS  (1<<5)
#define TASK_CHM_EVENT_MIXING        (1<<6)

// #define TASK_CHM_FLAG_NOT_PRESENT    (1<<7)
// #define TASK_CHM_FLAG_PTC_HEATER_ON  (1<<8)


// #define TASK_CHM_EVENT_NEW_STATE      (1<<19)

// #define TASK_CHM_EVENT_ERROR  (1<<20)


/**********************************************************************************/
// #define TASK_CHM_PAD_LOW_LEVEL_T   50  // 50C
// #define TASK_CHM_PAD_HIGH_LEVEL_T  55  // 55C
// #define TASK_CHM_PTC_LOW_LEVEL_T   40  // 40C
// #define TASK_CHM_PTC_HIGH_LEVEL_T  45  // 45C

#define TASK_CHM_REPEAT_TIME_MIXING  (120 * 60)  // 120 Minutes


/**********************************************************************************/
typedef enum
{
	TaskChamber_Left,
	TaskChamber_Right,
} ETaskChamber;


typedef enum
{
	TaskChmState_Idle = 0,
	TaskChmState_Composting = 1,
	TaskChmState_Collecting = 2,
} ETaskChmState;


/**********************************************************************************/
//==================================================================================
class TTaskCHM : public TOsTask
{
public:
	////// variables //////
	// DEBUG
	TPtcFan PtcFan;
	THeater PtcHeater;
	THeater PadHeater;
	TMotorChamber MotorChamber;
	// DEBUG


	////// constants //////


	////// functions //////
	EOsResult Init(ETaskChamber taskChamber);
	EOsResult Init(void) { return(OsResult_Ok); }
	ETaskChmState GetState(void);
//	void SetState(u32 event);


	void SetPtcTemperature(s8 temperature);
	s8 GetPtcTemperature(void);
	void SetPtcTemperatureLevels(s8 lowLevel, s8 highLevel);
	void SetPtcTime(u32 repeatTime, u32 workTime);
	void SetPadTemperature(s8 temperature);
	s8 GetPadTemperature(void);
	void SetPadTemperatureLevels(s8 lowLevel, s8 highLevel);
	void SetPadTime(u16 repeatTime, u16 workTime);


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
	
//	ETaskChamber taskChamber;
	ETaskChmState taskChmState;

//	TPtcFan PtcFan;
//	THeater PtcHeater;
//	THeater PadHeater;
//	TMotorChamber MotorChamber;

	s8 ptcLowLevel_T;
	s8 ptcHighLevel_T;
	s8 ptcTemperature;
	u32 ptcRepeatTime;
	u32 ptcCounterRepeatTime;
	u32 ptcWorkTime;
	u32 ptcCounterWorkTime;
	bool flagPtcOn;
	s8 padLowLevel_T;
	s8 padHighLevel_T;
	s8 padTemperature;
	u16 padRepeatTime;
	u16 padCounterRepeatTime;
	u16 padWorkTime;
	u16 padCounterWorkTime;
	bool flagPadOn;
	u16 mixingCounterRepeatTime;


	////// constants //////


	////// functions //////
	void Process(ETaskChmState taskChmState);
	void TickProcess(void);
	void Mixing(void);
	void StopProcess(void);
	EOsResult StartMotorForward(u8 pwm);
	EOsResult StartMotorBackward(u8 pwm);
	EOsResult StopMotor(void);
	EOsResult DelaySecond(u16 seconds);


	void Run(void);
	EOsResult CreateOsResources(void);


};
//=== end class TTaskCHM ===========================================================

/**********************************************************************************/
#endif
