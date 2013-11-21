#include "StdAfx.h"
#include "gui6.h"
#include "gui_utility.h"
#include "virtex.h"
#include "virmodel.h"

using namespace vir;
using std::string;
namespace fs = boost::filesystem;

static shared_ptr<GLSLProgram> vir_shaders;
static shared_ptr<VirTex> SparseNoise;
static shared_ptr<VirModel> Cube;


static void viTexCoord3f(float s, float t, float p){
	vir_shaders->SetAttribute("TexCoord",s,t,p);
}

void init_light() {
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


void create_proj6_panel(GLUI* main_glui) {
	GLUI_Panel* panel = main_glui->add_rollout("project6", 1);
	slider45_factory(main_glui, panel, SliderBundle(NULL, NULL, 
		vec2(1.0,2.0), vec2(1.0,2.0), vec2(0.0,5.0), T_CLAMP, 
		"TEMP = %2.4f- %2.4f", true));
	slider45_factory(main_glui, panel, SliderBundle(NULL, NULL, 
		vec2(0.5,2.0), vec2(0.5,2.0), vec2(0.0,1000.0), ALPHA, 
		"ALPHA = %2.4f", false));
// 	GLUI_RadioGroup* group = main_glui->add_radiogroup_to_panel(panel, &blah, -1, callback);
// 	char* labels[] = {"texture enviroment","no texture", "modulate", "replace"};
// 	RadioBundle(main_glui, panel, TEX_ENV, labels, 4, &env_radio_callback);
// 	char* labels2[] = {"texture sampling", "nearest", "linear"};

}

void draw_terrain() {
	
}


void draw6() {
	vir_shaders->Use();
	SparseNoise->pre_render();
	Cube->render();
	SparseNoise->post_render();
}

void reset6() {
	for (auto it : BunchOfSliders) {
		it.second.reset();
	}
	for (auto it : BunchOfCheckboxes) {
		it.second.reset();
	}
	for (auto it : Bunch_Of_Radios) {
		it.second.reset();
	}
}

void animate6(float t) {
	
}



void disable_light() {
	glDisable(GL_LIGHTING);
}



void init6() {
	static const string BASE_PATH("E:/Vault/3dlic");
	static fs::path VERTEX_SHADER_PATH(BASE_PATH + "/shader/vert.vert");
	static fs::path FRAGMENT_SHADER_PATH(BASE_PATH + "/shader/frag.frag");
	static fs::path NOISE_PATH(BASE_PATH + "/noise/noise-256-sparse");
	vir_shaders.reset(new GLSLProgram());
	vir_shaders->Create(VERTEX_SHADER_PATH.string().c_str(),
		FRAGMENT_SHADER_PATH.string().c_str());
	vir_shaders->Use();
	shared_ptr<TextureAbstractFactory> factory(new NoiseTex3DFactory(
		NOISE_PATH.string().c_str(), TextureAbstractFactory::RED));
	shared_ptr<TextureDelegatee> delegatee(new GLTexReplaceDelegatee(
		factory));
	SparseNoise.reset(new VirTex(delegatee));

	shared_ptr<GeometryAbstractFactory> cubeFactory(new CubeGeometryFactory());
	shared_ptr<GeometryDelegatee> vaoFreeable(new VAODelegatee(cubeFactory, vir_shaders));
	Cube.reset(new VirModel(vaoFreeable));
}
