#include "StdAfx.h"
#include "gui6.h"
#include "gui_utility.h"
#include "virtex.h"
#include "virmodel.h"

using namespace vir;
using namespace std;
GLSLProgram* vir_shaders;
VirTex perlin;

static void viTexCoord3f(float s, float t, float p){
	vir_shaders->SetAttribute("TexCoord",s,t,p);
}

void init_light()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GLfloat light_ambient0[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_diffuse0[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat light_specular0[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}


void create_proj6_panel(GLUI* main_glui)
{
	GLUI_Panel* panel = main_glui->add_rollout("project6", 1);
	slider45_factory(main_glui, panel, struct SliderBundle(NULL, NULL, 
		vec2(1.0,2.0), vec2(1.0,2.0), vec2(0.0,5.0), T_CLAMP, 
		"TEMP = %2.4f- %2.4f", true));
	slider45_factory(main_glui, panel, struct SliderBundle(NULL, NULL, 
		vec2(0.5,2.0), vec2(0.5,2.0), vec2(0.0,1000.0), ALPHA, 
		"ALPHA = %2.4f", false));
// 	GLUI_RadioGroup* group = main_glui->add_radiogroup_to_panel(panel, &blah, -1, callback);
// 	char* labels[] = {"texture enviroment","no texture", "modulate", "replace"};
// 	RadioBundle(main_glui, panel, TEX_ENV, labels, 4, &env_radio_callback);
// 	char* labels2[] = {"texture sampling", "nearest", "linear"};

}

void draw_terrain()
{
	
}


void draw6()
{
	vir_shaders->Use();
	perlin->pre_render();
	vir_shaders->SetUniform("noise3d", 0);
	float p = Bunch_Of_Sliders[ALPHA].vals.x;
	vir_shaders->SetUniform("multiplier", p);
	glBegin(GL_QUADS);
	viTexCoord3f(0.0,0.0,p);
	glVertex3f(-1.0,-1.0,0.0);
	viTexCoord3f(1.0,0.0,p);
	glVertex3f(1.0,-1.0,0.0);
	viTexCoord3f(1.0,1.0,p);
	glVertex3f(1.0,1.0,0.0);
	viTexCoord3f(0.0,1.0,p);
	glVertex3f(-1.0,1.0,0.0);
	glEnd();
	perlin->post_render();
}

void reset6()
{
	for(auto it1=Bunch_Of_Sliders.begin(); it1!=Bunch_Of_Sliders.end(); it1++)
		it1->second.reset();
	for(auto it2=Bunch_Of_Checkboxes.begin(); it2!=Bunch_Of_Checkboxes.end(); it2++)
		it2->second.reset();
	for(auto it3=Bunch_Of_Radios.begin(); it3!=Bunch_Of_Radios.end(); it3++)
		it3->second.reset();
}

void animate6(float t)
{
	
}



void disable_light()
{
	glDisable(GL_LIGHTING);
}



void init6()
{
	vir_shaders = new GLSLProgram();
	vir_shaders->Create("p10.vert", "p10.frag");
	vir_shaders->Use();
	vir_shaders->GetAttributeLocation("TexCoord");
	vir_shaders->GetAttributeLocation("asdf");
	shared_ptr<TextureAbstractFactory> factory(new NoiseTex3DFactoryImpl(
		"noise3d.256.tex"));
	auto delegatee = TextureDelegatee(new GLTexReplaceDelegateeImpl(
		factory));
	perlin = VirTex(new VirTexImpl(delegatee));
}
