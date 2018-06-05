#include "glm\glm.hpp"

class texture {
public:
	virtual glm::vec3 value(float u, float v, const glm::vec3 &p) const = 0;
};
class constant_texture : public texture {
public:
	glm::vec3 color;

	constant_texture(){}
	constant_texture(glm::vec3 c) : color(c){}
	virtual glm::vec3 value(float u, float v, const glm::vec3 &p) const {
		return color;
	}
};
class checker_texture : public texture {
public:
	texture * even;
	texture * odd;

	checker_texture() {}
	checker_texture(texture *t0, texture *t1) : even(t0), odd(t1) {}
	virtual glm::vec3 value(float u, float v, const glm::vec3 &p) const {
		float sines = sin(10 * p.x)*sin(10 * p.y)*sin(10 * p.z);
		if (sines < 0) {
			return odd->value(u, v, p);
		}
		else {
			return even->value(u, v, p);
		}
	}
};
class image_texture : public texture {
public:
	int nx, ny;
	unsigned char *data;

	image_texture() {}
	image_texture(unsigned char *pixels, int A, int B) : data(pixels), nx(A), ny(B) {}
	virtual glm::vec3 value(float u, float v, const glm::vec3 &p) const {
		//std::cout << "u: " << u << ", \tv: " << v << std::endl;
		int i = u * nx;
		int j = (1 - v) * ny - 0.001;
		if (i < 0) i = 0;
		if (j < 0) j = 0;
		if (i > nx - 1) i = nx - 1;
		if (j > ny - 1) j = ny - 1;
		float r = int(data[3 * i + 3 * nx  * j]);
		float g = int(data[3 * i + 3 * nx  * j + 1]);
		float b = int(data[3 * i + 3 * nx  * j + 2]);
		return glm::vec3((float)r/255.0, (float)g/255.0, (float)b/255.0);
	}
};