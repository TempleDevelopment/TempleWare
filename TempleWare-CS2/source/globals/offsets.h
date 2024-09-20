#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <string>

class Offset {
public:
    struct Offsets {
        std::ptrdiff_t dwLocalPlayerPawn;
        std::ptrdiff_t dwEntityList;
        std::ptrdiff_t dwViewMatrix;
        std::ptrdiff_t dwLocalPlayerController;
        std::ptrdiff_t m_hPlayerPawn;
        std::ptrdiff_t m_iHealth;
        std::ptrdiff_t m_iTeamNum;
        std::ptrdiff_t m_vOldOrigin;
        std::ptrdiff_t m_iFOV;
        std::ptrdiff_t m_bGlowing;
        std::ptrdiff_t m_glowColorOverride;
        std::ptrdiff_t m_Glow;
    };

    Offset() {
        offsets_ = {
            {"dwLocalPlayerPawn", 0x17C17F0},
            {"dwEntityList", 0x1954568},
            {"dwViewMatrix", 0x19B64F0},
            {"dwLocalPlayerController", 0x19A41C8},
            {"m_hPlayerPawn", 0x7EC},
            {"m_iHealth", 0x324},
            {"m_iTeamNum", 0x3C3},
            {"m_vOldOrigin", 0x1274},
            {"m_iFOV", 0x210},
            {"m_bGlowing", 0x51},
            {"m_glowColorOverride", 0x40},
            {"m_Glow", 0xBA0},
        };
    }

    std::ptrdiff_t GetOffset(const std::string& name) const {
        auto it = offsets_.find(name);
        return it != offsets_.end() ? it->second : 0;
    }

private:
    std::unordered_map<std::string, std::ptrdiff_t> offsets_;
};
