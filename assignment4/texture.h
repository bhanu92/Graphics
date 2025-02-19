//
// new texture class to support solid colors and procedural textures
// new for assignment 4
//
#ifndef TEXTUREH
#define TEXTUREH
#include "vec3.h"
class texture {
      public:
	virtual vec3 value(float u, float v, const vec3& p) const = 0;
};
class constant_texture : public texture {
      public:
	constant_texture() {}
	constant_texture(vec3 c) : color(c){};
	virtual vec3 value(float u, float v, const vec3& p) const {
		return color;
	}
	vec3 color;
};
class checker_texture : public texture {
      public:
	checker_texture() {}
	checker_texture(texture* t0, texture* t1) : even(t0), odd(t1) {}
	virtual vec3 value(float u, float v, const vec3& p) const {
		float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
	// the checker odd/even pointers can be to a constant texture or to
	// some other procedural texture
	// this is in the spirit of shader networks introduced by Pat
	// Hanrahan back in the 1980s
	texture* odd;
	texture* even;
};
#endif
