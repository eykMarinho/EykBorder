/*
    EykBorder - High Performance Zone Borders & Detection
    Author: Eyk Marinho
    Date: 2026-02-08

    Functions:
    - Border_Create(minx, miny, maxx, maxy, size)
    - Border_Destroy(border_id)
    - Border_Show(playerid, border_id, color)
    - Border_Hide(playerid, border_id)
    - Border_ShowForAll(border_id, color)
    - Border_HideForAll(border_id)
    - IsPlayerInBorder(playerid, border_id)
    
    Callbacks:
    - OnPlayerEnterEykBorder(playerid, border_id)
    - OnPlayerLeaveEykBorder(playerid, border_id)
*/

#if defined _eyk_border_included
    #endinput
#endif
#define _eyk_border_included

#tryinclude <open.mp>
#if !defined _inc_open_mp
    #include <a_samp>
#endif

#if !defined _streamer_included
    #error [EykBorder] A include <streamer> e necessaria. Baixe-a em: github.com/samp-incognito/samp-streamer-plugin
#endif

#if !defined MAX_BORDERS
    #define MAX_BORDERS 100
#endif

#if !defined BORDER_DEFAULT_SIZE
    #define BORDER_DEFAULT_SIZE 9.0
#endif

#define SPAR_ZONE_BORDER_SIZE BORDER_DEFAULT_SIZE

enum E_BORDER_DATA {
    Float:b_MinX,
    Float:b_MinY,
    Float:b_MaxX,
    Float:b_MaxY,
    bool:b_Used
}

static 
    g_BorderZones[MAX_BORDERS][4],
    g_BorderData[MAX_BORDERS][E_BORDER_DATA],
    g_PlayerInBorder[MAX_PLAYERS][MAX_BORDERS char],
    g_BorderArea[MAX_BORDERS],
    g_BorderPool[MAX_BORDERS],
    g_BorderPoolCount,
    g_FuncIdx_Enter = -1,
    g_FuncIdx_Leave = -1;

forward OnPlayerEnterEykBorder(playerid, border_id);
forward OnPlayerLeaveEykBorder(playerid, border_id);

