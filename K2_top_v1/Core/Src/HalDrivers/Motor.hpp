/**********************************************************************************/
/**
 *  @file Motor.hpp
 *  @brief ...
 *
 *  @copyright (c) 2023 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __Motor_H
#define __Motor_H

/**********************************************************************************/
#include "Os/Os.hpp"


/**********************************************************************************/



/**********************************************************************************/
//==================================================================================
class TMotor : public TOs
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void Init(void);
	void StartForward(void);
	void StartBackward(void);
	void Stop(void);
	void StopHZ(void);


protected:
	////// variables //////


	////// constants //////


	////// functions //////

private:
	////// variables //////



	////// constants //////



	////// functions //////




};
//=== end class TMotor =============================================================

/**********************************************************************************/
#endif
