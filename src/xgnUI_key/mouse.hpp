#pragma once
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <xgn_window/window.hpp>
#include <xgn_log/log.hpp>

namespace xgn {

class MouseHandler : public osgGA::GUIEventHandler {
public:
    MouseHandler(xgn::window* win) : _window(win) {}
    
    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) override {
        if (!_window || _window->interfaces.empty()) return false;
        
        auto* interface = _window->interfaces[0];
        
        switch(ea.getEventType()) {
            case osgGA::GUIEventAdapter::PUSH:
                if (ea.getButton() >= 1 && ea.getButton() <= 5) {
                    interface->mouse_buttons[ea.getButton() - 1] = true;
                }
                // Toggle mouse grab on right click
                if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) {
                    toggle_mouse_grab(interface);
                }
                break;
                
            case osgGA::GUIEventAdapter::RELEASE:
                if (ea.getButton() >= 1 && ea.getButton() <= 5) {
                    interface->mouse_buttons[ea.getButton() - 1] = false;
                }
                break;
                
            case osgGA::GUIEventAdapter::MOVE:
            case osgGA::GUIEventAdapter::DRAG:
                handle_mouse_movement(interface, ea);
                break;
                
            case osgGA::GUIEventAdapter::KEYDOWN:
                if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Escape) {
                    release_mouse(interface);
                }
                break;
        }
        
        return false;
    }
    
private:
    void toggle_mouse_grab(xgn::interface* interface) {
        if (interface->mouse_grabbed) {
            release_mouse(interface);
        } else {
            grab_mouse(interface);
        }
    }
    
    void grab_mouse(xgn::interface* interface) {
        interface->mouse_grabbed = true;
        
        // Get window from the viewer
        if (_window->viewer) {
            osgViewer::Viewer::Windows windows;
            _window->viewer->getWindows(windows);
            
            if (!windows.empty()) {
                auto* window = windows[0];
                int center_x = window->getTraits()->width / 2;
                int center_y = window->getTraits()->height / 2;
                
                // CORRECT: Use requestWarpPointer instead of setCursorPosition
                window->requestWarpPointer(center_x, center_y);
                
                // Hide cursor
                window->setCursor(osgViewer::GraphicsWindow::NoCursor);
                
                interface->mouse_x = center_x;
                interface->mouse_y = center_y;
                interface->mouse_delta_x = 0;
                interface->mouse_delta_y = 0;
            }
        }
        
        log("0x6001", 1, "Mouse grabbed");
    }
    
    void release_mouse(xgn::interface* interface) {
        interface->mouse_grabbed = false;
        
        // Show cursor
        if (_window->viewer) {
            osgViewer::Viewer::Windows windows;
            _window->viewer->getWindows(windows);
            
            if (!windows.empty()) {
                auto* window = windows[0];
                window->setCursor(osgViewer::GraphicsWindow::LeftArrowCursor);
            }
        }
        
        // Reset deltas
        interface->mouse_delta_x = 0;
        interface->mouse_delta_y = 0;
        
        log("0x6002", 1, "Mouse released");
    }
    
    void handle_mouse_movement(xgn::interface* interface, const osgGA::GUIEventAdapter& ea) {
        if (interface->mouse_grabbed) {
            // Get window dimensions for center calculation
            if (_window->viewer) {
                osgViewer::Viewer::Windows windows;
                _window->viewer->getWindows(windows);
                
                if (!windows.empty()) {
                    auto* window = windows[0];
                    int center_x = window->getTraits()->width / 2;
                    int center_y = window->getTraits()->height / 2;
                    
                    // Calculate delta from center
                    interface->mouse_delta_x = ea.getX() - center_x;
                    interface->mouse_delta_y = ea.getY() - center_y;
                    
                    // Warp mouse back to center if it moved significantly
                    if (abs(interface->mouse_delta_x) > 2 || abs(interface->mouse_delta_y) > 2) {
                        // CORRECT: Use requestWarpPointer
                        window->requestWarpPointer(center_x, center_y);
                        interface->mouse_x = center_x;
                        interface->mouse_y = center_y;
                    }
                }
            }
        } else {
            // Normal mouse movement
            interface->mouse_x = ea.getX();
            interface->mouse_y = ea.getY();
            interface->mouse_delta_x = 0;
            interface->mouse_delta_y = 0;
        }
    }
    
    xgn::window* _window;
};

} // namespace xgn