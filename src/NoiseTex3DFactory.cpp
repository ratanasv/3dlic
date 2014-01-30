#include "StdAfx.h"
#include "NoiseTex3DFactory.h"

using std::invalid_argument;

static GLenum toGLTexFormat(const int ch) {
	switch(ch) {
	case(1) :
		return GL_RED;
	case(3) :
		return GL_RGB;
	case(4) :
		return GL_RGBA;
	default:
		throw invalid_argument("unsupported GL texture format");
	}
}

NoiseTex3DFactory::NoiseTex3DFactory(const string& file_name, const int ch) : 
	_numChannel(ch), _internalFormat(toGLTexInternalFormat(ch)), 
	_format(toGLTexFormat(ch)), _type(GL_UNSIGNED_BYTE)
{
	shared_ptr<FILE> fp(fopen(file_name.c_str(), "rb"), [](FILE* f) {
		fclose(f);
	});
	if (fp.get() == NULL) {
		MessageBox( NULL,
			"Vir says: Cannot locate noise3d texture.",
			"Error", MB_ICONSTOP);
		exit(EXIT_FAILURE);
	}
	fread(&_width, sizeof(int), 1, fp.get());
	fread(&_height, sizeof(int), 1, fp.get());
	fread(&_depth, sizeof(int), 1, fp.get());

	const unsigned total = _width*_height*_depth*_numChannel;
	_texels = shared_ptr<unsigned char>(new unsigned char[total], 
		[](unsigned char* f) {
			delete[] f;
	}
	);

	fread(_texels.get(), sizeof(unsigned char), total, fp.get());
}


shared_ptr<void> NoiseTex3DFactory::get_data() {
	return _texels;
}

GLenum NoiseTex3DFactory::getInternalFormat() {
	return _internalFormat;
}

int NoiseTex3DFactory::getWidth() {
	return _width;
}

int NoiseTex3DFactory::getHeight() {
	return _height;
}

int NoiseTex3DFactory::getDepth() {
	return _depth;
}

GLenum NoiseTex3DFactory::getFormat() {
	return _format;
}

GLenum NoiseTex3DFactory::getType() {
	return _type;
}