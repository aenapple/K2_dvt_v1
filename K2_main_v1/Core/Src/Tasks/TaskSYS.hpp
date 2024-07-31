/**********************************************************************************/
/**
*  @file
*  TaskSYS.hpp - declaration of class TTaskSYS.
*
*  Target:  ...
*/

/**********************************************************************************/
#ifndef __TaskSYS_H
#define __TaskSYS_H

/**********************************************************************************/
#include "OsTask.hpp"
#include "Interfaces/InterfaceVIP.hpp"



/**********************************************************************************/
//#define TASK_SYS_OK                (1<<0)


#define TASK_SYS_EVENT_UART_RX_TIMEOUT    (1<<1)
#define TASK_SYS_EVENT_UART_RX_CPLT       (1<<2)
#define TASK_SYS_EVENT_UART_ERROR         (1<<3)
#define TASK_SYS_EVENT_RESET              (1<<5)

#define TASK_SYS_EVENT_TOP_REMOVED   (1<<6)
#define TASK_SYS_EVENT_TOP_PRESENT   (1<<7)
#define TASK_SYS_EVENT_LID_OPEN      (1<<8)
#define TASK_SYS_EVENT_LID_CLOSED    (1<<9)
#define TASK_SYS_EVENT_TOP_LOCKED    (1<<9)
#define TASK_SYS_EVENT_TOP_UNLOCKED  (1<<10)
#define TASK_SYS_EVENT_TICK_PROCESS  (1<<11)
#define TASK_SYS_EVENT_START_TEST    (1<<12)
#define TASK_SYS_EVENT_END_GRINDING  (1<<13)

#define TASK_SYS_CMD_SET_LEFT_OPEN   (1<<14)
#define TASK_SYS_CMD_SET_RIGHT_OPEN  (1<<15)

#define TASK_SYS_EVENT_OK     (1<<16)
#define TASK_SYS_EVENT_ERROR  (1<<17)



#define TASK_SYS_SIZE_VERSION_STRING  16


/**********************************************************************************/
#define TASK_SYS_ERROR_IFC_VIPM     1
#define TASK_SYS_NONE_APPLICATION   2
#define TASK_SYS_ERROR_APPLICATION	3
#define TASK_SYS_ERROR_I2C1         4
#define TASK_SYS_ERROR_I2C2         4
#define TASK_SYS_ERROR_UART         5
#define TASK_SYS_ERROR_MAIN_AC      6
#define TASK_SYS_ERROR_PRESENT      7


#define TASK_SYS_ADDRESS_APPLICATION  0x800C000
#define TASK_SYS_ADDRESS_IDENT      512
#define TASK_SYS_ADDRESS_VERSION    0
#define TASK_SYS_SIZE_VERSION       16
#define TASK_SYS_ADDRESS_CRC        (TASK_SYS_ADDRESS_VERSION + TASK_SYS_SIZE_VERSION)
#define TASK_SYS_SIZE_CRC_FILE      4
#define TASK_SYS_ADDRESS_SIZE_FILE  (TASK_SYS_ADDRESS_CRC + TASK_SYS_SIZE_CRC_FILE)
#define TASK_SYS_SIZE_SIZE_FILE     4
#define TASK_SYS_ADDRESS_KEY        (TASK_SYS_ADDRESS_SIZE_FILE + TASK_SYS_SIZE_SIZE_FILE)


/**********************************************************************************/
#define TASK_SYS_SENSOR_TOP_LOCK1     (1<<0)
#define TASK_SYS_SENSOR_TOP_LOCK2     (1<<1)
#define TASK_SYS_SENSOR_LID_OPEN      (1<<2)
#define TASK_SYS_SENSOR_AC_INTERRUPT  (1<<3)
#define TASK_SYS_SENSOR_TANK_PRESENT  (1<<4)
#define TASK_SYS_SENSOR_CHL_PRESENT   (1<<5)
#define TASK_SYS_SENSOR_CHR_PRESENT   (1<<6)

#define TASK_SYS_MOTOR_MAIN      (1<<0)
#define TASK_SYS_MOTOR_CH_LEFT   (1<<1)
#define TASK_SYS_MOTOR_CH_RIGHT  (1<<2)
#define TASK_SYS_MOTOR_DAM       (1<<3)
#define TASK_SYS_SENSOR_DAM1     (1<<4)
#define TASK_SYS_SENSOR_DAM2     (1<<5)

