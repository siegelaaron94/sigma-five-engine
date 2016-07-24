#include <sigmafive/opengl/index_buffer.hpp>

namespace sigmafive {
namespace opengl {
    index_buffer::index_buffer(buffer_usage usage)
        : buffer(buffer_type::element_array_buffer, usage)
    {
    }

    index_buffer::~index_buffer()
    {
    }
}
}