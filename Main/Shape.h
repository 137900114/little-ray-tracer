#pragma once
#include "../Tool/geometry.h"


struct Ray {
	Vec3f p, dir;
	Ray(Vec3f p, Vec3f dir)
		:p(p),dir(dir){}
};

struct Insection { Vec4f color; Vec3f n; float t; float reflect = 0.0;};

class Shape {
public:
	virtual bool insection(Ray r,Insection*) = 0;
	virtual ~Shape() {}
};


class Sphere :public Shape{
public:
	Sphere(Vec3f pos = Vec3f(0, 0, 0), Vec3f diffus = Vec3f(1, 0.7, 0.7), float radius = 1)
		:pos(pos),radius(radius),diffus(diffus){}
	virtual bool insection(Ray r,Insection* insec) override;
	
private:
	Vec3f pos,diffus;
	float radius;
};

class Plane : public Shape {
public:
	Plane(float width, float length, Matrix4x4f o2w,Vec3f diffus = Vec3f(1,1,1)):
		width(width),length(length),o2w(o2w),w2o(o2w.R()),diffus(diffus) {}
	virtual bool insection(Ray t,Insection* insec) override;
private:
	float width, length;
	Matrix4x4f w2o, o2w;
	Vec3f diffus;
};

class ReflectablePlane : public Plane{
public:
	using Plane::insection;

	ReflectablePlane(float reflect,float width,float length,Matrix4x4f o2w,Vec3f diffus = Vec3f(1,1,1)):
	Plane(width,length,o2w,diffus), reflect(reflect){}
	virtual bool insection(Ray r,Insection* insec)override;
private:
	float reflect;
};

class ReflectableSphere : public Sphere {
public:
	using Sphere::insection;

	ReflectableSphere(float reflect = 0,Vec3f pos = Vec3f(0, 0, 0), Vec3f diffus = Vec3f(1, 0.7, 0.7), float radius = 1):
	Sphere(pos,diffus,radius),reflect(reflect){}
	virtual bool insection(Ray r,Insection* insec) override;
private:
	float reflect;
};

struct Light {
	virtual ~Light() {}
	virtual Vec3f dir(Vec3f pos) = 0;
	Light(float intensity, Vec3f color) :
		intensity(clamp(intensity)),color(clamp3f(color,1,0)) {}

	float intensity;
	Vec3f color;
};

struct PointLight : public Light{
	PointLight(float intensity,Vec3f color,Vec3f pos)
	:Light(intensity,color),pos(pos){}
	virtual Vec3f dir(Vec3f pos) override { return (pos - this->pos).normalize(); }

	Vec3f pos;
};

struct DirectionalLight : public Light{
	DirectionalLight(float intensity,Vec3f color,Vec3f direction):
	Light(intensity,color),direction(direction.normalize()){}
	virtual Vec3f dir(Vec3f pos)override { return direction; }

	Vec3f direction;
};