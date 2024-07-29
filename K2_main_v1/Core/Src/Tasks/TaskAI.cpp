/*
 * TASKAI.cpp
 *
 *  Created on: Jul 9, 2024
 *      Author: junai
 */


#include "TaskCHM.hpp"
#include "TaskHAL.hpp"
#include "TaskSYS.hpp"
#include "TaskAI.hpp"

extern TTaskHAL TaskHAL;
extern TTaskSYS TaskSYS;

TTaskAI  TaskAILeft;
//TTaskAI  TaskAIRight;

#ifndef __RELEASE
	#include "TaskConsole.hpp"
	extern TTaskConsole TaskConsole;
#endif
/**********************************************************************************/
#ifdef __DEBUG_CHM_OUTPUT_ENABLED
	#define DiagPrintf(...) printf(__VA_ARGS__)  // TaskConsole.DebugPrintf(__VA_ARGS__)
	#define DiagNotice(fmt, ...) TaskConsole.DebugNotice("CHM: " fmt "\r\n", ##__VA_ARGS__)
#else
	#define DiagPrintf(...)
	#define DiagNotice(...)
#endif


/**********************************************************************************/


/**********************************************************************************/
/**
*  Todo: function description..
*
*  @return ... .
*/
u16 TTaskAI::GetMixingCounter(void) {

	u16 counter;
	return counter;
}
////=== end GetMixingCounter =========================================================

ETaskCompostingState TTaskAI::GetCompostingState(void) {
	return this->compostingState;
}

u32 GetMoisturePredictions(void) {
	u32 pMoisture;

	return pMoisture;
}

/**********************************************************************************/
void TTaskAI::PadDutyCycle(void) {
	switch (this->padDutyCycle) {
		case padDutyCycleMode_0:
			if (this->padDutyCycleOnFlag)
			{
				this->padHeaterWorkTime  = this->padHeaterIntervalTime;
				this->padDutyCycleOnFlag = false;

				padHeaterPwm = 20;

				mixIntervalTime    = TASK_SYS_5_MINUTES;
			}
			else
			{

				padHeaterWorkTime  = TASK_SYS_1_MINUTE;
				padDutyCycleOnFlag = true;

				padHeaterPwm = 100;
			}
		case padDutyCycleMode_1:
			if (this->padDutyCycleOnFlag)
			{
				this->padHeaterWorkTime  = this->padHeaterIntervalTime;
				this->padDutyCycleOnFlag = false;

				padHeaterPwm = 60;

				mixIntervalTime    = TASK_SYS_5_MINUTES;
			}
			else
			{

				padHeaterWorkTime  = TASK_SYS_1_MINUTE;
				padDutyCycleOnFlag = true;

				padHeaterPwm = 100;


			}
		case padDutyCycleMode_2:
			if (this->padDutyCycleOnFlag) {

				this->padHeaterWorkTime  = this->padHeaterIntervalTime;
				this->padDutyCycleOnFlag = false;

				padHeaterPwm = 40;

				mixIntervalTime    = TASK_SYS_15_MINUTES;
			}
			else
			{

				padHeaterWorkTime  = TASK_SYS_1_MINUTE;
				padDutyCycleOnFlag = true;

				padHeaterPwm = 80;
			}
		case padDutyCycleMode_3:
			if (this->padDutyCycleOnFlag)
			{
				this->padHeaterWorkTime  = this->padHeaterIntervalTime;
				this->padDutyCycleOnFlag = false;

				padHeaterPwm 		= 20;
				mixIntervalTime    	= TASK_SYS_40_MINUTES;
			}
			else
			{

				padHeaterWorkTime  = TASK_SYS_1_MINUTE;
				padDutyCycleOnFlag = true;

				padHeaterPwm = 60;
			}
	}

}
////=== end PadDutyCycle =========================================================


