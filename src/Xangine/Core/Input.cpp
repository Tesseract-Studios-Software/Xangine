#include <Xangine/Core/Input.hpp>

namespace Xangine {

void InputSystem::bindKey(Key key, KeyCallback callback, KeyAction action) {
    int keyCode = static_cast<int>(key);
    if (action == KeyAction::Press) {
        m_keyPressCallbacks[keyCode].push_back(std::move(callback));
    } else {
        m_keyReleaseCallbacks[keyCode].push_back(std::move(callback));
    }
}

bool InputSystem::isKeyDown(Key key) const {
    auto it = m_keyDown.find(static_cast<int>(key));
    return it != m_keyDown.end() && it->second;
}

bool InputSystem::isKeyPressed(Key key) const {
    auto it = m_keyPressed.find(static_cast<int>(key));
    return it != m_keyPressed.end() && it->second;
}

bool InputSystem::isKeyReleased(Key key) const {
    auto it = m_keyReleased.find(static_cast<int>(key));
    return it != m_keyReleased.end() && it->second;
}

void InputSystem::resetFrameState() {
    for (auto& entry : m_keyPressed) {
        entry.second = false;
    }
    for (auto& entry : m_keyReleased) {
        entry.second = false;
    }
}

void InputSystem::handleKeyEvent(int key, int action) {
    if (action == GLFW_PRESS) {
        m_keyDown[key] = true;
        m_keyPressed[key] = true;
        for (const auto& callback : m_keyPressCallbacks[key]) {
            if (callback) {
                callback();
            }
        }
    } else if (action == GLFW_RELEASE) {
        m_keyDown[key] = false;
        m_keyReleased[key] = true;
        for (const auto& callback : m_keyReleaseCallbacks[key]) {
            if (callback) {
                callback();
            }
        }
    }
}

} // namespace Xangine
