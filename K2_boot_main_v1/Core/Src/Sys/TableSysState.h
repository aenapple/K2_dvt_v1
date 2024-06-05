/**********************************************************************************/
/**
*  @file
*  TableSysState.h - todo.
*
*/
/**********************************************************************************/
//#define SYS_STATE_TABLE(enumState, interfaceVipCode, errorCode)

/**********************************************************************************/
SYS_STATE_TABLE	 (SysState_NoState, 	0x00, 0),
SYS_STATE_TABLE	 (SysState_SelfTest, 	0x01, 0),

SYS_STATE_TABLE	 (SysState_Idle, 				0x02,	0),
// SYS_STATE_TABLE	 (SysState_Composting,			0x03,	0),


SYS_STATE_TABLE	 (SysError_Start, 0, 0),

SYS_STATE_TABLE	 (SysError_InterfaceVipM,		0x0D,	TASK_SYS_ERROR_IFC_VIPM),
SYS_STATE_TABLE	 (SysError_ApplicationNone,  	0x0C,	TASK_SYS_NONE_APPLICATION),
SYS_STATE_TABLE	 (SysError_ApplicationError,  	0x0B,	TASK_SYS_ERROR_APPLICATION),








