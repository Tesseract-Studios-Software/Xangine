#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <osgGA/GUIEventHandler>
#include <osgViewer/View>
#include <iostream>
#include <vector>
using namespace std;

namespace xgnUI {

vector<string> keys_pressed;

int init_keyboard(osg::ref_ptr<osgViewer::View> view) {
    class key_press_handler : public osgGA::GUIEventHandler {
    public:
        bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) override {
            if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN) {
                int key = ea.getUnmodifiedKey();
                
                // Clear previous keys on new key press
                keys_pressed.clear();
                
                switch (key) {
                    // Alphabet keys
                    case 'a': case 'A': keys_pressed.push_back("a"); return true;
                    case 'b': case 'B': keys_pressed.push_back("b"); return true;
                    case 'c': case 'C': keys_pressed.push_back("c"); return true;
                    case 'd': case 'D': keys_pressed.push_back("d"); return true;
                    case 'e': case 'E': keys_pressed.push_back("e"); return true;
                    case 'f': case 'F': keys_pressed.push_back("f"); return true;
                    case 'g': case 'G': keys_pressed.push_back("g"); return true;
                    case 'h': case 'H': keys_pressed.push_back("h"); return true;
                    case 'i': case 'I': keys_pressed.push_back("i"); return true;
                    case 'j': case 'J': keys_pressed.push_back("j"); return true;
                    case 'k': case 'K': keys_pressed.push_back("k"); return true;
                    case 'l': case 'L': keys_pressed.push_back("l"); return true;
                    case 'm': case 'M': keys_pressed.push_back("m"); return true;
                    case 'n': case 'N': keys_pressed.push_back("n"); return true;
                    case 'o': case 'O': keys_pressed.push_back("o"); return true;
                    case 'p': case 'P': keys_pressed.push_back("p"); return true;
                    case 'q': case 'Q': keys_pressed.push_back("q"); return true;
                    case 'r': case 'R': keys_pressed.push_back("r"); return true;
                    case 's': case 'S': keys_pressed.push_back("s"); return true;
                    case 't': case 'T': keys_pressed.push_back("t"); return true;
                    case 'u': case 'U': keys_pressed.push_back("u"); return true;
                    case 'v': case 'V': keys_pressed.push_back("v"); return true;
                    case 'w': case 'W': keys_pressed.push_back("w"); return true;
                    case 'x': case 'X': keys_pressed.push_back("x"); return true;
                    case 'y': case 'Y': keys_pressed.push_back("y"); return true;
                    case 'z': case 'Z': keys_pressed.push_back("z"); return true;
                    
                    // Number keys
                    case '0': keys_pressed.push_back("0"); return true;
                    case '1': keys_pressed.push_back("1"); return true;
                    case '2': keys_pressed.push_back("2"); return true;
                    case '3': keys_pressed.push_back("3"); return true;
                    case '4': keys_pressed.push_back("4"); return true;
                    case '5': keys_pressed.push_back("5"); return true;
                    case '6': keys_pressed.push_back("6"); return true;
                    case '7': keys_pressed.push_back("7"); return true;
                    case '8': keys_pressed.push_back("8"); return true;
                    case '9': keys_pressed.push_back("9"); return true;
                    
                    // Function keys
                    case osgGA::GUIEventAdapter::KEY_F1: keys_pressed.push_back("F1"); return true;
                    case osgGA::GUIEventAdapter::KEY_F2: keys_pressed.push_back("F2"); return true;
                    case osgGA::GUIEventAdapter::KEY_F3: keys_pressed.push_back("F3"); return true;
                    case osgGA::GUIEventAdapter::KEY_F4: keys_pressed.push_back("F4"); return true;
                    case osgGA::GUIEventAdapter::KEY_F5: keys_pressed.push_back("F5"); return true;
                    case osgGA::GUIEventAdapter::KEY_F6: keys_pressed.push_back("F6"); return true;
                    case osgGA::GUIEventAdapter::KEY_F7: keys_pressed.push_back("F7"); return true;
                    case osgGA::GUIEventAdapter::KEY_F8: keys_pressed.push_back("F8"); return true;
                    case osgGA::GUIEventAdapter::KEY_F9: keys_pressed.push_back("F9"); return true;
                    case osgGA::GUIEventAdapter::KEY_F10: keys_pressed.push_back("F10"); return true;
                    case osgGA::GUIEventAdapter::KEY_F11: keys_pressed.push_back("F11"); return true;
                    case osgGA::GUIEventAdapter::KEY_F12: keys_pressed.push_back("F12"); return true;
                    
                    // Special keys
                    case osgGA::GUIEventAdapter::KEY_Space:
                        keys_pressed.push_back("Space");
                        return true;
                    case osgGA::GUIEventAdapter::KEY_Return:
                        keys_pressed.push_back("Enter");
                        return true;
                    case osgGA::GUIEventAdapter::KEY_Escape:
                        keys_pressed.push_back("Escape");
                        return true;
                    case osgGA::GUIEventAdapter::KEY_BackSpace:
                        keys_pressed.push_back("Backspace");
                        return true;
                    case osgGA::GUIEventAdapter::KEY_Tab:
                        keys_pressed.push_back("Tab");
                        return true;
                        
                    // Arrow keys
                    case osgGA::GUIEventAdapter::KEY_Left:
                        keys_pressed.push_back("Left");
                        return true;
                    case osgGA::GUIEventAdapter::KEY_Right:
                        keys_pressed.push_back("Right");
                        return true;
                    case osgGA::GUIEventAdapter::KEY_Up:
                        keys_pressed.push_back("Up");
                        return true;
                    case osgGA::GUIEventAdapter::KEY_Down:
                        keys_pressed.push_back("Down");
                        return true;
                }
            }
            return false; // Event not handled
        }
    };
    
    view->addEventHandler(new key_press_handler());
    return 0; // Return success
}

bool detect_key(string key) {
    for (const string& k : keys_pressed) {
        if (k == key) {
            return true; // Key is pressed
        }
    }
    return false; // Key is not pressed
}

};

#endif // KEYBOARD_HPP