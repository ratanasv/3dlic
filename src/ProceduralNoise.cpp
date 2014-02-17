#include "StdAfx.h"
#include "halton.hpp"
#include "ProceduralNoise.h"

using std::vector;


shared_ptr<const void> ProceduralNoise::get_data() {
	if (!_texels) {
		_texels.reset(new unsigned char[_width*_height*_depth]);
		memset(_texels.get(), 0, _width*_height*_depth*sizeof(unsigned char));
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

ProceduralNoise::ProceduralNoise(int s0, int s1, int s2, int dim, int density) : 
	_internalFormat(toGLTexInternalFormat(1)), _numChannel(1),
	_format(toGLTexFormat(1)), _type(GL_UNSIGNED_BYTE),
	_seed0(s0), _seed1(s1), _seed2(s2), _depth(dim), _width(dim), _height(dim), 
	_density(density)
{
	
}