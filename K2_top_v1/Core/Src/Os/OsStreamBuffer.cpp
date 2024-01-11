/**********************************************************************************/
/**
*  @file
*  Todo: OsStreamBuffer.cpp.
*
*/
/**********************************************************************************/
#include "OsStreamBuffer.hpp"

/**********************************************************************************/
//==================================================================================
/**
*  Constructor.
*
*  @return ... .
*/
TOsStreamBuffer::TOsStreamBuffer(void)
{
	this->StreamBufferHandle= NULL;
}
//=== end TOsStreamBuffer ==========================================================

//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
EOsResult TOsStreamBuffer::CreateStatic(u32 size_buffer, u32 trigger_level, u8* buffer)
{
	if(this->StreamBufferHandle != NULL)
	{
		return(OsResult_ErrorCreateResource);
	}

	this->StreamBufferHandle= xStreamBufferCreateStatic(size_buffer, trigger_level, buffer, &this->StaticStreamBuffer);
	if(this->StreamBufferHandle == NULL)
	{
		return(OsResult_ErrorCreateResource);
	}

	return(OsResult_Ok);
}
//=== end CreateStatic =============================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
StreamBufferHandle_t TOsStreamBuffer::GetHandle(void)
{
	return(this->StreamBufferHandle);
}
//==== end GetHandle ===============================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
EOsResult TOsStreamBuffer::Reset(void)
{
	if(xStreamBufferReset(this->StreamBufferHandle) == pdPASS)
	{
		return(OsResult_Ok);
	}

	return(OsResult_ResourceBusy);
}
//=== end Reset ====================================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
EOsResult TOsStreamBuffer::SetTriggerLevel(u32 trigger_level)
{
	if(xStreamBufferSetTriggerLevel(this->StreamBufferHandle, trigger_level) == pdPASS)
	{
		return(OsResult_Ok);
	}

	return(OsResult_WrongParameter);
}
//=== end SetTriggerLevel ==========================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
u32 TOsStreamBuffer::GetSpacesAvailable(void)
{
	return((u32)xStreamBufferSpacesAvailable(this->StreamBufferHandle));
}
//=== end GetSpacesAvailable =======================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
u32 TOsStreamBuffer::GetBytesAvailable(void)
{
	return((u32)xStreamBufferBytesAvailable(this->StreamBufferHandle));
}
//=== end GetBytesAvailable ========================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
u32 TOsStreamBuffer::Send(u8* output_buffer, u32 number_bytes, u32 timeout)
{
	return(xStreamBufferSend(this->StreamBufferHandle, output_buffer, number_bytes, timeout));
}
//=== end Send =====================================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
u32 TOsStreamBuffer::Receive(u8* input_buffer, u32 number_bytes, u32 timeout)
{
	return(xStreamBufferReceive(this->StreamBufferHandle, input_buffer, number_bytes, timeout));
}
//=== end Receive ==================================================================

//==================================================================================
/**
*  Todo: function description
*
*  	@return
*/
EOsResult TOsStreamBuffer::SendFromIsr(u8* output_buffer, u32 number_bytes)
{
	BaseType_t result;
	BaseType_t highPriorityTaskWoken;


	highPriorityTaskWoken = pdFALSE;
	result= xStreamBufferSendFromISR(this->StreamBufferHandle, output_buffer, number_bytes, &highPriorityTaskWoken);

	if(result == pdPASS)
	{
		portYIELD_FROM_ISR(highPriorityTaskWoken);
		return(OsResult_Ok);
	}

	return(OsResult_ResourceBusy);
}
//=== end SendFromIsr ==============================================================

/**********************************************************************************/
