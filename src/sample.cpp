#include "StdAfx.h"

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif
#include "gui6.h"
#include "property.h"
#include "3dlic_model.h"
#include "glui_utility.h"
#include "LICFloatParam.h"

#ifdef USE_GLUI
#include "glui.h"
#endif

//
//
//	This is a sample OpenGL / GLUT / GLUI program
//
//	The objective is to draw45 a 3d object and change the color of the axes
//		with radio buttons
//
//	The left mouse button allows rotation
//	The middle mouse button allows scaling
//	The glui window allows:
//		1. The 3d object to be transformed
//		2. The projection to be changed
//		3. The color of the axes to be changed
//		4. The axes to be turned on and off
//		5. The transformations to be reset
//		6. The program to quit
//
//	Author:			Joe Graphics
//
//  Latest update:	March 27, 2013
//

// title of these windows:
const char *GLUITITLE   = { "Vir GLACIER NATIONAL PARK!!!" };


// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };


// the escape key:

#define ESCAPE		0x1b


// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// able to use the left mouse for either rotation or scaling,
// in case have only a 2-button mouse:

enum LeftButton
{
	ROTATE,
	SCALE
};


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:

enum Projections
{
	ORTHO,
	PERSP
};


// which button:

enum ButtonVals
{
	RESET,
	QUIT
};


// window background color (rgba):

const float BACKCOLOR[ ] = { 0.0, 0.0, 0.0, 1.0 };

const int PERIOD_MS = 10*1000;
float Timer;

bool Paused = false;

//
// non-constant global variables:
//

int	ActiveButton;			// current button that is down
int	DebugOn;				// != 0 means to print debugging info

int	GluiWindow;				// the glut id for the glui window
int	LeftButton;				// either ROTATE or SCALE
int	MainWindow;				// window id for main graphics window
int	WhichProjection;		// ORTHO or PERSP
int	Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;			// rotation angles in degrees


//
// function prototypes:
//

void	Animate( void );
void	Buttons( int );
void	Display( void );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( void );
void	InitGlui( void );
void	InitGraphics( void );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( void );
void	Resize2( int, int );
void	Resize( int, int );
void	Visibility( int );

void	Cross( float [3], float [3], float [3] );
float	Dot( float [3], float [3] );
float	Unit( float [3], float [3] );
void	Axes( float );
void	HsvRgb( float[3], float [3] );
void apply_transformations();

shared_ptr<GLSLCameraBinder> GLSLCameraBinderFactory();

// which way is a surface facing:

const int MINUS = { 0 };
const int PLUS  = { 1 };


// what is the major direction:

#define X    0
#define Y    1
#define Z    2

int    Major;			// X, Y, or Z
int    Xside, Yside, Zside;	// which side is visible, PLUS or MINUS

//
// main program:
//

int main( int argc, char *argv[ ] ) {
	//try {
		// turn on the glut package:
		// (do this before checking argc and argv since it might
		// pull some command line arguments out)

		glutInit( &argc, argv );
		InitProgramOptions(argc, argv);

		// setup all the graphics stuff:

		InitGraphics( );


		// init all the global variables used by Display( ):
		// this will also post a redisplay
		// it is important to call this before InitGlui( )
		// so that the variables that glui will control are correct
		// when each glui widget is created

		Reset( );


		// setup all the user interface stuff:
#ifdef USE_GLUI
		InitGlui();
#endif
		glutSetWindow(MainWindow);
		init6();

		glutMainLoop( );

// 	} catch (std::exception& error) {
// 		
// 		MessageBox(NULL, error.what(), "Uncaught Exception", MB_OK | MB_ICONERROR);
// 		fprintf(stderr, "uncaught exception : %s", error.what());
// 		return 1;
// 	}
	

	// this is here to make the compiler happy:

	return 0;
}



//
// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it
//

void
Animate( void )
{
	// put animation stuff in here -- change some global variables
	// for Display( ) to find:
	int ms = glutGet( GLUT_ELAPSED_TIME );
	ms = ms % PERIOD_MS;
	Timer = (float)ms / (float)( PERIOD_MS - 1 );
	animate6(Timer);
	// force a call to Display( ) next time it is convenient:

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}




//
// glui buttons callback:
//

void
Buttons( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
//			Glui->sync_live( );
			glutSetWindow( MainWindow );
			glutPostRedisplay( );
			break;

		case QUIT:
			// gracefully close the glui window:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:

//			Glui->close( );
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Button ID %d\n", id );
	}

}




//
// draw45 the complete scene:
//

