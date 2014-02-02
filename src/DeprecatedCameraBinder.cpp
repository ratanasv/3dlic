#include "StdAfx.h"
#include "DeprecatedCameraBinder.h"

void DeprecatedCameraBinder::LookAt( const vec3<>& eye, const vec3<>& at, const vec3<>& up ) {
	glMatrixMode( GL_MODELVIEW );
	gluLookAt( eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);
}

void DeprecatedCameraBinder::Ortho( float left, float right, float bottom, 
								   float top, float zNear, float zFar ) 
{
	glMatrixMode( GL_PROJECTION );
	glOrtho(left, right, bottom, top, zNear, zFar);
}

void DeprecatedCameraBinder::Perspective( float fovy, float ratio, float zNear, 
										 float zFar ) 
{
	glMatrixMode( GL_PROJECTION );
	gluPerspective(fovy, ratio, zNear, zFar);
}

void DeprecatedCameraBinder::Rotate( float x, float y, float z ) {
	glMatrixMode( GL_MODELVIEW );
	glRotatef(z, 0.0, 0.0, 1.0);
	glRotatef(y, 0.0, 1.0, 0.0);
	glRotatef(x, 1.0, 0.0, 0.0);
}

void DeprecatedCameraBinder::Translate( float x, float y, float z ) {
	glMatrixMode( GL_MODELVIEW );
	glTranslatef(x, y, z);
}

void DeprecatedCameraBinder::Scale( float x, float y, float z ) {
	glMatrixMode( GL_MODELVIEW );
	glScalef(x,y,z);
}

void DeprecatedCameraBinder::ClearProjection() {
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
}

void DeprecatedCameraBinder::ClearModelView() {
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
}
