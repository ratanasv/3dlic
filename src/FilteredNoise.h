#pragma once
#include "ProceduralNoise.h"
#include <thread>
#include <mutex>

class FilteredNoise : public ProceduralNoise {
public:
	FilteredNoise(int s0, int s1, int s2, int dim, int density, float sigma);
	virtual shared_ptr<const void> get_data();
private:
	float DtoC(int dist);
	int GetIndex(int i, int j, int k);
	void ApplyFilter(unsigned char* p, int i, int j, int k);
	void initData();
	float* _filter;
	int _filterSize;
	int _center;
	std::shared_ptr<void> _data;
	std::once_flag _initFlag;
};