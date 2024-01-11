/**********************************************************************************/
/**
 *  @file Bme688.cpp
 *  @brief ...
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include <Bme688.hpp>


/**********************************************************************************/
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;


/**********************************************************************************/
const u16 TBme688::temperatureProf[] = { 320, 100, 100, 100, 200, 200, 200, 320, 320, 320 };  // Temperature
const u16 TBme688::multiplierProf[] = { 5, 2, 10, 30, 5, 5, 5, 5, 5, 5 };                     // Time


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TBme688::Init(EBme688Sensor bme688Sensor)
{
	EOsResult result;
	u8 regData;


	if(bme688Sensor == Bme688Sensor_Fan)
	{
		this->hI2c = &hi2c2;
		this->chipAddress = I2C_ADDRESS_BME688;
	}
	else
	{
		this->hI2c = &hi2c1;

		if(bme688Sensor == Bme688Sensor_Left)
		{
			this->chipAddress = I2C_ADDRESS_BME688;
		}
		else
		{
			this->chipAddress = I2C_ADDRESS_BME688 + 1;
		}
	}

	result = this->WriteRegister(BME688_REG_RESET, BME688_RESET_CMD);
	if(result != OsResult_Ok)
	{
		return(result);
	}

	this->Delay(4);

	result = this->ReadRegister(BME688_REG_CHIP_ID, &regData);
	if(result != OsResult_Ok)
	{
		return(result);
	}


	if(regData != BME688_CHIP_ID)
	{
		return(OsResult_Error);
	}

	result = this->GetCalibrationData();
	if(result != OsResult_Ok)
	{
		return(result);
	}


	return(OsResult_Ok);
}
//=== end Init =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TBme688::StartMeasurement()
{
	EOsResult result;
	u8 regData;


/*	conf.filter = BME68X_FILTER_OFF;
	conf.odr = BME68X_ODR_NONE;
	conf.os_hum = BME68X_OS_1X;
	conf.os_pres = BME68X_OS_16X;
	conf.os_temp = BME68X_OS_2X; */

	result = this->ReadRegister(BME688_REG_CTRL_MEAS, &regData);
	if(result != OsResult_Ok)
	{
		return(result);
	}

	regData = regData & 0x03;                      // reset temperature and pressure oversampling
	regData = regData | (u8)(BME688_OS_2X << 5);   // set temperature oversampling
	regData = regData | (u8)(BME688_OS_16X << 2);  // set pressure oversampling
	result = this->WriteRegister(BME688_REG_CTRL_MEAS, regData);
	if(result != OsResult_Ok)
	{
		return(result);
	}

	result = this->ReadRegister(BME688_REG_CTRL_HUM, &regData);
	if(result != OsResult_Ok)
	{
		return(result);
	}

	regData = (regData & 0xF8) | (u8)BME688_OS_1X;  // // set humidity oversampling
	result = this->WriteRegister(BME688_REG_CTRL_HUM, regData);
	if(result != OsResult_Ok)
	{
		return(result);
	}

	result = this->ReadRegister(BME688_REG_CONFIG, &regData);
	if(result != OsResult_Ok)
	{
		return(result);
	}

	regData = (regData & 0xE3) | (u8)BME688_FILTER_OFF;
	result = this->WriteRegister(BME688_REG_CONFIG, regData);
	if(result != OsResult_Ok)
	{
		return(result);
	}

	regData = this->CalculateHeaterDurShared(BME688_SHARED_HEATER_DURATION);
	result = this->WriteRegister(BME688_REG_SHD_HEATR_DUR, regData);
	if(result != OsResult_Ok)
	{
		return(result);
	}

	for(u16 i = 0; i < 10; i++)
	{
		regData = this->CalculateHeaterResistance(this->temperatureProf[i]);
		result = this->WriteRegister(BME688_REG_RES_HEAT0 + i, regData);
		if(result != OsResult_Ok)
		{
			return(result);
		}

		result = this->WriteRegister(BME688_REG_GAS_WAIT0 + i, this->multiplierProf[i]);
		if(result != OsResult_Ok)
		{
			return(result);
		}

	}

	////// Set run_gas to '1' and nb_conv to '10' //////
	result = this->ReadRegister(BME688_REG_CTRL_GAS_1, &regData);
	if(result != OsResult_Ok)
	{
		return(result);
	}

	regData = regData & 0xD0;
	regData = regData | 0x20 | 0x0A;

	result = this->WriteRegister(BME688_REG_CTRL_GAS_1, regData);
	if(result != OsResult_Ok)
	{
		return(result);
	}

	////// Set mode to '0b10' //////
	result = this->ReadRegister(BME688_REG_CTRL_MEAS, &regData);
	if(result != OsResult_Ok)
	{
		return(result);
	}

	regData = regData & 0xFC;
	regData = regData | 0x02;

	result = this->WriteRegister(BME688_REG_CTRL_MEAS, regData);
	if(result != OsResult_Ok)
	{
		return(result);
	}




	return(OsResult_Ok);
}
//=== end StartMeasurement =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u32 TBme688::GetSamplingTime()
{
	return(BME688_MEAS_DURATION + ((u32)BME688_SHARED_HEATER_DURATION + 1000));

}
//=== end GetSamplingTime ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TBme688::SetCounterSamplingTime(u32 time)
{
	this->counterSamplingTime = time;
}
//=== end SetCounterSamplingTime ===================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u32 TBme688::GetCounterSamplingTime()
{
	return(this->counterSamplingTime);
}
//=== end GetCounterSamplingTime ===================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TBme688::DecreaseCounterSamplingTime()
{
	this->counterSamplingTime--;
}
//=== end DecreaseCounterSamplingTime ==============================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TBme688::WriteSensors(TBme688Sensors* Bme688Sensors)
{
	memcpy((void*)&this->Bme688Sensors, (void*)Bme688Sensors, sizeof(TBme688Sensors));
}
//=== end WriteSensors =============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TBme688::GetData(DataBme688* dataBme688[])
{
	EOsResult result;
	u8 bufferData[BME688_LEN_FIELD * 3] = { 0 };
	u8 set_val[30] = { 0 };  // idac, res_heat, gas_wait


	for(u16 i = 0; i < (BME688_LEN_FIELD * 3); i++)
	{
		result = this->ReadRegister((BME688_REG_FIELD0 + i), &bufferData[i]);
		if(result != OsResult_Ok)
		{
			return(result);
		}
	}

	for(u16 i = 0; i < 30; i++)
	{
		result = this->ReadRegister((BME688_REG_IDAC_HEAT0 + i), &set_val[i]);
		if(result != OsResult_Ok)
		{
			return(result);
		}
	}

	for(u16 i = 0; i < 3; i++)
	{
	    u16 indexBuffer;
	    u32 adc_pres;
	    u32 adc_temp;
	    u16 adc_hum;
//	    u16 adc_gas_res_low;
	    u16 adc_gas_res_high;
//	    u8 gas_range_l;
	    u8 gas_range_h;


	    indexBuffer = i * BME688_LEN_FIELD;
	    dataBme688[i]->status = bufferData[indexBuffer] & BME688_NEW_DATA_MSK;
	    dataBme688[i]->gas_index = bufferData[indexBuffer] & BME688_GAS_INDEX_MSK;
	    dataBme688[i]->meas_index = bufferData[indexBuffer + 1];

	    ////// read the raw data from the sensor //////
	    adc_pres =  (u32)(((u32)bufferData[indexBuffer + 2] * 4096) | ((u32)bufferData[indexBuffer + 3] * 16)
	                | ((u32)bufferData[indexBuffer + 4] / 16));

	    adc_temp = (u32)(((u32)bufferData[indexBuffer + 5] * 4096) | ((u32) bufferData[indexBuffer + 6] * 16)
	               | ((u32) bufferData[indexBuffer + 7] / 16));

	    adc_hum = (u16)(((u32)bufferData[indexBuffer + 8] * 256) | (u32)bufferData[indexBuffer + 9]);

//	    adc_gas_res_low = (u16)((u32)bufferData[indexBuffer + 13] * 4 | (((u32) bufferData[indexBuffer + 14]) / 64));
	    adc_gas_res_high = (u16)((u32)bufferData[indexBuffer + 15] * 4 | (((u32) bufferData[indexBuffer + 16]) / 64));
//	    gas_range_l = bufferData[indexBuffer + 14] & BME688_GAS_RANGE_MSK;
	    gas_range_h = bufferData[indexBuffer + 16] & BME688_GAS_RANGE_MSK;

	    dataBme688[i]->status |= bufferData[indexBuffer + 16] & BME688_GASM_VALID_MSK;
	    dataBme688[i]->status |= bufferData[indexBuffer + 16] & BME688_HEAT_STAB_MSK;


	    dataBme688[i]->idac = set_val[dataBme688[i]->gas_index];
	    dataBme688[i]->res_heat = set_val[10 + dataBme688[i]->gas_index];
	    dataBme688[i]->gas_wait = set_val[20 + dataBme688[i]->gas_index];
	    dataBme688[i]->temperature = this->CalculateTemperature(adc_temp);
	    dataBme688[i]->pressure = this->CalculatePressure(adc_pres);
	    dataBme688[i]->humidity = this->CalculateHumidity(adc_hum);

	    dataBme688[i]->gas_resistance = this->CalculateGasResistanceHigh(adc_gas_res_high, gas_range_h);

	}


	return(OsResult_Ok);
}
//=== end GetData ==================================================================

