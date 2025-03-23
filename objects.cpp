#include "objects.h"
#include <vector>
#include <iostream>

using namespace std;
Mesh* objects::CreatePine(const std::string& name, int baseRadius, int height, glm::vec3 color) {
	vector<VertexFormat> vertices;
	vector<unsigned int> indices;

	vertices.push_back(VertexFormat(glm::vec3(0.0f, height, 0.0f), color));

	for (int i = 0; i < 32; i++) {
		float angle = i * 2.0f * M_PI / 32;
		float x = baseRadius * cos(angle);
		float z = baseRadius * sin(angle);
		vertices.push_back(VertexFormat(glm::vec3(x, 0.0f, z), color));
	}


	for (int i = 1; i < 32; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	indices.push_back(0);
	indices.push_back(32);
	indices.push_back(1);

	Mesh* pine = new Mesh(name);
	pine->InitFromData(vertices, indices);
	return pine;
}


Mesh* objects::CreateCilinder(const std::string& name, int baseRadius, int height, glm::vec3 color) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    vertices.push_back(VertexFormat(glm::vec3(0.0f, height, 0.0f), color));
    vertices.push_back(VertexFormat(glm::vec3(0.0f, 0.0f, 0.0f), color));

    for (int i = 0; i < 32; i++) {
        float angle = i * 2.0f * M_PI / 32;
        float x = baseRadius * cos(angle);
        float z = baseRadius * sin(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, height, z), color));
        vertices.push_back(VertexFormat(glm::vec3(x, 0.0f, z), color));
    }

    for (int i = 2; i < 64; i += 2) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 2);
    }
    indices.push_back(0);
    indices.push_back(64);
    indices.push_back(2);

    for (int i = 3; i < 64; i += 2) {
        indices.push_back(1);
        indices.push_back(i + 2);
        indices.push_back(i);
    }
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(65);

    for (int i = 2; i < 64; i += 2) {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 3);

        indices.push_back(i);
        indices.push_back(i + 3);
        indices.push_back(i + 2);
    }
    indices.push_back(64);
    indices.push_back(65);
    indices.push_back(3);

    indices.push_back(64);
    indices.push_back(3);
    indices.push_back(2);

    Mesh* cilinder = new Mesh(name);
    cilinder->InitFromData(vertices, indices);
    return cilinder;
}


Mesh* objects::CreateDisk(const std::string& name, int baseRadius, glm::vec3 color) {
	vector<VertexFormat> vertices;
	vector<unsigned int> indices;

	vertices.push_back(VertexFormat(glm::vec3(0.0f, 0.0f, 0.0f), color));

	for (int i = 0; i < 32; i++) {
		float angle = i * 2.0f * M_PI / 32;
		float x = baseRadius * cos(angle);
		float z = baseRadius * sin(angle);
		vertices.push_back(VertexFormat(glm::vec3(x, 0.0f, z), color));
	}

	for (int i = 1; i < 32; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}
	indices.push_back(0);
	indices.push_back(32);
	indices.push_back(1);

	Mesh* disk = new Mesh(name);
	disk->InitFromData(vertices, indices);
	return disk;
}

Mesh* objects::CreateRectangle(const std::string& name, glm::vec3 color) {
	vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(-0.5f, 0.0f, -0.5f), color),
		VertexFormat(glm::vec3(0.5f, 0.0f, -0.5f), color),
		VertexFormat(glm::vec3(0.5f, 0.0f, 0.5f), color),
		VertexFormat(glm::vec3(-0.5f, 0.0f, 0.5f), color)
	};

	vector<unsigned int> indices = {
		0, 1, 2,
		0, 2, 3
	};

	Mesh* rectangle = new Mesh(name);
	rectangle->InitFromData(vertices, indices);
	return rectangle;
}



Mesh* objects::CreateTimeBar(const std::string& name,
    glm::vec3 center_of_mesh,
    float length, glm::vec3 color,
    bool fill) {
    glm::vec3 center = center_of_mesh;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center, color),
        VertexFormat(center + glm::vec3(length * 80.f, length * 12, 0), color),
        VertexFormat(center + glm::vec3(length * 80.f, 0, 0), color),
        VertexFormat(center + glm::vec3(0, length * 12, 0), color),

    };

    Mesh* health = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 0, 3, 1 };

    if (!fill) {
        health->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    health->InitFromData(vertices, indices);
    return health;

}


Mesh* objects::CreateTimeBarContur(const std::string& name,
    glm::vec3 center_of_mesh,
    float length, glm::vec3 color,
    bool fill) {

    glm::vec3 center = center_of_mesh;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center, color),
        VertexFormat(center + glm::vec3(length * 40, length * 2, 0), color),
        VertexFormat(center + glm::vec3(length * 40, 0, 0), color),
        VertexFormat(center + glm::vec3(-length * 40, 0, 0), color),
        VertexFormat(center + glm::vec3(-length * 40, length * 2, 0), color),

        VertexFormat(center + glm::vec3(-length * 40, 0, 0), color),
        VertexFormat(center + glm::vec3(-length * 40, length * 12, 0), color),
        VertexFormat(center + glm::vec3((-length * 40) + (length * 2), 0, 0), color),
        VertexFormat(center + glm::vec3((-length * 40) + (length * 2), length * 12, 0), color),

        VertexFormat(center + glm::vec3(-length * 40, length * 12, 0), color),
        VertexFormat(center + glm::vec3(length * 40, length * 12, 0), color),
        VertexFormat(center + glm::vec3(length * 40, (length * 12) - (length * 2), 0), color),
        VertexFormat(center + glm::vec3((-length * 40), (length * 12) - (length * 2), 0), color),


        VertexFormat(center + glm::vec3((length * 40) - (length * 2), 0, 0), color),
        VertexFormat(center + glm::vec3((length * 40) - (length * 2), length * 12, 0), color),
        VertexFormat(center + glm::vec3(length * 40, length * 12, 0), color),
        VertexFormat(center + glm::vec3(length * 40, 0, 0), color),
    };

    Mesh* health = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 0, 3, 4, 0, 4, 1, 5, 6, 7, 7, 6, 8, 9, 10, 11, 9, 11, 12, 13, 14, 15, 13, 15, 16 };

    if (!fill) {
        health->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }
    health->InitFromData(vertices, indices);
    return health;
}
