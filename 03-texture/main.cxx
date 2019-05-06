#include <cstdlib>
#include <iostream>
#include <memory>

#include "engine.h"
#include "event.h"
#include "shader_path_constants.hpp"
#include "common/types.h"

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

    m->create_my_shader(spc::triangle_3D_color_vert, spc::triangle_3D_color_frag);
    //m->create_texture("/home/alisatsar/my_tiny_engine/my_tiny_engine/03-texture/container.jpg");

    te::triangle t1(0.0f, 0.4f,
                    0.6f, -0.4f,
                    -0.4f, -0.2f);
    te::color color(0.f, 1.0f, 0.f, 1.0f);
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
        m->render_color_triangle(t1, color);
		m->swap_buffers();
	};

	m->unintialize();

	return EXIT_SUCCESS;
}
