#pragma once
#include <vector>
#include <D3D11.h>
#include <d3d9types.h>
#include <dwmapi.h>
#include <d3d11.h>

#include "../skcrypt.hpp"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include <time.h>
#include <string>

#include "../Cheat.hpp"
#pragma comment(lib, "d3dx11.lib")

int currentItem = 0;
int ITEM = 0;
float FontSize = 16.f;

ImDrawList* DrawData;
ImFont* ubuntu_bold_font = nullptr;
ImFont* ubuntu_medium_font = nullptr;
ImFont* ubuntu_regular_font = nullptr;
ImFont* expand_font = nullptr;

ImFont* SetFont = NULL;

ImFont* Arial;
ImFont* Calibri;
ImFont* Ebrima;
ImFont* Tahoma;
ImFont* Comic;
enum RENDER_INFORMATION : int {
	RENDER_HIJACK_FAILED = 0,
	RENDER_IMGUI_FAILED = 1,
	RENDER_DRAW_FAILED = 2,
	RENDER_SETUP_SUCCESSFUL = 3,
    RENDER_COD_FAILED = 4,
};


using namespace ImGui;
int Segments = 10;
HWND window_handle;

ID3D11Device* d3d_device;
ID3D11DeviceContext* d3d_device_ctx;
IDXGISwapChain* d3d_swap_chain;
ID3D11RenderTargetView* d3d_render_target;
D3DPRESENT_PARAMETERS d3d_present_params;
#define vec4( r, g, b, a ) ImColor( r / 255.f, g / 255.f, b / 255.f, a )

const char* items[] = { "Arial", "Calibri", "Ebrima", "Tahoma" };

enum e_fonts : int {

    REGULAR = 0,
    MEDIUM,
    BOLD,
    LOGO
};
int FrameRate()
{
    static int iFps, iLastFps;
    static float flLastTickCount, flTickCount;
    flTickCount = clock() * 0.001f;
    iFps++;
    if ((flTickCount - flLastTickCount) >= 1.0f) {
        flLastTickCount = flTickCount;
        iLastFps = iFps;
        iFps = 0;
    }
    return iLastFps;
}
using namespace std;
void HelpMarker(const char* desc) {
    ImGui::TextDisabled(("[?]"));
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();

    }
}

void draw_text_outline_font_Font(ImFont* Font, float Size, float x, float y, ImColor color, const char* string) {
    char buf[512];
    ImVec2 len = ImGui::CalcTextSize(string);
    ImGui::GetBackgroundDrawList()->AddText(Font, Size, ImVec2(x - 1, y), ImColor(0.0f, 0.0f, 0.0f, 255.f), string);
    ImGui::GetBackgroundDrawList()->AddText(Font, Size, ImVec2(x + 1, y), ImColor(0.0f, 0.0f, 0.0f, 255.f), string);
    ImGui::GetBackgroundDrawList()->AddText(Font, Size, ImVec2(x, y - 1), ImColor(0.0f, 0.0f, 0.0f, 255.f), string);
    ImGui::GetBackgroundDrawList()->AddText(Font, Size, ImVec2(x, y + 1), ImColor(0.0f, 0.0f, 0.0f, 255.f), string);

    ImGui::GetBackgroundDrawList()->AddText(Font, Size, ImVec2(x, y), color, string);

}
bool drawmenu = true;

bool showMouse = false;
int* a;
bool weaponScreen = false;
bool attachmentsScreen = false;
bool armorScreen = false;
bool otherScreen = false;
HWND hwnd;
ImFont* DefeatureFont;



namespace render {
    class c_render {

        HWND fortnite_window = { };

    public:

        auto Setup() -> RENDER_INFORMATION {

            fortnite_window = FindWindowA(skCrypt("UnityWndClass").decrypt(), skCrypt("Rust").decrypt());
            if (!fortnite_window) {
                printf("[Client] Fail to Get Window\n");
                return RENDER_COD_FAILED;
            }

            if (!render::c_render::Hijack()) return RENDER_HIJACK_FAILED;

            if (!render::c_render::ImGui()) return RENDER_IMGUI_FAILED;
            printf("[+] Hijacked Window\n");
            return RENDER_SETUP_SUCCESSFUL;

        }

