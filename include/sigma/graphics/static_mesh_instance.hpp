#ifndef SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP
#define SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/static_mesh.hpp>

#include <unordered_map>

namespace sigma {
namespace graphics {
    struct static_mesh_instance {
        resource::handle<static_mesh> mesh;
        std::unordered_map<std::size_t, resource::handle<material>> materials;
    };
}
}

#endif // SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP
