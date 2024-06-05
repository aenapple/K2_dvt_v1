/**********************************************************************************/
/**
 *  @file MotorChamberRight.hpp
 *  @brief Header file for class TMotorChamberRight
 *
 *  @copyright (c) 2024 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __MotorChamberRight_H
#define __MotorChamberRight_H

/**********************************************************************************/
#include "MotorChamber.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
class TMotorChamberRight : public TMotorChamber
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void StartForward(void);
	void StartBackward(void);
	void Stop(void);


protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////


    ////// constants //////


    ////// functions //////


};
//=== end class TMotorChamberRight =================================================

/**********************************************************************************/
#endif
