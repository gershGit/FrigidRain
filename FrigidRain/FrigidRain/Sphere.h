#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "FR_Drawable.h"

void get_sphere_uv(const glm::vec3 &p, float &u, float &v) {
	float phi = atan2(p.z, p.x);
	float theta = asin(p.y);
	u = 1 - (phi + M_PI) / (2 * M_PI);
	v = (theta + M_PI / 2) / M_PI;
}

class Sphere : public FR_Drawable {
public:
	const char* myname;

	float time0 = 0.0;
	float time1 = 0.0;
	glm::vec3 center_t0;
	glm::vec3 center_t1;

	glm::vec3 center;
	float radius;
	FR_Material* material;

	Sphere() {}
	Sphere(glm::vec3 cen, float r, FR_Material* mat, float t0, float t1, glm::vec3 c0, glm::vec3 c1) : center(cen), radius(r), material(mat) { time0 = t0; time1 = t1; center_t0 = c1; center_t1 = c1; };
	void setName(const char* name) { myname = name; }
	virtual bool hit(const Ray& r, float tmin, float tmax, hit_info &info) const;
	virtual bool bounding_box(float t0, float t1, AABB &box) const;
	virtual glm::vec3 getCenter(float time) const;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_info &info) const {
	glm::vec3 oc = r.get_origin() - getCenter(r.mTime);
	float a = glm::dot(r.get_direction(), r.get_direction());
	float b = glm::dot(oc, r.get_direction());
	float c = glm::dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp < t_max && temp > t_min) {
			info.t = temp;
			info.p = r.point_at_parameter(info.t);
			get_sphere_uv((info.p-center)/radius, info.u, info.v);
			info.normal = (info.p - getCenter(r.mTime)) / radius;
			info.mat_ptr = material;
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp < t_max && temp > t_min) {
			info.t = temp;
			info.p = r.point_at_parameter(info.t);
			get_sphere_uv((info.p - center) / radius, info.u, info.v);
			info.normal = (info.p - getCenter(r.mTime)) / radius;
			info.mat_ptr = material;
			return true;
		}
	}
	return false;
}
bool Sphere::bounding_box(float t0, float t1, AABB &box) const {
	AABB box1 = AABB(center_t0 - glm::vec3(radius, radius, radius), center_t0 + glm::vec3(radius, radius, radius));
	AABB box2 = AABB(center_t1 - glm::vec3(radius, radius, radius), center_t1 + glm::vec3(radius, radius, radius));
	box = surrounding_box(box1, box2);
	return true;
}
glm::vec3 Sphere::getCenter(float time) const {
	if (time0 == 0.0 && time1 == 0.0) {
		return center;
	}
	return center_t0 + ((time1 - time0) / (time1 - time0))*(center_t1 - center_t0);
}