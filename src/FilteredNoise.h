#pragma once
#include "ProceduralNoise.h"

class FilteredNoise : public ProceduralNoise {
public:
	FilteredNoise(int s0, int s1, int s2, int dim, int density, float sigma);
	virtual shared_ptr<void> get_data();
private:
	float DtoC(int dist);
	int GetIndex(int i, int j, int k);
	void ApplyFilter(unsigned char* p, int i, int j, int k);
	float* _filter;
	int _filterSize;
	int _center;
};