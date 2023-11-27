/**********************************************************************************/
/**
*  @file
*  Os.hpp - declaration of the class TOs.
*
*/
/**********************************************************************************/
#ifndef __Os_H
#define __Os_H

/**********************************************************************************/
#include "main.h"
#include "stm32g0xx_hal.h"
// #include "stm32g0xx_hal_dma.h"
// #include "stm32g0xx_hal_flash.h"
// #include "stm32g0xx_hal_gpio.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <string>


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "timers.h"
#include "semphr.h"
#include "stream_buffer.h"
//#include "cmsis_os.h"
//#include "mpu_wrappers.h"

/**********************************************************************************/
////// Build configuration. //////


/**********************************************************************************/
#define OS_QUEUE_MAX_LENGHT        100
#define OS_QUEUE_MAX_SIZE_ELEMENT  100


/**********************************************************************************/
typedef enum
{
	OsResult_Ok,
	OsResult_Error,
	OsResult_Timeout,
	OsResult_ErrorCrc,
	OsResult_ErrorNAck,


	OsResult_ErrorI2cTransmit,
	OsResult_ErrorI2cReceive,
	OsResult_ErrorI2cData,

	OsResult_ErrorEeprom,


	OsResult_ErrorTimer,
	OsResult_ErrorUart,
	OsResult_ErrorUartTransmit,
	OsResult_ErrorUartReceive,


	OsResult_ErrorCreateResource,
	OsResult_ResourceBusy,
	OsResult_QueueEmpty,
	OsResult_QueueFull,
	OsResult_WrongParameter,
	OsResult_MessageBufferFull,





} EOsResult;

//typedef unsigned char u8;
typedef uint8_t u8;
//typedef char s8;
typedef int8_t s8;
//typedef unsigned short u16;
typedef uint16_t u16;
//typedef short s16;
typedef int16_t s16;
//typedef unsigned int u32;
typedef uint32_t u32;
//typedef int s32;
typedef int32_t s32;
//typedef unsigned long u64;
typedef uint64_t u64;
//typedef long s64;
typedef int64_t s64;



/**********************************************************************************/
//==================================================================================
class TOs
{
public:
	/// variables ///

	/// constants ///

	/// functions ///
	static void Delay(u64);
	static u32 GetTimeStamp(void);
	static bool IsTimeExpired(u32);


protected:
	/// variables ///

	/// constants ///

	/// functions ///
	

private:
	/// variables ///

	/// constants ///

	/// functions ///


};
//=== end class TOs ================================================================

/**********************************************************************************/
#endif
