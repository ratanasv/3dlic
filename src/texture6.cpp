#include "StdAfx.h"
#include "texture6.h"
#include "gui6.h"
#include <cstring>

using std::string;

static const string VOLUME_SAMPLER("uSparseNoiseSampler");


void GLSLTextureSamplerBinder::pre_render( const unsigned int whichTex, 
GLenum bindSite, const shared_ptr<unsigned> texHandle )
{
	glTexParameteri( bindSite, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); 
	glTexParameteri( bindSite, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); 
	VolumeTracingShader->SetUniform(VOLUME_SAMPLER, (int)whichTex);
}
