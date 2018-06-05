#pragma once
#include "FR_Drawable.h"

class Scene : public FR_Drawable {
public:
	FR_Drawable * *list;
	int list_size;

	Scene() {}
	Scene(FR_Drawable **l, int n) { list = l; list_size = n; }
	virtual bool hit(const Ray& r, float tmin, float tmax, hit_info &info) const;
	virtual bool bounding_box(float t0, float t1, AABB &box) const;
	virtual glm::vec3 getCenter(float time) const;
};

bool Scene::hit(const Ray &r, float t_min, float t_max, hit_info &info) const {
	hit_info temp_info;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (int i = 0; i < list_size; i++) {
		if (list[i]->hit(r, t_min, closest_so_far, temp_info)) {
			hit_anything = true;
			closest_so_far = temp_info.t;
			info = temp_info;
		}
	}
	return hit_anything;
}
bool Scene::bounding_box(float t0, float t1, AABB &box) const {
	if (list_size < 1) {
		return false;
	}
	AABB temp_box;
	bool first_true = list[0]->bounding_box(t0, t1, temp_box);
	if (!first_true) {
		return false;
	}
	else {
		box = temp_box;
	}
	for (int i = 1; i < list_size; i++) {
		if (list[i]->bounding_box(t0, t1, temp_box)) {
			box = surrounding_box(box, temp_box);
		}
		else {
			return false;
		}
	}
	return true;
}
glm::vec3 Scene::getCenter(float time) const {
	return glm::vec3(0, 0, 0);
}