        auto get_screen_status() -> bool
        {
            if (this->fortnite_window == GetForegroundWindow()) {
                return true;
            }

            if (this->fortnite_window == GetActiveWindow()) {
                return true;
            }

            if (GetActiveWindow() == GetForegroundWindow()) {
                return true;
            }

            return false;
        }

        auto Render() -> bool {

            static RECT rect_og;
            MSG msg = { NULL };
            ZeroMemory(&msg, sizeof(MSG));

            while (msg.message != WM_QUIT)
            {

                UpdateWindow(window_handle);
                ShowWindow(window_handle, SW_SHOW);

                if ((PeekMessageA)(&msg, window_handle, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                ImGuiIO& io = ImGui::GetIO();
                io.ImeWindowHandle = window_handle;
                io.DeltaTime = 1.0f / 60.0f;


                POINT p_cursor;
                GetCursorPos(&p_cursor);
                io.MousePos.x = p_cursor.x;
                io.MousePos.y = p_cursor.y;

                if (GetAsyncKeyState(VK_LBUTTON)) {
                    io.MouseDown[0] = true;
                    io.MouseClicked[0] = true;
                    io.MouseClickedPos[0].x = io.MousePos.x;
                    io.MouseClickedPos[0].x = io.MousePos.y;
                }
                else
                    io.MouseDown[0] = false;

                render::c_render::Draw();
               
            }
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();

            (DestroyWindow)(window_handle);
            printf("[+] Render Setup\n");
            return true;

        }

        auto ImGui() -> bool {

            DXGI_SWAP_CHAIN_DESC swap_chain_description;
            ZeroMemory(&swap_chain_description, sizeof(swap_chain_description));
            swap_chain_description.BufferCount = 2;
            swap_chain_description.BufferDesc.Width = 0;
            swap_chain_description.BufferDesc.Height = 0;
            swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            swap_chain_description.BufferDesc.RefreshRate.Numerator = 60;
            swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
            swap_chain_description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
            swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swap_chain_description.OutputWindow = window_handle;
            swap_chain_description.SampleDesc.Count = 1;
            swap_chain_description.SampleDesc.Quality = 0;
            swap_chain_description.Windowed = 1;
            swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

            D3D_FEATURE_LEVEL d3d_feature_lvl;

            const D3D_FEATURE_LEVEL d3d_feature_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

            D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, d3d_feature_array, 2, D3D11_SDK_VERSION, &swap_chain_description, &d3d_swap_chain, &d3d_device, &d3d_feature_lvl, &d3d_device_ctx);

            ID3D11Texture2D* pBackBuffer;

            d3d_swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

            d3d_device->CreateRenderTargetView(pBackBuffer, NULL, &d3d_render_target);

            pBackBuffer->Release();

            IMGUI_CHECKVERSION();

            ImGui::CreateContext();

            ImGuiIO& io = ImGui::GetIO();

            // Our state
            ImVec4 clear_color = GetStyleColorVec4(ImGuiCol_WindowBg);
            io.IniFilename = NULL;

            (void)io;

            ImGui::StyleColorsDark();

            auto& Style = ImGui::GetStyle();

            Style.WindowPadding = ImVec2(11.000f, 9.000f);
            Style.WindowBorderSize = 0.000f;
            Style.FramePadding = ImVec2(4.000f, 6.000f);
            Style.FrameBorderSize = 1.000f;
            Style.IndentSpacing = 20.000f;
            Style.TabBorderSize = 1.000f;

            Style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
            Style.Colors[ImGuiCol_WindowBg] = ImVec4(0.083f, 0.083f, 0.083f, 1.0f);
            Style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
            Style.Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
            Style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.1f, 0.1f, 1.000f);
            Style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.000f);
            Style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.000f);


            ImGui_ImplWin32_Init(window_handle);

            ImGui_ImplDX11_Init(d3d_device, d3d_device_ctx);

            d3d_device->Release();

            return true;

        }

        auto Hijack() -> bool
        {

            window_handle = FindWindowA(skCrypt("MedalOverlayClass").decrypt(), skCrypt("MedalOverlay").decrypt());
            if (!window_handle) return false;
            MARGINS Margin = { -1 };
            DwmExtendFrameIntoClientArea(window_handle, &Margin);
            SetWindowPos(window_handle, 0, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
            ShowWindow(window_handle, SW_SHOW);
            UpdateWindow(window_handle);
            return true;
        }
        RECT rect;
        bool StartCheat = false;
        int Slec = 0;
        auto Menu() -> void
        {
         
            ImGui::SetNextWindowSize(ImVec2(750.000f, 500.000f), ImGuiCond_Once);
            ImGui::Begin("rename me 0", NULL, 155);

            ImVec2 P1, P2;
            ImDrawList* pDrawList;
            const auto& CurrentWindowPos = ImGui::GetWindowPos();
            const auto& pWindowDrawList = ImGui::GetWindowDrawList();
            const auto& pBackgroundDrawList = ImGui::GetBackgroundDrawList();
            const auto& pForegroundDrawList = ImGui::GetForegroundDrawList();

            P1 = ImVec2(1.000f, 0.000f);
            P1.x += CurrentWindowPos.x;
            P1.y += CurrentWindowPos.y;
            P2 = ImVec2(750.000f, 25.000f);
            P2.x += CurrentWindowPos.x;
            P2.y += CurrentWindowPos.y;
            pDrawList = pWindowDrawList;
            pDrawList->AddRectFilled(P1, P2, ImColor(0.122f, 0.119f, 0.119f, 1.000f), 0.000f);

            P1 = ImVec2(5.000f, 30.000f);
            P1.x += CurrentWindowPos.x;
            P1.y += CurrentWindowPos.y;
            P2 = ImVec2(745.000f, 75.000f);
            P2.x += CurrentWindowPos.x;
            P2.y += CurrentWindowPos.y;
            pDrawList = pWindowDrawList;
            pDrawList->AddRect(P1, P2, ImColor(1.000f, 1.000f, 1.000f, 1.000f), 0.000f);

            P1 = ImVec2(5.000f, 490.000f);
            P1.x += CurrentWindowPos.x;
            P1.y += CurrentWindowPos.y;
            P2 = ImVec2(745.000f, 80.000f);
            P2.x += CurrentWindowPos.x;
            P2.y += CurrentWindowPos.y;
            pDrawList = pWindowDrawList;
            pDrawList->AddRect(P1, P2, ImColor(1.000f, 1.000f, 1.000f, 1.000f), 0.000f);

            P1 = ImVec2(5.000f, 25.000f);
            P1.x += CurrentWindowPos.x;
            P1.y += CurrentWindowPos.y;
            P2 = ImVec2(745.000f, 2.000f);
            P2.x += CurrentWindowPos.x;
            P2.y += CurrentWindowPos.y;
            pDrawList = pWindowDrawList;
            pDrawList->AddRect(P1, P2, ImColor(1.000f, 1.000f, 1.000f, 1.000f), 0.000f);

            P1 = ImVec2(7.000f, 32.000f);
            P1.x += CurrentWindowPos.x;
            P1.y += CurrentWindowPos.y;
            P2 = ImVec2(743.000f, 74.000f);
            P2.x += CurrentWindowPos.x;
            P2.y += CurrentWindowPos.y;
            pDrawList = pWindowDrawList;
            pDrawList->AddRectFilled(P1, P2, ImColor(0.099f, 0.099f, 0.099f, 1.000f), 0.000f);

            P1 = ImVec2(7.000f, 81.000f);
            P1.x += CurrentWindowPos.x;
            P1.y += CurrentWindowPos.y;
            P2 = ImVec2(743.000f, 490.000f);
            P2.x += CurrentWindowPos.x;
            P2.y += CurrentWindowPos.y;
            pDrawList = pWindowDrawList;
            pDrawList->AddRectFilled(P1, P2, ImColor(0.099f, 0.099f, 0.099f, 1.000f), 0.000f);

                ImGui::TextColored(ImColor(255,0,100,255),"Rust Private External");            

            
                ImGui::SetCursorPos(ImVec2(10, 37));
            if (ImGui::Button("Visuals", ImVec2(138, 30)))
                Slec = 0;

            ImGui::SameLine();
            if (ImGui::Button("Combat", ImVec2(138, 30)))
                Slec = 1;
            ImGui::SameLine();
            if (ImGui::Button("World", ImVec2(138, 30)))
                Slec = 2;

            ImGui::SameLine();
            if (ImGui::Button("Misc", ImVec2(138, 30)))
                Slec = 3;
            ImGui::SameLine();
            if (ImGui::Button("Settings", ImVec2(138, 30)))
                Slec = 4;

            if (Slec == 0)
            {
                ImGui::SetCursorPos(ImVec2(10, 100));
                ImGui::Checkbox("Box", &ESP::box);
                ImGui::SetCursorPos(ImVec2(10, 130));
                ImGui::Checkbox("Corner Box", &ESP::CornerBox);
                ImGui::SetCursorPos(ImVec2(10, 160));
                ImGui::Checkbox("Filled Box", &ESP::FilledBox);
                ImGui::SetCursorPos(ImVec2(10, 190));
                ImGui::Checkbox("Distance", &ESP::Distance);
                ImGui::SetCursorPos(ImVec2(10, 220));
                ImGui::Checkbox("Snap Lines", &ESP::SnapLines);
                ImGui::SetCursorPos(ImVec2(10, 250));
                ImGui::Checkbox("Skeleton", &ESP::Skeleton);
                ImGui::SetCursorPos(ImVec2(10, 280));
                ImGui::Checkbox("Name", &ESP::Name);
                ImGui::SetCursorPos(ImVec2(10, 310));
                ImGui::Checkbox("Held Item", &ESP::Weapon);
                ImGui::SetCursorPos(ImVec2(10, 340));
                ImGui::ColorEdit3("Box Color", ESP::BoxColor);
            }
            else if (Slec == 1)
            {
               
            }
            else if (Slec == 2)
            {

             
            }
            else if (Slec == 3)
            {

              


            }

            ImGui::End();
        }
        Cheat* Hack;
        bool  render_Menu = false;
        bool DoOnce = false;
		auto Draw() -> void {
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.f);

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			if (GetAsyncKeyState(VK_INSERT) & 1)
                render_Menu = !render_Menu;

            //if ( get_screen_status( ) ) 
				if (render_Menu)
				{
					Menu();
				}
                   
            bool ScreenSatus = get_screen_status();
            

            /* ImFont* Arial;
            ImFont* Calibri;
            ImFont* Ebrima;
            ImFont* Tahoma;
            ImFont* Comic;*/
            Hack->Do_Cheat();
            int FPS = FrameRate();
            std::string FPS_Text = "FPS : " + std::to_string(int(FPS)) + "";
            draw_text_outline_font_Font(SetFont, FontSize,10 + (FontSize), 10, ImColor(255, 255, 255, 255), "Prv");
            draw_text_outline_font_Font(SetFont, FontSize,35 + (FontSize), 10, ImColor(255, 0, 100, 255), "RustClient.exe");

            draw_text_outline_font_Font(SetFont, FontSize,10 + (FontSize), 25, ImColor(0, 155, 255, 255), FPS_Text.c_str());
			ImGui::Render();
			const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
			d3d_device_ctx->OMSetRenderTargets(1, &d3d_render_target, nullptr);
			d3d_device_ctx->ClearRenderTargetView(d3d_render_target, clear_color_with_alpha);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			d3d_swap_chain->Present(0, 0);

		}
	};
} static render::c_render* Renderer = new render::c_render();

