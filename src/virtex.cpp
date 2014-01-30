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

ImageTex2DFactory::ImageTex2DFactory(const string& file_name) {
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

void ImageTex2DFactory::flip_vertically() {
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

shared_ptr<void> ImageTex2DFactory::get_data() {
	return _texels;
}



atomic_uint GLTextureDelegatee::OGLActiveTextureCounter;

GLTextureDelegatee::GLTextureDelegatee(const shared_ptr<TextureAbstractFactory>& factory) :
	TextureDelegatee(factory), _which_tex(OGLActiveTextureCounter++) {};

void GLTextureDelegatee::send_to_gpu() {
	GLenum ch = _factory->getInternalFormat();
	int width = _factory->getWidth();
	int height = _factory->getHeight();
	int depth = _factory->getDepth();
	GLenum data_ch = _factory->getFormat();
	GLenum data_type = _factory->getType();
	
	if (_factory->getDepth() == 1) {
		_bind_site = GL_TEXTURE_2D;
	}
	else {
		_bind_site = GL_TEXTURE_3D;
	}

	_tex_handle = shared_ptr<unsigned>(new unsigned[1], [](unsigned* u) { 
		delete[] u; glDeleteTextures(1, u);
	});

	glActiveTexture(GL_TEXTURE0 + _which_tex);
	glGenTextures(1, _tex_handle.get());
	glBindTexture(_bind_site, *_tex_handle);
	if (_bind_site == GL_TEXTURE_2D) {
		glTexImage2D(_bind_site, 0, ch, width, height, 0, data_ch, data_type,
			_factory->get_data().get());
	}
	else {
		glTexImage3D(_bind_site, 0, ch, width, height, depth, 0, data_ch, data_type, 
			_factory->get_data().get());
	}
	glBindTexture(_bind_site, 0);
}

void GLTextureDelegatee::pre_render(const shared_ptr<TextureVisitor>& visitor) {
	glActiveTexture(GL_TEXTURE0+_which_tex);
	glBindTexture(_bind_site, *_tex_handle);
	visitor->pre_render(_which_tex, _bind_site, _tex_handle);
}

void GLTextureDelegatee::post_render() {
	glActiveTexture(GL_TEXTURE0+_which_tex);
	glBindTexture(_bind_site, 0);
}


void VirTex::pre_render( const shared_ptr<TextureVisitor>& visitor ) {
	_delegatee->pre_render(visitor);
}

void VirTex::post_render() {
	_delegatee->post_render();
}
