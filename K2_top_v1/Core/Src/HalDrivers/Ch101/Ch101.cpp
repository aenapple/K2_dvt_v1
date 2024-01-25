/**********************************************************************************/
/**
 *  @file Ch101.cpp
 *  @brief Driver for CH101 sensors
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#include "Ch101.hpp"
#include "Ch101.hpp"


/**********************************************************************************/
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern TIM_HandleTypeDef htim3;


/**********************************************************************************/
extern "C" uint8_t	ch_init(ch_dev_t *dev_ptr, ch_group_t *grp_ptr, uint8_t dev_num, ch_fw_init_func_t fw_init_func);
extern "C" uint8_t ch101_gpr_narrow_init(ch_dev_t *dev_ptr, ch_group_t *grp_ptr, uint8_t i2c_addr, uint8_t dev_num, uint8_t i2c_bus_index);


/**********************************************************************************/
/*!
 * \brief Read bytes from an I2C slave using memory addressing.
 *
 * \param dev_ptr 		pointer to the ch_dev_t config structure for a sensor
 * \param mem_addr		internal memory or register address within device
 * \param data 			pointer to receive data buffer
 * \param num_bytes 	number of bytes to read
 *
 * \return 0 if successful, 1 on error or NACK
 *
 * This function reads the specified number of bytes from an I2C slave device, using
 * an internal memory or register address.  The remote device will return \a num_bytes bytes
 * starting at internal memory/register address \a mem_addr.
 *
 * The I2C interface must have already been initialized using \a chbsp_i2c_init().
 */
extern "C" int chbsp_i2c_mem_read(ch_dev_t *dev_ptr, uint16_t mem_addr, uint8_t *data, uint16_t num_bytes)
{
	static TCh101& Ch101 = TCh101::GetInstance();
	if(Ch101.ReadMemory(dev_ptr, mem_addr, data, num_bytes) == OsResult_Ok)
	{
		return(0);
	}


	return(1); //  error;
}

/*!
 * \brief Write bytes to an I2C slave using memory addressing.
 *
 * \param dev_ptr 		pointer to the ch_dev_t config structure for a sensor
 * \param mem_addr		internal memory or register address within device
 * \param data 			data to be transmitted
 * \param num_bytes 	length of data to be transmitted
 *
 * \return 0 if successful, 1 on error or NACK
 *
 * This function writes one or more bytes of data to an I2C slave device using an internal
 * memory or register address.  The remote device will write \a num_bytes bytes of
 * data starting at internal memory/register address \a mem_addr.
 * The I2C interface must have already been initialized using \a chbsp_i2c_init().
 */
extern "C" int chbsp_i2c_mem_write(ch_dev_t *dev_ptr, uint16_t mem_addr, uint8_t *data, uint16_t num_bytes)
{
	static TCh101& Ch101 = TCh101::GetInstance();
	if(Ch101.WriteMemory(dev_ptr, mem_addr, data, num_bytes) == OsResult_Ok)
	{
		return(0);
	}


	return(1); //  error;
}

/*!
 * \brief Return I2C information for a sensor port on the board.
 *
 * \param grp_ptr 		pointer to the ch_group_t config structure for a group of sensors
 * \param dev_num		device number within sensor group
 * \param info_ptr		pointer to structure to be filled with I2C config values
 *
 * \return 0 if successful, 1 if error
 *
 * This function returns I2C values in the ch_i2c_info_t structure specified by \a info_ptr.
 * The structure includes three fields.
 *  - The \a address field contains the I2C address for the sensor.
 *  - The \a bus_num field contains the I2C bus number (index).
 *  - The \a drv_flags field contains various bit flags through which the BSP can inform
 *  SonicLib driver functions to perform specific actions during I2C I/O operations.
 */
extern "C" uint8_t chbsp_i2c_get_info(ch_group_t *grp_ptr, uint8_t dev_num, ch_i2c_info_t *info_ptr)
{
	static TCh101& Ch101 = TCh101::GetInstance();
	if(Ch101.GetInfo(dev_num, info_ptr) == OsResult_Ok)
	{
		return(0);
	}

	return(1);
}

