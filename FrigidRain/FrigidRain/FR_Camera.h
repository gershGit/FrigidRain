#pragma once
#include "glm\glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include <vector>
#include "Ray.h"
#include "bitmap_image.hpp"

enum cameraType {ORTHOGRAPHIC, PERSPECTIVE};
glm::vec3 random_in_unit_disk() {
	return 2.0f*glm::vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 0) - glm::vec3(1, 1, 0);
}
float rand_float() {
	return rand() / (RAND_MAX + 1.);
}

class FR_Camera {
public:
	float fov = 45;
	float lens_radius;

	float x_persp = 2;
	float y_persp = 1;
	float x_orth = 5;
	float y_orth = 5;
	glm::vec3 referenceVec;
	glm::vec3 horizontalVec;
	glm::vec3 verticalVec;
	glm::vec3 u, v, w;

	float time0, time1;

	int x_dim = 200;
	int y_dim = 100;
	glm::vec3 position;
	glm::vec3 eulerRotations;
	std::vector<std::vector<Ray>> rays;
	cameraType myType;

	bitmap_image image;

	FR_Camera(cameraType type, float in_fov, float lens_rad_in = 0.1f,  int x_dim_in = 200, int y_dim_in = 100) {
		myType = type;
		x_dim = x_dim_in;
		y_dim = y_dim_in;
		fov = in_fov;
		lens_radius = lens_rad_in/2.0f;
		time0 = 0.0;
		time1 = 0.0;

		float theta = radians(fov);
		float half_height = tan(theta / 2);
		float half_width = getImageRatio() * half_height;
		w = -forward();
		glm::vec3 vup = glm::vec3(0,1,0);
		u = glm::normalize(glm::cross(vup, w));
		v = glm::cross(w, u);
		referenceVec = glm::vec3(-half_width, -half_height, -1.0);
		referenceVec = position - half_width * u - half_height * v - w;
		horizontalVec = 2 * half_width*u;
		verticalVec = 2 * half_height*v;

		position = glm::vec3(0, 0, 0);
		eulerRotations = glm::vec3(0, 0, 0);
		image = bitmap_image(x_dim_in, y_dim_in);
	}

	void generateRays() {
		rays.resize(y_dim);
		for (int i = 0; i < y_dim; i++) {
			rays[i].resize(x_dim);
		}

		if (myType == ORTHOGRAPHIC) {
			float left = -x_orth * getOrthRatio();
			float right = x_orth * getOrthRatio();
			float top = y_orth;
			float bottom = -y_orth;

			for (int y = 0; y < y_dim; y++) {
				for (int x = 0; x < x_dim; x++) {
					Ray temp_ray = Ray();
					float tempx = left + ((right-left)*(x+0.5)) / x_dim;
					float tempy = top + ((bottom - top)*(y + 0.5)) / y_dim;
					temp_ray.origin = glm::vec3(tempx, tempy, 0) + position;
					temp_ray.direction = forward();
					rays[y][x] = temp_ray;
				}
			}
		}
		else if (myType == PERSPECTIVE) {
			for (int j = y_dim - 1; j >= 0; j--) {
				for (int i = 0; i < x_dim; i++) {
					float u = float(i) / float(x_dim);
					float v = float(j) / float(y_dim);
					Ray r = Ray();
					r.origin = glm::vec3(0, 0, 0);
					r.direction = glm::normalize(glm::vec3(-x_persp, -y_persp, -1) + glm::vec3(u * (2 * x_persp), 0, 0) + glm::vec3(0, v * (2 * y_persp),0));
					rays[j][i] = r;
				}
			}
		}
	}
	void setCameraLookAt(glm::vec3 lookAt, float t0 = 0.0f, float t1 = 0.0f) {
		time0 = t0;
		time1 = t1;
		float theta = radians(fov);
		float half_height = tan(theta / 2);
		float half_width = getImageRatio() * half_height;
		w = glm::normalize(position - lookAt);
		glm::vec3 vup = glm::vec3(0, 1, 0);
		u = glm::normalize(glm::cross(vup, w));
		v = glm::cross(w, u);
		float focus_dist = (position - lookAt).length();
		referenceVec = position - half_width * focus_dist*u - half_height * focus_dist*v - focus_dist * w;
		horizontalVec = (2 * half_width * focus_dist) *u ;
		verticalVec = (2 * half_height * focus_dist) *v;
	}
	void setTime(float t0, float t1) {
		time0 = t0;
		time1 = t1;
	}
	void setAperature(float aperature) {
		lens_radius = aperature / 2;
	}

	float radians(float degrees) {
		return degrees * 0.01745329252;
	}
	glm::vec3 radians(glm::vec3 degrees_vec) {
		return glm::vec3(radians(degrees_vec.x), radians(degrees_vec.y), radians(degrees_vec.z));
	}

	float getOrthRatio() {
		return x_orth / y_orth;
	}

	float getImageRatio() {
		return x_dim / y_dim;
	}

	glm::vec3 forward() {
		glm::quat q = glm::quat(radians(eulerRotations));
		glm::vec3 forwardVec(2 * (q.x*q.z + q.w*q.y), 2 * (q.y*q.z - q.w*q.x), 1 - 2 * (q.x*q.x + q.y*q.y));

		return glm::normalize(-forwardVec);
	};
	glm::vec3 up() {
		glm::quat q = glm::quat(eulerRotations);
		glm::vec3 upVec(2 * (q.x*q.y - q.w*q.z), 1 - 2 * (q.x*q.x + q.z*q.z), 2 * (q.y*q.z + q.w*q.x));

		return glm::normalize(upVec);
	};
	glm::vec3 right() {
		glm::quat q = glm::quat(eulerRotations);
		glm::vec3 rightVec(1 - 2 * (q.y*q.y + q.z*q.z), 2 * (q.x*q.y + q.w*q.z), 2 * (q.x*q.z - q.w*q.y));

		return glm::normalize(-rightVec);
	};

	Ray get_ray(float s, float t) {
		glm::vec3 rd = lens_radius * random_in_unit_disk();
		glm::vec3 offset = u * rd.x + v * rd.y;
		float time = time0 + rand_float()*(time1 - time0);
		return Ray(position + offset, glm::normalize(referenceVec + s*horizontalVec + t*verticalVec - position - offset), time); 
	}
};