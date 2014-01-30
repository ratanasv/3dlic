#pragma once
#include "virtex.h"

class ProceduralNoise : public TextureAbstractFactory {
private:
	shared_ptr<unsigned char> _texels;
	const int _numChannel;
	int _width;
	int _height;
	const GLenum _internalFormat;
	const GLenum _format;
	const GLenum _type;
	int _depth;
	int _seed0;
	int _seed1;
	int _seed2;
	int _density;
public:
	ProceduralNoise();

	virtual GLenum getInternalFormat();

	virtual int getWidth();

	virtual int getHeight();

	virtual int getDepth();

	virtual GLenum getFormat();

	virtual GLenum getType();

	virtual shared_ptr<void> get_data();

	ProceduralNoise& WithSeed0(int);
	ProceduralNoise& WithSeed1(int);
	ProceduralNoise& WithSeed2(int);
	ProceduralNoise& WithDimension(int);
	ProceduralNoise& WithDensity(int);

};