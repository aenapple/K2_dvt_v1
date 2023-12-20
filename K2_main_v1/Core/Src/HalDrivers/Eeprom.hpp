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


/**********************************************************************************/
#define EEPROM_DEVICE_ADDRESS   0xA0
#define EEPROM_16_BITS_ADDRESS  (u32)0xFFFF

#define EEPROM_FORMAT_SING  0xAA55

#define EEPROM_ADR_FORMAT_SING     0x0000                        // 2 bytes - (0xAA55)
#define EEPROM_ADR_PROCESS_COUNTER (EEPROM_ADR_FORMAT_SING + 2)  // 4bytes


/**********************************************************************************/
//==================================================================================
class TEeprom
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	EOsResult Init(void);
	EOsResult Format(void);
	EOsResult WriteProcessCounter(u32 data);
	EOsResult ReadProcessCounter(u32* data);



protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////
	TOsSemaphore Semaphore;



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
