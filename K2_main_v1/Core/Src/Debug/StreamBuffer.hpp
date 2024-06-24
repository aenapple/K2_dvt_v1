/**********************************************************************************/
/**
*  @file
*  StreamBuffer.hpp - declaration of class TStreamBuffer.
*
*/

/**********************************************************************************/
#ifndef __StreamBuffer_H
#define __StreamBuffer_H

/**********************************************************************************/
#include "Os\Os.hpp"

/**********************************************************************************/



/**********************************************************************************/
//==================================================================================
class TStreamBuffer : public TOs
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	TStreamBuffer(void)
	{
		this->streamBuffer= NULL;
	}
	EOsResult CreateStatic(u32, u32, u8*);
	StreamBufferHandle_t GetHandle(void);
	EOsResult Reset(void);
	EOsResult SetTriggerLevel(u32);
	u32 GetSpacesAvailable(void);
	u32 GetBytesAvailable(void);
	u32 Send(u8*, u32, u32);
	u32 Receive(u8*, u32, u32);



protected:
	////// variables //////


	////// constants //////


	////// functions //////
  

private:
	////// variables //////
	StreamBufferHandle_t streamBuffer;
	StaticStreamBuffer_t createdStreamBuffer;




	////// constants //////


	////// functions //////


};
//=== end class TStreamBuffer ======================================================

/**********************************************************************************/

#endif
