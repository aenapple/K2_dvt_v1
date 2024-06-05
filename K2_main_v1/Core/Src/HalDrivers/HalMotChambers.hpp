/**********************************************************************************/
/**
 *  @file HalMotChambers.hpp
 *  @brief Header file for class THalMotChambers
 *
 *  @copyright (c) 2024 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __HalMotChambers_H
#define __HalMotChambers_H

/**********************************************************************************/
#include "Os/Os.hpp"
#include "MotorChamberLeft.hpp"
#include "MotorChamberRight.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
class THalMotChambers
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	static THalMotChambers& GetInstance(void);

	void StartForwardMotorLeft(void);
	void StartBackwardMotorLeft(void);
	void StopMotorLeft(void);
	u8 GetPwmMotorLeft(void);
	bool GetDirMotorLeft(void);

	void StartForwardMotorRight(void);
	void StartBackwardMotorRight(void);
	void StopMotorRight(void);
	u8 GetPwmMotorRight(void);
	bool GetDirMotorRight(void);


protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////
	TMotorChamberLeft MotorChamberLeft;
	TMotorChamberRight MotorChamberRight;


    ////// constants //////


    ////// functions //////
	THalMotChambers(void);
	//~THalMotChambers(void);


	THalMotChambers(THalMotChambers const&);
	THalMotChambers& operator= (THalMotChambers const&);

};
//=== end class THalMotChambers ====================================================

/**********************************************************************************/
#endif
