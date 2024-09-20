#include "visuals.h"
#include "../../globals/globals.h"
#include "../../globals/offsets.h"
#include "../../math/math.h"
#include <cstdint>
#include <cstdio>

extern ImVec4 glowColor;
extern ImVec4 tracersColor;
extern ImVec4 espColor;
extern float espThickness;
extern bool showHealth;

Offset offsets;

void Visuals::PlayerESP()
{
    uintptr_t client = (uintptr_t)GetModuleHandle("client.dll");

    float(*ViewMatrix)[4][4] = (float(*)[4][4])(client + offsets.GetOffset("dwViewMatrix"));

    auto localpawn = *(uintptr_t*)(client + offsets.GetOffset("dwLocalPlayerPawn"));
    if (!localpawn)
        return;

    auto localTeam = *(int*)(localpawn + offsets.GetOffset("m_iTeamNum"));
    auto entitylist = *(uintptr_t*)(client + offsets.GetOffset("dwEntityList"));
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
        uint32_t playerPawn = *(uint32_t*)(playerController + offsets.GetOffset("m_hPlayerPawn"));
        if (!playerPawn)
            continue;
        uintptr_t list_entry2 = *(uintptr_t*)(entitylist + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
        if (!list_entry2)
            continue;
        uintptr_t pCSPlayerPawnPtr = *(uintptr_t*)(list_entry2 + 120 * (playerPawn & 0x1FF));
        if (!pCSPlayerPawnPtr)
            continue;

        int health = *(int*)(pCSPlayerPawnPtr + offsets.GetOffset("m_iHealth"));
        if (!health || health > 100)
            continue;

        int team = *(int*)(pCSPlayerPawnPtr + offsets.GetOffset("m_iTeamNum"));
        if (team == localTeam)
            continue;

        Vec3 feetpos = *(Vec3*)(pCSPlayerPawnPtr + offsets.GetOffset("m_vOldOrigin"));
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

    float(*ViewMatrix)[4][4] = (float(*)[4][4])(client + offsets.GetOffset("dwViewMatrix"));

    auto localpawn = *(uintptr_t*)(client + offsets.GetOffset("dwLocalPlayerPawn"));
    if (!localpawn)
        return;

    auto localTeam = *(int*)(localpawn + offsets.GetOffset("m_iTeamNum"));
    auto entitylist = *(uintptr_t*)(client + offsets.GetOffset("dwEntityList"));
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
        uint32_t playerPawn = *(uint32_t*)(playerController + offsets.GetOffset("m_hPlayerPawn"));
        if (!playerPawn)
            continue;
        uintptr_t list_entry2 = *(uintptr_t*)(entitylist + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
        if (!list_entry2)
            continue;
        uintptr_t pCSPlayerPawnPtr = *(uintptr_t*)(list_entry2 + 120 * (playerPawn & 0x1FF));
        if (!pCSPlayerPawnPtr)
            continue;

        int health = *(int*)(pCSPlayerPawnPtr + offsets.GetOffset("m_iHealth"));
        if (!health || health > 100)
            continue;

        int team = *(int*)(pCSPlayerPawnPtr + offsets.GetOffset("m_iTeamNum"));
        if (team == localTeam)
            continue;

        Vec3 feetpos = *(Vec3*)(pCSPlayerPawnPtr + offsets.GetOffset("m_vOldOrigin"));
        Vec3 headpos = { feetpos.x, feetpos.y, feetpos.z + 70.0f };

        Vec2 feet, head;

        if (feetpos.WorldToScreen(feet, ViewMatrix) && headpos.WorldToScreen(head, ViewMatrix))
        {
            if (showHealth)
            {
                char healthText[16];
                snprintf(healthText, sizeof(healthText), "%d HP", health);

                ImVec2 healthPos = ImVec2(feet.x - 30, head.y - 25);

                ImColor textColor = IM_COL32(255, 255, 255, 255);
                ImColor bgColor = IM_COL32(0, 0, 0, 180);

                ImGui::GetBackgroundDrawList()->AddRectFilled(
                    ImVec2(healthPos.x - 10, healthPos.y - 10),
                    ImVec2(healthPos.x + ImGui::CalcTextSize(healthText).x + 10, healthPos.y + ImGui::CalcTextSize(healthText).y + 5),
                    bgColor,
                    5.0f
                );

                ImGui::GetBackgroundDrawList()->AddText(healthPos, textColor, healthText);
            }
        }
    }
}

void Visuals::PlayerTracers()
{
    if (!showTracers)
        return;

    uintptr_t client = (uintptr_t)GetModuleHandle("client.dll");

    float(*ViewMatrix)[4][4] = (float(*)[4][4])(client + offsets.GetOffset("dwViewMatrix"));

    auto localpawn = *(uintptr_t*)(client + offsets.GetOffset("dwLocalPlayerPawn"));
    if (!localpawn)
        return;

    Vec2 screenCenter = { (float)GetSystemMetrics(SM_CXSCREEN) / 2, (float)GetSystemMetrics(SM_CYSCREEN) / 2 };

    auto localTeam = *(int*)(localpawn + offsets.GetOffset("m_iTeamNum"));
    auto entitylist = *(uintptr_t*)(client + offsets.GetOffset("dwEntityList"));
    if (!entitylist)
        return;

    ImColor lineColor = ImColor(tracersColor.x * 255, tracersColor.y * 255, tracersColor.z * 255, tracersColor.w * 255);

    for (int i = 1; i < 64; i++)
    {
        uintptr_t list_entry1 = *(uintptr_t*)(entitylist + (8 * (i & 0x7FFF) >> 9) + 16);
        if (!list_entry1)
            continue;

        uintptr_t playerController = *(uintptr_t*)(list_entry1 + 120 * (i & 0x1FF));
        if (!playerController)
            continue;

        uint32_t playerPawn = *(uint32_t*)(playerController + offsets.GetOffset("m_hPlayerPawn"));
        if (!playerPawn)
            continue;

        uintptr_t list_entry2 = *(uintptr_t*)(entitylist + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
        if (!list_entry2)
            continue;

        uintptr_t pCSPlayerPawnPtr = *(uintptr_t*)(list_entry2 + 120 * (playerPawn & 0x1FF));
        if (!pCSPlayerPawnPtr)
            continue;

        int team = *(int*)(pCSPlayerPawnPtr + offsets.GetOffset("m_iTeamNum"));
        if (team == localTeam)
            continue;

        int health = *(int*)(pCSPlayerPawnPtr + offsets.GetOffset("m_iHealth"));
        if (health <= 0)
            continue;

        Vec3 feetpos = *(Vec3*)(pCSPlayerPawnPtr + offsets.GetOffset("m_vOldOrigin"));
        Vec2 feet;

        if (feetpos.WorldToScreen(feet, ViewMatrix))
        {
            ImGui::GetBackgroundDrawList()->AddLine(
                { screenCenter.x, screenCenter.y },
                { feet.x, feet.y },
                lineColor,
                espThickness
            );
        }
    }
}

void Visuals::PlayerGlow()
{
    uintptr_t client = (uintptr_t)GetModuleHandle("client.dll");
    auto localpawn = *(uintptr_t*)(client + offsets_.GetOffset("dwLocalPlayerPawn"));
    if (!localpawn)
        return;

    auto localTeam = *(int*)(localpawn + offsets_.GetOffset("m_iTeamNum"));
    auto entitylist = *(uintptr_t*)(client + offsets_.GetOffset("dwEntityList"));
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

        uint32_t playerPawn = *(uint32_t*)(playerController + offsets_.GetOffset("m_hPlayerPawn"));
        if (!playerPawn)
            continue;

        uintptr_t list_entry2 = *(uintptr_t*)(entitylist + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
        if (!list_entry2)
            continue;

        uintptr_t pCSPlayerPawnPtr = *(uintptr_t*)(list_entry2 + 120 * (playerPawn & 0x1FF));
        if (!pCSPlayerPawnPtr)
            continue;

        int team = *(int*)(pCSPlayerPawnPtr + offsets_.GetOffset("m_iTeamNum"));
        if (team == localTeam)
            continue;

        int health = *(int*)(pCSPlayerPawnPtr + offsets_.GetOffset("m_iHealth"));
        if (health <= 0)
            continue;

        DWORD colorArgb = ((DWORD)(glowColor.w * 255) << 24) |
            ((DWORD)(glowColor.z * 255) << 16) |
            ((DWORD)(glowColor.y * 255) << 8) |
            ((DWORD)(glowColor.x * 255));

        *(DWORD*)(pCSPlayerPawnPtr + offsets_.GetOffset("m_Glow") + offsets_.GetOffset("m_glowColorOverride")) = colorArgb;
        *(DWORD*)(pCSPlayerPawnPtr + offsets_.GetOffset("m_Glow") + offsets_.GetOffset("m_bGlowing")) = 1;
    }
}
