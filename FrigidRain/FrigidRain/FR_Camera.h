#pragma once
#include "glm\glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include <vector>
#include "Ray.h"
#include "bitmap_image.hpp"

enum cameraType {ORTHOGRAPHIC, PERSPECTIVE};

class FR_Camera {
public:
	float fov = 45;

	float x_persp = 2;
	float y_persp = 1;
	float x_orth = 5;
	float y_orth = 5;

	int x_dim = 200;
	int y_dim = 100;
	glm::vec3 position;
	glm::vec3 eulerRotations;
	std::vector<std::vector<Ray>> rays;
	cameraType myType;

	bitmap_image image;

	FR_Camera(cameraType type, int x_dim_in = 200, int y_dim_in = 100) {
		myType = type;
		x_dim = x_dim_in; 
		y_dim = y_dim_in; 
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

	float radians(float degrees) {
		return degrees * 0.01745329252;
	}

	float getOrthRatio() {
		return x_orth / y_orth;
	}

	float getImageRatio() {
		return x_dim / y_dim;
	}

	glm::vec3 forward() {
		glm::quat q = glm::quat(eulerRotations);
		glm::vec3 forwardVec(2 * (q.x*q.z + q.w*q.y), 2 * (q.y*q.z - q.w*q.x), 1 - 2 * (q.x*q.x + q.y*q.y));

		return glm::normalize(-forwardVec);
	};

	Ray get_ray(float u, float v) { return Ray(position, glm::normalize(glm::vec3(-x_persp, -y_persp, -1) + glm::vec3(u * (2 * x_persp), 0, 0) + glm::vec3(0, v * (2 * y_persp), 0))); }
};