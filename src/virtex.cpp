#include "StdAfx.h"
#include "virtex.h"
#include <boost/thread/locks.hpp>

using namespace vir;
using std::shared_ptr;
using std::invalid_argument;
using namespace std;
using std::runtime_error;
namespace fs = boost::filesystem;

static void validatePath(const string& fileName) {
	const fs::path path(fileName);
	if (!fs::exists(fileName)) {
		throw invalid_argument(fs::current_path().string() + fileName + 
			"doesn't exist");
	}
}


GLenum toGLTexInternalFormat(const int ch) {
	switch(ch) {
	case(1) :
		return GL_RED;
	case(3) :
		return GL_RGB;
	case(4) :
		return GL_RGBA;
	default:
		throw invalid_argument("unsupported GL texture internal format");
	}
}


GLenum toGLTexFormat(const int ch) {
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

Texture2DData::Texture2DData(const string& file_name) {
	validatePath(file_name);
	const int numChannel = 3;
	int actualNumChannel;
	unsigned char* tex_ptr = SOIL_load_image(file_name.c_str(),
		&_width, &_height, &actualNumChannel, SOIL_LOAD_RGB);
	if (tex_ptr == NULL) {
		throw runtime_error("SOIL_load_image failed");
	}
	if (numChannel != actualNumChannel) {
		fprintf(stderr, "Imagefile %s : requested RGB but found %d\n", file_name.c_str(), 
			actualNumChannel);
	}
	_texels = initCStyleArray(tex_ptr);
	if (_height != 1) {
		flip_vertically();
	}
	_internalFormat = toGLTexInternalFormat(numChannel);
	_depth = 1;
	_format = GL_RGB;
	_type = GL_UNSIGNED_BYTE;
}

void Texture2DData::flip_vertically() {
	const int numChannel = 3;
	auto flipped = initCStyleArray(new unsigned char[_width*_height*numChannel]);
	unsigned char* upside_down = _texels.get();
	for (int i=0; i<_height; i++) {
		for (int j=0; j<_width; j++) {
			for (int k =0; k<numChannel; k++) {
				flipped.get()[(_height-1-i)*_width*numChannel+j*numChannel+k] = 
					(upside_down[_width*i*numChannel+j*numChannel+k]);
			}
		}
	}
	_texels = flipped;
}

shared_ptr<const void> Texture2DData::get_data() {
	return _texels;
}

GLenum Texture2DData::getInternalFormat() {
	return _internalFormat;
}

int Texture2DData::getWidth() {
	return _width;
}

int Texture2DData::getHeight() {
	return _height;
}

int Texture2DData::getDepth() {
	return _depth;
}

GLenum Texture2DData::getFormat() {
	return _format;
}

GLenum Texture2DData::getType() {
	return _type;
}



atomic_uint GLTexture::OGLActiveTextureCounter;

GLTexture::GLTexture(const shared_ptr<TextureData>& factory) :
	_which_tex(OGLActiveTextureCounter++) 
{
	send_to_gpu(factory);
};

void GLTexture::send_to_gpu(const shared_ptr<TextureData>& factory) {
	std::lock_guard<boost::shared_mutex> synchronous(_mutex);
	GLenum ch = factory->getInternalFormat();
	int width = factory->getWidth();
	int height = factory->getHeight();
	int depth = factory->getDepth();
	GLenum data_ch = factory->getFormat();
	GLenum data_type = factory->getType();
	

	if (factory->getHeight() ==1 ) {
		_bind_site = GL_TEXTURE_1D;
	} else if (factory->getDepth() == 1) {
		_bind_site = GL_TEXTURE_2D;
	} else {
		_bind_site = GL_TEXTURE_3D;
	}

	_tex_handle = shared_ptr<unsigned>(new unsigned[1], [](unsigned* u) { 
		glDeleteTextures(1, u);
		delete[] u; 
	});

	glActiveTexture(GL_TEXTURE0 + _which_tex);
	glGenTextures(1, _tex_handle.get());
	glBindTexture(_bind_site, *_tex_handle);

	if (_bind_site == GL_TEXTURE_1D) {
		glTexImage1D(_bind_site, 0, ch, width, 0, data_ch, data_type, 
			factory->get_data().get());
	} else if (_bind_site == GL_TEXTURE_2D) {
		glTexImage2D(_bind_site, 0, ch, width, height, 0, data_ch, data_type,
			factory->get_data().get());
	} else {
		glTexImage3D(_bind_site, 0, ch, width, height, depth, 0, data_ch, data_type, 
			factory->get_data().get());
	}
	glBindTexture(_bind_site, 0);
}

void GLTexture::pre_render(const shared_ptr<TextureVisitor>& visitor) {
	boost::shared_lock<boost::shared_mutex> synchronous(_mutex);
	glActiveTexture(GL_TEXTURE0+_which_tex);
	glBindTexture(_bind_site, *_tex_handle);
	visitor->PreRender(_which_tex, _bind_site, _tex_handle);
}

void GLTexture::post_render() {
	boost::shared_lock<boost::shared_mutex> synchronous(_mutex);
	glActiveTexture(GL_TEXTURE0+_which_tex);
	glBindTexture(_bind_site, 0);
}
