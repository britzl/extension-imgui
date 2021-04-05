// myextension.cpp
// Extension lib defines
#define LIB_NAME "ImGui"
#define MODULE_NAME "imgui"

#include <stdlib.h>

#include "imgui/imconfig.h"
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>
#endif

// include the Defold SDK
#include <dmsdk/sdk.h>


#include "imgui/imgui_impl_opengl3.h"




#define TEXTBUFFER_SIZE sizeof(char) * 1000 * 1024

static bool g_imgui_NewFrame = false;
static char* g_imgui_TextBuffer = 0;


static void imgui_NewFrame()
{
    if (g_imgui_NewFrame == false)
    {
        ImGui_ImplOpenGL3_NewFrame();
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


// ----------------------------
// ----- INPUT ----------------
// ----------------------------
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
static int imgui_SetKeyDown(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGuiIO& io = ImGui::GetIO();
    uint32_t key = luaL_checknumber(L, 1);
    io.KeysDown[key] = lua_toboolean(L, 2);
    return 0;
}


// ----------------------------
// ----- KEY MODIFIERS --------
// ----------------------------
static int imgui_SetKeyModifierCtrl(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGuiIO& io = ImGui::GetIO();
    io.KeyCtrl = lua_toboolean(L, 1);
    return 0;
}
static int imgui_SetKeyModifierShift(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGuiIO& io = ImGui::GetIO();
    io.KeyShift = lua_toboolean(L, 1);
    return 0;
}
static int imgui_SetKeyModifierAlt(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGuiIO& io = ImGui::GetIO();
    io.KeyAlt = lua_toboolean(L, 1);
    return 0;
}
static int imgui_SetKeyModifierSuper(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGuiIO& io = ImGui::GetIO();
    io.KeySuper = lua_toboolean(L, 1);
    return 0;
}


// ----------------------------
// ----- TEXT INPUT -----------
// ----------------------------
static int imgui_AddInputCharacter(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGuiIO& io = ImGui::GetIO();
    const char* s = luaL_checkstring(L, 1);
    unsigned int c = s[0];
    io.AddInputCharacter(c);
    return 0;
}


// ----------------------------
// ----- TREE -----------------
// ----------------------------
static int imgui_TreeNode(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    const char* text = luaL_checkstring(L, 1);
    uint32_t flags = 0;
    if (lua_isnumber(L, 2))
    {
        flags = luaL_checkint(L, 2);
    }
    bool result = ImGui::TreeNodeEx(text, flags);
    lua_pushboolean(L, result);
    return 1;
}
static int imgui_TreePop(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::TreePop();
    return 0;
}

// ----------------------------
// ----- WINDOW ---------------
// ----------------------------
static int imgui_Begin(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);
    imgui_NewFrame();
    const char* title = luaL_checkstring(L, 1);
    bool is_open = true;
    bool* is_open_ptr = NULL;
    if (lua_isboolean(L, 2) && lua_toboolean(L, 2))
    {
        is_open_ptr = &is_open;
    }
    uint32_t flags = 0;
    if (lua_isnumber(L, 3))
    {
        flags = luaL_checkint(L, 3);
    }
    bool result = ImGui::Begin(title, is_open_ptr, flags);
    lua_pushboolean(L, result);
    lua_pushboolean(L, is_open);
    return 2;
}
static int imgui_End(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::End();
    return 0;
}
static int imgui_SetNextWindowSize(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    float width = luaL_checknumber(L, 1);
    float height = luaL_checknumber(L, 2);
    ImGui::SetNextWindowSize(ImVec2(width, height));
    return 0;
}
static int imgui_SetNextWindowPos(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    ImGui::SetNextWindowPos(ImVec2(x, y));
    return 0;
}
static int imgui_IsWindowFocused(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    uint32_t flags = 0;
    if (lua_isnumber(L, 1))
    {
        flags = luaL_checkint(L, 1);
    }
    bool focused = ImGui::IsWindowFocused(flags);
    lua_pushboolean(L, focused);
    return 1;
}


// ----------------------------
// ----- CHILD WINDOW ---------
// ----------------------------
static int imgui_BeginChild(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();

    const char* title = luaL_checkstring(L, 1);
    float width = luaL_checknumber(L, 2);
    float height = luaL_checknumber(L, 3);

    bool result = ImGui::BeginChild(title, ImVec2(width, height));
    lua_pushboolean(L, result);

    return 1;
}
static int imgui_EndChild(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::EndChild();
    return 0;
}


// ----------------------------
// ----- COMBO ---------
// ----------------------------
static int imgui_BeginCombo(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    const char* label = luaL_checkstring(L, 1);
    const char* preview = luaL_checkstring(L, 2);
    bool result = ImGui::BeginCombo(label, preview);
    lua_pushboolean(L, result);
    return 1;
}
static int imgui_EndCombo(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::EndCombo();
    return 0;
}
static int imgui_Combo(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);
    imgui_NewFrame();
    const char* label = luaL_checkstring(L, 1);

    int current = luaL_checknumber(L, 2) - 1 ;
    if(!lua_istable(L, 3))
    {
        luaL_error(L, "You must provide a table");
    }
    const size_t len = lua_objlen(L, 3);
    const char* items[len];
    for(int i=0; i<len; i++)
    {
        lua_pushnumber(L, i + 1);
        int top = lua_gettop(L);
        lua_gettable(L, 3);
        const char* item = luaL_checkstring(L, 4);
        items[i] = item;
        lua_pop(L, 1);
    }

    bool result = ImGui::Combo(label, &current, items, len);
    lua_pushboolean(L, result);
    lua_pushnumber(L, current + 1);
    return 2;
}


