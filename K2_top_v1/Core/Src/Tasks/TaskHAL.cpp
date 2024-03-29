/**********************************************************************************/
/**
*  @file
*  Todo: TaskHAL.cpp.
*
*/
/**********************************************************************************/
#include "TaskHAL.hpp"
#include "TaskSYS.hpp"


TTaskHAL TaskHAL;
extern TTaskSYS TaskSYS;
/**********************************************************************************/
#ifdef __DEBUG_HAL_OUTPUT_ENABLED
	#include "TaskCLI.hpp"
	extern TTaskCLI TaskCLI;
	#define DiagPrintf(...) TaskCLI.DebugPrintf(__VA_ARGS__)
	#define DiagNotice(fmt, ...) TaskCLI.DebugNotice("HAL: " fmt "\r\n", ##__VA_ARGS__)
#else
	#define DiagPrintf(...)
	#define DiagNotice(...)
#endif

/**********************************************************************************/


/**********************************************************************************/


void HAL_GPIO_EXTI_Rising_Callback(u16 gpioPin)
{
	TaskHAL.HandlerCh101Interrupt(gpioPin);
}

void HAL_GPIO_EXTI_Falling_Callback(u16 gpioPin)
{
	TaskHAL.HandlerGpioInterrupt(gpioPin);
}




