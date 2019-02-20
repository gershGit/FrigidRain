#pragma once
#define _SCL_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
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
#include "Glass.h"
#include <iomanip>

FR_Drawable* generateRandomScene(int n) {
	int nx, ny, nn;
	unsigned char*tex_data = stbi_load("small.jpg", &nx, &ny, &nn, 0);
	FR_Material *mat = new Lambertian(new image_texture(tex_data, nx, ny));

	texture *checker = new checker_texture(new constant_texture(glm::vec3(0.2, 0.3, .1)), new constant_texture(glm::vec3(0.9, 0.9, 0.9)));

	FR_Drawable **list = new FR_Drawable*[n + 1];
	list[0] = new Sphere(glm::vec3(0, -1000, 0), 1000, new Lambertian(checker), 0, 0, glm::vec3(0, -1000, 0), glm::vec3(0, -1000, 0));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_set = rand_float();
			glm::vec3 center(a + 0.9*rand_float(), 0.2, b + 0.9*rand_float());
			if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9f) {
				if (choose_set < 0.8f) { //select diffuse sphere
					list[i++] = new Sphere(center, 0.2, new Lambertian(new constant_texture(glm::vec3(rand_float(), rand_float(), rand_float()) )), 0, 1, center, center + glm::vec3(0, rand_float()*0.5,0));
				}
				else if (choose_set < 0.95f) { //Metallic
					list[i++] = new Sphere(center, 0.2, new Metallic(new constant_texture (glm::vec3(0.5*(1 + rand_float()), 0.5*(1 + rand_float()), 0.5*(1 + rand_float()))), 0.5*rand_float()), 0, 0, center, center);
				}
				else { //Glass
					list[i++] = new Sphere(center, 0.2, new Glass(1.5), 0, 0, center, center);
				}
			}
		}
	}
	list[i++] = new Sphere(glm::vec3(0, 1, 0), 1.0, new Glass(1.5f), 0, 0, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	list[i++] = new Sphere(glm::vec3(-4, 1, 0), 1.0, new Lambertian(new constant_texture( glm::vec3(0.4, 0.2, 0.1))), 0, 0, glm::vec3(-4, 1, 0), glm::vec3(-4, 1, 0));
	list[i++] = new Sphere(glm::vec3(4, 1, 0), 1.0, new Metallic(new constant_texture(glm::vec3(0.7, 0.6, 0.5)), 0.0), 0, 0, glm::vec3(4, 1, 0), glm::vec3(4, 1, 0));
	const char* thename = "TEXTURED";
	Sphere* mSphere = new Sphere(glm::vec3(8, 1, 0), 1.0, mat, 0, 0, glm::vec3(4, 1, 0), glm::vec3(4, 1, 0));
	mSphere->setName(thename);
	list[i++] = mSphere;
	list[i++] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(glm::vec3(4, 4, 4))));

	return new Scene(list, i);
}

FR_Drawable* cornell_box() {
	FR_Drawable **list = new FR_Drawable*[7];
	int i = 0;
	FR_Material *red = new Lambertian(new constant_texture(glm::vec3(0.65, 0.05, 0.05)));
	FR_Material *white = new Lambertian(new constant_texture(glm::vec3(0.73, 0.73, 0.73)));
	FR_Material *green = new Lambertian(new constant_texture(glm::vec3(0.12, 0.45, 0.15)));
	FR_Material *light = new Lambertian(new constant_texture(glm::vec3(15, 15, 15)));
	list[i++] = new yz_rect(0, 555, 0, 555, 555, green);
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new yz_rect(213, 343, 227, 332, 554, light);
	list[i++] = new yz_rect(0, 555, 0, 555, 555, white);
	list[i++] = new yz_rect(0, 555, 0, 555, 555, white);
	list[i++] = new Sphere(glm::vec3(278, 278, 0), 1, red, 0, 0, glm::vec3(278, 278, 0), glm::vec3(278, 278, 0));
	return new Scene(list, i);
}

