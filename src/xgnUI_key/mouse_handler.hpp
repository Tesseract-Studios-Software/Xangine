#include <xgnUI_key/mouse.hpp>
#include <xgn_window/window.hpp>
#include <xgn_interface/interface.hpp>
#include <xgn_log/log.hpp>
#include <osgViewer/Viewer>

namespace xgn {

MouseHandler::MouseHandler(window* win) : _window(win) {}

bool MouseHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
    if (!_window || _window->interfaces.empty()) return false;
    
    auto* interface = _window->interfaces[0];
    
    // Get window center on first call
    if (_center_x == 0 && _window->viewer) {
        osgViewer::Viewer::Windows windows;
        _window->viewer->getWindows(windows);
        if (!windows.empty()) {
            auto* window = windows[0];
            _center_x = window->getTraits()->width / 2;
            _center_y = window->getTraits()->height / 2;
        }
    }
    
    // Handle right click to toggle mouse grab
    if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH &&
        ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) {
        
        interface->mouse_grabbed = !interface->mouse_grabbed;
        
        if (_window->viewer) {
            osgViewer::Viewer::Windows windows;
            _window->viewer->getWindows(windows);
            if (!windows.empty()) {
                auto* window = windows[0];
                if (interface->mouse_grabbed) {
                    window->setCursor(osgViewer::GraphicsWindow::NoCursor);
                    window->requestWarpPointer(_center_x, _center_y);
                } else {
                    window->setCursor(osgViewer::GraphicsWindow::LeftArrowCursor);
                }
            }
        }
    }
    
    // Handle ESC to release mouse
    if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN &&
        ea.getKey() == osgGA::GUIEventAdapter::KEY_Escape) {
        interface->mouse_grabbed = false;
        if (_window->viewer) {
            osgViewer::Viewer::Windows windows;
            _window->viewer->getWindows(windows);
            if (!windows.empty()) {
                windows[0]->setCursor(osgViewer::GraphicsWindow::LeftArrowCursor);
            }
        }
    }
    
    // Handle mouse movement
    if ((ea.getEventType() == osgGA::GUIEventAdapter::MOVE ||
         ea.getEventType() == osgGA::GUIEventAdapter::DRAG) &&
        interface->mouse_grabbed) {
        
        interface->mouse_delta_x = ea.getX() - _center_x;
        interface->mouse_delta_y = ea.getY() - _center_y;
        
        // Warp back to center
        if (_window->viewer) {
            osgViewer::Viewer::Windows windows;
            _window->viewer->getWindows(windows);
            if (!windows.empty()) {
                windows[0]->requestWarpPointer(_center_x, _center_y);
            }
        }
    }
    
    return false;
}

// Implement private methods
void MouseHandler::toggle_mouse_grab(interface* interface) {
    if (interface->mouse_grabbed) {
        release_mouse(interface);
    } else {
        grab_mouse(interface);
    }
}

void MouseHandler::grab_mouse(interface* interface) {
    interface->mouse_grabbed = true;
    
    if (_window->viewer) {
        osgViewer::Viewer::Windows windows;
        _window->viewer->getWindows(windows);
        if (!windows.empty()) {
            auto* window = windows[0];
            window->setCursor(osgViewer::GraphicsWindow::NoCursor);
            window->requestWarpPointer(_center_x, _center_y);
        }
    }
}

void MouseHandler::release_mouse(interface* interface) {
    interface->mouse_grabbed = false;
    
    if (_window->viewer) {
        osgViewer::Viewer::Windows windows;
        _window->viewer->getWindows(windows);
        if (!windows.empty()) {
            windows[0]->setCursor(osgViewer::GraphicsWindow::LeftArrowCursor);
        }
    }
}

void MouseHandler::handle_mouse_movement(interface* interface, const osgGA::GUIEventAdapter& ea) {
    // Implementation if needed
}

} // namespace xgn