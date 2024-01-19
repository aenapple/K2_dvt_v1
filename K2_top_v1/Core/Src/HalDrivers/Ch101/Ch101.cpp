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
	HAL_StatusTypeDef halResult;


	if(dev_ptr->i2c_bus_index == 0)
	{
		// I2C bus 0 (hi2c1)
		halResult = HAL_I2C_Mem_Read(&hi2c1, (u16)(dev_ptr->i2c_address << 1), mem_addr, I2C_MEMADD_SIZE_8BIT, data, num_bytes, 100);
		if(halResult == HAL_OK)
		{
			return(0);  // Ok
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
				return(0);  // Ok
			}
		}
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
	HAL_StatusTypeDef halResult;


	if(dev_ptr->i2c_bus_index == 0)
	{
		// I2C bus 0 (hi2c1)
		halResult = HAL_I2C_Mem_Write(&hi2c1, (u16)(dev_ptr->i2c_address << 1), mem_addr, I2C_MEMADD_SIZE_8BIT, data, num_bytes, 100);
		if(halResult == HAL_OK)
		{
			return(0);  // Ok
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
				return(0);  // Ok
			}
		}
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
	if (dev_num <= CHIRP_MAX_NUM_SENSORS) {
		info_ptr->address = TCh101::i2cAddresses[dev_num];
		info_ptr->bus_num = TCh101::i2c_buses[dev_num];

		info_ptr->drv_flags = 0;	// no special I2C handling by SonicLib driver is needed

		return(0);
	}

	return(1);
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
	switch(dev_ptr->io_index)
	{
		case Ch101Sensor_Left:
			HAL_GPIO_WritePin(PROG1_GPIO_Port, PROG1_Pin, GPIO_PIN_SET);
			break;

		case Ch101Sensor_Right:
			HAL_GPIO_WritePin(PROG2_GPIO_Port, PROG2_Pin, GPIO_PIN_SET);
			break;

		default: // Ch101Sensor_Tank
			HAL_GPIO_WritePin(PROG3_GPIO_Port, PROG3_Pin, GPIO_PIN_SET);
	}

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
	switch(dev_ptr->io_index)
	{
		case Ch101Sensor_Left:
			HAL_GPIO_WritePin(PROG1_GPIO_Port, PROG1_Pin, GPIO_PIN_RESET);
			break;

		case Ch101Sensor_Right:
			HAL_GPIO_WritePin(PROG2_GPIO_Port, PROG2_Pin, GPIO_PIN_RESET);
			break;

		default: // Ch101Sensor_Tank
			HAL_GPIO_WritePin(PROG3_GPIO_Port, PROG3_Pin, GPIO_PIN_RESET);
	}

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
	HAL_StatusTypeDef halResult;


	if(dev_ptr->i2c_bus_index == 0)
	{
		// I2C bus 0 (hi2c1)
		halResult = HAL_I2C_Master_Transmit(&hi2c1, (u16)(dev_ptr->i2c_address << 1), data, num_bytes, 100);
		if(halResult == HAL_OK)
		{
			return(0);  // Ok
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
				return(0);  // Ok
			}
		}
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
	HAL_StatusTypeDef halResult;


	if(dev_ptr->i2c_bus_index == 0)
	{
		// I2C bus 0 (hi2c1)
		halResult = HAL_I2C_Master_Receive(&hi2c1, (u16)(dev_ptr->i2c_address << 1), data, num_bytes, 100);
		if(halResult == HAL_OK)
		{
			return(0);  // Ok
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
				return(0);  // Ok
			}
		}
	}


	return(1); //  error;
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
void TCh101::Init()
{
	EOsResult result;
	ch_dev_t* dev_ptr;


	chirp_group.num_ports = CHIRP_MAX_NUM_SENSORS;
	chirp_group.num_i2c_buses = CHIRP_NUM_I2C_BUSES;
	chirp_group.rtc_cal_pulse_ms = CH101_RTC_CAL_PULSE_MS;

	HAL_GPIO_WritePin(RESET1_GPIO_Port, RESET1_Pin, GPIO_PIN_SET);  // reset = '1'
	HAL_GPIO_WritePin(RESET2_GPIO_Port, RESET2_Pin, GPIO_PIN_SET);  // reset = '1'
	HAL_GPIO_WritePin(RESET3_GPIO_Port, RESET3_Pin, GPIO_PIN_SET);  // reset = '1'

	HAL_GPIO_WritePin(PROG1_GPIO_Port, PROG1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PROG2_GPIO_Port, PROG2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PROG3_GPIO_Port, PROG3_Pin, GPIO_PIN_RESET);

	for(u16 i = 0; i < chirp_group.num_ports; i++)
	{
		this->Delay(10);
		result = this->FindSensor((ECh101Sensor)i);

		this->Delay(10);
		if(result != OsResult_Ok)
		{
			this->FindSensor((ECh101Sensor)i);
		}
	}

	for(u8 dev_num = 0; dev_num < chirp_group.num_ports; dev_num++)
	{
		dev_ptr = &this->chirp_devices[dev_num];	// init struct in array
		ch_init(dev_ptr, &this->chirp_group, dev_num, CHIRP_SENSOR_FW_INIT_FUNC);
	}




}
//=== end Init =====================================================================

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
EOsResult ReadRegisterI2c1(u8 devAddress, u8 regAddress, u16 regLength, u8* regValue)
{
	u16 tempDevAddress;
	HAL_StatusTypeDef halResult;


	tempDevAddress = devAddress << 1;
	halResult = HAL_I2C_Mem_Read(&hi2c1, tempDevAddress, regAddress, I2C_MEMADD_SIZE_8BIT, regValue, regLength, 100);
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2cReceive);
	}

	halResult = HAL_I2C_Master_Receive(&hi2c1, tempDevAddress, regValue, 1, 100);  // timeout - 100 mSec
	if(halResult != HAL_OK)
	{
		return(OsResult_ErrorI2cReceive);
	}


	return(OsResult_Ok);
}
//=== end ReadRegisterI2c1 =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult ReadRawI2c1(u8 devAddress, u16 dataLength, u8* readData)
{
/*	HAL_StatusTypeDef resultHal;
	u8 bufferTransmit[10];


	bufferTransmit[0] = regAddress;

	resultHal = HAL_I2C_Master_Transmit(this->hI2c, (u16)(this->chipAddress << 1), bufferTransmit, 1, 100);  // timeout - 100 mSec
	if(resultHal != HAL_OK)
	{
		return(OsResult_ErrorI2cTransmit);
	}

	resultHal = HAL_I2C_Master_Receive(this->hI2c, (u16)(this->chipAddress << 1), bufferTransmit, 1, 100);  // timeout - 100 mSec
	if(resultHal != HAL_OK)
	{
		return(OsResult_ErrorI2cReceive);
	}


	*regData = bufferTransmit[0]; */


	return(OsResult_Ok);
}
//=== end ReadRawI2c1 ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult WriteRegisterI2c1(u8 devAddress, u8 regAddress, u16 regLength, u8* regValue)
{

}
//=== end WriteRegisterI2c1 ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult WriteRawRegisterI2c1(u8 devAddress, u16 regLength, u8* regValue)
{

}
//=== end WriteRawRegisterI2c1 =====================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult ReadRegisterI2c2(u8 devAddress, u8 regAddress, u16 regLength, u8* regValue)
{

}
//=== end ReadRegisterI2c2 =========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult ReadRawRegisterI2c2(u8 devAddress, u16 regLength, u8* regValue)
{

}
//=== end ReadRawRegisterI2c2 ======================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult WriteRegisterI2c2(u8 devAddress, u8 regAddress, u16 regLength, u8* regValue)
{

}
//=== end WriteRegisterI2c2 ========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult WriteRawRegisterI2c2(u8 devAddress, u16 regLength, u8* regValue)
{

}


/**********************************************************************************/
