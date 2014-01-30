#include "StdAfx.h"
#include "ProceduralNoise.h"

using std::vector;


shared_ptr<void> ProceduralNoise::get_data() {
	if (!_texels) {
		_texels.reset(new unsigned char[_width*_height*_depth*_numChannel]);
		vector<float> seeds;
		float dx = 1.0/(float)_seed0;
		for (int i=0; i<_seed0-1; i++) {
			seeds.push_back(dx*(float)i);
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