void
Display( void )
{
	if( DebugOn != 0 )
	{
		fprintf( stderr, "Display\n" );
	}


	// set which window we want to do the graphics into:

	glutSetWindow( MainWindow );


	// erase the background:
	glReadBuffer(GL_FRONT);
	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );

	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );


	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	static auto camera = GLSLCameraBinderFactory();
	camera->ClearProjection();
	if( WhichProjection == ORTHO )
		camera->Ortho( -3., 3.,     -3., 3.,     0.1, 1000. );
	else
		camera->Perspective( 90., 1.,	0.1, 100. );


	// place the objects into the scene:

	apply_transformations();
	draw6();
	glutSwapBuffers();
	glFlush( );
}

//
// use glut to display a string of characters using a raster font:
//

void
DoRasterString( float x, float y, float z, char *s )
{
	char c;			// one character to print

	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}



//
// use glut to display a string of characters using a stroke font:
//

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
	char c;			// one character to print

	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05 + 33.33 );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}



//
// return the number of seconds since the start of the program:
//

float
ElapsedSeconds( void )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.;
}



//
// initialize the glui window:
//





//
// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions
//

void
InitGraphics( void )
{
	
	// setup the display mode:
	// ( *must* be done before call to glutCreateWindow( ) )
	// ask for color, double-buffering, and z-buffering:
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	const int windowSize = GetIntProperty(Property::WINDOW_SIZE);
	glutInitWindowSize( windowSize, windowSize );


	// open the window and set its title:

	MainWindow = glutCreateWindow(GetStringProperty(Property::WINDOW_TITLE).c_str());
	glutSetWindowTitle(GetStringProperty(Property::WINDOW_TITLE).c_str());
	// setup the clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );


	// setup the callback routines:


	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( 0, NULL, 0 );
	// DO NOT SET THE GLUT IDLE FUNCTION HERE !!
	glutIdleFunc( Display );
	// let glui take care of it in InitGlui ( )

	glutSetWindow( MainWindow );
	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);
}


//
// the keyboard callback:
//

void Keyboard( unsigned char c, int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

	switch( c )
	{
		case 'o':
		case 'O':
			WhichProjection = ORTHO;
			break;

		case 'p':
			Paused = ! Paused;
// 			if( Paused )
// 				GLUI_Master.set_glutIdleFunc( NULL );
// 			else
// 				GLUI_Master.set_glutIdleFunc( Animate );
			break;
		case 'P':
			WhichProjection = PERSP;
			break;

		case 'q':
		case 'Q':
		case ESCAPE:
			Buttons( QUIT );	// will not return here
			break;			// happy compiler

		case 'r':
		case 'R':
			LeftButton = ROTATE;
			break;

		case 's':
		case 'S':
			LeftButton = SCALE;
			break;

		default:
			fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
	}


	// synchronize the GLUI display with the variables:

//	Glui->sync_live( );


	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}



//
// called when the mouse button transitions down or up:
//

void
MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}


	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}



//
// called when the mouse moves while a button is down:
//

void MouseMotion( int x, int y ) {
	if( DebugOn != 0 )
		fprintf( stderr, "MouseMotion: %d, %d\n", x, y );


	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 ) {
		switch( LeftButton )
		{
			case ROTATE:
				Xrot += ( ANGFACT*dy );
				Yrot += ( ANGFACT*dx );
				break;

			case SCALE:

				break;
		}
	}


	if( ( ActiveButton & MIDDLE ) != 0 ) {
		const float zTranslate = GetTDLPInstance().GetFloatParameter(
			LICFloatParam::ZTRANSLATE).GetFloat();
		GetTDLPInstance().SetFloatParameter(
			LICFloatParam::ZTRANSLATE, zTranslate + SCLFACT*dy);
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}



// reset the transformations and the colors:
//
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void Reset( void ) {
	ActiveButton = 0;
	DebugOn = GLUIFALSE;
	LeftButton = ROTATE;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
	GetTDLPInstance().SetFloatParameter(
		LICFloatParam::XTRANSLATE, 0.0);
	GetTDLPInstance().SetFloatParameter(
		LICFloatParam::YTRANSLATE, 0.0);
	GetTDLPInstance().SetFloatParameter(
		LICFloatParam::ZTRANSLATE, 0.0);
	reset6();
}



//
// called when user resizes the window:
//

void
Resize( int width, int height )
{
	if( DebugOn != 0 )
		fprintf( stderr, "ReSize: %d, %d\n", width, height );

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


//
// handle a change to the window's visibility:
//

void
Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}




///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////
// axes:

#define X	1
#define Y	2
#define Z	3


float
Dot( float v1[3], float v2[3] )
{
	return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}



void
Cross( float v1[3], float v2[3], float vout[3] )
{
	float tmp[3];

	tmp[0] = v1[1]*v2[2] - v2[1]*v1[2];
	tmp[1] = v2[0]*v1[2] - v1[0]*v2[2];
	tmp[2] = v1[0]*v2[1] - v2[0]*v1[1];

	vout[0] = tmp[0];
	vout[1] = tmp[1];
	vout[2] = tmp[2];
}



