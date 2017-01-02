#ifndef SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP
#define SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP

#include <sigma/graphics/shader_technique.hpp>

#include <sigma/graphics/static_mesh.hpp>

namespace sigma {
namespace graphics {

    class SIGMA_API post_process_effect : public shader_technique {
    public:
        post_process_effect();

        post_process_effect(post_process_effect&&) = default;

        post_process_effect& operator=(post_process_effect&&) = default;

        static_mesh_cache::instance mesh() const;

        void set_mesh(static_mesh_cache::instance mesh);

    private:
        post_process_effect(const post_process_effect&) = delete;
        post_process_effect& operator=(const post_process_effect&) = delete;

        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& boost::serialization::base_object<shader_technique>(*this);
            ar& mesh_;
        }

        static_mesh_cache::instance mesh_;
    };

    class post_process_effect_cache : public shader_technique_cache<post_process_effect> {
    public:
        post_process_effect_cache(boost::filesystem::path cache_directory,texture_cache &textures,shader_cache &shaders, static_mesh_cache &static_meshes)
            : shader_technique_cache<post_process_effect>(cache_directory,textures,shaders)
            , static_meshes_(static_meshes)
        {
        }

        virtual void patch(std::shared_ptr<post_process_effect> effect) override
        {
            shader_technique_cache<post_process_effect>::patch(effect);
            effect->set_mesh(static_meshes_.get(effect->mesh().id()));
        }
    private:
        static_mesh_cache &static_meshes_;
    };
}
}

#endif //SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP
