// myextension.cpp
// Extension lib defines
#define LIB_NAME "ImGui"
#define MODULE_NAME "imgui"

// include the Defold SDK
#include <dmsdk/sdk.h>


#include "imgui/imgui.h"
// #include "imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"
// #ifdef __APPLE__
//     #include <GLUT/glut.h>
// #else
//     #include <GL/freeglut.h>
// #endif

// #if defined(_WIN32)
// 	#include <gl/GL.h>
// #else
// 	#include <GLES2/gl2.h>
// 	#include <GLES2/gl2ext.h>
// #endif

// #if defined(__APPLE__)
// #define GL_SILENCE_DEPRECATION
// #include <OpenGL/gl.h>
// #else
// #include <gl/GL.h>
// #endif

static bool g_imgui_NewFrame = false;


static void imgui_NewFrame()
{
    if (g_imgui_NewFrame == false)
    {
        ImGui_ImplOpenGL2_NewFrame();
        ImGui::NewFrame();
        g_imgui_NewFrame = true;
    }
}

static int imgui_SetDisplaySize(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    float w = luaL_checknumber(L, 1);
    float h = luaL_checknumber(L, 2);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(w, h);
    return 0;
}

static int imgui_SetMouseInput(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    ImGuiIO& io = ImGui::GetIO();

    const ImVec2 mouse_pos_backup = io.MousePos;
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

    if (io.WantSetMousePos)
    {
        return luaL_error(L, "WantSetMousePos not supported yet.");
    }
    else
    {
        io.MousePos = ImVec2(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
    }

    io.MouseDown[0] = luaL_checknumber(L, 3);
    io.MouseDown[1] = luaL_checknumber(L, 4);
    io.MouseDown[2] = luaL_checknumber(L, 5);
    io.MouseWheel += luaL_checknumber(L, 6);

    return 0;
}

static int imgui_Begin(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();

    const char* title = luaL_checkstring(L, 1);
    ImGui::Begin(title);
    return 0;
}

static int imgui_End(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();

    ImGui::End();
    return 0;
}

static int imgui_Text(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();

    const char* text = luaL_checkstring(L, 1);
    ImGui::Text("%s", text);
    return 0;
}

static int imgui_Button(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();

    const char* text = luaL_checkstring(L, 1);
    bool pushed = ImGui::Button(text);
    lua_pushboolean(L, pushed);

    return 1;
}

static int imgui_Checkbox(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();

    const char* text = luaL_checkstring(L, 1);
    bool checked = lua_toboolean(L, 2);
    ImGui::Checkbox(text, &checked);
    lua_pushboolean(L, checked);

    return 1;
}

static int imgui_SameLine(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();

    ImGui::SameLine();
    return 0;
}


static int imgui_Demo(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();

    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);
    return 0;
}


static dmExtension::Result imgui_Draw(dmExtension::Params* params)
{
    imgui_NewFrame();
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    g_imgui_NewFrame = false;
    return dmExtension::RESULT_OK;
}


static void imgui_Init(float width, float height)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(width, height);
    // io.DisplayFramebufferScale = ImVec2(w, h);
    ImGui_ImplOpenGL2_Init();
}


static void imgui_Shutdown()
{
    dmLogInfo("imgui_Shutdown");
    ImGui_ImplOpenGL2_Shutdown();
    ImGui::DestroyContext();
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"window_begin", imgui_Begin},
    {"window_end", imgui_End},
    {"text", imgui_Text},
    {"button", imgui_Button},
    {"checkbox", imgui_Checkbox},
    {"same_line", imgui_SameLine},
    {"demo", imgui_Demo},
    {"set_mouse_input", imgui_SetMouseInput},

    {"set_display_size", imgui_SetDisplaySize},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, Module_methods);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeDefoldImGui(dmExtension::AppParams* params)
{
    float displayWidth = dmConfigFile::GetFloat(params->m_ConfigFile, "display.width", 960.0f);
    float displayHeight = dmConfigFile::GetFloat(params->m_ConfigFile, "display.height", 540.0f);
    imgui_Init(displayWidth, displayHeight);
    dmExtension::RegisterCallback(dmExtension::CALLBACK_POST_RENDER, imgui_Draw );
    return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeDefoldImGui(dmExtension::Params* params)
{
    LuaInit(params->m_L);
    return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeDefoldImGui(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeDefoldImGui(dmExtension::Params* params)
{    dmLogInfo("FinalizeDefoldImGui\n");
    imgui_Shutdown();
    return dmExtension::RESULT_OK;
}

dmExtension::Result OnUpdateDefoldImGui(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

void OnEventDefoldImGui(dmExtension::Params* params, const dmExtension::Event* event)
{
    switch(event->m_Event)
    {
        case dmExtension::EVENT_ID_ACTIVATEAPP:
            dmLogInfo("OnEventDefoldImGui - EVENT_ID_ACTIVATEAPP\n");
            break;
        case dmExtension::EVENT_ID_DEACTIVATEAPP:
            dmLogInfo("OnEventDefoldImGui - EVENT_ID_DEACTIVATEAPP\n");
            break;
        case dmExtension::EVENT_ID_ICONIFYAPP:
            dmLogInfo("OnEventDefoldImGui - EVENT_ID_ICONIFYAPP\n");
            break;
        case dmExtension::EVENT_ID_DEICONIFYAPP:
            dmLogInfo("OnEventDefoldImGui - EVENT_ID_DEICONIFYAPP\n");
            break;
        default:
            dmLogWarning("OnEventDefoldImGui - Unknown event id\n");
            break;
    }
}

DM_DECLARE_EXTENSION(DefoldImGui, LIB_NAME, AppInitializeDefoldImGui, AppFinalizeDefoldImGui, InitializeDefoldImGui, OnUpdateDefoldImGui, OnEventDefoldImGui, FinalizeDefoldImGui)
