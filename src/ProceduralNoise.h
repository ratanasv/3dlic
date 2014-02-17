#pragma once
#include "virtex.h"

class ProceduralNoise : public TextureData {
protected:
	shared_ptr<unsigned char> _texels;
	const int _numChannel;
	const int _width;
	const int _height;
	const GLenum _internalFormat;
	const GLenum _format;
	const GLenum _type;
	const int _depth;
	const int _seed0;
	const int _seed1;
	const int _seed2;
	const int _density;
public:
	ProceduralNoise(int s0, int s1, int s2, int dim, int density);

	virtual GLenum getInternalFormat();

	virtual int getWidth();

	virtual int getHeight();

	virtual int getDepth();

	virtual GLenum getFormat();

	virtual GLenum getType();

	virtual shared_ptr<const void> get_data();
};