/**
*  Todo: function description..
*
*  @return ... .
*/
//
void TTaskAI::Run(TBme688Sensor bme688Sensor, EMixingPhase otherChamberMixPhase, u16 lowTemp, u16 highTemp) {

	if (this->padHeaterWorkTime <= 0)
	{
		this->PadDutyCycle();
	}
	else
	{
		this->padHeaterWorkTime -= this->incrementTime;
	}

	this->UpdateFeatureVectorAndPredictMoisture(false);


	u32 insertIndex = (this->mlWindowShort - this->mlWindowCounterShort) / this->incrementTime;
	if (otherChamberMixPhase == MixingPhase_0) {
		this->collectedTempShort[insertIndex] = bme688Sensor.temperature;
		this->collectedHumidityShort[insertIndex] = bme688Sensor.humidity;
		this->collectedGasShort[insertIndex] = bme688Sensor.gasResistance;
		this->collectedPressShort[insertIndex] = bme688Sensor.pressure;
		this->collectedPadHeaterPwmShort[insertIndex] = this->padHeaterPwm;

		this->mlWindowCounterShort -= this->incrementTime;
	}


	this->BmeController(bme688Sensor, this->moisturePredictionShort);
}


////=== end Run ======================================================================

/**
*  Todo: function description..
*
*  @return ... .
*/
//
void TTaskAI::InsertPtcValues(u8 ptcHeaterPwm, s8 ptcFanPwm, EMixingPhase otherChamberMixPhase) {

	u32 index = (this->mlWindowShort - this->mlWindowCounterShort) / this->incrementTime;
	if (otherChamberMixPhase == MixingPhase_0) {
		this->collectedPtcHeaterPwmShort[index] = ptcHeaterPwm;
		this->collectedPtcFanPwmShort[index]   = ptcFanPwm;
	}

}

////=== end InsertPtcValues ======================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
//
//void TTaskAI::InsertBmeValues(TBme688Sensor bme688Sensor, u32 insertIndex, u8 size) {
//
//}
////=== end InsertBmeValues ======================================================================

/**
*  Todo: function description..
*
*  @return ... .
*/
//
////=== end InsertPadHeaterValues ======================================================================



/**
*  Todo: function description..
*
*  @return ... .
*/
//
u8* TTaskAI::InterpolateValues(u8* collectedSensorValues, u16 size) {
    u16 i = 0;

    while (i < size) {
        if (collectedSensorValues[i] == 0) {
            // Find the start and end of the zero sequence
            u16 start = i;
            u16 end;


            // Find the end of the zero sequence
            // eg. {1,2,3,4,0,0,0,0,6,7,8,} Find the 0 before the 6.
			for (u16 j = start; j < size; j++) {
				if (collectedSensorValues[j] != 0) {
					end = j;
					break;
				}
				end = -1;
			} // end for

            if (start == 0 && end != -1) {
            	for (u16 j = start; j < end ; j++)
                    collectedSensorValues[j] = collectedSensorValues[end];
                i = end;
                continue;
            }

            // Handle trailing zeroes
            if (end == -1 && start != 0) {
            	u16 prev;
            	// in the example above, find the 4 starting
                for (u16 j = start; j > -1; j--) {
                	if (collectedSensorValues[j] != 0) {
                		prev = j;
                		break;
                	}
                	prev = -1;
            	} // end for

                for (u16 j; j < size; j++)
                    collectedSensorValues[j] = collectedSensorValues[prev];
                break;
            }

            if (end != -1) {
            	u16 prev;
                for (u16 j = start - 1; j > -1; j--) {
                	if (collectedSensorValues[j] != 0) {
                		prev = j;
                		break;
                	}
                	prev = -1;
            	} // end for
                u16 step = (collectedSensorValues[end] - collectedSensorValues[prev]) / (end - prev);

                for (u16 j = 1; j < (end - prev); j++)
                    collectedSensorValues[prev + j] = collectedSensorValues[prev] + step * j;
                i = end;
                continue;
            }
        i++;
        }
    return collectedSensorValues;
    } // end while
}
//=== end InterpolateValues ======================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
//

