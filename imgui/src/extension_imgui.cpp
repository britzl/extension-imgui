#define LIB_NAME "ImGui"
#define MODULE_NAME "imgui"

#include "imgui/imgui.h"
#include "imgui/imconfig.h"

// set in imconfig.h
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>
#endif

// imgui renderer backend and possible platform extras
#if defined(DM_PLATFORM_ANDROID)
#include "imgui/imgui_impl_android.h"
#endif
#include "imgui/imgui_impl_opengl3.h"

#include <dmsdk/sdk.h>
#include <dmsdk/dlib/crypt.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb/stb_image.h"

#define MAX_HISTOGRAM_VALUES    1000 * 1024
#define MAX_IMAGE_NAME          256

#define TEXTBUFFER_SIZE         sizeof(char) * 1000 * 1024

typedef struct ImgObject
{
    int                w;
    int                h;
    int                comp;
    GLuint             tid;
    char               name[MAX_IMAGE_NAME];
    unsigned char *    data;
} ImgObject;


static bool g_imgui_NewFrame        = false;
static char* g_imgui_TextBuffer     = 0;
static dmArray<ImFont*> g_imgui_Fonts;
static dmArray<ImgObject> g_imgui_Images;
static bool g_VerifyGraphicsCalls   = false;



static void imgui_ClearGLError()
{
    if (!g_VerifyGraphicsCalls) return;
    GLint err = glGetError();
    while (err != 0)
    {
        err = glGetError();
    }
}


// ----------------------------
// ----- IMAGES ---------------
// ----------------------------

// extern unsigned char *stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp);
//

static int imgui_ImageB64Decode(lua_State *L)
{
    DM_LUA_STACK_CHECK(L, 1);
    const char *data = luaL_checkstring(L,1);
    uint32_t datalen = luaL_checknumber(L,2);
    uint32_t dstlen  = 4*(datalen/3 + 1);

    char *datastr = (char *)malloc(dstlen);
    bool result;
    result = dmCrypt::Base64Decode((const uint8_t*)data, datalen, (uint8_t*)datastr, &dstlen);

    lua_pushlstring(L, datastr, dstlen);
    free(datastr);
    return 1;
}


static int imgui_ImageInternalLoad(const char *filename, ImgObject *image)
{
    if (image->data == 0)
    {
        dmLogError("Error loading image: %s", filename);
        return 0;
    }

    dmLogInfo("imgui_ImageInternalLoad before %d", image->tid);

    glGenTextures(1, &image->tid);
    dmLogInfo("imgui_ImageInternalLoad after %d", image->tid);
    glBindTexture(GL_TEXTURE_2D, image->tid);

    strcpy(image->name, filename);
    if (g_imgui_Images.Full())
    {
        g_imgui_Images.OffsetCapacity(2);
    }
    g_imgui_Images.Push(*image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);

    return 1;
}


// Image handling needs to be smarter, but this will do for the time being.
static int imgui_ImageLoadData(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 3);
    const char * filename = luaL_checkstring(L, 1);
    ImgObject image;

    // If its already in the vector, return the id
    for (int i=0; i<g_imgui_Images.Size(); i++)
    {
        if (strcmp(g_imgui_Images[i].name, filename) == 0)
        {
            image = g_imgui_Images[i];
            lua_pushinteger(L, image.tid);
            lua_pushinteger(L, image.w);
            lua_pushinteger(L, image.h);
            return 3;
        }
    }

    unsigned char *strdata = (unsigned char *)luaL_checkstring(L, 2);
    int lendata = luaL_checkinteger(L, 3);
    image.data = stbi_load_from_memory( strdata, lendata, &image.w, &image.h, NULL, STBI_rgb_alpha);
    if (image.data == 0)
    {
        dmLogError("Error loading image: %s", filename);
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
        return 3;
    }

    if (!imgui_ImageInternalLoad(filename, &image))
    {
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
        return 3;
    }

    stbi_image_free(image.data);
    image.data = 0;
    lua_pushinteger(L, image.tid);
    lua_pushinteger(L, image.w);
    lua_pushinteger(L, image.h);
    return 3;
}

// Image handling needs to be smarter, but this will do for the time being.
static int imgui_ImageLoad(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 3);
    const char * filename = luaL_checkstring(L, 1);
    dmLogInfo("imgui_ImageLoad %s", filename);
    ImgObject image;

    // If its already in the vector, return the id
    for (int i = 0; i < g_imgui_Images.Size(); i++)
    {
        if(strcmp(g_imgui_Images[i].name, filename) == 0)
        {
            image = g_imgui_Images[i];
            lua_pushinteger(L, image.tid);
            lua_pushinteger(L, image.w);
            lua_pushinteger(L, image.h);
            return 3;
        }
    }

    image.data = stbi_load(filename, &image.w, &image.h, NULL, STBI_rgb_alpha);
    if (image.data == 0)
    {
        dmLogError("Error loading image: %s", filename);
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
        return 3;
    }

    if (!imgui_ImageInternalLoad(filename, &image))
    {
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
        return 3;
    }

    stbi_image_free(image.data);
    image.data = 0;
    lua_pushinteger(L, image.tid);
    lua_pushinteger(L, image.w);
    lua_pushinteger(L, image.h);
    return 3;
}

static int imgui_ImageGet( lua_State *L )
{
    DM_LUA_STACK_CHECK(L, 3);
    GLuint tid = (GLuint)luaL_checkinteger(L, 1);

    for (int i = 0; i < g_imgui_Images.Size(); i++)
    {
        if (g_imgui_Images[i].tid == tid)
        {
            ImgObject image = g_imgui_Images[i];
            lua_pushinteger(L, image.tid);
            lua_pushinteger(L, image.w);
            lua_pushinteger(L, image.h);
            return 3;
        }
    }

    lua_pushnil(L);
    lua_pushnil(L);
    lua_pushnil(L);
    return 3;
}

static int imgui_ImageAdd( lua_State *L )
{
    DM_LUA_STACK_CHECK(L, 0);
    GLuint tid = (GLuint)luaL_checkinteger(L, 1);
    int w = luaL_checknumber(L, 2);
    int h = luaL_checknumber(L, 3);

    ImVec2 uv0 = ImVec2(0, 0);
    if (lua_isnumber(L, 4) && lua_isnumber(L, 5)) {
        uv0.x = luaL_checknumber(L, 4);
        uv0.y = luaL_checknumber(L, 5);
    }
    ImVec2 uv1 = ImVec2(1, 1);
    if (lua_isnumber(L, 6) && lua_isnumber(L, 7)) {
        uv1.x = luaL_checknumber(L, 6);
        uv1.y = luaL_checknumber(L, 7);
    }
    ImGui::Image((void*)(intptr_t)tid, ImVec2(w, h), uv0, uv1);
    return 0;
}

