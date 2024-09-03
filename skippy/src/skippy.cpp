// skippy.cpp
// Extension lib defines
#define LIB_NAME "Skippy"
#define MODULE_NAME "skippy"

#include "skippy_private.h"
// include the Defold SDK
#include <dmsdk/sdk.h>

static int Start(lua_State *L)
{
    Skippy_PlatformStart();

    return 0;
}

static int Stop(lua_State *L)
{
    Skippy_PlatformStop();

    return 0;
}

static int GetSteps(lua_State *L)
{
    lua_pushnumber(L, Skippy_GetSteps());

    return 1;
}

static int GetStepDetector(lua_State *L)
{
    lua_pushnumber(L, Skippy_GetStepDetector());

    return 1;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
    {
        {"start", Start},
        {"stop", Stop},
        {"get_steps", GetSteps},
        {"get_steps_detector", GetStepDetector},
        {0, 0}};

static void LuaInit(lua_State *L)
{
    int top = lua_gettop(L);
    luaL_register(L, MODULE_NAME, Module_methods);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeSkippy(dmExtension::AppParams *params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeSkippy(dmExtension::Params *params)
{
#if defined(DM_PLATFORM_ANDROID)
    LuaInit(params->m_L);
    printf("Registered %s Extension\n", MODULE_NAME);
#else
    printf("Extension %s is not supported\n", MODULE_NAME);
#endif
    return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeSkippy(dmExtension::AppParams *params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeSkippy(dmExtension::Params *params)
{
    Stop(params->m_L);

    return dmExtension::RESULT_OK;
}

dmExtension::Result UpdateSkippy(dmExtension::Params *params)
{
    return dmExtension::RESULT_OK;
}

// Defold SDK uses a macro for setting up extension entry points:
//
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)

// Skippy is the C++ symbol that holds all relevant extension data.
// It must match the name field in the `ext.manifest`
DM_DECLARE_EXTENSION(Skippy, LIB_NAME, AppInitializeSkippy, AppFinalizeSkippy, InitializeSkippy, UpdateSkippy, 0, FinalizeSkippy)
