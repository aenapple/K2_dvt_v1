/**********************************************************************************/
/**
*  @file
*  Todo: StreamBuffer.cpp.
*
*/
/**********************************************************************************/
#include "StreamBuffer.hpp"


/**********************************************************************************/



/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EOsResult TStreamBuffer::CreateStatic(u32 sizeBuffer, u32 triggerLevel, u8* buffer)
{
	if(this->streamBuffer != NULL) return(OsResult_ErrorCreateResource);

	this->streamBuffer= xStreamBufferCreateStatic(sizeBuffer, triggerLevel, buffer, &createdStreamBuffer);
	if(this->streamBuffer == NULL) return(OsResult_ErrorCreateResource);

	return(OsResult_Ok);
}
//=== end CreateStatic =============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
StreamBufferHandle_t TStreamBuffer::GetHandle(void)
{
	return(this->streamBuffer);
}
//=== end GetHandle ===============================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EOsResult TStreamBuffer::Reset(void)
{
	if(xStreamBufferReset(this->streamBuffer) == pdPASS) return(OsResult_Ok);
	else return(OsResult_ResourceBusy);
}
//=== end Reset ====================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
EOsResult TStreamBuffer::SetTriggerLevel(u32 triggerLevel)
{
	if(xStreamBufferSetTriggerLevel(this->streamBuffer, triggerLevel) == pdPASS) return(OsResult_Ok);
	else return(OsResult_WrongParameter);
}
//=== end SetTriggerLevel ==========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
u32 TStreamBuffer::GetSpacesAvailable(void)
{
	return((unsigned int)xStreamBufferSpacesAvailable(this->streamBuffer));
}
//=== end GetSpacesAvailable =======================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
u32 TStreamBuffer::GetBytesAvailable(void)
{
	return((unsigned int)xStreamBufferBytesAvailable(this->streamBuffer));
}
//=== end GetBytesAvailable ========================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
u32 TStreamBuffer::Send(u8* inputBuffer, u32 numberBytes, u32 timeout)
{
	return(xStreamBufferSend(this->streamBuffer, inputBuffer, numberBytes, timeout));
}
//=== end Send =====================================================================

//==================================================================================
/**
*  Todo: function description.
*
*  @return void .
*/
u32 TStreamBuffer::Receive(u8* inputBuffer, u32 numberBytes, u32 timeout)
{
	return(xStreamBufferReceive(this->streamBuffer, inputBuffer, numberBytes, timeout));
}
//=== end Receive ==================================================================

/**********************************************************************************/
