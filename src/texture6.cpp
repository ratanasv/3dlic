#include "StdAfx.h"
#include "texture6.h"
#include <cstring>

using std::string;

static const string VOLUME_SAMPLER("uSparseNoiseSampler");


GLSLTextureSamplerBinder::GLSLTextureSamplerBinder(
	const shared_ptr<GLSLUniformBinder>& binder, const string& uniformVariable) : 
	_binder(binder), _uniformVariable(uniformVariable)
{

}

void GLSLTextureSamplerBinder::pre_render( const unsigned int whichTex, 
	GLenum bindSite, const shared_ptr<unsigned> texHandle )
{
	glTexParameteri( bindSite, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); 
	glTexParameteri( bindSite, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); 
	_binder->SetUniform(_uniformVariable, (int)whichTex);
}
