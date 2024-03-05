/**********************************************************************************/
/**
 *  @file Bme688.hpp
 *  @brief ...
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __I2c_H
#define __I2c_H

/**********************************************************************************/
#include "Os/Os.hpp"


/**********************************************************************************/
#define I2C_ADDRESS_BME688    0x76  // (or 0x77), VOC sensor + Temperature and Humidity.

////// Register map addresses BME688 //////
#define BME688_REG_COEFF3         0x00  // Register for 3rd group of coefficients
#define BME688_REG_FIELD0         0x1D  // 0th Field address
#define BME688_REG_IDAC_HEAT0     0x50  // 0th Current DAC address
#define BME688_REG_RES_HEAT0      0x5A  // 0th Res heat address
#define BME688_REG_GAS_WAIT0      0x64  // 0th Gas wait address
#define BME688_REG_SHD_HEATR_DUR  0x6E  // Shared heating duration address
#define BME688_REG_CTRL_GAS_0     0x70  // CTRL_GAS_0 address
#define BME688_REG_CTRL_GAS_1     0x71  // CTRL_GAS_1 address
#define BME688_REG_CTRL_HUM       0x72  // CTRL_HUM address
#define BME688_REG_CTRL_MEAS      0x74  // CTRL_MEAS address
#define BME688_REG_CONFIG         0x75  // CONFIG address
#define BME688_REG_MEM_PAGE       0xF3  // MEM_PAGE address
#define BME688_REG_UNIQUE_ID      0x83  // Unique ID address
#define BME688_REG_COEFF1         0x8A  // Register for 1st group of coefficients
#define BME688_REG_CHIP_ID        0xD0  // Chip ID address
#define BME688_REG_RESET          0xE0  // Reset address
#define BME688_REG_COEFF2         0xE1  // Register for 2nd group of coefficients
#define BME688_REG_VARIANT_ID     0xF0  // Variant ID Register

#define BME688_CHIP_ID    0x61   // BME68X unique chip identifier
#define BME688_RESET_CMD  0xB6   // Reset command

////// Oversampling setting macros //////
#define BME688_OS_NONE  0  // Switch off measurement
#define BME688_OS_1X    1  // Perform 1 measurement
#define BME688_OS_2X    2  // Perform 2 measurements
#define BME688_OS_4X    3  // Perform 4 measurements
#define BME688_OS_8X    4  // Perform 8 measurements
#define BME688_OS_16X   5  // Perform 16 measurements

////// IIR Filter settings //////
#define BME688_FILTER_OFF       0  // Switch off the filter
#define BME688_FILTER_SIZE_1    1  // Filter coefficient of 2
#define BME688_FILTER_SIZE_3    2  // Filter coefficient of 4
#define BME688_FILTER_SIZE_7    3  // Filter coefficient of 8
#define BME688_FILTER_SIZE_15   4  // Filter coefficient of 16
#define BME688_FILTER_SIZE_31   5  // Filter coefficient of 32
#define BME688_FILTER_SIZE_63   6  // Filter coefficient of 64
#define BME688_FILTER_SIZE_127  7  // Filter coefficient of 128

////// ODR/Standby time macros //////
#define BME688_ODR_0_59_MS  0  // Standby time of 0.59ms
#define BME688_ODR_62_5_MS  1  // Standby time of 62.5ms
#define BME688_ODR_125_MS   2  // Standby time of 125ms
#define BME688_ODR_250_MS   3  // Standby time of 250ms
#define BME688_ODR_500_MS   4  // Standby time of 500ms
#define BME688_ODR_1000_MS  5  // Standby time of 1s
#define BME688_ODR_10_MS    6  // Standby time of 10ms
#define BME688_ODR_20_MS    7  // Standby time of 20ms
#define BME688_ODR_NONE     8  // No standby time

////// Coefficient index macros //////
#define BME688_LEN_COEFF_ALL        42  // Length for all coefficients
#define BME688_LEN_COEFF1           23  // Length for 1st group of coefficients
#define BME688_LEN_COEFF2           14  // Length for 2nd group of coefficients
#define BME688_LEN_COEFF3           5   // Length for 3rd group of coefficients
#define BME688_LEN_FIELD            17  // Length of the field
#define BME688_LEN_FIELD_OFFSET     17  // Length between two fields
#define BME688_LEN_CONFIG           5   // Length of the configuration register
#define BME688_LEN_INTERLEAVE_BUFF  20  // Length of the interleaved buffer

