/**********************************************************************************/
/**
 *  @file PtcFan.hpp
 *  @brief Header file for class TPtcFan
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __PtcFan_H
#define __PtcFan_H

/**********************************************************************************/
#include "Os.hpp"


/**********************************************************************************/
enum EPtcFan
{
	PtcFan_Left,
	PtcFan_Right,
};


/**********************************************************************************/
//==================================================================================
class TPtcFan
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void Init(EPtcFan ptcFan);
	void Start(u8 pwm);
	void Stop(void);



protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////
	u8 pwm;
	bool pwmStarted;
	TIM_HandleTypeDef* pTimer;
	u32 channel;


    ////// constants //////


    ////// functions //////


};
//=== end class TPtcFan ============================================================

/**********************************************************************************/
#endif