// ----------------------------
// ----- TABLES ---------
// ----------------------------
static int imgui_BeginTable(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    const char* id = luaL_checkstring(L, 1);
    int column = luaL_checkinteger(L, 2);
    bool result = ImGui::BeginTable(id, column);
    lua_pushboolean(L, result);
    return 1;
}
static int imgui_EndTable(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::EndTable();
    return 0;
}
static int imgui_TableHeadersRow(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::TableHeadersRow();
    return 0;
}
static int imgui_TableSetupColumn(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    const char* label = luaL_checkstring(L, 1);
    uint32_t flags = 0;
    if (lua_isnumber(L, 2))
    {
        flags = luaL_checkint(L, 2);
    }
    ImGui::TableSetupColumn(label, flags);
    return 0;
}
static int imgui_TableSetColumnIndex(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    int column = luaL_checkinteger(L, 1);
    ImGui::TableSetColumnIndex(column);
    return 0;
}
static int imgui_TableNextColumn(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::TableNextColumn();
    return 0;
}
static int imgui_TableNextRow(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::TableNextRow();
    return 0;
}


// ----------------------------
// ----- TAB BAR ---------
// ----------------------------
static int imgui_BeginTabBar(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    const char* id = luaL_checkstring(L, 1);
    bool result = ImGui::BeginTabBar(id);
    lua_pushboolean(L, result);
    return 1;
}
static int imgui_EndTabBar(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::EndTabBar();
    return 0;
}
static int imgui_BeginTabItem(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);
    imgui_NewFrame();
    const char* label = luaL_checkstring(L, 1);

    bool* p_open = 0;
    bool open = 1;
    if (lua_isboolean(L, 2))
    {
        open = lua_toboolean(L, 2);
        p_open = &open;
    }

    uint32_t flags = 0;
    if (lua_isnumber(L, 3))
    {
        flags = luaL_checkint(L, 3);
    }
    bool result = ImGui::BeginTabItem(label, p_open, flags);
    lua_pushboolean(L, result);
    lua_pushboolean(L, open);
    return 2;
}
static int imgui_EndTabItem(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::EndTabItem();
    return 0;
}


// ----------------------------
// ----- WIDGETS ---------
// ----------------------------
static int imgui_Text(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    const char* text = luaL_checkstring(L, 1);
    ImGui::Text("%s", text);
    return 0;
}

static int imgui_InputText(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);
    imgui_NewFrame();
    const char* label = luaL_checkstring(L, 1);
    const char* text = luaL_checkstring(L, 2);
    dmStrlCpy(g_imgui_TextBuffer, text, TEXTBUFFER_SIZE);
    bool changed = ImGui::InputText(label, g_imgui_TextBuffer, TEXTBUFFER_SIZE);
    lua_pushboolean(L, changed);
    if (changed)
    {
        lua_pushstring(L, g_imgui_TextBuffer);
    }
    else
    {
        lua_pushnil(L);
    }
    return 2;
}

static int imgui_InputInt(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);
    imgui_NewFrame();
    const char* label = luaL_checkstring(L, 1);
    int value = luaL_checkinteger(L, 2);
    bool changed = ImGui::InputInt(label, &value);
    lua_pushboolean(L, changed);
    if (changed)
    {
        lua_pushinteger(L, value);
    }
    else
    {
        lua_pushnil(L);
    }
    return 2;
}

static int imgui_InputFloat3(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 4);
    imgui_NewFrame();
    const char* label = luaL_checkstring(L, 1);
    float v[3];
    v[0]  = luaL_checknumber(L, 2);
    v[1]  = luaL_checknumber(L, 3);
    v[2]  = luaL_checknumber(L, 4);

    bool changed = ImGui::InputFloat3(label, v);
    lua_pushboolean(L, changed);
    if (changed)
    {
        lua_pushnumber(L, v[0]);
        lua_pushnumber(L, v[1]);
        lua_pushnumber(L, v[2]);
    }
    else
    {
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
    }
    return 4;
}

