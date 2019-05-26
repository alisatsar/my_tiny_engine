#include <cstdlib>
#include <iostream>
#include <memory>

#include "engine.h"
#include "event.h"
#include "shader_path_constants.hpp"
#include "common/types.h"

#include "vao.h"
#include "texture.h"

int main()
{
	std::unique_ptr<te::engine, void(*)(te::engine*)> m(te::create_engine(), te::destroy_engine);

	std::cout << m->check_version();

	if(!m->initialize())
	{
		return EXIT_FAILURE;
	}

	if(!m->create_window("My window", 0, 0, 300, 300))
	{
		return EXIT_FAILURE;
	}

    m->create_my_shader("/home/alisatsar/my_tiny_engine/my_tiny_engine/03-texture/shaders/texture.vert",
                        "/home/alisatsar/my_tiny_engine/my_tiny_engine/03-texture/shaders/texture.frag");
    texture* tex = new texture("/home/alisatsar/my_tiny_engine/my_tiny_engine/03-texture/container.jpg");

    texture* tex2 = new texture();

    bool continue_loop = true;
	while(continue_loop)
	{
		te::event event;

        while(te::event_manager::read_input(event))
		{
			switch(event)
			{
            case te::event::UP:
				std::cout << "up" << std::endl;
				break;
            case te::event::QUIT:
				continue_loop = false;
				break;
			}
		}
        m->render_texture(tex->get_tex_id());
        m->swap_buffers();
	};

	m->unintialize();

	return EXIT_SUCCESS;
}
