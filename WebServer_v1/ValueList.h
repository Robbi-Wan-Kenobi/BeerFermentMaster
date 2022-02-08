#pragma once

#include <ArduinoSTL.h>
//#include <string>
//#include <vector>
//#include <iterator>



class ValueList
{
private:
	
	int maxListSize = 0;
	
	float avrTotal = 0;
	std::vector<float> list;

public:
	// Current size of the list
	unsigned int size;
	// The running avarage of all the items in the list
	float avarage;
	//The highest value in the list
	float lowestValue = 1000;
	//The lowest value in the list
	float highestValue = -1000;

	// Default constructor (sets the list legth to 10)
	ValueList();

	// Constructor
	ValueList(int listSize);

	// Destructor 
	~ValueList();

	// Adds a new value to the list
	void AddValue(float value);
	
	// Get a value from a position
	float GetValue(int position);

};
