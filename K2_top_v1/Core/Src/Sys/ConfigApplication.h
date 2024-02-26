/**********************************************************************************/
/**
*  @file
*  ConfigApplication.h - todo.
*
*/
/**********************************************************************************/
#ifndef __ConfigApplication_H
#define __ConfigApplication_H



#ifdef __DEBUG
//	#define __DEBUG_SYS_OUTPUT_ENABLED


	#define __SENSORS_BME688_ENABLED
//	#define __SENSORS_CH101_ENABLED

#else

	#define __SENSORS_BME688_ENABLED
	#define __SENSORS_CH101_ENABLED

#endif





/**********************************************************************************/


/**********************************************************************************/
#endif
