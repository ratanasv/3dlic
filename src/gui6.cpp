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
auto LIC_PARAM(THREEDLICParameters::GetInstance());

static shared_ptr<TextureVisitor> TextureVisitorFactory() {
	return shared_ptr<TextureVisitor>(new GLSLTextureSamplerBinder());
}

static void BindFloatUniform(const char* var, THREEDLICParameters::FLOAT_PARAM param) {
#ifdef USE_GLUI
	VolumeTracingShader->SetUniform(var, BunchOfSliders.at(param).vals.x);
#else
	VolumeTracingShader->SetUniform(var, LIC_PARAM->GetFloatParameter(
		param));
#endif
}


void draw6() {
	static auto textureVisitor = TextureVisitorFactory();
	VolumeTracingShader->Use();
	SparseNoise->pre_render(textureVisitor);
	BindFloatUniform("uNumSteps", THREEDLICParameters::NUM_STEPS);
	BindFloatUniform("uBaseAlpha", THREEDLICParameters::BASE_ALPHA);
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