#define TASK_SYS_LAMP_LEFT   (1<<0)
#define TASK_SYS_LAMP_RIGHT  (1<<2)


/**********************************************************************************/
#define TASK_SYS_TIME_TICK_PROCESS  1000  // 1 Sec

#define TASK_SYS_1_SECONDS   (u16)1
#define TASK_SYS_2_SECONDS   (u16)2
#define TASK_SYS_3_SECONDS   (u16)3
#define TASK_SYS_4_SECONDS   (u16)4
#define TASK_SYS_5_SECONDS   (u16)5
#define TASK_SYS_6_SECONDS   (u16)6
#define TASK_SYS_7_SECONDS   (u16)7
#define TASK_SYS_8_SECONDS   (u16)8
#define TASK_SYS_10_SECONDS  (u16)10

#define TASK_SYS_20_SECONDS  (u16)20

#define TASK_SYS_30_SECONDS  (u16)30

#define TASK_SYS_1_MINUTE   59
#define TASK_SYS_2_MINUTES  (u16)(2 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_3_MINUTES  (u16)(3 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_4_MINUTES  (u16)(4 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_5_MINUTES  (u16)(5 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_6_MINUTES  (u16)(6 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_7_MINUTES  (u16)(7 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_8_MINUTES  (u16)(8 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_10_MINUTES (u16)(10 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_12_MINUTES (u16)(12 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_15_MINUTES (u16)(15 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_20_MINUTES (u16)(20 * (TASK_SYS_1_MINUTE + 1) - 1)

#define TASK_SYS_1_HOUR     (u16)(60 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_2_HOURS    (u16)(120 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_3_HOURS    (u16)(120 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_5_HOURS    (u16)(300 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_10_HOURS   (u16)(600 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_12_HOURS   (u32)(720 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_24_HOURS   (u32)(1440 * (TASK_SYS_1_MINUTE + 1) - 1)

#define TASK_SYS_0_MINUTES    (u16)0
#define TASK_SYS_30_MINUTES   (u16)(30 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_40_MINUTES   (u16)(40 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_60_MINUTES   (u16)(60 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_80_MINUTES   (u16)(80 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_90_MINUTES   (u16)(90 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_105_MINUTES  (u16)(105 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_120_MINUTES  (u16)(120 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_150_MINUTES  (u16)(150 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_165_MINUTES  (u16)(165 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_180_MINUTES  (u16)(180 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_210_MINUTES  (u16)(210 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_240_MINUTES  (u16)(240 * (TASK_SYS_1_MINUTE + 1) - 1)
#define TASK_SYS_360_MINUTES  (u16)(360 * (TASK_SYS_1_MINUTE + 1) - 1)

#define TASK_SYS_REAL_TIME_6AM  0x06
#define TASK_SYS_REAL_TIME_6PM  0x18


/**********************************************************************************/
struct TBme688Sensor  // sizeof = 12 bytes
{
	s16 temperature;
	u16 humidity;
	u32 pressure;
	u32 gasResistance;
};

struct TRtc  // sizeof = 8 bytes, format BCD
{
	u8 seconds;
	u8 minutes;
	u8 hours;
	u8 day;
	u8 date;
	u8 month;
	u8 year;
	u8 reserved1;
};

/* struct TBetaTestRecord  // sizeof = 48 bytes
{
	TBme688Sensor bme688SensorLeft;
	TBme688Sensor bme688SensorRight;
	TBme688Sensor bme688SensorFan;
	s8 tPadLeft;
	s8 tPadRight;
	s8 tPtcLeft;
	s8 tPtcRight;
	TimeSystem timestamp;
}; */

/**********************************************************************************/
//==================================================================================
class TTaskSYS : public TOsTask
{
public:
	struct DateTime
	{
		u16 year;
		u8 month;
		u8 day;
		u32 seconds;

		DateTime():year(0), month(0), day(0), seconds(0)
		{}
	};

	struct TimeSystem  // sizeof = 8 bytes
	{
		u16 ms;
		u16 d;
		u8 h;
		u8 m;
		u8 s;
		u8 reserved;
	};

	////// variables //////


	////// constants //////


