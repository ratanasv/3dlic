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
		->GetFloatParameter(param).GetFloat());
}

static void BindBoolUniform(const char* var, LICBoolParam param) {
	VolumeTracingShader->SetUniform(var, THREEDLICParameters::INSTANCE->
		GetBoolParameter(param).GetBool());
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
	BindFloatUniform("uRainbowValMin", LICFloatParam::RAINBOW_VAL_MIN);
	BindFloatUniform("uRainbowValMax", LICFloatParam::RAINBOW_VAL_MAX);
	BindFloatUniform("uNumStepsLIC", LICFloatParam::NUM_STEPS_LIC);
	BindFloatUniform("uVelocityScale", LICFloatParam::VELOCITY_SCALE);
	BindFloatUniform("uDT", LICFloatParam::DT);
	BindFloatUniform("uMinMagnitude", LICFloatParam::MAGNITUDE_MIN);
	BindFloatUniform("uMaxMagnitude", LICFloatParam::MAGNITUDE_MAX);
	BindFloatUniform("uColorIntensity", LICFloatParam::COLOR_INTENSITY);
	BindBoolUniform("uRenderRayDepth", LICBoolParam::RENDER_RAY_DEPTH);
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
	ExistingPath VERTEX_SHADER_PATH(GetStringProperty(Property::PATH_VERTEX_SHADER));
	ExistingPath FRAGMENT_SHADER_PATH(GetStringProperty(Property::PATH_FRAGMENT_SHADER));
	ExistingPath NOISE_PATH(GetStringProperty(Property::PATH_NOISE));
	ExistingPath DATA_PATH(GetStringProperty(Property::PATH_DATA));

	string wtf = VERTEX_SHADER_PATH;
	VolumeTracingShader.reset(new GLSLProgram());
	VolumeTracingShader->Create(VERTEX_SHADER_PATH.c_str(),
		FRAGMENT_SHADER_PATH.c_str());
	VolumeTracingShader->Use();

	shared_ptr<TextureAbstractFactory> factory(new NoiseTex3DFactory(
		NOISE_PATH, 1));
	shared_ptr<TextureDelegatee> delegatee(new GLTextureDelegatee(factory));
	SparseNoise.reset(new VirTex(delegatee));

	factory.reset(new MFalkDataTex3DFactory(DATA_PATH.c_str(), GL_RGBA32F));
	delegatee.reset(new GLTextureDelegatee(factory));
	VectorDataTexture.reset(new VirTex(delegatee));

	shared_ptr<GeometryAbstractFactory> cubeFactory(new CubeGeometryFactory());
	shared_ptr<GeometryDelegatee> vaoFreeable(new VAODelegatee(cubeFactory, 
		VolumeTracingShader));
	Cube.reset(new VirModel(vaoFreeable));
}
