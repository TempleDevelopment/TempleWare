#include "visuals.h"
#include "../../globals/globals.h"
#include "../../globals/offsets.h"
#include "../../math/math.h"
#include <cstdint>
#include <cstdio>

extern ImVec4 espColor;
extern float espThickness;
extern bool showHealth;

void Visuals::PlayerESP()
{
    uintptr_t client = (uintptr_t)GetModuleHandle("client.dll");

    float(*ViewMatrix)[4][4] = (float(*)[4][4])(client + Offsets::dwViewMatrix);

    auto localpawn = *(uintptr_t*)(client + Offsets::dwLocalPlayerPawn);
    if (!localpawn)
        return;

    auto localTeam = *(int*)(localpawn + Offsets::m_iTeamNum);
    auto entitylist = *(uintptr_t*)(client + Offsets::dwEntityList);
    if (!entitylist)
        return;

    for (int i = 1; i < 64; i++)
    {
        uintptr_t list_entry1 = *(uintptr_t*)(entitylist + (8 * (i & 0x7FFF) >> 9) + 16);
        if (!list_entry1)
            continue;
        uintptr_t playerController = *(uintptr_t*)(list_entry1 + 120 * (i & 0x1FF));
        if (!playerController)
            continue;
        uint32_t playerPawn = *(uint32_t*)(playerController + Offsets::m_hPlayerPawn);
        if (!playerPawn)
            continue;
        uintptr_t list_entry2 = *(uintptr_t*)(entitylist + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
        if (!list_entry2)
            continue;
        uintptr_t pCSPlayerPawnPtr = *(uintptr_t*)(list_entry2 + 120 * (playerPawn & 0x1FF));
        if (!pCSPlayerPawnPtr)
            continue;

        int health = *(int*)(pCSPlayerPawnPtr + Offsets::m_iHealth);
        if (!health || health > 100)
            continue;

        int team = *(int*)(pCSPlayerPawnPtr + Offsets::m_iTeamNum);
        if (team == localTeam)
            continue;

        Vec3 feetpos = *(Vec3*)(pCSPlayerPawnPtr + Offsets::m_vOldOrigin);
        Vec3 headpos = { feetpos.x, feetpos.y, feetpos.z + 70.0f };

        Vec2 feet, head;

        if (feetpos.WorldToScreen(feet, ViewMatrix) && headpos.WorldToScreen(head, ViewMatrix))
        {
            float height = (feet.y - head.y) / 1.5f;
            float width = height / 1.5f;
            float x = feet.x - width / 2;

            ImGui::GetBackgroundDrawList()->AddRect(
                { x, head.y },
                { x + width, feet.y },
                ImColor(espColor.x * 255, espColor.y * 255, espColor.z * 255, espColor.w * 255),
                0.0f, 0, espThickness
            );
        }
    }
}

void Visuals::PlayerNameTags()
{
    uintptr_t client = (uintptr_t)GetModuleHandle("client.dll");

    float(*ViewMatrix)[4][4] = (float(*)[4][4])(client + Offsets::dwViewMatrix);

    auto localpawn = *(uintptr_t*)(client + Offsets::dwLocalPlayerPawn);
    if (!localpawn)
        return;

    auto localTeam = *(int*)(localpawn + Offsets::m_iTeamNum);
    auto entitylist = *(uintptr_t*)(client + Offsets::dwEntityList);
    if (!entitylist)
        return;

    for (int i = 1; i < 64; i++)
    {
        uintptr_t list_entry1 = *(uintptr_t*)(entitylist + (8 * (i & 0x7FFF) >> 9) + 16);
        if (!list_entry1)
            continue;
        uintptr_t playerController = *(uintptr_t*)(list_entry1 + 120 * (i & 0x1FF));
        if (!playerController)
            continue;
        uint32_t playerPawn = *(uint32_t*)(playerController + Offsets::m_hPlayerPawn);
        if (!playerPawn)
            continue;
        uintptr_t list_entry2 = *(uintptr_t*)(entitylist + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
        if (!list_entry2)
            continue;
        uintptr_t pCSPlayerPawnPtr = *(uintptr_t*)(list_entry2 + 120 * (playerPawn & 0x1FF));
        if (!pCSPlayerPawnPtr)
            continue;

        int health = *(int*)(pCSPlayerPawnPtr + Offsets::m_iHealth);
        if (!health || health > 100)
            continue;

        int team = *(int*)(pCSPlayerPawnPtr + Offsets::m_iTeamNum);
        if (team == localTeam)
            continue;

        Vec3 feetpos = *(Vec3*)(pCSPlayerPawnPtr + Offsets::m_vOldOrigin);
        Vec3 headpos = { feetpos.x, feetpos.y, feetpos.z + 70.0f };

        Vec2 feet, head;

        if (feetpos.WorldToScreen(feet, ViewMatrix) && headpos.WorldToScreen(head, ViewMatrix))
        {
            char nameText[32];
            snprintf(nameText, sizeof(nameText), "Player %d", i);

            ImVec2 namePos = ImVec2(feet.x - 50, head.y - 20);
            ImVec2 healthPos = ImVec2(feet.x + 20, head.y - 20);

            if (showOutline)
            {
                ImVec2 nameOutlinePos = ImVec2(namePos.x - 1, namePos.y - 1);
                ImVec2 healthOutlinePos = ImVec2(healthPos.x - 1, healthPos.y - 1);

                ImGui::GetBackgroundDrawList()->AddText(nameOutlinePos, IM_COL32(0, 0, 0, 255), nameText);
                if (showHealth)
                    ImGui::GetBackgroundDrawList()->AddText(healthOutlinePos, IM_COL32(0, 0, 0, 255), "100 HP");
            }

            ImGui::GetBackgroundDrawList()->AddText(namePos, IM_COL32(255, 255, 255, 255), nameText);
            if (showHealth)
            {
                char healthText[16];
                snprintf(healthText, sizeof(healthText), "%d HP", health);
                ImGui::GetBackgroundDrawList()->AddText(healthPos, IM_COL32(255, 255, 255, 255), healthText);
            }
        }
    }
}