#pragma once
#include "FR_Material.h"

class Lambertian : public FR_Material {
public:
	glm::vec3 albedo;

	Lambertian(const glm::vec3 &a) : albedo(a) {}
	virtual bool scatter(const Ray &r_in, const hit_info &info, glm::vec3 &attenuation, Ray& scattered) const {
		glm::vec3 target = info.p + info.normal + random_in_unit_sphere();
		scattered = Ray(info.p, target - info.p);
		attenuation = albedo;
		return true;
	}
};