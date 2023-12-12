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
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;


/**********************************************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hAdc)
{
	TaskHAL.SetEventAdcConvCpltFromISR(hAdc);
}
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef* hAdc)
{
	TaskHAL.SetEventAdcErrorFromISR(hAdc);
}

/* void HAL_GPIO_EXTI_Rising_Callback(u16 gpioPin)
{
	TaskHAL.HandlerGpioInterrupt(gpioPin);
} */

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
	this->Adc.StartAdc1();

	if(this->counterTimeAcMeasurement < TASK_HAL_AC_TIME_MEASUREMENT)
	{
		this->counterTimeAcMeasurement++;
	}
	else
	{
		this->counterTimeAcMeasurement = 0;

		if(this->counterAcMain < TASK_HAL_AC_PULSE_NUMBERS)
		{
			this->flagAcMainPresent = false;
		}
		else
		{
			this->flagAcMainPresent = true;
		}
		this->counterAcMain = 0;

		if(this->counterAcMotor < TASK_HAL_AC_PULSE_NUMBERS)
		{
			this->flagAcMotorPresent = false;
		}
		else
		{
			this->flagAcMotorPresent = true;
		}
		this->counterAcMotor = 0;

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
//	EOsResult result;


	this->Delay(20);

	while(true)
	{
		if(this->EventGroup.WaitAndBits(TASK_HAL_CMD_START, 1000) == OsResult_Ok)
		{
			break;
		}
	}

	this->Delay(10);


	while(true)
	{
        if(this->EventGroup.WaitOrBits(
//        			TASK_HAL_EVENT_GET_BME688	|
					TASK_HAL_EVENT_UART_RX_CPLT	|
					TASK_HAL_EVENT_UART_ERROR	|
					TASK_HAL_EVENT_SYS_COMMAND	|
					TASK_HAL_EVENT_T_READY,
					&resultBits,
					100
					) == OsResult_Timeout)
        {
            this->GetStateTopCpu();
            this->GetSensorBme688();

        	continue;
        }
      

        if((resultBits & TASK_HAL_EVENT_T_READY) > 0)
        {
        	this->CalculatingTSensors();
//        	TaskCLI.DebugPrintf("Heater");
        }
        
        if((resultBits & TASK_HAL_EVENT_UART_ERROR) > 0)
        {
            this->InterfaceMasterVIP.ReInit();
            this->Delay(2);
        }

        if((resultBits & TASK_HAL_EVENT_SYS_COMMAND) > 0)
        {
            this->ProcessSysCommand();
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
void TTaskHAL::GetStateTopCpu(void)
{
	EOsResult result;
	u8* pData;


	result = this->SendCommand(IfcVipCommand_GetState, 0);
	if(result != OsResult_Ok)
	{
		this->InterfaceMasterVIP.ReInit();
		this->Delay(10);

		result = this->SendCommand(IfcVipCommand_GetState, 0);
		if(result != OsResult_Ok)
		{
			TaskSYS.SetSysState(SysError_InterfaceVipM);
			return;
		}
	}

	pData = this->InterfaceMasterVIP.GetPointerDataRx();

	TaskSYS.UpdateTopCpuState(pData);

}
//=== end GetStateTopCpu ===========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::GetSensorBme688(void)
{
	EOsResult result;
	u8* pData;


	result = this->SendCommand(IfcVipCommand_GetBme688_Part1, 0);
	if(result != OsResult_Ok)
	{
		this->InterfaceMasterVIP.ReInit();
		this->Delay(10);

		result = this->SendCommand(IfcVipCommand_GetBme688_Part1, 0);
		if(result != OsResult_Ok)
		{
			TaskSYS.SetSysState(SysError_InterfaceVipM);
			return;
		}
	}

	pData = this->InterfaceMasterVIP.GetPointerDataRx();

	TaskSYS.UpdateSensorBme688(pData + IFC_VIP_BME688_TEMPERATURE);

}
//=== end GetSensorBme688 ==========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::ProcessSysCommand(void)
{
	EOsResult result;
	TSysCommand sysCommand;


	while(true)
	{
		result = this->OsQueue.Receive((void*)&sysCommand, 0);
		if(result == OsResult_QueueEmpty)
		{
			return;
		}

		switch(sysCommand.command)
		{
			case SysCommand_ControlHeater:
				this->ControlHeater(sysCommand.parameters);
				break;

			case SysCommand_ControlMotor:
				this->ControlMotor(sysCommand.parameters);
				break;

			case SysCommand_ControlLamp:
				this->ControlLamp(sysCommand.parameters);
				break;

			case SysCommand_SetPosition:
				break;


			default:
				break;
		}

/*		result = this->SendCommand(sysCommand.command, sysCommand.parameters);
		if(result != OsResult_Ok)
		{
			this->InterfaceMasterVIP.ReInit();
			this->Delay(10);

			result = this->SendCommand(sysCommand.command, sysCommand.parameters);
			if(result != OsResult_Ok)
			{
				TaskSYS.SetSysState(SysError_InterfaceVipM);
				return;
			}
		} */

	}

}
//=== end ProcessSysCommand ========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EOsResult TTaskHAL::ControlMotor(u8* parameters)
{


	return(OsResult_Ok);
}
//=== end ControlMotor =============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EOsResult TTaskHAL::ControlHeater(u8* parameters)
{


	return(OsResult_Ok);
}
//=== end ControlHeater ============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EOsResult TTaskHAL::ControlLamp(u8* parameters)
{


	return(OsResult_Ok);
}
//=== end ControlLamp ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::AcPowerOn()
{
	this->Gpio.AcPowerOn();
}
//=== end AcPowerOn ================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::AcPowerOff()
{
	this->Gpio.AcPowerOff();
}
//=== end AcPowerOff ===============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::StartMainMotorCW()
{
	if(this->flagAcMainPresent)
	{
		HAL_GPIO_WritePin(SW_STATOR2_GPIO_Port, SW_STATOR2_Pin, GPIO_PIN_SET);
		this->Delay(100);
		this->flagStartMainMotor = true;
	}
}
//=== end StartMainMotorCW =========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::StartMainMotorCCW()
{
	if(this->flagAcMainPresent)
	{
		HAL_GPIO_WritePin(SW_STATOR2_GPIO_Port, SW_STATOR2_Pin, GPIO_PIN_RESET);
		this->Delay(100);
		this->flagStartMainMotor = true;
	}
}
//=== end StartMainMotorCCW ========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::StopMainMotor()
{
	this->flagStartMainMotor = false;
}
//=== end StopMainMotor ============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::BrakeOnMainMotor()
{
//	HAL_GPIO_WritePin(BREAK_ON_GPIO_Port, BREAK_ON_Pin, GPIO_PIN_SET);
	HAL_GPIO_TogglePin(SW_STATOR2_GPIO_Port, SW_STATOR2_Pin);
}
//=== end BrakeOnMainMotor =========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::BrakeOffMainMotor()
{
	HAL_GPIO_WritePin(BREAK_ON_GPIO_Port, BREAK_ON_Pin, GPIO_PIN_RESET);
}
//=== end BrakeOffMainMotor =========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::CalculatingTSensors()
{
	for(u16 i = 0; i < TASK_HAL_NUMBER_T_SENSORS; i++)
	{
		switch(i)
		{
			case 0:
				this->tPtcLeft = this->CalculatingTSensor(IfcVipTemperature_PtcHeater1);
				if(this->tPtcLeft >= 50) this->tPtcLeft++;
				if(this->tPtcLeft >= 60) this->tPtcLeft++;
				break;

			case 1:
				this->tPtcRight = this->CalculatingTSensor(IfcVipTemperature_PtcHeater2);
				if(this->tPtcRight >= 50) this->tPtcRight++;
				if(this->tPtcRight >= 60) this->tPtcRight++;
				break;

			case 2:
				this->tPadLeft = this->CalculatingTSensor(IfcVipTemperature_PadHeater1);
				if(this->tPadLeft >= 40) this->tPadLeft++;
				if(this->tPadLeft >= 50) this->tPadLeft++;
				if(this->tPadLeft >= 60) this->tPadLeft++;
				break;

			case 3:
				this->tPadRight = this->CalculatingTSensor(IfcVipTemperature_PadHeater2);
				if(this->tPadRight >= 40) this->tPadRight++;
				if(this->tPadRight >= 50) this->tPadRight++;
				if(this->tPadRight >= 60) this->tPadRight++;
				break;

			default:
				break;
		}

	}

//	DiagPrintf("PTC temperature - %d", this->tPtc);

}
//=== end CalculatingTSensors ======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
s8 TTaskHAL::CalculatingTSensor(EIfcVipTemperature ifcVipTemperature)
{
	u16 adcTSensor;


	taskENTER_CRITICAL();
	switch(ifcVipTemperature)
	{
		case IfcVipTemperature_PtcHeater1:
			adcTSensor = this->adcTPtcLeft;
			break;

		case IfcVipTemperature_PtcHeater2:
			adcTSensor = this->adcTPtcRight;
			break;

		case IfcVipTemperature_PadHeater1:
			adcTSensor = this->adcTPadLeft;
			break;

		case IfcVipTemperature_PadHeater2:
			adcTSensor = this->adcTPadRight;
			break;

		default:
			return(-5);
	}
	taskEXIT_CRITICAL();


	if(adcTSensor > TASK_HAL_TH_0_DEG_C)
	{
		return(-1);
	}

	if(adcTSensor < TASK_HAL_TH_75_DEG_C)
	{
		return(80);
	}


	if(adcTSensor > TASK_HAL_TH_5_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_5_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_10_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_10_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_15_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_15_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_20_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_20_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_25_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_25_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_30_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_30_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_35_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_35_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_40_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_40_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_45_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_45_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_50_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_50_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_55_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_55_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_60_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_60_DEG_C));
	}

	if(adcTSensor > TASK_HAL_TH_65_DEG_C)
	{
		return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_65_DEG_C));
	}


	return(this->CalculatingTSensorDigC(adcTSensor, TASK_HAL_TH_70_DEG_C));
}
//=== end CalculatingTSensor =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
s8 TTaskHAL::CalculatingTSensorDigC(u16 localAdcTHeater, u16 adcLowLevel)
{
	u16 rangeAdc;
	u16 adcHighLevel;
	s8 lowLevelT;


	switch(adcLowLevel)
	{
		case TASK_HAL_TH_5_DEG_C:
			adcHighLevel = TASK_HAL_TH_0_DEG_C;
			lowLevelT = 0;
			break;

		case TASK_HAL_TH_10_DEG_C:
			adcHighLevel = TASK_HAL_TH_5_DEG_C;
			lowLevelT = 5;
			break;

		case TASK_HAL_TH_15_DEG_C:
			adcHighLevel = TASK_HAL_TH_10_DEG_C;
			lowLevelT = 10;
			break;

		case TASK_HAL_TH_20_DEG_C:
			adcHighLevel = TASK_HAL_TH_15_DEG_C;
			lowLevelT = 15;
			break;

		case TASK_HAL_TH_25_DEG_C:
			adcHighLevel = TASK_HAL_TH_20_DEG_C;
			lowLevelT = 20;
			break;

		case TASK_HAL_TH_30_DEG_C:
			adcHighLevel = TASK_HAL_TH_25_DEG_C;
			lowLevelT = 25;
			break;

		case TASK_HAL_TH_35_DEG_C:
			adcHighLevel = TASK_HAL_TH_30_DEG_C;
			lowLevelT = 30;
			break;

		case TASK_HAL_TH_40_DEG_C:
			adcHighLevel = TASK_HAL_TH_35_DEG_C;
			lowLevelT = 35;
			break;

		case TASK_HAL_TH_45_DEG_C:
			adcHighLevel = TASK_HAL_TH_40_DEG_C;
			lowLevelT = 40;
			break;

		case TASK_HAL_TH_50_DEG_C:
			adcHighLevel = TASK_HAL_TH_45_DEG_C;
			lowLevelT = 45;
			break;

		case TASK_HAL_TH_55_DEG_C:
			adcHighLevel = TASK_HAL_TH_50_DEG_C;
			lowLevelT = 50;
			break;

		case TASK_HAL_TH_60_DEG_C:
			adcHighLevel = TASK_HAL_TH_55_DEG_C;
			lowLevelT = 55;
			break;

		case TASK_HAL_TH_65_DEG_C:
			adcHighLevel = TASK_HAL_TH_60_DEG_C;
			lowLevelT = 60;
			break;

		case TASK_HAL_TH_70_DEG_C:
			adcHighLevel = TASK_HAL_TH_65_DEG_C;
			lowLevelT = 65;
			break;


		default:  // TASK_HAL_TS_75_DEG_C
			adcHighLevel = TASK_HAL_TH_70_DEG_C;
			lowLevelT = 70;
			break;
	}

	rangeAdc = adcHighLevel - adcLowLevel;
	rangeAdc /= 5;

	if(localAdcTHeater >= (adcLowLevel + rangeAdc * 4))
	{
		return(lowLevelT);
	}

	if(localAdcTHeater >= (adcLowLevel + rangeAdc * 3))
	{
		return(lowLevelT + 1);
	}

	if(localAdcTHeater >= (adcLowLevel + rangeAdc * 2))
	{
		return(lowLevelT + 2);
	}

	if(localAdcTHeater >= (adcLowLevel + rangeAdc))
	{
		return(lowLevelT + 3);
	}


	return(lowLevelT + 4);
}
//=== end CalculatingTSensorDigC ===================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
u16 TTaskHAL::GetAdcValue(EAdcChannel adcChanel)
{
	u16 adcValue;


	switch(adcChanel)
	{
/*		case AdcChannel_Input6: adcValue = this->ArraySensors.SensorOxygen1.GetState(); break;
		case AdcChannel_Input8: adcValue = this->ArraySensors.SensorOxygen2.GetState(); break;
		case AdcChannel_Input9: adcValue = this->ArraySensors.SensorOxygen3.GetState(); break;
		case AdcChannel_THeater: adcValue = this->ArraySensors.SensorMethane1.GetState(); break;
		case AdcChannel_OzoneLamp: adcValue = this->ArraySensors.SensorMethane2.GetState(); break;
		case AdcChannel_UvLamp: adcValue = this->ArraySensors.SensorMethane3.GetState(); break;
		case AdcChannel_TCpu: adcValue = 0; break;
		case AdcChannel_Vref: adcValue = 0; break; */

		default:
			adcValue = 0;
	}


	return(adcValue);
}
//=== end GetAdcValue ==============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
s8 TTaskHAL::GetTemperature(EIfcVipTemperature ifcVipTemperature)
{
	s8 temperature;

	switch(ifcVipTemperature)
	{
		case IfcVipTemperature_PadHeater1:
			temperature = this->GetTemperaturePadLeft();
			break;

		case IfcVipTemperature_PadHeater2:
			temperature = this->GetTemperaturePadRight();
			break;

		case IfcVipTemperature_Mcu2:
			temperature = this->GetTemperatureCpu2();
			break;

		case IfcVipTemperature_Mcu3:
			temperature = this->GetTemperatureCpu3();
			break;

		case IfcVipTemperature_PtcHeater1:
			temperature = this->GetTemperaturePtcLeft();
			break;

		case IfcVipTemperature_PtcHeater2:
			temperature = this->GetTemperaturePtcRight();
			break;


		default:
			temperature = 0;
	}


	return(temperature);
}
//=== end GetTemperature ===========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
/* void TTaskHAL::ReadZ1911()
{
	u32 resultBits;
	u8 data[IFC_Z1911_UART_SIZE_PACKET];


	this->InterfaceZ1911.StartRxData();

	memset((void*)data, 0, IFC_Z1911_UART_SIZE_PACKET);
	data[IFC_Z1911_INDEX_START_BYTE] = IFC_Z1911_START_BYTE;
	data[IFC_Z1911_SENSOR_NUM] = IFC_Z1911_SENSOR_NUM;
	data[IFC_Z1911_INDEX_CMD_TX] = IFC_Z1911_CMD_READ;
	this->InterfaceZ1911.StartTxData(data);

	if(this->EventGroup.WaitOrBits(
	        			TASK_HAL_EVENT_UART2_ERROR	|
						TASK_HAL_EVENT_UART2_RX_CPLT,
						&resultBits,
						100
						) == OsResult_Timeout)
	{
		this->SetEvents(TASK_HAL_EVENT_UART2_ERROR);
		return;
	}

	////// if result == Ok. //////
	if((resultBits & TASK_HAL_EVENT_UART2_ERROR) > 0)
	{
		this->SetEvents(TASK_HAL_EVENT_UART2_ERROR);
		return;
	}

	////// if(resultBits & TASK_HAL_EVENT_UART2_RX_CPLT) > 0) //////
} */
//=== end ReadZ1911 ================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
void TTaskHAL::AdcConversionComplete()
{
	u16* ptrBuffer;


	this->Adc.StopDmaAdc1();

	ptrBuffer = this->Adc.GetPtrBufferAdc1();
	for(u16 i = 0; i < ADC1_MAX_NUMBER_CHANNEL; i++)
	{
		this->calculationResultAdc1[i] += ptrBuffer[i];
	}

	this->adcIndexConversion++;

	if(this->adcIndexConversion >= HAL_ADC_NUMBER_CONVERSION)
	{
#ifdef __DEBUG_ADC_CALIBRATION_ENABLED
		u16* adrAdcVrefCalibration;
		u16 adcVrefCalibration;
		u16 tmpU16;
		float kVref;


		adrAdcVrefCalibration = (u16*)ADC_VREF_ADDRESS;
		adcVrefCalibration = (*adrAdcVrefCalibration);
		tmpU16 = this->calculationResultAdc1[ADC_VREF_CHANNEL] / HAL_ADC_NUMBER_CONVERSION;
		kVref = ADC_VREF_CALIBRATION * adcVrefCalibration / tmpU16;
		kVref = kVref / ADC_VREF_WORKING;
#else
		u16 kVref;
		u16 tmpU16;



		kVref = 1;
#endif

		tmpU16 = (u16)(this->calculationResultAdc1[ADC1_IN0_AC_CURRENT] / HAL_ADC_NUMBER_CONVERSION * kVref);

		this->indexAccumulativeT++;
		if(this->indexAccumulativeT >= HAL_T_NUMBER_CONVERSION)
		{
			this->adcTPadLeft = (u16)(this->accumulativeTPadLeft / HAL_T_NUMBER_CONVERSION);
			this->adcTPadRight = (u16)(this->accumulativeTPadRight / HAL_T_NUMBER_CONVERSION);
			this->adcTPtcLeft = (u16)(this->accumulativeTPtcLeft / HAL_T_NUMBER_CONVERSION);
			this->adcTPtcRight = (u16)(this->accumulativeTPtcRight / HAL_T_NUMBER_CONVERSION);

			this->indexAccumulativeT = 0;
			this->accumulativeTPadLeft = 0;
			this->accumulativeTPadRight = 0;
			this->accumulativeTPtcLeft = 0;
			this->accumulativeTPtcRight = 0;
			this->SetEventsFromISR(TASK_HAL_EVENT_T_READY);
		}
		else
		{
			u16 calcT;

			tmpU16 = (u16)(this->calculationResultAdc1[ADC1_IN1_T_PAD_LEFT] / HAL_ADC_NUMBER_CONVERSION * kVref);
			calcT = (this->lastTPadLeft[0] + this->lastTPadLeft[1] +  tmpU16) / 3;
			this->lastTPadLeft[0] = this->lastTPadLeft[1];
			this->lastTPadLeft[1] =  tmpU16;
			this->accumulativeTPadLeft += calcT;

			tmpU16 = (u16)(this->calculationResultAdc1[ADC1_IN2_T_PAD_RIGHT] / HAL_ADC_NUMBER_CONVERSION * kVref);
			calcT = (this->lastTPadRight[0] + this->lastTPadRight[1] +  tmpU16) / 3;
			this->lastTPadRight[0] = this->lastTPadRight[1];
			this->lastTPadRight[1] =  tmpU16;
			this->accumulativeTPadRight += calcT;

			tmpU16 = (u16)(this->calculationResultAdc1[ADC1_IN3_T_PTC_LEFT] / HAL_ADC_NUMBER_CONVERSION * kVref);
			calcT = (this->lastTPtcLeft[0] + this->lastTPtcLeft[1] +  tmpU16) / 3;
			this->lastTPtcLeft[0] = this->lastTPtcLeft[1];
			this->lastTPtcLeft[1] =  tmpU16;
			this->accumulativeTPtcLeft += calcT;

			tmpU16 = (u16)(this->calculationResultAdc1[ADC1_IN4_T_PTC_RIGHT] / HAL_ADC_NUMBER_CONVERSION * kVref);
			calcT = (this->lastTPtcRight[0] + this->lastTPtcRight[1] +  tmpU16) / 3;
			this->lastTPtcRight[0] = this->lastTPtcRight[1];
			this->lastTPtcRight[1] =  tmpU16;
			this->accumulativeTPtcRight += calcT;


		}



		this->adcIndexConversion = 0;
		memset((void*)calculationResultAdc1, 0, ADC1_MAX_NUMBER_CHANNEL * sizeof(u16));

	}



}
//=== end AdcConversionComplete ====================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::SetEventAdcConvCpltFromISR(ADC_HandleTypeDef* hAdc)
{
	this->AdcConversionComplete();
}
//=== end SetEventAdcConvCpltFromISR ===============================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::SetEventAdcErrorFromISR(ADC_HandleTypeDef* hAdc)
{
	// DEBUG
	// this->DebugGpio.ResetPin1();
	// DEBUG
}
//=== end SetEventAdcErrorFromISR ==================================================

