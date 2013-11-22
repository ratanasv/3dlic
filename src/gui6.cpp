#include "StdAfx.h"
#include "gui6.h"
#include "glui_utility.h"
#include "virtex.h"
#include "virmodel.h"
#include "texture6.h"
#include "3dlic_model.h"

using namespace vir;
using std::string;
namespace fs = boost::filesystem;

shared_ptr<GLSLProgram> VolumeTracingShader;
static shared_ptr<VirTex> SparseNoise;
static shared_ptr<VirModel> Cube;

static shared_ptr<TextureVisitor> TextureVisitorFactory() {
	return shared_ptr<TextureVisitor>(new GLSLTextureSamplerBinder());
}


void create_proj6_panel(GLUI* main_glui) {
	GLUI_Panel* panel = main_glui->add_rollout("project6", 1);
	slider45_factory(main_glui, panel, SliderBundle(NULL, NULL, 
		vec2(100.0, 100.0), vec2(100.0,100.0), vec2(0.0, 2000.0), NUM_STEPS, 
		"NUM_STEPS = %3.2f", false));
	slider45_factory(main_glui, panel, SliderBundle(NULL, NULL, 
		vec2(0.2, 1.0), vec2(0.2,1.0), vec2(0.0,1.0), BASE_ALPHA, 
		"ALPHA = %0.4f", false));
// 	GLUI_RadioGroup* group = main_glui->add_radiogroup_to_panel(panel, &blah, -1, callback);
// 	char* labels[] = {"texture enviroment","no texture", "modulate", "replace"};
// 	RadioBundle(main_glui, panel, TEX_ENV, labels, 4, &env_radio_callback);
// 	char* labels2[] = {"texture sampling", "nearest", "linear"};

}

void draw_terrain() {
	
}


void draw6() {
	VolumeTracingShader->Use();
	SparseNoise->pre_render(TextureVisitorFactory());
	VolumeTracingShader->SetUniform("uNumSteps", BunchOfSliders.at(NUM_STEPS).vals.x);
	VolumeTracingShader->SetUniform("uBaseAlpha", BunchOfSliders.at(BASE_ALPHA).vals.x);
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


void init6() {
	static const string BASE_PATH("E:/Vault/3dlic");
	static fs::path VERTEX_SHADER_PATH(BASE_PATH + "/shader/vert.vert");
	static fs::path FRAGMENT_SHADER_PATH(BASE_PATH + "/shader/frag.frag");
	static fs::path NOISE_PATH(BASE_PATH + "/noise/noise-256-sparse");
	VolumeTracingShader.reset(new GLSLProgram());
	VolumeTracingShader->Create(VERTEX_SHADER_PATH.string().c_str(),
		FRAGMENT_SHADER_PATH.string().c_str());
	VolumeTracingShader->Use();
	shared_ptr<TextureAbstractFactory> factory(new NoiseTex3DFactory(
		NOISE_PATH.string().c_str(), TextureAbstractFactory::RED));
	shared_ptr<TextureDelegatee> delegatee(new GLTextureDelegatee(factory));
	SparseNoise.reset(new VirTex(delegatee));

	shared_ptr<GeometryAbstractFactory> cubeFactory(new CubeGeometryFactory());
	shared_ptr<GeometryDelegatee> vaoFreeable(new VAODelegatee(cubeFactory, 
		VolumeTracingShader));
	Cube.reset(new VirModel(vaoFreeable));
}
