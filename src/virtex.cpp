#include "StdAfx.h"
#include "virtex.h"

using namespace vir;
using std::shared_ptr;
using std::invalid_argument;

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
	int numChannel;
	unsigned char* tex_ptr = SOIL_load_image(file_name.c_str(),
		&_width, &_height, &numChannel, SOIL_LOAD_RGB);
	if (tex_ptr == NULL) {
		throw runtime_error("SOIL_load_image failed");
	}
	_texels = shared_ptr<unsigned char>( tex_ptr, [](unsigned char* uc){
		delete[] uc;
	});
	flip_vertically();
	_internalFormat = toGLTexInternalFormat(numChannel);
	_depth = 1;
	_format = GL_RGB;
	_type = GL_UNSIGNED_BYTE;
}

void Texture2DData::flip_vertically() {
	shared_ptr<unsigned char> flipped(new unsigned char[_width*_height*_internalFormat],
		[] (unsigned char* uc){delete[] uc;});
	unsigned char* upside_down = _texels.get();
	for (int i =0; i<_width; i++) {
		for (int j= 0 ;j<_height; j++) {
			for (int k =0; k<_internalFormat; k++) {
				flipped.get()[(_width-1-i)*_height*_internalFormat+j*_internalFormat+k] = 
					(upside_down[_height*i*_internalFormat+j*_internalFormat+k]);
			}
		}
	}
	_texels = flipped;
}

shared_ptr<void> Texture2DData::get_data() {
	return _texels;
}



atomic_uint GLTexture::OGLActiveTextureCounter;

GLTexture::GLTexture() :
	_which_tex(OGLActiveTextureCounter++) {};

void GLTexture::send_to_gpu(const shared_ptr<TextureData>& factory) {
	GLenum ch = factory->getInternalFormat();
	int width = factory->getWidth();
	int height = factory->getHeight();
	int depth = factory->getDepth();
	GLenum data_ch = factory->getFormat();
	GLenum data_type = factory->getType();
	
	if (factory->getDepth() == 1) {
		_bind_site = GL_TEXTURE_2D;
	}
	else {
		_bind_site = GL_TEXTURE_3D;
	}

	_tex_handle = shared_ptr<unsigned>(new unsigned[1], [](unsigned* u) { 
		glDeleteTextures(1, u);
		delete[] u; 
	});

	glActiveTexture(GL_TEXTURE0 + _which_tex);
	glGenTextures(1, _tex_handle.get());
	glBindTexture(_bind_site, *_tex_handle);
	if (_bind_site == GL_TEXTURE_2D) {
		glTexImage2D(_bind_site, 0, ch, width, height, 0, data_ch, data_type,
			factory->get_data().get());
	}
	else {
		glTexImage3D(_bind_site, 0, ch, width, height, depth, 0, data_ch, data_type, 
			factory->get_data().get());
	}
	glBindTexture(_bind_site, 0);
}

void GLTexture::pre_render(const shared_ptr<TextureVisitor>& visitor) {
	glActiveTexture(GL_TEXTURE0+_which_tex);
	glBindTexture(_bind_site, *_tex_handle);
	visitor->PreRender(_which_tex, _bind_site, _tex_handle);
}

void GLTexture::post_render() {
	glActiveTexture(GL_TEXTURE0+_which_tex);
	glBindTexture(_bind_site, 0);
}
