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
#define TASK_SYS_EVENT_TICK_PROCESS  (1<<10)
#define TASK_SYS_EVENT_START_TEST    (1<<11)
#define TASK_SYS_EVENT_END_GRINDING  (1<<12)


#define TASK_SYS_EVENT_OK     (1<<14)
#define TASK_SYS_EVENT_ERROR  (1<<15)



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

#define TASK_SYS_1_MINUTE   (u32)60
#define TASK_SYS_10_MINUTES (u32)(10 * TASK_SYS_1_MINUTE)
#define TASK_SYS_1_HOUR     (u32)(60 * TASK_SYS_1_MINUTE)
#define TASK_SYS_2_HOURS    (u32)(2 * TASK_SYS_1_HOUR)
#define TASK_SYS_12_HOURS   (u32)(12 * TASK_SYS_1_HOUR)
#define TASK_SYS_24_HOURS   (u32)(24 * TASK_SYS_1_HOUR)

#define TASK_SYS_REAL_TIME_6AM  0x06
#define TASK_SYS_REAL_TIME_6PM  0x18


/**********************************************************************************/
struct TBme688Sensor  // sizeof = 16 bytes
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

	struct TimeSystem
	{
	    u32 d;
	    u8 h;
	    u8 m;
	    u8 s;
	    u16 ms;
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
	void UpdateTopCpuState(u8* pBufferState);
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
    u32 counterMinute;
    u8 prevHours;

    EIfcVipComponent ifcVipComponent;


	////// constants //////
    static const u8 errorCode[];
    static const u8 interfaceVipCode[];
    static const u8 module_ident[];


	////// functions //////
    void SelfTest(void);
    void ProcessLidOpen(void);
    void ProcessTest(void);
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
    EOsResult ControlLamp(EIfcVipCommand ifcVipCommand, u8* pData);
    EOsResult GetStateLamp(EIfcVipCommand ifcVipCommand, u8* pData);
    EOsResult ControlMotor(EIfcVipCommand ifcVipCommand, u8* pData);
    EOsResult GetStateMotor(EIfcVipCommand ifcVipCommand, u8* pData);
    EOsResult ControlHeater(EIfcVipCommand ifcVipCommand, u8* pData);
    EOsResult GetStateHeater(EIfcVipCommand ifcVipCommand, u8* pData);
    void Reset(void);
    void ProcessTick();
    void ReInitUart(void);
    EOsResult WaitEvent(u32 event);

    // DEBUG
    void TestChambers(void);
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
