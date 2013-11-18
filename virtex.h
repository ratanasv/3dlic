#pragma once
#include "StdAfx.h"

using std::shared_ptr;

//--------START OF FACTORY----------
class TextureAbstractFactory{
protected:
	int _width;
	int _height;
	int _channel;
	int _depth;
	GLenum _data_channel;
	GLenum _data_type;
public:
	int get_channel(){return _channel;}
	int get_width(){return _width;}
	int get_height(){return _height;}
	int get_depth(){return _depth;}
	GLenum get_data_channel(){return _data_channel;}
	GLenum get_data_type(){return _data_type;}
	virtual void* get_data()=0;

	virtual ~TextureAbstractFactory(){};
};

class ImageTex2DFactory: public TextureAbstractFactory{
private:
	shared_ptr<unsigned char> _texels;
public:
	ImageTex2DFactory(const char* file_name);
private:
	void flip_vertically();
public:
	virtual void* get_data();

};

class NoiseTex3DFactory: public TextureAbstractFactory{
private:
	shared_ptr<unsigned char> _texels;
public:
	NoiseTex3DFactory(const char* file_name);
public:
	virtual void* get_data();
};
//-------END OF FACTORY----------

//-------START OF DELEGATEE--------
class TextureDelegatee{
public:
	virtual ~TextureDelegatee(){};
protected:
	TextureDelegatee(const shared_ptr<TextureAbstractFactory>& factory):
		_factory(factory){};
	shared_ptr<TextureAbstractFactory> _factory;
public:
	virtual void send_to_gpu()=0;
	virtual void pre_render()=0;
	virtual void post_render()=0;
};

class GLTextureDelegatee: public TextureDelegatee{
public:
	GLTextureDelegatee(const shared_ptr<TextureAbstractFactory>& factory);
	virtual void send_to_gpu();
	virtual void pre_render();
	virtual void post_render();
protected:
	shared_ptr<unsigned> _tex_handle;
	const unsigned _which_tex;
	GLenum _bind_site;
};

class GLTexReplaceDelegatee: public GLTextureDelegatee{
public:
	GLTexReplaceDelegatee(const shared_ptr<TextureAbstractFactory>& factory);
	virtual void pre_render();
};
//------END OF DELEGATEE------

//------ACTUAL END-USERS API-------
class VirTex{
public:
	virtual ~VirTex(){};
	VirTex(const shared_ptr<TextureDelegatee>& delegatee):
		_delegatee(delegatee){_delegatee->send_to_gpu();};
protected:
	shared_ptr<TextureDelegatee> _delegatee;
public:
	virtual void pre_render(){
		_delegatee->pre_render();
	}
	virtual void post_render(){
		_delegatee->post_render();
	}
};

//------ACTUAL END-USERS API-------
