#include <stdlib.h>
#include <stdint.h>

#include "lua.h"
#include "lauxlib.h"

#include "bloom.h"

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#elif
#define DLLEXPORT
#endif /* _WIN32 */

static int
lbloom_add(lua_State *L)
{
    if (lua_gettop(L) != 2)
    {
        return luaL_error(L, "must have data to add");
    }
    struct bloom *b = (struct bloom *) luaL_checkudata(L, 1, "bloom.Bloom");
    size_t size;
    void *buf = (void *) luaL_checklstring(L, 2, &size);
    int ret = bloom_add(b, buf, (int) size);
    lua_pushinteger(L, (lua_Integer) ret);
    return 1;
}

static int
lbloom_check(lua_State *L)
{
    if (lua_gettop(L) != 2)
    {
        return luaL_error(L, "must have data to check");
    }
    struct bloom *b = (struct bloom *) luaL_checkudata(L, 1, "bloom.Bloom");
    size_t size;
    void *buf = (void *) luaL_checklstring(L, 2, &size);
    int ret = bloom_check(b, buf, (int) size);
    lua_pushinteger(L, (lua_Integer) ret);
    return 1;
}

static int
lbloom_print(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "must have bloom to print");
    }
    struct bloom *b = (struct bloom *) luaL_checkudata(L, 1, "bloom.Bloom");
    bloom_print(b);
    return 0;
}

static int
lbloom_reset(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "must have bloom to reset");
    }
    struct bloom *b = (struct bloom *) luaL_checkudata(L, 1, "bloom.Bloom");
    int ret = bloom_reset(b);
    lua_pushinteger(L, (lua_Integer) ret);
    return 1;
}

static int
lbloom_free(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "must have bloom to free");
    }
    struct bloom *b = (struct bloom *) luaL_checkudata(L, 1, "bloom.Bloom");
    bloom_free(b);
    return 0;
}

static luaL_Reg lua_bloom_methods[] = {
        {"add",   lbloom_add},
        {"check", lbloom_check},
        {"print", lbloom_print},
        {"reset", lbloom_reset},
        {"__gc",  lbloom_free},
        {NULL, NULL}
};

static int
lbloom_new(lua_State *L)
{
    if (lua_gettop(L) != 2)
    {
        return luaL_error(L, "must have entries and error");
    }
    int entries = (int) luaL_checkinteger(L, 1);
    double error = luaL_checknumber(L, 2);

    struct bloom *b = (struct bloom *) lua_newuserdata(L, sizeof(struct bloom));
    bloom_init(b, entries, error);
    luaL_setmetatable(L, "bloom.Bloom");
    return 1;
}

static int
lbloom_version(lua_State *L)
{
    if (lua_gettop(L) != 0)
    {
        return luaL_error(L, "need no param");
    }
    lua_pushstring(L, bloom_version());
    return 1;
}

static luaL_Reg lua_funcs[] = {
        {"new", lbloom_new},
        {"version", lbloom_version},
        {NULL, NULL}
};


DLLEXPORT int luaopen_bloom(lua_State *L)
{
    if (!luaL_newmetatable(L, "bloom.Bloom"))
    {
        return luaL_error(L, "bloom.Bloom already in register");
    }
    lua_pushvalue(L, -1); // mt mt
    lua_setfield(L, -2, "__index"); // mt
    luaL_setfuncs(L, lua_bloom_methods, 0); // mt

    luaL_newlib(L, lua_funcs);
    return 1;
}