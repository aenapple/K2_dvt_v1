/**********************************************************************************/
/**
 *  @file MotorMain.hpp
 *  @brief Header file for class TMotorMain
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __MotorMain_H
#define __MotorMain_H

/**********************************************************************************/
#include "Os/Os.hpp"


/**********************************************************************************/



/**********************************************************************************/
//==================================================================================
class TMotorMain : public TOs
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	EOsResult Init(void);
	void StartForward(void);
	void StartBackward(void);
	void Stop(void);
	void BrakeOn(void);
	void BrakeOff(void);
	


protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////


    ////// constants //////


    ////// functions //////


};
//=== end class TMotorMain =========================================================

/**********************************************************************************/
#endif
