#pragma once

#include "GLSLAttributeBinder.h"

class DeprecatedAttributeBinder : public GLSLAttributeBinder {
public:
	virtual void EnablePositionAttribute( const int vecLength = 4 );
	virtual void EnableNormalAttribute( const int vecLength = 4 );
	virtual void EnableColorAttribute( const int vecLength = 4 );
	virtual void EnableTexCoordAttribute( const int vecLength = 4 );
};