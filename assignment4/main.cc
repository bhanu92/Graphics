//
//	This ray tracer is based on work by Peter Shirley, one of the authors of
// the text book Fundamentals of Computer Graphics.
//

#include "texture.h"
#include "camera.h"
#include "cstdio"
#include "float.h"
#include "hitable_list.h"
#include "material.h"
#include "sphere.h"
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;

int nx, ny, ns;
vec3 cameraPos, center, viewup;

hitable* parseScene(string sceneFile) {
	// cout << "Scene File Name: " << sceneFile << endl;

	vector<hitable*> list;
	vector<string> tokens;
	string token;

	ifstream file;
	file.open(sceneFile);
	if (!file.is_open()) {
		cerr << "Cannot open Scene file" << endl;
		exit(EXIT_FAILURE);
	}

	while (file >> token) tokens.push_back(token);

	// Adding NULL values at the end of the vector
	// As the scenefiles are of varying size
	for (int i = 0; i < 8; i++) tokens.push_back("");

	nx = stoi(tokens.at(1));
	ny = stoi(tokens.at(3));
	ns = stoi(tokens.at(5));
	// cout << "nx " << nx << endl;
	// cout << "ny " << ny << endl;
	// cout << "ns " << ns << endl;

	cameraPos = vec3(stof(tokens.at(7)), stof(tokens.at(8)), stof(tokens.at(9)));
	center = vec3(stof(tokens.at(11)), stof(tokens.at(12)), stof(tokens.at(13)));
	viewup = vec3(stof(tokens.at(15)), stof(tokens.at(16)), stof(tokens.at(17)));

	// cout << "cameraPos " << stof(tokens.at(7)) << stof(tokens.at(8)) << stof(tokens.at(9)) << endl;
	// cout << "center " << stof(tokens.at(11)) << stof(tokens.at(12)) << stof(tokens.at(13)) << endl;
	// cout << "viewup " << stof(tokens.at(15)) << stof(tokens.at(16)) << stof(tokens.at(17)) << endl;

	for (int i = 18; i < tokens.size(); i++) {
		if (tokens.at(i).compare("sphere") == 0) {
			vec3 circleCenter =
			    vec3(stof(tokens.at(i + 2)), stof(tokens.at(i + 3)), stof(tokens.at(i + 4)));
			float circleRadius = stof(tokens.at(i + 6));

			// cout << "circleCenter " << stof(tokens.at(i + 2)) << stof(tokens.at(i + 3))
			//<< stof(tokens.at(i + 4)) << endl;
			// cout << "circleRadius " << stof(tokens.at(i + 6));

			if (tokens.at(i + 8).compare("lambertian") == 0) {
				if (tokens.at(i + 9).compare("albedo") == 0) {
					vec3 albedo = vec3(stof(tokens.at(i + 10)), stof(tokens.at(i + 11)),
					                   stof(tokens.at(i + 12)));
					// cout << "albedo " << stof(tokens.at(i + 10)) << stof(tokens.at(i + 11))
					//   << stof(tokens.at(i + 12)) << endl;
					list.push_back(new sphere(circleCenter, circleRadius,
					                          new lambertian(new constant_texture(albedo))));
				}
				else if (tokens.at(i + 9).compare("checker_texture") == 0) {
					vec3 texture1 = vec3(stof(tokens.at(i + 10)), stof(tokens.at(i + 11)),
					                     stof(tokens.at(i + 12)));
					vec3 texture2 = vec3(stof(tokens.at(i + 13)), stof(tokens.at(i + 14)),
					                     stof(tokens.at(i + 15)));
					texture* checker = new checker_texture(new constant_texture(texture1),
					                                       new constant_texture(texture2));
					list.push_back(new sphere(circleCenter, circleRadius, new lambertian(checker)));
				}
			}

			else if (tokens.at(i + 8).compare("metal") == 0) {
				vec3 albedo =
				    vec3(stof(tokens.at(i + 10)), stof(tokens.at(i + 11)), stof(tokens.at(i + 12)));
				//	cout << "albedo " << stof(tokens.at(i + 1)) << stof(tokens.at(i + 11))
				//     << stof(tokens.at(i + 12)) << endl;
				// cout << "Fuzz " << tokens.at(i + 14) << endl;
				float fuzz = stof(tokens.at(i + 14));
				list.push_back(new sphere(circleCenter, circleRadius, new metal(albedo, fuzz)));
			}

			else if (tokens.at(i + 8).compare("dielectric") == 0) {
				float ior = stof(tokens.at(i + 10));
				// cout << "ior " << stof(tokens.at(i + 10)) << endl;
				list.push_back(new sphere(circleCenter, circleRadius, new dielectric(ior)));
			}
		}
	}

	hitable** array = new hitable*[list.size()];
	for (int i = 0; i < list.size(); i++) {
		array[i] = list[i];
	}

	return new hitable_list(array, list.size());
}


vec3 color(const ray& r, hitable* world, int depth) {
	hit_record rec;

#ifdef WIN32
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
#else
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
#endif
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}
/*
hitable* random_scene() {
        int n = 500;
        hitable** list = new hitable*[n + 1];
        list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));  // scene's base sphere
        int i = 1;
        for (int a = -11; a < 11; a++) {
                for (int b = -11; b < 11; b++) {
                        float choose_mat = drand48();
                        vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
                        if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                                if (choose_mat < 0.8) {  // diffuse
                                        list[i++] =
                                            new sphere(center, 0.2,
                                                       new lambertian(vec3(drand48() * drand48(), drand48() * drand48(),
                                                                           drand48() * drand48())));
                                }
                                else if (choose_mat < 0.95) {  // metal
                                        list[i++] = new sphere(
                                            center, 0.2, new metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()),
                                                                        0.5 * (1 + drand48())),
                                                                   0.5 * drand48()));
                                }
                                else {  // glass
                                        list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                                }
                        }
                }
        }

        list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
        list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
        list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

        return new hitable_list(list, i);
}
*/
int main(int argc, char** argv) {
	if (argc != 2) {
		cerr << "Wrong set of arguments" << endl;
		exit(EXIT_FAILURE);
	}

	string fileName = argv[1];
	hitable* world = parseScene(fileName);
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";  // header for PPM file
	/*
	hitable* list[5];
	float R = cos(M_PI / 4);
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
	// hitable *world = new hitable_list(list, 5);
	// hitable* world = parseScene(fileName);
	*/
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;

	camera cam(cameraPos, center, viewup, 20, float(nx) / float(ny), aperture, dist_to_focus);

	for (int j = ny - 1; j >= 0; j--) {  // for each line, top to bottom
		if (j % 10 == 0) fprintf(stderr, "On line: %d\n", j);  // informational print
		for (int i = 0; i < nx; i++) {  // for each pixel on the line
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {  // samples within a pixel
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			col /= float(ns);  // adjust for number of samples
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}
