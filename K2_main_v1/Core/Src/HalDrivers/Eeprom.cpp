/**********************************************************************************/
/**
 *  @file Eeprom.cpp
 *  @brief Driver for EEPROM emulator
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "Eeprom.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::Init()
{

	return(OsResult_Ok);
}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::WriteProcessCounter(u32 data)
{
	EOsResult result;


	taskENTER_CRITICAL();
	result = this->WriteVariable32bits(EEPROM_ADR_PROCESS_COUNTER, data);
	taskEXIT_CRITICAL();
	if(result != OsResult_Ok)
	{
		return(OsResult_ErrorEeprom);
	}


	return(OsResult_Ok);
}
//=== end WriteProcessCounter ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::ReadProcessCounter(u32* data)
{
	EOsResult result;


	taskENTER_CRITICAL();
	result = this->ReadVariable32bits(EEPROM_ADR_PROCESS_COUNTER, data);
	taskEXIT_CRITICAL();
	if(result != OsResult_Ok)
	{
		return(OsResult_ErrorEeprom);
	}


	return(OsResult_Ok);
}
//=== end ReadProcessCounter =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::ReadVariable32bits(u16 address, u32* data)
{


	return(OsResult_Ok);
}
//=== end ReadVariable32bits =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::ReadVariable16bits(u16 address, u16* data)
{


	return(OsResult_Ok);
}
//=== end ReadVariable16bits =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::ReadVariable8bits(u16 address, u8* data)
{


	return(OsResult_Ok);
}
//=== end ReadVariable8bits ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::WriteVariable32bits(u16 address, u32 data)
{

	return(OsResult_Ok);
}
//=== end WriteVariable32bits ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::WriteVariable16bits(u16 address, u16 data)
{


	return(OsResult_Ok);
}
//=== end WriteVariable16bits ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::WriteVariable8bits(u16 address, u8 data)
{


	return(OsResult_Ok);
}
//=== end WriteVariable8bits =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::CleanUp(void)
{



	return(OsResult_Ok);
}
//=== end CleanUp ==================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TEeprom::Format(void)
{


	return(OsResult_Ok);
}
//=== end Format ===================================================================

/**********************************************************************************/