static int imgui_InputFloat4(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 5);
    imgui_NewFrame();
    const char* label = luaL_checkstring(L, 1);
    float v[4];
    v[0]  = luaL_checknumber(L, 2);
    v[1]  = luaL_checknumber(L, 3);
    v[2]  = luaL_checknumber(L, 4);
    v[3]  = luaL_checknumber(L, 5);

    bool changed = ImGui::InputFloat4(label, v);
    lua_pushboolean(L, changed);
    if (changed)
    {
        lua_pushnumber(L, v[0]);
        lua_pushnumber(L, v[1]);
        lua_pushnumber(L, v[2]);
        lua_pushnumber(L, v[3]);
    }
    else
    {
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
    }
    return 5;
}

static int imgui_Selectable(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    const char* text = luaL_checkstring(L, 1);
    bool selected = lua_toboolean(L, 2);
    uint32_t flags = 0;
    if (lua_isnumber(L, 3))
    {
        flags = luaL_checkint(L, 3);
    }
    ImGui::Selectable(text, &selected, flags);
    lua_pushboolean(L, selected);
    return 1;
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
    DM_LUA_STACK_CHECK(L, 2);
    imgui_NewFrame();
    const char* text = luaL_checkstring(L, 1);
    bool checked = lua_toboolean(L, 2);
    bool changed = ImGui::Checkbox(text, &checked);
    lua_pushboolean(L, changed);
    lua_pushboolean(L, checked);
    return 2;
}


// ----------------------------
// ----- LAYOUT ---------
// ----------------------------
static int imgui_SameLine(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();

    float offset = 0.0f;
    if (lua_isnumber(L, 1))
    {
        offset = lua_tonumber(L, 1);
    }
    ImGui::SameLine(offset);
    return 0;
}
static int imgui_NewLine(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::NewLine();
    return 0;
}

static int imgui_Bullet(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::Bullet();
    return 0;
}

static int imgui_Indent(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::Indent();
    return 0;
}

static int imgui_Unindent(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::Unindent();
    return 0;
}

static int imgui_Spacing(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::Spacing();
    return 0;
}

static int imgui_Separator(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::Separator();
    return 0;
}


// ----------------------------
// ----- IMGUI DEMO -----------
// ----------------------------
static int imgui_Demo(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);
    return 0;
}



// ----------------------------
// ----- INPUT ----------------
// ----------------------------
static int imgui_IsMouseDoubleClicked(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    uint32_t button = luaL_checknumber(L, 1);
    ImGui::IsMouseDoubleClicked(button);
    return 0;
}

static int imgui_IsMouseClicked(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    uint32_t button = luaL_checknumber(L, 1);
    bool clicked = ImGui::IsMouseClicked(button);
    lua_pushboolean(L, clicked);
    return 1;
}

static int imgui_IsItemClicked(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    uint32_t button = luaL_checknumber(L, 1);
    bool clicked = ImGui::IsItemClicked(button);
    lua_pushboolean(L, clicked);
    return 1;
}

static int imgui_IsItemHovered(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    bool hovered = ImGui::IsItemHovered();
    lua_pushboolean(L, hovered);
    return 1;
}

// ----------------------------
// ----- STYLE ----------------
// ----------------------------

static int imgui_SetStyleWindowRounding(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = luaL_checknumber(L, 1);
    return 0;
}
static int imgui_SetStyleFrameRounding(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = luaL_checknumber(L, 1);
    return 0;
}
static int imgui_SetStyleScrollbarRounding(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScrollbarRounding = luaL_checknumber(L, 1);
    return 0;
}
static int imgui_SetStyleColor(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGuiStyle& style = ImGui::GetStyle();
    double r = luaL_checknumber(L, 2);
    double g = luaL_checknumber(L, 3);
    double b = luaL_checknumber(L, 4);
    double a = luaL_checknumber(L, 5);
    style.Colors[luaL_checkinteger(L, 1)] = ImVec4(r, g, b, a);
    return 0;
}


// ----------------------------
// ----- CONFIG -----------------
// ----------------------------
static int imgui_SetIniFilename(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    const char *filename = 0;
    if (lua_isstring(L, 1)) {
        filename = luaL_checkstring(L, 1);
    }

    ImGui::GetIO().IniFilename = filename;

    return 0;
}

// ----------------------------
// ----- DRAW -----------------
// ----------------------------
static dmExtension::Result imgui_Draw(dmExtension::Params* params)
{
    imgui_NewFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    g_imgui_NewFrame = false;
    return dmExtension::RESULT_OK;
}


static void imgui_Init(float width, float height)
{
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    int r = gl3wInit();
    if (r != GL3W_OK) {
        dmLogError("Failed to initialize OpenGL: %d", r);
    }
#endif

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(width, height);

    // init keymap list
    // We will be sending the correct ImGuiKey_ enums from Lua
    for (int i = 0; i < ImGuiKey_COUNT; i++)
    {
        io.KeyMap[i] = i;
    }

    ImGui_ImplOpenGL3_Init();
}