/*!
 * \brief Write bytes to an I2C slave.
 *
 * \param dev_ptr 		pointer to the ch_dev_t config structure for a sensor
 * \param data 			data to be transmitted
 * \param num_bytes 	length of data to be transmitted
 *
 * \return 0 if successful, 1 on error or NACK
 *
 * This function writes one or more bytes of data to an I2C slave device.
 * The I2C interface must have already been initialized using \a chbsp_i2c_init().
 */
extern "C" int chbsp_i2c_write(ch_dev_t *dev_ptr, uint8_t *data, uint16_t num_bytes)
{
	static TCh101& Ch101 = TCh101::GetInstance();
	if(Ch101.Write(dev_ptr, data, num_bytes) == OsResult_Ok)
	{
		return(0);
	}


	return(1); //  error;
}

/*!
 * \brief Read bytes from an I2C slave.
 *
 * \param dev_ptr 		pointer to the ch_dev_t config structure for a sensor
 * \param data 			pointer to receive data buffer
 * \param num_bytes 	number of bytes to read
 *
 * \return 0 if successful, 1 on error or NACK
 *
 * This function reads the specified number of bytes from an I2C slave device.
 * The I2C interface must have already been initialized using \a chbsp_i2c_init().
 */
extern "C" int chbsp_i2c_read(ch_dev_t *dev_ptr, uint8_t *data, uint16_t num_bytes)
{



	return(1); //  error;
}


/*!
 * \brief Assert the reset pin
 *
 * This function drives the sensor reset pin low.
 */
extern "C" void chbsp_reset_assert(void)
{
	HAL_GPIO_WritePin(RESET1_GPIO_Port, RESET1_Pin, GPIO_PIN_RESET);  // reset = '0'
	HAL_GPIO_WritePin(RESET2_GPIO_Port, RESET2_Pin, GPIO_PIN_RESET);  // reset = '0'
	HAL_GPIO_WritePin(RESET3_GPIO_Port, RESET3_Pin, GPIO_PIN_RESET);  // reset = '0'
}

/*!
 * \brief Deassert the reset pin
 *
 * This function drives the sensor reset pin high.
 */
extern "C" void chbsp_reset_release(void)
{
	HAL_GPIO_WritePin(RESET1_GPIO_Port, RESET1_Pin, GPIO_PIN_SET);  // reset = '1'
	HAL_GPIO_WritePin(RESET2_GPIO_Port, RESET2_Pin, GPIO_PIN_SET);  // reset = '1'
	HAL_GPIO_WritePin(RESET3_GPIO_Port, RESET3_Pin, GPIO_PIN_SET);  // reset = '1'
}

/*!
 * \brief Assert the PROG pin
 *
 * \param dev_ptr 		pointer to the ch_dev_t config structure for a sensor
 *
 * This function drives the sensor PROG pin high on the specified port.
 */
extern "C" void chbsp_program_enable(ch_dev_t *dev_ptr)
{
	static TCh101& Ch101 = TCh101::GetInstance();
	Ch101.SetPinProgram(dev_ptr->io_index);
}

/*!
 * \brief Deassert the PROG pin
 *
 * \param dev_ptr 		pointer to the ch_dev_t config structure for a sensor
 *
 * This function drives the sensor PROG pin low on the specified port.
 */
extern "C" void chbsp_program_disable(ch_dev_t *dev_ptr)
{
	static TCh101& Ch101 = TCh101::GetInstance();
	Ch101.ResetPinProgram(dev_ptr->io_index);
}

/*!
 * \brief Configure the Chirp sensor INT pin as an output for one sensor.
 *
 * \param dev_ptr 		pointer to the ch_dev_t config structure for a sensor
 *
 * This function configures the Chirp sensor INT pin as an output (from the perspective
 * of the host system).
 */
void chbsp_set_io_dir_out(ch_dev_t *dev_ptr)
{
//	static TCh101& Ch101 = TCh101::GetInstance();
//	Ch101.SetPinIntOutput(dev_ptr->io_index);
}


/*!
 * \brief Configure the Chirp sensor INT pin as an input for one sensor.
 *
 * \param dev_ptr 		pointer to the ch_dev_t config structure for a sensor
 *
 * This function configures the Chirp sensor INT pin as an input (from the perspective of
 * the host system).
 */