#define BME688_IDX_T2_LSB   0   // Coefficient T2 LSB position
#define BME688_IDX_T2_MSB   1   // Coefficient T2 MSB position
#define BME688_IDX_T3       2   // Coefficient T3 position
#define BME688_IDX_P1_LSB   4   // Coefficient P1 LSB position
#define BME688_IDX_P1_MSB   5   // Coefficient P1 MSB position
#define BME688_IDX_P2_LSB   6   // Coefficient P2 LSB position
#define BME688_IDX_P2_MSB   7   // Coefficient P2 MSB position
#define BME688_IDX_P3       8   // Coefficient P3 position
#define BME688_IDX_P4_LSB   10  // Coefficient P4 LSB position
#define BME688_IDX_P4_MSB   11  // Coefficient P4 MSB position
#define BME688_IDX_P5_LSB   12  // Coefficient P5 LSB position
#define BME688_IDX_P5_MSB   13  // Coefficient P5 MSB position
#define BME688_IDX_P7       14  // Coefficient P7 position
#define BME688_IDX_P6       15  // Coefficient P6 position
#define BME688_IDX_P8_LSB   18  // Coefficient P8 LSB position
#define BME688_IDX_P8_MSB   19  // Coefficient P8 MSB position
#define BME688_IDX_P9_LSB   20  // Coefficient P9 LSB position
#define BME688_IDX_P9_MSB   21  // Coefficient P9 MSB position
#define BME688_IDX_P10      22  // Coefficient P10 position
#define BME688_IDX_H2_MSB   23  // Coefficient H2 MSB position
#define BME688_IDX_H2_LSB   24  // Coefficient H2 LSB position
#define BME688_IDX_H1_LSB   24  // Coefficient H1 LSB position
#define BME688_IDX_H1_MSB   25  // Coefficient H1 MSB position
#define BME688_IDX_H3       26  // Coefficient H3 position
#define BME688_IDX_H4       27  // Coefficient H4 position
#define BME688_IDX_H5       28  // Coefficient H5 position
#define BME688_IDX_H6       29  // Coefficient H6 position
#define BME688_IDX_H7       30  // Coefficient H7 position
#define BME688_IDX_T1_LSB   31  // Coefficient T1 LSB position
#define BME688_IDX_T1_MSB   32  // Coefficient T1 MSB position
#define BME688_IDX_GH2_LSB  33  // Coefficient GH2 LSB position
#define BME688_IDX_GH2_MSB  34  // Coefficient GH2 MSB position
#define BME688_IDX_GH1      35  // Coefficient GH1 position
#define BME688_IDX_GH3      36  // Coefficient GH3 position
#define BME688_IDX_RES_HEAT_VAL    37  // Coefficient res heat value position
#define BME688_IDX_RES_HEAT_RANGE  39  // Coefficient res heat range position
#define BME688_IDX_RANGE_SW_ERR    41  // Coefficient range switching error position

#define BME688_NEW_DATA_MSK    0x80  // Mask for new data
#define BME688_GASM_VALID_MSK  0x20  // Mask for gas measurement valid
#define BME688_HEAT_STAB_MSK   0x10  // Mask for heater stability
#define BME688_GAS_INDEX_MSK   0x0F  // Mask for gas index
#define BME688_GAS_RANGE_MSK   0x0F  // Mask for gas range
/*
 * Macro definition for valid new data (0x80) AND
 * heater stability (0x10) AND gas resistance (0x20) values
 */
#define BME688_VALID_DATA  (BME688_NEW_DATA_MSK | BME688_GASM_VALID_MSK | BME688_HEAT_STAB_MSK)


/*
 * @brief Structure to hold the calibration coefficients
 */
struct CalibDataBme688
{
	////// Calibration coefficients for the humidity sensor //////
    u16 par_h1;
    u16 par_h2;
    s8 par_h3;
    s8 par_h4;
    s8 par_h5;
    u8 par_h6;
    s8 par_h7;

    ////// Calibration coefficients for the gas sensor //////
    s8 par_gh1;
    s16 par_gh2;
    s8 par_gh3;

    ////// Calibration coefficients for the temperature sensor //////
    u16 par_t1;
    s16 par_t2;
    s8 par_t3;

    ////// Calibration coefficient for the pressure sensor //////
    u16 par_p1;
    s16 par_p2;
    s8 par_p3;
    s16 par_p4;
    s16 par_p5;
    s8 par_p6;
    s8 par_p7;
    s16 par_p8;
    s16 par_p9;
    u8 par_p10;

