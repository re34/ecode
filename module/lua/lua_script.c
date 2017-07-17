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
    int m = lua_tonumber(L, 1);
    print_log("%d %d %d\n", n, isnum, m);
    lua_pushnumber(L, m);
    //lua_pushtring(L, "lua demo string\n");
    
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
    
    luaL_dostring(L, "lua_demo()");
    
    lua_close(L);
}


