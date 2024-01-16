/**********************************************************************************/
/**
*  @file
*  InterfaceVIP.hpp - declaration of class TInterfaceVIP.
*
*  Target:  ...
*/

/**********************************************************************************/
#ifndef __InterfaceVIP_H
#define __InterfaceVIP_H

/**********************************************************************************/
#include "Os.hpp"
#include "SysState.h"


/**********************************************************************************/
#define IFC_VIP_UART_SIZE_PACKET    16
#define IFC_VIP_INDEX_CMD           0
#define IFC_VIP_INDEX_DATA          1
#define IFC_VIP_INDEX_CRC          (IFC_VIP_UART_SIZE_PACKET - 1)
#define IFC_VIP_UART_SIZE_DATA     (IFC_VIP_UART_SIZE_PACKET - 2)

////// commands. //////
enum EIfcVipCommand
{
	IfcVipCommand_GetState = 0x00,
	IfcVipCommand_GetBme688_Part1 = 0x01,
	IfcVipCommand_GetBme688_Part2 = 0x02,
	IfcVipCommand_SetFanSpeed = 0x03,
	IfcVipCommand_GetFanSpeed = 0x04,
	IfcVipCommand_GetAdcChannel = 0x05,
	IfcVipCommand_GetAcParameters = 0x06,
	IfcVipCommand_GetTemperature = 0x07,
	IfcVipCommand_SetHeaterParameters = 0x08,
	IfcVipCommand_SetMotorParameters = 0x09,
	IfcVipCommand_SetLampParameters = 0x0A,
	IfcVipCommand_GetGasSensor = 0x0B,
	IfcVipCommand_GetMotorParameters = 0x0C,
	IfcVipCommand_GetLampParameters = 0x0D,
	IfcVipCommand_GetHeaterParameters = 0x0E,
	IfcVipCommand_StartTest = 0x0F,
	IfcVipCommand_ContinueProcess = 0x10,
	IfcVipCommand_StopProcess = 0x11,
	IfcVipCommand_StartProcess = 0x12,
	IfcVipCommand_WriteByte = 0x13,
	IfcVipCommand_ReadByte = 0x14,
	IfcVipCommand_WritePacket = 0x15,
	IfcVipCommand_ReadPacket = 0x16,
	IfcVipCommand_JumpApplication = 0x17,
	IfcVipCommand_Reset = 0x18,
	IfcVipCommand_ControlLamp = 0x19,
	IfcVipCommand_GetStateLamp = 0x1A,
	IfcVipCommand_ControlMotor = 0x1B,
	IfcVipCommand_GetStateMotor = 0x1C,
	IfcVipCommand_ControlHeater = 0x1D,
	IfcVipCommand_GetStateHeater = 0x1E,
	IfcVipCommand_SetPosition = 0x1F,
	IfcVipCommand_GetLevelSensor = 0x20,
	IfcVipCommand_GetResultTest = 0x21,


	IfcVipCommand_nAck = 0xFF,
};

enum EIfcVipState
{
	IfcVipState_NoState = 0x00,
	IfcVipState_SelfTest = 0x01,
	IfcVipState_Idle = 0x02,
	IfcVipState_ProcessCh1 = 0x03,
	IfcVipState_ProcessCh2 = 0x04,
	IfcVipState_Init = 0x05,
	IfcVipState_MesophilicPhase = 0x06,
	IfcVipState_ThermophilicPhase = 0x07,
	IfcVipState_CoolingPhase = 0x08,
	IfcVipState_Busy = 0x09,
	IfcVipState_Phase0 = 0x0A,
	IfcVipState_Phase1 = 0x0B,
	IfcVipState_Phase2 = 0x0C,
	IfcVipState_TopRemoved = 0x0D,
	IfcVipState_LidOpen = 0x0E,
	IfcVipState_Grinding = 0x0F,
	IfcVipState_TankFull = 0x10,


	IfcVipState_Error = 0xFF,

};

