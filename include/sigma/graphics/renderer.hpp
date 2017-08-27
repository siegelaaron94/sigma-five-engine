#ifndef SIGMA_GRAPHICS_RENDERER_HPP
#define SIGMA_GRAPHICS_RENDERER_HPP

#include <sigma/config.hpp>

#include <sigma/frustum.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/technique.hpp>

#include <glm/vec2.hpp>

namespace sigma {
namespace graphics {
    struct SIGMA_API view_port {
        glm::ivec2 size;
        frustum view_frustum;
    };

    class SIGMA_API renderer {
    public:
        struct SIGMA_API settings {
            resource::handle<graphics::post_process_effect> image_based_light_effect;
            resource::handle<graphics::post_process_effect> point_light_effect;
            resource::handle<graphics::post_process_effect> directional_light_effect;
            resource::handle<graphics::post_process_effect> spot_light_effect;
            resource::handle<graphics::post_process_effect> texture_blit_effect;
            resource::handle<graphics::post_process_effect> gamma_conversion;
            resource::handle<graphics::post_process_effect> vignette_effect;
            resource::handle<graphics::technique> shadow_technique;

            template <class Archive>
            void serialize(Archive& ar, const unsigned int version)
            {
                ar& image_based_light_effect;
                ar& point_light_effect;
                ar& directional_light_effect;
                ar& spot_light_effect;
                ar& texture_blit_effect;
                ar& gamma_conversion;
                ar& vignette_effect;
                ar& shadow_technique;
            }
        };

        renderer(glm::ivec2 size);

        virtual ~renderer();

        virtual void resize(glm::uvec2 size) = 0;

    protected:
        settings settings_;

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;
    };
}
}

#endif // SIGMA_GRAPHICS_RENDERER_HPP
