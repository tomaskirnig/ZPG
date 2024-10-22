#pragma once
#include <vector>
#include "Observer.h"

class Subject {
	virtual void registerObserver(Observer* observer) = 0;
	virtual void notifyObservers(float aspectRatio) = 0;
};