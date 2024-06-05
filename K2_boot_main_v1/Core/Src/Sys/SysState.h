/**********************************************************************************/
/**
*  @file
*  SysState.h - todo.
*
*/
/**********************************************************************************/
#ifndef __SysState_H
#define __SysState_H

enum { ESYS_STATE_BASE = __COUNTER__ };

#define LOCAL_ESYS_STATE (__COUNTER__ - ESYS_STATE_BASE - 1)

#define SYS_STATE_TABLE(enumState, interfaceCode, numberFlashes) \
		enumState = LOCAL_ESYS_STATE

typedef enum
{
#include "TableSysState.h"
	Number_ESysState,

} ESysState;

#undef SYS_STATE_TABLE

/**********************************************************************************/
/*typedef enum
{
	SysState_NoState,
	SysState_Initialization,

	...
	SysError_SEvseDiode,




} ESysState;
*/
/**********************************************************************************/
#endif
