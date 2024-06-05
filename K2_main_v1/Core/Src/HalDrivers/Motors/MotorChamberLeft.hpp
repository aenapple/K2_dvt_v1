/**********************************************************************************/
/**
 *  @file MotorChamberLeft.hpp
 *  @brief Header file for class TMotorChamberLeft
 *
 *  @copyright (c) 2024 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __MotorChamberLeft_H
#define __MotorChamberLeft_H

/**********************************************************************************/
#include "MotorChamber.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
class TMotorChamberLeft : public TMotorChamber
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
//=== end class TMotorChamberLeft ==================================================

/**********************************************************************************/
#endif
