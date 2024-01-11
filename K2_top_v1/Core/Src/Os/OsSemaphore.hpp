/**********************************************************************************/
/**
*  @file
*  OsSemaphore.hpp - declaration of the class TOsSemaphore.
*
*/
/**********************************************************************************/
#ifndef __OsSemaphore_H
#define __OsSemaphore_H

/**********************************************************************************/
#include "Os.hpp"


/**********************************************************************************/
//==================================================================================
class TOsSemaphore : public TOs
{
public:
	/// variables ///

	/// constants ///

	/// functions ///
	TOsSemaphore(void);
	EOsResult CreateStatic(void);
	SemaphoreHandle_t GetHandle(void);
	EOsResult Take(u32);
	EOsResult TakeRecursive(u32);
	EOsResult Reset(void);
	void Give(void);
	void GiveRecursive(void);
	void GiveFromIsr(void);
	void TakeFromIsr(void);

  
protected:
	/// variables //
	SemaphoreHandle_t SemaphoreHandle;
	StaticSemaphore_t StaticSemaphore;

	/// constants ///

	/// functions ///


private:
	/// variables ///

	/// constants ///

	/// functions ///


};
//=== end class TOsSemaphore =======================================================

/**********************************************************************************/
#endif
