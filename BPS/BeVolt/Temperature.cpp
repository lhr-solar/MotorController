// Temperature.cpp
/**
 * Temperature class that holds all temperature related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo
 * @lastRevised 9/3/2018
 */

// NOTE: All units are in Celsius

#include <stdint.h>
#include "Temperature.h"
#include "LTC2983.h"

/** Constructor
 * Creates Temperature instance with NULL pointer to temperature list
 */
Temperature::Temperature(){
	
}


/** Constructor
 * Creates Temperature instance with NULL pointer to temperature list
 * @param max temperature limit of the lithium ion cells
 */
Temperature::Temperature(int ceiling){
	setLimits(ceiling);
}

/** Constructor
 * Creates Temperature instance with NULL pointer to temperature list
 * @param max temperature limit of the lithium ion cells
 */
Temperature::Temperature(int ceiling, int *modules){
	setLimits(ceiling);
	updateMeasurements(modules);
}

/** Destructor
 * Destroys Temperature instance
 */
Temperature::~Temperature(){

}

/** setLimits
 * Sets the max temperature limit the cells can reach before danger
 * @param max temperature limit
 */
void Temperature::setLimits(int ceiling){
	maxTemperatureLimit = ceiling;
}

/** updateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new temperature measurements
 * @return 1 if successfully stored, 0 if failed
 */
uint8_t Temperature::updateMeasurements(){
	modules = LTC2983_Measure();

	if(sizeof(modules)/sizeof(uint16_t) == NUM_MODULES){
		return 1;
	}else{
		return 0;
	}
}

/** isSafe
 * Checks if all modules are safe
 * @return 1 if pack is safe, 0 if in danger
 */
uint8_t Temperature::isSafe(void){
	for(int i = 0; i < sizeof(modules)/sizeof(uint16_t); ++i){
		if(moduleTemperature(i) > maxTemperatureLimit){
			return 0;
		}
	}

	return 1;
}

/** modulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
int *Temperature::modulesInDanger(void){
	int checks[NUM_MODULES];
	for(int i = 0; i < NUM_MODULES; ++i){
		if(moduleTemperature(i) > maxTemperatureLimit){
			checks[i] = 1;	// 1 shows that the unit is in danger
		}else{
			checks[i] = 0;	// 0 shows that the unit is not in danger
		}
	}

	int sum = 0;
	for(int i = 0; i < NUM_MODULES; ++i){
		sum += checks[i];
	}

	int endangeredModules[sum];
	int j = 0;
	for(int i = 0; i < NUM_MODULES; ++i){
		if(checks[i]){
			endangeredModules[j] = i;
			++j;
		}
	}

	return endangeredModules;
}

/** moduleTemperature
 * Gets the temperature of a certain module in the battery pack
 * @param index of module
 * @return temperature of module at specified index
 */
int Voltage::moduleTemperature(int moduleIdx){
	return modules[moduleIdx];
}

/** totalPackAvgTemperature
 * Gets the average temperature of the whole battery pack
 * @return average temperature of battery pack
 */
int Voltage::totalPackAvgTemperature(void){
	int sum = 0;
	for(int i = 0; i < sizeof(modules)/sizeof(uint16_t); ++i){
		sum += moduleTemperature(i);
	}
	return (sum / (sizeof(modules)/sizeof(uint16_t)));
}
