#include "FR_Drawable.h"
#include <iostream>

int box_x_compare(const void * a, const void * b) {
	AABB box_left, box_right;
	FR_Drawable *ah = *(FR_Drawable**)a;
	FR_Drawable *bh = *(FR_Drawable**)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right)) {
		std::cerr << "No bounding box in bvh_node compare x\n" << std::endl;
	}
	if (box_left.min().x - box_right.min().x < 0.0) {
		return -1;
	}
	else {
		return 1;
	}
}
int box_y_compare(const void * a, const void * b) {
	AABB box_left, box_right;
	FR_Drawable *ah = *(FR_Drawable**)a;
	FR_Drawable *bh = *(FR_Drawable**)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right)) {
		std::cerr << "No bounding box in bvh_node compare x\n" << std::endl;
	}
	if (box_left.min().y - box_right.min().y < 0.0) {
		return -1;
	}
	else {
		return 1;
	}
}
int box_z_compare(const void * a, const void * b) {
	AABB box_left, box_right;
	FR_Drawable *ah = *(FR_Drawable**)a;
	FR_Drawable *bh = *(FR_Drawable**)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right)) {
		std::cerr << "No bounding box in bvh_node compare x\n" << std::endl;
	}
	if (box_left.min().z - box_right.min().z < 0.0) {
		return -1;
	}
	else {
		return 1;
	}
}

class bvh_node : public FR_Drawable {
public:
	FR_Drawable * left;
	FR_Drawable * right;
	AABB box;

	bvh_node() {}
	bvh_node(FR_Drawable **l, int n, float time0, float time1);
	virtual bool hit(const Ray &r, float tmin, float tmax, hit_info &info) const;
	virtual bool bounding_box(float t0, float t1, AABB &box) const;
	virtual glm::vec3 getCenter(float time) const;
};

bvh_node::bvh_node(FR_Drawable **l, int n, float time0, float time1) {
	int axis = rand() % 3;
	if (axis == 0) {
		qsort(l, n, sizeof(FR_Drawable *), box_x_compare);
	}
	else if (axis == 1) {
		qsort(l, n, sizeof(FR_Drawable *), box_y_compare);
	}
	else if (axis == 2) {
		qsort(l, n, sizeof(FR_Drawable *), box_z_compare);
	}
	if (n == 1) {
		left = right = l[0];
	}
	else if (n == 2) {
		left = l[0];
		right = l[1];
	}
	else {
		left = new bvh_node(1, n / 2, time0, time1);
		right = new bvh_node(1 + n/2, n - (n / 2), time0, time1);
	}
	AABB box_left, box_right;
	if (!left->bounding_box(time0, time1, box_left) || right->bounding_box(time0, time1, box_right)) {
		std::cerr << "No bounding box in bvh_node constructor\n" << std::endl;
	}
	box = surrounding_box(box_left, box_right);
}

bool bvh_node::bounding_box(float t0, float t1, AABB &b) const {
	b = box;
	return true;
}

bool bvh_node::hit(const Ray &r, float t_min, float t_max, hit_info &info) const {
	if (box.hit(r, t_min, t_max)) {
		hit_info left_info, right_info;
		bool hit_left = left->hit(r, t_min, t_max, left_info);
		bool hit_right = right->hit(r, t_min, t_max, right_info);
		if (hit_left && hit_right) {
			if (left_info.t < right_info.t) {
				info = left_info;
			}
			else {
				info = right_info;
			}
			return true;
		}
		else if (hit_left) {
			info = left_info;
			return true;
		}
		else if (hit_right) {
			info = right_info;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}
glm::vec3 bvh_node::getCenter(float time) const {
	return glm::vec3(0, 0, 0);
}