void chbsp_set_io_dir_in(ch_dev_t *dev_ptr)
{
	static TCh101& Ch101 = TCh101::GetInstance();
	Ch101.SetPinIntInputEXTI(dev_ptr->io_index);
}

/*!
 * \brief Configure the Chirp sensor INT pins as outputs for a group of sensors
 *
 * \param grp_ptr 		pointer to the ch_group_t config structure for a group of sensors
 *
 * This function configures each Chirp sensor's INT pin as an output (from the perspective
 * of the host system).
 */
void chbsp_group_set_io_dir_out(ch_group_t *grp_ptr)
{
//	static TCh101& Ch101 = TCh101::GetInstance();
//	Ch101.SetGroupPinIntOutput();
}

/*!
 * \brief Configure the Chirp sensor INT pins as inputs for a group of sensors
 *
 * \param grp_ptr 		pointer to the ch_group_t config structure for a group of sensors
 *
 * \note This function assumes a bidirectional level shifter is interfacing the ICs.
 */
void chbsp_group_set_io_dir_in(ch_group_t *grp_ptr)
{
	static TCh101& Ch101 = TCh101::GetInstance();
	Ch101.SetGroupPinIntInputEXTI();
}

/*!
 * \brief Initialize the I/O pins.
 *
 * \param grp_ptr 		pointer to the ch_group_t config structure for a group of sensors
 *
 * Configure reset and program pins as outputs. Assert reset and program. Configure
 * sensor INT pin as input.
 */
void chbsp_group_pin_init(ch_group_t *grp_ptr)
{
	static TCh101& Ch101 = TCh101::GetInstance();
	Ch101.InitAllPins();
}

/*!
 * \brief Set the INT pins low for a one sensor.
 *
 * \param dev_ptr 		pointer to the ch_dev_t config structure for a sensor
 *
 * This function drives the INT line low for one sensor.
 */
void chbsp_io_clear(ch_dev_t *dev_ptr)
{
//	static TCh101& Ch101 = TCh101::GetInstance();
//	Ch101.ClearPinInt(dev_ptr->io_index);
}

/*!
 * \brief Set the INT pins high for a one sensor.
 *
 * \param dev_ptr 		pointer to the ch_dev_t config structure for a sensor
 *
 * This function drives the INT line high for one sensor.
 */
void chbsp_io_set(ch_dev_t *dev_ptr)
{
//	static TCh101& Ch101 = TCh101::GetInstance();
//	Ch101.SetPinInt(dev_ptr->io_index);
}

/*!
 * \brief Set the INT pins low for a group of sensors.
 *
 * \param grp_ptr 		pointer to the ch_group_t config structure for a group of sensors
 *
 * This function drives the INT line low for each sensor in the group.
 */
void chbsp_group_io_clear(ch_group_t *grp_ptr)
{
//	static TCh101& Ch101 = TCh101::GetInstance();
//	Ch101.ClearGroupPinInt();
}

 /*!
 * \brief Set the INT pins high for a group of sensors.
 *
 * \param grp_ptr 		pointer to the ch_group_t config structure for a group of sensors
 *
 * This function drives the INT line high for each sensor in the group.
 */
void chbsp_group_io_set(ch_group_t *grp_ptr)
{
//	static TCh101& Ch101 = TCh101::GetInstance();
//	Ch101.SetGroupPinInt();
}

/*!
 * \brief Disable interrupts for a group of sensors
 *
 * \param grp_ptr 	pointer to the ch_group_t config structure for a group of sensors
 *
 * For each sensor in the group, this function disables the host interrupt associated
 * with the Chirp sensor device's INT line.
 */
void chbsp_group_io_interrupt_enable(ch_group_t *grp_ptr)
{
	static TCh101& Ch101 = TCh101::GetInstance();
	Ch101.InterruptGroupEnable();
}

/*!
 * \brief Enable the interrupt for one sensor
 *
 * \param dev_ptr	pointer to the ch_dev_t config structure for a sensor
 *
 * This function enables the host interrupt associated with the Chirp sensor device's
 * INT line.
 */
void chbsp_io_interrupt_enable(ch_dev_t *dev_ptr)
{
	static TCh101& Ch101 = TCh101::GetInstance();
	Ch101.InterruptEnable(dev_ptr->io_index);
}

