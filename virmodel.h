#pragma once
#include "StdAfx.h"

class GeometryAbstractFactoryImpl{
public:
	virtual ~GeometryAbstractFactoryImpl(){};
	virtual std::shared_ptr<std::vector<vir::vec3>> get_vertices()=0;
	virtual std::shared_ptr<std::vector<vir::vec3>> get_normals()=0;
	virtual std::shared_ptr<std::vector<vir::vec2>> get_tex_coord()=0;
	virtual std::shared_ptr<std::vector<unsigned>> get_indices()=0;
};
typedef std::shared_ptr<GeometryAbstractFactoryImpl> GeometryAbstractFactory;

class OBJFactoryImpl: public GeometryAbstractFactoryImpl{
public:
	OBJFactoryImpl(const char* fn);
	virtual std::shared_ptr<std::vector<vir::vec3>> get_vertices();
	virtual std::shared_ptr<std::vector<vir::vec3>> get_normals();
	virtual std::shared_ptr<std::vector<vir::vec2>> get_tex_coord();
	virtual std::shared_ptr<std::vector<unsigned>> get_indices();
private:
	obj::ObjModel _model;
};

class GeometryDelegateeImpl{
public:
	virtual ~GeometryDelegateeImpl(){};
protected:
	GeometryDelegateeImpl(const GeometryAbstractFactory& factory):
		_factory(factory){};
	GeometryAbstractFactory _factory;
public:
	virtual void send_to_gpu()=0;
	virtual void pre_render()=0;
	virtual void post_render()=0;
	virtual void render()=0;
};
typedef std::shared_ptr<GeometryDelegateeImpl> GeometryDelegatee;

class VAODelegateeImpl:public GeometryDelegateeImpl{
public:
	VAODelegateeImpl(const GeometryAbstractFactory& factory);
	virtual void send_to_gpu();
	virtual void pre_render();
	virtual void post_render();
	virtual void render();
private:
	GLuint _vao;
	unsigned _num_indices;
};

class VirModelImpl{
public:
	virtual ~VirModelImpl(){};
	VirModelImpl(const GeometryDelegatee& delegatee):
		_delegatee(delegatee){_delegatee->send_to_gpu();};
	GeometryDelegatee _delegatee;
public:
	virtual void render(){
		_delegatee->pre_render();
		_delegatee->render();
		_delegatee->post_render();
	}
};
typedef std::shared_ptr<VirModelImpl> VirModel;
