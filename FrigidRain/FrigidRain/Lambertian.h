#pragma once
#include "FR_Material.h"

class Lambertian : public FR_Material {
public:
	texture *albedo;

	Lambertian(texture *a) : albedo(a) {}
	virtual bool scatter(const Ray &r_in, const hit_info &info, glm::vec3 &attenuation, Ray& scattered) const {
		glm::vec3 target = info.p + info.normal + random_in_unit_sphere();
		scattered = Ray(info.p, target - info.p, r_in.mTime);
		attenuation = albedo->value(info.u, info.v, info.p);
		//attenuation = albedo->value(info.u,info.v,info.p);
		return true;
	}
};