//==================================================================================
/**
*  This internal API is used to calculate the temperature value.
*
*  @return ... .
*/
s16 TBme688::CalculateTemperature(u32 adc)
{
    s64 var1;
    s64 var2;
    s64 var3;
    s16 calcTemp;

    ////// lint -save -e701 -e702 -e704 //////
    var1 = ((s32)adc >> 3) - ((s32)this->CalibData.par_t1 << 1);
    var2 = (var1 * (s32)this->CalibData.par_t2) >> 11;
    var3 = ((var1 >> 1) * (var1 >> 1)) >> 12;
    var3 = (var3 * ((s32)this->CalibData.par_t3 << 4)) >> 14;
    this->CalibData.t_fine = (s32)(var2 + var3);
    calcTemp = (s16)(((this->CalibData.t_fine * 5) + 128) >> 8);


    ///// lint -restore //////
    return(calcTemp);
}
//=== end CalculateTemperature =====================================================

//==================================================================================
/**
*  This internal API is used to calculate the pressure value.
*
*  @return ... .
*/
u32 TBme688::CalculatePressure(u32 adc)
{
    s32 var1;
    s32 var2;
    s32 var3;
    s32 pressureComp;


    /* This value is used to check precedence to multiplication or division
     * in the pressure compensation equation to achieve least loss of precision and
     * avoiding overflows.
     * i.e Comparing value, pres_ovf_check = (1 << 31) >> 1
     */
    const s32 pres_ovf_check = 0x40000000;

    ////// lint -save -e701 -e702 -e713 //////
    var1 = (((s32)this->CalibData.t_fine) >> 1) - 64000;
    var2 = ((((var1 >> 2) * (var1 >> 2)) >> 11) * (s32)this->CalibData.par_p6) >> 2;
    var2 = var2 + ((var1 * (s32)this->CalibData.par_p5) << 1);
    var2 = (var2 >> 2) + ((s32)this->CalibData.par_p4 << 16);
    var1 = (((((var1 >> 2) * (var1 >> 2)) >> 13) * ((s32)this->CalibData.par_p3 << 5)) >> 3)
           + (((s32)this->CalibData.par_p2 * var1) >> 1);
    var1 = var1 >> 18;
    var1 = ((32768 + var1) * (s32)this->CalibData.par_p1) >> 15;
    pressureComp = 1048576 - adc;
    pressureComp = (s32)((pressureComp - (var2 >> 12)) * ((s32)3125));
    if (pressureComp >= pres_ovf_check)
    {
        pressureComp = ((pressureComp / var1) << 1);
    }
    else
    {
        pressureComp = ((pressureComp << 1) / var1);
    }

    var1 = ((s32)this->CalibData.par_p9 * (s32)(((pressureComp >> 3) * (pressureComp >> 3)) >> 13)) >> 12;
    var2 = ((s32)(pressureComp >> 2) * (s32)this->CalibData.par_p8) >> 13;
    var3 = ((s32)(pressureComp >> 8) * (s32)(pressureComp >> 8) * (s32)(pressureComp >> 8)
           * (s32)this->CalibData.par_p10) >> 17;
    pressureComp = (s32)(pressureComp) + ((var1 + var2 + var3 + ((s32)this->CalibData.par_p7 << 7)) >> 4);


    ////// lint -restore //////
    return((u32)pressureComp);
}
//=== end CalculatePressure ========================================================

