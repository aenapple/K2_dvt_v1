/**********************************************************************************/
/**
*  @file
*  Todo: OsQueue.cpp.
*
*/
/**********************************************************************************/
#include "OsMutex.hpp"

/**********************************************************************************/
//==================================================================================
/**
*  Todo: function description
*
*  @return ... .
*/
EOsResult TOsMutex::CreateStatic(void)
{
    if(this->SemaphoreHandle != NULL)
    {
        return(OsResult_ErrorCreateResource);
    }

    this->SemaphoreHandle = xSemaphoreCreateMutexStatic(&this->StaticSemaphore);
    if(this->SemaphoreHandle == NULL)
    {
        return(OsResult_ErrorCreateResource);
    }

    return(OsResult_Ok);
}
//=== end CreateStatic =============================================================

/**********************************************************************************/
