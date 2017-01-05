#ifndef SIGMA_GRAPHICS_TEXTURE_HPP
#define SIGMA_GRAPHICS_TEXTURE_HPP

#include <sigma/config.hpp>
#include <sigma/resource/manager.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <glm/vec2.hpp>

#include <boost/serialization/vector.hpp>

namespace sigma {
namespace graphics {

    struct texture_data {
        glm::ivec2 size;
        std::vector<unsigned char> pixels;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& size;
            ar& pixels;
        }
    };

    class SIGMA_API texture {
    public:
        using resource_data = texture_data;

        texture() = default;

        texture(texture&&) noexcept = default;

        texture& operator=(texture&&) noexcept = default;

        virtual ~texture() = default;

    private:
        texture(const texture&) = delete;
        texture& operator=(const texture&) = delete;
    };

    using texture_manager = resource::manager<texture>;
}
}

BOOST_CLASS_VERSION(sigma::graphics::texture_data, 1);

#endif // SIGMA_GRAPHICS_TEXTURE_HPP