//==================================================================================
/**
*  This internal API is used to calculate the humidity in integer.
*
*  @return ... .
*/
u32 TBme688::CalculateHumidity(u16 adc)
{
    s32 var1;
    s32 var2;
    s32 var3;
    s32 var4;
    s32 var5;
    s32 var6;
    s32 tempScaled;
    s32 calcHumidity;

    ////// lint -save -e702 -e704 //////
    tempScaled = (((s32)this->CalibData.t_fine * 5) + 128) >> 8;
    var1 = (s32)(adc - ((s32)((s32)this->CalibData.par_h1 * 16)))
           - (((tempScaled * (s32)this->CalibData.par_h3) / ((s32)100)) >> 1);
    var2 = ((s32)this->CalibData.par_h2 * (((tempScaled * (s32)this->CalibData.par_h4) / ((s32)100))
           + (((tempScaled * ((tempScaled * (s32)this->CalibData.par_h5) / ((s32)100))) >> 6) / ((s32)100))
           + (s32)(1 << 14))) >> 10;
    var3 = var1 * var2;
    var4 = (s32)this->CalibData.par_h6 << 7;
    var4 = ((var4) + ((tempScaled * (s32)this->CalibData.par_h7) / ((s32)100))) >> 4;
    var5 = ((var3 >> 14) * (var3 >> 14)) >> 10;
    var6 = (var4 * var5) >> 1;
    calcHumidity = (((var3 + var6) >> 10) * ((s32)1000)) >> 12;

    if (calcHumidity > 100000) // Cap at 100%rH
    {
    	calcHumidity = 100000;
    }
    else if (calcHumidity < 0)
    {
    	calcHumidity = 0;
    }


    ////// lint -restore //////
    return((u32)calcHumidity);
}
//=== end CalculateHumidity ========================================================

