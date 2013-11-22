#pragma once
#include "vec.h"
#include <memory>
#include <list>

using std::list;
using std::weak_ptr;
using std::shared_ptr;
using Angel::vec3;

class Observable;

class Observer {
public:
	virtual ~Observer() {};
	virtual void Update(const Observable* const observable) = 0;
protected:
	Observer() {};
};

class Observable {
private:
	list<weak_ptr<Observer>> i_observers;
protected:
	Observable() {};
public:
	virtual ~Observable() {};
	virtual void AddObserver(const shared_ptr<Observer>& observer);
	virtual void DeleteObserver(const shared_ptr<Observer>& observer);
	virtual void NotifyObservers() const;
private:
	void DeleteAllStaleObservers();
};
