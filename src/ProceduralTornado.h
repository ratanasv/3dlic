#pragma once
#include "virtex.h"

class ProceduralTornado : public TextureData {
private:
	LazyInitOnce<shared_ptr<const void>> _data;
	const int _xDim;
	const int _yDim;
	const int _zDim;
public:
	ProceduralTornado(int xDim, int yDim, int zDim, int time);

	virtual GLenum getInternalFormat();
	virtual int getWidth();
	virtual int getHeight();
	virtual int getDepth();
	virtual GLenum getFormat();
	virtual GLenum getType();
	virtual shared_ptr<const void> get_data();

};