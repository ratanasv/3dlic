#pragma once
#include <memory>
#include <vector>

template <class T> std::shared_ptr<std::vector<T>> initSmartArray(const int size = 0) {
	std::shared_ptr<vector<T>> returned(new std::vector<T>(size));
	return returned;
}

template <class T> std::shared_ptr<T> initCStyleArray(T* dumb) {
	return std::shared_ptr<T>(dumb, [](T* ptr) {
		delete[] ptr;
	});
}