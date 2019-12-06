#include "../Tool/geometry.h"
#include "../Tool/PNGImage.h"
#include "../Tool/ImageMASS.h"
#include "Shape.h"
#include <vector>
#define MAX_VIEW_DIST 1e3
#define MAX_TRACE_TIME 5

int height = 1080, width = 1920;

Vec3f lightDir(1,-2,3);


const char* filename = "RayTracerWithMASS.png";
bool insect(const std::vector<Shape*>& shape,Ray r,Insection* insec);
Vec4f cast_ray(Ray r,const std::vector<Shape*>& shapes,const std::vector<Light*>& light,int depth);
void render(Image*,std::vector<Shape*>,std::vector<Light*>);

Matrix4x4f transformM(Vec3f pos,Vec3f RotationXYZ) {
	Matrix4x4f position = PositionM(pos);
	Matrix4x4f rotation = RotationM(Vec3f(1,0,0),RotationXYZ.x) * RotationM(Vec3f(0,1,0),RotationXYZ.y) * RotationM(Vec3f(0,0,1),RotationXYZ.z);
	return position * rotation;
}

int main() {
	lightDir = lightDir.normalize();

	
	ImageMASS mass(width,height);
	PNGImage png(width, height, RGBCOLOR);
	Sphere sphere1(Vec3f(2, 0, 3));
	ReflectableSphere sphere2(1,Vec3f(-2, 1, 4), Vec3f(0.8, 0.8, 1));
	ReflectableSphere sphere3(0.3,Vec3f(0.5, 0.5, 5), Vec3f(1,1,1));
	Sphere sphere4(Vec3f(0,3,5),Vec3f(1,0.5,0.5));
	
	Plane plane(10, 10, PositionM(Vec3f(0, -1, 0)));
	ReflectablePlane mirror(0.9,10,10,transformM(Vec3f(0,5,8),Vec3f(90,0,0)));
	std::vector<Shape*> shapes = { &sphere1,&sphere3 ,&sphere2,&plane};
	DirectionalLight light1(0.5, Vec3f(1, 1, 1), Vec3f(1, -1, 1));
	DirectionalLight light2(0.5, Vec3f(1, 0, 0), Vec3f(0, 0, 1));
	std::vector<Light*> lights = {&light1,&light2};

	//render(&png,shapes,lights);
	render(&mass,shapes,lights);
	mass.sample(&png);

	png.flip_vertically();
	png.save(filename);
}


void render(Image* image,std::vector<Shape*> shape,std::vector<Light*> light) {
	int height = image->get_height();
	int width = image->get_width();
	
	for (int x = 0; x != width; x++) {
		for (int y = 0; y != height;y++) {
			float xh = 2.f * (float)x / (float)height - (float)width / (float)height;
			float yh = 2.f * (float)y / (float)height - 1.f;
			Vec3f pos(0, 0, 0), dir(xh,yh,1);
			Ray r(pos,dir.normalize());
			Vec4f color = cast_ray(r, shape, light,0);
			image->set(x,y,Color(color.x * 255,color.y * 255,color.z * 255,color.a * 255));
		}
	}
}

bool insect(const std::vector<Shape*>& shape, Ray r, Insection* insec) {
	float depth = MAX_VIEW_DIST;
	Insection temp;
	for (int i = 0; i != shape.size(); i++) {
		if (!shape[i]->insection(r, &temp) || temp.t > depth) continue;
		depth = temp.t;
		if(insec) *insec = temp;
	}
	if (depth == MAX_VIEW_DIST) return false;
	return true;
}

Vec4f cast_ray(Ray r,const std::vector<Shape*>& shape,const std::vector<Light*>& light,int depth) {
	Vec4f diffus(0,0,0,0),specular(0,0,0,0),color;
	Insection insec;
	if (depth >= MAX_TRACE_TIME || !insect(shape, r, &insec)) {
		return Vec4f(0.3, 0.4, 0.5, 1);
	}
	Vec3f normal = insec.n,viewDir = r.dir,hitP = r.p + r.dir * insec.t + normal * 1e-3;
	Vec4f aldebo = insec.color;
	for (int i = 0; i != light.size(); i++) {
		Vec3f lightDir = light[i]->dir(hitP);

		Ray shadow_caster(hitP,-lightDir);
		if (insect(shape, shadow_caster, nullptr)) continue;

		Vec3f half = - (lightDir + viewDir).normalize();

	    diffus = diffus + aldebo * light[i]->intensity * clamp(-lightDir * normal);
		specular = specular + light[i]->color * light[i]->intensity * pow(clamp(half * normal),256);
	}
	if (!equalZero(insec.reflect)) {
		Vec3f reflectDir = viewDir - normal * (viewDir * normal) * 2;
		Ray reflect(hitP, reflectDir.normalize());
		color = diffus * (1 - insec.reflect) + cast_ray(reflect, shape, light, depth + 1) * insec.reflect + specular;
	}
	else
		color = diffus + specular;

	return clamp4f(color, 1, 0);
}


