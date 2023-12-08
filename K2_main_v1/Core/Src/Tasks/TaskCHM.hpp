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


/**********************************************************************************/
#define TASK_CHM_CMD_START        (1<<0)

#define TASK_CHM_EVENT_START_COMPOSTING  (1<<2)
#define TASK_CHM_EVENT_START_COLLECTING  (1<<3)
#define TASK_CHM_EVENT_STOP_PROCESS      (1<<4)

#define TASK_CHM_EVENT_TICK_PROCESS  (1<<5)
#define TASK_CHM_EVENT_MIXING        (1<<6)

#define TASK_CHM_FLAG_NOT_PRESENT  (1<<7)


#define TASK_CHM_EVENT_NEW_STATE      (1<<19)

#define TASK_CHM_EVENT_ERROR  (1<<20)




/**********************************************************************************/
typedef enum
{
	TaskChmIndex_Left,
	TaskChmIndex_Right,
} ETaskChmIndex;


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


	////// constants //////


	////// functions //////
	EOsResult Init(ETaskChmIndex taskChmIndex);
	EOsResult Init(void) { return(OsResult_Ok); }
	ETaskChmState GetTaskState(void)
	{
		return(this->taskChmState);
	}
	void SetState(u32 event);
	ETaskChmState GetTaskChmState(void) { return(this->taskChmState); }


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
	
	ETaskChmIndex taskChmIndex;
	ETaskChmState taskChmState;

	s8 ptcHeaterLowLevel;
	s8 ptcHeaterHighLevel;
	s8 padHeaterLowLevel;
	s8 padHeaterHighLevel;


	////// constants //////


	////// functions //////
	void Process(ETaskChmState taskChmState);
	void TickProcess(void);
	void Mixing(void);
	void StopProcess(void);
	EOsResult StartMotorForward(u8 pwm);
	EOsResult StartMotorBackward(u8 pwm);
	EOsResult StopMotor(void);
	EOsResult Delay_IT(u32 time);


	void Run(void);
	EOsResult CreateOsResources(void);


};
//=== end class TTaskCHM ===========================================================

/**********************************************************************************/
#endif
