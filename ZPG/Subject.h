#pragma once
#include <vector>
#include "Observer.h"

class Subject {
protected:
    std::vector<Observer*> observers; // List of observers

public:
    virtual ~Subject() {}

    // Register an observer
    void registerObserver(Observer* observer) {
        observers.push_back(observer);
    }

    // Deregister an observer
    void deregisterObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    // Notify all observers
    void notifyObservers(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
        for (Observer* observer : observers) {
            observer->update(viewMatrix, projectionMatrix);
        }
    }
};