glm::vec3 getBackgroundColor(const Ray& r) {
	float t = 0.5*(glm::normalize(r.get_direction()).y + 1.0f);
	return (1.0f - t) * glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0);
	//return glm::vec3(0, 0, 0);
}

glm::vec3 getRayColor(const Ray& r, FR_Drawable *scene, int depth) {
	hit_info info;
	if (scene->hit(r, 0.001, FLT_MAX, info)) {
		Ray scattered;
		glm::vec3 attenuation;
		glm::vec3 emitted = info.mat_ptr->emitted(info.u, info.v, info.p);
		if (depth < 5 && info.mat_ptr->scatter(r, info, attenuation, scattered)) {
			return emitted + attenuation * getRayColor(scattered, scene, depth + 1);
		}
		else {
			return emitted;
		}
	}
	else {
		return getBackgroundColor(r);
	}
}

int render(FR_Camera &camera, FR_Drawable *scene, int samples) {
	clock_t begin = clock();
	clock_t current;
	int lastPercentage = 0;
	for (int y = 0; y < camera.rays.size(); y++) {
		double elapsed = double(clock() - begin) / CLOCKS_PER_SEC;
		float decimal = ((float)y / (float)camera.rays.size()) * 100;
		int percentage = (int)decimal;
		if (percentage > 0 && percentage != lastPercentage) {
			lastPercentage = percentage;
			int minutes = elapsed / 60;
			int seconds = (int) elapsed % 60;
			float remaining = (elapsed / ((float)percentage / 100)) - elapsed;
			int remainingMinutes = remaining / 60;
			int remainingSeconds =(int) remaining % 60;
			std::cout << "\t" << percentage << "%" << " \tElapsed: " << minutes << ":" << std::setfill('0') << std::setw(2) << seconds << "\t\tRemaining: " << remainingMinutes << ":" << std::setfill('0') << std::setw(2) << remainingSeconds << std::endl;
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
	srand(time(NULL));

	FR_Camera mainCam = FR_Camera(PERSPECTIVE, 40, 0.01, 200, 150);
	mainCam.position = glm::vec3(13, 2, 3);
	mainCam.eulerRotations = glm::vec3(-45,-30,0);
	mainCam.setCameraLookAt(glm::vec3(0, 0, 0));
	mainCam.setAperature(0.0);
	mainCam.setTime(0.0, 1.0);
	mainCam.generateRays();

	
	FR_Drawable *list[6];
	list[0] = new Sphere(glm::vec3(0, 0, -1), 0.5f, new Lambertian(new constant_texture(glm::vec3(0.8f,0.3f,0.3f))), 0, 0, glm::vec3(0, 0, -1), glm::vec3(0, 0, -1));
	list[1] = new Sphere(glm::vec3(0, -100.5, -1), 100, new Lambertian(new constant_texture(glm::vec3(0.8f, 0.8f, 0.0f))), 0, 0, glm::vec3(0,-100.5, -1), glm::vec3(0, -100.5, -1));
	list[2] = new Sphere(glm::vec3(1, 0, -1), 0.5f, new Metallic(new constant_texture(glm::vec3(0.8f, 0.6f, 0.2f)), 0.1f), 0, 0, glm::vec3(1, 0, -1), glm::vec3(1, 0, -1));
	list[3] = new Sphere(glm::vec3(-1, 0, -1), 0.5f, new Glass(1.5f), 0, 0, glm::vec3(-1, 0, -1), glm::vec3(-1, 0, -1));
	list[4] = new Sphere(glm::vec3(-1, 0, -1), -0.45f, new Glass(1.5f), 0, 0, glm::vec3(-1, 0, -1), glm::vec3(-1, 0, -1));
	list[5] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(glm::vec3(4, 4, 4))));
	FR_Drawable *world = new Scene(list, 6);
	world = generateRandomScene(500);
	//world = cornell_box();

	std::cout << "Rendering..." << std::endl;
	render(mainCam, world, 16);
	std::cout << "Finished" << std::endl;

	rgb_t testCol = make_colour(255, 120, 255);
	//mainCam.image.set_pixel(50, 50, testCol);

	mainCam.image.save_image("book2.bmp");
}