#include "components/simple_scene.h"
#include "core/gpu/mesh.h"
#include <vector>


namespace drone {
	Mesh* CreateParallelepiped(const std::string& name, glm::vec3 size, glm::vec3 color);
}