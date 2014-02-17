/**
 * Adapter/Wrapper for converting Martin Falk's DatFile class into something
 * more like Vir's texture classes.
 */

#pragma once
#include "reader.h"
#include "virtex.h"
#include <cstring>
#include <memory>
#include "Angel.h"

using Angel::vec4;
using std::weak_ptr;
using std::string;
using std::shared_ptr;

class PipelineVisitor;
class PipelineElement {
public:
	virtual ~PipelineElement() {};
	virtual void Accept(weak_ptr<PipelineVisitor> visitor) = 0;
};


class Normalizer;
class AlphaTrimmer;

class MFalkDataTex3DFactory : public TextureData {
private:
	DatFile _datFile;
	const GLenum _internalFormat;
public:
	MFalkDataTex3DFactory(const string& fileName, const GLenum internalFormat);
	virtual GLenum getInternalFormat();
	virtual int getWidth();
	virtual int getHeight();
	virtual int getDepth();
	virtual GLenum getFormat();
	virtual GLenum getType();
	virtual shared_ptr<void> get_data();
	virtual shared_ptr<vec4<>> getNormalizedData(const Normalizer& normalizer);
	virtual shared_ptr<vec3<>> getFloatVecData();
};