#include "../seal.h"

EXTERN_GAME;

int lscheduler_new(lua_State* L)
{
    struct scheduler* scheduler = scheduler_new();
    lua_pushlightuserdata(L, scheduler);
    return 1;
}

int lscheduler_free(lua_State* L)
{
    struct scheduler* self = (struct scheduler*)lua_touserdata(L, 1);
    scheduler_free(self);
    return 1;
}

int lscheduler_schedule(lua_State* L)
{
    struct scheduler* self = (struct scheduler*)lua_touserdata(L, 1);
    struct sprite* sprite = (struct sprite*)lua_touserdata(L, 2);
    struct action* action = (struct action*)lua_touserdata(L, 3);
    scheduler_schedule(self, sprite, action);
    return 1;
}

int lscheduler_stop_target(lua_State* L)
{
    struct scheduler* self = (struct scheduler*)lua_touserdata(L, 1);
    struct sprite* sprite = (struct sprite*)lua_touserdata(L, 2);
    scheduler_stop_target(self, sprite);
    return 1;
}

int lscheduler_update(lua_State* L)
{
    struct scheduler* self = (struct scheduler*)lua_touserdata(L, 1);
    lua_Number dt = luaL_checknumber(L, 2);
    scheduler_update(self, dt);
    return 1;
}

int luaopen_seal_scheduler(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif
    luaL_Reg lib[] = {
        { "new", lscheduler_new },
        { "free", lscheduler_free },
        { "schedule", lscheduler_schedule },
        { "stop_target", lscheduler_stop_target },
        { "update", lscheduler_update },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

    return 1;
}