#ifndef EVENT_H
#define EVENT_H

namespace te
{
enum class event
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    ENTER,
    QUIT
};
namespace event_manager {
    bool read_input(te::event& e);

}//end namespace event

}//end namespace te


#endif // EVENT_H
