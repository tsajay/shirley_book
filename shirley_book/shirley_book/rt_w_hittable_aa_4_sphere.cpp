#include <iostream>
#include "hitable_sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
#include <limits>


vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, std::numeric_limits<float>::max(), rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation*color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_dir = unit_vector(r.direction());
		float t = 0.5 * (unit_dir.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main(int argc, char **argv) {
	int nx = 200;
	int ny = 100;
	int ns = 100;
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	hitable *list[5];
	
	list[0] = new sphere(vec3(0.0, 0.0, -1.0), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0.0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1.0, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
	list[3] = new sphere(vec3(-1.0, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1.0, 0, -1), -0.45, new dielectric(1.5));

	hitable *world = new hitable_list(list, 5);
	camera cam;
	//camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist)
	//camera cam(origin, sphere0_center, vertical, );
	for (int j = ny - 1; j >= 0; --j) {
		for (int i = 0; i < nx; ++i) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; ++s) {
				float u = float(i + rand_0_1()) / float(nx);
				float v = float(j + rand_0_1()) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			col /= float(ns);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}
