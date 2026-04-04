#pragma once

#include <string>
#include <Xangine/Data/VersionNumber.hpp>

namespace Xangine {

class Version {
public:
    static std::string get() {
        return "V" + std::to_string(VersionNumber::MAJOR) + "." + 
                     std::to_string(VersionNumber::MINOR) + "." + 
                     std::to_string(VersionNumber::PATCH) + "-" + 
                     VersionNumber::PRE_RELEASE;
    }
    
    static int getCode() {
        return (VersionNumber::MAJOR << 16) | 
               (VersionNumber::MINOR << 8) | 
               VersionNumber::PATCH;
    }
    
    static std::string getString() {
        return get();
    }
    
    static std::string getFull() {
        return "Xangine V" + std::to_string(VersionNumber::MAJOR) + "." + 
                              std::to_string(VersionNumber::MINOR) + "." + 
                              std::to_string(VersionNumber::PATCH) + " " + 
                              VersionNumber::PRE_RELEASE;
    }
    
    static std::string getShort() {
        return std::to_string(VersionNumber::MAJOR) + "." + 
               std::to_string(VersionNumber::MINOR) + "." + 
               std::to_string(VersionNumber::PATCH);
    }
    
    static bool isPreRelease() {
        return VersionNumber::PRE_RELEASE[0] != '\0';
    }
    
    static const char* getPreReleaseTag() {
        return VersionNumber::PRE_RELEASE;
    }
};

} // namespace Xangine