//==================================================================================
/**
*  This internal API is used to calculate the gas resistance.
*
*  @return ... .
*/
u32 TBme688::CalculateGasResistanceHigh(u16 adc, u8 gasRange)
{
    u32 calcGasResistance;
    u32 var1;
    s32 var2;


    var1 = 262144 >> gasRange;
    var2 = (s32)adc - 512;
    var2 *= 3;
    var2 = 4096 + var2;

    ////// multiplying 10000 then dividing then multiplying by 100 instead of multiplying by 1000000 to prevent overflow //////
    calcGasResistance = ((u32)10000 * var1) / (u32)var2;
    calcGasResistance = calcGasResistance * 100;


    return(calcGasResistance);
}
//=== end CalculateGasResistanceHigh ===============================================

//==================================================================================
/**
*  This internal API is used to calculate the register value for shared heater duration.
*
*  @return ... .
*/
u8 TBme688::CalculateHeaterDurShared(u16 duration)
{
    u8 factor;
    u8 heaterDurValue;
    u16 tempDuration;


    if(duration >= 0x783)
    {
    	return(0xFF);  // Max duration
    }


    ////// Step size of 0.477ms //////
    tempDuration = (u16)(((u32)duration * 1000) / 477);
    factor = 0;
    while(tempDuration > 0x3F)
    {
    	tempDuration = tempDuration >> 2;
        factor += 1;
    }

    heaterDurValue = (u8)(tempDuration + (factor * 64));


    return(heaterDurValue);
}
//=== end CalculateHeaterDurShared =================================================

//==================================================================================
/**
*  This internal API is used to calculate the heater resistance value.
*
*  @return ... .
*/
u8 TBme688::CalculateHeaterResistance(u16 temperature)
{
    u8 heaterResistance;
    s32 var1;
    s32 var2;
    s32 var3;
    s32 var4;
    s32 var5;
    s32 heaterResistance_x100;


    if (temperature > 400) // Cap temperature
    {
        return(0);
    }


    var1 = (((s32)BME688_AMB_TEMPERATURE * this->CalibData.par_gh3) / 1000) * 256;
    var2 = (this->CalibData.par_gh1 + 784) * (((((this->CalibData.par_gh2 + 154009) * temperature * 5) / 100) + 3276800) / 10);
    var3 = var1 + (var2 / 2);
    var4 = (var3 / (this->CalibData.res_heat_range + 4));
    var5 = (131 * (s32)this->CalibData.res_heat_val) + 65536;

    heaterResistance_x100 = (s32)(((var4 / var5) - 250) * 34);
    heaterResistance = (u8)((heaterResistance_x100 + 50) / 100);


    return(heaterResistance);
}
//=== end CalculateHeaterResistance ================================================

