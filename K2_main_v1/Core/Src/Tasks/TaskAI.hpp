/*
 * TASKAI.hpp
 *
 *  Created on: Jul 9, 2024
 *      Author: junaid
 */

#ifndef SRC_TASKS_TASKAI_HPP_
#define SRC_TASKS_TASKAI_HPP_

/**********************************************************************************/
/**
*  @file
*  TaskCHM.hpp - declaration of class TTaskCHM.
*
*  Target:  ...
*/

/**********************************************************************************/
#ifndef __TaskAI_H
#define __TaskAI_H

/**********************************************************************************/
#include "TaskSYS.hpp"
#include "TaskCHM.hpp"
#include "Interfaces/InterfaceVIP.hpp"

#ifndef __RELEASE
	#include "TaskConsole.hpp"
	extern TTaskConsole TaskConsole;
#endif
/**********************************************************************************/
#ifdef __DEBUG_AI_OUTPUT_ENABLED
	#define DiagPrintf(...) printf(__VA_ARGS__)  // TaskConsole.DebugPrintf(__VA_ARGS__)
	#define DiagNotice(fmt, ...) TaskConsole.DebugNotice("AI: " fmt "\r\n", ##__VA_ARGS__)
#else
	#define DiagPrintf(...)
	#define DiagNotice(...)
#endif



/**********************************************************************************/

#define TASK_AI_SAMPLES_PER_MINUTE			 (u8)(2)
#define TASK_AI_SAMPLES_SHORT                (u8)(2*15)
#define TASK_AI_SAMPLES_MEDIUM               (u8)(2*60)


#define TASK_AI_NUM_FEATURES_FOUR                 4

#define TASK_AI_DEGREE_POLYNOMIAL_FOUR		      4
#define TASK_AI_DEGREE_FOUR_COEFFICIENTS          9


#define TASK_AI_TIME_SHORT                        0
#define TASK_AI_TIME_MEDIUM                       1
#define TASK_AI_TIME_LONG 						  2
/**********************************************************************************/

typedef enum
{
	CompostingState_StartingUp  = 0,
	CompostingState_Collect     = 1,
	CompostingState_Meso        = 2,
	CompostingState_Thermo      = 3,
	CompostingState_HeatPurge   = 4,
	CompostingState_Stasis      = 5

} ETaskCompostingState;

typedef enum
{
	MixingPhase_0,
	MixingPhase_1,
	MixingPhase_2,
	MixingPhase_3,
	MixingPhase_4,

} EMixingPhase;

typedef enum
{
	padDutyCycleMode_0 = 0,
	padDutyCycleMode_1 = 1,
	padDutyCycleMode_2 = 2,
	padDutyCycleMode_3 = 3
} ETaskPadDutyCycle;

/**********************************************************************************/
//==================================================================================
class TTaskAI: public TOsTask
{
public:
	////// variables //////
	// DEBUG


	// DEBUG


	////// constants //////
//
//
//	////// functions //////
	EOsResult Init(u8 incrementTime);
	EOsResult Init(void) { return(OsResult_Ok); }


	// Need to make this consistent with the other classes ---- I can just call it update, and then use the Run function instead :)
	void RunChamber(TBme688Sensor bme688Sensor, u8 ptcHeaterPwm, s8 ptcFanPwm, EMixingPhase otherChamberMixPhase);
//	void SetError(void);
//

	float GetPredictions(void);
	s32 GetMixingCounter(void);
	ETaskCompostingState GetCompostingState(void);

	void DecrementMixingCounter(void);
	void SetMixingCounter(void);

//	void SetEventTickProcessFromISR(void);
//
//
	void CreateTask(void);
	void CreateTaskStatic(void);


protected:
	////// variables //////

	////// constants //////


	////// functions //////




private:
	////// variables //////
	StackType_t xStackBuffer[OS_TASK_AI_SIZE_STACK];
	bool error;


	////// Composting Parameters /////
	ETaskCompostingState compostingState;

	u32 mixIntervalTime;
	s32 mixingCounter;
//	EMixingPhase mixPhase;
//	EMixingPhase otherChamberMixPhase;

