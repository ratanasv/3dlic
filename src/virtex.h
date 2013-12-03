#pragma once
#include <memory>
#include <cstring>
#include "glew.h"
#include <atomic>

using std::shared_ptr;
using std::string;
using std::atomic_uint;

class TextureVisitor {
public:
	virtual ~TextureVisitor() {};
	virtual void pre_render(const unsigned int whichTex, GLenum bindSite, 
		const shared_ptr<unsigned> texHandle) = 0;
};


//--------START OF FACTORY----------
class TextureAbstractFactory {

public:
	virtual GLenum getInternalFormat() = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual int getDepth() = 0;
	virtual GLenum getFormat() = 0;
	virtual GLenum getType() = 0;
	virtual void* get_data() = 0;

	virtual ~TextureAbstractFactory() {};
};

class ImageTex2DFactory: public TextureAbstractFactory {
private:
	shared_ptr<unsigned char> _texels;
	int _width;
	int _height;
	GLenum _internalFormat;
	int _depth;
	GLenum _format;
	GLenum _type;
public:
	ImageTex2DFactory(const string& file_name);
private:
	void flip_vertically();
public:
	virtual void* get_data();

};

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
	virtual void* get_data();
	virtual GLenum getInternalFormat();
	virtual int getWidth();
	virtual int getHeight();
	virtual int getDepth();
	virtual GLenum getFormat();
	virtual GLenum getType();
};
//-------END OF FACTORY----------

//-------START OF DELEGATEE--------
class TextureDelegatee {
public:
	virtual ~TextureDelegatee() {};
protected:
	TextureDelegatee(const shared_ptr<TextureAbstractFactory>& factory) :
		_factory(factory) {};
	shared_ptr<TextureAbstractFactory> _factory;
public:
	virtual void send_to_gpu() = 0;
	virtual void pre_render(const shared_ptr<TextureVisitor>& visitor) = 0;
	virtual void post_render() = 0;
};

class GLTextureDelegatee: public TextureDelegatee {
public:
	GLTextureDelegatee(const shared_ptr<TextureAbstractFactory>& factory);
	virtual void send_to_gpu();
	virtual void pre_render(const shared_ptr<TextureVisitor>& visitor);
	virtual void post_render();
protected:
	shared_ptr<unsigned> _tex_handle;
	const unsigned _which_tex;
	GLenum _bind_site;
private:
	static atomic_uint OGLActiveTextureCounter;
};

//------END OF DELEGATEE------

//------ACTUAL END-USERS API-------
class VirTex {
public:
	virtual ~VirTex() {};
	VirTex(const shared_ptr<TextureDelegatee>& delegatee) :
		_delegatee(delegatee) {_delegatee->send_to_gpu();};
protected:
	shared_ptr<TextureDelegatee> _delegatee;
public:
	virtual void pre_render(const shared_ptr<TextureVisitor>& visitor);
	virtual void post_render();
};

//------ACTUAL END-USERS API-------
