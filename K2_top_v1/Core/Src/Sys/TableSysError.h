/**********************************************************************************/
/**
*  @file
*  TableSysError.h - todo.
*
*/
/**********************************************************************************/
//#define SYS_ERROR_TABLE(interfaceVipError, errorCode)

/**********************************************************************************/
SYS_ERROR_TABLE	 (SysError_InterfaceVipM,		IfcVipError_InterfaceMasterVip,	TASK_SYS_ERROR_IFC_VIPM),
SYS_ERROR_TABLE	 (SysError_ApplicationNone,  	IfcVipError_ApplicationNone,	TASK_SYS_NONE_APPLICATION),
SYS_ERROR_TABLE	 (SysError_ApplicationError,  	IfcVipError_Application,		TASK_SYS_ERROR_APPLICATION),

SYS_ERROR_TABLE	 (SysError_I2cErrorChannel1,  	IfcVipError_I2c1,	TASK_SYS_ERROR_I2C1),
SYS_ERROR_TABLE	 (SysError_I2cErrorChannel2,  	IfcVipError_I2c2,	TASK_SYS_ERROR_I2C2),
SYS_ERROR_TABLE	 (SysError_UartErrorChannel,  	IfcVipError_Uart,	TASK_SYS_ERROR_UART),






