#pragma once
#include "FR_Material.h"
#include <random>

class Glass : public FR_Material {
public:
	glm::vec3 albedo;
	float IOR;

	Glass(float i) : IOR(i) {}
	virtual bool scatter(const Ray &r_in, const hit_info &info, glm::vec3 &attenuation, Ray& scattered) const {
		glm::vec3 outward_normal;
		glm::vec3 reflected = glm::reflect(r_in.get_direction(), info.normal);
		float ni_over_nt;
		attenuation = glm::vec3(1.0, 1.0, 1.0);
		glm::vec3 refracted;
		float reflect_prob;
		float cosine;
		if (glm::dot(r_in.get_direction(), info.normal) > 0) {
			outward_normal = -info.normal;
			ni_over_nt = IOR;
			cosine = IOR * glm::dot(r_in.get_direction(), info.normal) / r_in.get_direction().length();
		}
		else {
			outward_normal = info.normal;
			ni_over_nt = 1.0 / IOR;
			cosine = -glm::dot(r_in.get_direction(), info.normal) / r_in.get_direction().length();
		}
		if (refract(r_in.get_direction(), outward_normal, ni_over_nt, refracted)) {
			reflect_prob = schlick(cosine, IOR);
		}
		else {
			scattered = Ray(info.p, reflected, r_in.mTime);
			reflect_prob = 1.0;
		}
		if (rand_float() < reflect_prob) {
			scattered = Ray(info.p, reflected, r_in.mTime);
		}
		else {
			scattered = Ray(info.p, refracted, r_in.mTime);
		}
		return true;
	}
};