static void imgui_Shutdown()
{
    dmLogInfo("imgui_Shutdown");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

static void imgui_ExtensionInit()
{
    dmExtension::RegisterCallback(dmExtension::CALLBACK_POST_RENDER, imgui_Draw );
    if (g_imgui_TextBuffer)
    {
        free(g_imgui_TextBuffer);
    }
    g_imgui_TextBuffer = (char*)malloc(TEXTBUFFER_SIZE);
}

static void imgui_ExtensionShutdown()
{
    if (g_imgui_TextBuffer)
    {
        free(g_imgui_TextBuffer);
        g_imgui_TextBuffer = 0;
    }
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"set_next_window_size", imgui_SetNextWindowSize},
    {"set_next_window_pos", imgui_SetNextWindowPos},
    {"begin_window", imgui_Begin},
    {"end_window", imgui_End},
    {"is_window_focused", imgui_IsWindowFocused},

    {"begin_child", imgui_BeginChild},
    {"end_child", imgui_EndChild},

    {"begin_tab_bar", imgui_BeginTabBar},
    {"end_tab_bar", imgui_EndTabBar},

    {"begin_tab_item", imgui_BeginTabItem},
    {"end_tab_item", imgui_EndTabItem},

    {"begin_combo", imgui_BeginCombo},
    {"end_combo", imgui_EndCombo},
    {"combo", imgui_Combo},

    {"begin_table", imgui_BeginTable},
    {"end_table", imgui_EndTable},
    {"table_next_row", imgui_TableNextRow},
    {"table_next_column", imgui_TableNextColumn},
    {"table_set_column_index", imgui_TableSetColumnIndex},
    {"table_setup_column", imgui_TableSetupColumn},
    {"table_headers_row", imgui_TableHeadersRow},

    {"tree_node", imgui_TreeNode},
    {"tree_pop", imgui_TreePop},

    {"selectable", imgui_Selectable},
    {"text", imgui_Text},
    {"input_text", imgui_InputText},
    {"input_int", imgui_InputInt},
    {"input_float3", imgui_InputFloat3},
    {"input_float4", imgui_InputFloat4},
    {"button", imgui_Button},
    {"checkbox", imgui_Checkbox},
    {"same_line", imgui_SameLine},
    {"new_line", imgui_NewLine},
    {"bullet", imgui_Bullet},
    {"indent", imgui_Indent},
    {"unindent", imgui_Unindent},
    {"spacing", imgui_Spacing},
    {"separator", imgui_Separator},

    {"demo", imgui_Demo},

    {"set_mouse_input", imgui_SetMouseInput},
    {"set_key_down", imgui_SetKeyDown},
    {"set_key_modifier_ctrl", imgui_SetKeyModifierCtrl},
    {"set_key_modifier_shift", imgui_SetKeyModifierShift},
    {"set_key_modifier_alt", imgui_SetKeyModifierAlt},
    {"set_key_modifier_super", imgui_SetKeyModifierSuper},
    {"add_input_character", imgui_AddInputCharacter},
    {"set_display_size", imgui_SetDisplaySize},

    {"is_item_clicked", imgui_IsItemClicked},
    {"is_item_hovered", imgui_IsItemHovered},
    {"is_mouse_clicked", imgui_IsMouseClicked},
    {"is_mouse_double_clicked", imgui_IsMouseDoubleClicked},

    {"set_style_window_rounding", imgui_SetStyleWindowRounding},
    {"set_style_frame_rounding", imgui_SetStyleFrameRounding},
    {"set_style_scrollbar_rounding", imgui_SetStyleScrollbarRounding},
    {"set_style_color", imgui_SetStyleColor},

    {"set_ini_filename", imgui_SetIniFilename},

    {0, 0}
};

