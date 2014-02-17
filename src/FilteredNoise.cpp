#include "StdAfx.h"
#include "FilteredNoise.h"
#include <vector>

using namespace std;

shared_ptr<void> FilteredNoise::get_data() {
	const int total = _depth*_width*_height;
	shared_ptr<void> data = ProceduralNoise::get_data();
	shared_ptr<void> result(new unsigned char[total]);
	memset(result.get(), 0, total*sizeof(unsigned char));
	unsigned char* rawData = (unsigned char*)data.get();
	unsigned char* rawResult = (unsigned char*)result.get();

	int counter = 0;
	for (int i=0; i<_depth; i++) {
		for (int j=0; j<_height; j++) {
			for (int k=0; k<_width; k++) {
				if (rawData[counter] == (unsigned char)255) {
					ApplyFilter(rawResult, i, j, k);
				}
				counter++;
			}
		}
	}
	return result;
}

FilteredNoise::FilteredNoise(int s0, int s1, int s2, int dim, int density, float sigma) : 
	ProceduralNoise(s0, s1, s2, dim, density)
{
	int size = 0;
	const float eps = 0.01;
	const float factor = 1.0/pow(2.0*3.14*sigma, 2);

	while (true) {
		const float x2 = pow(DtoC(size), 2);
		const float falloff = exp(-1.0*(x2 + x2 + x2)/(2.0*sigma));
		if (falloff < eps) {
			break;
		} else {
			size++;
		}
	}

	_center = size-1;
	_filterSize = size*2-1;

	_filter = new float[_filterSize*_filterSize*_filterSize];
	for (int i=0; i<_filterSize; i++) {
		for (int j=0; j<_filterSize; j++) {
			for (int k=0; k<_filterSize; k++) {
				const float x2 = pow(DtoC(_center - k), 2);
				const float y2 = pow(DtoC(_center - j), 2);
				const float z2 = pow(DtoC(_center - i), 2);
				const float falloff = exp(-1.0*(x2 + y2 + z2)/(2.0*sigma));
				_filter[i*_filterSize*_filterSize + j*_filterSize + k] = 255.0*falloff;
			}
		}
	}
}

float FilteredNoise::DtoC(int dist) {
	static float size = (float)_width;
	static float fivePercent = 0.05*size;
	static float factor = 6.0/fivePercent;
	return ((float)dist)*factor;
}

int FilteredNoise::GetIndex(int i, int j, int k) {
	return i*_height*_width + j*_width + k;
}

void FilteredNoise::ApplyFilter(unsigned char* p, int i, int j, int k) {
	for (int a=0; a<_filterSize; a++) {
		for (int b=0; b<_filterSize; b++) {
			for (int c=0; c<_filterSize; c++) {
				const int ai = i + (_center - a);
				const int bj = j + (_center - b);
				const int ck = k + (_center - c);
				if (ai >= 0 && ai < _depth &&
					bj >= 0 && bj < _height &&
					ck >= 0 && ck < _width) 
				{
					const int index = GetIndex(ai, bj, ck);
					unsigned char prev = p[index];
					unsigned char next = (unsigned char)(_filter[
						a*_filterSize*_filterSize + b*_filterSize + c]);
					p[index] = max(prev, next);
				}
			}
		}
	}
}

