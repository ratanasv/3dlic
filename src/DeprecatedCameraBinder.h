#pragma once

#include "GLSLCameraBinder.h"

// namespace Angel {
// 	template<class T = float> class vec3;
// }

class DeprecatedCameraBinder : public GLSLCameraBinder {
public:
	virtual void LookAt( const Angel::vec3<>& eye, const Angel::vec3<>& at, 
		const Angel::vec3<>& up );
	virtual void Ortho( float left, float right, float bottom, float top, 
		float zNear, float zFar );
	virtual void Perspective( float fovy, float ratio, float zNear, float zFar );
	virtual void Rotate( float x, float y, float z );
	virtual void Translate( float x, float y, float z );
	virtual void Scale( float x, float y, float z );
	virtual void ClearProjection();
	virtual void ClearModelView();

};