//==================================================================================
/**
*  The function ... .
*
*  @return
*  		none.
*/
void TTaskHAL::HandlerGpioInterrupt(u16 gpioPin)
{
	if(gpioPin == AC_MAIN_Pin)
	{
		this->counterAcMain++;

/*		if(this->PadHeaterLeft.GetPwm() > this->counterPwmHeater)
		{
			this->PadHeaterLeft.PulseOn();
		}
		else
		{
			this->PadHeaterLeft.PulseOff();
		} */

		if(this->acPhase)
		{
			this->counterPwmHeater++;

			if(this->counterPwmHeater >= TASK_HAL_AC_MAX_PWM_HEATER)
			{
				this->counterPwmHeater = 0;
			}
		}

		this->acPhase = !this->acPhase;

		if(this->flagStartMainMotor)
		{
			HAL_GPIO_WritePin(MOTOR_ON_GPIO_Port, MOTOR_ON_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(MOTOR_ON_GPIO_Port, MOTOR_ON_Pin, GPIO_PIN_RESET);
		}

	}

	if(gpioPin == AC_MOTOR_Pin)
	{
		this->counterAcMotor++;
	}






}
//=== end HandlerGpioInterrupt =====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskHAL::SetEventUart2_TxCpltFromISR(void)
{

}
//=== end SetEventUart5_TxCpltFromISR ==============================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskHAL::SetEventUart2_RxCpltFromISR(void)
{
	if(this->InterfaceMasterVIP.ParsingRxPacket() == OsResult_Ok)
	{
		this->SetEventsFromISR(TASK_HAL_EVENT_UART_RX_CPLT);
	}
	else
	{
		this->SetEventsFromISR(TASK_HAL_EVENT_UART_ERROR);
	}

}
//=== end SetEventUart2_RxCpltFromISR ==============================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskHAL::SetEventUart2_ErrorFromISR(void)
{
	this->SetEventsFromISR(TASK_HAL_EVENT_UART_ERROR);
}
//=== end SetEventUart2_ErrorFromISR ===============================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskHAL::SendSysCommand(TSysCommand* sysCommand)
{
	EOsResult result;


	result= this->OsQueue.Send(sysCommand, 20); // timeout 20 mSec
	if(result != OsResult_Ok)
	{
		return(result);
	}

	this->EventGroup.SetBits(TASK_HAL_EVENT_SYS_COMMAND);

	this->Delay(4);

	return(OsResult_Ok);
}
//=== end SendSysCommand ===========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return
*  		none.
*/
EOsResult TTaskHAL::SendCommand(EIfcVipCommand command, u8* pBuffer)
{
	u32 resultBits;
	EOsResult result;
	u16 i;


//	this->Delay(10);

//	for(i = 0; i < 4; i ++)
//	{
		// this->InterfaceEvseController.ReInit();

	this->InterfaceMasterVIP.StartRxData();
	this->InterfaceMasterVIP.StartTxData(command, pBuffer);

	result = this->EventGroup.WaitOrBits(
						TASK_HAL_EVENT_UART_RX_CPLT |
						TASK_HAL_EVENT_UART_ERROR   |
						TASK_HAL_EVENT_UART_RX_TIMEOUT,
						&resultBits,
						40);

	if(result == OsResult_Timeout)
	{
		return(OsResult_Timeout);
	}

	if((resultBits & TASK_HAL_EVENT_UART_RX_CPLT) > 0)
	{
		this->SetEvents(TASK_HAL_EVENT_UART_RX_CPLT);
		return(OsResult_Ok);
	}

	if((resultBits & TASK_HAL_EVENT_UART_ERROR) > 0)
	{
		return(OsResult_ErrorI2cReceive);
	}

/*	if((resultBits & TASK_HAL_EVENT_UART_RX_TIMEOUT) > 0)
	{
		return(OsResult_Timeout);
	} */


//	}


	return(OsResult_Timeout);
}
//=== end SendCommand ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskHAL::Init(void)
{
//	EOsResult result;


	this->adcIndexConversion = 0;
	this->accumulativeTPadLeft = 0;
	this->accumulativeTPadRight = 0;
	this->accumulativeTPtcLeft = 0;
	this->accumulativeTPtcRight = 0;
	this->indexAccumulativeT = 0;
	this->lastTPadLeft[0] = this->lastTPadLeft[1] = 2000;
	this->lastTPadRight[0] = this->lastTPadRight[1] = 2000;
	this->lastTPtcLeft[0] = this->lastTPtcLeft[1] = 2000;
	this->lastTPtcRight[0] = this->lastTPtcRight[1] = 2000;
	
	this->InterfaceMasterVIP.Init(huart2, USART2);

	this->counterPwmHeater = 0;
	this->pwmPtcFan = 100;

	this->acPhase = false;
	this->flagAcMainPresent = false;
	this->flagAcMotorPresent = false;
	this->flagStartMainMotor = false;


	this->PtcHeaterLeft.Init(Heater_PtcHeaterLeft);
	this->PtcHeaterRight.Init(Heater_PtcHeaterRight);
	this->PadHeaterLeft.Init(Heater_PadHeaterLeft);
	this->PadHeaterRight.Init(Heater_PadHeaterRight);

	this->PtcFanLeft.Init(PtcFan_Left);
	this->PtcFanRight.Init(PtcFan_Right);


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
	EOsResult result;

	result= this->OsQueue.Create(TASK_HAL_MAX_NUMBER_COMMANDS, TASK_HAL_SIZE_PARAM_COMMANDS);
	if(result != OsResult_Ok)
	{
		return(result);
	}

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