static int imgui_ImageFree( lua_State *L )
{
    DM_LUA_STACK_CHECK(L, 0);
    GLuint tid = (GLuint)luaL_checkinteger(L, 1);
    for (int i = 0; i < g_imgui_Images.Size(); i++)
    {
        if (g_imgui_Images[i].tid == tid)
        {
            glDeleteTextures(1, &tid);
            g_imgui_Images.EraseSwap(i);
            break;
        }
    }
    return 0;
}

// ----------------------------
// ----- PRIMITIVES -----------
// ----------------------------

static int imgui_DrawListAddLine(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    float x1 = luaL_checknumber(L, 1);
    float y1 = luaL_checknumber(L, 2);
    float x2 = luaL_checknumber(L, 3);
    float y2 = luaL_checknumber(L, 4);

    float r = (float)luaL_checknumber(L, 5);
    float g = (float)luaL_checknumber(L, 6);
    float b = (float)luaL_checknumber(L, 7);
    float a = (float)luaL_checknumber(L, 8);

    float thickness = 1.0f;
    if (lua_isnumber(L, 9))
    {
        thickness = luaL_checknumber(L, 9);
    }

    ImGui::GetWindowDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(r, g, b, a), thickness);
    return 0;
}

static int imgui_DrawListAddRect(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    float x1 = luaL_checknumber(L, 1);
    float y1 = luaL_checknumber(L, 2);
    float x2 = luaL_checknumber(L, 3);
    float y2 = luaL_checknumber(L, 4);

    float r = (float)luaL_checknumber(L, 5);
    float g = (float)luaL_checknumber(L, 6);
    float b = (float)luaL_checknumber(L, 7);
    float a = (float)luaL_checknumber(L, 8);

    float thickness = 1.0f;
    if (lua_isnumber(L, 9))
    {
        thickness = luaL_checknumber(L, 9);
    }
    float rounding = 0.0f;
    int flags = 0;

    ImGui::GetWindowDrawList()->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(r, g, b, a), rounding, flags, thickness);
    return 0;
}

// ----------------------------
// ----- FRAMES ---------------
// ----------------------------

static void imgui_NewFrame()
{
    if (g_imgui_NewFrame == false)
    {
        ImGui_ImplOpenGL3_NewFrame();
        imgui_ClearGLError();
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
// ----- Push/Pop ID ----------
// ----------------------------
static int imgui_PushId(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    const char* text = luaL_checkstring(L, 1);
    ImGui::PushID(text);

    return 0;
}

static int imgui_PopId(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGui::PopID();
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
    bool border = false;
    if (lua_isnumber(L, 4)) border = luaL_checkinteger(L, 4) == 1;
    int flags = 0;
    if (lua_isnumber(L, 5)) flags = luaL_checknumber(L, 5);

    bool result = ImGui::BeginChild(title, ImVec2(width, height), border, flags);
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
// ----- POPUP ---------
// ----------------------------

static int imgui_BeginPopupContextItem(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    const char* id = luaL_checkstring(L, 1);
    bool result = ImGui::BeginPopupContextItem(id);
    lua_pushboolean(L, result);
    return 1;
}

static int imgui_BeginPopup(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    const char* id = luaL_checkstring(L, 1);
    int flags = 0;
    if (lua_isnumber(L, 2)) flags = luaL_checknumber(L, 2);
    bool result = ImGui::BeginPopup(id, flags);
    lua_pushboolean(L, result);
    return 1;
}

static int imgui_BeginPopupModal(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    const char* name = luaL_checkstring(L, 1);
    int flags = 0;
    if (lua_isnumber(L, 2)) flags = luaL_checknumber(L, 2);
    bool result = ImGui::BeginPopupModal(name, 0, flags);
    lua_pushboolean(L, result);
    return 1;
}

static int imgui_OpenPopup(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    const char* id = luaL_checkstring(L, 1);
    int flags = 0;
    if (lua_isnumber(L, 2)) flags = luaL_checknumber(L, 2);
    ImGui::OpenPopup(id, flags);
    return 0;
}

static int imgui_CloseCurrentPopup(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::CloseCurrentPopup();
    return 0;
}

static int imgui_EndPopup(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::EndPopup();
    return 0;
}

// ----------------------------
// ----- DRAG AND DROP --------
// ----------------------------
static int imgui_BeginDragDropSource(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    uint32_t flags = 0;
    if (lua_isnumber(L, 1))
    {
        flags = luaL_checkint(L, 1);
    }
    bool result = ImGui::BeginDragDropSource(flags);
    lua_pushboolean(L, result);
    return 1;
}
static int imgui_EndDragDropSource(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::EndDragDropSource();
    return 0;
}
static int imgui_BeginDragDropTarget(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    bool result = ImGui::BeginDragDropTarget();
    lua_pushboolean(L, result);
    return 1;
}
static int imgui_EndDragDropTarget(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::EndDragDropTarget();
    return 0;
}

static int imgui_SetDragDropPayload(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    const char* type = luaL_checkstring(L, 1);
    const char* payload = luaL_checkstring(L, 2);

    bool result = ImGui::SetDragDropPayload(type, payload, strlen(payload));
    lua_pushboolean(L, result);
    return 1;
}
static int imgui_AcceptDragDropPayload(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    const char* type = luaL_checkstring(L, 1);
    uint32_t flags = 0;
    if (lua_isnumber(L, 2))
    {
        flags = luaL_checkint(L, 2);
    }
    const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type, flags);
    if (payload) {
        lua_pushlstring(L, (char*)payload->Data, payload->DataSize);
    }
    else {
        lua_pushnil(L);
    }
    return 1;
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
    uint32_t flags = 0;
    if (lua_isnumber(L, 3))
    {
        flags = luaL_checkint(L, 3);
    }
    bool result = ImGui::BeginTable(id, column, flags);
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
    float weight = 0.0;
    if (lua_isnumber(L, 3))
    {
        weight = luaL_checknumber(L, 3);
    }
    ImGui::TableSetupColumn(label, flags, weight);
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
// ----- TOOLTIP ---------
// ----------------------------
static int imgui_BeginTooltip(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::BeginTooltip();
    return 0;
}
static int imgui_EndTooltip(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    ImGui::EndTooltip();
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
    int wrapped =  0;
    if(lua_isnumber(L, 2)) wrapped = luaL_checknumber(L, 2);

    if(wrapped == 1)
        ImGui::TextWrapped("%s", text);
    else
        ImGui::Text("%s", text);
    return 0;
}

static int imgui_TextGetSize(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);
    int argc = lua_gettop(L);
    const char* text = luaL_checkstring(L, 1);
    float font_size = luaL_checknumber(L, 2);
    int fontid = 0;
    if(argc > 2)
    {
        fontid = luaL_checkinteger(L, 3);
    }
    ImFont *font = g_imgui_Fonts[fontid];
    ImVec2 sz = font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, text);

    lua_pushnumber(L, sz.x);
    lua_pushnumber(L, sz.y);
    return 2;
}

static int imgui_TextColored(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    const char* text = luaL_checkstring(L, 1);
    float r = (float)luaL_checknumber(L, 2);
    float g = (float)luaL_checknumber(L, 3);
    float b = (float)luaL_checknumber(L, 4);
    float a = (float)luaL_checknumber(L, 5);
    ImVec4    color(r, g, b, a);
    ImGui::TextColored(color, "%s", text);
    return 0;
}

static int imgui_InputText(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);
    imgui_NewFrame();
    const char* label = luaL_checkstring(L, 1);
    const char* text = luaL_checkstring(L, 2);
    dmStrlCpy(g_imgui_TextBuffer, text, TEXTBUFFER_SIZE);
    uint32_t flags = 0;
    if (lua_isnumber(L, 3))
    {
        flags = luaL_checkint(L, 3);
    }
    bool changed = ImGui::InputText(label, g_imgui_TextBuffer, TEXTBUFFER_SIZE, flags);
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

static int imgui_InputFloat(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);
    imgui_NewFrame();
    const char* label = luaL_checkstring(L, 1);
    float value = luaL_checknumber(L, 2);
    float step = 0.01f;
    float step_fast = 1.0f;
    char float_precision[20] = { "%.6f" };

    // check if the third argument is a number
    if (lua_isnumber(L, 3))
    {
        step = luaL_checknumber(L, 3);
        // Only accept 4th if we have 3rd param
        if (lua_isnumber(L, 4))
        {
            step_fast = luaL_checknumber(L, 4);

            // Only accept 5th if we have 4th param
            if (lua_isnumber(L, 5))
            {
                int precision_count = lua_tointeger(L, 5);
                dmSnPrintf(float_precision, sizeof(float_precision), "%%.%df", precision_count );
            }
        }
    }
    bool changed = ImGui::InputFloat(label, &value, step, step_fast, float_precision);
    lua_pushboolean(L, changed);
    if (changed)
    {
        lua_pushnumber(L, value);
    }
    else
    {
        lua_pushnil(L);
    }
    return 2;
}

