#include "lua_script.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "types.h"
#include "print_log.h"

static int lua_demo(lua_State *L)
{
    int n = lua_gettop(L);
    int isnum = lua_isnumber(L, 1);
    float m = lua_tonumber(L, 1);
    print_log("%d %d %f\n", n, isnum, m);
    lua_pushnumber(L, m);
    //lua_pushstring(L, "lua demo string\n");
    print_log("lua demo end!\r\n");
    
    return 2;
}


static const struct luaL_Reg lua_lib[]={
    {"demo", lua_demo},
    {NULL, NULL},
};

void lua_script_init(void)
{
    lua_State *L;

    L = luaL_newstate();
    luaopen_base(L);
    
    luaL_setfuncs(L, lua_lib, 0);
    
    luaL_dostring(L, "demo()");
    
    lua_close(L);
}


