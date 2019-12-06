#pragma once
#include "geometry.h"

struct Color {
	union {
		struct {
			unsigned char b, g, r, a;
		};
		unsigned char raw[4];
		unsigned int val;
		float    depth;
	};
	int bytespp;

	Color() : val(0), bytespp(1) {
	}

	Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) : b(B), g(G), r(R), a(A), bytespp(4) {
	}

	Color(int v, int bpp) : val(v), bytespp(bpp) {
	}

	Color(const Color& c) : val(c.val), bytespp(c.bytespp) {
	}

	Color(Vec3f color):r(color.x),g(color.y),b(color.z),a(255){}

	Color(Vec4f color) :r(color.x), g(color.y), b(color.z), a(color.a) {}

	Color(const unsigned char* p, int bpp) : val(0), bytespp(bpp) {
		for (int i = 0; i < bpp; i++) {
			raw[i] = p[i];
		}
	}

	Color& operator =(const Color& c) {
		if (this != &c) {
			bytespp = c.bytespp;
			val = c.val;
		}
		return *this;
	}

};

class Image
{
public:
	virtual int get_width() = 0;
	virtual int get_height() = 0;
	virtual Color get(int x,int y) = 0;
	virtual bool set(int x,int y,Color color) = 0;
};

inline Vec4f ColorToVec4f(Color c) {
	return Vec4f(c.r,c.g,c.b,c.a);
}