static int imgui_InputDouble(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);
    imgui_NewFrame();
    const char* label = luaL_checkstring(L, 1);
    double value = luaL_checknumber(L, 2);
    double step = 0.01f;
    double step_fast = 1.0f;
    char dbl_precision[20] = { "%.6f" };

    // check if the third argument is a number
    if (lua_isnumber(L, 3))
    {
        step = luaL_checknumber(L, 3);
        // Only accept 4th if we have 3rd param
        if (lua_isnumber(L, 4))
        {
            step_fast = luaL_checknumber(L, 4);

            // Only accept 5th if we have 4th param
            if (lua_isnumber(L, 5))
            {
                int precision_count = lua_tointeger(L, 5);
                dmSnPrintf(dbl_precision, sizeof(dbl_precision), "%%.%df", precision_count );
            }
        }
    }
    bool changed = ImGui::InputDouble(label, &value, step, step_fast, dbl_precision);
    lua_pushboolean(L, changed);
    if (changed)
    {
        lua_pushnumber(L, value);
    }
    else
    {
        lua_pushnil(L);
    }
    return 2;
}

static int imgui_InputInt4(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 5);
    imgui_NewFrame();
    const char* label = luaL_checkstring(L, 1);
    int v[4];
    v[0]  = luaL_checkinteger(L, 2);
    v[1]  = luaL_checkinteger(L, 3);
    v[2]  = luaL_checkinteger(L, 4);
    v[3]  = luaL_checkinteger(L, 5);

    bool changed = ImGui::InputInt4(label, v);
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
    int argc = lua_gettop(L);
    imgui_NewFrame();
    const char* text = luaL_checkstring(L, 1);
    bool pushed = false;
    if(argc > 1)
    {
        int width = luaL_checkinteger(L, 2);
        int height = luaL_checkinteger(L, 3);
        pushed = ImGui::Button(text, ImVec2(width, height));
    }
    else
    {
        pushed = ImGui::Button(text);
    }
    lua_pushboolean(L, pushed);
    return 1;
}

static int imgui_ButtonImage(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    int argc = lua_gettop(L);
    imgui_NewFrame();
    GLuint tid = (GLuint)luaL_checknumber(L, 1);
    bool pushed = false;
    if(argc > 1)
    {
        int width = luaL_checkinteger(L, 2);
        int height = luaL_checkinteger(L, 3);
        pushed = ImGui::ImageButton((void*)(intptr_t)tid, ImVec2(width, height));
    }
    else
    {
        pushed = ImGui::ImageButton((void*)(intptr_t)tid, ImVec2(0,0));
    }
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

static int imgui_GetCursorScreenPos(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 2);

    ImVec2 p = ImGui::GetCursorScreenPos();
    lua_pushnumber(L, p.x);
    lua_pushnumber(L, p.y);
    return 2;
}

// ----------------------------
// ----- IMGUI PLOT -----------
// ----------------------------

static float     values_lines[MAX_HISTOGRAM_VALUES];

static int imgui_PlotLines(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const char *lbl = luaL_checkstring(L, 1);
    int valoff = luaL_checkinteger(L, 2);
    int width = luaL_checkinteger(L, 3);
    int height = luaL_checkinteger(L, 4);
    luaL_checktype(L, 5, LUA_TTABLE);

    // Table is at idx 5
    lua_pushnil(L);
    int valct = 0;
    // Build a number array matching the buffer. They are all assumed to be type float (for the time being)
    while(( lua_next( L, 5 ) != 0) && (valct < MAX_HISTOGRAM_VALUES)) {
        values_lines[valct++] = (float)lua_tonumber( L, -1 );
        lua_pop( L, 1 );
    }

    imgui_NewFrame();
    ImVec2    gsize(width, height);
    ImGui::PlotLines(lbl, values_lines, valct, valoff, NULL, FLT_MAX, FLT_MAX, gsize);
    return 0;
}

// Keep a label mapped histograms. This minimised alloc and realloc of value mem
static float     values_hist[MAX_HISTOGRAM_VALUES];

