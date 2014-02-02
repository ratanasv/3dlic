#include "StdAfx.h"
#include "halton.hpp"
#include "ProceduralNoise.h"

using std::vector;


shared_ptr<void> ProceduralNoise::get_data() {
	if (!_texels) {
		_texels.reset(new unsigned char[_width*_height*_depth]);
		int seeds[3] = {0, 0, 0};
		int bases[3] = {_seed0, _seed1, _seed2};
		int jumps[3] = {1, 1, 1};
		double* result = new double[3*_density];
		i4_to_halton_sequence(3, _density, 0, seeds, jumps, bases, result);
		for (int i=0; i<_density; i++) {
			int xi = (int)(result[3*i+0]*_width);
			int yi = (int)(result[3*i+1]*_width);
			int zi = (int)(result[3*i+2]*_width);

			unsigned char* rawPtr = _texels.get();
			rawPtr[zi*(_width*_height) + yi*(_width) + xi] = (unsigned char)255;
		}
	}
	return _texels;
}

GLenum ProceduralNoise::getInternalFormat() {
	return _internalFormat;
}

int ProceduralNoise::getWidth() {
	return _width;
}

int ProceduralNoise::getHeight() {
	return _height;
}

int ProceduralNoise::getDepth() {
	return _depth;
}

GLenum ProceduralNoise::getFormat() {
	return _format;
}

GLenum ProceduralNoise::getType() {
	return _type;
}

ProceduralNoise::ProceduralNoise() : 
	_internalFormat(toGLTexInternalFormat(1)), _numChannel(1),
	_format(toGLTexFormat(1)), _type(GL_UNSIGNED_BYTE)
{
	
}

ProceduralNoise& ProceduralNoise::WithSeed0(int i) {
	_seed0 = i;
	return *this;
}

ProceduralNoise& ProceduralNoise::WithSeed1(int i) {
	_seed1 = i;
	return *this;
}

ProceduralNoise& ProceduralNoise::WithSeed2(int i) {
	_seed2 = i;
	return *this;
}

ProceduralNoise& ProceduralNoise::WithDimension(int i) {
	_width = i;
	_height = i;
	_depth = i;
	return *this;
}

ProceduralNoise& ProceduralNoise::WithDensity(int i) {
	_density = i;
	return *this;
}
