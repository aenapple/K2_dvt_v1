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
#include "Interfaces/InterfaceVIP.hpp"
#include "Heater.hpp"
#include "MotorMain.hpp"
// #include "PtcFan.hpp"
// #include "MotorChamber.hpp"
#include "Eeprom.hpp"


/**********************************************************************************/
#define TASK_HAL_CMD_START    (1<<0)

#define TASK_HAL_EVENT_UART_RX_TIMEOUT    (1<<1)
#define TASK_HAL_EVENT_UART_RX_CPLT       (1<<2)
#define TASK_HAL_EVENT_UART_ERROR         (1<<3)
#define TASK_HAL_EVENT_SYS_COMMAND        (1<<4)

#define TASK_HAL_EVENT_GET_BME688_FAN    (1<<5)
#define TASK_HAL_EVENT_GET_BME688_LEFT   (1<<6)
#define TASK_HAL_EVENT_GET_BME688_RIGHT  (1<<7)

// #define TASK_HAL_EVT_NEW_RPM_READY     (1<<2)
#define TASK_HAL_EVENT_T_READY     (1<<8)

#define TASK_HAL_CMD_SELF_TEST  (1<<9)


#define TASK_HAL_MAX_NUMBER_COMMANDS  10
#define TASK_HAL_SIZE_PARAM_COMMANDS  (1 + IFC_VIP_UART_SIZE_DATA)  // command 1 byte + data 14 bytes




/**********************************************************************************/
#define HAL_ADC_NUMBER_CONVERSION  10  // time - 10 mS
#define HAL_T_NUMBER_CONVERSION    (1000 / HAL_ADC_NUMBER_CONVERSION)  // time - 1000 mS

#define TASK_HAL_NUMBER_T_SENSORS  4

#define TASK_HAL_AC_MAX_PWM_HEATER  10

#define TASK_HAL_AC_TIME_MEASUREMENT  100        // 0.1 Sec
#define TASK_HAL_AC_PULSE_NUMBERS     (12 - 1)   // 60*2 times per 1 Sec

#define TASK_HAL_TIME_GET_BME688_FAN    1000  // 1 Sec
#define TASK_HAL_TIME_GET_BME688_LEFT   2000  // 2 Sec
#define TASK_HAL_TIME_GET_BME688_RIGHT  3000  // 3 Sec

/**********************************************************************************/
///// VREF = 2.5V - Pad Heater temperature sensor (10K) //////
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

///// VREF = 2.5V - PTC Heater temperature sensor (22K) //////
#define TASK_HAL_TP_0_DEG_C   3796  // 2.318V
#define TASK_HAL_TP_5_DEG_C   3733  // 2.279V
#define TASK_HAL_TP_10_DEG_C  3659  // 2.234V
#define TASK_HAL_TP_15_DEG_C  3574  // 2.182V
#define TASK_HAL_TP_20_DEG_C  3479  // 2.124V
#define TASK_HAL_TP_25_DEG_C  3372  // 2.059V
#define TASK_HAL_TP_30_DEG_C  3257  // 1.989V
#define TASK_HAL_TP_35_DEG_C  3130  // 1.911V
#define TASK_HAL_TP_40_DEG_C  2995  // 1.829V
#define TASK_HAL_TP_45_DEG_C  2853  // 1.742V
#define TASK_HAL_TP_50_DEG_C  2704  // 1.651V
#define TASK_HAL_TP_55_DEG_C  2552  // 1.558V
#define TASK_HAL_TP_60_DEG_C  2398  // 1.464V
#define TASK_HAL_TP_65_DEG_C  2242  // 1.369V
#define TASK_HAL_TP_70_DEG_C  2090  // 1.276V
#define TASK_HAL_TP_75_DEG_C  1939  // 1.184V


/**********************************************************************************/
typedef enum
{
	SysCommand_ControlHeater,
	SysCommand_ControlMotor,
	SysCommand_ControlLamp,
	SysCommand_ControlFan,
	SysCommand_SetPosition,
	SysCommand_AcPowerOn,
	SysCommand_AcPowerOff,

	SysCommand_Last,

} ESysCommand;