/*!
 * \brief Disable interrupts for a group of sensors
 *
 * \param grp_ptr 		pointer to the ch_group_t config structure for a group of sensors
 *
 * For each sensor in the group, this function disables the host interrupt associated
 * with the Chirp sensor device's INT line.
 */
void chbsp_group_io_interrupt_disable(ch_group_t *grp_ptr)
{
//	static TCh101& Ch101 = TCh101::GetInstance();
//	Ch101.InterruptGroupDisable();
}

/*!
 * \brief Disable the interrupt for one sensor
 *
 * \param dev_ptr 		pointer to the ch_dev_t config structure for a sensor
 *
 * This function disables the host interrupt associated with the Chirp sensor device's
 * INT line.
 */
void chbsp_io_interrupt_disable(ch_dev_t *dev_ptr)
{
//	static TCh101& Ch101 = TCh101::GetInstance();
//	Ch101.InterruptDisable(dev_ptr->io_index);
}

/*!
 * \brief Delay for specified number of microseconds
 *
 * \param us  	number of microseconds to delay before returning
 *
 * This function waits for the specified number of microseconds before returning to
 * the caller.
 */
void chbsp_delay_us(uint32_t us)
{
	uint32_t counter;

	counter = us * 64 - 1;  // clk = 64MHz
	htim3.Instance->CNT = 0;
	while(htim3.Instance->CNT < counter);
}

/*!
 * \brief Delay for specified number of milliseconds.
 *
 * \param ms 	number of milliseconds to delay before returning
 *
 * This function waits for the specified number of milliseconds before returning to
 * the caller.
 */
void chbsp_delay_ms(uint32_t ms)
{
	chbsp_delay_us(ms * 1000);
}






/**********************************************************************************/
const u8 TCh101::i2cAddresses[] = { 45, 43, 44 };
const u8 TCh101::i2c_buses[] = { 0, 0, 1 };


