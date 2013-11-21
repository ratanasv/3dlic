#pragma once
#include <cstdio>
#include <cmath>

#ifdef WIN32
#include <windows.h>
#endif

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include <map>
#include <stdarg.h>
#include <cstring>

using std::string;

#define NVIDIA_SHADER_BINARY	0x00008e21		// nvidia binary enum

inline int GetOSU( int flag )
{
	int i;
	glGetIntegerv( flag, &i );
	return i;
}


void	CheckGlErrors( const char* );

class GLSLAttributeBinder {
public:
	virtual ~GLSLAttributeBinder() {};
	virtual bool EnablePositionAttribute(const int vecLength = 4) = 0;
	virtual bool EnableNormalAttribute(const int vecLength = 4) = 0;
	virtual bool EnableColorAttribute(const int vecLength = 4) = 0;
	virtual bool EnableTexCoordAttribute(const int vecLength = 4) = 0;
};

class GLSLProgram
{
  private:
	std::map<const string, int>	AttributeLocs;
	char *			Cfile;
	unsigned int		Cshader;
	char *			Ffile;
	unsigned int		Fshader;
	char *			Gfile;
	GLuint			Gshader;
	bool			IncludeGstap;
	GLenum			InputTopology;
	GLenum			OutputTopology;
	GLuint			Program;
	char *			TCfile;
	GLuint			TCshader;
	char *			TEfile;
	GLuint			TEshader;
	std::map<char *, int>	UniformLocs;
	bool			Valid;
	char *			Vfile;
	GLuint			Vshader;
	bool			Verbose;

	static int		CurrentProgram;

	void	AttachShader( GLuint );
	bool	CanDoBinaryFiles;
	bool	CanDoComputeShaders;
	bool	CanDoFragmentShaders;
	bool	CanDoGeometryShaders;
	bool	CanDoTessControlShaders;
	bool	CanDoTessEvaluationShaders;
	bool	CanDoVertexShaders;
	int	CompileShader( GLuint );
	bool	CreateHelper( const char *, ... );
	
	int	GetUniformLocation( char * );


  public:
		GLSLProgram( );
	int	GetAttributeLocation( const char * );
	bool	Create( const char*, const char* = NULL, const char* = NULL, const char* = NULL, const char* = NULL, const char* = NULL );
	void	DispatchCompute( GLuint, GLuint = 1, GLuint = 1 );
	bool	IsExtensionSupported( const char * );
	bool	IsNotValid( );
	bool	IsValid( );
	void	LoadBinaryFile( char * );
	void	LoadProgramBinary( const char *, GLenum );
	void	SaveBinaryFile( char * );
	void	SaveProgramBinary( const char *, GLenum * );
	void	SetAttribute( char *, int );
	void	SetAttribute( char *, float );
	void	SetAttribute( char *, float, float, float );
	void	SetAttribute( char *, float[3] );
#ifdef VEC3_H
	void	SetAttribute( char *, Vec3& );
#endif
#ifdef VERTEX_ARRAY_H
	void	SetAttribute( char *, VertexArray&, GLenum );
#endif
#ifdef VERTEX_BUFFER_OBJECT_H
	void	SetAttribute( char *, VertexBufferObject&, GLenum );
#endif
	void	EnableVertexAttribute(const char* name, const int vecLength = 4); 
	void	SetGstap( bool );
	void	SetInputTopology( GLenum );
	void	SetOutputTopology( GLenum );
	void	SetUniform( char *, int );
	void	SetUniform( char *, float );
	void	SetUniform( char *, float, float, float );
	void	SetUniform( char *, float[3] );
#ifdef VEC3_H
	void	SetUniform( char *, Vec3& );
#endif
#ifdef MATRIX4_H
	void	SetUniform( char *, Matrix4& );
#endif
	void	SetVerbose( bool );
	void	Use( );
	void	Use( GLuint );
	void	UseFixedFunction( );
};