	u16 padHeaterIntervalTime;
	u16 padHeaterWorkTime;
	u16 padHeaterPwm;


	ETaskPadDutyCycle padDutyCycle;
	bool padDutyCycleOnFlag;
	////// End Composting Parameters /////

	////// Moisture Predictions //////

	float moisturePredictionShort;
	float previousMoistureShort;

	////// End Moisture Predictions //////

	////// Feature Collection //////

	TBme688Sensor bme688Sensor;

	u32 mlWindowShort;
	u32 mlWindowCounterShort;


	u16 collectedTempShort[TASK_AI_SAMPLES_SHORT];
	u16 collectedHumidityShort[TASK_AI_SAMPLES_SHORT];
	u32 collectedGasShort[TASK_AI_SAMPLES_SHORT];
	u32 collectedPressShort[TASK_AI_SAMPLES_SHORT];

	u8 collectedPadHeaterPwmShort[TASK_AI_SAMPLES_SHORT];

	u8 collectedPtcHeaterPwmShort[TASK_AI_SAMPLES_SHORT];
	u8 collectedPtcFanPwmShort[TASK_AI_SAMPLES_SHORT];

	////// End Feature Collection //////

	////// Features Calculated //////
	float meanTempShort;
	float meanHumidityShort;
	float meanGasShort;
	float meanPressShort;

	float meanPadHeaterPwmShort;
	float meanPtcHeaterPwmShort;
	float meanPtcFanPwmShort;

	float meanAbsChangeTempShort;
	float meanAbsChangeHumidityShort;
	float meanAbsChangeGasShort;
	float meanAbsChangePressShort;

	u16 sumTempShort;
	u16 sumHumidityShort;
	u32 sumGasShort;
	u32 sumPressShort;
	////// End Features Calculated //////

	////// constants //////
	u8 incrementTime;
	u8 incrementCounter;

	////// functions //////


	//// Chamber Actuation
	void PadDutyCycle(void);
	void BmeController(TBme688Sensor bme688Sensor, float predictedMoisture);

	void ActuatorErrorCheck(void);

	//// End Chamber Actuation

	//// Collect Data
	//					Consider moving the data structure for the ptc values to TASKCHM because it might be not useful here. I can get the necessary values from the functions.

//	void InsertBmeValues(TBme688Sensor bme688Sensor, u32 insertIndex, u8 size);
//	void InsertPadHeaterValues(u32 index);
	//// End Collect Data

	//// Populate Features
	u8* InterpolateValues(u8* collectedSensorValues, u16 size);


	void CalculateAllFeatures(void);

	float CalcMeanValues(u8* parameterValues, u16 size);
	float CalcMeanValues(u16* parameterValues, u16 size);
	float CalcMeanValues(u32* parameterValues, u16 size);

	float CalcMeanAbsChangeValues(u8* parameterValues, u16 size);
	float CalcMeanAbsChangeValues(u16* parameterValues, u16 size);
	float CalcMeanAbsChangeValues(u32* parameterValues, u16 size);

	float CalcSumValues(u8* parameterValues, u16 size);
	float CalcSumValues(u16* parameterValues, u16 size);
	float CalcSumValues(u32* parameterValues, u16 size);

	//// End Populate Features

    //// Feature Vector Generator

	float MinMaxScaler(float rawValue, float min_, float scale_) {return (rawValue - min_) * scale_;}

	void ScaleAllParameters(void);
	void UpdateFeatureVectorAndPredictMoisture(bool nn);

	u16 factorial(u8 n);
	u16 CountCombinations(u8 numFeatures, u8 degree);
	float* CombinationsWithReplacement(float* arr, u8 n, u8 degree, u8 &totalCombinations);
    //// End Feature Vector Generator

	//// Moisture Predictor Models

	void PredictMoistureUsingRegression(void);
	void PredictMoistureUsingNeuralNets(void);

	//// End Moisture Predictor Models


	EOsResult CreateOsResources(void);

	void Run(void) {}

};
//=== end class TTaskCHM ===========================================================

/**********************************************************************************/
#endif

#endif /* SRC_TASKS_TASKAI_HPP_ */
