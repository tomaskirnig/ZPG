#pragma once
#include <vector>
#include "Observer.h"
#include "Light.h"

class Subject {
	virtual void registerObserver(Observer* observer) = 0;
	virtual void notifyObservers(float aspectRatio, vector<Light>& lights) = 0;
};