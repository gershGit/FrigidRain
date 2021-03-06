#pragma once
#include "glm\glm.hpp"

class Ray
{
public:
	glm::vec3 origin;
	glm::vec3 direction;
	float mTime;

	Ray();
	Ray(const glm::vec3& a, const glm::vec3& b, float ti = 0.0) { origin = a; direction = b; mTime = ti; }
	glm::vec3 get_origin() const { return origin; }
	glm::vec3 get_direction() const { return direction;  }
	void set_origin(glm::vec3 vec_in) { origin = vec_in; }
	void set_direction(glm::vec3 vec_in) { direction = vec_in; }
	glm::vec3 point_at_parameter(float t) const { return origin + t * direction; }
	~Ray();
};