struct TSysCommand  // sizeof = 16 bytes
{
	ESysCommand command;
    u8 parameters[IFC_VIP_UART_SIZE_DATA];
    u8 reserved;
};

enum EBme688Sensor
{
	Bme688Sensor_Left,
	Bme688Sensor_Right,
	Bme688Sensor_Fan
};

struct TBme688Sensors  // sizeof = 16bytes.
{
	s16 temperature;
	u32 pressure;
	u16 humidity;
	u32 gasResistance;
	u16 status;
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
	void TurnOnHeater(EHeater heater, EHeaterPwm heaterPwm);
	void TurnOffHeater(EHeater heater);
	void StartMainFan(u8 pwm);
	void StopMainFan(void);
//	void TurnOnMotorChamber(EMotorChamber motorChamber, EDirMotorChamber dirMotorChamber, u8 pwm);
//	void TurnOffMotorChamber(EMotorChamber motorChamber);
	TIfcSystemState* GetPointerIfcSystemState(void);

	void HandlerGpioInterrupt(u16 gpioPin);

	void SetEventUart2_TxCpltFromISR(void);
	void SetEventUart2_RxCpltFromISR(void);
	void SetEventUart2_ErrorFromISR(void);


	s8 GetTemperatureCpu2(void);
	s8 GetTemperatureCpu3(void);
	s8 GetTemperaturePtcLeft(void);
	s8 GetTemperaturePtcRight(void);
	s8 GetTemperaturePadLeft(void);
	s8 GetTemperaturePadRight(void);


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



//	TPtcFan PtcFanLeft;
//	TPtcFan PtcFanRight;

//	THeater PtcHeaterLeft;
//	THeater PtcHeaterRight;
//	THeater PadHeaterLeft;
//	THeater PadHeaterRight;

//	TMotorChamber MotorChamberLeft;
//	TMotorChamber MotorChamberRight;

	TEeprom Eeprom;
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
	TIfcSystemState IfcSystemState;
	TAdc Adc;
	TI2c I2c;
	TGpio Gpio;
	u8 adcIndexConversion;
	u16 calculationResultAdc1[ADC1_MAX_NUMBER_CHANNEL];

	s8 tCpu2;
	s8 tCpu3;
//	s8 tPtcLeft;
//	s8 tPtcRight;
//	s8 tPadLeft;
//	s8 tPadRight;

//	u8 pwmAcPadHeater;
//	u8 pwmAcPtcHeater;
	u8 counterPwmHeater;

	u16 counterGetBme688;

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
	bool flagTopLocked;

	bool flagPresentTank;
	bool flagPresentChamberLeft;
	bool flagPresentChamberRight;


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
	EOsResult GetStateTopCpu(void);
	void GetSensorBme688(EIfcBme688Sensor ifcBme688Sensor);
	void ProcessSysCommand(void);
	void ProcessSelfTest(void);
	EOsResult CheckConnectionTopCpu(void);
	EOsResult CheckLockTop(void);
	bool CheckTopPresent(void);
	bool CheckTopRemoved(void);
	bool CheckTopRemovedFromISR();
	bool CheckLidOpen(void);
	bool CheckLidOpenFromISR(void);
	void AdcConversionComplete(void);
	void CalculatingTSensors(void);
	s8 CalculatingTSensorDigC(u16 localAdcTHeater, u16 adcLowLevel);
	s8 CalculatingTSensor(EIfcVipTemperature ifcVipTemperature);
	s8 CalculatingTPtcSensorDigC(u16 localAdcTHeater, u16 adcLowLevel);
	s8 CalculatingTPtcSensor(EIfcVipTemperature ifcVipTemperature);
	EOsResult SendCommand(EIfcVipCommand command, u8* pBuffer);
	void ProcessAcPhase(void);
	void ProcessHeater(void);
	void ReInitUart(void);

	EOsResult ControlMotor(u8* parameters);
	EOsResult ControlHeater(u8* parameters);
	EOsResult ControlLamp(u8* parameters);
	EOsResult ControlFan(u8* parameters);
	u8 SetSysStateSensor(u16 typeSensor);


	void Run(void);
	EOsResult CreateOsResources(void);


};
//=== end class TTaskHAL ===========================================================

/**********************************************************************************/
#endif
