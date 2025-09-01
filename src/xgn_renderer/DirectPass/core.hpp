// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef DIRECT_PASS_CORE
#define DIRECT_PASS_CORE

#include <xgn_renderer/shader_settings.hpp>

namespace DirectPass {

inline constexpr const char* Invert_Enabled = "directpass.invert.enabled";

void setup_passes() {
    if (Invert_Enabled) {
        _invert_shader = new Invert_Shas
    }
}

}

#endif // DIRECT_PASS_CORE