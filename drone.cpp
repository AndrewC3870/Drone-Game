#include "drone.h"
#include <vector>
#include <iostream>


Mesh* drone::CreateParallelepiped(const std::string& name, glm::vec3 size, glm::vec3 color) {
    std::vector<VertexFormat> vertices = {
        // Front
        VertexFormat(glm::vec3(-size.x, -size.y,  size.z), color),
        VertexFormat(glm::vec3(size.x, -size.y,  size.z), color),
        VertexFormat(glm::vec3(size.x,  size.y,  size.z), color),
        VertexFormat(glm::vec3(-size.x,  size.y,  size.z), color),

        // Back
        VertexFormat(glm::vec3(-size.x, -size.y, -size.z), color),
        VertexFormat(glm::vec3(size.x, -size.y, -size.z), color),
        VertexFormat(glm::vec3(size.x,  size.y, -size.z), color),
        VertexFormat(glm::vec3(-size.x,  size.y, -size.z), color),

        // Left
        VertexFormat(glm::vec3(-size.x, -size.y, -size.z), color),
        VertexFormat(glm::vec3(-size.x, -size.y,  size.z), color),
        VertexFormat(glm::vec3(-size.x,  size.y,  size.z), color),
        VertexFormat(glm::vec3(-size.x,  size.y, -size.z), color),

        // Right
        VertexFormat(glm::vec3(size.x, -size.y, -size.z), color),
        VertexFormat(glm::vec3(size.x, -size.y,  size.z), color),
        VertexFormat(glm::vec3(size.x,  size.y,  size.z), color),
        VertexFormat(glm::vec3(size.x,  size.y, -size.z), color),

        // Top
        VertexFormat(glm::vec3(-size.x,  size.y, -size.z), color),
        VertexFormat(glm::vec3(-size.x,  size.y,  size.z), color),
        VertexFormat(glm::vec3(size.x,  size.y,  size.z), color),
        VertexFormat(glm::vec3(size.x,  size.y, -size.z), color),

        // Bottom
        VertexFormat(glm::vec3(-size.x, -size.y, -size.z), color),
        VertexFormat(glm::vec3(-size.x, -size.y,  size.z), color),
        VertexFormat(glm::vec3(size.x, -size.y,  size.z), color),
        VertexFormat(glm::vec3(size.x, -size.y, -size.z), color),
    };

    std::vector<unsigned int> indices = {
        // Front
        0, 1, 2, 2, 3, 0,
        // Back
        4, 5, 6, 6, 7, 4,
        // Left
        8, 9, 10, 10, 11, 8,
        // Right
        12, 13, 14, 14, 15, 12,
        // Top
        16, 17, 18, 18, 19, 16,
        // Bottom
        20, 21, 22, 22, 23, 20,
    };

    Mesh* mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    return mesh;
}
