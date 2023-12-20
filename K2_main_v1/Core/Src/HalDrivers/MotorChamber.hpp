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

enum EDirMotorChamber
{
	DirMotorChamber_Forward,
	DirMotorChamber_Backward,
};

/**********************************************************************************/
//==================================================================================
class TMotorChamber
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void Init(EMotorChamber motorChamber);
	void StartForward(void);
	void StartBackward(void);
	void Stop(void);


protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////
	GPIO_TypeDef *GPIOx_ON;
	u16 GPIO_Pin_ON;
	GPIO_TypeDef *GPIOx_DIR;
	u16 GPIO_Pin_DIR;

    ////// constants //////


    ////// functions //////


};
//=== end class TMotorChamber ======================================================

/**********************************************************************************/
#endif
