/**********************************************************************************/
/**
 *  @file HalHeaters.hpp
 *  @brief Header file for class THalHeaters
 *
 *  @copyright (c) 2024 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __HalHeaters_H
#define __HalHeaters_H

/**********************************************************************************/
#include "HeaterPtcLeft.hpp"
#include "HeaterPtcRight.hpp"
#include "HeaterPadLeft.hpp"
#include "HeaterPadRight.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
class THalHeaters
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	static THalHeaters& GetInstance(void);

	void StartHeaterPtcLeft(u8 pwm);
	void StopHeaterPtcLeft(void);
	EHeaterPwm GetPwmHeaterPtcLeft(void);
	void PulseOnHeaterPtcLeft(void);
	void PulseOffHeaterPtcLeft(void);

	void StartHeaterPtcRight(u8 pwm);
	void StopHeaterPtcRight(void);
	EHeaterPwm GetPwmHeaterPtcRight(void);
	void PulseOnHeaterPtcRight(void);
	void PulseOffHeaterPtcRight(void);

	void StartHeaterPadLeft(u8 pwm);
	void StopHeaterPadLeft(void);
	EHeaterPwm GetPwmHeaterPadLeft(void);
	void PulseOnHeaterPadLeft(void);
	void PulseOffHeaterPadLeft(void);

	void StartHeaterPadRight(u8 pwm);
	void StopHeaterPadRight(void);
	EHeaterPwm GetPwmHeaterPadRight(void);
	void PulseOnHeaterPadRight(void);
	void PulseOffHeaterPadRight(void);



protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////
	THeaterPtcLeft HeaterPtcLeft;
	THeaterPtcRight HeaterPtcRight;
	THeaterPadLeft HeaterPadLeft;
	THeaterPadRight HeaterPadRight;


    ////// constants //////


    ////// functions //////
	THalHeaters(void);
	//~THalHeaters(void);


	THalHeaters(THalHeaters const&);
	THalHeaters& operator= (THalHeaters const&);


};
//=== end class THalHeaters =======================================================

/**********************************************************************************/
#endif
