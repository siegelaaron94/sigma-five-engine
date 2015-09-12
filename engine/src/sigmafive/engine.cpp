#include <sigmafive/engine.hpp>

#include <sigmafive/game/transform_component.hpp>
#include <sigmafive/game/static_mesh_component.hpp>

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

namespace sigmafive {

    engine::engine(int &argc, char **argv) {
        //TODO clean this up
        //this is a hack just to get things working
        for(auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(default_plugin_path()), {})) {
            if(boost::dll::shared_library::suffix() == entry.path().extension()) {
                plugins_.emplace_back(entry.path());
                plugins_.back().get<void(sigmafive::engine *)>("register_plugin")(this);
            }
        }

        component_registry_.register_component(game::transform_component::CLASS_ID,
                                               std::unique_ptr<game::transform_component_pool_factory>(new game::transform_component_pool_factory{}));
        component_registry_.register_component(game::static_mesh_component::CLASS_ID,
                                               std::unique_ptr<game::static_mesh_component_pool_factory>(new game::static_mesh_component_pool_factory{}));
    }

    boost::filesystem::path engine::default_plugin_path() {
#ifdef CMAKE_IDE_GENERATOR
    #ifdef SIGMAFIVE_DEBUG
		return "../plugins/Debug";
    #else
		return "../plugins/Release";
    #endif
#else
        return boost::filesystem::path("../plugins");
#endif
    }

    system::resource_manager &engine::resource_manager() {
        return resource_manager_;
    }

    game::component_registry &engine::component_registry() {
        return component_registry_;
    }

    graphics::context_manager &engine::graphics_context_manager() {
        return graphics_context_manager_;
    }
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::engine)