enum EIfcVipError
{
	IfcVipError_MainMotor = 0x00,
	IfcVipError_MotorChamber1 = 0x01,
	IfcVipError_MotorChamber2 = 0x02,
	IfcVipError_Lamp1 = 0x03,
	IfcVipError_Lamp2 = 0x04,
	IfcVipError_MainFan = 0x05,
	IfcVipError_PadHeater1 = 0x06,
	IfcVipError_PadHeater2 = 0x07,
	IfcVipError_I2c1 = 0x08,
	IfcVipError_I2c2 = 0x09,
	IfcVipError_Uart = 0x0A,
	IfcVipError_Application = 0x0B,
	IfcVipError_ApplicationNone = 0x0C,
	IfcVipError_InterfaceMasterVip = 0x0D,
	IfcVipError_InterfaceSlaveVip = 0x0E,
	IfcVipError_CriticalGasLevel = 0x0F,
	IfcVipError_PtcHeater1 = 0x10,
	IfcVipError_PtcHeater2 = 0x11,
	IfcVipError_AcMainNotPresent = 0x12,
	IfcVipError_RemovedChaber1 = 0x13,
	IfcVipError_RemovedChaber2 = 0x14,
	IfcVipError_RemovedTank = 0x15,
	IfcVipError_DamMotor = 0x16,
};


enum EIfcVipSubState
{
	IfcVipSubState_Application = 0x00,
	IfcVipSubState_Bootloader = 0x01,
};

enum EIfcVipComponent
{
	IfcVipComponent_All = 0x00,
	IfcVipComponent_LampUv = 0x01,
	IfcVipComponent_LampOzone = 0x02,
	IfcVipComponent_MotorChamber1 = 0x03,
	IfcVipComponent_MotorChamber2 = 0x04,
	IfcVipComponent_MotorMain = 0x05,
	IfcVipComponent_GasSensor = 0x06,
	IfcVipComponent_LevelSensorTank = 0x07,
	IfcVipComponent_LevelSensorChamber1 = 0x08,
	IfcVipComponent_LevelSensorChamber2 = 0x09,
	IfcVipComponent_HeaterChamber1 = 0x0A,
	IfcVipComponent_HeaterChamber2 = 0x0B,
	IfcVipComponent_OpenLidSensor = 0x0C,
	IfcVipComponent_OpenHopperSensor1 = 0x0D,
	IfcVipComponent_OpenHopperSensor2 = 0x0E,
	IfcVipComponent_Speaker = 0x0F,
	IfcVipComponent_Leds = 0x10,
	IfcVipComponent_Fan = 0x11,
	IfcVipComponent_MainMotorTimer = 0x12,
};

enum EIfcVipGasType
{
	IfcVipGasType_CarboneDioxide = 0x01,
	IfcVipGasType_Ammonia = 0x02,
	IfcVipGasType_Ozone = 0x03,
	IfcVipGasType_CarboneMonoxide = 0x04,
	IfcVipGasType_HydrogenSulfide = 0x05,
	IfcVipGasType_Oxygen = 0x06,
	IfcVipGasType_Methane = 0x07,
	IfcVipGasType_Bme688 = 0x08,
};

enum EIfcVipTemperature
{
	IfcVipTemperature_PadHeater1 = 0x01,
	IfcVipTemperature_PadHeater2 = 0x02,
	IfcVipTemperature_Mcu1 = 0x03,
	IfcVipTemperature_Mcu2 = 0x04,
	IfcVipTemperature_PtcHeater1 = 0x05,
	IfcVipTemperature_PtcHeater2 = 0x06,
	IfcVipTemperature_T1 = 0x07,
	IfcVipTemperature_T2 = 0x08,
	IfcVipTemperature_T3 = 0x09,
	IfcVipTemperature_T4 = 0x0A,
	IfcVipTemperature_Mcu3 = 0x0B,
};

enum EIfcVipMotor
{
	IfcVipMotor_Main = 0x01,
	IfcVipMotor_Chamber1 = 0x02,
	IfcVipMotor_Chamber2 = 0x03,
	IfcVipMotor_Dam = 0x04,
};

enum EIfcVipMotorDirection
{
	IfcVipMotorDirection_Forward = 0x01,
	IfcVipMotorDirection_Backward = 0x02,
};

