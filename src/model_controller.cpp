#include "StdAfx.h"
#include "model_controller.h"
#include <algorithm>

using std::find;

template <class T> class WeakSharedComparison {
private:
	const shared_ptr<T> _shared;
public:
	WeakSharedComparison(const shared_ptr<T>& shared) : _shared(shared) {};
	bool operator() (const weak_ptr<T>& weak) const {
		return weak.lock() == _shared;
	}
};

void Observable::AddObserver( const shared_ptr<Observer>& observer ) {
	WeakSharedComparison<Observer> isSame(observer);
	auto it = find_if(i_observers.begin(), i_observers.end(), isSame);
	if (it == i_observers.end()) {
		i_observers.push_back(weak_ptr<Observer>(observer));
	}
}

void Observable::DeleteObserver(const shared_ptr<Observer>& observer) {
	WeakSharedComparison<Observer> isSame(observer);
	i_observers.remove_if(isSame);
}

void Observable::NotifyObservers() const {
	for (auto weak : i_observers) {
		auto sharedIt = weak.lock();
		if (sharedIt) {
			sharedIt->Update(this);
		} else {
			
		}
	}
}

void Observable::DeleteAllStaleObservers() {
	DeleteObserver(shared_ptr<Observer>());
}
