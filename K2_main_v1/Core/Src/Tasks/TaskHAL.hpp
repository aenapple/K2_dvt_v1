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


/**********************************************************************************/
#define TASK_HAL_CMD_START    (1<<0)

#define TASK_HAL_EVENT_UART_RX_TIMEOUT    (1<<1)
#define TASK_HAL_EVENT_UART_RX_CPLT       (1<<2)
#define TASK_HAL_EVENT_UART_ERROR         (1<<3)
#define TASK_HAL_EVENT_SYS_COMMAND        (1<<4)

// #define TASK_HAL_EVENT_GET_BME688  (1<<1)
// #define TASK_HAL_EVT_NEW_RPM_READY     (1<<2)
#define TASK_HAL_EVENT_T_READY     (1<<5)


#define TASK_HAL_MAX_NUMBER_COMMANDS  10
#define TASK_HAL_SIZE_PARAM_COMMANDS  (1 + IFC_VIP_UART_SIZE_DATA)  // command 1 byte + data 14 bytes




/**********************************************************************************/
#define HAL_ADC_NUMBER_CONVERSION  10  // time - 10 mS
#define HAL_T_NUMBER_CONVERSION    (1000 / HAL_ADC_NUMBER_CONVERSION)  // time - 1000 mS

#define TASK_HAL_NUMBER_T_SENSORS  4

#define TASK_HAL_AC_MAX_PWM_HEATER  10

#define TASK_HAL_AC_TIME_MEASUREMENT  500  // 500 mSec
#define TASK_HAL_AC_PULSE_NUMBERS     8    // 8.3333333 times per 0.5 Sec


/**********************************************************************************/
///// VREF = 2.5V - Heater temperature sensor //////
#define TASK_HAL_TH_0_DEG_C   585   // 0.357V
#define TASK_HAL_TH_5_DEG_C   693   // 0.423V
#define TASK_HAL_TH_10_DEG_C  826   // 0.504V
#define TASK_HAL_TH_15_DEG_C  968   // 0.591V
#define TASK_HAL_TH_20_DEG_C  1119  // 0.683V
#define TASK_HAL_TH_25_DEG_C  1279  // 0.781V
#define TASK_HAL_TH_30_DEG_C  1448  // 0.884V
#define TASK_HAL_TH_35_DEG_C  1620  // 0.989V
#define TASK_HAL_TH_40_DEG_C  1794  // 1.095V
#define TASK_HAL_TH_45_DEG_C  1969  // 1.202V
#define TASK_HAL_TH_50_DEG_C  2140  // 1.306V
#define TASK_HAL_TH_55_DEG_C  2303  // 1.406V
#define TASK_HAL_TH_60_DEG_C  2461  // 1.502V
#define TASK_HAL_TH_65_DEG_C  2610  // 1.593V
#define TASK_HAL_TH_70_DEG_C  2749  // 1.678V
#define TASK_HAL_TH_75_DEG_C  2877  // 1.756V

///// VREF = 2.5V - other temperature sensors //////
#define TASK_HAL_TS_0_DEG_C   515   // 0.315V
#define TASK_HAL_TS_5_DEG_C   640   // 0.391V
#define TASK_HAL_TS_10_DEG_C  783   // 0.478V
#define TASK_HAL_TS_15_DEG_C  943   // 0.576V
#define TASK_HAL_TS_20_DEG_C  1120  // 0.684V
#define TASK_HAL_TS_25_DEG_C  1309  // 0.799V
#define TASK_HAL_TS_30_DEG_C  1509  // 0.921V
#define TASK_HAL_TS_35_DEG_C  1713  // 1.046V
#define TASK_HAL_TS_40_DEG_C  1918  // 1.171V
#define TASK_HAL_TS_45_DEG_C  2121  // 1.295V
#define TASK_HAL_TS_50_DEG_C  2316  // 1.414V
#define TASK_HAL_TS_55_DEG_C  2503  // 1.528V
#define TASK_HAL_TS_60_DEG_C  2676  // 1.634V
#define TASK_HAL_TS_65_DEG_C  2837  // 1.732V
#define TASK_HAL_TS_70_DEG_C  2983  // 1.821V
#define TASK_HAL_TS_75_DEG_C  3114  // 1.901V


/**********************************************************************************/


struct TSysCommand
{
	EIfcVipCommand command;
    u8 data[IFC_VIP_UART_SIZE_DATA];
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
	EOsResult SendSysCommand(TSysCommand sysCommand);
	u16 GetAdcValue(EAdcChannel adcChannel);
	s8 GetTemperature(EIfcVipTemperature ifcVipTemperature);


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

	u8 pwmAcPadHeater;
	u8 pwmAcPtcHeater;
	u8 pwmPtcFan;

	bool acPhase;
	u8 counterAcMain;
	bool flagAcMainPresent;
	u8 counterAcMotor;
	bool flagAcMotorPresent;

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


	////// constants //////


	////// functions //////
	void GetStateTopCpu(void);
	void GetSensorBme688(void);
	void ProcessSysCommand(void);
	void AdcConversionComplete(void);
	void CalculatingTSensors(void);
	s8 CalculatingTSensorDigC(u16 localAdcTHeater, u16 adcLowLevel);
	s8 CalculatingTSensor(EIfcVipTemperature ifcVipTemperature);
	EOsResult SendCommand(EIfcVipCommand command, u8* pBuffer);


	void Run(void);
	EOsResult CreateOsResources(void);


};
//=== end class TTaskHAL ===========================================================

/**********************************************************************************/
#endif
