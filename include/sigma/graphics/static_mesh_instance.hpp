#ifndef SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP
#define SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP

#include <sigma/config.hpp>

#include <sigma/component.hpp>
#include <sigma/graphics/static_mesh.hpp>

namespace sigma {
namespace graphics {
    struct static_mesh_instance {
        resource::handle_type<static_mesh> mesh;
        bool cast_shadows;

        static_mesh_instance(resource::handle_type<static_mesh> mesh, bool cast_shadows = true)
            : mesh { mesh }
            , cast_shadows { true }
        {
        }
    };
}
}

REGISTER_COMPONENT(sigma::graphics::static_mesh_instance)

#endif // SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP
