#pragma once
#include "Ray.h"
#include "AABB.h"

class FR_Material;

struct hit_info {
	float t, u, v;
	glm::vec3 p;
	glm::vec3 normal;
	FR_Material *mat_ptr;
};

AABB surrounding_box(AABB box0, AABB box1) {
	glm::vec3 small(fmin(box0.min().x, box1.min().x),
		fmin(box0.min().y, box1.min().y),
		fmin(box0.min().z, box1.min().z));
	glm::vec3 big(fmax(box0.max().x, box1.max().x),
		fmax(box0.max().y, box1.max().y),
		fmax(box0.max().z, box1.max().z));
	return AABB(small, big);
}

class FR_Drawable
{
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_info &info) const = 0;
	virtual bool bounding_box(float t0, float t1, AABB &box) const = 0;
	virtual glm::vec3 getCenter(float time) const = 0;
};