enum EIfcVipFan
{
	IfcVipFan_Main = 0x01,
	IfcVipFan_Ptc1 = 0x02,
	IfcVipFan_Ptc2 = 0x03,
};

enum EIfcVipLamp
{
	IfcVipLamp_Ozone1 = 0x01,
	IfcVipLamp_Ozone2 = 0x02,
};

enum EIfcVipHeater
{
	IfcVipHeater_Pad1 = 0x01,
	IfcVipHeater_Pad2 = 0x02,
	IfcVipHeater_Ptc1 = 0x03,
	IfcVipHeater_Ptc2 = 0x04,
};

enum EIfcVipMemory
{
	IfcVipMemory_Cpu1 = 0x01,
	IfcVipMemory_Cpu2 = 0x02,
	IfcVipMemory_Eeprom = 0x03,
};

enum EIfcDamPosition
{
	IfcDamPosition_None = 0x00,
	IfcDamPosition_OpenLeft = 0x01,
	IfcDamPosition_OpenRight = 0x02,
};

enum EIfcControl
{
	IfcControl_Off = 0x00,
	IfcControl_On = 0x01,
};

enum EIfcBme688Sensor
{
	IfcBme688Sensor_Fan = 0x00,
	IfcBme688Sensor_Left = 0x01,
	IfcBme688Sensor_Right = 0x02,

};

////// System State. //////
struct TIfcSystemState
{
	u8 sysState;
    u8 error;
    u8 subSysState;
    u16 sensorStates;
    u8 motorStates;
    u8 lampStates;
    u16 acCurrent;         // mA
    u8 levelTank;          // cm
    u8 levelChamberLeft;   // cm
    u8 levelChamberRight;  // cm
    u8 heaterStates;
    u8 reserved2;
};

#define IFC_SYS_SHIFT_SWITCH_LID_OPEN          0
#define IFC_SYS_STATE_SWITCH_LID_OPEN          (1 << IFC_SYS_SHIFT_SWITCH_LID_OPEN)
#define IFC_SYS_SHIFT_SWITCH_TOP_REMOVED       1
#define IFC_SYS_STATE_SWITCH_TOP_REMOVED       (1 << IFC_SYS_SHIFT_SWITCH_TOP_REMOVED)
#define IFC_SYS_SHIFT_SWITCH_LOCK_LEFT         2
#define IFC_SYS_STATE_SWITCH_LOCK_LEFT         (1 << IFC_SYS_SHIFT_SWITCH_LOCK_LEFT)
#define IFC_SYS_SHIFT_SWITCH_LOCK_RIGHT        3
#define IFC_SYS_STATE_SWITCH_LOCK_RIGHT        (1 << IFC_SYS_SHIFT_SWITCH_LOCK_RIGHT)
#define IFC_SYS_SHIFT_SWITCH_PRESENT_TANK      4
#define IFC_SYS_STATE_SWITCH_PRESENT_TANK      (1 << IFC_SYS_SHIFT_SWITCH_PRESENT_TANK)
#define IFC_SYS_SHIFT_SWITCH_PRESENT_CH_LEFT   5
#define IFC_SYS_STATE_SWITCH_PRESENT_CH_LEFT   (1 << IFC_SYS_SHIFT_SWITCH_PRESENT_CH_LEFT)
#define IFC_SYS_SHIFT_SWITCH_PRESENT_CH_RIGHT  6
#define IFC_SYS_STATE_SWITCH_PRESENT_CH_RIGHT  (1 << IFC_SYS_SHIFT_SWITCH_PRESENT_CH_RIGHT)
#define IFC_SYS_SHIFT_SWITCH_DAM_LEFT          7
#define IFC_SYS_STATE_SWITCH_DAM_LEFT          (1 << IFC_SYS_SHIFT_SWITCH_DAM_LEFT)
#define IFC_SYS_SHIFT_SWITCH_DAM_RIGHT         8
#define IFC_SYS_STATE_SWITCH_DAM_RIGHT         (1 << IFC_SYS_SHIFT_SWITCH_DAM_RIGHT)
#define IFC_SYS_SHIFT_BME688_FAN               9
#define IFC_SYS_STATE_BME688_FAN               (1 << IFC_SYS_SHIFT_BME688_FAN)
#define IFC_SYS_SHIFT_BME688_LEFT              10
#define IFC_SYS_STATE_BME688_LEFT              (1 << IFC_SYS_SHIFT_BME688_LEFT)
#define IFC_SYS_SHIFT_BME688_RIGHT             11
#define IFC_SYS_STATE_BME688_RIGHT             (1 << IFC_SYS_SHIFT_BME688_RIGHT)

