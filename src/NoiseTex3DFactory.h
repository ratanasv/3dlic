#pragma once
#include "virtex.h"

class NoiseTex3DFactory: public TextureAbstractFactory {
private:
	shared_ptr<unsigned char> _texels;
	const int _numChannel;
	int _width;
	int _height;
	const GLenum _internalFormat;
	const GLenum _format;
	const GLenum _type;
	int _depth;
public:
	NoiseTex3DFactory(const string& file_name, const int ch);
public:
	virtual shared_ptr<void> get_data();
	virtual GLenum getInternalFormat();
	virtual int getWidth();
	virtual int getHeight();
	virtual int getDepth();
	virtual GLenum getFormat();
	virtual GLenum getType();
};