/**********************************************************************************/
/**
*  @file
*  TaskHAL.hpp - declaration of class TTaskHAL.
*
*  Target:  ...
*/

/**********************************************************************************/
#ifndef __TaskHAL_H
#define __TaskHAL_H

/**********************************************************************************/
#include "OsTask.hpp"
#include "Bme688.hpp"
#include "Fan.hpp"
#include "Motor.hpp"
#include "InterfaceVIP.hpp"


/**********************************************************************************/
#define TASK_HAL_CMD_START    (1<<0)




#define TASK_HAL_EVENT_GET_BME688_FAN    (1<<1)
#define TASK_HAL_EVENT_GET_BME688_LEFT   (1<<2)
#define TASK_HAL_EVENT_GET_BME688_RIGHT  (1<<3)




/**********************************************************************************/
#define TASK_HAL_AC_TIME_MEASUREMENT  100        // 0.1 Sec
#define TASK_HAL_AC_PULSE_NUMBERS     (12 - 1)   // 60*2 times per 1 Sec

#define TASK_HAL_TIME_FAN_RPM  1000  // 1 Sec


/**********************************************************************************/
enum EHalTurn
{
	HalTurn_Off = 0x00,
	HalTurn_On = 0x01,
};

enum EHalLamp
{
	HalLamp_Front = 0x01,
	HalLamp_Back = 0x02,
};

enum EGpioLevel
{
	GpioLevel_Low,
	GpioLevel_High,
};



/**********************************************************************************/
//==================================================================================
class TTaskHAL : public TOsTask
{
public:
	////// variables //////

//	TMotor Fan;
//	TMotor MotorDc;
//	TMotor Heater;


	////// constants //////



	////// functions //////
	EOsResult Init(void);
	void StartFan(u8 pwm);
	void StopFan(void);
	u8 GetFanPwm(void);
	u16 GetFanRpm(void);
	TBme688Sensors* GetPointerBme688Sensors(EIfcBme688Sensor ifcBme688Sensor);
	void ControlLamp(EHalLamp halLamp, EHalTurn halTurn);
	EHalTurn GetLampState(EHalLamp halLamp);
	EGpioLevel ReadLockLeft(void);
	EGpioLevel ReadLockRight(void);
	EGpioLevel ReadDamSensorLeft(void);
	EGpioLevel ReadDamSensorRight(void);
	void DamMotorStartForward(void);
	void DamMotorStartBackward(void);
	void DamMotorStop(void);
	TIfcSystemState* GetPointerIfcSystemState(void);

	void HandlerGpioInterrupt(u16 gpioPin);



	s8 GetTemperatureCpu2(void);
	s8 GetTemperatureCpu3(void);


	void SetEventTickFromISR(void);

	void CreateTask(void);
	void CreateTaskStatic(void);


  
protected:
	////// variables //////


	////// constants //////


	////// functions //////


  
private:
	////// variables //////
	StaticTask_t xTaskBuffer;
	StackType_t xStackBuffer[OS_TASK_HAL_SIZE_STACK];

	TBme688 Bme688_Left;
	bool flagErrorBme688_Left;
	TBme688 Bme688_Right;
	bool flagErrorBme688_Right;
	TBme688 Bme688_Fan;
	bool flagErrorBme688_Fan;

	EHalTurn halTurnLampFront;
	u8 counterLampFront;
	EHalTurn halTurnLampBack;
	u8 counterLampBack;

	TFan Fan;
	u16 rpmFan;
	u16 timeRpmFanCounter;

	s8 tCpu2;
	s8 tCpu3;

	u16 counterTimeAcMeasurement;

	TMotor DamMotor;
	TIfcSystemState IfcSystemState;
//	u16 switchStates;
//	u8 lampStates;
//	u8 levelTank;          // cm
//	u8 levelChamberLeft;   // cm
//	u8 levelChamberRight;  // cm


	////// constants //////


	////// functions //////
	void ProcessDataBme688(TBme688* pBme688);
	void UpdateHardwareStates(void);
	void SetSysStateDamMotor(u8 stateDmaMotor);
	void SetSysStateLamp(EHalLamp halLamp, u8 stateDmaMotor);
	void SetSysStateSensor(u16 typeSensor, u8 stateSensor);


	void Run(void);
	EOsResult CreateOsResources(void);


};
//=== end class TTaskHAL ===========================================================

/**********************************************************************************/
#endif
