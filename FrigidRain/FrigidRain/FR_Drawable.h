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
	virtual glm::vec3 getCenter(float time) const { return glm::vec3(0, 0, 0); }
};

class xy_rect : public FR_Drawable {
public:
	float x0, x1, y0, y1, k;
	FR_Material *material;

	xy_rect() {}
	xy_rect (float _x0, float _x1, float _y0, float _y1, float _k, FR_Material *mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), material(mat)  {}
	virtual bool hit(const Ray &r, float t0, float t1, hit_info &info) const {
		float t = (k - r.get_origin().z) / r.get_direction().z;
		if (t < t0 || t > t1) {
			return false;
		}
		float x = r.get_origin().x + t * r.get_direction().x;
		float y = r.get_origin().y + t * r.get_direction().y;
		if (x<x0 || x>x1 || y<y0 || y>y1) {
			return false;
		}
		info.u = (x - x0) / (x1 - x0);
		info.v = (y - y0) / (y1 - y0);
		info.t = t;
		info.mat_ptr = material;
		info.p = r.point_at_parameter(t);
		info.normal = glm::vec3(0, 0, 1);
		return true;
	}
	virtual bool bounding_box(float t0, float t1, AABB &box) const {
		box = AABB(glm::vec3(x0, y0, k - 0.0001), glm::vec3(x1, y1, k + 0.0001));
		return true;
	}
};
class xz_rect : public FR_Drawable {
public:
	float x0, x1, z0, z1, k;
	FR_Material *material;

	xz_rect() {}
	xz_rect(float _x0, float _x1, float _z0, float _z1, float _k, FR_Material *mat) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), material(mat) {}
	virtual bool hit(const Ray &r, float t0, float t1, hit_info &info) const {
		float t = (k - r.get_origin().y) / r.get_direction().y;
		if (t < t0 || t > t1) {
			return false;
		}
		float x = r.get_origin().x + t * r.get_direction().x;
		float z = r.get_origin().z + t * r.get_direction().z;
		if (x<x0 || x>x1 || z<z0 || z>z1) {
			return false;
		}
		info.u = (x - x0) / (x1 - x0);
		info.v = (z - z0) / (z1 - z0);
		info.t = t;
		info.mat_ptr = material;
		info.p = r.point_at_parameter(t);
		info.normal = glm::vec3(0, 0, 1);
		return true;
	}
	virtual bool bounding_box(float t0, float t1, AABB &box) const {
		box = AABB(glm::vec3(x0, z0, k - 0.0001), glm::vec3(x1, z1, k + 0.0001));
		return true;
	}
};
class yz_rect : public FR_Drawable {
public:
	float y0, y1, z0, z1, k;
	FR_Material *material;

	yz_rect() {}
	yz_rect(float _y0, float _y1, float _z0, float _z1, float _k, FR_Material *mat) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), material(mat) {}
	virtual bool hit(const Ray &r, float t0, float t1, hit_info &info) const {
		float t = (k - r.get_origin().x) / r.get_direction().x;
		if (t < t0 || t > t1) {
			return false;
		}
		float y = r.get_origin().y + t * r.get_direction().y;
		float z = r.get_origin().z + t * r.get_direction().z;
		if (y<y0 || y>y1 || z<z0 || z>z1) {
			return false;
		}
		info.u = (y - y0) / (y1 - y0);
		info.v = (z - z0) / (z1 - z0);
		info.t = t;
		info.mat_ptr = material;
		info.p = r.point_at_parameter(t);
		info.normal = glm::vec3(0, 0, 1);
		return true;
	}
	virtual bool bounding_box(float t0, float t1, AABB &box) const {
		box = AABB(glm::vec3(y0, z0, k - 0.0001), glm::vec3(y1, z1, k + 0.0001));
		return true;
	}
};