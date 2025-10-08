#pragma once
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIActionAdapter>

// Forward declarations instead of includes
namespace xgn {
    class window;  // Forward declaration
}

namespace xgn {

class MouseHandler : public osgGA::GUIEventHandler {
public:
    MouseHandler(window* win);  // Just declaration
    
    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) override;
    
private:
    void toggle_mouse_grab(class interface* interface);  // Forward declaration in parameter
    void grab_mouse(class interface* interface);
    void release_mouse(class interface* interface);
    void handle_mouse_movement(class interface* interface, const osgGA::GUIEventAdapter& ea);
    
    window* _window;
    int _center_x = 0, _center_y = 0;
};

} // namespace xgn