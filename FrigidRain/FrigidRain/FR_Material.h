#pragma once
#include "Ray.h"
#include "FR_Drawable.h"

glm::vec3 random_in_unit_sphere() {
	return 2.0f*glm::vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - glm::vec3(1, 1, 1);
}

class FR_Material {
public:
	virtual bool scatter(const Ray &r_in, const hit_info &info, glm::vec3 &attenuation, Ray& scattered) const = 0;
};