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

SYS_STATE_TABLE	 (SysState_SelsfTest,		IfcVipState_SelfTest,		0),
SYS_STATE_TABLE	 (SysState_Idle,			IfcVipState_Idle,			0),
SYS_STATE_TABLE	 (SysState_Init,			IfcVipState_Init,			0),
SYS_STATE_TABLE	 (SysState_Busy,			IfcVipState_Busy,			0),
SYS_STATE_TABLE	 (SysState_TopRemoved,		IfcVipState_TopRemoved,		0),
SYS_STATE_TABLE	 (SysState_LidOpen,			IfcVipState_LidOpen,		0),
SYS_STATE_TABLE	 (SysState_Grinding,		IfcVipState_Grinding,		0),
SYS_STATE_TABLE	 (SysState_TopUnlocked,		IfcVipState_TopUnlocked,	0),


SYS_STATE_TABLE	 (SysError_Start,	IfcVipState_Error, 0),

SYS_STATE_TABLE	 (SysError_InterfaceVipM,		IfcVipError_InterfaceMasterVip,	TASK_SYS_ERROR_IFC_VIPM),
SYS_STATE_TABLE	 (SysError_ApplicationNone,  	IfcVipError_ApplicationNone,	TASK_SYS_NONE_APPLICATION),
SYS_STATE_TABLE	 (SysError_ApplicationError,  	IfcVipError_Application,		TASK_SYS_ERROR_APPLICATION),

SYS_STATE_TABLE	 (SysError_I2cErrorChannel1,  	IfcVipError_I2c1,	TASK_SYS_ERROR_I2C1),
SYS_STATE_TABLE	 (SysError_I2cErrorChannel2,  	IfcVipError_I2c2,	TASK_SYS_ERROR_I2C2),
SYS_STATE_TABLE	 (SysError_UartErrorChannel,  	IfcVipError_Uart,	TASK_SYS_ERROR_UART),

SYS_STATE_TABLE	 (SysError_RemovedChamber1,	IfcVipError_RemovedChamber1,	TASK_SYS_ERROR_PRESENT),
SYS_STATE_TABLE	 (SysError_RemovedChamber2,	IfcVipError_RemovedChamber2,	TASK_SYS_ERROR_PRESENT),
SYS_STATE_TABLE	 (SysError_RemovedTank,		IfcVipError_RemovedTank,		TASK_SYS_ERROR_PRESENT),

SYS_STATE_TABLE	 (SysError_MainAcNotPresent,	IfcVipError_AcMainNotPresent,	TASK_SYS_ERROR_MAIN_AC),






