/**********************************************************************************/
/**
 *  @file HeaterPtcRight.hpp
 *  @brief Header file for class THeaterPtcRight
 *
 *  @copyright (c) 2024 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __HeaterPtcRight_H
#define __HeaterPtcRight_H

/**********************************************************************************/
#include "Heater.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
class THeaterPtcRight : public THeater
{
public:
	////// variables //////


	////// constants //////


	////// functions //////
	void PulseOn(void);
	void PulseOff(void);


protected:
    ////// variables //////


    ////// constants //////


    ////// functions //////


private:
    ////// variables //////


    ////// constants //////


    ////// functions //////


};
//=== end class THeaterPtcRight ====================================================

/**********************************************************************************/
#endif