/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
TCh101::TCh101()
{
	HAL_TIM_Base_Start(&htim3);

	HAL_GPIO_WritePin(RESET1_GPIO_Port, RESET1_Pin, GPIO_PIN_SET);  // reset = '1'
	HAL_GPIO_WritePin(RESET2_GPIO_Port, RESET2_Pin, GPIO_PIN_SET);  // reset = '1'
	HAL_GPIO_WritePin(RESET3_GPIO_Port, RESET3_Pin, GPIO_PIN_SET);  // reset = '1'

	HAL_GPIO_WritePin(PROG1_GPIO_Port, PROG1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PROG2_GPIO_Port, PROG2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PROG3_GPIO_Port, PROG3_Pin, GPIO_PIN_RESET);
}
//=== end TCh101 ===================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
TCh101& TCh101::GetInstance()
{
	static TCh101 Ch101;
	return(Ch101);
}
//=== end GetInstance ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TCh101::Init()
{
	EOsResult result;
	ch_dev_t* dev_ptr;


	this->leftInterruptEnable = true;
	this->rightInterruptEnable = true;
	this->tankInterruptEnable = true;

	this->chirp_group.num_ports = CHIRP_MAX_NUM_SENSORS;
	this->chirp_group.num_i2c_buses = CHIRP_NUM_I2C_BUSES;
	this->chirp_group.rtc_cal_pulse_ms = CH101_RTC_CAL_PULSE_MS;

	for(u16 i = 0; i < chirp_group.num_ports; i++)
	{
		this->Delay(10);
		result = this->FindSensor((ECh101Sensor)i);

		if(result != OsResult_Ok)
		{
			this->Delay(10);
			result = this->FindSensor((ECh101Sensor)i);
			if(result != OsResult_Ok)
			{
				return(result);
			}
		}
	}

	for(u8 dev_num = 0; dev_num < chirp_group.num_ports; dev_num++)
	{
		dev_ptr = &this->chirp_devices[dev_num];	// init struct in array
		if(ch_init(dev_ptr, &this->chirp_group, dev_num, CHIRP_SENSOR_FW_INIT_FUNC) != 0)
		{
			return(OsResult_Error);
		}
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
EOsResult TCh101::StartMeasurement(ECh101Sensor ch101Sensor)
{
//	EOsResult result;
	ch_dev_t* dev_ptr;
	ch_config_t dev_config;


	dev_config.mode = CH_MODE_FREERUN;
	dev_config.sample_interval = MEASUREMENT_INTERVAL_MS;  // 1 Sec
	dev_config.thresh_ptr = 0;
	dev_config.max_range       = CHIRP_SENSOR_MAX_RANGE_MM;
	dev_config.static_range    = CHIRP_SENSOR_STATIC_RANGE;


	for(u8 dev_num = 0; dev_num < chirp_group.num_ports; dev_num++)
	{
		dev_ptr = &this->chirp_devices[dev_num];	// init struct in array
		if(ch_set_config(dev_ptr, &dev_config) != 0)
		{
			return(OsResult_Error);
		}
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
EOsResult TCh101::FindSensor(ECh101Sensor ch101Sensor)
{
	u8 sigBytes[2];
	HAL_StatusTypeDef halResult;


	sigBytes[0] = 0;
	sigBytes[1] = 0;

	if(ch101Sensor == Ch101Sensor_Tank)
	{
		HAL_GPIO_WritePin(PROG3_GPIO_Port, PROG3_Pin, GPIO_PIN_SET);
		halResult = HAL_I2C_Mem_Read(&hi2c2, (u16)(CH_I2C_ADDR_PROG << 1), 0x00, I2C_MEMADD_SIZE_8BIT, sigBytes, 2, 100);
	}
	else
	{
		if(ch101Sensor == Ch101Sensor_Left)
		{
			HAL_GPIO_WritePin(PROG1_GPIO_Port, PROG1_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(PROG2_GPIO_Port, PROG2_Pin, GPIO_PIN_SET);
		}

		halResult = HAL_I2C_Mem_Read(&hi2c1, (u16)(CH_I2C_ADDR_PROG << 1), 0x00, I2C_MEMADD_SIZE_8BIT, sigBytes, 2, 100);
	}

	if(ch101Sensor == Ch101Sensor_Tank)
	{
		HAL_GPIO_WritePin(PROG3_GPIO_Port, PROG3_Pin, GPIO_PIN_RESET);
		if(halResult == HAL_OK)
		{
			this->presentTankSensor = true;
		}
		else
		{
			this->presentTankSensor = false;
			return(OsResult_ErrorI2cReceive);
		}

	}
	else
	{
		if(ch101Sensor == Ch101Sensor_Left)
		{
			HAL_GPIO_WritePin(PROG1_GPIO_Port, PROG1_Pin, GPIO_PIN_RESET);
			if(halResult == HAL_OK)
			{
				this->presentLeftSensor = true;
			}
			else
			{
				this->presentLeftSensor = false;
				return(OsResult_ErrorI2cReceive);
			}

		}
		else
		{
			HAL_GPIO_WritePin(PROG2_GPIO_Port, PROG2_Pin, GPIO_PIN_RESET);
			if(halResult == HAL_OK)
			{
				this->presentRightSensor = true;
			}
			else
			{
				this->presentRightSensor = false;
				return(OsResult_ErrorI2cReceive);
			}
		}
	}


	return(OsResult_Ok);
}
//=== end FindSensor ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TCh101::Read(ch_dev_t *dev_ptr, uint8_t *data, uint16_t num_bytes)
{
	HAL_StatusTypeDef halResult;


	if(dev_ptr->i2c_bus_index == 0)
	{
		// I2C bus 0 (hi2c1)
		halResult = HAL_I2C_Master_Receive(&hi2c1, (u16)(dev_ptr->i2c_address << 1), data, num_bytes, 100);
		if(halResult == HAL_OK)
		{
			return(OsResult_Ok);
		}
	}
	else
	{
		if(dev_ptr->i2c_bus_index == 1)
		{
			// I2C bus 1 (hi2c2)
			halResult = HAL_I2C_Master_Receive(&hi2c2, (u16)(dev_ptr->i2c_address << 1), data, num_bytes, 100);
			if(halResult == HAL_OK)
			{
				return(OsResult_Ok);
			}
		}
	}


	return(OsResult_ErrorI2cReceive);
}
//=== end Read =====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TCh101::ReadMemory(ch_dev_t *dev_ptr, u16 mem_addr, u8 *data, u16 num_bytes)
{
	HAL_StatusTypeDef halResult;


	if(dev_ptr->i2c_bus_index == 0)
	{
		// I2C bus 0 (hi2c1)
		halResult = HAL_I2C_Mem_Read(&hi2c1, (u16)(dev_ptr->i2c_address << 1), mem_addr, I2C_MEMADD_SIZE_8BIT, data, num_bytes, 100);
		if(halResult == HAL_OK)
		{
			return(OsResult_Ok);
		}
	}
	else
	{
		if(dev_ptr->i2c_bus_index == 1)
		{
			// I2C bus 1 (hi2c2)
			halResult = HAL_I2C_Mem_Read(&hi2c2, (u16)(dev_ptr->i2c_address << 1), mem_addr, I2C_MEMADD_SIZE_8BIT, data, num_bytes, 100);
			if(halResult == HAL_OK)
			{
				return(OsResult_Ok);
			}
		}
	}


	return(OsResult_ErrorI2cReceive);
}
//=== end ReadMemory ===============================================================

//int chbsp_i2c_write(s)

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TCh101::Write(ch_dev_t *dev_ptr, uint8_t *data, uint16_t num_bytes)
{
	HAL_StatusTypeDef halResult;


	if(dev_ptr->i2c_bus_index == 0)
	{
		// I2C bus 0 (hi2c1)
		halResult = HAL_I2C_Master_Transmit(&hi2c1, (u16)(dev_ptr->i2c_address << 1), data, num_bytes, 100);
		if(halResult == HAL_OK)
		{
			return(OsResult_Ok);
		}
	}
	else
	{
		if(dev_ptr->i2c_bus_index == 1)
		{
			// I2C bus 1 (hi2c2)
			halResult = HAL_I2C_Master_Transmit(&hi2c2, (u16)(dev_ptr->i2c_address << 1), data, num_bytes, 100);
			if(halResult == HAL_OK)
			{
				return(OsResult_Ok);
			}
		}
	}


	return(OsResult_ErrorI2cTransmit);
}
//=== end Write ====================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TCh101::WriteMemory(ch_dev_t *dev_ptr, u16 mem_addr, u8 *data, u16 num_bytes)
{
	HAL_StatusTypeDef halResult;


	if(dev_ptr->i2c_bus_index == 0)
	{
		// I2C bus 0 (hi2c1)
		halResult = HAL_I2C_Mem_Write(&hi2c1, (u16)(dev_ptr->i2c_address << 1), mem_addr, I2C_MEMADD_SIZE_8BIT, data, num_bytes, 100);
		if(halResult == HAL_OK)
		{
			return(OsResult_Ok);
		}
	}
	else
	{
		if(dev_ptr->i2c_bus_index == 1)
		{
			// I2C bus 1 (hi2c2)
			halResult = HAL_I2C_Mem_Write(&hi2c2, (u16)(dev_ptr->i2c_address << 1), mem_addr, I2C_MEMADD_SIZE_8BIT, data, num_bytes, 100);
			if(halResult == HAL_OK)
			{
				return(OsResult_Ok);
			}
		}
	}


	return(OsResult_ErrorI2cTransmit);
}
//=== end WriteMemory ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TCh101::GetInfo(u8 dev_num, ch_i2c_info_t *info_ptr)
{
	if(dev_num <= CHIRP_MAX_NUM_SENSORS)
	{
		info_ptr->address = this->i2cAddresses[dev_num];
		info_ptr->bus_num = this->i2c_buses[dev_num];

		info_ptr->drv_flags = 0;	// no special I2C handling by SonicLib driver is needed

		return(OsResult_Ok);
	}


	return(OsResult_Error);
}
//=== end GetInfo ==================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::SetPinProgram(u8 io_index)
{
	if(io_index == Ch101Sensor_Left)
	{
		HAL_GPIO_WritePin(PROG1_GPIO_Port, PROG1_Pin, GPIO_PIN_SET);
	}
	else
	{
		if(io_index == Ch101Sensor_Right)
		{
			HAL_GPIO_WritePin(PROG2_GPIO_Port, PROG2_Pin, GPIO_PIN_SET);
		}
		else // Ch101Sensor_Tank
		{
			HAL_GPIO_WritePin(PROG3_GPIO_Port, PROG3_Pin, GPIO_PIN_SET);
		}
	}

}
//=== end SetPinProgram ============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::ResetPinProgram(u8 io_index)
{
	if(io_index == Ch101Sensor_Left)
	{
		HAL_GPIO_WritePin(PROG1_GPIO_Port, PROG1_Pin, GPIO_PIN_RESET);
	}
	else
	{
		if(io_index == Ch101Sensor_Right)
		{
			HAL_GPIO_WritePin(PROG2_GPIO_Port, PROG2_Pin, GPIO_PIN_RESET);
		}
		else // Ch101Sensor_Tank
		{
			HAL_GPIO_WritePin(PROG3_GPIO_Port, PROG3_Pin, GPIO_PIN_RESET);
		}
	}

}
//=== end ResetPinProgram ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::SetPinIntOutput(u8 io_index)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};


	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	switch(io_index)
	{
		case Ch101Sensor_Left:
			HAL_GPIO_DeInit(INT1_GPIO_Port, INT1_Pin);
			GPIO_InitStruct.Pin = INT1_Pin;
			HAL_GPIO_Init(INT1_GPIO_Port, &GPIO_InitStruct);
			break;

		case Ch101Sensor_Right:
			HAL_GPIO_DeInit(INT2_GPIO_Port, INT2_Pin);
			GPIO_InitStruct.Pin = INT2_Pin;
			HAL_GPIO_Init(INT2_GPIO_Port, &GPIO_InitStruct);
			break;

		default: // Ch101Sensor_Tank
			HAL_GPIO_DeInit(INT3_GPIO_Port, INT3_Pin);
			GPIO_InitStruct.Pin = INT3_Pin;
			HAL_GPIO_Init(INT3_GPIO_Port, &GPIO_InitStruct);
	}

}
//=== end SetPinIntOutput ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::SetPinIntInputEXTI(u8 io_index)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};


	GPIO_InitStruct.Pin = IN_FRONT_LAMP_Pin|INT1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	switch(io_index)
	{
		case Ch101Sensor_Left:
			HAL_GPIO_DeInit(INT1_GPIO_Port, INT1_Pin);
			GPIO_InitStruct.Pin = INT1_Pin;
			HAL_GPIO_Init(INT1_GPIO_Port, &GPIO_InitStruct);
			break;

		case Ch101Sensor_Right:
			HAL_GPIO_DeInit(INT2_GPIO_Port, INT2_Pin);
			GPIO_InitStruct.Pin = INT2_Pin;
			HAL_GPIO_Init(INT2_GPIO_Port, &GPIO_InitStruct);
			break;

		default: // Ch101Sensor_Tank
			HAL_GPIO_DeInit(INT3_GPIO_Port, INT3_Pin);
			GPIO_InitStruct.Pin = INT3_Pin;
			HAL_GPIO_Init(INT3_GPIO_Port, &GPIO_InitStruct);
	}

}
//=== end SetPinIntInputEXTI =======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::SetGroupPinIntOutput()
{
	this->SetPinIntOutput(Ch101Sensor_Left);
	this->SetPinIntOutput(Ch101Sensor_Right);
	this->SetPinIntOutput(Ch101Sensor_Tank);
}
//=== end SetGroupPinIntOutput =====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::SetGroupPinIntInputEXTI()
{
	this->SetPinIntInputEXTI(Ch101Sensor_Left);
	this->SetPinIntInputEXTI(Ch101Sensor_Right);
	this->SetPinIntInputEXTI(Ch101Sensor_Tank);
}
//=== end SetGroupPinIntInputEXTI ==================================================



//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::ClearPinInt(u8 io_index)
{
	if(io_index == Ch101Sensor_Left)
	{
		HAL_GPIO_WritePin(INT1_GPIO_Port, INT1_Pin, GPIO_PIN_RESET);
	}
	else
	{
		if(io_index == Ch101Sensor_Right)
		{
			HAL_GPIO_WritePin(INT2_GPIO_Port, INT2_Pin, GPIO_PIN_RESET);
		}
		else // Ch101Sensor_Tank
		{
			HAL_GPIO_WritePin(INT3_GPIO_Port, INT3_Pin, GPIO_PIN_RESET);
		}
	}

}
//=== end ClearPinInt ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::SetPinInt(u8 io_index)
{
	if(io_index == Ch101Sensor_Left)
	{
		HAL_GPIO_WritePin(INT1_GPIO_Port, INT1_Pin, GPIO_PIN_SET);
	}
	else
	{
		if(io_index == Ch101Sensor_Right)
		{
			HAL_GPIO_WritePin(INT2_GPIO_Port, INT2_Pin, GPIO_PIN_SET);
		}
		else // Ch101Sensor_Tank
		{
			HAL_GPIO_WritePin(INT3_GPIO_Port, INT3_Pin, GPIO_PIN_SET);
		}
	}

}
//=== end SetPinInt ================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::ClearGroupPinInt()
{
	this->ClearPinInt(Ch101Sensor_Left);
	this->ClearPinInt(Ch101Sensor_Right);
	this->ClearPinInt(Ch101Sensor_Tank);
}
//=== end ClearGroupPinInt =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::SetGroupPinInt()
{
	this->SetPinInt(Ch101Sensor_Left);
	this->SetPinInt(Ch101Sensor_Right);
	this->SetPinInt(Ch101Sensor_Tank);
}
//=== end SetGroupPinInt ===========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::InitAllPins()
{
	HAL_GPIO_WritePin(PROG1_GPIO_Port, PROG1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PROG2_GPIO_Port, PROG2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PROG3_GPIO_Port, PROG3_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(RESET1_GPIO_Port, RESET1_Pin, GPIO_PIN_RESET);  // reset = '0'
	HAL_GPIO_WritePin(RESET2_GPIO_Port, RESET2_Pin, GPIO_PIN_RESET);  // reset = '0'
	HAL_GPIO_WritePin(RESET3_GPIO_Port, RESET3_Pin, GPIO_PIN_RESET);  // reset = '0'

	this->Delay(2);

	HAL_GPIO_WritePin(PROG1_GPIO_Port, PROG1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PROG2_GPIO_Port, PROG2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PROG3_GPIO_Port, PROG3_Pin, GPIO_PIN_SET);

	this->SetGroupPinIntInputEXTI();

	this->leftInterruptEnable = true;
	this->rightInterruptEnable = true;
	this->tankInterruptEnable = true;
}
//=== end InitAllPins ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::InterruptEnable(u8 io_index)
{
	if(io_index == Ch101Sensor_Left)
	{
		this->leftInterruptEnable = true;
	}
	else
	{
		if(io_index == Ch101Sensor_Right)
		{
			this->rightInterruptEnable = true;
		}
		else // Ch101Sensor_Tank
		{
			this->tankInterruptEnable = true;
		}
	}

}
//=== end InterruptEnable ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::InterruptGroupEnable()
{
	this->InterruptEnable(Ch101Sensor_Left);
	this->InterruptEnable(Ch101Sensor_Right);
	this->InterruptEnable(Ch101Sensor_Tank);
}
//=== end InterruptGroupEnable =====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::InterruptDisable(u8 io_index)
{

	if(io_index == Ch101Sensor_Left)
	{
		this->leftInterruptEnable = false;
	}
	else
	{
		if(io_index == Ch101Sensor_Right)
		{
			this->rightInterruptEnable = false;
		}
		else // Ch101Sensor_Tank
		{
			this->tankInterruptEnable = false;
		}
	}

}
//=== end InterruptDisable =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TCh101::InterruptGroupDisable()
{
	this->InterruptDisable(Ch101Sensor_Left);
	this->InterruptDisable(Ch101Sensor_Right);
	this->InterruptDisable(Ch101Sensor_Tank);
}
//=== end InterruptGroupDisable ====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
bool TCh101::GetInterruptEnable(ECh101Sensor ch101Sensor)
{
	if(ch101Sensor == Ch101Sensor_Left)
	{
		return(this->leftInterruptEnable);
	}
	else
	{
		if(ch101Sensor == Ch101Sensor_Right)
		{
			return(this->rightInterruptEnable);
		}
		else // Ch101Sensor_Tank
		{
			return(this->tankInterruptEnable);
		}
	}

}
//=== end GetInterruptEnable =======================================================

/**********************************************************************************/
