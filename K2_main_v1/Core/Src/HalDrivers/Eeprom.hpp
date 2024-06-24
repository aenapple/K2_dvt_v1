/**********************************************************************************/
/**
 *  @file Eeprom.hpp
 *  @brief Header file for class TEeprom
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __Eeprom_H
#define __Eeprom_H

/**********************************************************************************/
#include "OsTask.hpp"
// #include "TaskSYS.hpp"


/**********************************************************************************/
#define EEPROM_DEVICE_ADDRESS   0xA0
#define EEPROM_16_BITS_ADDRESS  (u32)0xFFFF

#define EEPROM_MAX_SIZE  0x20000  // 128kBytes

#define EEPROM_FORMAT_SING  0xAA55

#define EEPROM_ADR_FORMAT_SING     0x0000                        // 2 bytes - (0xAA55)
#define EEPROM_ADR_PROCESS_COUNTER (EEPROM_ADR_FORMAT_SING + 2)  // 4bytes

////// log file //////
#define EEPROM_ADR_FORMAT_LOG_FILE  (EEPROM_ADR_PROCESS_COUNTER + 4)  // 2 bytes
#define EEPROM_ADR_INDEX_RECORD     (EEPROM_ADR_FORMAT_LOG_FILE + 2)  // 2 bytes
#define EEPROM_SIZE_INDEX_RECORD  2
#define EEPROM_ADR_FIRST_RECORD  (EEPROM_ADR_INDEX_RECORD + EEPROM_SIZE_INDEX_RECORD)
#define EEPROM_SIZE_LOG_FILE     (sizeof(TLogFIleRecord) * 20000)  // 240000 bytes


/* #define EEPROM_RTC_SLAVE_ADDRESS    0xD0  // 1101.0000
#define EEPROM_RTC_SECONDS_ADDRESS  0x00  // 00-59
#define EEPROM_RTC_MINUTES_ADDRESS  0x01  // 00-59
#define EEPROM_RTC_HOURS_ADDRESS    0x02  // 0-23
#define EEPROM_RTC_DAY_ADDRESS      0x03  // 1-7
#define EEPROM_RTC_DATE_ADDRESS     0x04  // 1-31

#define EEPROM_BETA_TEST_RECORD_SIZE       48    // 48 bytes
#define EEPROM_BETA_TEST_MAX_RECORDS       (u16)2016  // 2016 records (* 48 bytes = 96768 bytes)
#define EEPROM_BETA_TEST_INDEX_RECORD_ADR  (EEPROM_ADR_PROCESS_COUNTER + 4)         // 2 byte, 2016 records.
#define EEPROM_BETA_TEST_TIMESTAMP_ADR     (EEPROM_BETA_TEST_INDEX_RECORD_ADR + 2)  // 8 bytes, RTC - start time
#define EEPROM_BETA_TEST_FIRST_RECORD_ADR  (EEPROM_BETA_TEST_TIMESTAMP_ADR + 8)     // each record = 48 bytes
*/

struct TLogFIleRecord  // sizeof = 12 bytes
{
	u64 systemTime;
	u8 state;
	u8 reserved1;
	u8 reserved2;
	u8 reserved3;
};

/**********************************************************************************/
//==================================================================================
class TEeprom : public TOs
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	EOsResult Init(void);
	EOsResult Format(void);
	EOsResult WriteProcessCounter(u32 data);
	EOsResult ReadProcessCounter(u32* data);

/*	EOsResult WriteTimestamp(TRtc* rtc);
	EOsResult ReadTimestamp(TRtc* rtc);
	EOsResult WriteRecord(TBetaTestRecord* record);
	EOsResult ReadRecord(TBetaTestRecord* record);
*/

	EOsResult WritePacket(u32 address, u8* data);  // always read 8 bytes
	EOsResult ReadPacket(u32 address, u8* data);   // always read 8 bytes

/*	EOsResult WriteRtc(TRtc* rtc);
	EOsResult ReadRtc(TRtc* rtc);
	EOsResult WriteSeconds(u8 seconds);
	EOsResult ReadSeconds(u8* seconds);
	EOsResult WriteMinutes(u8 minutes);
	EOsResult ReadMinutes(u8* minutes);
	EOsResult WriteHours(u8 hours);
	EOsResult ReadHours(u8* hours);
	EOsResult WriteDay(u8 day);
	EOsResult ReadDay(u8* day);
	EOsResult WriteDate(u8 date);
	EOsResult ReadDate(u8* date);
	EOsResult WriteMonth(u8 month);
	EOsResult ReadMonth(u8* month);
	EOsResult WriteYear(u8 year);
	EOsResult ReadYear(u8* year);
*/

protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////
	TOsSemaphore Semaphore;
	u16 indexRecord;


    ////// constants //////


    ////// functions //////
	EOsResult ReadVariable32bits(u32 address, u32* data);
	EOsResult ReadVariable16bits(u32 address, u16* data);
	EOsResult ReadVariable8bits(u32 address, u8* data);
	EOsResult WriteVariable32bits(u32 address, u32 data);
	EOsResult WriteVariable16bits(u32 address, u16 data);
	EOsResult WriteVariable8bits(u32 address, u8 data);


};
//=== end class TEeprom ============================================================

/**********************************************************************************/
#endif
