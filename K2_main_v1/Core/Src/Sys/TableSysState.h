/**********************************************************************************/
/**
*  @file
*  TableSysState.h - todo.
*
*/
/**********************************************************************************/
//#define SYS_STATE_TABLE(enumState, interfaceVipCode, errorCode)

/**********************************************************************************/
SYS_STATE_TABLE	 (SysState_NoState, IfcVipState_NoState, 0),

SYS_STATE_TABLE	 (SysState_Idle,	IfcVipState_Idle,	0),
SYS_STATE_TABLE	 (SysState_Init,	IfcVipState_Init,	0),
SYS_STATE_TABLE	 (SysState_Busy,	IfcVipState_Busy,	0),


SYS_STATE_TABLE	 (SysError_Start,	IfcVipState_Error, 0),

SYS_STATE_TABLE	 (SysError_InterfaceVipM,		IfcVipError_InterfaceMasterVip,	TASK_SYS_ERROR_IFC_VIPM),
SYS_STATE_TABLE	 (SysError_ApplicationNone,  	IfcVipError_ApplicationNone,	TASK_SYS_NONE_APPLICATION),
SYS_STATE_TABLE	 (SysError_ApplicationError,  	IfcVipError_Application,		TASK_SYS_ERROR_APPLICATION),

SYS_STATE_TABLE	 (SysError_I2cErrorChannel1,  	IfcVipError_I2c1,	TASK_SYS_ERROR_I2C1),
SYS_STATE_TABLE	 (SysError_I2cErrorChannel2,  	IfcVipError_I2c2,	TASK_SYS_ERROR_I2C2),
SYS_STATE_TABLE	 (SysError_UartErrorChannel,  	IfcVipError_Uart,	TASK_SYS_ERROR_UART),






