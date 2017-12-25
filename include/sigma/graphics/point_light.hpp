// TEMPLATE(generated.hpp)
#ifndef SIGMA_GRAPHICS_POINT_LIGHT_HPP
#define SIGMA_GRAPHICS_POINT_LIGHT_HPP

#include <sigma/config.hpp>
#include <sigma/reflect.hpp>

#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    struct R_EXPORT() point_light {
        BOOST_HANA_DEFINE_STRUCT(
            point_light,
            (glm::vec3, color),
            (float, intensity));

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& color;
            ar& intensity;
        }
    };
}
}

#endif // SIGMA_GRAPHICS_POINT_LIGHT_HPP
