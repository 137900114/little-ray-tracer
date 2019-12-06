#include "Shape.h"

bool Sphere::insection(Ray r,Insection* insec) {
	float t1, t2,ra = radius;
	Vec3f o = r.p - pos,d = r.dir;
	float a = d.x * d.x + d.y * d.y + d.z * d.z;
	float b = 2 * (d.x * o.x + d.y * o.y + d.z * o.z);
	float c = o.x * o.x + o.y * o.y + o.z * o.z - ra * ra;
	if (!solveX2(a, b, c, &t1, &t2)) return false;
	if (t2 < 0) return false;
	if (insec) {
		insec->color = Vec4f(diffus);
		insec->t = t1 > 0 ? t1 : t2;
		Vec3f insecPos = r.dir * insec->t + r.p;
		insec->n = (insecPos - pos).normalize();
	}
	return true;
}

bool Plane::insection(Ray r,Insection* insec) {
	Vec3f d = transform(w2o, r.dir, 0), o = transform(w2o, r.p, 1);
	float t;
	if (equalZero(d.y)|| (t = - o.y / d.y) < 0) return false;
	Vec3f p = o + d * t;
	if (p.x < -width || p.x > width || p.z <-length || p.z > length) return false;
	if (insec) {
		insec->color = diffus;
		insec->n = transform(o2w,Vec3f(0,1,0),0);
		insec->t = t;
	}
	return true;
}

bool ReflectableSphere::insection(Ray r,Insection* insec) {
	if (Sphere::insection(r, insec)) {
		insec->reflect = reflect;
		return true;
	}
	return false;
}

bool ReflectablePlane::insection(Ray r,Insection* insec) {
	if (Plane::insection(r, insec)) {
		insec->reflect = reflect;
		return true;
	}
	return false;
}