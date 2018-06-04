#pragma once
#define _SCL_SECURE_NO_WARNINGS
#include "stb_image.h"
#include "bitmap_image.hpp"
#include "glm/glm.hpp"
#include "Ray.h"
#include "Sphere.h"
#include "FR_Camera.h"
#include "Scene.h"
#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>
#include "FR_Material.h"
#include "Lambertian.h"
#include "metallic.h"

glm::vec3 getBackgroundColor(const Ray& r) {
	float t = 0.5*(glm::normalize(r.get_direction()).y + 1.0f);
	return (1.0f - t) * glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0);
}

glm::vec3 getRayColor(const Ray& r, FR_Drawable *scene, int depth) {
	hit_info info;
	if (scene->hit(r, 0.001, FLT_MAX, info)) {
		Ray scattered;
		glm::vec3 attenuation;
		if (depth < 5 && info.mat_ptr->scatter(r, info, attenuation, scattered)) {
			return attenuation * getRayColor(scattered, scene, depth + 1);
		}
		else {
			return glm::vec3(0);
		}
	}
	else {
		return getBackgroundColor(r);
	}
}

int render(FR_Camera &camera, FR_Drawable *scene, int samples) {
	srand(time(NULL));
	int lastPercentage = 0;
	for (int y = 0; y < camera.rays.size(); y++) {

		float decimal = ((float)y / (float)camera.rays.size()) * 100;
		int percentage = (int)decimal;
		if (percentage > 0 && percentage != lastPercentage) {
			lastPercentage = percentage;
			std::cout << "\t" << percentage << "%" << std::endl;
		}

		for (int x = 0; x < camera.rays[0].size(); x++) {
			glm::vec3 col(0, 0, 0);
			for (int s = 0; s < samples; s++) {
				float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float u = float(x + r1) / float(camera.x_dim);
				float v = float(y + r1) / float(camera.y_dim);
				Ray r = camera.get_ray(u, v);
				glm::vec3 p = r.point_at_parameter(2.0);
				col += getRayColor(r, scene, 0);
			}
			col *= float(1 / float(samples));
			col = glm::vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));
			rgb_t rgb_color = make_colour(col.x*255, col.y*255, col.z*255);
			camera.image.set_pixel(x, camera.rays.size() - (y+1) , rgb_color);
		}
	}
	return 0;
}

int main(int argc, char** argv) {
	FR_Camera mainCam = FR_Camera(PERSPECTIVE, 800, 400);
	mainCam.generateRays();

	FR_Drawable *list[4];
	list[0] = new Sphere(glm::vec3(0, 0, -1), 0.5f, new Lambertian(glm::vec3(0.8f,0.3f,0.3f)));
	list[1] = new Sphere(glm::vec3(0, -100.5, -1), 100, new Lambertian(glm::vec3(0.8f, 0.8f, 0.0f)));
	list[2] = new Sphere(glm::vec3(1, 0, -1), 0.5f, new Metallic(glm::vec3(0.8f, 0.6f, 0.2f), 0.9f));
	list[3] = new Sphere(glm::vec3(-1, 0, -1), 0.5f, new Metallic(glm::vec3(0.8f, 0.8f, 0.8f), 0.1f));
	Scene *world = new Scene(list, 4);

	std::cout << "Rendering..." << std::endl;
	render(mainCam, world, 64);
	std::cout << "Finished" << std::endl;

	rgb_t testCol = make_colour(255, 120, 255);
	//mainCam.image.set_pixel(50, 50, testCol);

	mainCam.image.save_image("test.bmp");
}