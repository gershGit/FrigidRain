#pragma once
#include "Ray.h"

class FR_Material;

struct hit_info {
	float t;
	glm::vec3 p;
	glm::vec3 normal;
	FR_Material *mat_ptr;
};

class FR_Drawable
{
public:
	FR_Material *material;
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_info &info) const = 0;
};