#include "components/simple_scene.h"
#include "core/gpu/mesh.h"
#include <vector>

namespace objects {
	Mesh* CreatePine(const std::string& name, int baseRadius, int height, glm::vec3 color);
	Mesh* CreateCilinder(const std::string& name, int baseRadius, int height, glm::vec3 color);
	Mesh* CreateDisk(const std::string& name, int baseRadius, glm::vec3 color);
	Mesh* CreateRectangle(const std::string& name, glm::vec3 color);
	Mesh* CreateTimeBar(const std::string& name, glm::vec3 center, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateTimeBarContur(const std::string& name, glm::vec3 center, float length, glm::vec3 color, bool fill = false);
}
