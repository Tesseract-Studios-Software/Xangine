// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#pragma once
#include <string>
#include <unordered_map>
#include <variant>

// Define the types of settings we can have
using SettingValue = std::variant<int, float, bool, std::string>;

class EngineSettings {
public:
    template<typename T>
    void set(const std::string& key, const T& value) {
        _settings[key] = value;
    }

    template<typename T>
    T get(const std::string& key, const T& defaultValue = T()) const {
        auto it = _settings.find(key);
        if (it != _settings.end()) {
            // Attempt to get the value if it's the correct type
            if (const T* value = std::get_if<T>(&it->second)) {
                return *value;
            }
        }
        return defaultValue;
    }

    // Check if a setting exists (useful for toggling features)
    bool has(const std::string& key) const {
        return _settings.find(key) != _settings.end();
    }

private:
    std::unordered_map<std::string, SettingValue> _settings;
};