void TTaskAI::CalculateAllFeatures(void) {
	this->CalcMeanValues(this->collectedTempShort, TASK_AI_SAMPLES_SHORT);
//
//
	this->CalcMeanAbsChangeValues(this->collectedTempShort, TASK_AI_SAMPLES_SHORT);
//

	this->CalcSumValues(this->collectedTempShort, TASK_AI_SAMPLES_SHORT);

}
//=== end CalculateFeatures ======================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
//

float TTaskAI::CalcMeanValues(u8* parameterValues, u16 size) {

	float meanValue = 0;
	u16 tmpCount = 0;
	for (u16 i = 0; i < size; i++) {
		if (parameterValues[i] != 0) {
			meanValue += parameterValues[i];
			tmpCount++;
		}
	}

	return meanValue / tmpCount;
}

float TTaskAI::CalcMeanValues(u16* parameterValues, u16 size) {
	u32 meanValue = 0;

	return meanValue;
}

float TTaskAI::CalcMeanValues(u32* parameterValues, u16 size) {
	u64 meanValue = 0;

	return meanValue;
}


//=== end CalcMeanValues ======================================================================

/**
*  Todo: function description..
*
*  @return ... .
*/
//
float CalcMeanAbsChangeValues(u8* parameterValues, u16 size) {

	u16 sumOfAbsoluteChanges = 0;

    for (u16 i = 1; i < size; ++i) {
        u8 change = parameterValues[i] - parameterValues[i - 1];
        // Manually calculate the absolute value
        if (change < 0) {
            change = -change;
        }
        sumOfAbsoluteChanges += change;
    }

    // Calculate the mean absolute change
    u16 meanAbsoluteChange = sumOfAbsoluteChanges / (size - 1);
    return meanAbsoluteChange;

}

float CalcMeanAbsChangeValues(u16* parameterValues) {

	u32 meanAbsoluteChange = 0;
	return meanAbsoluteChange;
}

float CalcMeanAbsChangeValues(u32* parameterValues) {
	u64 meanAbsoluteChange = 0;
	return meanAbsoluteChange;
}
//=== end GetMeanAbsChangeValues ======================================================================

/**
*  Todo: function description..
*
*  @return ... .
*/
//
u16 CalcSumValues(u8* parameterValues, u16 size) {
	u16 sumValue = 0;

	for (u16 i = 0; i < size; i++) {
		sumValue += parameterValues[i];
	}

	return sumValue;

}

u32 CalcSumValues(u16* parameterValues, u16 size) {

}

u64 CalcSumValues(u32* parameterValues, u16 size) {

}


//=== end GetSumValues ======================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
//
void TTaskAI::ScaleAllParameters(void) {

	this->meanTempShort = this->MinMaxScaler(this->meanTempShort, 0, 1200);




}
//=== end ScaleAllParameters ======================================================================

/**
*  Todo: function description..
*
*  @return ... .
*/
//
void TTaskAI::UpdateFeatureVectorAndPredictMoisture(bool nn) {

	// TODO ask how to calculate these values because they're decimal values


	if (this->mlWindowCounterShort <= 0 || this->moisturePredictionShort != -1) {

		if (this->mlWindowCounterShort <= 0) {
			this->mlWindowCounterShort = this->mlWindowShort;
		}


		this->CalculateAllFeatures();


		if (nn) {
			this->PredictMoistureUsingNeuralNets();
		} else {
			this->PredictMoistureUsingRegression();
		}


	}



}

//=== end UpdateFeatureVectorAndPredictMoisture ======================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
//