static void lua_setfieldstringstring(lua_State* L, const char* key, const char* value)
{
    int top = lua_gettop(L);
    lua_pushstring(L, value);
    lua_setfield(L, -2, key);
    assert(top == lua_gettop(L));
}
static void lua_setfieldstringint(lua_State* L, const char* key, uint32_t value)
{
    int top = lua_gettop(L);
    lua_pushnumber(L, value);
    lua_setfield(L, -2, key);
    assert(top == lua_gettop(L));
}

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, Module_methods);

    lua_setfieldstringint(L, "MOUSEBUTTON_LEFT", ImGuiMouseButton_Left);
    lua_setfieldstringint(L, "MOUSEBUTTON_RIGHT", ImGuiMouseButton_Right);
    lua_setfieldstringint(L, "MOUSEBUTTON_MIDDLE", ImGuiMouseButton_Middle);

    lua_setfieldstringint(L, "SELECTABLE_DONT_CLOSE_POPUPS", ImGuiSelectableFlags_DontClosePopups);
    lua_setfieldstringint(L, "SELECTABLE_SPAN_ALL_COLUMNS", ImGuiSelectableFlags_SpanAllColumns);
    lua_setfieldstringint(L, "SELECTABLE_ALLOW_DOUBLE_CLICK", ImGuiSelectableFlags_AllowDoubleClick);
    lua_setfieldstringint(L, "SELECTABLE_DISABLED", ImGuiSelectableFlags_Disabled);
    lua_setfieldstringint(L, "SELECTABLE_ALLOW_ITEM_OVERLAP", ImGuiSelectableFlags_AllowItemOverlap);

    lua_setfieldstringint(L, "TABITEM_UNSAVED_DOCUMENT", ImGuiTabItemFlags_UnsavedDocument);
    lua_setfieldstringint(L, "TABITEM_SET_SELECTED", ImGuiTabItemFlags_SetSelected);
    lua_setfieldstringint(L, "TABITEM_NO_CLOSE_WITH_MIDDLE_MOUSE_BUTTON", ImGuiTabItemFlags_NoCloseWithMiddleMouseButton);
    lua_setfieldstringint(L, "TABITEM_NO_PUSH_ID", ImGuiTabItemFlags_NoPushId);
    lua_setfieldstringint(L, "TABITEM_NO_TOOLTIP", ImGuiTabItemFlags_NoTooltip);
    lua_setfieldstringint(L, "TABITEM_NO_REORDER", ImGuiTabItemFlags_NoReorder);
    lua_setfieldstringint(L, "TABITEM_LEADING", ImGuiTabItemFlags_Leading);
    lua_setfieldstringint(L, "TABITEM_TRAILING", ImGuiTabItemFlags_Trailing);

    lua_setfieldstringint(L, "FOCUSED_CHILD_WINDOWS", ImGuiFocusedFlags_ChildWindows);
    lua_setfieldstringint(L, "FOCUSED_ROOT_WINDOW", ImGuiFocusedFlags_RootWindow);
    lua_setfieldstringint(L, "FOCUSED_ANY_WINDOW", ImGuiFocusedFlags_AnyWindow);
    lua_setfieldstringint(L, "FOCUSED_ROOT_AND_CHILD_WINDOWS", ImGuiFocusedFlags_RootAndChildWindows);

    lua_setfieldstringint(L, "TREENODE_SELECTED", ImGuiTreeNodeFlags_Selected);
    lua_setfieldstringint(L, "TREENODE_FRAMED", ImGuiTreeNodeFlags_Framed);
    lua_setfieldstringint(L, "TREENODE_ALLOW_ITEM_OVERLAP", ImGuiTreeNodeFlags_AllowItemOverlap);
    lua_setfieldstringint(L, "TREENODE_NO_TREE_PUSH_ON_OPEN", ImGuiTreeNodeFlags_NoTreePushOnOpen);
    lua_setfieldstringint(L, "TREENODE_NO_AUTO_OPEN_ON_LOG", ImGuiTreeNodeFlags_NoAutoOpenOnLog);
    lua_setfieldstringint(L, "TREENODE_DEFAULT_OPEN", ImGuiTreeNodeFlags_DefaultOpen);
    lua_setfieldstringint(L, "TREENODE_OPEN_ON_DOUBLE_CLICK", ImGuiTreeNodeFlags_OpenOnDoubleClick);
    lua_setfieldstringint(L, "TREENODE_OPEN_ON_ARROW", ImGuiTreeNodeFlags_OpenOnArrow);
    lua_setfieldstringint(L, "TREENODE_LEAF", ImGuiTreeNodeFlags_Leaf);
    lua_setfieldstringint(L, "TREENODE_BULLET", ImGuiTreeNodeFlags_Bullet);
    lua_setfieldstringint(L, "TREENODE_FRAME_PADDING", ImGuiTreeNodeFlags_FramePadding);
    lua_setfieldstringint(L, "TREENODE_SPAN_AVAILABLE_WIDTH", ImGuiTreeNodeFlags_SpanAvailWidth);
    lua_setfieldstringint(L, "TREENODE_SPAN_FULL_WIDTH", ImGuiTreeNodeFlags_SpanFullWidth);
    lua_setfieldstringint(L, "TREENODE_NAV_LEFT_JUMPS_BACK_HERE", ImGuiTreeNodeFlags_NavLeftJumpsBackHere);

    lua_setfieldstringint(L, "ImGuiCol_Text", ImGuiCol_Text);
    lua_setfieldstringint(L, "ImGuiCol_TextDisabled", ImGuiCol_TextDisabled);
    lua_setfieldstringint(L, "ImGuiCol_WindowBg", ImGuiCol_WindowBg);
    lua_setfieldstringint(L, "ImGuiCol_ChildBg", ImGuiCol_ChildBg);
    lua_setfieldstringint(L, "ImGuiCol_PopupBg", ImGuiCol_PopupBg);
    lua_setfieldstringint(L, "ImGuiCol_Border", ImGuiCol_Border);
    lua_setfieldstringint(L, "ImGuiCol_BorderShadow", ImGuiCol_BorderShadow);
    lua_setfieldstringint(L, "ImGuiCol_FrameBg", ImGuiCol_FrameBg);
    lua_setfieldstringint(L, "ImGuiCol_FrameBgHovered", ImGuiCol_FrameBgHovered);
    lua_setfieldstringint(L, "ImGuiCol_FrameBgActive", ImGuiCol_FrameBgActive);
    lua_setfieldstringint(L, "ImGuiCol_TitleBg", ImGuiCol_TitleBg);
    lua_setfieldstringint(L, "ImGuiCol_TitleBgActive", ImGuiCol_TitleBgActive);
    lua_setfieldstringint(L, "ImGuiCol_TitleBgCollapsed", ImGuiCol_TitleBgCollapsed);
    lua_setfieldstringint(L, "ImGuiCol_MenuBarBg", ImGuiCol_MenuBarBg);
    lua_setfieldstringint(L, "ImGuiCol_ScrollbarBg", ImGuiCol_ScrollbarBg);
    lua_setfieldstringint(L, "ImGuiCol_ScrollbarGrab", ImGuiCol_ScrollbarGrab);
    lua_setfieldstringint(L, "ImGuiCol_ScrollbarGrabHovered", ImGuiCol_ScrollbarGrabHovered);
    lua_setfieldstringint(L, "ImGuiCol_ScrollbarGrabActive", ImGuiCol_ScrollbarGrabActive);
    lua_setfieldstringint(L, "ImGuiCol_CheckMark", ImGuiCol_CheckMark);
    lua_setfieldstringint(L, "ImGuiCol_SliderGrab", ImGuiCol_SliderGrab);
    lua_setfieldstringint(L, "ImGuiCol_SliderGrabActive", ImGuiCol_SliderGrabActive);
    lua_setfieldstringint(L, "ImGuiCol_Button", ImGuiCol_Button);
    lua_setfieldstringint(L, "ImGuiCol_ButtonHovered", ImGuiCol_ButtonHovered);
    lua_setfieldstringint(L, "ImGuiCol_ButtonActive", ImGuiCol_ButtonActive);
    lua_setfieldstringint(L, "ImGuiCol_Header", ImGuiCol_Header);
    lua_setfieldstringint(L, "ImGuiCol_HeaderHovered", ImGuiCol_HeaderHovered);
    lua_setfieldstringint(L, "ImGuiCol_HeaderActive", ImGuiCol_HeaderActive);
    lua_setfieldstringint(L, "ImGuiCol_Separator", ImGuiCol_Separator);
    lua_setfieldstringint(L, "ImGuiCol_SeparatorHovered", ImGuiCol_SeparatorHovered);
    lua_setfieldstringint(L, "ImGuiCol_SeparatorActive", ImGuiCol_SeparatorActive);
    lua_setfieldstringint(L, "ImGuiCol_ResizeGrip", ImGuiCol_ResizeGrip);
    lua_setfieldstringint(L, "ImGuiCol_ResizeGripHovered", ImGuiCol_ResizeGripHovered);
    lua_setfieldstringint(L, "ImGuiCol_ResizeGripActive", ImGuiCol_ResizeGripActive);
    lua_setfieldstringint(L, "ImGuiCol_Tab", ImGuiCol_Tab);
    lua_setfieldstringint(L, "ImGuiCol_TabHovered", ImGuiCol_TabHovered);
    lua_setfieldstringint(L, "ImGuiCol_TabActive", ImGuiCol_TabActive);
    lua_setfieldstringint(L, "ImGuiCol_TabUnfocused", ImGuiCol_TabUnfocused);
    lua_setfieldstringint(L, "ImGuiCol_TabUnfocusedActive", ImGuiCol_TabUnfocusedActive);
    lua_setfieldstringint(L, "ImGuiCol_PlotLines", ImGuiCol_PlotLines);
    lua_setfieldstringint(L, "ImGuiCol_PlotLinesHovered", ImGuiCol_PlotLinesHovered);
    lua_setfieldstringint(L, "ImGuiCol_PlotHistogram", ImGuiCol_PlotHistogram);
    lua_setfieldstringint(L, "ImGuiCol_PlotHistogramHovered", ImGuiCol_PlotHistogramHovered);
    lua_setfieldstringint(L, "ImGuiCol_TableHeaderBg", ImGuiCol_TableHeaderBg);
    lua_setfieldstringint(L, "ImGuiCol_TableBorderStrong", ImGuiCol_TableBorderStrong);
    lua_setfieldstringint(L, "ImGuiCol_TableBorderLight", ImGuiCol_TableBorderLight);
    lua_setfieldstringint(L, "ImGuiCol_TableRowBg", ImGuiCol_TableRowBg);
    lua_setfieldstringint(L, "ImGuiCol_TableRowBgAlt", ImGuiCol_TableRowBgAlt);
    lua_setfieldstringint(L, "ImGuiCol_TextSelectedBg", ImGuiCol_TextSelectedBg);
    lua_setfieldstringint(L, "ImGuiCol_DragDropTarget", ImGuiCol_DragDropTarget);
    lua_setfieldstringint(L, "ImGuiCol_NavHighlight", ImGuiCol_NavHighlight);
    lua_setfieldstringint(L, "ImGuiCol_NavWindowingHighlight", ImGuiCol_NavWindowingHighlight);
    lua_setfieldstringint(L, "ImGuiCol_NavWindowingDimBg", ImGuiCol_NavWindowingDimBg);
    lua_setfieldstringint(L, "ImGuiCol_ModalWindowDimBg", ImGuiCol_ModalWindowDimBg);

    lua_setfieldstringint(L, "TABLECOLUMN_NONE", ImGuiTableColumnFlags_None);
    lua_setfieldstringint(L, "TABLECOLUMN_DEFAULTHIDE", ImGuiTableColumnFlags_DefaultHide);   // Default as a hidden/disabled column.
    lua_setfieldstringint(L, "TABLECOLUMN_DEFAULTSORT", ImGuiTableColumnFlags_DefaultSort);   // Default as a sorting column.
    lua_setfieldstringint(L, "TABLECOLUMN_WIDTHSTRETCH", ImGuiTableColumnFlags_WidthStretch);   // Column will stretch. Preferable with horizontal scrolling disabled (default if table sizing policy is _SizingStretchSame or _SizingStretchProp).
    lua_setfieldstringint(L, "TABLECOLUMN_WIDTHFIXED", ImGuiTableColumnFlags_WidthFixed);   // Column will not stretch. Preferable with horizontal scrolling enabled (default if table sizing policy is _SizingFixedFit and table is resizable).
    lua_setfieldstringint(L, "TABLECOLUMN_NORESIZE", ImGuiTableColumnFlags_NoResize);   // Disable manual resizing.
    lua_setfieldstringint(L, "TABLECOLUMN_NOREORDER", ImGuiTableColumnFlags_NoReorder);   // Disable manual reordering this column, this will also prevent other columns from crossing over this column.
    lua_setfieldstringint(L, "TABLECOLUMN_NOHIDE", ImGuiTableColumnFlags_NoHide);   // Disable ability to hide/disable this column.
    lua_setfieldstringint(L, "TABLECOLUMN_NOCLIP", ImGuiTableColumnFlags_NoClip);   // Disable clipping for this column (all NoClip columns will render in a same draw command).
    lua_setfieldstringint(L, "TABLECOLUMN_NOSORT", ImGuiTableColumnFlags_NoSort);   // Disable ability to sort on this field (even if ImGuiTableFlags_Sortable is set on the table).
    lua_setfieldstringint(L, "TABLECOLUMN_NOSORTASCENDING", ImGuiTableColumnFlags_NoSortAscending);   // Disable ability to sort in the ascending direction.
    lua_setfieldstringint(L, "TABLECOLUMN_NOSORTDESCENDING", ImGuiTableColumnFlags_NoSortDescending);  // Disable ability to sort in the descending direction.
    lua_setfieldstringint(L, "TABLECOLUMN_NOHEADERWIDTH", ImGuiTableColumnFlags_NoHeaderWidth);  // Disable header text width contribution to automatic column width.
    lua_setfieldstringint(L, "TABLECOLUMN_PREFERSORTASCENDING", ImGuiTableColumnFlags_PreferSortAscending);  // Make the initial sort direction Ascending when first sorting on this column (default).
    lua_setfieldstringint(L, "TABLECOLUMN_PREFERSORTDESCENDING", ImGuiTableColumnFlags_PreferSortDescending);  // Make the initial sort direction Descending when first sorting on this column.
    lua_setfieldstringint(L, "TABLECOLUMN_INDENTENABLE", ImGuiTableColumnFlags_IndentEnable);  // Use current Indent value when entering cell (default for column 0).
    lua_setfieldstringint(L, "TABLECOLUMN_INDENTDISABLE", ImGuiTableColumnFlags_IndentDisable);  // Ignore current Indent value when entering cell (default for columns > 0). Indentation changes _within_ the cell will still be honored.

    lua_setfieldstringint(L, "WINDOWFLAGS_NONE", ImGuiWindowFlags_None);
    lua_setfieldstringint(L, "WINDOWFLAGS_NOTITLEBAR", ImGuiWindowFlags_NoTitleBar); // Disable title-bar
    lua_setfieldstringint(L, "WINDOWFLAGS_NORESIZE", ImGuiWindowFlags_NoResize); // Disable user resizing with the lower-right grip
    lua_setfieldstringint(L, "WINDOWFLAGS_NOMOVE", ImGuiWindowFlags_NoMove); // Disable user moving the window
    lua_setfieldstringint(L, "WINDOWFLAGS_NOSCROLLBAR", ImGuiWindowFlags_NoScrollbar); // Disable scrollbars (window can still scroll with mouse or programmatically)
    lua_setfieldstringint(L, "WINDOWFLAGS_NOSCROLLWITHMOUSE", ImGuiWindowFlags_NoScrollWithMouse); // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
    lua_setfieldstringint(L, "WINDOWFLAGS_NOCOLLAPSE", ImGuiWindowFlags_NoCollapse); // Disable user collapsing window by double-clicking on it
    lua_setfieldstringint(L, "WINDOWFLAGS_ALWAYSAUTORESIZE", ImGuiWindowFlags_AlwaysAutoResize); // Resize every window to its content every frame
    lua_setfieldstringint(L, "WINDOWFLAGS_NOBACKGROUND", ImGuiWindowFlags_NoBackground); // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
    lua_setfieldstringint(L, "WINDOWFLAGS_NOSAVEDSETTINGS", ImGuiWindowFlags_NoSavedSettings); // Never load/save settings in .ini file
    lua_setfieldstringint(L, "WINDOWFLAGS_NOMOUSEINPUTS", ImGuiWindowFlags_NoMouseInputs); // Disable catching mouse, hovering test with pass through.
    lua_setfieldstringint(L, "WINDOWFLAGS_MENUBAR", ImGuiWindowFlags_MenuBar); // Has a menu-bar
    lua_setfieldstringint(L, "WINDOWFLAGS_HORIZONTALSCROLLBAR", ImGuiWindowFlags_HorizontalScrollbar); // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
    lua_setfieldstringint(L, "WINDOWFLAGS_NOFOCUSONAPPEARING", ImGuiWindowFlags_NoFocusOnAppearing); // Disable taking focus when transitioning from hidden to visible state
    lua_setfieldstringint(L, "WINDOWFLAGS_NOBRINGTOFRONTONFOCUS", ImGuiWindowFlags_NoBringToFrontOnFocus); // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
    lua_setfieldstringint(L, "WINDOWFLAGS_ALWAYSVERTICALSCROLLBAR", ImGuiWindowFlags_AlwaysVerticalScrollbar); // Always show vertical scrollbar (even if ContentSize.y < Size.y)
    lua_setfieldstringint(L, "WINDOWFLAGS_ALWAYSHORIZONTALSCROLLBAR", ImGuiWindowFlags_AlwaysHorizontalScrollbar); // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
    lua_setfieldstringint(L, "WINDOWFLAGS_ALWAYSUSEWINDOWPADDING", ImGuiWindowFlags_AlwaysUseWindowPadding); // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
    lua_setfieldstringint(L, "WINDOWFLAGS_NONAVINPUTS", ImGuiWindowFlags_NoNavInputs); // No gamepad/keyboard navigation within the window
    lua_setfieldstringint(L, "WINDOWFLAGS_NONAVFOCUS", ImGuiWindowFlags_NoNavFocus); // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
    lua_setfieldstringint(L, "WINDOWFLAGS_UNSAVEDDOCUMENT", ImGuiWindowFlags_UnsavedDocument); // Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator. When used in a tab/docking context, tab is selected on closure and closure is deferred by one frame to allow code to cancel the closure (with a confirmation popup, etc.) without flicker.
    lua_setfieldstringint(L, "WINDOWFLAGS_NONAV", ImGuiWindowFlags_NoNav); // ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
    lua_setfieldstringint(L, "WINDOWFLAGS_NODECORATION", ImGuiWindowFlags_NoDecoration); // ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse,
    lua_setfieldstringint(L, "WINDOWFLAGS_NOINPUTS", ImGuiWindowFlags_NoInputs); // ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeDefoldImGui(dmExtension::AppParams* params)
{
    imgui_ExtensionInit();
    return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeDefoldImGui(dmExtension::Params* params)
{
    LuaInit(params->m_L);
    float displayWidth = dmConfigFile::GetFloat(params->m_ConfigFile, "display.width", 960.0f);
    float displayHeight = dmConfigFile::GetFloat(params->m_ConfigFile, "display.height", 540.0f);
    imgui_Init(displayWidth, displayHeight);

    return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeDefoldImGui(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeDefoldImGui(dmExtension::Params* params)
{
    dmLogInfo("FinalizeDefoldImGui");
    imgui_Shutdown();
    imgui_ExtensionShutdown();
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
            //dmLogInfo("OnEventDefoldImGui - EVENT_ID_ACTIVATEAPP\n");
            break;
        case dmExtension::EVENT_ID_DEACTIVATEAPP:
            //dmLogInfo("OnEventDefoldImGui - EVENT_ID_DEACTIVATEAPP\n");
            break;
        case dmExtension::EVENT_ID_ICONIFYAPP:
            //dmLogInfo("OnEventDefoldImGui - EVENT_ID_ICONIFYAPP\n");
            break;
        case dmExtension::EVENT_ID_DEICONIFYAPP:
            //dmLogInfo("OnEventDefoldImGui - EVENT_ID_DEICONIFYAPP\n");
            break;
        default:
            //dmLogWarning("OnEventDefoldImGui - Unknown event id\n");
            break;
    }
}

DM_DECLARE_EXTENSION(DefoldImGui, LIB_NAME, AppInitializeDefoldImGui, AppFinalizeDefoldImGui, InitializeDefoldImGui, OnUpdateDefoldImGui, OnEventDefoldImGui, FinalizeDefoldImGui)
