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
    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabRounding = 0.0f;

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.3f, 0.15f, 0.6f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.4f, 0.2f, 0.6f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.3f, 0.7f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.2f, 0.6f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3f, 0.3f, 0.5f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.4f, 0.2f, 0.6f, 1.0f);
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

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 18.0f);
}

void RenderImGui()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = showMenu;

    if (esp) {
        Visuals::PlayerESP();
    }

    if (showNameTags) {
        Visuals::PlayerNameTags();
    }

    if (showMenu) {
        ImGui::SetNextWindowFocus();
        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Once);
        ImGui::Begin("TempleWare v2.0", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        ImGui::BeginChild("Side Menu", ImVec2(150, 0), true);
        if (ImGui::Button("Exit", ImVec2(-1, 30))) {
            showMenu = false;
            CleanupImGui();
            kiero::shutdown();
        }
        ImGui::Separator();
        if (ImGui::Button("Visuals", ImVec2(-1, 30))) activeTab = 0;
        if (ImGui::Button("Misc", ImVec2(-1, 30))) activeTab = 1;
        if (ImGui::Button("Config", ImVec2(-1, 30))) activeTab = 2;
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("Main Content", ImVec2(0, 0), false);

        switch (activeTab) {
        case 0:
            ImGui::Text("Visuals");
            ImGui::Checkbox("ESP", &esp);
            if (esp) {
                ImGui::ColorEdit4("Color", (float*)&espColor);
                ImGui::SliderFloat("Thickness", &espThickness, 1.0f, 5.0f);
            }
            ImGui::Separator();

            ImGui::Checkbox("Name Tags", &showNameTags);
            if (showNameTags) {
                ImGui::Checkbox("Health", &showHealth);
                ImGui::Checkbox("Outline", &showOutline);
            }
            ImGui::Separator();
            break;

        case 1:
            ImGui::Text("Miscellaneous");
            break;
        case 2:
            ImGui::Text("Configuration");
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
