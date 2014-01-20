#pragma once

#include "virtex.h"
class GLSLUniformBinder;

class GLSLTextureSamplerBinder : public TextureVisitor {
private:
	const shared_ptr<GLSLUniformBinder> _binder;
	const string _uniformVariable;
public:
	GLSLTextureSamplerBinder(const shared_ptr<GLSLUniformBinder>& binder, 
		const string& uniformVariable);
	virtual void pre_render( const unsigned int whichTex, GLenum bindSite, 
		const shared_ptr<unsigned> texHandle );

};