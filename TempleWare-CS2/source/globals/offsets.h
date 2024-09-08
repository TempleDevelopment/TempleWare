#pragma once

#include <cstddef>
#include <cstdint>

namespace Offsets
{
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x17C37F0;
	constexpr std::ptrdiff_t dwEntityList = 0x1956A68;
	constexpr std::ptrdiff_t dwViewMatrix = 0x19B89F0;
	constexpr std::ptrdiff_t dwLocalPlayerController = 0x19A66C8;

	constexpr std::ptrdiff_t m_hPlayerPawn = 0x7EC;

	constexpr std::ptrdiff_t m_iHealth = 0x324;
	constexpr std::ptrdiff_t m_iTeamNum = 0x3C3;
	constexpr std::ptrdiff_t m_vOldOrigin = 0x1274;
	constexpr std::ptrdiff_t m_iFOV = 0x210;
}