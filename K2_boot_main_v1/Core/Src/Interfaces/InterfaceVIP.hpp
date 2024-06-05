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
	IfcVipCommand_Test = 0x0F,
	IfcVipCommand_ContinueProcess = 0x10,
	IfcVipCommand_StopProcess = 0x11,
	IfcVipCommand_StartProcess = 0x12,
	IfcVipCommand_WriteByte = 0x13,
	IfcVipCommand_ReadByte = 0x14,
	IfcVipCommand_WritePacket = 0x15,
	IfcVipCommand_ReadPacket = 0x16,
	IfcVipCommand_JumpApplication = 0x17,
	IfcVipCommand_Reset = 0x18,

	IfcVipCommand_nAck = 0xFF,
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
	IfcVipTemperature_Heater1 = 0x01,
	IfcVipTemperature_Heater2 = 0x02,
	IfcVipTemperature_Mcu1 = 0x03,
	IfcVipTemperature_Mcu2 = 0x04,
	IfcVipTemperature_TPtc1 = 0x05,
	IfcVipTemperature_TPtc2 = 0x06,
	IfcVipTemperature_T1 = 0x07,
	IfcVipTemperature_T2 = 0x08,
	IfcVipTemperature_T3 = 0x09,
	IfcVipTemperature_T4 = 0x0A,
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
	IfcVipHeater_Cpu2 = 0x02,
	IfcVipHeater_Eeprom = 0x03,
};




////// Data. //////
#define IFC_VIP_DATA_START       0

#define IFC_VIP_STATE_INDEX      IFC_VIP_DATA_START         // 1 byte
#define IFC_VIP_ERROR_INDEX      (IFC_VIP_STATE_INDEX + 1)  // 1 byte
#define IFC_VIP_SUB_STATE_INDEX  (IFC_VIP_ERROR_INDEX + 1)  // 1 byte

#define IFC_VIP_NUMBER_OF_ITEM   IFC_VIP_DATA_START  // 1 byte

#define IFC_VIP_BME688_TEMPERATURE     (IFC_VIP_NUMBER_OF_ITEM + 1)          // 2 byte
#define IFC_VIP_BME688_PRESSURE        (IFC_VIP_BME688_TEMPERATURE + 2)      // 4 bytes
#define IFC_VIP_BME688_GAS_RESISTANCE  (IFC_VIP_BME688_PRESSURE + 4)         // 4 bytes
#define IFC_VIP_BME688_HUMIDITY        (IFC_VIP_BME688_GAS_RESISTANCE + 4)   // 1 byte

#define IFC_VIP_ADC_CHANNEL_INDEX     IFC_VIP_NUMBER_OF_ITEM
#define IFC_VIP_FAN_SPEED_INDEX       (IFC_VIP_NUMBER_OF_ITEM + 1)        // 1 byte - PWM 0-100%
#define IFC_VIP_T_SENSOR_INDEX        (IFC_VIP_NUMBER_OF_ITEM + 1)

#define IFC_VIP_TEST_COMPONENT_INDEX  IFC_VIP_NUMBER_OF_ITEM
#define IFC_VIP_TEST_GAS_TYPE_INDEX   (IFC_VIP_TEST_COMPONENT_INDEX + 1)  // 1 byte

#define IFC_VIP_HEATER_LOW_T_INDEX    (IFC_VIP_NUMBER_OF_ITEM + 1)        // 1 byte - +/-127
#define IFC_VIP_HEATER_HIGH_T_INDEX   (IFC_VIP_HEATER_LOW_T_INDEX + 1)    // 1 byte - +/-127
#define IFC_VIP_HEATER_PWM_INDEX      (IFC_VIP_HEATER_HIGH_T_INDEX + 1)   // 1 byte - PWM 0-100%

#define IFC_VIP_MOTOR_ON_TIME_INDEX   (IFC_VIP_NUMBER_OF_ITEM + 1)        // 2 bytes - Sec
#define IFC_VIP_MOTOR_OFF_TIME_INDEX  (IFC_VIP_MOTOR_ON_TIME_INDEX + 2)   // 2 bytes
#define IFC_VIP_MOTOR_PWM_INDEX       (IFC_VIP_MOTOR_OFF_TIME_INDEX + 2)  // 1 byte

#define IFC_VIP_LAMP_MAX_GAS_LEVEL_INDEX  (IFC_VIP_NUMBER_OF_ITEM + 1)            // 2 bytes - ppb
#define IFC_VIP_LAMP_ON_TIME_INDEX        (IFC_VIP_LAMP_MAX_GAS_LEVEL_INDEX + 2)  // 2 bytes - Sec
#define IFC_VIP_LAMP_OFF_TIME_INDEX       (IFC_VIP_LAMP_ON_TIME_INDEX + 2)        // 2 bytes

#define IFC_VIP_GAS_TYPE_INDEX   (IFC_VIP_NUMBER_OF_ITEM + 1)        // 1 byte
#define IFC_VIP_GAS_VALUE_INDEX  (IFC_VIP_GAS_TYPE_INDEX + 1)        // 4 bytes

#define IFC_VIP_MEMORY_TYPE_INDEX     IFC_VIP_DATA_START               // 1 byte
#define IFC_VIP_MEMORY_ADR_INDEX      (IFC_VIP_MEMORY_TYPE_INDEX + 1)  // 4 byte
#define IFC_VIP_MEMORY_WR_DATA_INDEX  (IFC_VIP_MEMORY_ADR_INDEX + 4)   // 1/8 bytes
#define IFC_VIP_MEMORY_RD_DATA_INDEX  (IFC_VIP_MEMORY_TYPE_INDEX + 1)  // 1/8 bytes
#define IFC_VIP_MEMORY_PACKET_SIZE    8


/**********************************************************************************/
//==================================================================================
class TInterfaceVIP
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
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
