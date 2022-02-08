#include "ValueList.h"

// Constructor 
ValueList::ValueList(int listSize)
{
	maxListSize = listSize;
}

// Default constructor (sets the list legth to 10)
ValueList::ValueList()
{
	maxListSize = 10;
}

// Destructor 
ValueList::~ValueList()
{
	list.clear();
}



void ValueList::AddValue(float value)
{
	list.insert(list.begin(), value);

	avrTotal += value;

	if (list.size() > maxListSize)
	{
		avrTotal -= list.back();
		list.erase(list.end() -1);
	}

	size = list.size();

	if (value > highestValue)
		highestValue = value;

	if (value < lowestValue)
		lowestValue = value;

	avarage = avrTotal / list.size();
}

//std::vector<float> ValueList::GetValues()
//{
//	return List;
//}

// Get a value from a position
float ValueList::GetValue(int position)
{
	return ValueList::list[position];	
}

//float ValueList::GetAvarage()
//{
//	return avrTotal / List.size();
//}

