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

class ImageTex2DFactoryImpl: public TextureAbstractFactory{
private:
	std::shared_ptr<unsigned char> _texels;
public:
	ImageTex2DFactoryImpl(const char* file_name);
private:
	void flip_vertically();
public:
	virtual void* get_data();

};
typedef std::shared_ptr<ImageTex2DFactoryImpl> ImageTex2DFactory;

class NoiseTex3DFactoryImpl: public TextureAbstractFactory{
private:
	std::shared_ptr<unsigned char> _texels;
public:
	NoiseTex3DFactoryImpl(const char* file_name);
public:
	virtual void* get_data();
};
typedef std::shared_ptr<NoiseTex3DFactoryImpl> NoiseTex3DFactory;
//-------END OF FACTORY----------

//-------START OF DELEGATEE--------
class TextureDelegateeImpl{
public:
	virtual ~TextureDelegateeImpl(){};
protected:
	TextureDelegateeImpl(const shared_ptr<TextureAbstractFactory>& factory):
		_factory(factory){};
	shared_ptr<TextureAbstractFactory> _factory;
public:
	virtual void send_to_gpu()=0;
	virtual void pre_render()=0;
	virtual void post_render()=0;
};
typedef std::shared_ptr<TextureDelegateeImpl> TextureDelegatee;

class GLTextureDelegateeImpl: public TextureDelegateeImpl{
public:
	GLTextureDelegateeImpl(const shared_ptr<TextureAbstractFactory>& factory);
	virtual void send_to_gpu();
	virtual void pre_render();
	virtual void post_render();
protected:
	std::shared_ptr<unsigned> _tex_handle;
	const unsigned _which_tex;
	GLenum _bind_site;
};
typedef std::shared_ptr<GLTextureDelegateeImpl> GLTextureDelegatee;

class GLTexReplaceDelegateeImpl: public GLTextureDelegateeImpl{
public:
	GLTexReplaceDelegateeImpl(const shared_ptr<TextureAbstractFactory>& factory);
	virtual void pre_render();
};
typedef std::shared_ptr<GLTexReplaceDelegateeImpl> GLTexReplaceDelegatee;
//------END OF DELEGATEE------

//------ACTUAL END-USERS API-------
class VirTexImpl{
public:
	virtual ~VirTexImpl(){};
	VirTexImpl(const TextureDelegatee& delegatee):
		_delegatee(delegatee){_delegatee->send_to_gpu();};
protected:
	TextureDelegatee _delegatee;
public:
	virtual void pre_render(){
		_delegatee->pre_render();
	}
	virtual void post_render(){
		_delegatee->post_render();
	}
};
typedef std::shared_ptr<VirTexImpl> VirTex;

//------ACTUAL END-USERS API-------