float
Unit( float vin[3], float vout[3] )
{
	float dist = vin[0]*vin[0] + vin[1]*vin[1] + vin[2]*vin[2];

	if( dist > 0.0 )
	{
		dist = sqrt( dist );
		vout[0] = vin[0] / dist;
		vout[1] = vin[1] / dist;
		vout[2] = vin[2] / dist;
	}
	else
	{
		vout[0] = vin[0];
		vout[1] = vin[1];
		vout[2] = vin[2];
	}

	return dist;
}



// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = {
		0.f, 1.f, 0.f, 1.f
	      };

static float xy[ ] = {
		-.5f, .5f, .5f, -.5f
	      };

static int xorder[ ] = {
		1, 2, -3, 4
		};


static float yx[ ] = {
		0.f, 0.f, -.5f, .5f
	      };

static float yy[ ] = {
		0.f, .6f, 1.f, 1.f
	      };

static int yorder[ ] = {
		1, 2, 3, -2, 4
		};


static float zx[ ] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
	      };

static float zy[ ] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
	      };

static int zorder[ ] = {
		1, 2, 3, 4, -5, 6
		};


// fraction of the length to use as height of the characters:

const float LENFRAC = 0.10f;


// fraction of length to use as start location of the characters:

const float BASEFRAC = 1.10f;


//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}




// routine to convert HSV to RGB
//
// Reference:  Foley, van Dam, Feiner, Hughes,
//		"Computer Graphics Principles and Practices,"


void
HsvRgb( float hsv[3], float rgb[3] )
{
	float r, g, b;			// red, green, blue

	// guarantee valid input:

	float h = hsv[0] / 60.;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;


	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}


	// get an rgb from the hue itself:
	
	float i = floor( h );
	float f = h - i;
	float p = v * ( 1. - s );
	float q = v * ( 1. - s*f );
	float t = v * ( 1. - ( s * (1.-f) ) );

	switch( (int) i )
	{
		case 0:
			r = v;	g = t;	b = p;
			break;
	
		case 1:
			r = q;	g = v;	b = p;
			break;
	
		case 2:
			r = p;	g = v;	b = t;
			break;
	
		case 3:
			r = p;	g = q;	b = v;
			break;
	
		case 4:
			r = t;	g = p;	b = v;
			break;
	
		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

void InitGlui( void ) {
	GLUI_Panel *panel;
	GLUI_RadioGroup *group;
	GLUI_Rotation *rot;
	GLUI_Translation *trans;


	// setup the glui window:

	glutInitWindowPosition( GetIntProperty(Property::WINDOW_SIZE) + 50, 0 );
	GLUI* Glui = GLUI_Master.create_glui( const_cast<char*>(
		GetStringProperty(Property::WINDOW_TITLE).c_str()) );


	Glui->add_statictext(const_cast<char*>(
		GetStringProperty(Property::WINDOW_TITLE).c_str()));
	Glui->add_separator( );

	Glui->add_checkbox( "Perspective", &WhichProjection );

	Glui->add_checkbox( "Debug", &DebugOn );


	panel = Glui->add_panel( "", GLUIFALSE );

	Glui->add_button_to_panel( panel, "Reset", RESET, (GLUI_Update_CB) Buttons );

	Glui->add_column_to_panel( panel, GLUIFALSE );

	Glui->add_button_to_panel( panel, "Quit", QUIT, (GLUI_Update_CB) Buttons );


	// tell glui what graphics window it needs to post a redisplay to:

	Glui->set_main_gfx_window( MainWindow );


	// set the graphics window's idle function if needed:

	GLUI_Master.set_glutIdleFunc( Animate );
	create_proj6_panel(Glui);
}

void apply_transformations() {
	static auto camera = GLSLCameraBinderFactory();
	camera->ClearModelView();
	camera->LookAt( vec3<>(0., 0., 15.), vec3<>(0., 0., 0.), vec3<>(0., 1., 0.));
	
	camera->Translate(
		GetTDLPInstance().GetFloatParameter(
			LICFloatParam::XTRANSLATE).GetFloat(), 
		GetTDLPInstance().GetFloatParameter(
			LICFloatParam::YTRANSLATE).GetFloat(),
		GetTDLPInstance().GetFloatParameter(
			LICFloatParam::ZTRANSLATE).GetFloat()
	);
	camera->Rotate(Xrot, Yrot, 0.0);

}

shared_ptr<GLSLCameraBinder> GLSLCameraBinderFactory() {
	return shared_ptr<GLSLCameraBinder>(VolumeTracingShader);
}

