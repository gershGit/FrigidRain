#pragma once
#include "Ray.h"
#include "FR_Drawable.h"
#include "FR_Texture.h"

float schlick(float cosine, float ior) {
	float r0 = (1 - ior) / (1 + ior);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}
bool refract(const glm::vec3 & v, const glm::vec3 &n, float ni_over_nt, glm::vec3 &refracted) {
	glm::vec3 uv = glm::normalize(v);
	float dt = glm::dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt*(1 - dt * dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else {
		return false;
	}
}
glm::vec3 random_in_unit_sphere() {
	return 2.0f*glm::vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - glm::vec3(1, 1, 1);
}

class FR_Material {
public:
	virtual bool scatter(const Ray &r_in, const hit_info &info, glm::vec3 &attenuation, Ray& scattered) const = 0;
};