#include "event.h"

#include <algorithm>
#include <array>

#include <SDL2/SDL_events.h>

struct bind_event
{
    SDL_Keycode key;
    std::string_view name;
    te::event pressed;
};

const std::array<bind_event, 8> keys{
    { { SDLK_w, "UP", te::event::UP },
      { SDLK_a, "LEFT", te::event::LEFT },
      { SDLK_s, "DOWN", te::event::DOWN },
      { SDLK_d, "RIGHT", te::event::RIGHT },
      { SDLK_RETURN, "ENTER", te::event::ENTER },
      { SDL_QUIT, "QUIT", te::event::QUIT }}
};

bool check_input(const SDL_Event& e, const bind_event*& result)
{
        auto it = std::find_if(std::begin(keys), std::end(keys),[&](const ::bind_event& b) {
                return b.key == e.key.keysym.sym;
                } );

        if(it != std::end(keys))
        {
                result = it;
                return true;
        }
        return true;
}

bool te::event_manager::read_input(te::event& e)
{
    SDL_Event sdl_event;

    if(SDL_PollEvent(&sdl_event))
    {
        const bind_event* binding = nullptr;

        if(sdl_event.type == SDL_QUIT)
        {
            e = te::event::QUIT;
            return true;
        }
        else if(sdl_event.type == SDL_KEYDOWN)
        {
            if(check_input(sdl_event, binding))
            {
                e = binding->pressed;
                return true;
            }
        }
    }
    return false;
}
