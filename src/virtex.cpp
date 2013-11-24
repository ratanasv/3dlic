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

GLenum toGLTexFormat( TextureAbstractFactory::NUM_CHANNEL ch ) {
	switch(ch) {
	case(TextureAbstractFactory::RED) :
		return GL_RED;
	case(TextureAbstractFactory::RGB) :
		return GL_RGB;
	case(TextureAbstractFactory::RGBA) :
		return GL_RGBA;
	default:
		throw invalid_argument("unsupported GL texture format");
	}
}

GLenum toGLTexInternalFormat(TextureAbstractFactory::NUM_CHANNEL ch) {
	switch(ch) {
	case(TextureAbstractFactory::RED) :
		return GL_R8;
	case(TextureAbstractFactory::RGB) :
		return GL_RGB8;
	case(TextureAbstractFactory::RGBA) :
		return GL_RGBA8;
	default:
		throw invalid_argument("unsupported GL texture internal format");
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
	_channel = toGLTexInternalFormat(
		static_cast<TextureAbstractFactory::NUM_CHANNEL>(numChannel));
	_depth = 1;
	_data_channel = GL_RGB;
	_data_type = GL_UNSIGNED_BYTE;
}

void ImageTex2DFactory::flip_vertically() {
	shared_ptr<unsigned char> flipped(new unsigned char[_width*_height*_channel],
		[] (unsigned char* uc){delete[] uc;});
	unsigned char* upside_down = _texels.get();
	for (int i =0; i<_width; i++) {
		for (int j= 0 ;j<_height; j++) {
			for (int k =0; k<_channel; k++) {
				flipped.get()[(_width-1-i)*_height*_channel+j*_channel+k] = 
					(upside_down[_height*i*_channel+j*_channel+k]);
			}
		}
	}
	_texels = flipped;
}

void* ImageTex2DFactory::get_data() {
	return (void*)_texels.get();
}

NoiseTex3DFactory::NoiseTex3DFactory(const string& file_name, 
	TextureAbstractFactory::NUM_CHANNEL ch) : _numChannel(ch) 
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
	_channel = toGLTexInternalFormat(_numChannel);
	_data_channel = toGLTexFormat(_numChannel);

	_data_type = GL_UNSIGNED_BYTE;
	unsigned total = _width*_height*_depth*_numChannel;
	_texels = shared_ptr<unsigned char>(new unsigned char[total], [](unsigned char* f) {
		delete[] f;
	});
	
	fread(_texels.get(), sizeof(unsigned char), total, fp.get());
}


void* NoiseTex3DFactory::get_data() {
	return _texels.get();
}


GLTextureDelegatee::GLTextureDelegatee(const shared_ptr<TextureAbstractFactory>& factory) :
	TextureDelegatee(factory), _which_tex(0) {};

void GLTextureDelegatee::send_to_gpu() {
	GLenum ch = _factory->get_channel();
	int width = _factory->get_width();
	int height = _factory->get_height();
	int depth = _factory->get_depth();
	GLenum data_ch = _factory->get_data_channel();
	GLenum data_type = _factory->get_data_type();
	void* data = _factory->get_data();
	
	if (_factory->get_depth() == 1) {
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
		glTexImage2D(_bind_site, 0, ch, width, height, 0, data_ch, data_type, data);
	}
	else {
		glTexImage3D(_bind_site, 0, ch, width, height, depth, 0, data_ch, data_type, data);
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
