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

SYS_STATE_TABLE	 (SysState_SelsfTest,	IfcVipState_SelfTest,	0),
SYS_STATE_TABLE	 (SysState_Idle,		IfcVipState_Idle,		0),
SYS_STATE_TABLE	 (SysState_Init,		IfcVipState_Init,		0),
SYS_STATE_TABLE	 (SysState_Busy,		IfcVipState_Busy,		0),



SYS_STATE_TABLE	 (SysError_Start,	IfcVipState_Error, 0),

SYS_STATE_TABLE	 (SysError_DamMotor,			IfcVipError_DamMotor,		TASK_SYS_ERROR_DAM_MOTOR),
SYS_STATE_TABLE	 (SysError_LampFront,  			IfcVipError_Lamp1,			TASK_SYS_ERROR_LAMP_FRONT),
SYS_STATE_TABLE	 (SysError_LampBack,  			IfcVipError_Lamp2,			TASK_SYS_ERROR_LAMP_BACK),
SYS_STATE_TABLE	 (SysError_I2cErrorChannel1,  	IfcVipError_I2c1,			TASK_SYS_ERROR_I2C1),
SYS_STATE_TABLE	 (SysError_I2cErrorChannel2,  	IfcVipError_I2c2,			TASK_SYS_ERROR_I2C2),
SYS_STATE_TABLE	 (SysError_ApplicationError,  	IfcVipError_Application,	TASK_SYS_ERROR_APPLICATION),