static int imgui_PlotHistogram(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const char *lbl = luaL_checkstring(L, 1);
    int valoff = luaL_checkinteger(L, 2);
    int width = luaL_checkinteger(L, 3);
    int height = luaL_checkinteger(L, 4);
    luaL_checktype(L, 5, LUA_TTABLE);

    // Table is at idx 5
    lua_pushnil(L);
    int valct = 0;
    // Build a number array matching the buffer. They are all assumed to be type float (for the time being)
    while(( lua_next( L, 5 ) != 0) && (valct < MAX_HISTOGRAM_VALUES)) {
        values_hist[valct++] = (float)lua_tonumber( L, -1 );
        lua_pop( L, 1 );
    }

    imgui_NewFrame();
    ImVec2    gsize(width, height);
    ImGui::PlotHistogram(lbl, values_hist, valct, valoff, NULL, FLT_MAX, FLT_MAX, gsize);
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
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    uint32_t button = luaL_checknumber(L, 1);
    bool clicked = ImGui::IsMouseDoubleClicked(button);
    lua_pushboolean(L, clicked);
    return 1;
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

static int imgui_IsItemActive(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    bool active = ImGui::IsItemActive();
    lua_pushboolean(L, active);
    return 1;
}

static int imgui_IsItemFocused(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    bool focused = ImGui::IsItemFocused();
    lua_pushboolean(L, focused);
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

static int imgui_IsItemDoubleClicked(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    imgui_NewFrame();
    uint32_t button = luaL_checknumber(L, 1);
    bool clicked = ImGui::IsItemClicked(button);
    bool double_clicked = ImGui::IsMouseDoubleClicked(button);
    lua_pushboolean(L, clicked && double_clicked);
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

static int imgui_SetStyleWindowBorderSize(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = luaL_checknumber(L, 1);
    return 0;
}

static int imgui_SetStyleChildBorderSize(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGuiStyle& style = ImGui::GetStyle();
    style.ChildBorderSize = luaL_checknumber(L, 1);
    return 0;
}

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
static int imgui_SetStyleTabRounding(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGuiStyle& style = ImGui::GetStyle();
    style.TabRounding = luaL_checknumber(L, 1);
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
static int imgui_PushStyleColor(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    double r = luaL_checknumber(L, 2);
    double g = luaL_checknumber(L, 3);
    double b = luaL_checknumber(L, 4);
    double a = luaL_checknumber(L, 5);
    ImGuiCol col = luaL_checkinteger(L, 1);
    ImGui::PushStyleColor(col, ImVec4(r, g, b, a));
    return 0;
}
static int imgui_PopStyleColor(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    int count = 1;
    if (lua_isnumber(L, 1))
    {
        count = luaL_checkint(L, 1);
    }
    ImGui::PopStyleColor(count);
    return 0;
}

static int imgui_SetWindowFontScale(lua_State *L)
{
    DM_LUA_STACK_CHECK(L, 0);
    float scale = luaL_checknumber(L, 1);
    ImGui::SetWindowFontScale(scale);
    return 0;
}

static int imgui_SetGlobalFontScale(lua_State *L)
{
    DM_LUA_STACK_CHECK(L, 0);
    float scale = luaL_checknumber(L, 1);
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = scale;
    return 0;
}

static int imgui_ScaleAllSizes(lua_State *L)
{
    DM_LUA_STACK_CHECK(L, 0);
    float scale = luaL_checknumber(L, 1);
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(scale);
    return 0;
}


static int imgui_SetCursorPos(lua_State *L)
{
    DM_LUA_STACK_CHECK(L, 0);
    int posx = luaL_checkinteger(L, 1);
    int posy = luaL_checkinteger(L, 2);
    ImVec2     pos(posx, posy);
    ImGui::SetCursorPos(pos);
    return 0;
}


// ----------------------------
// ----- NAVIGATION -----------------
// ----------------------------

static int imgui_SetScrollHereY(lua_State *L)
{
    DM_LUA_STACK_CHECK(L, 0);
    imgui_NewFrame();
    float center_y_ratio = luaL_checknumber(L, 1);
    ImGui::SetScrollHereY(center_y_ratio);
    return 0;
}


// ----------------------------
// ----- FONT -----------------
// ----------------------------

static int imgui_StoreFont(ImFont* font)
{
    if (g_imgui_Fonts.Full())
    {
        g_imgui_Fonts.OffsetCapacity(2);
    }
    g_imgui_Fonts.Push(font);
    return g_imgui_Fonts.Size() - 1;
}

static ImFont* imgui_GetFont(int index)
{
    if (index >= 0 && index < g_imgui_Fonts.Size())
    {
        return g_imgui_Fonts[index];
    }
    return 0;
}

static int imgui_FontAddTTFFile(lua_State * L)
{
    DM_LUA_STACK_CHECK(L, 1);
    const char * ttf_filename = luaL_checkstring(L, 1);
    float font_size = luaL_checknumber(L, 2);

    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF(ttf_filename, font_size);
    // Put font in map.
    if(font != NULL)
    {
        int index = imgui_StoreFont(font);
        lua_pushinteger(L, index);
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int imgui_FontAddTTFData(lua_State * L)
{
    DM_LUA_STACK_CHECK(L, 1);
    const char * ttf_data = luaL_checkstring(L, 1);
    int ttf_data_size = luaL_checknumber(L, 2);
    float font_size = luaL_checknumber(L, 3);
    int font_pixels = luaL_checknumber(L, 4);

    char *ttf_data_cpy = (char *)calloc(ttf_data_size, sizeof(char));
    memcpy(ttf_data_cpy, ttf_data, ttf_data_size);

    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromMemoryTTF((void *)ttf_data_cpy, font_size, font_pixels);
    // Put font in map.
    if(font != NULL)
    {
        int index = imgui_StoreFont(font);
        lua_pushinteger(L, index);
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int imgui_FontPush(lua_State *L)
{
    DM_LUA_STACK_CHECK(L, 0);
    int fontid = luaL_checkinteger(L, 1);
    ImFont* font = imgui_GetFont(fontid);
    if (font)
    {
        ImGui::PushFont(font);
    }
    return 0;
}

static int imgui_FontPop(lua_State *L)
{
    DM_LUA_STACK_CHECK(L, 0);
    ImGui::PopFont();
    return 0;
}

static int imgui_FontScale(lua_State *L)
{
    DM_LUA_STACK_CHECK(L, 1);
    float oldscale = 1.0f;
    int fontid = luaL_checkinteger(L, 1);
    float fontscale = luaL_checknumber(L, 2);

    ImFont* font = imgui_GetFont(fontid);
    if(font)
    {
        oldscale = font->Scale;
        font->Scale = fontscale;
    }
    lua_pushnumber(L, oldscale);
    return 1;
}

// ----------------------------
// ----- DRAW -----------------
// ----------------------------
static dmExtension::Result imgui_Draw(dmExtension::Params* params)
{
    imgui_NewFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    imgui_ClearGLError();

    g_imgui_NewFrame = false;
    return dmExtension::RESULT_OK;
}

static int imgui_DrawLine(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    int x1 = luaL_checkinteger(L, 1);
    int y1 = luaL_checkinteger(L, 2);
    int x2 = luaL_checkinteger(L, 3);
    int y2 = luaL_checkinteger(L, 4);
    unsigned int col = (unsigned int)luaL_checkinteger(L, 5);
    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 p1(x1, y1);
    ImVec2 p2(x2, y2);
    dl->AddLine(p1, p2, col);
    return 0;
}

static int imgui_DrawRect(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    unsigned int col = (unsigned int)luaL_checkinteger(L, 5);
    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 minv(x, y);
    ImVec2 maxv(x + w, y + h);
    dl->AddRect(minv, maxv, col);
    return 0;
}

static int imgui_DrawRectFilled(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    unsigned int col = (unsigned int)luaL_checkinteger(L, 5);
    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 minv(x, y);
    ImVec2 maxv(x + w, y + h);
    dl->AddRectFilled(minv, maxv, col);
    return 0;
}

static int imgui_DrawProgressBar(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    float progress = luaL_checknumber(L, 1);
    float xsize = luaL_checknumber(L, 2);
    float ysize = luaL_checknumber(L, 3);

    ImVec2 size_param(xsize, ysize);
    ImGui::ProgressBar(progress, size_param);
    return 0;
}

// ----------------------------
// ----- CONFIG -----------------
// ----------------------------
static int imgui_SetDefaults(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    ImGuiIO& io = ImGui::GetIO();
    ImFont* def = io.Fonts->AddFontDefault();
    imgui_StoreFont(def);
    return 0;
}

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
// ----- IMGUI INIT/SHUTDOWN --
// ----------------------------

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
    imgui_ClearGLError();
}

static void imgui_Shutdown()
{
    dmLogInfo("imgui_Shutdown");

    ImGui_ImplOpenGL3_Shutdown();
    imgui_ClearGLError();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
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

    while (!g_imgui_Images.Empty())
    {
        glDeleteTextures(1, &g_imgui_Images.Back().tid);
        g_imgui_Images.Pop();
    }

    g_imgui_Fonts.SetSize(0);
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"image_load", imgui_ImageLoad},
    {"image_load_data", imgui_ImageLoadData},
    {"image_get", imgui_ImageGet},
    {"image_free", imgui_ImageFree},
    {"image_add", imgui_ImageAdd},

    {"image_b64_decode", imgui_ImageB64Decode},

    {"font_add_ttf_file", imgui_FontAddTTFFile},
    {"font_add_ttf_data", imgui_FontAddTTFData},
    {"font_push", imgui_FontPush},
    {"font_pop", imgui_FontPop},
    {"font_scale", imgui_FontScale},

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

    {"begin_tooltip", imgui_BeginTooltip},
    {"end_tooltip", imgui_EndTooltip},

    {"begin_combo", imgui_BeginCombo},
    {"end_combo", imgui_EndCombo},
    {"combo", imgui_Combo},

    {"begin_dragdrop_source", imgui_BeginDragDropSource},
    {"end_dragdrop_source", imgui_EndDragDropSource},
    {"begin_dragdrop_target", imgui_BeginDragDropTarget},
    {"end_dragdrop_target", imgui_EndDragDropTarget},
    {"set_dragdrop_payload", imgui_SetDragDropPayload},
    {"accept_dragdrop_payload", imgui_AcceptDragDropPayload},

    {"begin_table", imgui_BeginTable},
    {"end_table", imgui_EndTable},
    {"table_next_row", imgui_TableNextRow},
    {"table_next_column", imgui_TableNextColumn},
    {"table_set_column_index", imgui_TableSetColumnIndex},
    {"table_setup_column", imgui_TableSetupColumn},
    {"table_headers_row", imgui_TableHeadersRow},

    {"begin_popup_context_item", imgui_BeginPopupContextItem},
    {"begin_popup", imgui_BeginPopup},
    {"begin_popup_modal", imgui_BeginPopupModal},
    {"open_popup", imgui_OpenPopup},
    {"close_current_popup", imgui_CloseCurrentPopup},
    {"end_popup", imgui_EndPopup},

    {"tree_node", imgui_TreeNode},
    {"tree_pop", imgui_TreePop},

    {"push_id", imgui_PushId},
    {"pop_id", imgui_PopId},

    {"selectable", imgui_Selectable},
    {"text", imgui_Text},
    {"text_colored", imgui_TextColored},
    {"input_text", imgui_InputText},
    {"input_int", imgui_InputInt},
    {"input_int4", imgui_InputInt4},
    {"input_float", imgui_InputFloat},
    {"input_double", imgui_InputDouble},
    {"input_float3", imgui_InputFloat3},
    {"input_float4", imgui_InputFloat4},
    {"button", imgui_Button},
    {"button_image", imgui_ButtonImage},
    {"checkbox", imgui_Checkbox},

    {"same_line", imgui_SameLine},
    {"new_line", imgui_NewLine},
    {"bullet", imgui_Bullet},
    {"indent", imgui_Indent},
    {"unindent", imgui_Unindent},
    {"spacing", imgui_Spacing},
    {"separator", imgui_Separator},
    {"get_cursor_screen_pos", imgui_GetCursorScreenPos},

    {"add_line", imgui_DrawListAddLine},
    {"add_rect", imgui_DrawListAddRect},

    {"plot_lines", imgui_PlotLines},
    {"plot_histogram", imgui_PlotHistogram},

    {"text_getsize", imgui_TextGetSize},

    {"draw_rect", imgui_DrawRect},
    {"draw_rect_filled", imgui_DrawRectFilled},
    {"draw_line", imgui_DrawLine},
    {"draw_progress", imgui_DrawProgressBar},

    {"demo", imgui_Demo},

    {"set_mouse_input", imgui_SetMouseInput},
    {"set_key_down", imgui_SetKeyDown},
    {"set_key_modifier_ctrl", imgui_SetKeyModifierCtrl},
    {"set_key_modifier_shift", imgui_SetKeyModifierShift},
    {"set_key_modifier_alt", imgui_SetKeyModifierAlt},
    {"set_key_modifier_super", imgui_SetKeyModifierSuper},
    {"add_input_character", imgui_AddInputCharacter},

    {"is_item_active", imgui_IsItemActive},
    {"is_item_focused", imgui_IsItemFocused},
    {"is_item_clicked", imgui_IsItemClicked},
    {"is_item_double_clicked", imgui_IsItemDoubleClicked},
    {"is_item_hovered", imgui_IsItemHovered},
    {"is_mouse_clicked", imgui_IsMouseClicked},
    {"is_mouse_double_clicked", imgui_IsMouseDoubleClicked},

    {"set_style_window_rounding", imgui_SetStyleWindowRounding},
    {"set_style_window_bordersize", imgui_SetStyleWindowBorderSize},
    {"set_style_child_bordersize", imgui_SetStyleChildBorderSize},
    {"set_style_frame_rounding", imgui_SetStyleFrameRounding},
    {"set_style_tab_rounding", imgui_SetStyleTabRounding},
    {"set_style_scrollbar_rounding", imgui_SetStyleScrollbarRounding},
    {"set_style_color", imgui_SetStyleColor},
    {"push_style_color", imgui_PushStyleColor},
    {"pop_style_color", imgui_PopStyleColor},

    {"set_defaults", imgui_SetDefaults},
    {"set_ini_filename", imgui_SetIniFilename},

    {"set_cursor_pos", imgui_SetCursorPos},
    {"set_display_size", imgui_SetDisplaySize},
    {"set_window_font_scale", imgui_SetWindowFontScale},
    {"set_global_font_scale", imgui_SetGlobalFontScale},
    {"scale_all_sizes", imgui_ScaleAllSizes},

    {"set_scroll_here_y", imgui_SetScrollHereY},
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

    lua_setfieldstringint(L, "KEY_TAB", ImGuiKey_Tab);
    lua_setfieldstringint(L, "KEY_LEFTARROW", ImGuiKey_LeftArrow);
    lua_setfieldstringint(L, "KEY_RIGHTARROW", ImGuiKey_RightArrow);
    lua_setfieldstringint(L, "KEY_UPARROW", ImGuiKey_UpArrow);
    lua_setfieldstringint(L, "KEY_DOWNARROW", ImGuiKey_DownArrow);
    lua_setfieldstringint(L, "KEY_PAGEUP", ImGuiKey_PageUp);
    lua_setfieldstringint(L, "KEY_PAGEDOWN", ImGuiKey_PageDown);
    lua_setfieldstringint(L, "KEY_HOME", ImGuiKey_Home);
    lua_setfieldstringint(L, "KEY_END", ImGuiKey_End);
    lua_setfieldstringint(L, "KEY_INSERT", ImGuiKey_Insert);
    lua_setfieldstringint(L, "KEY_DELETE", ImGuiKey_Delete);
    lua_setfieldstringint(L, "KEY_BACKSPACE", ImGuiKey_Backspace);
    lua_setfieldstringint(L, "KEY_SPACE", ImGuiKey_Space);
    lua_setfieldstringint(L, "KEY_ENTER", ImGuiKey_Enter);
    lua_setfieldstringint(L, "KEY_ESCAPE", ImGuiKey_Escape);
    lua_setfieldstringint(L, "KEY_KEYPADENTER", ImGuiKey_KeyPadEnter);
    lua_setfieldstringint(L, "KEY_A", ImGuiKey_A);
    lua_setfieldstringint(L, "KEY_C", ImGuiKey_C);
    lua_setfieldstringint(L, "KEY_V", ImGuiKey_V);
    lua_setfieldstringint(L, "KEY_X", ImGuiKey_X);
    lua_setfieldstringint(L, "KEY_Y", ImGuiKey_Y);
    lua_setfieldstringint(L, "KEY_Z", ImGuiKey_Z);

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

    lua_setfieldstringint(L, "TABLE_NONE", ImGuiTableFlags_None);
    lua_setfieldstringint(L, "TABLE_RESIZABLE", ImGuiTableFlags_Resizable);   // Enable resizing columns.
    lua_setfieldstringint(L, "TABLE_REORDERABLE", ImGuiTableFlags_Reorderable);   // Enable reordering columns in header row (need calling TableSetupColumn() + TableHeadersRow() to display headers)
    lua_setfieldstringint(L, "TABLE_HIDEABLE", ImGuiTableFlags_Hideable);   // Enable hiding/disabling columns in context menu.
    lua_setfieldstringint(L, "TABLE_SORTABLE", ImGuiTableFlags_Sortable);   // Enable sorting. Call TableGetSortSpecs() to obtain sort specs. Also see ImGuiTableFlags_SortMulti and ImGuiTableFlags_SortTristate.
    lua_setfieldstringint(L, "TABLE_NOSAVEDSETTINGS", ImGuiTableFlags_NoSavedSettings);   // Disable persisting columns order, width and sort settings in the .ini file.
    lua_setfieldstringint(L, "TABLE_CONTEXTMENUINBODY", ImGuiTableFlags_ContextMenuInBody);   // Right-click on columns body/contents will display table context menu. By default it is available in TableHeadersRow().
    lua_setfieldstringint(L, "TABLE_ROWBG", ImGuiTableFlags_RowBg);   // Set each RowBg color with ImGuiCol_TableRowBg or ImGuiCol_TableRowBgAlt (equivalent of calling TableSetBgColor with ImGuiTableBgFlags_RowBg0 on each row manually)
    lua_setfieldstringint(L, "TABLE_BORDERSINNERH", ImGuiTableFlags_BordersInnerH);   // Draw horizontal borders between rows.
    lua_setfieldstringint(L, "TABLE_BORDERSOUTERH", ImGuiTableFlags_BordersOuterH);   // Draw horizontal borders at the top and bottom.
    lua_setfieldstringint(L, "TABLE_BORDERSINNERV", ImGuiTableFlags_BordersInnerV);   // Draw vertical borders between columns.
    lua_setfieldstringint(L, "TABLE_BORDERSOUTERV", ImGuiTableFlags_BordersOuterV);  // Draw vertical borders on the left and right sides.
    lua_setfieldstringint(L, "TABLE_BORDERSH", ImGuiTableFlags_BordersH); // Draw horizontal borders.
    lua_setfieldstringint(L, "TABLE_BORDERSV", ImGuiTableFlags_BordersV); // Draw vertical borders.
    lua_setfieldstringint(L, "TABLE_BORDERSINNER", ImGuiTableFlags_BordersInner); // Draw inner borders.
    lua_setfieldstringint(L, "TABLE_BORDERSOUTER", ImGuiTableFlags_BordersOuter); // Draw outer borders.
    lua_setfieldstringint(L, "TABLE_BORDERS", ImGuiTableFlags_Borders);   // Draw all borders.
    lua_setfieldstringint(L, "TABLE_NOBORDERSINBODY", ImGuiTableFlags_NoBordersInBody);  // [ALPHA] Disable vertical borders in columns Body (borders will always appears in Headers). -> May move to style
    lua_setfieldstringint(L, "TABLE_NOBORDERSINBODYUNTILRESIZE", ImGuiTableFlags_NoBordersInBodyUntilResize);  // [ALPHA] Disable vertical borders in columns Body until hovered for resize (borders will always appears in Headers). -> May move to style
    lua_setfieldstringint(L, "TABLE_SIZINGFIXEDFIT", ImGuiTableFlags_SizingFixedFit);  // Columns default to _WidthFixed or _WidthAuto (if resizable or not resizable), matching contents width.
    lua_setfieldstringint(L, "TABLE_SIZINGFIXEDSAME", ImGuiTableFlags_SizingFixedSame);  // Columns default to _WidthFixed or _WidthAuto (if resizable or not resizable), matching the maximum contents width of all columns. Implicitly enable ImGuiTableFlags_NoKeepColumnsVisible.
    lua_setfieldstringint(L, "TABLE_SIZINGSTRETCHPROP", ImGuiTableFlags_SizingStretchProp);  // Columns default to _WidthStretch with default weights proportional to each columns contents widths.
    lua_setfieldstringint(L, "TABLE_SIZINGSTRETCHSAME", ImGuiTableFlags_SizingStretchSame);  // Columns default to _WidthStretch with default weights all equal, unless overriden by TableSetupColumn().
    lua_setfieldstringint(L, "TABLE_NOHOSTEXTENDX", ImGuiTableFlags_NoHostExtendX);  // Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used.
    lua_setfieldstringint(L, "TABLE_NOHOSTEXTENDY", ImGuiTableFlags_NoHostExtendY);  // Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.
    lua_setfieldstringint(L, "TABLE_NOKEEPCOLUMNSVISIBLE", ImGuiTableFlags_NoKeepColumnsVisible);  // Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable.
    lua_setfieldstringint(L, "TABLE_PRECISEWIDTHS", ImGuiTableFlags_PreciseWidths);  // Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.
    lua_setfieldstringint(L, "TABLE_NOCLIP", ImGuiTableFlags_NoClip);  // Disable clipping rectangle for every individual columns (reduce draw command count, items will be able to overflow into other columns). Generally incompatible with TableSetupScrollFreeze().
    lua_setfieldstringint(L, "TABLE_PADOUTERX", ImGuiTableFlags_PadOuterX);  // Default if BordersOuterV is on. Enable outer-most padding. Generally desirable if you have headers.
    lua_setfieldstringint(L, "TABLE_NOPADOUTERX", ImGuiTableFlags_NoPadOuterX);  // Default if BordersOuterV is off. Disable outer-most padding.
    lua_setfieldstringint(L, "TABLE_NOPADINNERX", ImGuiTableFlags_NoPadInnerX);  // Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off).
    lua_setfieldstringint(L, "TABLE_SCROLLX", ImGuiTableFlags_ScrollX);  // Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size. Changes default sizing policy. Because this create a child window, ScrollY is currently generally recommended when using ScrollX.
    lua_setfieldstringint(L, "TABLE_SCROLLY", ImGuiTableFlags_ScrollY);  // Enable vertical scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size.
    lua_setfieldstringint(L, "TABLE_SORTMULTI", ImGuiTableFlags_SortMulti);  // Hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).
    lua_setfieldstringint(L, "TABLE_SORTTRISTATE", ImGuiTableFlags_SortTristate);  // Allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).

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

    lua_setfieldstringint(L, "POPUPFLAGS_NONE", ImGuiPopupFlags_None);
    lua_setfieldstringint(L, "POPUPFLAGS_MOUSEBUTTONLEFT", ImGuiPopupFlags_MouseButtonLeft);        // For BeginPopupContext*(): open on Left Mouse release. Guaranteed to always be == 0 (same as ImGuiMouseButton_Left)
    lua_setfieldstringint(L, "POPUPFLAGS_MOUSEBUTTONRIGHT", ImGuiPopupFlags_MouseButtonRight);        // For BeginPopupContext*(): open on Right Mouse release. Guaranteed to always be == 1 (same as ImGuiMouseButton_Right)
    lua_setfieldstringint(L, "POPUPFLAGS_MOUSEBUTTONMIDDLE", ImGuiPopupFlags_MouseButtonMiddle);        // For BeginPopupContext*(): open on Middle Mouse release. Guaranteed to always be == 2 (same as ImGuiMouseButton_Middle)
    lua_setfieldstringint(L, "POPUPFLAGS_MOUSEBUTTONMASK", ImGuiPopupFlags_MouseButtonMask_);
    lua_setfieldstringint(L, "POPUPFLAGS_MOUSEBUTTONDEFAULT", ImGuiPopupFlags_MouseButtonDefault_);
    lua_setfieldstringint(L, "POPUPFLAGS_NOOPENOVEREXISTINGPOPUP", ImGuiPopupFlags_NoOpenOverExistingPopup);   // For OpenPopup*(), BeginPopupContext*(): don't open if there's already a popup at the same level of the popup stack
    lua_setfieldstringint(L, "POPUPFLAGS_NOOPENOVERITEMS", ImGuiPopupFlags_NoOpenOverItems);   // For BeginPopupContextWindow(): don't return true when hovering items, only when hovering empty space
    lua_setfieldstringint(L, "POPUPFLAGS_ANYPOPUPID", ImGuiPopupFlags_AnyPopupId);   // For IsPopupOpen(): ignore the ImGuiID parameter and test for any popup.
    lua_setfieldstringint(L, "POPUPFLAGS_ANYPOPUPLEVEL", ImGuiPopupFlags_AnyPopupLevel);   // For IsPopupOpen(): search/test at any level of the popup stack (default test in the current level)
    lua_setfieldstringint(L, "POPUPFLAGS_ANYPOPUP", ImGuiPopupFlags_AnyPopup);

    lua_setfieldstringint(L, "DROPFLAGS_NONE", ImGuiDragDropFlags_None);
    lua_setfieldstringint(L, "DROPFLAGS_SOURCENOPREVIEWTOOLTIP", ImGuiDragDropFlags_SourceNoPreviewTooltip);   // By default, a successful call to BeginDragDropSource opens a tooltip so you can display a preview or description of the source contents. This flag disable this behavior.
    lua_setfieldstringint(L, "DROPFLAGS_SOURCENODISABLEHOVER", ImGuiDragDropFlags_SourceNoDisableHover);   // By default, when dragging we clear data so that IsItemHovered() will return false, to avoid subsequent user code submitting tooltips. This flag disable this behavior so you can still call IsItemHovered() on the source item.
    lua_setfieldstringint(L, "DROPFLAGS_SOURCENOHOLDTOOPENOTHERS", ImGuiDragDropFlags_SourceNoHoldToOpenOthers);   // Disable the behavior that allows to open tree nodes and collapsing header by holding over them while dragging a source item.
    lua_setfieldstringint(L, "DROPFLAGS_SOURCEALLOWNULLID", ImGuiDragDropFlags_SourceAllowNullID);   // Allow items such as Text(), Image() that have no unique identifier to be used as drag source, by manufacturing a temporary identifier based on their window-relative position. This is extremely unusual within the dear imgui ecosystem and so we made it explicit.
    lua_setfieldstringint(L, "DROPFLAGS_SOURCEEXTERN", ImGuiDragDropFlags_SourceExtern);   // External source (from outside of dear imgui), won't attempt to read current item/window info. Will always return true. Only one Extern source can be active simultaneously.
    lua_setfieldstringint(L, "DROPFLAGS_SOURCEAUTOEXPIREPAYLOAD", ImGuiDragDropFlags_SourceAutoExpirePayload);   // Automatically expire the payload if the source cease to be submitted (otherwise payloads are persisting while being dragged)
    lua_setfieldstringint(L, "DROPFLAGS_ACCEPTBEFOREDELIVERY", ImGuiDragDropFlags_AcceptBeforeDelivery);  // AcceptDragDropPayload() will returns true even before the mouse button is released. You can then call IsDelivery() to test if the payload needs to be delivered.
    lua_setfieldstringint(L, "DROPFLAGS_ACCEPTNODRAWDEFAULTRECT", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);  // Do not draw the default highlight rectangle when hovering over target.
    lua_setfieldstringint(L, "DROPFLAGS_ACCEPTNOPREVIEWTOOLTIP", ImGuiDragDropFlags_AcceptNoPreviewTooltip);  // Request hiding the BeginDragDropSource tooltip from the BeginDragDropTarget site.
    lua_setfieldstringint(L, "DROPFLAGS_ACCEPTPEEKONLY", ImGuiDragDropFlags_AcceptPeekOnly);  // For peeking ahead and inspecting the payload before delivery.


    lua_setfieldstringint(L, "INPUTFLAGS_NONE", ImGuiInputTextFlags_None);
    lua_setfieldstringint(L, "INPUTFLAGS_CHARSDECIMAL", ImGuiInputTextFlags_CharsDecimal);   // Allow 0123456789.+-*/
    lua_setfieldstringint(L, "INPUTFLAGS_CHARSHEXADECIMAL", ImGuiInputTextFlags_CharsHexadecimal);   // Allow 0123456789ABCDEFabcdef
    lua_setfieldstringint(L, "INPUTFLAGS_CHARSUPPERCASE", ImGuiInputTextFlags_CharsUppercase);   // Turn a..z into A..Z
    lua_setfieldstringint(L, "INPUTFLAGS_CHARSNOBLANK", ImGuiInputTextFlags_CharsNoBlank);   // Filter out spaces, tabs
    lua_setfieldstringint(L, "INPUTFLAGS_AUTOSELECTALL", ImGuiInputTextFlags_AutoSelectAll);   // Select entire text when first taking mouse focus
    lua_setfieldstringint(L, "INPUTFLAGS_ENTERRETURNSTRUE", ImGuiInputTextFlags_EnterReturnsTrue);   // Return 'true' when Enter is pressed (as opposed to every time the value was modified). Consider looking at the IsItemDeactivatedAfterEdit() function.
    lua_setfieldstringint(L, "INPUTFLAGS_CALLBACKCOMPLETION", ImGuiInputTextFlags_CallbackCompletion);   // Callback on pressing TAB (for completion handling)
    lua_setfieldstringint(L, "INPUTFLAGS_CALLBACKHISTORY", ImGuiInputTextFlags_CallbackHistory);   // Callback on pressing Up/Down arrows (for history handling)
    lua_setfieldstringint(L, "INPUTFLAGS_CALLBACKALWAYS", ImGuiInputTextFlags_CallbackAlways);   // Callback on each iteration. User code may query cursor position, modify text buffer.
    lua_setfieldstringint(L, "INPUTFLAGS_CALLBACKCHARFILTER", ImGuiInputTextFlags_CallbackCharFilter);   // Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
    lua_setfieldstringint(L, "INPUTFLAGS_ALLOWTABINPUT", ImGuiInputTextFlags_AllowTabInput);  // Pressing TAB input a '\t' character into the text field
    lua_setfieldstringint(L, "INPUTFLAGS_CTRLENTERFORNEWLINE", ImGuiInputTextFlags_CtrlEnterForNewLine);  // In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
    lua_setfieldstringint(L, "INPUTFLAGS_NOHORIZONTALSCROLL", ImGuiInputTextFlags_NoHorizontalScroll);  // Disable following the cursor horizontally
    lua_setfieldstringint(L, "INPUTFLAGS_ALWAYSINSERTMODE", ImGuiInputTextFlags_AlwaysInsertMode);  // Insert mode
    lua_setfieldstringint(L, "INPUTFLAGS_READONLY", ImGuiInputTextFlags_ReadOnly);  // Read-only mode
    lua_setfieldstringint(L, "INPUTFLAGS_PASSWORD", ImGuiInputTextFlags_Password);  // Password mode, display all characters as '*'
    lua_setfieldstringint(L, "INPUTFLAGS_NOUNDOREDO", ImGuiInputTextFlags_NoUndoRedo);  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
    lua_setfieldstringint(L, "INPUTFLAGS_CHARSSCIENTIFIC", ImGuiInputTextFlags_CharsScientific);  // Allow 0123456789.+-*/eE (Scientific notation input)
    lua_setfieldstringint(L, "INPUTFLAGS_CALLBACKRESIZE", ImGuiInputTextFlags_CallbackResize);  // Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/imgui_stdlib.h for an example of using this)
    lua_setfieldstringint(L, "INPUTFLAGS_CALLBACKEDIT", ImGuiInputTextFlags_CallbackEdit);  // Callback on any edit (note that InputText() already returns true on edit, the callback is useful mainly to manipulate the underlying buffer while focus is active)


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
    // This is actually more complex than this,
    // but that value is buried deep in the private internals of dmGraphics_OpenGL
    #ifdef DM_RELEASE
    g_VerifyGraphicsCalls = false;
    #else
    g_VerifyGraphicsCalls = true;
    #endif

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
