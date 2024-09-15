#include "menu.h"
#include "../features/visuals/visuals.h"
#include "../globals/offsets.h"
#include "../globals/globals.h"

ImVec4 espColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
bool showMenu = true;
int activeTab = 0;

extern ID3D11Device* pDevice;
extern ID3D11DeviceContext* pContext;
extern ID3D11RenderTargetView* mainRenderTargetView;
extern HWND window;

void ApplyImGuiTheme()
{
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding = 2.0f;

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.10f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.27f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.50f, 0.75f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.60f, 0.85f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.45f, 0.70f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.10f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.20f, 0.30f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.30f, 0.40f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.25f, 0.50f, 0.75f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.60f, 0.85f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.45f, 0.70f, 1.0f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.50f, 0.75f, 1.0f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.60f, 0.85f, 1.0f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.20f, 0.45f, 0.70f, 1.0f);

    style.ItemSpacing = ImVec2(10, 5);
    style.FramePadding = ImVec2(5, 5);
}

void InitImGui()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(pDevice, pContext);

    ApplyImGuiTheme();

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16.0f);
}

void RenderImGui()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();

    if (esp) {
        Visuals::PlayerESP();
    }

    if (showNameTags) {
        Visuals::PlayerNameTags();
    }

    if (showTracers) {
        Visuals::PlayerTracers();
    }

    if (showMenu) {
        ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
        ImGui::Begin("TempleWare | Internal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);

        ImGui::BeginChild("Side Menu", ImVec2(200, 0), true);
        ImGui::Text("TempleWare | v2.0.1");
        ImGui::Separator();
        if (ImGui::Button("Aim", ImVec2(-1, 40))) activeTab = 0;
        if (ImGui::Button("Visual", ImVec2(-1, 40))) activeTab = 1;
        if (ImGui::Button("Misc", ImVec2(-1, 40))) activeTab = 2;
        ImGui::Separator();

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Main Content", ImVec2(0, 0), true);
        switch (activeTab) {
        case 0:
            ImGui::Text("Aim");
            ImGui::Separator();

            break;

        case 1:
            ImGui::Text("Visual");
            ImGui::Separator();
            ImGui::Checkbox("ESP", &esp);
            if (esp) {
                ImGui::ColorEdit4("Color", (float*)&espColor);
                ImGui::SliderFloat("Thickness", &espThickness, 1.0f, 5.0f);
            }
            ImGui::Separator();

            ImGui::Checkbox("Tracers", &showTracers);
            ImGui::Separator();

            ImGui::Checkbox("NameTags", &showNameTags);
            if (showNameTags) {
                ImGui::Checkbox("Health", &showHealth);
            }
            ImGui::Separator();
            break;

        case 2:
            ImGui::Text("Misc");
            ImGui::Separator();
            break;
        }
        ImGui::EndChild();

        ImGui::End();
    }

    ImGui::Render();
    pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CleanupImGui()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
