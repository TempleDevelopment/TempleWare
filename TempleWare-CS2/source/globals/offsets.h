#pragma once

#include <cstddef>
#include <cstdint>

namespace Offsets
{
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x17C17F0;
	constexpr std::ptrdiff_t dwEntityList = 0x1954568;
	constexpr std::ptrdiff_t dwViewMatrix = 0x19B64F0;
	constexpr std::ptrdiff_t dwLocalPlayerController = 0x19A41C8;

	constexpr std::ptrdiff_t m_hPlayerPawn = 0x7EC;

	constexpr std::ptrdiff_t m_iHealth = 0x324;
	constexpr std::ptrdiff_t m_iTeamNum = 0x3C3;
	constexpr std::ptrdiff_t m_vOldOrigin = 0x1274;
	constexpr std::ptrdiff_t m_iFOV = 0x210;
}