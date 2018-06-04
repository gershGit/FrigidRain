#pragma once
#include "FR_Material.h"

class Metallic : public FR_Material {
public:
	glm::vec3 albedo;
	float roughness;

	Metallic(const glm::vec3 &a, float f) : albedo(a), roughness(f) { if (f < 1) roughness = f; else roughness = 1; }
	virtual bool scatter(const Ray& r_in, const hit_info &info, glm::vec3 &attenuation, Ray& scattered) const {
		glm::vec3 reflected = glm::reflect(glm::normalize(r_in.get_direction()), info.normal);
		scattered = Ray(info.p, reflected + roughness*random_in_unit_sphere());
		attenuation = albedo;
		return (glm::dot(scattered.get_direction(), info.normal) > 0);
	}
};