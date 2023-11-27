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
#include "Os.hpp"


/**********************************************************************************/
#define EEPROM_ADR_PROCESS_COUNTER 0x0001


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



    ////// constants //////


    ////// functions //////
	EOsResult ReadVariable32bits(u16 address, u32* data);
	EOsResult ReadVariable16bits(u16 address, u16* data);
	EOsResult ReadVariable8bits(u16 address, u8* data);
	EOsResult WriteVariable32bits(u16 address, u32 data);
	EOsResult WriteVariable16bits(u16 address, u16 data);
	EOsResult WriteVariable8bits(u16 address, u8 data);
	EOsResult CleanUp(void);


};
//=== end class TEeprom ============================================================

/**********************************************************************************/
#endif
