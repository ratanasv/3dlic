/*
 *  vec.h
 *
 *  Created by Viriya Ratanasangpunth on 4/16/2013
 *  
 *	vec3 and vec4 classes
 */
#pragma once
#include <cmath>
#include <iostream>

namespace vir{
struct vec3{
	float x,y,z;
	inline vec3(float a=0.0, float b=0.0, float c=0.0):x(a),y(b),z(c){}
	inline vec3 operator/ (const vec3& o) const{
		return vec3(x/o.x, y/o.y, z/o.z);
	}
	inline vec3 operator* (const vec3& o) const{
		return vec3(o.x*x, o.y*y, o.z*z);
	}
	inline vec3 operator+ (const vec3& o) const{
		return vec3(o.x+x, o.y+y, o.z+z);
	}
	inline vec3 operator- (const vec3& o) const{
		return vec3(x-o.x, y-o.y, z-o.z);
	}
	inline vec3 operator* (const float c) const{
		return vec3(c*x, c*y, c*z);
	}
	inline vec3 operator/ (const float c) const{
		return vec3(x/c, y/c, z/c);
	}
	inline float operator[] (int i) const{
		return *(&x+i);
	}
	inline float& operator[] (int i){
		return *(&x+i);
	}
	inline operator float* (){
		return static_cast<float*>(&x);
	}
	inline operator const float* () const{
		return static_cast<const float*>(&x);
	}
	inline vec3 cross(const vec3& o) const{
		return vec3(y*o.z-o.y*z, o.x*z-x*o.z, x*o.y-o.x*y);
	}
};
inline bool is_equal(const vec3& l, const vec3& r){
	return l.x == r.x && l.y == r.y && l.z == r.z;
}
inline vec3 operator+ (float c, const vec3& o){
	return vec3(c+o.x, c+o.y, c+o.z);
}
inline vec3 operator- (float c, const vec3& o){
	return vec3(c-o.x, c-o.y, c-o.z);
}
inline vec3 operator* (float c, const vec3& o){
	return vec3(c*o.x, c*o.y, c*o.z);
}
inline vec3 operator/ (float c, const vec3& o){
	return vec3(c/o.x, c/o.y, c/o.z);
}
inline float length(const vec3& o){
	return sqrt(o.x*o.x+o.y*o.y+o.z*o.z);
}
inline vec3 normalize(const vec3& o){
	return o/length(o);
}
inline void swap(vec3& l, vec3& r){
	vec3 t = l;
	l = r;
	r = t;
}

inline std::ostream& operator<<(std::ostream& os, const vec3& b){
	os << "<" << b.x << "," << b.y << "," << b.z << ">";
	return os;
}

struct vec2{
	float x,y;
	inline vec2(float a=0.0, float b=0.0):x(a),y(b){}
	inline vec2 operator/ (const vec2& o) const{
		return vec2(x/o.x, y/o.y);
	}
	inline vec2 operator* (const vec2& o) const{
		return vec2(o.x*x, o.y*y);
	}
	inline vec2 operator+ (const vec2& o) const{
		return vec2(o.x+x, o.y+y);
	}
	inline vec2 operator- (const vec2& o) const{
		return vec2(x-o.x, y-o.y);
	}
	inline vec2 operator* (const float c) const{
		return vec2(c*x, c*y);
	}
	inline vec2 operator/ (const float c) const{
		return vec2(x/c, y/c);
	}
	inline float operator[] (int i) const{
		return *(&x+i);
	}
	inline float& operator[] (int i){
		return *(&x+i);
	}
	inline operator float* (){
		return static_cast<float*>(&x);
	}
	inline operator const float* () const{
		return static_cast<const float*>(&x);
	}
};
inline vec2 operator* (float c, const vec2& o){
	return vec2(c*o.x, c*o.y);
}
inline vec2 operator/ (float c, const vec2& o){
	return vec2(c/o.x, c/o.y);
}
inline float length(const vec2& o){
	return sqrt(o.x*o.x+o.y*o.y);
}
inline vec2 normalize(const vec2& o){
	return o/length(o);
}

inline std::ostream& operator<<(std::ostream& os, const vec2& b)
{
	os << "<" << b.x << "," << b.y  << ">";
	return os;
}
}

