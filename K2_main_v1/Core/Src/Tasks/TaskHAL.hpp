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
#include "Adc.hpp"
#include "I2c.hpp"
#include "Gpio.hpp"
#include "InterfaceVIP.hpp"
#include "Heater.hpp"
#include "MotorMain.hpp"
#include "PtcFan.hpp"
#include "MotorChamber.hpp"


/**********************************************************************************/
#define TASK_HAL_CMD_START    (1<<0)

#define TASK_HAL_EVENT_UART_RX_TIMEOUT    (1<<1)
#define TASK_HAL_EVENT_UART_RX_CPLT       (1<<2)
#define TASK_HAL_EVENT_UART_ERROR         (1<<3)
#define TASK_HAL_EVENT_SYS_COMMAND        (1<<4)

// #define TASK_HAL_EVENT_GET_BME688  (1<<1)
// #define TASK_HAL_EVT_NEW_RPM_READY     (1<<2)
#define TASK_HAL_EVENT_T_READY     (1<<5)

#define TASK_HAL_CMD_SELF_TEST  (1<<6)


#define TASK_HAL_MAX_NUMBER_COMMANDS  10
#define TASK_HAL_SIZE_PARAM_COMMANDS  (1 + IFC_VIP_UART_SIZE_DATA)  // command 1 byte + data 14 bytes




/**********************************************************************************/
#define HAL_ADC_NUMBER_CONVERSION  10  // time - 10 mS
#define HAL_T_NUMBER_CONVERSION    (1000 / HAL_ADC_NUMBER_CONVERSION)  // time - 1000 mS

#define TASK_HAL_NUMBER_T_SENSORS  4

#define TASK_HAL_AC_MAX_PWM_HEATER  10

#define TASK_HAL_AC_TIME_MEASUREMENT  100        // 0.1 Sec
#define TASK_HAL_AC_PULSE_NUMBERS     (12 - 1)   // 60*2 times per 1 Sec


/**********************************************************************************/
///// VREF = 2.5V - Heater temperature sensor //////
#define TASK_HAL_TH_0_DEG_C   3579  // 2.185V
#define TASK_HAL_TH_5_DEG_C   3454  // 2.109V
#define TASK_HAL_TH_10_DEG_C  3312  // 2.022V
#define TASK_HAL_TH_15_DEG_C  3151  // 1.924V
#define TASK_HAL_TH_20_DEG_C  2974  // 1.816V
#define TASK_HAL_TH_25_DEG_C  2786  // 1.701V
#define TASK_HAL_TH_30_DEG_C  2586  // 1.579V
#define TASK_HAL_TH_35_DEG_C  2381  // 1.454V
#define TASK_HAL_TH_40_DEG_C  2175  // 1.328V
#define TASK_HAL_TH_45_DEG_C  1972  // 1.204V
#define TASK_HAL_TH_50_DEG_C  1777  // 1.085V
#define TASK_HAL_TH_55_DEG_C  1592  // 0.972V
#define TASK_HAL_TH_60_DEG_C  1418  // 0.866V
#define TASK_HAL_TH_65_DEG_C  1256  // 0.767V
#define TASK_HAL_TH_70_DEG_C  1112  // 0.679V
#define TASK_HAL_TH_75_DEG_C  981   // 0.599V


/**********************************************************************************/
typedef enum
{
	SysCommand_ControlHeater,
	SysCommand_ControlMotor,
	SysCommand_ControlLamp,
	SysCommand_SetPosition,
	SysCommand_AcPowerOn,
	SysCommand_AcPowerOff,

	SysCommand_Last,

} ESysCommand;

struct TSysCommand
{
	ESysCommand command;
    u8 parameters[IFC_VIP_UART_SIZE_DATA];
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
	EOsResult SendSysCommand(TSysCommand* sysCommand);
	u16 GetAdcValue(EAdcChannel adcChannel);
	s8 GetTemperature(EIfcVipTemperature ifcVipTemperature);
	void TurnOnHeater(EIfcVipHeater ifcVipHeater, EHeaterPwm heaterPwm);


	void HandlerGpioInterrupt(u16 gpioPin);

	void SetEventUart2_TxCpltFromISR(void);
	void SetEventUart2_RxCpltFromISR(void);
	void SetEventUart2_ErrorFromISR(void);


