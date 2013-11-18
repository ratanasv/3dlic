#pragma once
#include "StdAfx.h"
using std::shared_ptr;
using std::vector;

class GeometryAbstractFactory{
public:
	virtual ~GeometryAbstractFactory(){};
	virtual shared_ptr<vector<vir::vec3>> get_vertices()=0;
	virtual shared_ptr<vector<vir::vec3>> get_normals()=0;
	virtual shared_ptr<vector<vir::vec2>> get_tex_coord()=0;
	virtual shared_ptr<vector<unsigned>> get_indices()=0;
};

class OBJFactory: public GeometryAbstractFactory{
public:
	OBJFactory(const char* fn);
	virtual shared_ptr<vector<vir::vec3>> get_vertices();
	virtual shared_ptr<vector<vir::vec3>> get_normals();
	virtual shared_ptr<vector<vir::vec2>> get_tex_coord();
	virtual shared_ptr<vector<unsigned>> get_indices();
private:
	obj::ObjModel _model;
};

class GeometryDelegatee{
public:
	virtual ~GeometryDelegatee(){};
protected:
	GeometryDelegatee(const shared_ptr<GeometryAbstractFactory>& factory):
		_factory(factory){};
	shared_ptr<GeometryAbstractFactory> _factory;
public:
	virtual void send_to_gpu()=0;
	virtual void pre_render()=0;
	virtual void post_render()=0;
	virtual void render()=0;
};

class VAODelegatee:public GeometryDelegatee{
public:
	VAODelegatee(const shared_ptr<GeometryAbstractFactory>& factory);
	virtual void send_to_gpu();
	virtual void pre_render();
	virtual void post_render();
	virtual void render();
private:
	GLuint _vao;
	unsigned _num_indices;
};

class VirModel{
public:
	virtual ~VirModel(){};
	VirModel(const shared_ptr<GeometryDelegatee>& delegatee):
		_delegatee(delegatee){_delegatee->send_to_gpu();};
	shared_ptr<GeometryDelegatee> _delegatee;
public:
	virtual void render(){
		_delegatee->pre_render();
		_delegatee->render();
		_delegatee->post_render();
	}
};
