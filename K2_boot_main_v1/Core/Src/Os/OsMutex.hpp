/**
*  @file
*  OsMutex.hpp - declaration of the class TOsMutex.
*
*/
/**********************************************************************************/
#ifndef __OsMutex_H
#define __OsMutex_H

/**********************************************************************************/
#include "OsSemaphore.hpp"

/**********************************************************************************/
class TOsMutex : public TOsSemaphore
{
public:
	/// variables ///

	/// constants ///

	/// functions ///
	EOsResult CreateStatic(void);


protected:
	/// variables ///

	/// constants ///

	/// functions ///


private:
	/// variables ///

	/// constants ///

	/// functions ///

};
//=== end class TOsMutex ===========================================================

/**********************************************************************************/
#endif