public OnGameModeInit() {
    g_FuncIdx_Enter = funcidx("OnPlayerEnterEykBorder");
    g_FuncIdx_Leave = funcidx("OnPlayerLeaveEykBorder");

    g_BorderPoolCount = 0;
    for(new i = MAX_BORDERS - 1; i >= 0; i--) {
        g_BorderData[i][b_Used] = false;
        g_BorderPool[g_BorderPoolCount++] = i;
        
        for(new j = 0; j < 4; j++) g_BorderZones[i][j] = -1;
        g_BorderArea[i] = -1;
    }

    #if defined EykBorder_OnGameModeInit
        return EykBorder_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit EykBorder_OnGameModeInit
#if defined EykBorder_OnGameModeInit
    forward EykBorder_OnGameModeInit();
#endif

public OnGameModeExit() {
    #if defined EykBorder_OnGameModeExit
        return EykBorder_OnGameModeExit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeExit
    #undef OnGameModeExit
#else
    #define _ALS_OnGameModeExit
#endif
#define OnGameModeExit EykBorder_OnGameModeExit
#if defined EykBorder_OnGameModeExit
    forward EykBorder_OnGameModeExit();
#endif

public OnPlayerConnect(playerid) {
    for(new i = 0; i < MAX_BORDERS; i++) {
        g_PlayerInBorder[playerid]{i} = 0;
    }
    #if defined EykBorder_OnPlayerConnect
        return EykBorder_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect EykBorder_OnPlayerConnect
#if defined EykBorder_OnPlayerConnect
    forward EykBorder_OnPlayerConnect(playerid);
#endif

public OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA:areaid) {
    new bid = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID);
    if(bid >= 0 && bid < MAX_BORDERS && g_BorderArea[bid] == _:areaid) {
        g_PlayerInBorder[playerid]{bid} = 1;
        if(g_FuncIdx_Enter != -1) CallLocalFunction("OnPlayerEnterEykBorder", "ii", playerid, bid);
    }
    #if defined Eyk_OnPlayerEnterDynamicArea
        return Eyk_OnPlayerEnterDynamicArea(playerid, areaid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerEnterDynamicArea
    #undef OnPlayerEnterDynamicArea
#else
    #define _ALS_OnPlayerEnterDynamicArea
#endif
#define OnPlayerEnterDynamicArea Eyk_OnPlayerEnterDynamicArea
#if defined Eyk_OnPlayerEnterDynamicArea
    forward Eyk_OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA:areaid);
#endif

public OnPlayerLeaveDynamicArea(playerid, STREAMER_TAG_AREA:areaid) {
    new bid = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID);
    if(bid >= 0 && bid < MAX_BORDERS && g_BorderArea[bid] == _:areaid) {
        g_PlayerInBorder[playerid]{bid} = 0;
        if(g_FuncIdx_Leave != -1) CallLocalFunction("OnPlayerLeaveEykBorder", "ii", playerid, bid);
    }
    #if defined Eyk_OnPlayerLeaveDynamicArea
        return Eyk_OnPlayerLeaveDynamicArea(playerid, areaid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerLeaveDynamicArea
    #undef OnPlayerLeaveDynamicArea
#else
    #define _ALS_OnPlayerLeaveDynamicArea
#endif
#define OnPlayerLeaveDynamicArea Eyk_OnPlayerLeaveDynamicArea
#if defined Eyk_OnPlayerLeaveDynamicArea
    forward Eyk_OnPlayerLeaveDynamicArea(playerid, STREAMER_TAG_AREA:areaid);
#endif

stock Border_Create(Float:minx, Float:miny, Float:maxx, Float:maxy, Float:size = BORDER_DEFAULT_SIZE) {
    if(g_BorderPoolCount <= 0) {
        printf("[EykBorder] Error: MAX_BORDERS limit reached (%d)", MAX_BORDERS);
        return -1;
    }
    
    new id = g_BorderPool[--g_BorderPoolCount];

    g_BorderData[id][b_MinX] = minx;
    g_BorderData[id][b_MinY] = miny;
    g_BorderData[id][b_MaxX] = maxx;
    g_BorderData[id][b_MaxY] = maxy;
    g_BorderData[id][b_Used] = true;
    g_BorderArea[id] = _:CreateDynamicRectangle(minx, miny, maxx, maxy);
    Streamer_SetIntData(STREAMER_TYPE_AREA, STREAMER_TAG_AREA:g_BorderArea[id], E_STREAMER_EXTRA_ID, id);

    g_BorderZones[id][0] = GangZoneCreate(minx, miny, minx + size, maxy);
    
    g_BorderZones[id][1] = GangZoneCreate(maxx - size, miny, maxx, maxy);
    
    g_BorderZones[id][2] = GangZoneCreate(minx, miny, maxx, miny + size);
    
    g_BorderZones[id][3] = GangZoneCreate(minx, maxy - size, maxx, maxy);

    return id;
}

stock CreateMapBorders(Float:minx, Float:miny, Float:maxx, Float:maxy) {
    return Border_Create(minx, miny, maxx, maxy, SPAR_ZONE_BORDER_SIZE);
}

stock Border_Destroy(border_id) {
    if(border_id < 0 || border_id >= MAX_BORDERS || !g_BorderData[border_id][b_Used]) return 0;
    
    for(new i = 0; i < 4; i++) {
        if(g_BorderZones[border_id][i] != -1) {
            GangZoneDestroy(g_BorderZones[border_id][i]);
            g_BorderZones[border_id][i] = -1;
        }
    }
    if(g_BorderArea[border_id] != -1) {
        DestroyDynamicArea(STREAMER_TAG_AREA:g_BorderArea[border_id]);
        g_BorderArea[border_id] = -1;
    }
    g_BorderData[border_id][b_Used] = false;
    
    g_BorderPool[g_BorderPoolCount++] = border_id;
    
    for(new i = 0; i < MAX_PLAYERS; i++) {
        g_PlayerInBorder[i]{border_id} = 0;
    }
    return 1;
}

stock Border_Show(playerid, border_id, color) {
    if(border_id < 0 || border_id >= MAX_BORDERS || !g_BorderData[border_id][b_Used]) return 0;
    
    GangZoneShowForPlayer(playerid, g_BorderZones[border_id][0], color);
    GangZoneShowForPlayer(playerid, g_BorderZones[border_id][1], color);
    GangZoneShowForPlayer(playerid, g_BorderZones[border_id][2], color);
    GangZoneShowForPlayer(playerid, g_BorderZones[border_id][3], color);
    return 1;
}

stock Border_Hide(playerid, border_id) {
    if(border_id < 0 || border_id >= MAX_BORDERS || !g_BorderData[border_id][b_Used]) return 0;
    
    GangZoneHideForPlayer(playerid, g_BorderZones[border_id][0]);
    GangZoneHideForPlayer(playerid, g_BorderZones[border_id][1]);
    GangZoneHideForPlayer(playerid, g_BorderZones[border_id][2]);
    GangZoneHideForPlayer(playerid, g_BorderZones[border_id][3]);
    return 1;
}

stock Border_ShowForAll(border_id, color) {
    if(border_id < 0 || border_id >= MAX_BORDERS || !g_BorderData[border_id][b_Used]) return 0;
    
    GangZoneShowForAll(g_BorderZones[border_id][0], color);
    GangZoneShowForAll(g_BorderZones[border_id][1], color);
    GangZoneShowForAll(g_BorderZones[border_id][2], color);
    GangZoneShowForAll(g_BorderZones[border_id][3], color);
    return 1;
}

stock Border_HideForAll(border_id) {
    if(border_id < 0 || border_id >= MAX_BORDERS || !g_BorderData[border_id][b_Used]) return 0;
    
    GangZoneHideForAll(g_BorderZones[border_id][0]);
    GangZoneHideForAll(g_BorderZones[border_id][1]);
    GangZoneHideForAll(g_BorderZones[border_id][2]);
    GangZoneHideForAll(g_BorderZones[border_id][3]);
    return 1;
}

stock IsPlayerInBorder(playerid, border_id) {
    if(border_id < 0 || border_id >= MAX_BORDERS || !g_BorderData[border_id][b_Used]) return 0;
    return g_PlayerInBorder[playerid]{border_id};
}
