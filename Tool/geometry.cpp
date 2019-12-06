#include "geometry.h"


Matrix4x4f RotationM(const Vec3f& vec,float a) {
	Vec3f n = vec / vec.norm();
	a = a * PI / 180;
	float c = cos(a);
	float s = sin(a);
	float onemicx = (1 - c) * n.x;
	float onemicy = (1 - c) * n.y;
	float onemicz = (1 - c) * n.z;
	float sx = s * n.x;
	float sy = s * n.y;
	float sz = s * n.z;

	float m[] = {
		onemicx * n.x + c ,onemicx * n.y + sz,onemicx * n.z - sy,0,
		onemicy * n.x - sz,onemicy * n.y + c ,onemicy * n.z + sx,0,
		onemicz * n.x + sy,onemicz * n.y - sx,onemicz * n.z + c ,0,
		0                 , 0                 ,0                ,1

	};
	for (int i = 0; i != 16;i++) {
		if (abs(m[i]) < EBP)
			m[i] = 0;
	}

	return Matrix4x4f(m);
}

Matrix4x4f PositionM(const Vec3f& pos) {
	float m[] = {
		1    ,0    ,0    ,pos.x,
		0    ,1    ,0    ,pos.y,
		0    ,0    ,1    ,pos.z,
		0    ,0    ,0    ,1
	};
	return Matrix4x4f(m);
}

Matrix4x4f ScaleM(const Vec3f& scale) {
	float m[] = {
		scale.x,0      ,0      ,0,
		0      ,scale.y,0      ,0,
		0      ,0      ,scale.z,0,
		0      ,0      ,0      ,1
	};
	return  Matrix4x4f(m);
}

Vec3<int> ftoi(Vec3<float> vec) {
	return Vec3i(vec.x, vec.y, vec.z);
}

float clamp(float f) {
	return  clamp(f,1,0);
}

float clamp(float f, float max, float min) {
	if (max < min) std::swap(max,min);
	return f > max ? max : (f < min ? min : f);
}

bool solveX2(float a,float b,float c,float* x1,float* x2) {
	float delta = b * b - 4 * a * c;
	if (delta < 0) return false;
	delta = sqrtf(delta);
	*x1 = (-b + delta) / (a * 2);
	*x2 = (-b - delta) / (a * 2);
	if (*x1 > * x2) std::swap(*x1,*x2);
	return true;
}
