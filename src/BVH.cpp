#include "BVH.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <iostream>

void BVH::LoadModel(char* name)
{
	// Taken from Shinjiro Sueda with slight modification
	std::string meshName(name);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if (!rc) {
		std::cout << "WARNING NOTHING FOUND";
	}
	else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				std::vector<glm::vec3> vecs;
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

					vecs.push_back({ attrib.vertices[3 * idx.vertex_index + 0], attrib.vertices[3 * idx.vertex_index + 1], attrib.vertices[3 * idx.vertex_index + 2] });

					if (!attrib.normals.empty()) {
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 0]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 1]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 2]);
						vecs.push_back({ attrib.normals[3 * idx.normal_index + 0], attrib.normals[3 * idx.normal_index + 1], attrib.normals[3 * idx.normal_index + 2]});
					}
					else {
						vecs.push_back(glm::vec3(0.0f));
					}
				}
				Triangle* t = new Triangle(vecs[0], vecs[2], vecs[4], vecs[1], vecs[3], vecs[5]);
				triangles.push_back(t);
				MinMaxBox(t);
				index_offset += fv;
				//per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
}

BVH::BVH() {
}

BVH::~BVH() {
	for (int i = 0; i < triangles.size(); i++) {
		delete triangles[i];
	}
}

void BVH::MinMaxBox(Triangle* tri) {
	auto vert1 = tri->giveVert1();
	auto vert2 = tri->giveVert2();
	auto vert3 = tri->giveVert3();

	X_min = vert1.x < X_min ? vert1.x : X_min;
	X_min = vert2.x < X_min ? vert2.x : X_min;
	X_min = vert3.x < X_min ? vert3.x : X_min;

	X_max = vert1.x > X_max ? vert1.x : X_max;
	X_max = vert2.x > X_max ? vert2.x : X_max;
	X_max = vert3.x > X_max ? vert3.x : X_max;

	Y_min = vert1.y < Y_min ? vert1.y : Y_min;
	Y_min = vert2.y < Y_min ? vert2.y : Y_min;
	Y_min = vert3.y < Y_min ? vert3.y : Y_min;

	Y_max = vert1.y > Y_max ? vert1.y : Y_max;
	Y_max = vert2.y > Y_max ? vert2.y : Y_max;
	Y_max = vert3.y > Y_max ? vert3.y : Y_max;

	Z_min = vert1.z < Z_min ? vert1.z : Z_min;
	Z_min = vert2.z < Z_min ? vert2.z : Z_min;
	Z_min = vert3.z < Z_min ? vert3.z : Z_min;

	Z_max = vert1.z > Z_max ? vert1.z : Z_max;
	Z_max = vert2.z > Z_max ? vert2.z : Z_max;
	Z_max = vert3.z > Z_max ? vert3.z : Z_max;

}

bool BVH::HitsBox(glm::vec3 o, glm::vec3 d) {
	float dx = 1.0f / d.x;
	float dy = 1.0f / d.y;
	float dz = 1.0f / d.z;

	float xmin = (boxMin.x - o.x) * dx;
	float xmax = (boxMax.x - o.x) * dx;

	float x_min = (xmin < xmax) ? xmin : xmax;
	float x_max = (xmin < xmax) ? xmax : xmin;

	float ymin = (boxMin.y - o.y) * dy;
	float ymax = (boxMax.y - o.y) * dy;

	float y_min = (ymin < ymax) ? ymin : ymax;
	float y_max = (ymin < ymax) ? ymax : ymin;

	float xy_max = (x_min < y_min) ? y_min : x_min;
	float xy_min = (x_max < y_max) ? x_max : y_max;

	float zmin = (boxMin.z - o.z) * dz;
	float zmax = (boxMax.z - o.z) * dz;

	float z_min = (zmin < zmax) ? zmin : zmax;
	float z_max = (zmin < zmax) ? zmax : zmin;

	float max = (z_min > xy_max) ? z_min : xy_max;
	float min = (z_max < xy_min) ? z_max : xy_min;

	if (max < 0) {
		return false;
	}
	if (min < max) {
		return false;
	}
	return true;

}

bool BVH::intersection(glm::vec3 o, glm::vec3 d, float t0, float t1, glm::vec3& t, material& rec, float& tf, glm::vec3& norm) {

	if (!HitsBox(o, d)) {
		return false;
	}

	float ts = FLT_MAX;
	glm::vec3 t_vec;
	glm::vec3 n;
	material temp;

	bool hit = false;
	for (int i = 0; i < triangles.size(); i++) { 
		float tc = FLT_MAX;
		glm::vec3 canV;
		glm::vec3 canN;
		if (triangles[i]->intersection(o, d, t0, t1, canV, temp, tc, canN)) {
			hit = true;
			if (tc < ts) {
				t_vec = canV;
				n = canN;
				ts = tc;
			}
		};
	}

	if (hit == false) {
		return false;
	}

	tf = ts;
	t = t_vec;
	norm = n;
	rec = mat;
	return true;
}

BVH::BVH(char* file, material m) : Shape({0, 0, 0}, m) {
	LoadModel(file);
	std::cout << "\nLoaded # of tri: " << triangles.size()<< " and # of norms: " << norBuff.size() << std::endl;
	this->boxMin = { X_min, Y_min, Z_min };
	this->boxMax = { X_max, Y_max, Z_max };
	//std::cout << "\nBOUNDING BOX MADE \n";
	//std::cout << boxMin.x << " " << boxMin.y << " " << boxMin.z << std::endl;
	//std::cout << boxMax.x << " " << boxMax.y << " " << boxMax.z << std::endl;
}

glm::vec3 BVH::giveNorm(glm::vec3 ray) {
	return { 0, 0, 0 };
}