u8 factorial(u8 n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    u8 result = 1;
    for (u8 i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

u16 CountCombinations(u8 numFeatures, u8 degree) {
    return factorial(numFeatures + degree - 1) / (factorial(degree) * factorial(numFeatures - 1));
}

float* CombinationsWithReplacement(float* arr, u8 n, u8 degree, u8& totalCombinations) {
    // Calculate the number of combinations
    totalCombinations = CountCombinations(n, degree);


    // Allocate memory for the products array
    float* products = new float[totalCombinations];

    // Initialize the combination indices
    u8* indices = new u8[degree];
    for (u8 i = 0; i < degree; ++i) {
        indices[i] = 0;
    }

    u8 combinationIndex = 0;

    while (true) {
        // Calculate the product of the current combination based on indices
        u8 product = 1;
        for (u8 i = 0; i < degree; ++i) {
            product *= arr[indices[i]];
        }
        products[combinationIndex] = product;
        combinationIndex++;

        // Find the rightmost index that can be incremented
        u8 incrementIndex = degree - 1;
        while (incrementIndex >= 0 && indices[incrementIndex] == n - 1) {
            --incrementIndex;
        }

        // If all indices are at their maximum value, we are done
        if (incrementIndex < 0) {
            break;
        }

        // Increment this index
        ++indices[incrementIndex];

        // Set all subsequent indices to the value of incrementIndex
        for (u8 i = incrementIndex + 1; i < degree; ++i) {
            indices[i] = indices[incrementIndex];
        }
    }

    delete[] indices;
    return products;
}
//=== end CombinationsWithReplacement ======================================================================

/**
*  Todo: function description..
*
*  @return ... .
*/
//
void TTaskAI::PredictMoistureUsingRegression(void) {

	float regressionFeatures[TASK_AI_NUM_FEATURES_FOUR] = {1, 2, 3, 4};
	float regressionCoefficients[TASK_AI_DEGREE_FOUR_COEFFICIENTS] = {1,2,3,4,5,6,6,7,8};
	float regressionu8ercept = 0;

    u8 degree = 3;  // Specify the degree of the combinations
    u8 n = sizeof(regressionFeatures) / sizeof(regressionFeatures[0]);

    u8 totalCombinations = 0;
    float* regressionFeaturesCombination = this->CombinationsWithReplacement(regressionFeatures, n, degree, totalCombinations);
    float prediction = 0;

    for (u8 i = 0; i < TASK_AI_DEGREE_FOUR_COEFFICIENTS; i++) {
    	prediction += regressionFeaturesCombination[i] * regressionCoefficients[i];
    }
    prediction += regressionu8ercept;


}
//=== end PredictMoistureUsingRegression ======================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
//
void TTaskAI::PredictMoistureUsingNeuralNets(void) {

}
//=== end PredictMoistureUsingNeuralNets =========================================================



//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskAI::Init(u8 incrementTime)
{
	this->incrementTime = incrementTime;

    this->padHeaterPwm    		= 70;

    this->mixIntervalTime 		= -1;
    this->mixingCounter   		= this->mixIntervalTime;
    this->mixPhase        		= MixingPhase_0;

    this->padDutyCycle          = padDutyCycleMode_0;
    this->padDutyCycleOnFlag 	= true;

    this->padHeaterIntervalTime = TASK_SYS_30_SECONDS;
    this->padHeaterWorkTime     = this->padHeaterIntervalTime;

    this->mlWindowShort   		= TASK_SYS_15_MINUTES;
    this->mlWindowCounterShort  = this->mlWindowShort;

    this->moisturePredictionShort = -1;

    this->compostingState = CompostingState_StartingUp;



	return OsResult_Ok;
}
//=== end Init ======================================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
EOsResult TTaskAI::CreateOsResources(void)
{

	return(OsResult_Ok);
}
//
//=== end CreateOsResources ========================================================
//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskAI::CreateTask(void)
{
	((TOsTask*)this)->CreateTask("UID TASK", OS_TASK_AI_SIZE_STACK, OS_TASK_AI_PRIORITY);
}
//=== end CreateTask ===============================================================

//==================================================================================
/**
*  Todo: function description..
*
*  @return ... .
*/
void TTaskAI::CreateTaskStatic(void)
{
	((TOsTask*)this)->CreateTaskStatic("UI TASK", OS_TASK_AI_SIZE_STACK, OS_TASK_AI_PRIORITY, xStackBuffer, &xTaskBuffer);
}
////=== end CreateTaskStatic =========================================================