/**********************************************************************************/
//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::SetEventTickFromISR(void)
{
	if(this->Bme688_Fan.GetCounterSamplingTime() > 0)
	{
		this->Bme688_Fan.DecreaseCounterSamplingTime();
	}
	else
	{
		this->Bme688_Fan.SetCounterSamplingTime(this->Bme688_Fan.GetSamplingTime());
		this->SetEventsFromISR(TASK_HAL_EVENT_GET_BME688_FAN);
	}

	if(this->Bme688_Left.GetCounterSamplingTime() > 0)
	{
		this->Bme688_Left.DecreaseCounterSamplingTime();
	}
	else
	{
		this->Bme688_Left.SetCounterSamplingTime(this->Bme688_Left.GetSamplingTime());
		this->SetEventsFromISR(TASK_HAL_EVENT_GET_BME688_LEFT);
	}

	if(this->Bme688_Right.GetCounterSamplingTime() > 0)
	{
		this->Bme688_Right.DecreaseCounterSamplingTime();
	}
	else
	{
		this->Bme688_Right.SetCounterSamplingTime(this->Bme688_Right.GetSamplingTime());
		this->SetEventsFromISR(TASK_HAL_EVENT_GET_BME688_RIGHT);
	}


	if(this->counterTimeAcMeasurement < TASK_HAL_AC_TIME_MEASUREMENT)
	{
		this->counterTimeAcMeasurement++;
	}
	else
	{
		////// Input each 100 mSec. //////
		this->counterTimeAcMeasurement = 0;

		if(this->counterLampFront < TASK_HAL_AC_PULSE_NUMBERS)
		{
			this->SetSysStateLamp(HalLamp_Front, IFC_ITEM_STATE_OFF);
		}
		else
		{
			this->SetSysStateLamp(HalLamp_Front, IFC_ITEM_STATE_ON);
		}
		this->counterLampFront = 0;


		if(this->counterLampBack < TASK_HAL_AC_PULSE_NUMBERS)
		{
			this->SetSysStateLamp(HalLamp_Back, IFC_ITEM_STATE_OFF);
		}
		else
		{
			this->SetSysStateLamp(HalLamp_Back, IFC_ITEM_STATE_ON);
		}
		this->counterLampBack = 0;


	}

	if(this->timeRpmFanCounter < TASK_HAL_TIME_FAN_RPM)
	{
		this->timeRpmFanCounter++;
	}
	else
	{
		this->timeRpmFanCounter = 0;
		this->rpmFan = this->Fan.GetCounterRpm() * 60;
		this->Fan.ResetCounterRpm();
	}

}
//=== end SetEventTickFromISR ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskHAL::Run(void)
{
	u32 resultBits;
	EOsResult result;
	bool flagBme688_Left;
	bool flagBme688_Right;
//	u64 timeStamp;
	float bufferTank[10];
	float bufferLeft[10];
	float bufferRight[10];
	float left;
	float right;
	float tank;


	this->Delay(20);

	while(true)
	{
		if(this->EventGroup.WaitAndBits(TASK_HAL_CMD_START, 1000) == OsResult_Ok)
		{
			break;
		}
	}

	this->Delay(10);


#ifdef __SENSORS_BME688_ENABLED

	////// Start Bme688 sensors. //////
	if(!this->flagErrorBme688_Fan)
	{
		result = this->Bme688_Fan.StartMeasurement();
		if(result != OsResult_Ok)
		{
			this->flagErrorBme688_Fan = true;
			this->SetSysStateSensor(IFC_SYS_STATE_BME688_FAN, IFC_ITEM_STATE_OFF);
		}
	}

	if(!this->flagErrorBme688_Left)
	{
		result = this->Bme688_Left.StartMeasurement();
		if(result != OsResult_Ok)
		{
			this->flagErrorBme688_Left = true;
			this->SetSysStateSensor(IFC_SYS_STATE_BME688_LEFT, IFC_ITEM_STATE_OFF);
		}
	}

	if(!this->flagErrorBme688_Right)
	{
		result = this->Bme688_Right.StartMeasurement();
		if(result != OsResult_Ok)
		{
			this->flagErrorBme688_Right = true;
			this->SetSysStateSensor(IFC_SYS_STATE_BME688_RIGHT, IFC_ITEM_STATE_OFF);
		}
	}

#endif


#ifdef __SENSORS_CH101_ENABLED

	result = this->Ch101->StartMeasurement(Ch101Sensor_Left);
	if(result != OsResult_Ok)
	{
		TaskSYS.SetSysState(SysError_LevelSensor2);
	}

	result = this->Ch101->StartMeasurement(Ch101Sensor_Right);
	if(result != OsResult_Ok)
	{
		TaskSYS.SetSysState(SysError_LevelSensor3);
	}

	result = this->Ch101->StartMeasurement(Ch101Sensor_Tank);
	if(result != OsResult_Ok)
	{
		TaskSYS.SetSysState(SysError_LevelSensor1);
	}

#endif


	this->enableCh101 = true;
	flagBme688_Left = false;
	flagBme688_Right = false;
	while(true)
	{
        if(this->EventGroup.WaitOrBits(
        			TASK_HAL_EVENT_GET_BME688_FAN   |
					TASK_HAL_EVENT_GET_BME688_LEFT  |
					TASK_HAL_EVENT_GET_BME688_RIGHT |
					TASK_HAL_EVENT_GET_CH101_TANK   |
					TASK_HAL_EVENT_GET_CH101_LEFT   |
					TASK_HAL_EVENT_GET_CH101_RIGHT,
					&resultBits,
					100
					) == OsResult_Timeout)
        {
        	this->UpdateHardwareStates();

        	if(this->counterExti3 >= 10)
        	{
        		left = left / 10;
        		right = right / 10;
        		tank = tank / 10;

        		left = 0;
        		right = 0;
        		tank = 0;

        		this->counterExti1 = 0;
        		this->counterExti2 = 0;
        		this->counterExti3 = 0;
        	}
        	continue;
        }

      
#ifdef __SENSORS_CH101_ENABLED

        if((resultBits & TASK_HAL_EVENT_GET_CH101_TANK) > 0)
        {
        	this->Ch101->GetCh101Sensor(Ch101Sensor_Tank, &this->Ch101SensorTank);
        	this->IfcSystemState.levelTank = (u8)(this->Ch101SensorTank.range / 10);  // mm / 10 = cm
        	bufferTank[this->counterExti1] = this->Ch101SensorTank.range;
        	this->counterExti1++;
        	tank += this->Ch101SensorTank.range;
        }

		if((resultBits & TASK_HAL_EVENT_GET_CH101_LEFT) > 0)
		{
			this->Ch101->GetCh101Sensor(Ch101Sensor_Left, &this->Ch101SensorLeft);
			this->IfcSystemState.levelChamberLeft = (u8)(this->Ch101SensorLeft.range / 10);  // mm / 10 = cm
			bufferLeft[this->counterExti2] = this->Ch101SensorLeft.range;
			this->counterExti2++;
			left += this->Ch101SensorLeft.range;
		}

		if((resultBits & TASK_HAL_EVENT_GET_CH101_RIGHT) > 0)
		{
			this->Ch101->GetCh101Sensor(Ch101Sensor_Right, &this->Ch101SensorRight);
			this->IfcSystemState.levelChamberRight = (u8)(this->Ch101SensorRight.range / 10);  // mm / 10 = cm
			bufferRight[this->counterExti3] = this->Ch101SensorRight.range;
			this->counterExti3++;
			right += this->Ch101SensorRight.range;
		}

#endif

#ifdef __SENSORS_BME688_ENABLED

        if((resultBits & TASK_HAL_EVENT_GET_BME688_FAN) > 0)
        {
        	if(!this->flagErrorBme688_Fan)
        	{
        		this->ProcessDataBme688(&this->Bme688_Fan);
        	}
        }

        if((resultBits & TASK_HAL_EVENT_GET_BME688_LEFT) > 0)
        {
        	if(!this->flagErrorBme688_Left)
        	{
        		this->ProcessDataBme688(&this->Bme688_Left);

        		TBme688Sensors* bme688Sensors;
        		bme688Sensors = this->Bme688_Left.GetPointerBme688Sensors();
        		if(bme688Sensors->humidity > 8200)
        		{
        			flagBme688_Left = true;
        		}
        		else
        		{
        			flagBme688_Left = false;
        		}

        	}

        }

        if((resultBits & TASK_HAL_EVENT_GET_BME688_RIGHT) > 0)
        {
        	if(!this->flagErrorBme688_Right)
        	{
        		this->ProcessDataBme688(&this->Bme688_Right);

        		TBme688Sensors* bme688Sensors;
        		bme688Sensors = this->Bme688_Right.GetPointerBme688Sensors();
        		if(bme688Sensors->humidity > 8200)
        		{
        			flagBme688_Right = true;
        		}
        		else
        		{
        			flagBme688_Right = false;
        		}

        	}

        }

#endif

        if(flagBme688_Left || flagBme688_Right)
        {
        	this->Fan.Start(100);
        }
        else
        {
        	this->Fan.Stop();
        }
        
        
        // this->Delay(50);  // mSec
		
      
      //this->DebugPrint("Cycles - %06d\r\n", counter);



	}  // end while(true)


}
//=== end Run ======================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::ProcessDataBme688(TBme688* pBme688)
{
	DataBme688* pDataBme688[3];
	DataBme688 dataBme688[3];
	TBme688Sensors Bme688Sensors;


	pDataBme688[0] = &dataBme688[0];
	pDataBme688[1] = &dataBme688[1];
	pDataBme688[2] = &dataBme688[2];

	if(pBme688->GetData(pDataBme688) != OsResult_Ok)
	{
		if(pBme688 == &this->Bme688_Fan)
		{
			this->flagErrorBme688_Fan = true;
			this->SetSysStateSensor(IFC_SYS_STATE_BME688_FAN, IFC_ITEM_STATE_OFF);
			return;
		}

		if(pBme688 == &this->Bme688_Left)
		{
			this->flagErrorBme688_Left = true;
			this->SetSysStateSensor(IFC_SYS_STATE_BME688_LEFT, IFC_ITEM_STATE_OFF);
			return;
		}

		////// if(pBme688 == &this->Bme688_Right) //////
		this->flagErrorBme688_Right = true;
		this->SetSysStateSensor(IFC_SYS_STATE_BME688_RIGHT, IFC_ITEM_STATE_OFF);
		return;
	}

	for(u16 i = 0; i < 3; i++)
	{
		/*	DiagNotice("%d, %d, %d, %d, %x, %x, %x",
	                    i2cDataBme688[i].temperature, / 100,
		                i2cDataBme688[i].pressure,
		                i2cDataBme688[i].humidity / 1000,
		                i2cDataBme688[i].gas_resistance,
					   	i2cDataBme688[i].status,
						i2cDataBme688[i].gas_index,
						i2cDataBme688[i].meas_index); */

		u16 tempU16;
		tempU16 = (dataBme688[i].status & BME688_VALID_DATA)
					| (dataBme688[i].gas_index & 0x03);
		tempU16 <<= 8;
		tempU16 |= dataBme688[i].meas_index;

		if((dataBme688[i].status & BME688_VALID_DATA) == BME688_VALID_DATA)
		{
			taskENTER_CRITICAL();
			Bme688Sensors.temperature = dataBme688[i].temperature;
			Bme688Sensors.pressure = dataBme688[i].pressure;
			Bme688Sensors.humidity = (u16)dataBme688[i].humidity / 10;
			Bme688Sensors.gasResistance = dataBme688[i].gas_resistance;
			Bme688Sensors.status = tempU16;
			taskEXIT_CRITICAL();

			pBme688->WriteSensors(&Bme688Sensors);

			return;
		}
		/* else
		{
		taskENTER_CRITICAL();

		SensorBme688.SetStatus(tempU16);

		taskEXIT_CRITICAL();
		}*/
	}

	for(u16 i = 0; i < 3; i++)
	{
		if((dataBme688[0].status & BME688_NEW_DATA_MSK) == BME688_NEW_DATA_MSK)
		{
			taskENTER_CRITICAL();
			Bme688Sensors.temperature = dataBme688[i].temperature;
			Bme688Sensors.pressure = 0;
			Bme688Sensors.humidity = (u16)dataBme688[i].humidity / 10;
			Bme688Sensors.gasResistance = dataBme688[i].gas_resistance;
			taskEXIT_CRITICAL();

			pBme688->WriteSensors(&Bme688Sensors);

			return;
		}
	}


	taskENTER_CRITICAL();
	Bme688Sensors.temperature = 0;
	Bme688Sensors.pressure = 0;
	Bme688Sensors.humidity = 0;
	Bme688Sensors.gasResistance = 0;
	taskEXIT_CRITICAL();

	pBme688->WriteSensors(&Bme688Sensors);
}
//=== end GetSensorBme688 ==========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
void TTaskHAL::UpdateHardwareStates(void)
{
	if(this->ReadLockLeft() == GpioLevel_Low)
	{
		this->SetSysStateSensor(IFC_SYS_STATE_SWITCH_LOCK_LEFT, IFC_ITEM_STATE_ON);
	}
	else
	{
		this->SetSysStateSensor(IFC_SYS_STATE_SWITCH_LOCK_LEFT, IFC_ITEM_STATE_OFF);
	}

	if(this->ReadLockRight() == GpioLevel_Low)
	{
		this->SetSysStateSensor(IFC_SYS_STATE_SWITCH_LOCK_RIGHT, IFC_ITEM_STATE_ON);
	}
	else
	{
		this->SetSysStateSensor(IFC_SYS_STATE_SWITCH_LOCK_RIGHT, IFC_ITEM_STATE_OFF);
	}

	if(this->ReadDamSensorLeft() == GpioLevel_Low)
	{
		this->SetSysStateSensor(IFC_SYS_STATE_SWITCH_DAM_LEFT, IFC_ITEM_STATE_ON);
	}
	else
	{
		this->SetSysStateSensor(IFC_SYS_STATE_SWITCH_DAM_LEFT, IFC_ITEM_STATE_OFF);
	}

	if(this->ReadDamSensorRight() == GpioLevel_Low)
	{
		this->SetSysStateSensor(IFC_SYS_STATE_SWITCH_DAM_RIGHT, IFC_ITEM_STATE_ON);
	}
	else
	{
		this->SetSysStateSensor(IFC_SYS_STATE_SWITCH_DAM_RIGHT, IFC_ITEM_STATE_OFF);
	}

	// todo:
	// read Tank Level Sensor
	// read Chamber Left Level Sensor
	// read Chamber Right Level Sensor

}
//=== end UpdateHardwareStates =====================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
s8 TTaskHAL::GetTemperatureCpu2(void)
{
	return(this->tCpu2);
}
//=== end GetTemperatureCpu2 =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
s8 TTaskHAL::GetTemperatureCpu3(void)
{
	return(this->tCpu3);
}
//=== end GetTemperatureCpu3 =======================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::StartFan(u8 pwm)
{
	this->Fan.Start(pwm);
}
//=== end StartFan =================================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::StopFan()
{
	this->Fan.Stop();
}
//=== end StopFan ==================================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
u8 TTaskHAL::GetFanPwm()
{
	return(this->Fan.GetPwm());
}
//=== end GetFanPwm ================================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
u16 TTaskHAL::GetFanRpm()
{
	return(this->rpmFan);
}
//=== end GetFanRpm ================================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
TBme688Sensors* TTaskHAL::GetPointerBme688Sensors(EIfcBme688Sensor ifcBme688Sensor)
{
	if(ifcBme688Sensor == IfcBme688Sensor_Fan)
	{
		return(this->Bme688_Fan.GetPointerBme688Sensors());
	}
	else
	{
		if(ifcBme688Sensor == IfcBme688Sensor_Left)
		{
			return(this->Bme688_Left.GetPointerBme688Sensors());
		}
		else
		{
			return(this->Bme688_Right.GetPointerBme688Sensors());
		}
	}
}
//=== end GetPointerBme688Sensors ==================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
u16 TTaskHAL::GetBme688SensorStatus(EIfcBme688Sensor ifcBme688Sensor)
{
	if(ifcBme688Sensor == IfcBme688Sensor_Fan)
	{
		return(this->Bme688_Fan.GetBme688SensorStatus());
	}
	else
	{
		if(ifcBme688Sensor == IfcBme688Sensor_Left)
		{
			return(this->Bme688_Left.GetBme688SensorStatus());
		}
		else
		{
			return(this->Bme688_Right.GetBme688SensorStatus());
		}
	}
}
//=== end GetPointerBme688Sensors ==================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::ControlLamp(EHalLamp halLamp, EHalTurn halTurn)
{
	if(halLamp == HalLamp_Front)
	{
		if(halTurn == HalTurn_Off)
		{
			HAL_GPIO_WritePin(ON_FRONT_LAMP_GPIO_Port, ON_FRONT_LAMP_Pin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(ON_FRONT_LAMP_GPIO_Port, ON_FRONT_LAMP_Pin, GPIO_PIN_SET);
		}
	}
	else  // HalLamp_Back
	{
		if(halTurn == HalTurn_Off)
		{
			HAL_GPIO_WritePin(ON_BACK_LAMP_GPIO_Port, ON_BACK_LAMP_Pin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(ON_BACK_LAMP_GPIO_Port, ON_BACK_LAMP_Pin, GPIO_PIN_SET);
		}
	}
}
//=== end ControlLamp ==============================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
u8 TTaskHAL::GetLampState(EHalLamp halLamp)
{
	if(halLamp == HalLamp_Front)
	{
		return(this->stateLampFront);
	}
	else  // HalLamp_Back
	{
		return(this->stateLampBack);
	}
}
//=== end GetLampState =============================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
EGpioLevel TTaskHAL::ReadLockLeft()
{
	if(HAL_GPIO_ReadPin(LOCK_LEFT_GPIO_Port, LOCK_LEFT_Pin) == GPIO_PIN_RESET)
	{
		return(GpioLevel_Low);
	}
	else
	{
		return(GpioLevel_High);
	}
}
//=== end ReadLockLeft =============================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
EGpioLevel TTaskHAL::ReadLockRight()
{
	if(HAL_GPIO_ReadPin(LOCK_RIGHT_GPIO_Port, LOCK_RIGHT_Pin) == GPIO_PIN_RESET)
	{
		return(GpioLevel_Low);
	}
	else
	{
		return(GpioLevel_High);
	}
}
//=== end ReadLockRight ============================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
EGpioLevel TTaskHAL::ReadDamSensorLeft()
{
	if(HAL_GPIO_ReadPin(DAM_RIGHT_GPIO_Port, DAM_RIGHT_Pin) == GPIO_PIN_RESET)
	{
		return(GpioLevel_Low);
	}
	else
	{
		return(GpioLevel_High);
	}
}
//=== end ReadDamSensorLeft ========================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
EGpioLevel TTaskHAL::ReadDamSensorRight()
{
	if(HAL_GPIO_ReadPin(DAM_LEFT_GPIO_Port, DAM_LEFT_Pin) == GPIO_PIN_RESET)
	{
		return(GpioLevel_Low);
	}
	else
	{
		return(GpioLevel_High);
	}
}
//=== end ReadDamSensorRight =======================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::DamMotorStartForward()
{
	this->DamMotor.StartForward();
	this->SetSysStateDamMotor(IFC_ITEM_STATE_ON);
}
//=== end DamMotorStartForward =====================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::DamMotorStartBackward()
{
	this->DamMotor.StartBackward();
	this->SetSysStateDamMotor(IFC_ITEM_STATE_ON);
}
//=== end DamMotorStartBackward ====================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::DamMotorStop()
{
	this->DamMotor.Stop();
	this->SetSysStateDamMotor(IFC_ITEM_STATE_OFF);
}
//=== end DamMotorStop =============================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::SetSysStateDamMotor(u8 stateDmaMotor)
{
	this->IfcSystemState.motorStates = this->IfcSystemState.motorStates & (0xFF - IFC_SYS_STATE_MOTOR_DAM);
	this->IfcSystemState.motorStates = this->IfcSystemState.motorStates | (stateDmaMotor << IFC_SYS_SHIFT_MOTOR_DAM);
}
//=== end SetSysStateDamMotor ======================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::SetSysStateLamp(EHalLamp halLamp, u8 stateLamp)
{
	if(halLamp == HalLamp_Front)
	{
		this->IfcSystemState.lampStates = this->IfcSystemState.lampStates & (0xFF - IFC_SYS_STATE_LAMP_FRONT);
		this->IfcSystemState.lampStates = this->IfcSystemState.lampStates | (stateLamp << IFC_SYS_SHIFT_LAMP_FRONT);
		this->stateLampFront = stateLamp;
	}
	else  // HalLamp_Back
	{
		this->IfcSystemState.lampStates = this->IfcSystemState.lampStates & (0xFF - IFC_SYS_STATE_LAMP_BACK);
		this->IfcSystemState.lampStates = this->IfcSystemState.lampStates | (stateLamp << IFC_SYS_SHIFT_LAMP_BACK);
		this->stateLampBack = stateLamp;
	}
}
//=== end SetSysStateLamp ==========================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::SetSysStateSensor(u16 typeSensor, u8 stateSensor)
{
	this->IfcSystemState.sensorStates = this->IfcSystemState.sensorStates & (0xFFFF - typeSensor);
	switch(typeSensor)
	{
		case IFC_SYS_STATE_SWITCH_LID_OPEN:
			this->IfcSystemState.sensorStates = this->IfcSystemState.sensorStates | (stateSensor << IFC_SYS_SHIFT_SWITCH_LID_OPEN);
			break;

		case IFC_SYS_STATE_SWITCH_TOP_REMOVED:
			this->IfcSystemState.sensorStates = this->IfcSystemState.sensorStates | (stateSensor << IFC_SYS_SHIFT_SWITCH_TOP_REMOVED);
			break;

		case IFC_SYS_STATE_SWITCH_LOCK_LEFT:
			this->IfcSystemState.sensorStates = this->IfcSystemState.sensorStates | (stateSensor << IFC_SYS_SHIFT_SWITCH_LOCK_LEFT);
			break;

		case IFC_SYS_STATE_SWITCH_LOCK_RIGHT:
			this->IfcSystemState.sensorStates = this->IfcSystemState.sensorStates | (stateSensor << IFC_SYS_SHIFT_SWITCH_LOCK_RIGHT);
			break;

		case IFC_SYS_STATE_SWITCH_PRESENT_TANK:
			this->IfcSystemState.sensorStates = this->IfcSystemState.sensorStates | (stateSensor << IFC_SYS_SHIFT_SWITCH_PRESENT_TANK);
			break;

		case IFC_SYS_STATE_SWITCH_PRESENT_CH_LEFT:
			this->IfcSystemState.sensorStates = this->IfcSystemState.sensorStates | (stateSensor << IFC_SYS_SHIFT_SWITCH_PRESENT_CH_LEFT);
			break;

		case IFC_SYS_STATE_SWITCH_PRESENT_CH_RIGHT:
			this->IfcSystemState.sensorStates = this->IfcSystemState.sensorStates | (stateSensor << IFC_SYS_SHIFT_SWITCH_PRESENT_CH_RIGHT);
			break;

		case IFC_SYS_STATE_SWITCH_DAM_LEFT:
			this->IfcSystemState.sensorStates = this->IfcSystemState.sensorStates | (stateSensor << IFC_SYS_SHIFT_SWITCH_DAM_LEFT);
			break;

		case IFC_SYS_STATE_SWITCH_DAM_RIGHT:
			this->IfcSystemState.sensorStates = this->IfcSystemState.sensorStates | (stateSensor << IFC_SYS_SHIFT_SWITCH_DAM_RIGHT);
			break;

		case IFC_SYS_STATE_BME688_FAN:
			this->IfcSystemState.sensorStates = this->IfcSystemState.sensorStates | (stateSensor << IFC_SYS_SHIFT_BME688_FAN);
			break;

		case IFC_SYS_STATE_BME688_LEFT:
			this->IfcSystemState.sensorStates = this->IfcSystemState.sensorStates | (stateSensor << IFC_SYS_SHIFT_BME688_LEFT);
			break;

		case IFC_SYS_STATE_BME688_RIGHT:
			this->IfcSystemState.sensorStates = this->IfcSystemState.sensorStates | (stateSensor << IFC_SYS_SHIFT_BME688_RIGHT);
			break;


	}

}
//=== end SetSysStateSwitch ========================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
TIfcSystemState* TTaskHAL::GetPointerIfcSystemState()
{
	return(&this->IfcSystemState);
}
//=== end GetPointerIfcSystemState =================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::HandlerGpioInterrupt(u16 gpioPin)
{
	if(gpioPin == IN_BACK_LAMP_Pin)
	{
		this->counterLampBack++;
	}

	if(gpioPin == IN_FRONT_LAMP_Pin)
	{
		this->counterLampFront++;
	}

	if(gpioPin == FAN_RPM_Pin)
	{
		this->Fan.IncrementCounterRpm();
	}

}
//=== end HandlerGpioInterrupt =====================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::HandlerCh101Interrupt(u16 gpioPin)
{
	if(this->enableCh101)
	{
		if(gpioPin == INT1_Pin)
		{
			this->SetEventsFromISR(TASK_HAL_EVENT_GET_CH101_LEFT);
//			this->counterExti1++;
		}

		if(gpioPin == INT2_Pin)
		{
			this->SetEventsFromISR(TASK_HAL_EVENT_GET_CH101_RIGHT);
//			this->counterExti2++;
		}

		if(gpioPin == INT3_Pin)
		{
			this->SetEventsFromISR(TASK_HAL_EVENT_GET_CH101_TANK);
//			this->counterExti3++;
		}

	}

}
//=== end HandlerCh101Interrupt ====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskHAL::Init(void)
{
	EOsResult result;


	this->enableCh101 = false;

	this->Fan.Init();

	this->ControlLamp(HalLamp_Front, HalTurn_Off);
	this->SetSysStateLamp(HalLamp_Front, IFC_ITEM_STATE_NO_STATE);
	this->ControlLamp(HalLamp_Back, HalTurn_Off);
	this->SetSysStateLamp(HalLamp_Back, IFC_ITEM_STATE_NO_STATE);

	this->DamMotor.Init();
	this->SetSysStateDamMotor(IFC_ITEM_STATE_NO_STATE);


/*	TBme688Sensors Bme688Sensors;
	Bme688Sensors.temperature = 0x0101; // dataBme688[i].temperature;
	Bme688Sensors.pressure = 0x0202; // dataBme688[i].pressure;
	Bme688Sensors.humidity = 0x0303; //(u16)dataBme688[i].humidity / 10;
	Bme688Sensors.gasResistance = 0x0404; // dataBme688[i].gas_resistance;
	Bme688Sensors.status = 0x0505;

	this->Bme688_Fan.WriteSensors(&Bme688Sensors);
	this->Bme688_Left.WriteSensors(&Bme688Sensors);
	this->Bme688_Right.WriteSensors(&Bme688Sensors); */


#ifdef __SENSORS_BME688_ENABLED

	u32 samplingTime;
	samplingTime = this->Bme688_Fan.GetSamplingTime();

	result = this->Bme688_Fan.Init(Bme688Sensor_Fan);
	if(result != OsResult_Ok)
	{
		this->flagErrorBme688_Fan = true;
		this->SetSysStateSensor(IFC_SYS_STATE_BME688_FAN, IFC_ITEM_STATE_OFF);
	}
	else
	{
		this->flagErrorBme688_Fan = false;
		this->SetSysStateSensor(IFC_SYS_STATE_BME688_FAN, IFC_ITEM_STATE_ON);
	}
	this->Bme688_Fan.SetCounterSamplingTime(samplingTime);

	result = this->Bme688_Left.Init(Bme688Sensor_Left);
	if(result != OsResult_Ok)
	{
		this->flagErrorBme688_Left = true;
		this->SetSysStateSensor(IFC_SYS_STATE_BME688_LEFT, IFC_ITEM_STATE_OFF);
	}
	else
	{
		this->flagErrorBme688_Left = false;
		this->SetSysStateSensor(IFC_SYS_STATE_BME688_LEFT, IFC_ITEM_STATE_ON);
	}
	this->Bme688_Left.SetCounterSamplingTime(samplingTime + (samplingTime / 3));

	result = this->Bme688_Right.Init(Bme688Sensor_Right);
	if(result != OsResult_Ok)
	{
		this->flagErrorBme688_Right = true;
		this->SetSysStateSensor(IFC_SYS_STATE_BME688_RIGHT, IFC_ITEM_STATE_OFF);
	}
	else
	{
		this->flagErrorBme688_Right = false;
		this->SetSysStateSensor(IFC_SYS_STATE_BME688_RIGHT, IFC_ITEM_STATE_ON);
	}
	this->Bme688_Right.SetCounterSamplingTime(samplingTime + (samplingTime / 3) + (samplingTime / 3));

#endif

#ifdef __SENSORS_CH101_ENABLED

	static TCh101& ch101 = TCh101::GetInstance();
	this->Ch101 = &ch101;
	result = this->Ch101->Init();
	if(result != OsResult_Ok)
	{
		return(result);
	}
#endif


	return(OsResult_Ok);
}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskHAL::CreateOsResources(void)
{
//	EOsResult result;


	return(OsResult_Ok);
}
//=== end CreateOsResources ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskHAL::CreateTask(void)
{
	((TOsTask*)this)->CreateTask("HAL TASK", OS_TASK_HAL_SIZE_STACK, OS_TASK_HAL_PRIORITY);
}
//=== end CreateTask ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskHAL::CreateTaskStatic(void)
{
	((TOsTask*)this)->CreateTaskStatic("HAL TASK", OS_TASK_HAL_SIZE_STACK, OS_TASK_HAL_PRIORITY, xStackBuffer, &xTaskBuffer);
}
//=== end CreateTaskStatic =========================================================