//==================================================================================
/**
*  This internal API is used to read the calibration coefficients.
*
*  @return ... .
*/
EOsResult TBme688::GetCalibrationData()
{
	EOsResult result;
    u8 coefficientsArray[BME688_LEN_COEFF_ALL];
    u8 regAddress;



    regAddress = BME688_REG_COEFF1;
    for(u16 i = 0; i < BME688_LEN_COEFF1; i++)
    {
    	result = this->ReadRegister(regAddress, &coefficientsArray[i]);
    	if(result != OsResult_Ok)
    	{
    		return(result);
    	}

    	regAddress++;
    }

    regAddress = BME688_REG_COEFF2;
    for(u16 i = 0; i < BME688_LEN_COEFF2; i++)
    {
      	result = this->ReadRegister(regAddress, &coefficientsArray[BME688_LEN_COEFF1 + i]);
       	if(result != OsResult_Ok)
       	{
       		return(result);
       	}

       	regAddress++;
    }

    regAddress = BME688_REG_COEFF3;
    for(u16 i = 0; i < BME688_LEN_COEFF3; i++)
    {
    	result = this->ReadRegister(regAddress, &coefficientsArray[BME688_LEN_COEFF1 + BME688_LEN_COEFF2 + i]);
    	if(result != OsResult_Ok)
    	{
    		return(result);
        }

    	regAddress++;
    }


    ////// Temperature related coefficients //////
    this->CalibData.par_t1 = ((u16)coefficientsArray[BME688_IDX_T1_MSB] << 8) | coefficientsArray[BME688_IDX_T1_LSB];
    this->CalibData.par_t2 = (s16)(((u16)coefficientsArray[BME688_IDX_T2_MSB] << 8) | coefficientsArray[BME688_IDX_T2_LSB]);
    this->CalibData.par_t3 = (s8)coefficientsArray[BME688_IDX_T3];

    ////// Pressure related coefficients //////
    this->CalibData.par_p1 = ((u16)coefficientsArray[BME688_IDX_P1_MSB] << 8) | coefficientsArray[BME688_IDX_P1_LSB];
    this->CalibData.par_p2 = (s16)(((u16)coefficientsArray[BME688_IDX_P2_MSB] << 8) | coefficientsArray[BME688_IDX_P2_LSB]);
    this->CalibData.par_p3 = (s8)coefficientsArray[BME688_IDX_P3];
    this->CalibData.par_p4 = (s16)(((u16)coefficientsArray[BME688_IDX_P4_MSB] << 8) | coefficientsArray[BME688_IDX_P4_LSB]);
    this->CalibData.par_p5 = (s16)(((u16)coefficientsArray[BME688_IDX_P5_MSB] << 8) | coefficientsArray[BME688_IDX_P5_LSB]);
    this->CalibData.par_p6 = (s8)coefficientsArray[BME688_IDX_P6];
    this->CalibData.par_p7 = (s8)coefficientsArray[BME688_IDX_P7];
    this->CalibData.par_p8 = (s16)(((u16)coefficientsArray[BME688_IDX_P8_MSB] << 8) | coefficientsArray[BME688_IDX_P8_LSB]);
    this->CalibData.par_p9 = (s16)(((u16)coefficientsArray[BME688_IDX_P9_MSB] << 8) | coefficientsArray[BME688_IDX_P9_LSB]);
    this->CalibData.par_p10 = coefficientsArray[BME688_IDX_P10];

    ////// Humidity related coefficients //////
    this->CalibData.par_h1 = ((u16)coefficientsArray[BME688_IDX_H1_MSB] << 4) | (coefficientsArray[BME688_IDX_H1_LSB] & 0x0F);
    this->CalibData.par_h2 = ((u16)coefficientsArray[BME688_IDX_H2_MSB] << 4) | (coefficientsArray[BME688_IDX_H2_LSB] >> 4);
    this->CalibData.par_h3 = (s8)coefficientsArray[BME688_IDX_H3];
    this->CalibData.par_h4 = (s8)coefficientsArray[BME688_IDX_H4];
    this->CalibData.par_h5 = (s8)coefficientsArray[BME688_IDX_H5];
    this->CalibData.par_h6 = coefficientsArray[BME688_IDX_H6];
    this->CalibData.par_h7 = (s8)coefficientsArray[BME688_IDX_H7];

    ////// Gas heater related coefficients //////
    this->CalibData.par_gh1 = (s8)coefficientsArray[BME688_IDX_GH1];
    this->CalibData.par_gh2 = (s16)(((u16)coefficientsArray[BME688_IDX_GH2_MSB] << 8) | coefficientsArray[BME688_IDX_GH2_LSB]);
    this->CalibData.par_gh3 = (s8)coefficientsArray[BME688_IDX_GH3];

    ////// Other coefficients //////
    this->CalibData.res_heat_range = (coefficientsArray[BME688_IDX_RES_HEAT_RANGE] & 0x30) / 16;
    this->CalibData.res_heat_val = (s8)coefficientsArray[BME688_IDX_RES_HEAT_VAL];
    this->CalibData.range_sw_err = ((s8)(coefficientsArray[BME688_IDX_RES_HEAT_VAL] & 0xF0)) / 16;


    return(OsResult_Ok);
}
//=== end GetCalibrationData =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TBme688::WriteRegister(u8 regAddress, u8 regData)
{
	HAL_StatusTypeDef resultHal;
	u8 bufferTransmit[10];


	bufferTransmit[0] = regAddress;
	bufferTransmit[1] = regData;

	taskENTER_CRITICAL();

	resultHal = HAL_I2C_Master_Transmit(this->hI2c, (u16)(this->chipAddress << 1), bufferTransmit, 2, 100);  // timeout - 100 mSec
	if(resultHal != HAL_OK)
	{
		taskEXIT_CRITICAL();
		return(OsResult_ErrorI2cTransmit);
	}

	taskEXIT_CRITICAL();


	return(OsResult_Ok);
}
//=== end WriteRegister ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TBme688::ReadRegister(u8 regAddress, u8* regData)
{
	HAL_StatusTypeDef resultHal;
	u8 bufferTransmit[10];


	bufferTransmit[0] = regAddress;

	taskENTER_CRITICAL();

	resultHal = HAL_I2C_Master_Transmit(this->hI2c, (u16)(this->chipAddress << 1), bufferTransmit, 1, 100);  // timeout - 100 mSec
	if(resultHal != HAL_OK)
	{
		taskEXIT_CRITICAL();
		return(OsResult_ErrorI2cTransmit);
	}

	resultHal = HAL_I2C_Master_Receive(this->hI2c, (u16)(this->chipAddress << 1), bufferTransmit, 1, 100);  // timeout - 100 mSec
	if(resultHal != HAL_OK)
	{
		taskEXIT_CRITICAL();
		return(OsResult_ErrorI2cReceive);
	}

	taskEXIT_CRITICAL();

	*regData = bufferTransmit[0];


	return(OsResult_Ok);
}
//=== end ReadRegister =============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
/* EOsResult TBme688::TransmitChannel(u8 devAddress, u8* pData, u16 numBytes)
{
	HAL_StatusTypeDef resultHal;


	resultHal = HAL_I2C_Master_Transmit(this->hI2c, (u16)devAddress, pData, numBytes, 100);  // timeout - 100 mSec
	if(resultHal != HAL_OK)
	{
		return(OsResult_ErrorI2cTransmit);
	}


	return(OsResult_Ok);
} */
//=== end TransmitChannel1 =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
/* EOsResult TBme688::ReceiveChannel(u8 devAddress, u8* pData, u16 numBytes)
{
	HAL_StatusTypeDef resultHal;


	resultHal = HAL_I2C_Master_Receive(this->hI2c, (u16)devAddress, pData, numBytes, 100);  // timeout - 100 mSec
	if(resultHal != HAL_OK)
	{
		return(OsResult_ErrorI2cReceive);
	}


	return(OsResult_Ok);
} */
//=== end ReceiveChannel1 ==========================================================

/**********************************************************************************/
