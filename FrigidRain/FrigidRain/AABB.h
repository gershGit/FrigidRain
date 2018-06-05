#include "glm\glm.hpp"
#include "Ray.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class AABB {
public:
	glm::vec3 _min;
	glm::vec3 _max;

	AABB() {}
	AABB(const glm::vec3 &a, const glm::vec3 &b) { _min = a; _max = b; }

	glm::vec3 min() const { return _min; }
	glm::vec3 max() const { return _max; }

	bool hit(const Ray &r, float tmin, float tmax) const {
		for (int a = 0; a < 3; a++) {
			float invD = 1.0f / r.get_direction()[a];
			float t0 = (_min[a] - r.get_origin()[a] * invD);
			float t1 = (_max[a] - r.get_origin()[a] * invD);
			if (invD < 0.0f) {
				std::swap(t0, t1);
			}
			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;
			if (tmax <= tmin) {
				return false;
			}
			return true;
		}
	}
};