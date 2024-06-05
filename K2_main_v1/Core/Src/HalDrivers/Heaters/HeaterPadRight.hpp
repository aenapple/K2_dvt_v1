/**********************************************************************************/
/**
 *  @file HeaterPadRight.hpp
 *  @brief Header file for class THeaterPadRight
 *
 *  @copyright (c) 2024 VCycene, Inc. All rights reserved. \n
 */
/**********************************************************************************/
#ifndef __HeaterPadRight_H
#define __HeaterPadRight_H

/**********************************************************************************/
#include "Heater.hpp"


/**********************************************************************************/


/**********************************************************************************/
//==================================================================================
class THeaterPadRight : public THeater
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
//=== end class THeaterPadRight ====================================================

/**********************************************************************************/
#endif
