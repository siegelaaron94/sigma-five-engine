#ifndef SIGMA_ENGINE_OPENGL_MATERIAL_MANAGER_HPP
#define SIGMA_ENGINE_OPENGL_MATERIAL_MANAGER_HPP

#include <sigma/opengl/material.hpp>
#include <sigma/opengl/resource_manager.hpp>

#include <sigma/graphics/material.hpp>

namespace sigma {
namespace opengl {
    class texture_manager;
    class shader_manager;
    class material_manager : public resource::resource_manager<graphics::material, opengl::material> {
    public:
        material_manager(resource::cache<graphics::material>& cache, texture_manager& textures, shader_manager& shaders);

        virtual std::shared_ptr<opengl::material> create(const graphics::material& cpu_material) override;

    private:
        material_manager(const material_manager&) = delete;
        material_manager& operator=(const material_manager&) = delete;

        texture_manager& textures_;
        shader_manager& shaders_;
    };
}
}

#endif // SIGMA_ENGINE_OPENGL_MATERIAL_MANAGER_HPP
