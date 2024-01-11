/**********************************************************************************/
/**
*  @file
*  Todo: InterfaceVIP.cpp.
*
*/
/**********************************************************************************/
#include "InterfaceVIP.hpp"


/**********************************************************************************/
/* #define SYS_STATE_TABLE(enumState, interfaceEvseCode, errorCode) \
		interfaceEvseCode

u8 TInterfaceVIP::ifsEvseStateCode[Number_ESysState] =
{
#include <TableSysState.h>
};

#undef SYS_STATE_TABLE */

extern UART_HandleTypeDef huart1;



/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TInterfaceVIP::Init(UART_HandleTypeDef* hUart, USART_TypeDef* uartInstance)
{
//	this->hUart = hUart;
//	this->uartInstance = uartInstance;
}
//=== end Init ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TInterfaceVIP::ParsingRxPacket(void)
{
    if(this->bufferUartDmaRx[IFC_VIP_INDEX_CRC] != this->CalcCrc(this->bufferUartDmaRx, IFC_VIP_INDEX_CRC))
    {
    	return(OsResult_ErrorCrc);
    }
    
    this->command = (EIfcVipCommand)this->bufferUartDmaRx[IFC_VIP_INDEX_CMD];

    memcpy(this->bufferUartDataRx, &this->bufferUartDmaRx[IFC_VIP_INDEX_DATA], IFC_VIP_UART_SIZE_DATA);


	return(OsResult_Ok);
}
//=== end ParsingRxPacket ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TInterfaceVIP::StartTxData(EIfcVipCommand command, u8* data)
{
	this->bufferUartDmaTx[IFC_VIP_INDEX_CMD] = (u8)command;
	for(u16 i = 0; i < IFC_VIP_UART_SIZE_DATA; i++)
	{
		if(data == NULL) this->bufferUartDmaTx[IFC_VIP_INDEX_DATA + i] = 0;
		else this->bufferUartDmaTx[IFC_VIP_INDEX_DATA + i] = data[i];
	}
	this->bufferUartDmaTx[IFC_VIP_INDEX_CRC] = this->CalcCrc(this->bufferUartDmaTx, IFC_VIP_UART_SIZE_PACKET - 1);

	this->StartUartTxData(this->bufferUartDmaTx, IFC_VIP_UART_SIZE_PACKET);


    return(OsResult_Ok);
}
//=== end StartTxData ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TInterfaceVIP::StartRxData(void)
{
	this->StartUartRxData(this->bufferUartDmaRx, IFC_VIP_UART_SIZE_PACKET);


	return(OsResult_Ok);
}
//=== end StartRxData ==============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TInterfaceVIP::StartUartTxData(u8* pBuffer, u16 numBytes)
{
	HAL_UART_Transmit_DMA(&huart1, pBuffer, numBytes);

	return(OsResult_Ok);
}
//=== end StartUartTxData ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TInterfaceVIP::StartUartRxData(u8* pBuffer, u16 numBytes)
{
	HAL_UART_Receive_DMA(&huart1, pBuffer, numBytes);

	return(OsResult_Ok);
}
//=== end StartUartRxData ==========================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
/* u8 TInterfaceVIP::GetStateCode(ESysState sysState)
{
	return(this->ifsEvseStateCode[sysState]);
} */
//=== end GetStateCode =============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TInterfaceVIP::ReInit(void)
{
	HAL_UART_DeInit(&huart1);
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 9600;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_TXINVERT_INIT;
	huart1.AdvancedInit.TxPinLevelInvert = UART_ADVFEATURE_TXINV_ENABLE;
  	HAL_UART_Init(&huart1);
// 	HAL_UARTEx_SetTxFifoThreshold(this->hUart, UART_TXFIFO_THRESHOLD_1_8);
// 	HAL_UARTEx_SetRxFifoThreshold(this->hUart, UART_RXFIFO_THRESHOLD_1_8);
// 	HAL_UARTEx_DisableFifoMode(this->hUart);

}
//=== end ReInit ===================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u8 TInterfaceVIP::CalcCrc(u8* pBuffer, u16 numBytes)
{
	u8 crc;


    crc = 0;
    for(u16 i= 0; i < numBytes; i++) crc += pBuffer[i];
    crc += 1;


	return(crc);
}
//=== end CalcCrc ==================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EIfcVipCommand TInterfaceVIP::GetCommand(void)
{
	return(this->command);
}
//=== end GetCommand ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
u8* TInterfaceVIP::GetPointerDataRx(void)
{
	return(this->bufferUartDataRx);
}
//=== end GetPointerDataRx =========================================================

/**********************************************************************************/