	s8 GetTemperatureCpu2(void)
	{
		return(this->tCpu2);
	}
	s8 GetTemperatureCpu3(void)
	{
		return(this->tCpu3);
	}
	s8 GetTemperaturePtcLeft(void)
	{
		return(this->tPtcLeft);
	}
	s8 GetTemperaturePtcRight(void)
	{
		return(this->tPtcRight);
	}
	s8 GetTemperaturePadLeft(void)
	{
		return(this->tPadLeft);
	}
	s8 GetTemperaturePadRight(void)
	{
		return(this->tPadRight);
	}


	void SetEventTickFromISR(void);
	void SetEventAdcConvCpltFromISR(ADC_HandleTypeDef*);
	void SetEventAdcErrorFromISR(ADC_HandleTypeDef*);

	void CreateTask(void);
	void CreateTaskStatic(void);

	// DEBUG
	void AcPowerOn(void);
	void AcPowerOff(void);
	TMotorMain MotorMain;
	void StartMainMotorCW(void);
	void StartMainMotorCCW(void);
	void StopMainMotor(void);
	void BrakeOnMainMotor(void);
	void BrakeOffMainMotor(void);

	void TurnOnHeater(EHeater heater, EHeaterPwm heaterPwm);
	void TurnOffHeater(EHeater heater);

	TPtcFan PtcFanLeft;
	TPtcFan PtcFanRight;

	THeater PtcHeaterLeft;
	THeater PtcHeaterRight;
	THeater PadHeaterLeft;
	THeater PadHeaterRight;

	TMotorChamber MotorChamberLeft;
	TMotorChamber MotorChamberRight;
	// DEBUG


  
protected:
	////// variables //////


	////// constants //////


	////// functions //////


  
private:
	////// variables //////
	StaticTask_t xTaskBuffer;
	StackType_t xStackBuffer[OS_TASK_HAL_SIZE_STACK];

	TOsQueue OsQueue;
	TInterfaceVIP InterfaceMasterVIP;
	TAdc Adc;
	TI2c I2c;
	TGpio Gpio;
	u8 adcIndexConversion;
	u16 calculationResultAdc1[ADC1_MAX_NUMBER_CHANNEL];

	s8 tCpu2;
	s8 tCpu3;
	s8 tPtcLeft;
	s8 tPtcRight;
	s8 tPadLeft;
	s8 tPadRight;

//	u8 pwmAcPadHeater;
//	u8 pwmAcPtcHeater;
	u8 counterPwmHeater;

	u16 counterTimeAcMeasurement;
	bool acPhase;
	u8 counterAcMain;
	bool flagAcMainPresent;
	bool flagAcMainTurnedOn;
	u8 counterAcMotor;
	bool flagAcMotorPresent;
	bool flagStartMainMotor;

	bool flagSentEventTopRemoved;
	bool flagSentEventTopPresent;
	bool flagSentEventLidOpen;
	bool flagSentEventLidClosed;
	bool flagTopUnlocked;


	u16 adcTPtcLeft;
	u32 accumulativeTPtcLeft;
	u16 adcTPtcRight;
	u32 accumulativeTPtcRight;
	u16 adcTPadLeft;
	u32 accumulativeTPadLeft;
	u16 adcTPadRight;
	u32 accumulativeTPadRight;
	u16 indexAccumulativeT;
	u16 lastTPtcLeft[2];
	u16 lastTPtcRight[2];
	u16 lastTPadLeft[2];
	u16 lastTPadRight[2];

/*	THeater PtcHeaterLeft;
	THeater PtcHeaterRight;
	THeater PadHeaterLeft;
	THeater PadHeaterRight; */


	////// constants //////


	////// functions //////
	void GetStateTopCpu(void);
	void GetSensorBme688(void);
	void ProcessSysCommand(void);
	void ProcessSelfTest(void);
	bool CheckTopRemoved(void);
	bool CheckTopRemovedFromISR();
	bool CheckLidOpen(void);
	bool CheckLidOpenFromISR(void);
	void AdcConversionComplete(void);
	void CalculatingTSensors(void);
	s8 CalculatingTSensorDigC(u16 localAdcTHeater, u16 adcLowLevel);
	s8 CalculatingTSensor(EIfcVipTemperature ifcVipTemperature);
	EOsResult SendCommand(EIfcVipCommand command, u8* pBuffer);
	void ProcessAcPhase(void);
	void ProcessHeater(void);

	EOsResult ControlMotor(u8* parameters);
	EOsResult ControlHeater(u8* parameters);
	EOsResult ControlLamp(u8* parameters);


	void Run(void);
	EOsResult CreateOsResources(void);


};
//=== end class TTaskHAL ===========================================================

/**********************************************************************************/
#endif
