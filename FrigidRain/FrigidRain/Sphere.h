#pragma once
#include "FR_Drawable.h"

class Sphere : public FR_Drawable {
public:
	glm::vec3 center;
	float radius;
	FR_Material* material;

	Sphere() {}
	Sphere(glm::vec3 cen, float r, FR_Material* mat) : center(cen), radius(r), material(mat) {};
	virtual bool hit(const Ray& r, float tmin, float tmax, hit_info &info) const;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_info &info) const {
	glm::vec3 oc = r.get_origin() - center;
	float a = glm::dot(r.get_direction(), r.get_direction());
	float b = glm::dot(oc, r.get_direction());
	float c = glm::dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp < t_max && temp > t_min) {
			info.t = temp;
			info.p = r.point_at_parameter(info.t);
			info.normal = (info.p - center) / radius;
			info.mat_ptr = material;
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp < t_max && temp > t_min) {
			info.t = temp;
			info.p = r.point_at_parameter(info.t);
			info.normal = (info.p - center) / radius;
			info.mat_ptr = material;
			return true;
		}
	}
	return false;
}