#define IFC_ITEM_STATE_NO_STATE  0x03
#define IFC_ITEM_STATE_ON        0x00
#define IFC_ITEM_STATE_OFF       0x01
#define IFC_ITEM_STATE_ERROR     0x02

#define IFC_SYS_SHIFT_LAMP_FRONT  0
#define IFC_SYS_STATE_LAMP_FRONT  (3 << IFC_SYS_SHIFT_LAMP_FRONT)
#define IFC_SYS_SHIFT_LAMP_BACK   2
#define IFC_SYS_STATE_LAMP_BACK   (3 << IFC_SYS_SHIFT_LAMP_BACK)

#define IFC_SYS_SHIFT_MOTOR_MAIN      0
#define IFC_SYS_STATE_MOTOR_MAIN      (3 << IFC_SYS_SHIFT_MOTOR_MAIN)
#define IFC_SYS_SHIFT_MOTOR_CH_LEFT   2
#define IFC_SYS_STATE_MOTOR_CH_LEFT   (3 << IFC_SYS_SHIFT_MOTOR_CH_LEFT)
#define IFC_SYS_SHIFT_MOTOR_CH_RIGHT  4
#define IFC_SYS_STATE_MOTOR_CH_RIGHT  (3 << IFC_SYS_SHIFT_MOTOR_CH_RIGHT)
#define IFC_SYS_SHIFT_MOTOR_DAM       6
#define IFC_SYS_STATE_MOTOR_DAM       (3 << IFC_SYS_SHIFT_MOTOR_DAM)

#define IFC_SYS_SHIFT_PAD_HEATER_LEFT   0
#define IFC_SYS_STATE_PAD_HEATER_LEFT   (3 << IFC_SYS_SHIFT_PAD_HEATER_LEFT)
#define IFC_SYS_SHIFT_PAD_HEATER_RIGHT  2
#define IFC_SYS_STATE_PAD_HEATER_RIGHT  (3 << IFC_SYS_SHIFT_PAD_HEATER_RIGHT)
#define IFC_SYS_SHIFT_PTC_HEATER_LEFT   4
#define IFC_SYS_STATE_PTC_HEATER_LEFT   (3 << IFC_SYS_SHIFT_PTC_HEATER_LEFT)
#define IFC_SYS_SHIFT_PTC_HEATER_RIGHT  6
#define IFC_SYS_STATE_PTC_HEATER_RIGHT  (3 << IFC_SYS_SHIFT_PTC_HEATER_RIGHT)

////// Data. //////
#define IFC_VIP_DATA_START       0

#define IFC_VIP_STATE_INDEX         IFC_VIP_DATA_START                // 1 byte
#define IFC_VIP_ERROR_INDEX         (IFC_VIP_STATE_INDEX + 1)         // 1 byte
#define IFC_VIP_SUB_STATE_INDEX     (IFC_VIP_ERROR_INDEX + 1)         // 1 byte
#define IFC_VIP_SENSOR_STATE_INDEX  (IFC_VIP_SUB_STATE_INDEX + 1)     // 2 bytes
#define IFC_VIP_MOTOR_STATE_INDEX   (IFC_VIP_SENSOR_STATE_INDEX + 2)  // 1 byte
#define IFC_VIP_LAMP_STATE_INDEX    (IFC_VIP_MOTOR_STATE_INDEX + 1)   // 1 byte
#define IFC_VIP_AC_CURRENT_INDEX    (IFC_VIP_LAMP_STATE_INDEX + 1)    // 2 bytes
#define IFC_VIP_TANK_LEVEL_INDEX    (IFC_VIP_AC_CURRENT_INDEX + 2)    // 1 byte
#define IFC_VIP_CHL_LEVEL_INDEX     (IFC_VIP_TANK_LEVEL_INDEX + 1)    // 1 byte
#define IFC_VIP_CHR_LEVEL_INDEX     (IFC_VIP_CHL_LEVEL_INDEX + 1)     // 1 byte

