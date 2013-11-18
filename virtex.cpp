#include "StdAfx.h"
#include "virtex.h"

using namespace std;
using namespace vir;

ImageTex2DFactory::ImageTex2DFactory(const char* file_name){
	unsigned char* tex_ptr = SOIL_load_image(file_name,
		&_width,&_height,&_channel,SOIL_LOAD_RGB);
	assert(tex_ptr != NULL);
	_texels = shared_ptr<unsigned char>( tex_ptr, 
		[] (unsigned char* uc){delete[] uc;});
	flip_vertically();
	_depth = 1;
	_data_channel = GL_RGB;
	_data_type = GL_UNSIGNED_BYTE;
}

void ImageTex2DFactory::flip_vertically(){
	shared_ptr<unsigned char> flipped(new unsigned char[_width*_height*_channel],
		[] (unsigned char* uc){delete[] uc;});
	unsigned char* upside_down = _texels.get();
	for(int i =0; i<_width; i++){
		for(int j= 0 ;j<_height; j++){
			for(int k =0; k<_channel; k++){
				flipped.get()[(_width-1-i)*_height*_channel+j*_channel+k] = 
					(upside_down[_height*i*_channel+j*_channel+k]);
			}
		}
	}
	_texels = flipped;
}

void* ImageTex2DFactory::get_data(){
	return (void*)_texels.get();
}

NoiseTex3DFactory::NoiseTex3DFactory( const char* file_name){
	FILE* fp = fopen(file_name, "rb");
	if(!fp){
		MessageBox( NULL,
			"Vir says: Cannot locate noise3d texture.",
			"Error", MB_ICONSTOP);
		exit(EXIT_FAILURE);
	}
	fread(&_width, sizeof(int), 1, fp);
	fread(&_height, sizeof(int), 1, fp);
	fread(&_depth, sizeof(int), 1, fp);
	_channel = 4;
	_data_channel = GL_RGBA;
	_data_type = GL_UNSIGNED_BYTE;
	unsigned total = _width*_height*_depth*_channel;
	unsigned char* data = new unsigned char[total];
	
	fread(data, sizeof(unsigned char), total, fp);
	memset(data, 240, sizeof(unsigned char)*total);
	fclose(fp);
 	_texels = shared_ptr<unsigned char>(data, 
 		[](unsigned char* f){delete[] f;});
}


void* NoiseTex3DFactory::get_data(){
	return _texels.get();
}


GLTextureDelegatee::GLTextureDelegatee( const shared_ptr<TextureAbstractFactory>& factory ):
TextureDelegatee(factory), _which_tex(0), _tex_handle(NULL){};

void GLTextureDelegatee::send_to_gpu(){
	int ch = _factory->get_channel();
	int width = _factory->get_width();
	int height = _factory->get_height();
	int depth = _factory->get_depth();
	GLenum data_ch = _factory->get_data_channel();
	GLenum data_type = _factory->get_data_type();
	void* data = _factory->get_data();
	
	if(_factory->get_depth() == 1)
		_bind_site = GL_TEXTURE_2D;
	else
		_bind_site = GL_TEXTURE_3D;

	_tex_handle = shared_ptr<unsigned>(new unsigned[1], [](unsigned* u){ 
		delete[] u; glDeleteTextures(1, u);});

	glActiveTexture(GL_TEXTURE0+_which_tex);
	glEnable(_bind_site);
	glGenTextures(1, _tex_handle.get());
	glBindTexture(_bind_site, *_tex_handle);
	if(_bind_site == GL_TEXTURE_2D)
		glTexImage2D(_bind_site, 0, ch, width, height, 0, data_ch, data_type, data);
	else
		glTexImage3D(_bind_site, 0, ch, width, height, depth, 0, data_ch, data_type, data);
	glBindTexture(_bind_site, 0);
}

void GLTextureDelegatee::pre_render(){
	glActiveTexture(GL_TEXTURE0+_which_tex);
	glEnable(_bind_site);
	glBindTexture(_bind_site, *_tex_handle);
}

void GLTextureDelegatee::post_render(){
	glActiveTexture(GL_TEXTURE0+_which_tex);
	glEnable(_bind_site);
	glBindTexture(_bind_site, 0);
}


GLTexReplaceDelegatee::GLTexReplaceDelegatee( const shared_ptr<TextureAbstractFactory>& factory ):
	GLTextureDelegatee(factory){};

void GLTexReplaceDelegatee::pre_render()
{
	glActiveTexture(GL_TEXTURE0+_which_tex);
	glEnable(_bind_site);
	glBindTexture(_bind_site, *_tex_handle);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); 
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}