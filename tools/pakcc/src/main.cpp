#include <package_cubemap.hpp>
#include <package_material.hpp>
#include <package_post_process_effect.hpp>
#include <package_shader.hpp>
#include <package_static_mesh.hpp>
#include <package_technique.hpp>
#include <package_texture.hpp>

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/renderer.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/resource/database.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp>

#include <iostream>

int main(int argc, char const* argv[])
{
    boost::program_options::options_description global{ "options" };
    boost::program_options::positional_options_description positional;

    // clang-format off
    global.add_options()
    ("include-directory,I", boost::program_options::value<std::vector<std::string>>()->default_value(std::vector<std::string>{}, ""), "Extra include directories")
    ("source-directory,H", boost::program_options::value<std::vector<std::string>>()->default_value(std::vector<std::string>{}, ""), "The top level package directory.")
    ("build-directory,B", boost::program_options::value<std::string>()->default_value(boost::filesystem::current_path().string()), "The top level project build directory.");
    // clang-format on

    try {
        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(global).positional(positional).run(), vm);
        boost::program_options::notify(vm);

        auto build_directory = boost::filesystem::canonical(vm["build-directory"].as<std::string>());
        auto data_directory = build_directory / "data";

        if (!boost::filesystem::exists(data_directory))
            boost::filesystem::create_directories(data_directory);

        sigma::resource::database<sigma::graphics::texture> texture_database{ data_directory, "texture" };
        sigma::resource::database<sigma::graphics::cubemap> cubemap_database{ data_directory, "cubemap" };
        sigma::resource::database<sigma::graphics::shader> shader_database{ data_directory, "shader" };
        sigma::resource::database<sigma::graphics::technique> technique_database{ data_directory, "technique" };
        sigma::resource::database<sigma::graphics::material> material_database{ data_directory, "material" };
        sigma::resource::database<sigma::graphics::static_mesh> static_mesh_database{ data_directory, "static_mesh" };
        sigma::resource::database<sigma::graphics::post_process_effect> post_process_effect_database{ data_directory, "post_process_effect" };

        std::vector<boost::filesystem::path> include_directories;
        // Add the source directories to the include path
        for (auto path : vm["source-directory"].as<std::vector<std::string>>()) {
            boost::trim(path);
            if (path.size() > 0)
                include_directories.push_back(path);
        }

        // Add the extra include paths
        for (auto path : vm["include-directory"].as<std::vector<std::string>>()) {
            boost::trim(path);
            if (path.size() > 0)
                include_directories.push_back(path);
        }

        std::vector<boost::filesystem::path> source_directories;
        for (auto path : vm["source-directory"].as<std::vector<std::string>>())
            source_directories.push_back(boost::filesystem::canonical(path));

        for (auto source_directory : source_directories)
            package_textures(texture_database, source_directory);

        for (auto source_directory : source_directories)
            package_cubemaps(texture_database, cubemap_database, source_directory);

        for (auto source_directory : source_directories)
            package_shaders(shader_database, source_directory, include_directories);

        for (auto source_directory : source_directories)
            package_techniques(shader_database, technique_database, source_directory, include_directories);

        for (auto source_directory : source_directories)
            package_materials(texture_database, cubemap_database, technique_database, material_database, source_directory);

        for (auto source_directory : source_directories)
            package_static_meshes(material_database, static_mesh_database, source_directory);

        for (auto source_directory : source_directories)
            package_post_process_effects(texture_database, cubemap_database, technique_database, static_mesh_database, post_process_effect_database, source_directory);

        // TODO: (NOW) blueprints

        auto graphics_directory = data_directory / "graphics";
        if (!boost::filesystem::exists(graphics_directory))
            boost::filesystem::create_directories(graphics_directory);
        sigma::graphics::renderer::settings settings;
        settings.image_based_light_effect = post_process_effect_database.handle_for({ "post_process_effect/pbr/deffered/lights/image_based" });
        settings.point_light_effect = post_process_effect_database.handle_for({ "post_process_effect/pbr/deffered/lights/point" });
        settings.directional_light_effect = post_process_effect_database.handle_for({ "post_process_effect/pbr/deffered/lights/directional" });
        settings.spot_light_effect = post_process_effect_database.handle_for({ "post_process_effect/pbr/deffered/lights/spot" });
        settings.texture_blit_effect = post_process_effect_database.handle_for({ "post_process_effect/pbr/deffered/texture_blit" });
        settings.vignette_effect = post_process_effect_database.handle_for({ "post_process_effect/vignette" });
        settings.gamma_conversion = post_process_effect_database.handle_for({ "post_process_effect/pbr/deffered/gamma_conversion" });
        settings.shadow_technique = technique_database.handle_for({ "vertex/default", "fragment/shadow" });
        std::ofstream stream{ (graphics_directory / "settings").string(), std::ios::binary };
        boost::archive::binary_oarchive oa(stream);
        oa << settings;
    } catch (boost::program_options::error& e) {
        std::cerr << "pakcc\nerror: " << e.what() << '\n';
        std::cerr << global << '\n';
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "pakcc\nerror: " << e.what() << '\n';
        return -1;
    }
    return 0;
}