#define IFC_VIP_NUMBER_OF_ITEM   IFC_VIP_DATA_START  // 1 byte

#define IFC_VIP_BME688_TEMPERATURE     (IFC_VIP_NUMBER_OF_ITEM + 1)          // 2 byte
#define IFC_VIP_BME688_PRESSURE        (IFC_VIP_BME688_TEMPERATURE + 2)      // 4 bytes
#define IFC_VIP_BME688_GAS_RESISTANCE  (IFC_VIP_BME688_PRESSURE + 4)         // 4 bytes
#define IFC_VIP_BME688_HUMIDITY        (IFC_VIP_BME688_GAS_RESISTANCE + 4)   // 2 byte

#define IFC_VIP_ADC_CHANNEL_INDEX  IFC_VIP_NUMBER_OF_ITEM
#define IFC_VIP_FAN_PWM_INDEX      (IFC_VIP_NUMBER_OF_ITEM + 1)  // 1 byte - PWM 0-100%
#define IFC_VIP_FAN_RPM_INDEX      (IFC_VIP_FAN_PWM_INDEX + 1)   // 2 bytes - RPM
#define IFC_VIP_T_SENSOR_INDEX     (IFC_VIP_NUMBER_OF_ITEM + 1)


#define IFC_VIP_MEMORY_TYPE_INDEX     IFC_VIP_DATA_START               // 1 byte
#define IFC_VIP_MEMORY_ADR_INDEX      (IFC_VIP_MEMORY_TYPE_INDEX + 1)  // 4 byte
#define IFC_VIP_MEMORY_WR_DATA_INDEX  (IFC_VIP_MEMORY_ADR_INDEX + 4)   // 1/8 bytes
#define IFC_VIP_MEMORY_RD_DATA_INDEX  (IFC_VIP_MEMORY_TYPE_INDEX + 1)  // 1/8 bytes
#define IFC_VIP_MEMORY_PACKET_SIZE    8

#define IFC_VIP_LAMP_NUMBER_INDEX   IFC_VIP_DATA_START               // 1 byte
#define IFC_VIP_LAMP_CONTROL_INDEX  (IFC_VIP_LAMP_NUMBER_INDEX + 1)  // 1 byte

#define IFC_VIP_POSITION_INDEX   (IFC_VIP_NUMBER_OF_ITEM + 1)  // 1 byte


/**********************************************************************************/
//==================================================================================
class TInterfaceVIP
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void Init(UART_HandleTypeDef* hUart, USART_TypeDef* uartInstance);
	EOsResult StartTxData(EIfcVipCommand command, u8* data = NULL);
	EOsResult StartRxData(void);
	EOsResult ParsingRxPacket(void);
	EIfcVipCommand GetCommand(void);
	u8* GetPointerDataRx(void);
	u8 GetStateCode(ESysState sysState);
	void ReInit(void);

    
  
protected:
	////// variables //////


	////// constants //////


	////// functions //////


  
private:
	////// variables //////
	u8 bufferUartDmaTx[IFC_VIP_UART_SIZE_PACKET];
	u8 bufferUartDmaRx[IFC_VIP_UART_SIZE_PACKET];
	u8 bufferUartDataRx[IFC_VIP_UART_SIZE_PACKET - 2];
    
	EIfcVipCommand command;  // for slave mode and 'state' for master mode

//	UART_HandleTypeDef* hUart;
//	USART_TypeDef* uartInstance;
    
    // DEBUG
    // DEBUG

	
	////// constants //////
	// static u8 ifsEvseStateCode[];


	////// functions //////
	EOsResult StartUartTxData(u8* pBuffer, u16 numBytes);
	EOsResult StartUartRxData(u8* pBuffer, u16 numBytes);
	u8 CalcCrc(u8* pBuffer, u16 numBytes);


};
//=== end class TInterfaceVIP ======================================================

/**********************************************************************************/
#endif
