#pragma once
#include <vector>
#include "IObserver.h"
#include "Light.h"

class ISubject {
	virtual void registerObserver(IObserver* observer) = 0;
	virtual void notifyObservers(float aspectRatio, vector<Light>& lights) = 0;
};