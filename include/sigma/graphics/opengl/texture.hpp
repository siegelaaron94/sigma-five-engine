#ifndef SIGMA_GRAPHICS_OPENGL_TEXTURE_HPP
#define SIGMA_GRAPHICS_OPENGL_TEXTURE_HPP

#include <sigma/config.hpp>

#include <sigma/graphics/texture.hpp>
#include <sigma/resource/cache.hpp>

#include <glad/glad.h>

#include <cassert>
#include <utility>
#include <vector>

#define TEXTURE_PTR(texture_mgr, x) texture_mgr.acquire(x)

namespace sigma {
namespace opengl {
    enum class texture_unit : GLenum {
        TEXTURE0 = GL_TEXTURE0,
        TEXTURE1 = GL_TEXTURE1,
        TEXTURE2 = GL_TEXTURE2,
        TEXTURE3 = GL_TEXTURE3,
        TEXTURE4 = GL_TEXTURE4,
        TEXTURE5 = GL_TEXTURE5,
        TEXTURE6 = GL_TEXTURE6,
        TEXTURE7 = GL_TEXTURE7,
        TEXTURE8 = GL_TEXTURE8,
        TEXTURE9 = GL_TEXTURE9,
    };

    enum class internal_format : GLenum {
        // Base internal formats
        DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
        DEPTH_STENCIL = GL_DEPTH_STENCIL,
        RED = GL_RED,
        RG = GL_RG,
        RGB = GL_RGB,
        RGBA = GL_RGBA,

        // Sized internal formats
        R8 = GL_R8,
        R8_SNORM = GL_R8_SNORM,
        R16 = GL_R16,
        R16_SNORM = GL_R16_SNORM,
        RG8 = GL_RG8,
        RG8_SNORM = GL_RG8_SNORM,
        RG16 = GL_RG16,
        RG16_SNORM = GL_RG16_SNORM,
        R3_G3_B2 = GL_R3_G3_B2,
        RGB4 = GL_RGB4,
        RGB5 = GL_RGB5,
        RGB8 = GL_RGB8,
        RGB8_SNORM = GL_RGB8_SNORM,
        RGB10 = GL_RGB10,
        RGB12 = GL_RGB12,
        RGB16_SNORM = GL_RGB16_SNORM,
        RGBA2 = GL_RGBA2,
        RGBA4 = GL_RGBA4,
        RGB5_A1 = GL_RGB5_A1,
        RGBA8 = GL_RGBA8,
        RGBA8_SNORM = GL_RGBA8_SNORM,
        RGB10_A2 = GL_RGB10_A2,
        RGB10_A2UI = GL_RGB10_A2UI,
        RGBA12 = GL_RGBA12,
        RGBA16 = GL_RGBA16,
        SRGB8 = GL_SRGB8,
        SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
        R16F = GL_R16F,
        RG16F = GL_RG16F,
        RGB16F = GL_RGB16F,
        RGBA16F = GL_RGBA16F,
        R32F = GL_R32F,
        RG32F = GL_RG32F,
        RGB32F = GL_RGB32F,
        RGBA32F = GL_RGBA32F,
        R11F_G11F_B10F = GL_R11F_G11F_B10F,
        RGB9_E5 = GL_RGB9_E5,
        R8I = GL_R8I,
        R8UI = GL_R8UI,
        R16I = GL_R16I,
        R16UI = GL_R16UI,
        R32I = GL_R32I,
        R32UI = GL_R32UI,
        RG8I = GL_RG8I,
        RG8UI = GL_RG8UI,
        RG16I = GL_RG16I,
        RG16UI = GL_RG16UI,
        RG32I = GL_RG32I,
        RG32UI = GL_RG32UI,
        RGB8I = GL_RGB8I,
        RGB8UI = GL_RGB8UI,
        RGB16I = GL_RGB16I,
        RGB16UI = GL_RGB16UI,
        RGB32I = GL_RGB32I,
        RGB32UI = GL_RGB32UI,
        RGBA8I = GL_RGBA8I,
        RGBA8UI = GL_RGBA8UI,
        RGBA16I = GL_RGBA16I,
        RGBA16UI = GL_RGBA16UI,
        RGBA32I = GL_RGBA32I,
        RGBA32UI = GL_RGBA32UI,

        DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
        DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
        DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
        DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
        STENCIL_INDEX8 = GL_STENCIL_INDEX8,

        // Compressed internal formats
        COMPRESSED_RED = GL_COMPRESSED_RED,
        COMPRESSED_RG = GL_COMPRESSED_RG,
        COMPRESSED_RGB = GL_COMPRESSED_RGB,
        COMPRESSED_RGBA = GL_COMPRESSED_RGBA,
        COMPRESSED_SRGB = GL_COMPRESSED_SRGB,
        COMPRESSED_SRGB_ALPHA = GL_COMPRESSED_SRGB_ALPHA,
        COMPRESSED_RED_RGTC1 = GL_COMPRESSED_RED_RGTC1,
        COMPRESSED_SIGNED_RED_RGTC1 = GL_COMPRESSED_SIGNED_RED_RGTC1,
        COMPRESSED_RG_RGTC2 = GL_COMPRESSED_RG_RGTC2,
        COMPRESSED_SIGNED_RG_RGTC2 = GL_COMPRESSED_SIGNED_RG_RGTC2,
        //COMPRESSED_RGBA_BPTC_UNORM = GL_COMPRESSED_RGBA_BPTC_UNORM,
        //COMPRESSED_SRGB_ALPHA_BPTC_UNORM = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
        //COMPRESSED_RGB_BPTC_SIGNED_FLOAT = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
        //COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,
    };

    // TODO hack remove this
    std::pair<GLenum, GLenum> convert_gl(graphics::texture_format fmt);

    // TODO hack remove this
    internal_format convert_internal(graphics::texture_format fmt);

    class texture {
    public:
        texture(internal_format format, glm::ivec2 size,
            graphics::texture_filter minification_filter = graphics::texture_filter::NEAREST,
            graphics::texture_filter magnification_filter = graphics::texture_filter::NEAREST,
            graphics::texture_filter mipmap_filter = graphics::texture_filter::NONE);

        texture(internal_format format, glm::ivec2 size,
            graphics::texture_filter minification_filter,
            graphics::texture_filter magnification_filter,
            graphics::texture_filter mipmap_filter,
            graphics::texture_format data_format,
            const std::vector<char>& data);

        texture(const graphics::texture& data);

        texture(texture&& other);

        texture& operator=(texture&& other);

        ~texture();

        GLuint get_object() const;

        void bind() const;

        void generate_mipmaps();

    private:
        texture(const texture&) = delete;
        texture& operator=(const texture&) = delete;

        GLuint object_ = 0;
    };

    class texture_manager {
    public:
        // TODO remove the use of unique_ptr

        texture_manager(resource::cache<graphics::texture>& texture_cache)
            : texture_cache_(texture_cache)
        {
        }

        opengl::texture* acquire(const resource::handle<graphics::texture>& hndl)
        {
            // TODO not thread safe
            if (hndl.index >= textures_.size())
                textures_.resize(hndl.index + 1);

            if (textures_[hndl.index] == nullptr)
                textures_[hndl.index] = std::make_unique<texture>(*texture_cache_.acquire(hndl));

            return textures_.at(hndl.index).get();
        }

    private:
        resource::cache<graphics::texture>& texture_cache_;
        std::vector<std::unique_ptr<texture>> textures_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_TEXTURE_HPP
