#pragma once
#include "virtex.h"

class GLSLTextureSamplerBinder : public TextureVisitor {
public:
	virtual void pre_render( const unsigned int whichTex, GLenum bindSite, 
		const shared_ptr<unsigned> texHandle );

};