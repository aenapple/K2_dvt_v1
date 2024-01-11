/**********************************************************************************/
/**
*  @file
*  SysError.h - todo.
*
*/
/**********************************************************************************/
#ifndef __SysError_H
#define __SysError_H

enum { ESYS_ERROR_BASE = __COUNTER__ };

#define LOCAL_ESYS_ERROR (__COUNTER__ - ESYS_ERROR_BASE - 1)

#define SYS_ERROR_TABLE(enumError, interfaceCode, numberFlashes) \
		enumError = LOCAL_ESYS_ERROR

typedef enum
{
#include "TableSysError.h"
	Number_ESysError,

} ESysError;

#undef SYS_ERROR_TABLE

/**********************************************************************************/
/*typedef enum
{
	SysError_xx,

	...
	SysError_xx,




} ESysSError;
*/
/**********************************************************************************/
#endif
