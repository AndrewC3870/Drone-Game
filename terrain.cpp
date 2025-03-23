#include "terrain.h"
#include <vector>
#include <iostream>
#include "lab_m1/Tema2/transform.h"
using namespace std;
using namespace terrain;

Mesh* terrain::CreateTerrain(const std::string& name, int width, int height) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    float uvScale = 10.0f;
    for (int z = 0; z < height; z++) {
        for (int x = 0; x < width; x++) {
            float u = (float)x / (width - 1) * uvScale;
            float v = (float)z / (height - 1) * uvScale;
            float y = 0.0f;

            vertices.push_back(VertexFormat(glm::vec3(x, y, z), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1), glm::vec2(u, v)));
        }
    }

    for (int z = 0; z < height - 1; z++) {
        for (int x = 0; x < width - 1; x++) {
            int topLeft = z * width + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * width + x;
            int bottomRight = bottomLeft + 1;
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
    Mesh* terrain = new Mesh(name);
    terrain->InitFromData(vertices, indices);
    return terrain;
}
