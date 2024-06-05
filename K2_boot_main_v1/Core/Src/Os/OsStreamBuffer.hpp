/**********************************************************************************/
/**
*  @file
*  OsStreamBuffer.hpp - declaration of the class TOsStreamBuffer.
*
*/
/**********************************************************************************/
#ifndef __OsStreamBuffer_H
#define __OsStreamBuffer_H

/**********************************************************************************/
#include "Os.hpp"

/**********************************************************************************/
//==================================================================================
class TOsStreamBuffer : public TOs
{
public:
	/// variables ///

	/// constants ///

	/// functions ///
	TOsStreamBuffer(void);
	EOsResult CreateStatic(u32, u32, u8*);
	StreamBufferHandle_t GetHandle(void);
	EOsResult Reset(void);
	EOsResult SetTriggerLevel(u32);
	u32 GetSpacesAvailable(void);
	u32 GetBytesAvailable(void);
	u32 Send(u8*, u32, u32);
	u32 Receive(u8*, u32, u32);
	EOsResult SendFromIsr(u8*, u32);



protected:
	/// variables ///

	/// constants ///

	/// functions ///


private:
	/// variables ///
	StreamBufferHandle_t StreamBufferHandle;
	StaticStreamBuffer_t StaticStreamBuffer;

	/// constants ///

	/// functions ///


};
//=== end class TOsStreamBuffer ====================================================

/**********************************************************************************/
#endif
