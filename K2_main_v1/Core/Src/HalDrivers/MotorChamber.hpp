/**********************************************************************************/
/**
 *  @file MotorChamber.hpp
 *  @brief Header file for class TMotorChamber
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __MotorChamber_H
#define __MotorChamber_H

/**********************************************************************************/
#include "Os.hpp"


/**********************************************************************************/
enum EMotorChamber
{
	MotorChamber_Left,
	MotorChamber_Right,
};

/**********************************************************************************/
//==================================================================================
class TMotorChamber
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	EOsResult Init(void);
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
//=== end class TMotorChamber ======================================================

/**********************************************************************************/
#endif
