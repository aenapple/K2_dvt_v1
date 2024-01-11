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
#include "InterfaceVIP.hpp"
#include "Motor.hpp"


/**********************************************************************************/
//#define TASK_SYS_OK                (1<<0)


#define TASK_SYS_EVENT_UART_RX_TIMEOUT    (1<<1)
#define TASK_SYS_EVENT_UART_RX_CPLT       (1<<2)
#define TASK_SYS_EVENT_UART_ERROR         (1<<3)
#define TASK_SYS_EVENT_RESET              (1<<5)

#define TASK_SYS_EVENT_SET_DAM_POSITION  (1<<8)

#define TASK_SYS_FLAG_STATE_BUSY  (1<<9)



#define TASK_SYS_EVENT_OK     (1<<14)
#define TASK_SYS_EVENT_ERROR  (1<<15)



#define TASK_SYS_SIZE_VERSION_STRING  16


/**********************************************************************************/
#define TASK_SYS_ERROR_DAM_MOTOR    1
#define TASK_SYS_ERROR_LAMP_FRONT   2
#define TASK_SYS_ERROR_LAMP_BACK    3
#define TASK_SYS_ERROR_I2C1         4
#define TASK_SYS_ERROR_I2C2         5
#define TASK_SYS_ERROR_APPLICATION	6



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
enum EDamPosition
{
	DamPosition_NoPosition = 0x00,
	DamPosition_LeftOpen = 0x01,
	DamPosition_RightOpen = 0x02,
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
	TimeSystem GetTimeSystem(void);
	DateTime GetDateTime();
	void UpdateDateTime(const TTaskSYS::DateTime& newDateTime);
//	void SetError(ESysState error);
	u8 GetErrorCode(void);
	void SetSysState(ESysState sysState);
	ESysState GetSysState(void);
	void UpdateSensorBme688(u8* pBufferState);


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

    TInterfaceVIP InterfaceSlaveVIP;

    u16 counterTimeTickProcess;

    EDamPosition currentDamPosition;
    EDamPosition setDamPosition;


	////// constants //////
    static const u8 errorCode[];
    static const u8 interfaceVipCode[];
    static const u8 module_ident[];


	////// functions //////
    void ProcessError(void);
    void InitProcessError(EOsResult result);
    void ProcessRxData(void);
    void StartRxData(void);
    void CopyVersionString(void);
    
    void SetDamPosition(void);
    EDamPosition GetDamPosition(void);
    EOsResult WaitDamPosition(u16 waitTime);

    void Reset(void);
    EOsResult Delay_IT(u16 timeDelay);


    void Run(void);

    EOsResult Init(void);
	EOsResult CreateOsResources(void);


};
//=== end class TTaskSYS ===========================================================

/**********************************************************************************/
#endif
