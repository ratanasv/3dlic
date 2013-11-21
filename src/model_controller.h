#include "abstract.h"

/** @file Observer.h 
 *	Define interface of Observer and implementation of Subject.
 *	This is a Gang of Four's Observer design pattern.
  */
#pragma once
#include <memory>
#include <list>

using std::list;
using std::weak_ptr;
using std::shared_ptr;

class Observable;

class Observer {
public:
	virtual ~Observer() {};
	virtual void Update(const Observable* const observable) = 0;
protected:
	Observer();
};

class Observable {
private:
	list<weak_ptr<Observer>> i_observers;
protected:
	Observable();
public:
	virtual ~Observable() {};
	virtual void AddObserver(const shared_ptr<Observer>& observer);
	virtual void DeleteObserver(const shared_ptr<Observer>& observer);
	virtual void NotifyObservers() const;
private:
	void DeleteAllStaleObservers();
};

class THREEDLICParameters : public Observable {
private:

};

