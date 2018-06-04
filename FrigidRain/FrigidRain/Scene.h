#pragma once
#include "FR_Drawable.h"

class Scene : public FR_Drawable {
public:
	FR_Drawable * *list;
	int list_size;

	Scene() {}
	Scene(FR_Drawable **l, int n) { list = l; list_size = n; }
	virtual bool hit(const Ray& r, float tmin, float tmax, hit_info &info) const;
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