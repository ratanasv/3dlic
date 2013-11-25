/**
 * Adapter/Wrapper for converting Martin Falk's DatFile class into something
 * more like Vir's texture classes.
 */

#pragma once
#include "reader.h"
#include "virtex.h"
#include <cstring>

using std::string;


class MFalkDataTex3DFactory : public TextureAbstractFactory {
private:
	DatFile _datFile;
public:
	MFalkDataTex3DFactory(const string& fileName);
	virtual GLenum getInternalFormat();
	virtual int getWidth();
	virtual int getHeight();
	virtual int getDepth();
	virtual GLenum getFormat();
	virtual GLenum getType();
	virtual void* get_data();

};