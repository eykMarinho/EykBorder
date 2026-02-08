
#include <Streamer>
#include <EykBorder>

main() {
    print("EykBorder Test Loaded");
}

new g_TestBorder1, g_TestBorder2, g_TestBorder3;

public OnGameModeInit() {

    // Create some test borders (Los Santos areas)
    // Glen Park
    g_TestBorder1 = Border_Create(1872.0, -1250.0, 2050.0, -1100.0);
    print("Created Border 1 (Glen Park) - Red");

    // Pershing Square
    g_TestBorder2 = Border_Create(1420.0, -1730.0, 1600.0, -1570.0);
    print("Created Border 2 (Pershing Square) - Green");
    
    // Area 51 (Restricted)
    g_TestBorder3 = Border_Create(96.0, 1775.0, 420.0, 2080.0);
    print("Created Border 3 (Area 51) - Blue");

    return 1;
}

public OnPlayerSpawn(playerid) {
    Border_Show(playerid, g_TestBorder1, 0xFF0000AA); // Red border
    Border_Show(playerid, g_TestBorder2, 0x00FF00AA); // Green border
    Border_Show(playerid, g_TestBorder3, 0x0000FFAA); // Blue border
    return 1;
}

public OnPlayerEnterEykBorder(playerid, border_id) {
    new msg[128];
    format(msg, sizeof(msg), "Você entrou na borda %d!", border_id);
    SendClientMessage(playerid, -1, msg);
    printf("Player %d entered border %d", playerid, border_id);
    return 1;
}

public OnPlayerLeaveEykBorder(playerid, border_id) {
    new msg[128];
    format(msg, sizeof(msg), "Você saiu da borda %d!", border_id);
    SendClientMessage(playerid, -1, msg);
    printf("Player %d left border %d", playerid, border_id);
    return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ) {
    SetPlayerPos(playerid, fX, fY, fZ);
    return 1;
}