    /*! Variable to store the intermediate temperature coefficient */
    s32 t_fine;

    /*! Heater resistance range coefficient */
    u8 res_heat_range;

    /*! Heater resistance value coefficient */
    s8 res_heat_val;

    /*! Gas resistance range switching error coefficient */
    s8 range_sw_err;
};

/*
 * @brief Sensor field data structure
 */
struct DataBme688
{
    u8 status;           // Contains new_data, gasm_valid & heat_stab
    u8 gas_index;        // The index of the heater profile used
    u8 meas_index;       // Measurement index to track order
    u8 res_heat;         // Heater resistance
    u8 idac;             // Current DAC
    u8 gas_wait;         // Gas wait period
    s16 temperature;     // Temperature in degree celsius x100
    u32 pressure;        // Pressure in Pascal
    u32 humidity;        // Humidity in % relative humidity x1000
    u32 gas_resistance;  // Gas resistance in Ohms

};



/**********************************************************************************/
////// TPH measurement duration //////
/*	u32 measCycles;
	u32 measDuration;
	u8 oversamplingMeasCycles[6] = { 0, 1, 2, 4, 8, 16 };  // oversampling
	measCycles = oversamplingMeasCycles[BME688_OS_2X];     // temperature oversampling - '2'
	meas_cycles += oversamplingMeasCycles[BME688_OS_16X];  // pressure oversampling - '16'
	meas_cycles += oversamplingMeasCycles[BME688_OS_1X];  // humidity oversampling - '1'
	// measCycles = 2 + 16 + 1;

	////// TPH measurement duration //////
	measDuration = (2 + 16 + 1) * 1963;
	measDuration += (477 * 4);          // TPH switching duration
	measDuration += (477 * 5);          // Gas measurement duration
	measDuration /= 1000;
	measDuration = 140 - measDuration;
	measDuration = 99; */

#define BME688_MEAS_DURATION           41000
#define BME688_SHARED_HEATER_DURATION  99
#define BME688_AMB_TEMPERATURE         25  // Typical room temperature in Celsius


enum EBme688Sensor
{
	Bme688Sensor_Left,
	Bme688Sensor_Right,
	Bme688Sensor_Fan
};

struct TBme688Sensors  // sizeof = 16 bytes
{
	s16 temperature;
	u16 humidity;
	u32 pressure;
	u32 gasResistance;
	u16 status;
};

/**********************************************************************************/
//==================================================================================
class TBme688 : public TOs
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	EOsResult Init(EBme688Sensor bme688Sensor);
	EOsResult StartMeasurement(void);  // in parallel mode
	u32 GetSamplingTime(void);
	void SetCounterSamplingTime(u32 time);
	u32 GetCounterSamplingTime(void);
	void DecreaseCounterSamplingTime(void);
	void WriteSensors(TBme688Sensors* Bme688Sensors);
	EOsResult GetData(DataBme688* dataBme688[]);
	TBme688Sensors* GetPointerBme688Sensors(void);
	u16 GetBme688SensorStatus(void);



protected:
	////// variables //////


	////// constants //////


	////// functions //////

private:
	////// variables //////
	CalibDataBme688 CalibData;
	I2C_HandleTypeDef* hI2c;
	u8 chipAddress;
	u32 counterSamplingTime;
	TBme688Sensors Bme688Sensors;


	////// constants //////
	static const u16 temperatureProf[];  // Heater temperature in degree Celsius
	static const u16 multiplierProf[];   // Multiplier to the shared heater duration



	////// functions //////
	s16 CalculateTemperature(u32 adc);
	u32 CalculatePressure(u32 adc);
	u32 CalculateHumidity(u16 adc);
	u32 CalculateGasResistanceHigh(u16 adc, u8 gasRange);
	u8 CalculateHeaterDurShared(u16 duration);
	u8 CalculateHeaterResistance(u16 temperature);
	EOsResult GetCalibrationData(void);
	EOsResult WriteRegister(u8 regAddress, u8 regData);
	EOsResult ReadRegister(u8 regAddress, u8* regData);



//	EOsResult TransmitChannel(u8 devAddress, u8* pData, u16 numBytes);
//	EOsResult ReceiveChannel(u8 devAddress, u8* pData, u16 numBytes);


};
//=== end class TBme688 ============================================================

/**********************************************************************************/
#endif
