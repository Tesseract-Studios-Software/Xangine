#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <Xangine/Platform/GLFW.hpp>

namespace Xangine {

enum class KeyAction {
    Press,
    Release
};

enum class Key {
    Unknown = GLFW_KEY_UNKNOWN,
    Space = GLFW_KEY_SPACE,
    Escape = GLFW_KEY_ESCAPE,
    Enter = GLFW_KEY_ENTER,
    Tab = GLFW_KEY_TAB,
    Backspace = GLFW_KEY_BACKSPACE,
    Right = GLFW_KEY_RIGHT,
    Left = GLFW_KEY_LEFT,
    Down = GLFW_KEY_DOWN,
    Up = GLFW_KEY_UP,
    A = GLFW_KEY_A,
    D = GLFW_KEY_D,
    S = GLFW_KEY_S,
    W = GLFW_KEY_W,
    Q = GLFW_KEY_Q,
    E = GLFW_KEY_E,
    R = GLFW_KEY_R,
    F = GLFW_KEY_F,
    G = GLFW_KEY_G,
    H = GLFW_KEY_H,
    J = GLFW_KEY_J,
    K = GLFW_KEY_K,
    L = GLFW_KEY_L,
    Z = GLFW_KEY_Z,
    X = GLFW_KEY_X,
    C = GLFW_KEY_C,
    V = GLFW_KEY_V,
    B = GLFW_KEY_B,
    N = GLFW_KEY_N,
    M = GLFW_KEY_M,
    Zero = GLFW_KEY_0,
    One = GLFW_KEY_1,
    Two = GLFW_KEY_2,
    Three = GLFW_KEY_3,
    Four = GLFW_KEY_4,
    Five = GLFW_KEY_5,
    Six = GLFW_KEY_6,
    Seven = GLFW_KEY_7,
    Eight = GLFW_KEY_8,
    Nine = GLFW_KEY_9,
    F1 = GLFW_KEY_F1,
    F2 = GLFW_KEY_F2,
    F3 = GLFW_KEY_F3,
    F4 = GLFW_KEY_F4,
    F5 = GLFW_KEY_F5,
    F6 = GLFW_KEY_F6,
    F7 = GLFW_KEY_F7,
    F8 = GLFW_KEY_F8,
    F9 = GLFW_KEY_F9,
    F10 = GLFW_KEY_F10,
    F11 = GLFW_KEY_F11,
    F12 = GLFW_KEY_F12
};

class InputSystem {
public:
    using KeyCallback = std::function<void()>;

    void bindKey(Key key, KeyCallback callback, KeyAction action = KeyAction::Press);
    bool isKeyDown(Key key) const;
    bool isKeyPressed(Key key) const;
    bool isKeyReleased(Key key) const;
    void resetFrameState();
    void handleKeyEvent(int key, int action);

private:
    std::unordered_map<int, bool> m_keyDown;
    std::unordered_map<int, bool> m_keyPressed;
    std::unordered_map<int, bool> m_keyReleased;
    std::unordered_map<int, std::vector<KeyCallback>> m_keyPressCallbacks;
    std::unordered_map<int, std::vector<KeyCallback>> m_keyReleaseCallbacks;
};

} // namespace Xangine