	////// functions //////
	TTaskSYS()
	{
		this->enableTickHook = false;
	}
	u64 GetSystemCounter(void);
	TimeSystem GetTimeSystem(void);
	DateTime GetDateTime();
	void UpdateDateTime(const TTaskSYS::DateTime& newDateTime);
//	void SetError(ESysState error);
	u8 GetErrorCode(void);
	void SetSysState(ESysState sysState);
	ESysState GetSysState(void);
	void UpdateTopCpuState(TIfcSystemState* IfcSystemState);
	void UpdateSensorBme688(EIfcBme688Sensor ifcBme688Sensor, u8* pBufferState);


	void CreateTask(void);
	void CreateTaskStatic(void);

	void SetEventTickFromISR(void);
	void SetEventUartTxCpltFromISR(void);
	void SetEventUartRxCpltFromISR(void);
	void SetEventUartErrorFromISR(void);
//	void SetEventUartRxHalfCpltFromISR(void);


  
protected:
	////// variables //////


	////// constants //////


	////// functions //////


  
private:
	////// variables //////
	StaticTask_t xTaskBuffer;
	StackType_t xStackBuffer[OS_TASK_SYS_SIZE_STACK];

	char versionString[TASK_SYS_SIZE_VERSION_STRING];

	u64 systemCounter;
    DateTime dateTime;
    bool enableTickHook;
//    ESysState error;
    ESysState sysState;
    EIfcVipState ifcVipState;
    TIfcSystemState ifcSystemState;
    TBme688Sensor bme688SensorFan;
    TBme688Sensor bme688SensorLeft;
    TBme688Sensor bme688SensorRight;

    TInterfaceVIP InterfaceSlaveVIP;

    u16 counterTimeTickProcess;
    u16 counterMinute;
    u8 prevHours;
    u16 counterBetaTestLog;

    EIfcVipComponent ifcVipComponent;

    ////// constants //////
    static const u8 errorCode[];
    static const u8 interfaceVipCode[];
    static const u8 module_ident[];


	////// functions //////
    void SelfTest(void);
    void ProcessTopUnlocked(void);
    void ProcessTopRemoved(void);
    void ProcessLidOpen(void);
    void ProcessTest(void);
    void ProcessSetPosition(u8 position);
    void ProcessError(void);
    void InitProcessError(EOsResult result);
    void ProcessRxData(void);
    void StartRxData(void);
    void CopyVersionString(void);
    
    EOsResult WriteByteToMemory(u8* pReceivedPacket);
    EOsResult ReadByteFromMemory(u8* pReceivedPacket, u8* readByte);
    EOsResult WritePacketToMemory(u8* pReceivedPacket, u8* writePacket);
    EOsResult ReadPacketFromMemory(u8* pReceivedPacket, u8* readPacket);
    EOsResult WriteByteToSlaveCpu(u32 address, u8 data);
    EOsResult ReadByteFromSlaveCpu(u32 address, u8* data);
    EOsResult WritePacketToSlaveCpu(u32 address, u8* data);
    EOsResult ReadPacketFromSlaveCpu(u32 address, u8* data);
    EOsResult WriteByteToEeprom(u32 address, u8 data);
    EOsResult ReadByteFromEeprom(u32 address, u8* data);
    EOsResult WritePacketToEeprom(u32 address, u8* data);
    EOsResult ReadPacketFromEeprom(u32 address, u8* data);
    EOsResult ControlLamp(u8* pData);
    EOsResult GetStateLamp(EIfcVipCommand ifcVipCommand, u8* pData);
    EOsResult ControlMainFan(u8 pwm);
    EOsResult ControlFan(u8* pData);
    EOsResult GetStateFan(u8* pData);
    EOsResult ControlMotor(u8* pData);
    EOsResult GetStateMotor(u8* pData);
    EOsResult ControlHeater(u8* pData);
    EOsResult GetStateHeater(u8* pData);
    void Reset(void);
    void ProcessTick();
    void ReInitUart(void);
    EOsResult WaitEvent(u32 event);

    // DEBUG
    void TestChamberMotors(void);
    void TestMainMotor(void);
    void TestPtcFans(void);
    void TestPtcHeaters(void);
    void TestPadHeaters(void);
    // DEBUG


    void Run(void);

    EOsResult Init(void);
	EOsResult CreateOsResources(void);


};
//=== end class TTaskSYS ===========================================================

/**********************************************************************************/
#endif
