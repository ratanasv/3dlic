#include "StdAfx.h"
#include "gui6.h"
#include "glui_utility.h"
#include "virtex.h"
#include "virmodel.h"
#include "texture6.h"
#include "3dlic_model.h"
#include "mfalk_reader_wrapper.h"

using namespace vir;
using std::string;
namespace fs = boost::filesystem;

shared_ptr<GLSLProgram> VolumeTracingShader;
static shared_ptr<VirTex> SparseNoise;
static shared_ptr<VirModel> Cube;
static shared_ptr<VirTex> VectorDataTexture;

static void BindFloatUniform(const char* var, LICFloatParam param) {
	VolumeTracingShader->SetUniform(var, THREEDLICParameters::INSTANCE
		->GetFloatParameter(param));
}


void draw6() {
	static shared_ptr<TextureVisitor> sparseNoiseVisitor(new GLSLTextureSamplerBinder(
		VolumeTracingShader, "uSparseNoiseSampler"));
	static shared_ptr<TextureVisitor> vectorDataVisitor(new GLSLTextureSamplerBinder(
		VolumeTracingShader, "uVectorData"));
	SparseNoise->pre_render(sparseNoiseVisitor);
	VectorDataTexture->pre_render(vectorDataVisitor);
	BindFloatUniform("uNumSteps", LICFloatParam::NUM_STEPS);
	BindFloatUniform("uBaseAlpha", LICFloatParam::BASE_ALPHA);
	BindFloatUniform("uValMin", LICFloatParam::CLAMP_VAL_MIN);
	BindFloatUniform("uValMax", LICFloatParam::CLAMP_VAL_MAX);
	BindFloatUniform("uNumStepsLIC", LICFloatParam::NUM_STEPS_LIC);
	BindFloatUniform("uVelocityScale", LICFloatParam::VELOCITY_SCALE);
	BindFloatUniform("uDT", LICFloatParam::DT);
	Cube->render();
	VectorDataTexture->post_render();
	SparseNoise->post_render();

}

void reset6() {
// 	for (auto it : BunchOfSliders) {
// 		it.second.reset();
// 	}
// 	for (auto it : BunchOfCheckboxes) {
// 		it.second.reset();
// 	}
}

void animate6(float t) {
	
}


void init6() {
	static const string BASE_PATH("E:/Vault/3dlic");
	static fs::path VERTEX_SHADER_PATH(BASE_PATH + "/shader/vert.vert");
	static fs::path FRAGMENT_SHADER_PATH(BASE_PATH + "/shader/frag.frag");
	static fs::path NOISE_PATH(BASE_PATH + "/noise/noise-256-sparse");
	static fs::path DATA_PATH(BASE_PATH + "/data/cyl_float.dat");

	VolumeTracingShader.reset(new GLSLProgram());
	VolumeTracingShader->Create(VERTEX_SHADER_PATH.string().c_str(),
		FRAGMENT_SHADER_PATH.string().c_str());
	VolumeTracingShader->Use();

	shared_ptr<TextureAbstractFactory> factory(new NoiseTex3DFactory(
		NOISE_PATH.string().c_str(), 1));
	shared_ptr<TextureDelegatee> delegatee(new GLTextureDelegatee(factory));
	SparseNoise.reset(new VirTex(delegatee));

	factory.reset(new MFalkDataTex3DFactory(DATA_PATH.string().c_str(), GL_RGBA32F));
	delegatee.reset(new GLTextureDelegatee(factory));
	VectorDataTexture.reset(new VirTex(delegatee));

	shared_ptr<GeometryAbstractFactory> cubeFactory(new CubeGeometryFactory());
	shared_ptr<GeometryDelegatee> vaoFreeable(new VAODelegatee(cubeFactory, 
		VolumeTracingShader));
	Cube.reset(new VirModel(vaoFreeable));
}
