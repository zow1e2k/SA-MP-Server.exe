//Библиотеки
#include        <a_samp>
#include        <foreach>
#include        <a_mysql>
#include        <streamer>
#include        <dc_cmd>
#include        <sscanf2>
#include        <nex-ac>
//MySQL подключение
#define         Host                            "localhost"
#define         User                            "root"
#define         DB                              "faterp"
#define         Password                        ""
//Диалоги
#define         DIALOG_REGISTER                 1500
#define         DIALOG_LOGIN                    1501
#define         DIALOG_SEX                      1502
#define         DIALOG_AGE                      1503
#define         DIALOG_EMAIL                    1504
#define         DIALOG_FORUM                    1505
#define         DIALOG_CREDITS                  1506
#define         DIALOG_MENU                     1507
#define         DIALOG_MENU_INVENTORY           1508
#define         DIALOG_FIRSTJOB_HIRE            1509
#define         DIALOG_FIRSTJOB_FIRE            1510
#define         DIALOG_PLAYER_INFO              1511
//Функции
#define         SCM                             SendClientMessage
#define         SPD                             ShowPlayerDialog
#define         SAM                             sendAdminMessage
#define         SGM                             sendGovernmentMessage
#define         SBM                             sendBanditMessage
#define         SSM                             sendStalkerMessage
#define         SMM                             sendMonolitMessage
//Цвета
#define         COLOR_SYSTEM                    0xD5D5D5AA
#define         COLOR_YELLOW                    0xFFCC00AA
#define         COLOR_GREEN                     0x32CD32AA
#define         COLOR_BLACK                     0x000000AA
#define         COLOR_RED                       0xFF0000AA
//Итераторы
new Iterator:admins<MAX_PLAYERS>;
new Iterator:governments<MAX_PLAYERS>;
new Iterator:bandits<MAX_PLAYERS>;
new Iterator:stalkers<MAX_PLAYERS>;
new Iterator:monolites<MAX_PLAYERS>;
//Классы
enum Info
{
    id,
    password[129],
    mail[129],
    sex,
    age,
    money,
    skin,
    kills,
    deaths,
    adminLevel,
    wrongPass,
    centsLevel,
    bool:logged,
    nickName[20],
    member,
    rank,
    warnLevel,
    clan[20],
    donateLevel,
    vipLevel,
    hoursLevel,
    accessLevel,
    expLevel,
    radiation,
    hunger,
    thirst,
    virus,
    Float:health,
    ban,
    mute,
    jail
};
enum Inventory
{
    id,
    antiRadin,
    alcohol,
    weed,
    food,
    medcine,
    zombiesBlood,
    instruments,
    canister,
    masterKey,
    bolt,
    specialAmmo,
    metal,
    wood,
    blanks,
    drugs,
    flesh,
    fruits,
    pumpkin,
    croup,
    milk,
    egg,
    artefacts
};
//Декларирование классов
new PlayerInfo[MAX_PLAYERS][Info];
new PlayerInventory[MAX_PLAYERS][Inventory];
//Колбэки
forward loadInventory(playerid);
forward saveAccount(playerid);
forward loadAccount(playerid);
forward registerInventory(playerid);
forward registerAccount(playerid);
forward timerKick(playerid);
forward checkAccount(playerid);
forward IsPlayerInArea();
forward loadNewsTD(playerid);
forward OnCheatDetected(playerid, ip_address[], type, code);
//Глобальные переменные
new MySQL:connection;
new Text:logo[1];
new pass[129];
new Text:tSkin[3];
new cskin[MAX_PLAYERS];
new skinMale[3][0] = {{26},{25},{35}};
new skinFemale[3][0] = {{12},{55},{69}};
new Text:background[7];
new Text:registration[9];
new Text:login[MAX_PLAYERS][7];
new Text:mainText[MAX_PLAYERS][8];
new vehID[MAX_PLAYERS];
new Text:cents[MAX_PLAYERS][1];
new passWrote[MAX_PLAYERS], sexWrote[MAX_PLAYERS], ageWrote[MAX_PLAYERS], mailWrote[MAX_PLAYERS];
new greenZone;
new redZone[8];
new yellowZone[2];
new firstJobPick;
new bool: killed[MAX_PLAYERS];
new bool: working[MAX_PLAYERS];
new firstJobCP[MAX_PLAYERS][10];
new Float: firstJobPoints[10][3] = 
{ 
{2793.0930,-2425.7480,13.6324},
{2798.6563,-2410.7419,13.6310},
{2793.0925,-2411.1208,13.6324},
{2784.9209,-2424.5891,13.6344},
{2784.9185,-2410.6975,13.6344},
{2798.6543,-2449.4849,13.6310},
{2793.0930,-2464.4519,13.6324},
{2793.0955,-2449.2766,13.6324},
{2784.9214,-2449.2573,13.6344},
{2784.9187,-2464.3848,13.6344} 
};
main()
{
    return true;
}
public OnGameModeInit()
{
    SendRconCommand("ackslimit 5000");
    DisableInteriorEnterExits();
    connection = mysql_connect(Host, User, Password, DB);
    mysql_tquery(connection, !"SET CHARACTER SET 'utf8'", "", "");
    mysql_tquery(connection, !"SET NAMES 'utf8'", "", "");
    mysql_tquery(connection, !"SET character_set_client = 'cp1251'", "", "");
    mysql_tquery(connection, !"SET character_set_connection = 'cp1251'", "", "");
    mysql_tquery(connection, !"SET character_set_results = 'cp1251'", "", "");
    mysql_tquery(connection, !"SET SESSION collation_connection = 'utf8_general_ci'", "", "");
    if(mysql_errno(connection) != 0) print("[ERROR] Failed MySQL connection");
    loadTextDraw();
    //
    tSkin[0] = TextDrawCreate(250.000000, 300.000000, "LD_BEAT:left");
    TextDrawBackgroundColor(tSkin[0], 255);
    TextDrawFont(tSkin[0], 4);
    TextDrawLetterSize(tSkin[0], 0.500000, 1.000000);
    TextDrawColor(tSkin[0], -1);
    TextDrawSetOutline(tSkin[0], 0);
    TextDrawSetProportional(tSkin[0], 1);
    TextDrawSetShadow(tSkin[0], 1);
    TextDrawUseBox(tSkin[0], 1);
    TextDrawBoxColor(tSkin[0], 255);
    TextDrawTextSize(tSkin[0], 34.000000, 28.000000);
    TextDrawSetSelectable(tSkin[0], true);

    tSkin[1] = TextDrawCreate(360.000000, 300.000000, "LD_BEAT:right");
    TextDrawBackgroundColor(tSkin[1], 255);
    TextDrawFont(tSkin[1], 4);
    TextDrawLetterSize(tSkin[1], 0.500000, 1.000000);
    TextDrawColor(tSkin[1], -1);
    TextDrawSetOutline(tSkin[1], 0);
    TextDrawSetProportional(tSkin[1], 1);
    TextDrawSetShadow(tSkin[1], 1);
    TextDrawUseBox(tSkin[1], 1);
    TextDrawBoxColor(tSkin[1], 255);
    TextDrawTextSize(tSkin[1], 34.000000, 28.000000);
    TextDrawSetSelectable(tSkin[1], true);

    tSkin[2] = TextDrawCreate(318.000000, 370.000000, "CONFIRM");
    TextDrawAlignment(tSkin[2], 2);
    TextDrawBackgroundColor(tSkin[2], 255);
    TextDrawFont(tSkin[2], 1);
    TextDrawLetterSize(tSkin[2], 0.679999, 1.900000);
    TextDrawColor(tSkin[2], -1);
    TextDrawSetOutline(tSkin[2], 0);
    TextDrawSetProportional(tSkin[2], 1);
    TextDrawSetShadow(tSkin[2], 1);
    TextDrawUseBox(tSkin[2], 1);
    TextDrawBoxColor(tSkin[2], 0);
    TextDrawTextSize(tSkin[2], 15.000000, 68.000000);
    TextDrawSetSelectable(tSkin[2], true);
    
    //Зоны
    SetTimer("IsPlayerInArea",60000, 1);
    greenZone = GangZoneCreate(861.328125,-2853.515625,2917.96875,-1916.015625);
    redZone[0] = GangZoneCreate(125,-1916.015625,2929.6875,-415.0625);
    redZone[1] = GangZoneCreate(-2433.59375,-2617.1875,-1910.15625,-2167.96875);
    redZone[2] = GangZoneCreate(-2982.421875,-1365.234375,-1904.296875,1505.859375);
    redZone[3] = GangZoneCreate(-1904.296875,-650.390625,-1517.578125,1505.859375);
    redZone[4] = GangZoneCreate(74.21875,-300.78125,433.59375,82.03125);
    redZone[5] = GangZoneCreate(1132.8125,101.5625,1433.59375,453.125);
    redZone[6] =  GangZoneCreate(2156.25,-164.0625,2593.75,164.0625);
    redZone[7] = GangZoneCreate(925.78125,550.78125,2929.6875,2923.828125);
    yellowZone[0] = GangZoneCreate(-1517.578125,550.78125,925.78125,2923.828125);
    yellowZone[1] = GangZoneCreate(-2982.421875,1505.859375,-1505.859375,2923.828125);
    //Объекты
    //Пикапы
    firstJobPick = CreatePickup(1239,2,2828.8342,-2447.3989,12.0917);
    return true;
}

public OnGameModeExit()
{
    TextDrawDestroy(tSkin[0]);
    TextDrawDestroy(tSkin[1]);
    TextDrawDestroy(tSkin[2]);
    foreach(Player, i) saveAccount(i);
    mysql_close(connection);
    return true;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(newkeys == KEY_YES)
    {
        if (GetPVarInt(playerid,"accept"))
        {
            PlayerInfo[playerid][member] = 3;
            PlayerInfo[playerid][rank] = 1;
            getRankSkin(playerid);
            SCM(playerid,COLOR_SYSTEM,"Вы присоединились к Вооруженным силам");
            SetPVarInt(playerid,"accept",0);
            return true;
        }
        SPD(playerid,DIALOG_MENU,DIALOG_STYLE_LIST,"{FFCC00}Рюкзак","{FFFFFF}Содержимое\nДосье\nНавыки\nЗаписки\nНавигатор\n\nИспользовать аптечку\nИспользовать антирадин\n\
            Выпить алкоголь\nПоесть\nПокурить\nШирнуться\nИспользовать рем. набор\nИспользовать канистру\n\nПритвориться мирным\n\
            Активировать артефакт\nСпец. пт\nСтиль боя\nПангит","Выбрать","Выйти");
    }
    if (newkeys == KEY_NO)
    {
        if (GetPVarInt(playerid,"accept"))
        {
            SCM(playerid,COLOR_SYSTEM,"Отказались присоединиться к Вооруженным силам");
            SetPVarInt(playerid,"accept",0);
            return true;
        } 
    }
    if ((newkeys == KEY_SECONDARY_ATTACK || newkeys == KEY_JUMP || newkeys == KEY_SECONDARY_ATTACK 
    || newkeys == KEY_FIRE || newkeys == KEY_CROUCH) && (working[playerid])) 
    { 
        RemovePlayerAttachedObject(playerid,5); 
        ApplyAnimation(playerid,"PED","IDLE_tired",4.1,0,1,1,0,1);
        working[playerid] = false;
        for (new k=0;k<10;k++) DestroyDynamicCP(firstJobCP[playerid][k]);
        SCM(playerid,COLOR_SYSTEM,"Вы уронили ящик и были оштрафованы на $.. Оплатите и начните заново");       
    }
    return true;
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
    if(success == -1)
    {
        return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    }
    return true;
}

public OnPlayerRequestClass(playerid, classid)
{
    //InterpolateCameraPos(playerid, -2397.553710, 1436.038452, 140.798019, -2876.055175, 1382.189697, 159.150161, 17000);
    //InterpolateCameraLookAt(playerid, -2401.776855, 1438.431640, 139.599197, -2872.188964, 1384.818359, 157.377395, 17000);
    //SetSpawnInfo(playerid, 1, PlayerInfo[playerid][skin], 613.1469,-1415.0784,13.5688, 269.15, 0, 0, 0, 0, 0, 0);
    //SpawnPlayer(playerid);
    TogglePlayerSpectating(playerid, true);
    SetSpawnInfo(playerid, 1, PlayerInfo[playerid][skin], 613.1469,-1415.0784,13.5688, 269.15, 0, 0, 0, 0, 0, 0);
    OnPlayerSpawn(playerid);
    return true;
}

public OnPlayerConnect(playerid)
{
    killed[playerid] = false;
    working[playerid] = false;
    RemoveBuildingForPlayer(playerid, 3688, 2450.8750, -2680.4531, 17.7891, 0.25);
	RemoveBuildingForPlayer(playerid, 3688, 2192.8984, -2580.8672, 17.7891, 0.25);
	RemoveBuildingForPlayer(playerid, 3686, 2288.2656, -2342.0703, 15.5625, 0.25);
	RemoveBuildingForPlayer(playerid, 3709, 2660.4766, -2429.2969, 17.0703, 0.25);
	RemoveBuildingForPlayer(playerid, 3709, 2639.5469, -2429.2969, 17.0703, 0.25);
	RemoveBuildingForPlayer(playerid, 3744, 2771.0703, -2372.4453, 15.2188, 0.25);
	RemoveBuildingForPlayer(playerid, 3744, 2789.2109, -2377.6250, 15.2188, 0.25);
	RemoveBuildingForPlayer(playerid, 3744, 2774.7969, -2386.8516, 15.2188, 0.25);
	RemoveBuildingForPlayer(playerid, 3744, 2423.5313, -2260.8438, 14.9609, 0.25);
	RemoveBuildingForPlayer(playerid, 3744, 2359.4766, -2325.1484, 15.0234, 0.25);
	RemoveBuildingForPlayer(playerid, 3744, 2437.2109, -2490.0938, 15.2188, 0.25);
	RemoveBuildingForPlayer(playerid, 3744, 2259.3594, -2604.1797, 9.8750, 0.25);
	RemoveBuildingForPlayer(playerid, 3744, 2262.8359, -2638.7734, 14.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 3744, 2205.1563, -2582.7891, 14.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 3744, 2551.5313, -2472.6953, 15.2188, 0.25);
	RemoveBuildingForPlayer(playerid, 3746, 2814.2656, -2356.5703, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3746, 2814.2656, -2521.4922, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3746, 2568.4453, -2483.3906, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3746, 2563.1563, -2563.5781, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3746, 2531.7031, -2629.2266, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3746, 2519.8047, -2701.8750, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3746, 2381.1016, -2701.8750, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3746, 2283.5625, -2601.2734, 20.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1396, 2406.2813, -2265.7891, 36.4922, 0.25);
	RemoveBuildingForPlayer(playerid, 1396, 2232.4375, -2458.5781, 36.1953, 0.25);
	RemoveBuildingForPlayer(playerid, 5325, 2488.9922, -2509.2578, 18.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3746, 2422.7031, -2411.9219, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3746, 2472.4453, -2362.9375, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3770, 2795.8281, -2394.2422, 14.1719, 0.25);
	RemoveBuildingForPlayer(playerid, 3770, 2507.3672, -2640.0703, 14.0781, 0.25);
	RemoveBuildingForPlayer(playerid, 3769, 2464.1250, -2571.6328, 15.1641, 0.25);
	RemoveBuildingForPlayer(playerid, 3620, 2381.1016, -2701.8750, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2239.3672, -2676.2891, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2257.4297, -2667.6172, 16.3438, 0.25);
	RemoveBuildingForPlayer(playerid, 3574, 2262.8359, -2638.7734, 14.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2210.7031, -2637.5938, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 3574, 2259.3594, -2604.1797, 9.8750, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2239.3672, -2598.8984, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 5324, 2240.9375, -2590.6797, 18.4766, 0.25);
	RemoveBuildingForPlayer(playerid, 3620, 2283.5625, -2601.2734, 20.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 3574, 2205.1563, -2582.7891, 14.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2229.4375, -2581.4141, 16.3203, 0.25);
	RemoveBuildingForPlayer(playerid, 3621, 2192.8984, -2580.8672, 17.7891, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2210.7031, -2565.1094, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 3578, 2259.3438, -2564.4297, 7.6484, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2220.8984, -2553.8984, 16.3203, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2239.3672, -2521.5078, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2229.4375, -2510.2344, 16.3203, 0.25);
	RemoveBuildingForPlayer(playerid, 1315, 2228.0859, -2506.7734, 15.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 1315, 2213.9297, -2498.3984, 15.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2210.7031, -2482.8125, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 1315, 2222.1875, -2484.6094, 15.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 1377, 2201.6250, -2458.5781, 38.9844, 0.25);
	RemoveBuildingForPlayer(playerid, 1376, 2227.1016, -2458.5938, 31.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1378, 2232.4375, -2458.5781, 36.1953, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2272.0000, -2410.6484, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2304.5859, -2363.0469, 16.3438, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2321.0625, -2361.5859, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 2323.6563, -2353.5625, 13.7422, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 2327.4141, -2357.2656, 13.7422, 0.25);
	RemoveBuildingForPlayer(playerid, 3627, 2288.2656, -2342.0703, 15.5625, 0.25);
	RemoveBuildingForPlayer(playerid, 1315, 2313.3047, -2333.0938, 15.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 1315, 2317.2109, -2348.6875, 15.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 1315, 2333.1484, -2344.5938, 15.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 2341.4766, -2343.1484, 13.8203, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 2337.7188, -2339.4453, 13.8203, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2343.9531, -2338.6406, 19.1484, 0.25);
	RemoveBuildingForPlayer(playerid, 1315, 2329.0859, -2328.6875, 15.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 5323, 2336.4688, -2322.3984, 21.0469, 0.25);
	RemoveBuildingForPlayer(playerid, 3574, 2359.4766, -2325.1484, 15.0234, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 2282.0703, -2312.0469, 13.7578, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 2285.8281, -2315.7578, 13.7578, 0.25);
	RemoveBuildingForPlayer(playerid, 1315, 2289.7813, -2301.0078, 15.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 2300.0703, -2301.6094, 13.7734, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 2296.3125, -2297.8984, 13.7734, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2347.1797, -2320.7656, 16.3125, 0.25);
	RemoveBuildingForPlayer(playerid, 3578, 2375.0000, -2300.4141, 13.2109, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2389.4375, -2278.1797, 16.3125, 0.25);
	RemoveBuildingForPlayer(playerid, 1635, 2391.9453, -2252.0000, 16.3438, 0.25);
	RemoveBuildingForPlayer(playerid, 1377, 2384.4922, -2244.0000, 39.2813, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2395.2344, -2290.0234, 19.1484, 0.25);
	RemoveBuildingForPlayer(playerid, 1376, 2402.5000, -2262.0234, 31.9766, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2406.7969, -2248.9375, 16.3125, 0.25);
	RemoveBuildingForPlayer(playerid, 1378, 2406.2813, -2265.7891, 36.4922, 0.25);
	RemoveBuildingForPlayer(playerid, 3620, 2519.8047, -2701.8750, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3621, 2450.8750, -2680.4531, 17.7891, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2450.0156, -2632.7734, 16.3594, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2469.6016, -2645.3203, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 3578, 2470.1406, -2628.2656, 13.1719, 0.25);
	RemoveBuildingForPlayer(playerid, 3626, 2507.3672, -2640.0703, 14.0781, 0.25);
	RemoveBuildingForPlayer(playerid, 3620, 2531.7031, -2629.2266, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2450.0156, -2604.9297, 16.3594, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2480.3281, -2588.3281, 16.3594, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2469.6016, -2579.9844, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 3625, 2464.1250, -2571.6328, 15.1641, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2489.3516, -2625.7109, 16.2969, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2498.3438, -2612.6563, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 1413, 2496.5547, -2585.1797, 13.9063, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2489.3516, -2566.2734, 16.2969, 0.25);
	RemoveBuildingForPlayer(playerid, 1413, 2501.8359, -2585.2422, 13.9063, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2450.0156, -2563.2188, 16.3594, 0.25);
	RemoveBuildingForPlayer(playerid, 1413, 2496.5547, -2557.3359, 13.9063, 0.25);
	RemoveBuildingForPlayer(playerid, 1413, 2501.8359, -2557.3984, 13.9063, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2498.3438, -2547.3203, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2450.0156, -2535.5703, 16.3594, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2480.3281, -2536.4375, 16.3594, 0.25);
	RemoveBuildingForPlayer(playerid, 3578, 2470.1406, -2530.5547, 13.1719, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2469.6016, -2514.6484, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 3574, 2437.2109, -2490.0938, 15.2188, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2498.3438, -2481.9766, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2444.3281, -2435.0625, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2461.9141, -2444.3438, 16.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 3578, 2526.4297, -2561.3047, 13.1719, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2533.3906, -2514.1094, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 3574, 2551.5313, -2472.6953, 15.2188, 0.25);
	RemoveBuildingForPlayer(playerid, 3620, 2563.1563, -2563.5781, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3620, 2568.4453, -2483.3906, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3578, 2468.8594, -2413.5234, 13.1719, 0.25);
	RemoveBuildingForPlayer(playerid, 3620, 2422.7031, -2411.9219, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2455.0703, -2399.0156, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2491.7031, -2383.3281, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 3578, 2495.8438, -2386.9375, 13.1719, 0.25);
	RemoveBuildingForPlayer(playerid, 3620, 2472.4453, -2362.9375, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3574, 2423.5313, -2260.8438, 14.9609, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2432.8281, -2252.9531, 19.1484, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2431.1641, -2236.5547, 16.3125, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2476.1484, -2222.4375, 16.2109, 0.25);
	RemoveBuildingForPlayer(playerid, 3578, 2546.0469, -2396.5938, 13.1719, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2535.6875, -2377.6563, 16.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 3578, 2571.1641, -2421.1328, 13.1719, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2624.3281, -2409.5625, 16.4141, 0.25);
	RemoveBuildingForPlayer(playerid, 1278, 2626.2344, -2391.5234, 26.7031, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2742.2656, -2481.5156, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 5326, 2661.5156, -2465.1406, 20.1094, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2669.9063, -2447.2891, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 3623, 2639.5469, -2429.2969, 17.0703, 0.25);
	RemoveBuildingForPlayer(playerid, 3623, 2660.4766, -2429.2969, 17.0703, 0.25);
	RemoveBuildingForPlayer(playerid, 1307, 2629.2109, -2419.6875, 12.2891, 0.25);
	RemoveBuildingForPlayer(playerid, 1307, 2649.8984, -2419.6875, 12.2891, 0.25);
	RemoveBuildingForPlayer(playerid, 1315, 2686.7578, -2416.6250, 15.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 2663.5078, -2409.5625, 16.4141, 0.25);
	RemoveBuildingForPlayer(playerid, 1315, 2672.5938, -2408.2500, 15.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2742.2656, -2416.5234, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 3578, 2639.1953, -2392.8203, 13.1719, 0.25);
	RemoveBuildingForPlayer(playerid, 3578, 2663.8359, -2392.8203, 13.1719, 0.25);
	RemoveBuildingForPlayer(playerid, 1306, 2669.9063, -2394.5078, 19.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 1278, 2708.4063, -2391.5234, 26.7031, 0.25);
	RemoveBuildingForPlayer(playerid, 3578, 2747.0078, -2480.2422, 13.1719, 0.25);
	RemoveBuildingForPlayer(playerid, 3574, 2774.7969, -2386.8516, 15.2188, 0.25);
	RemoveBuildingForPlayer(playerid, 3574, 2771.0703, -2372.4453, 15.2188, 0.25);
	RemoveBuildingForPlayer(playerid, 3574, 2789.2109, -2377.6250, 15.2188, 0.25);
	RemoveBuildingForPlayer(playerid, 3626, 2795.8281, -2394.2422, 14.1719, 0.25);
	RemoveBuildingForPlayer(playerid, 3620, 2814.2656, -2521.4922, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3620, 2814.2656, -2356.5703, 25.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 11618, -688.1172, 939.1797, 11.1250, 0.25);
	RemoveBuildingForPlayer(playerid, 11654, -681.8750, 965.8906, 11.1250, 0.25);
	RemoveBuildingForPlayer(playerid, 11491, -688.1094, 928.1328, 12.6250, 0.25);
	RemoveBuildingForPlayer(playerid, 669, -712.0703, 928.3047, 11.5391, 0.25);
	RemoveBuildingForPlayer(playerid, 11490, -688.1172, 939.1797, 11.1250, 0.25);
	RemoveBuildingForPlayer(playerid, 11631, -691.5938, 942.7188, 13.8750, 0.25);
	RemoveBuildingForPlayer(playerid, 11663, -688.1172, 939.1797, 11.1250, 0.25);
	RemoveBuildingForPlayer(playerid, 11666, -688.1406, 934.8203, 14.3906, 0.25);
	RemoveBuildingForPlayer(playerid, 11664, -685.0938, 941.9141, 13.1406, 0.25);
	RemoveBuildingForPlayer(playerid, 11665, -685.1719, 935.6953, 13.3203, 0.25);
	RemoveBuildingForPlayer(playerid, 700, -700.6563, 984.1406, 11.5547, 0.25);
	RemoveBuildingForPlayer(playerid, 1223, -661.8984, 884.8359, 0.9063, 0.25);
	RemoveBuildingForPlayer(playerid, 1223, -661.3359, 880.4688, 0.8672, 0.25);
	RemoveBuildingForPlayer(playerid, 1223, -660.1563, 891.9688, 3.6484, 0.25);
	RemoveBuildingForPlayer(playerid, 1223, -658.1250, 877.3047, 0.8672, 0.25);
	RemoveBuildingForPlayer(playerid, 1223, -658.7344, 898.8984, 5.8047, 0.25);
	RemoveBuildingForPlayer(playerid, 1223, -657.3828, 907.2578, 8.4375, 0.25);
	RemoveBuildingForPlayer(playerid, 1223, -673.2891, 920.3203, 10.9141, 0.25);
	RemoveBuildingForPlayer(playerid, 1223, -683.0703, 920.4844, 10.9141, 0.25);
	RemoveBuildingForPlayer(playerid, 1223, -656.9297, 915.7578, 10.5625, 0.25);
	RemoveBuildingForPlayer(playerid, 1223, -664.8516, 920.0625, 10.9141, 0.25);
	RemoveBuildingForPlayer(playerid, 700, -658.8203, 936.1797, 11.2500, 0.25);
	RemoveBuildingForPlayer(playerid, 11492, -681.8750, 965.8906, 11.1250, 0.25);
	RemoveBuildingForPlayer(playerid, 669, -656.5781, 974.4688, 11.2734, 0.25);
	RemoveBuildingForPlayer(playerid, 3345, 400.1172, 2543.5703, 15.4844, 0.25);
	RemoveBuildingForPlayer(playerid, 3338, 259.3828, 1246.4609, 15.0781, 0.25);
	RemoveBuildingForPlayer(playerid, 3338, 358.1094, 1231.6484, 8.4297, 0.25);
	RemoveBuildingForPlayer(playerid, 3338, 441.8047, 1192.4063, 7.6953, 0.25);
	RemoveBuildingForPlayer(playerid, 3338, 629.3438, 1279.6797, 7.3203, 0.25);
	RemoveBuildingForPlayer(playerid, 16600, 435.6250, 2532.0859, 20.1797, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -261.1016, 1013.9141, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -261.1016, 1023.1641, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -264.2891, 1029.3047, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 16738, -217.4922, 1026.8203, 27.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -198.6797, 984.0156, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -204.4141, 971.7109, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -206.5000, 1000.9063, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -199.6719, 1007.9141, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -199.6719, 1028.7266, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -207.0625, 1013.9141, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -207.0625, 1023.1641, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -186.4609, 1028.8750, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -186.4609, 1007.5703, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -170.3750, 977.8984, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -286.1406, 1053.2344, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -285.4766, 1058.4063, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -199.6719, 1043.1875, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -199.6719, 1058.1250, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -186.4609, 1058.6484, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -297.2500, 1068.4297, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -270.3203, 1071.4297, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -186.4609, 1073.5625, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -199.6719, 1073.7734, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -264.8125, 1078.2734, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -165.5625, 1000.7656, 18.7031, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -269.7813, 1088.1094, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -285.9375, 1093.0156, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -263.1094, 1093.6953, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -304.5234, 1093.6953, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -264.9063, 1103.9922, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -263.1094, 1102.9375, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -292.1797, 1102.9375, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -281.0703, 1108.8984, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -265.2266, 1112.9609, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -205.9219, 1093.0156, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -235.5703, 1093.6953, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -207.0625, 1093.6953, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -201.0469, 1108.8984, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -207.0625, 1102.9375, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -235.5703, 1102.9375, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 16434, -180.7109, 1081.0781, 27.1094, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -185.1797, 1087.8438, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -180.2656, 1103.9141, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -174.3828, 1093.6953, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -174.3828, 1102.9375, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -166.7500, 1107.9688, 18.7344, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -146.8438, 1102.9375, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -146.8438, 1093.6953, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -355.8438, 1115.6719, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -340.3672, 1115.2031, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -280.3828, 1123.4141, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -199.6719, 1123.5547, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -186.4609, 1123.3984, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -286.1250, 1137.8750, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -332.1953, 1137.8125, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -350.2813, 1137.8672, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -186.4609, 1138.4609, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -162.1953, 1136.2266, 18.7344, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -199.6719, 1141.6875, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -270.3203, 1140.8984, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -164.1719, 1144.0625, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -309.5234, 1143.8984, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 16740, -152.3203, 1144.0703, 30.3047, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -364.8438, 1149.0234, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -287.9766, 1153.0938, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -329.2969, 1152.9453, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 16739, -297.1016, 1152.9688, 27.0078, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -309.8359, 1158.8359, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -264.3516, 1162.6328, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -229.4375, 1156.6250, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -186.4609, 1158.4219, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -161.9297, 1162.0781, 18.7344, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -164.1719, 1153.3125, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -270.3203, 1163.0859, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -186.4609, 1173.5625, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -280.2266, 1184.7500, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -184.9766, 1188.0156, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -159.8594, 1187.8281, 18.7344, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -250.6172, 1187.9453, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -206.0703, 1192.9219, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -160.7734, 1193.6406, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -220.0391, 1193.6406, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -250.2188, 1193.6406, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -180.1094, 1203.8984, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -160.7734, 1202.8828, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -192.1172, 1202.8828, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -220.0391, 1202.8828, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -250.2188, 1202.8828, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -201.1953, 1208.8047, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -225.7344, 1208.8125, 17.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -191.5781, 1210.2422, 18.7344, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -111.7422, 1087.5000, 19.4844, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -86.8438, 1088.4141, 19.4844, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -75.9375, 1093.0156, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -90.7891, 1093.6953, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -118.3359, 1093.6953, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -50.1797, 1093.6953, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -90.7891, 1102.9375, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -118.3359, 1102.9375, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -133.3594, 1137.5938, 18.7344, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -106.6719, 1140.0234, 18.7344, 0.25);
	RemoveBuildingForPlayer(playerid, 16735, -49.2422, 1137.7031, 28.7813, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -136.6328, 1144.0625, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -136.6328, 1153.3125, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -110.7813, 1158.7422, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -96.7188, 1164.3516, 18.7344, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -70.5859, 1173.5234, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -46.6953, 1179.5703, 18.5703, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -109.9453, 1188.0156, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -132.0703, 1187.4609, 18.7344, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -76.5313, 1187.6406, 18.7344, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -120.4922, 1183.3516, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -126.1016, 1192.9219, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -133.2266, 1193.6406, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -81.7188, 1193.6406, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -133.2266, 1202.8828, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -65.8984, 1202.8828, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -96.0781, 1202.8828, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -105.0703, 1203.8984, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1352, -121.2344, 1208.8047, 18.7109, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -95.1250, 1208.9453, 18.7344, 0.25);
	RemoveBuildingForPlayer(playerid, 16442, -18.4688, 1178.8828, 29.3750, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, -25.7813, 1188.0313, 18.6328, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -0.1484, 1193.6406, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -25.7813, 1193.6406, 22.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -11.7656, 1202.8828, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, -37.9766, 1202.8828, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, 20.9141, 1202.8828, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, 20.9141, 1193.6406, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 40.5625, 1188.6875, 17.9922, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, 49.1406, 1193.6406, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, 49.1406, 1202.8828, 23.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 76.9297, 1187.2969, 17.9922, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 278.1484, 2555.1875, 15.7969, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 316.3984, 2570.3672, 15.3047, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 361.3750, 2571.8125, 15.4063, 0.25);
	RemoveBuildingForPlayer(playerid, 1224, 410.8281, 2528.5703, 16.1563, 0.25);
	RemoveBuildingForPlayer(playerid, 1224, 409.8047, 2529.6328, 16.1563, 0.25);
	RemoveBuildingForPlayer(playerid, 1224, 408.7188, 2530.7656, 16.1563, 0.25);
	RemoveBuildingForPlayer(playerid, 1224, 407.1563, 2530.4688, 16.1563, 0.25);
	RemoveBuildingForPlayer(playerid, 3172, 400.1172, 2543.5703, 15.4844, 0.25);
	RemoveBuildingForPlayer(playerid, 1224, 407.8828, 2532.0078, 16.1563, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 405.3594, 2563.0859, 15.6250, 0.25);
	RemoveBuildingForPlayer(playerid, 16374, 425.4688, 2531.0000, 22.5547, 0.25);
	RemoveBuildingForPlayer(playerid, 16501, 429.9844, 2546.5156, 17.3516, 0.25);
	RemoveBuildingForPlayer(playerid, 16601, 435.6250, 2532.0859, 20.1797, 0.25);
	RemoveBuildingForPlayer(playerid, 16000, 110.8125, 1023.9922, 12.6484, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 94.9688, 1067.2031, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 108.9688, 1067.2031, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 96.8750, 1057.2188, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 98.6484, 1047.6563, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 91.3750, 1085.6719, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 105.3750, 1085.6719, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 100.1406, 1076.7891, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 107.1406, 1076.7891, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 112.3750, 1085.6719, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 112.6484, 1047.6563, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 121.1406, 1076.7891, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 128.1406, 1076.7891, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 122.9688, 1067.2031, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 124.8750, 1057.2188, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 3221, 119.6484, 1047.6563, 12.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 16001, 110.6172, 1109.5156, 12.7266, 0.25);
	RemoveBuildingForPlayer(playerid, 3244, 259.3828, 1246.4609, 15.0781, 0.25);
	RemoveBuildingForPlayer(playerid, 3244, 358.1094, 1231.6484, 8.4297, 0.25);
	RemoveBuildingForPlayer(playerid, 3244, 441.8047, 1192.4063, 7.6953, 0.25);
	RemoveBuildingForPlayer(playerid, 3244, 629.3438, 1279.6797, 7.3203, 0.25);
	RemoveBuildingForPlayer(playerid, 16403, -795.9297, 2395.0781, 153.6094, 0.25);
	RemoveBuildingForPlayer(playerid, 16411, -806.0078, 2387.6719, 153.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 3275, -812.0547, 2406.4531, 155.9688, 0.25);
	RemoveBuildingForPlayer(playerid, 3275, -775.9922, 2408.3594, 156.8281, 0.25);
	RemoveBuildingForPlayer(playerid, 3350, -773.0391, 2419.6484, 155.9688, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, -774.2266, 2421.2656, 157.0313, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, -775.1953, 2424.9609, 157.0313, 0.25);
	RemoveBuildingForPlayer(playerid, 3302, -774.2031, 2425.3672, 159.2969, 0.25);
	RemoveBuildingForPlayer(playerid, 16404, -772.1641, 2424.2031, 157.9219, 0.25);
	RemoveBuildingForPlayer(playerid, 3302, -772.2734, 2422.0234, 159.5781, 0.25);
	RemoveBuildingForPlayer(playerid, 3302, -772.2578, 2425.3828, 159.3750, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, -770.2422, 2421.2656, 157.0313, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, -769.1563, 2424.7656, 157.0313, 0.25);
	RemoveBuildingForPlayer(playerid, 3302, -770.3359, 2422.0234, 159.5313, 0.25);
	RemoveBuildingForPlayer(playerid, 3302, -808.5469, 2428.9375, 159.2109, 0.25);
	RemoveBuildingForPlayer(playerid, 3302, -812.3281, 2428.1406, 159.3750, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, -810.0859, 2426.1797, 156.8672, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, -813.0859, 2428.2891, 156.8672, 0.25);
	RemoveBuildingForPlayer(playerid, 16405, -809.6953, 2429.2188, 157.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, -771.5547, 2425.9609, 157.0313, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, -773.7500, 2426.0938, 157.0391, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, -807.4063, 2430.1328, 156.8828, 0.25);
	RemoveBuildingForPlayer(playerid, 3302, -807.9609, 2430.7891, 159.1406, 0.25);
	RemoveBuildingForPlayer(playerid, 3302, -811.7344, 2429.9922, 159.4219, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, -808.0391, 2431.8672, 156.8672, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, -811.8516, 2432.0781, 156.8672, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, 259.0234, 1861.9063, 8.7578, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, 259.9688, 1864.9375, 8.7578, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, 258.0781, 1862.9531, 8.7578, 0.25);
	RemoveBuildingForPlayer(playerid, 16322, 248.7266, 1869.9844, 11.8516, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, 259.9688, 1866.9219, 8.7578, 0.25);
	RemoveBuildingForPlayer(playerid, 3260, 258.0781, 1866.9219, 8.7578, 0.25);
	RemoveBuildingForPlayer(playerid, 16641, 251.4453, 1866.3047, 9.2891, 0.25);
	RemoveBuildingForPlayer(playerid, 14598, 364.7266, 173.8906, 1022.9375, 0.25);
	RemoveBuildingForPlayer(playerid, 14633, 364.9297, 188.1797, 1020.4844, 0.25);
	RemoveBuildingForPlayer(playerid, 14781, 761.3203, -0.1797, 1000.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 14780, 760.8672, 11.1719, 1000.5547, 0.25);
	RemoveBuildingForPlayer(playerid, 2186, 325.2656, 302.5000, 998.1406, 0.25);
	RemoveBuildingForPlayer(playerid, 1722, 323.3750, 313.5234, 998.1484, 0.25);
	RemoveBuildingForPlayer(playerid, 1722, 323.3750, 314.1719, 998.1484, 0.25);
	RemoveBuildingForPlayer(playerid, 1722, 323.3750, 314.8125, 998.1484, 0.25);
	RemoveBuildingForPlayer(playerid, 18041, 296.9297, -84.8984, 1002.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 2068, 288.1797, -81.5781, 1004.4219, 0.25);
	RemoveBuildingForPlayer(playerid, 1885, 291.7422, -81.6953, 1000.5078, 0.25);
	RemoveBuildingForPlayer(playerid, 18042, 293.2891, -81.6875, 1001.0156, 0.25);
	RemoveBuildingForPlayer(playerid, 2619, 293.3984, -76.3516, 1003.0469, 0.25);
	RemoveBuildingForPlayer(playerid, 2047, 283.7188, -73.9531, 1003.3281, 0.25);
	RemoveBuildingForPlayer(playerid, 2048, 283.7188, -70.6953, 1003.5547, 0.25);
	RemoveBuildingForPlayer(playerid, 2358, 284.1328, -68.2266, 1003.0391, 0.25);
	RemoveBuildingForPlayer(playerid, 18104, 289.7656, -71.2969, 1001.6172, 0.25);
	RemoveBuildingForPlayer(playerid, 18043, 293.5234, -71.5547, 1003.1406, 0.25);
	RemoveBuildingForPlayer(playerid, 2358, 284.1328, -67.1719, 1003.0391, 0.25);
	RemoveBuildingForPlayer(playerid, 2358, 284.1328, -66.3281, 1003.0391, 0.25);
	RemoveBuildingForPlayer(playerid, 2358, 284.1328, -66.8281, 1003.2969, 0.25);
	RemoveBuildingForPlayer(playerid, 2060, 291.7969, -65.9297, 1000.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 2060, 291.8203, -65.3203, 1000.9141, 0.25);
	RemoveBuildingForPlayer(playerid, 2358, 284.1328, -65.2813, 1003.0391, 0.25);
	RemoveBuildingForPlayer(playerid, 2060, 291.7500, -64.7266, 1000.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 2060, 291.8438, -64.7266, 1001.1328, 0.25);
	RemoveBuildingForPlayer(playerid, 2068, 288.1797, -64.7266, 1004.4219, 0.25);
	RemoveBuildingForPlayer(playerid, 2060, 291.7969, -62.3203, 1000.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 2060, 291.7969, -63.5234, 1000.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 2060, 291.8203, -64.1172, 1000.9141, 0.25);
	RemoveBuildingForPlayer(playerid, 2060, 291.8203, -62.9141, 1000.9141, 0.25);
	RemoveBuildingForPlayer(playerid, 2060, 291.8203, -63.5234, 1001.1328, 0.25);
	RemoveBuildingForPlayer(playerid, 2358, 284.1328, -60.7031, 1003.0391, 0.25);
	RemoveBuildingForPlayer(playerid, 2048, 289.5938, -56.3359, 1003.5625, 0.25);
	RemoveBuildingForPlayer(playerid, 4504, 56.3828, -1531.4531, 6.7266, 0.25);
	RemoveBuildingForPlayer(playerid, 4505, 14.4609, -1347.3281, 11.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 4506, -18.2266, -1335.9844, 12.7266, 0.25);
	RemoveBuildingForPlayer(playerid, 4507, 53.9609, -1018.9922, 23.3359, 0.25);
	RemoveBuildingForPlayer(playerid, 4521, -84.6797, -909.3438, 18.2422, 0.25);
	RemoveBuildingForPlayer(playerid, 4516, -141.3359, 468.6484, 12.9141, 0.25);
	RemoveBuildingForPlayer(playerid, 4517, -193.8281, 269.5078, 12.8906, 0.25);
    GangZoneShowForPlayer(playerid,greenZone,COLOR_GREEN);
    GangZoneShowForPlayer(playerid,redZone[0],COLOR_RED);
    GangZoneShowForPlayer(playerid,redZone[1],COLOR_RED);
    GangZoneShowForPlayer(playerid,redZone[2],COLOR_RED);
    GangZoneShowForPlayer(playerid,redZone[3],COLOR_RED);
    GangZoneShowForPlayer(playerid,redZone[4],COLOR_RED);
    GangZoneShowForPlayer(playerid,redZone[5],COLOR_RED);
    GangZoneShowForPlayer(playerid,redZone[6],COLOR_RED);
    GangZoneShowForPlayer(playerid,redZone[7],COLOR_RED);
    GangZoneShowForPlayer(playerid,yellowZone[0],COLOR_YELLOW);
    GangZoneShowForPlayer(playerid,yellowZone[1],COLOR_YELLOW);
    loadTextDrawToPlayer(playerid);
    TogglePlayerSpectating(playerid, true);
    cskin[playerid] = 0;
    TextDrawShowForPlayer(playerid,logo[0]);
    new queryPlayerInventory[512];
    mysql_format(connection, queryPlayerInventory, sizeof(queryPlayerInventory), "SELECT `AntiRadin`, `Alcohol`, `Weed`, `Food`, `Medcine`,\
    `ZombiesBlood`, `Instruments`, `Canister`, `MasterKey`, `Bolt`, `SpecialAmmo`, `Metal`,\
    `Wood`, `Blanks`, `Drugs`, `Flesh`, `Fruits`, `Pumpkin`, `Croup`, `Milk`, `Egg`, `Artefacts` FROM inventory WHERE Name='%e'",playerName(playerid));
    mysql_tquery(connection, queryPlayerInventory, "", "d", playerid);
    new queryPlayerInfo[128];
    mysql_format(connection, queryPlayerInfo, sizeof(queryPlayerInfo), "SELECT `Name`,`ID`,`Skin`,`Password`,`Cents`,`Radiation`,`Hunger`,`Thirst`,`Virus` FROM account WHERE Name='%e'", playerName(playerid));
    mysql_tquery(connection, queryPlayerInfo, "checkAccount", "d", playerid);
    return true;
}
public checkAccount(playerid)
{
    SelectTextDraw(playerid, 0xFF0000FF);
    for(new i = 0; i < sizeof(background); i++) TextDrawShowForPlayer(playerid, background[i]);
    if(cache_num_rows())
    {
        new skinTmp,idTmp;
        format(PlayerInfo[playerid][password], 129, "%s", pass);
        cache_get_value_name(0, "Password", pass);
        cache_get_value_name_int(0, "ID", idTmp);
        cache_get_value_name_int(0, "Skin", skinTmp);
        login[playerid][6] = TextDrawCreate(343.000000, 110.000000, "skin");
        TextDrawBackgroundColor(login[playerid][6], 0);
        TextDrawFont(login[playerid][6], 5);
        TextDrawLetterSize(login[playerid][6], 0.479999, 10.600000);
        TextDrawColor(login[playerid][6], -1);
        TextDrawSetOutline(login[playerid][6], 1);
        TextDrawSetProportional(login[playerid][6], 1);
        TextDrawUseBox(login[playerid][6], 1);
        TextDrawBoxColor(login[playerid][6], 255);
        TextDrawTextSize(login[playerid][6], 99.000000, 120.000000);
        TextDrawSetPreviewModel(login[playerid][6], skinTmp);
        TextDrawSetPreviewRot(login[playerid][6], 1.000000, 1.000000, 1.000000, 1.000000);
        TextDrawSetSelectable(login[playerid][6], 0);
        new strName[48], strId[20];
        format(strName, sizeof(strName), "~g~~h~Name: ~w~%s", playerName(playerid));
        TextDrawSetString(login[playerid][1], strName);
        format(strId, sizeof(strId), "~g~~h~Acc #: ~w~%i", idTmp);
        TextDrawSetString(login[playerid][2], strId);
        for(new i = 0; i < 7; i++) TextDrawShowForPlayer(playerid, login[playerid][i]);
    }
    else
    {
        for(new i = 0; i < 8; i++) TextDrawShowForPlayer(playerid, registration[i]);
    }
    return true;
}
//Общие команды
CMD:menu(playerid)
{
    if (PlayerInfo[playerid][logged] == false) return true;
    else SPD(playerid,DIALOG_MENU,DIALOG_STYLE_LIST,"{FFCC00}Рюкзак","{FFFFFF}Содержимое\nДосье\nНавыки\nЗаписки\nНавигатор\n\nИспользовать аптечку\nИспользовать антирадин\n\
    Выпить алкоголь\nПоесть\nПокурить\nШирнуться\nИспользовать рем. набор\nИспользовать канистру\n\nПритвориться мирным\n\
    Активировать артефакт\nСпец. пт\nСтиль боя\nПангит","Выбрать","Выйти");
    return true;
}
CMD:jail(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][member] == 2 && PlayerInfo[playerid][rank] != 16 ||
    PlayerInfo[playerid][member] == 3 && PlayerInfo[playerid][rank] != 16 ||
    PlayerInfo[playerid][member] == 4 && PlayerInfo[playerid][rank] != 16 ||
    PlayerInfo[playerid][member] == 6 && PlayerInfo[playerid][rank] != 6 ||
    PlayerInfo[playerid][member] == 7 && PlayerInfo[playerid][rank] != 6 ||
    PlayerInfo[playerid][member] == 8 && PlayerInfo[playerid][rank] != 6 ||
    PlayerInfo[playerid][member] == 9 && PlayerInfo[playerid][rank] != 6 ||
    PlayerInfo[playerid][member] == 10 && PlayerInfo[playerid][rank] != 6)
        return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "iis[128]", params[0], params[1], params[2])) return SCM(playerid, COLOR_SYSTEM, "Используйте /jail [id] [минуты] [причина]");
    if (PlayerInfo[params[0]][member] != PlayerInfo[playerid][member]) return SCM(playerid, COLOR_SYSTEM, "Не в Вашей фракции");
    new string[128];
    new const str[] = "%s %s[%i] посадил %s %s[%i] в КПЗ на %i мин по причине: %s";
    format(string, sizeof(string), str, rankName(playerid),playerName(playerid),playerid,rankName(params[0]),playerName(params[0]),params[0],params[1],params[2]);
    SCM(params[0],COLOR_RED,string);
    SCM(playerid,COLOR_RED,string);
    SAM(COLOR_SYSTEM,string);
    PlayerInfo[params[0]][jail] += params[1];
    switch (PlayerInfo[params[0]][member])
    {
        case 2,8: 
        {
            SetPlayerInterior(params[0],0);
            SetPlayerPos(params[0],111.4851, 1047.4766, 6.2349);
        }
        case 1,7: 
        {
            SetPlayerInterior(params[0],0);
            SetPlayerPos(params[0],-699.9142, 978.1697, 7.8307);
        }
        case 3,6,10: 
        {
            SetPlayerInterior(params[0], 5);
            SetPlayerPos(params[0],322.1140, 314.1566, 999.3682);
        }
        case 4,9: 
        {
            SetPlayerInterior(params[0],6);
            SetPlayerPos(params[0],347.0079, 306.2476, 999.7764);
        }    
    }   
    return true;
}
CMD:warn(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][member] == 1 && PlayerInfo[playerid][rank] != 16 ||
    PlayerInfo[playerid][member] == 2 && PlayerInfo[playerid][rank] != 16 ||
    PlayerInfo[playerid][member] == 3 && PlayerInfo[playerid][rank] != 16 ||
    PlayerInfo[playerid][member] == 4 && PlayerInfo[playerid][rank] != 16 ||
    PlayerInfo[playerid][member] == 6 && PlayerInfo[playerid][rank] != 6 ||
    PlayerInfo[playerid][member] == 7 && PlayerInfo[playerid][rank] != 6 ||
    PlayerInfo[playerid][member] == 8 && PlayerInfo[playerid][rank] != 6 ||
    PlayerInfo[playerid][member] == 9 && PlayerInfo[playerid][rank] != 6 ||
    PlayerInfo[playerid][member] == 10 && PlayerInfo[playerid][rank] != 6)  
        return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "is[128]", params[0], params[1])) return SCM(playerid, COLOR_SYSTEM, "Используйте /warn [id] [причина]");
    new string[128];
    new const str[] = "%s %s[%i] выдал предупреждение %s %s[%i] по причине: %s";
    format(string,sizeof(string),str,rankName(playerid),playerName(playerid),playerid,rankName(params[0]),playerName(params[0]),params[0],params[1]);
    SAM(COLOR_SYSTEM,string);
    SCM(params[0],COLOR_RED,string);
    PlayerInfo[params[0]][warnLevel] += 1;
    if (PlayerInfo[params[0]][warnLevel] == 2) 
    {
        PlayerInfo[params[0]][rank] = 1;
        SCM(params[0],COLOR_RED,"Ваш ранг был понижен до 1 (2 предупреждения)");
    }
    else if (PlayerInfo[params[0]][warnLevel] > 2)
    {
        PlayerInfo[params[0]][member] = 0;
        PlayerInfo[params[0]][rank] = 0;
        PlayerInfo[params[0]][warnLevel] = 0;
        SCM(params[0],COLOR_RED,"Ваш были выгнаны из фракции (3 предупреждения)");    
    } 
    return true;
}
CMD:invite(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][member] != 3 && PlayerInfo[playerid][rank] != 16)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "i", params[0])) return SCM(playerid, COLOR_SYSTEM, "Используйте /invite [id]");
    if (PlayerInfo[params[0]][member] != 0) return SCM(playerid, COLOR_SYSTEM, "Уже состоит в организации");
    new string[128];
    new const str[] = "%s %s[%i] предлагает Вам присоединиться к Вооруженным Силам";
    format(string,sizeof(string),str,rankName(playerid),playerName(playerid),playerid);
    SCM(params[0],COLOR_RED,string);
    SetPVarInt(params[0],"accept",1);
    SCM(params[0],COLOR_RED,"Нажмите Y чтобы согласиться или N чтобы отказаться");
    return true; 
}
CMD:f(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][member] < 1)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "s[128]", params[0])) return SCM(playerid, COLOR_SYSTEM, "Используйте /f [сообщение]");
    new string[128];
    format(string, sizeof(string), "[F] %s (%s %s[%i])",params[0],rankName(playerid),playerName(playerid),playerid);
    switch (PlayerInfo[playerid][member])
    {
        case 3,6,10: return SGM(0, string);
        case 2,8: return SSM(0, string);
        case 4,9: return SBM(0, string);
        case 1,7: return SMM(0, string);
    }
    return true;
}
CMD:r(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][member] < 1)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "s[128]", params[0])) return SCM(playerid, COLOR_SYSTEM, "Используйте /r [сообщение]");
    new string[128];
    format(string, sizeof(string), "[R] %s (%s %s[%i])",params[0],rankName(playerid),playerName(playerid),playerid);
    switch (PlayerInfo[playerid][member])
    {
        case 3: return SGM(3, string);
        case 6: return SGM(6, string);
        case 10: return SGM(10, string);
        case 2: return SSM(2, string);
        case 8: return SSM(8, string);
        case 4: return SBM(4, string);
        case 9: return SBM(9, string);
        case 1: return SMM(1, string);
        case 7: return SMM(7, string);
    }
    return true;
}
// Команды администрации
CMD:gethealth(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][adminLevel] < 1)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "i", params[0])) return SCM(playerid, COLOR_SYSTEM, "Используйте /gethealth [id]");
    new Float: healthP, string[64];
    GetPlayerHealth(params[0], healthP); 
    format(string,sizeof(string),"%s[%i] HP: %f и %f",playerName(params[0]),params[0],healthP, PlayerInfo[params[0]][health]);
    SCM(playerid,COLOR_SYSTEM,string);
    return true;
}
CMD:skinfix(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][adminLevel] < 3)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "i", params[0])) return SCM(playerid, COLOR_SYSTEM, "Используйте /memberskin [id]");
    if (PlayerInfo[params[0]][member] == 0) return SCM(playerid, COLOR_SYSTEM, "Игрок не состоит в организации");
    getRankSkin(params[0]);
    SetPlayerSkin(params[0],PlayerInfo[params[0]][skin]);
    new string[128];
    new const str[] = "[A] [%s] %s[%i] пофиксил облик игроку %s[%i]";
    format(string,sizeof(string),str, adminRankName(playerid), playerName(playerid),playerid, playerName(params[0]),params[0]);
    SAM(COLOR_SYSTEM, string);
    return true;
}
CMD:setmember(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][adminLevel] < 3)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "iii", params[0], params[1], params[2])) return SCM(playerid, COLOR_SYSTEM, "Используйте /setmember [id] [member id] [rank]");
    PlayerInfo[params[0]][member] = params[1];
    PlayerInfo[params[0]][rank] = params[2];
    new string[128];
    new const str[] = "[A] [%s] %s[%i] назначил игрока %s[%i] '%s' в организации: '%s'";
    format(string,sizeof(string),str, adminRankName(playerid), playerName(playerid),playerid, playerName(params[0]),params[0], rankName(params[0]),memberName(params[0]));
    SAM(COLOR_SYSTEM, string);
    switch (PlayerInfo[params[0]][member])
    {
        case 3,6,10:
        {
            Iter_Remove(stalkers, params[0]);
            Iter_Remove(bandits, params[0]);
            Iter_Remove(monolites, params[0]);
            Iter_Add(governments,params[0]);
        }
        case 2,8:
        {
            Iter_Remove(bandits, params[0]);
            Iter_Remove(monolites, params[0]);
            Iter_Remove(governments, params[0]);
            Iter_Add(stalkers,params[0]);
        }
        case 4,9:
        {
            Iter_Remove(stalkers, params[0]);
            Iter_Remove(monolites, params[0]);
            Iter_Remove(governments, params[0]);
            Iter_Add(bandits,params[0]);
        }
        case 1,7:
        {
            Iter_Remove(stalkers, params[0]);
            Iter_Remove(bandits, params[0]);
            Iter_Remove(governments, params[0]);
            Iter_Add(monolites,params[0]);
        }
    }
    return true;
}
CMD:setskin(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][adminLevel] < 3)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "ii", params[0], params[1])) return SCM(playerid, COLOR_SYSTEM, "Используйте /setskin [id] [id скина]");
    SetPlayerSkin(params[0],params[1]);
    PlayerInfo[params[0]][skin] = params[1];
    new string[128];
    format(string,sizeof(string),"[A] [%s] %s[%i] изменил облик игроку %s[%i]", adminRankName(playerid), playerName(playerid),playerid, playerName(params[0]),params[0]);
    SAM(COLOR_SYSTEM, string);
    return true;
}
CMD:kick(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][adminLevel] < 1)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "is[64]", params[0], params[1])) return SCM(playerid, COLOR_SYSTEM, "Используйте /kick [id] [причина]");
    if (PlayerInfo[params[0]][adminLevel] > 0 && PlayerInfo[playerid][adminLevel] < 5) return true;
    new string[128];
    format(string,sizeof(string),"[A] [%s] %s[%i] кикнул %s[%i] по причине: %s", adminRankName(playerid), playerName(playerid),playerid, playerName(params[0]),params[0],params[1]);
    SCM(params[0],COLOR_RED,string);
    SAM(COLOR_SYSTEM, string);
    Kick(params[0]);   
    return true; 
}
CMD:ban(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][adminLevel] < 3) return SCM(playerid,COLOR_SYSTEM,"Несуществующая команда");
    if (sscanf(params,"iis[64]", params[0], params[1], params[2])) return SCM(playerid, COLOR_SYSTEM, "Используйте /ban [id] [срок в часах] [причина]");
    if (PlayerInfo[params[0]][adminLevel] > 0 && PlayerInfo[playerid][adminLevel] < 5) return SCM(playerid,COLOR_SYSTEM,"Несуществующая команда");
    new string[256];
    format(string,sizeof(string),"[A] [%s] %s[%i] забанил %s[%i] на %i час(-a/-ов) по причине: %s",adminRankName(playerid), playerName(playerid), playerid, playerName(params[0]),params[0],params[1],params[2]);
    SCM(params[0],COLOR_RED,string);
    SAM(COLOR_SYSTEM, string);
    PlayerInfo[params[0]][ban] = params[1];
    Kick(params[0]);
    return true;   
}
CMD:mute(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][adminLevel] < 1) return SCM(playerid,COLOR_SYSTEM,"Несуществующая команда");
    if (sscanf(params,"iis[64]",params[0],params[1],params[2])) return SCM(playerid,COLOR_SYSTEM,"Используйте: /mute [id] [срок в минутах] [причина]");
    //if (PlayerInfo[params[0]][adminLevel] > 0 && PlayerInfo[playerid][adminLevel] < 5) return true;
    new string[64+67+32];
    new const str[] = "[A] [%s] %s[%i] поставил затычку игроку %s[%i] на %i минут по причине: %s"; 
    format(string,sizeof(string),str,adminRankName(playerid),playerName(playerid),playerid,playerName(params[0]),params[0],params[1],params[2]);
    SCM(params[0],COLOR_RED,string);
    SAM(COLOR_SYSTEM,string);
    PlayerInfo[params[0]][mute] = params[1];
    return true;    
}
CMD:unmute(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][adminLevel] < 1) return SCM(playerid,COLOR_SYSTEM,"Несуществующая команда");
    if (sscanf(params,"i",params[0])) return SCM(playerid,COLOR_SYSTEM,"Используйте: /unmute [id]");
    new string[104];
    new const str[] = "[A] [%s] %s[%i] снял затычку с игрока %s[%i]";
    format(string,sizeof(string),str,adminRankName(playerid),playerName(playerid),playerid,playerName(params[0]),params[0],params[1],params[2]);
    SCM(params[0],COLOR_RED,string);
    SAM(COLOR_SYSTEM,string);
    PlayerInfo[params[0]][mute] = 0;
    return true;    
}
CMD:info(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][adminLevel] < 1) return SCM(playerid,COLOR_SYSTEM,"Несуществующая команда");
    if (sscanf(params,"i",params[0])) return SCM(playerid,COLOR_SYSTEM,"Используйте: /info [id]");
    new string[512];
    format(string,sizeof(string),"{FFFFFF}Позывной\t\t%s\nУровень допуска\t%i\nГруппировка\t\t%s\nЗвание\t\t\t%s\nОпыт\t\t\t%i\nЗамечания\t\t%i\n\
                        Клан\t\t\t%s\nДонат очки\t\t%i\nЧасы в зоне\t\t%i\nVIP уровень\t\t%i\nIP адрес\t\t%s\nДеньги\t\t%i",
                        PlayerInfo[params[0]][nickName],PlayerInfo[params[0]][accessLevel],memberName(params[0]),
                        rankName(params[0]),PlayerInfo[params[0]][expLevel],PlayerInfo[params[0]][warnLevel],
                        PlayerInfo[params[0]][clan],PlayerInfo[params[0]][donateLevel],
                        PlayerInfo[params[0]][hoursLevel],PlayerInfo[params[0]][vipLevel], playerIp(params[0]),PlayerInfo[params[0]][money]);
    SPD(playerid,DIALOG_PLAYER_INFO,DIALOG_STYLE_MSGBOX,"{FFCC00}Информация об игроке",string,"Продолжить","");
    return true;    
}
CMD:goto(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][adminLevel] < 1) return SCM(playerid,COLOR_SYSTEM,"Несуществующая команда");
    if (sscanf(params,"i",params[0])) return SCM(playerid,COLOR_SYSTEM,"Используйте: /goto [id]");
    new string[128];
    format(string,sizeof(string),"[A] [%s] %s[%i] телепортировался к игроку %s[%i]", adminRankName(playerid), playerName(playerid),playerid, playerName(params[0]),params[0]);
    SAM(COLOR_SYSTEM, string);
    new Float: x, Float: y, Float: z;
    GetPlayerPos(params[0],x,y,z);
    SetPlayerPos(playerid,x+2.0,y,z);
    return true; 
}
CMD:gethere(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][adminLevel] < 1) return SCM(playerid,COLOR_SYSTEM,"Несуществующая команда");
    if (sscanf(params,"i",params[0])) return SCM(playerid,COLOR_SYSTEM,"Используйте: /gethere [id]");
    new string[128];
    format(string,sizeof(string),"[A] [%s] %s[%i] телепортировал к себе игрока %s[%i]", adminRankName(playerid), playerName(playerid),playerid, playerName(params[0]),params[0]);
    SAM(COLOR_SYSTEM, string);
    new Float: x, Float: y, Float: z;
    GetPlayerPos(playerid,x,y,z);
    SetPlayerPos(params[0],x+2.0,y,z);
    return true; 
}       
CMD:a(playerid, params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][adminLevel] < 1)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "s[128]", params[0])) return SCM(playerid, COLOR_SYSTEM, "Используйте /a [сообщение]");
    new string[128];
    format(string,sizeof(string),"[A] [%s] %s[%i]: %s", adminRankName(playerid), playerName(playerid),playerid, params[0]);
    SAM(0x32CD32AA, string);
    return true;
}
CMD:veh(playerid,params[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][adminLevel] <= 3) return true;
    new string[64];
    new Float:pX,Float:pY,Float:pZ;  
    if (sscanf(params, "iii", params[0],params[1],params[2])) return SCM(playerid, COLOR_SYSTEM, "Используйте: /veh [id транспорта] [цвет 1] [цвет 2]");
    {
        if (vehID[playerid] > 0)
        {
            DestroyVehicle(vehID[playerid]);
            vehID[playerid] = 0;
        }
        if (params[1] > 126 || params[1] < 0 || params[2] > 126 || params[2] < 0) return SCM(playerid, COLOR_SYSTEM, "id цвета от 0 до 126");
        GetPlayerPos(playerid,pX,pY,pZ);
        vehID[playerid] = CreateVehicle(params[0],pX+2,pY,pZ,0.0,1,1,0,0);
        format(string,sizeof(string),"[A] [%s] %s[%i] создал т/с %i id [%i server id]",adminRankName(playerid),playerName(playerid),playerid,params[0],vehID[playerid]);
        SAM(COLOR_SYSTEM,string);
        ChangeVehicleColor(vehID[playerid],params[1],params[2]);
    }
    return true;
}
CMD:delveh(playerid)
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][adminLevel] <= 3) return true;
    if (vehID[playerid] > 0)
    {
        DestroyVehicle(vehID[playerid]);
        vehID[playerid] = 0;
    }
    else SCM(playerid,COLOR_SYSTEM,"Вы не создавали транспорт");
    return true;
}
//Функции проекта
public OnPlayerDisconnect(playerid, reason)
{
    if(vehID[playerid] > 0)
    {
        DestroyVehicle(vehID[playerid]);
        vehID[playerid] = 0;
    }
    switch(PlayerInfo[playerid][member])
    {
        case 3,6,10: Iter_Remove(governments,playerid);
        case 2,8: Iter_Remove(stalkers,playerid);
        case 4,9: Iter_Remove(bandits,playerid);
        case 1,7: Iter_Remove(monolites,playerid);
    }
    if (GetPVarInt(playerid,"accept")) DeletePVar(playerid,"accept");
    if (PlayerInfo[playerid][adminLevel] > 0) Iter_Remove(admins, playerid);
    saveAccount(playerid);
    return true;
}

public OnPlayerText(playerid, text[])
{
    if (PlayerInfo[playerid][logged] == false) return true;
    if (PlayerInfo[playerid][mute] > 0) return SCM(playerid,COLOR_SYSTEM,"Бан чата");
    new string[128], Float:x, Float:y, Float: z;
    new const str[] = "- %s (%s %s[%i])";
    format(string,sizeof string,str,text,rankName(playerid),playerName(playerid),playerid);
    GetPlayerPos(playerid,x,y,z);
    foreach (Player, i)
    {
        if(!IsPlayerInRangeOfPoint(i,50.0,x,y,z)) continue;
        SCM(i,0xFFFFFFFF,string);
    }
    return true;
}

public OnPlayerSpawn(playerid)
{
    SetPlayerInterior(playerid,0);
    if (PlayerInfo[playerid][jail] > 0)
    {
        switch (PlayerInfo[playerid][member])
        {
            case 2,8: return SetPlayerPos(playerid,111.4851, 1047.4766, 6.2349);
            case 1,7: return SetPlayerPos(playerid,-699.9142, 978.1697, 7.8307);
            case 3,6,10: return SetPlayerPos(playerid,322.1140, 314.1566, 999.3682);
            case 4,9: return SetPlayerPos(playerid,347.0079, 306.2476, 999.7764);    
        }   
    }
    if (killed[playerid] == false)
    {
        loadCents(playerid);
        loadMainText(playerid);
        SetPlayerHealth(playerid,PlayerInfo[playerid][health]);
        //PreloadAllAnimLibs(playerid);
        new query[512];
        mysql_format(connection, query, sizeof(query), "SELECT `Cents`,`Radiation`,`Hunger`,`Thirst`,`Virus` FROM `account` WHERE Name='%e'", playerName(playerid));
        mysql_tquery(connection, query, "loadNewsTD", "d", playerid);
    }
    SetPlayerHealth(playerid,30);
    TogglePlayerSpectating(playerid, false);
    if(PlayerInfo[playerid][logged] == false) return true;
    return false;
}

public OnPlayerDeath(playerid, killerid, reason)
{
    killed[playerid] = true;
    SendDeathMessage(killerid, playerid, reason);
    PlayerInfo[playerid][deaths]++;
    if (killerid != INVALID_PLAYER_ID) PlayerInfo[killerid][kills]++;
    //SetSpawnInfo(playerid, 1, PlayerInfo[playerid][skin], 613.1469,-1415.0784,13.5688, 269.15, 0, 0, 0, 0, 0, 0);
    //SpawnPlayer(playerid);
    return true;
}

public OnPlayerUpdate(playerid)
{
    if (PlayerInfo[playerid][jail] < 0) 
    {
        PlayerInfo[playerid][jail] = 0;
        switch (PlayerInfo[playerid][member])
        {
            case 1:
            {
                SetPlayerInterior(playerid,17);
                SetPlayerPos(playerid,-951.5187, 1867.1998, 5.9427);
            }
            case 2: 
            {
                SetPlayerInterior(playerid,0);
                SetPlayerPos(playerid,419.6006, 2536.7456, 10.6572);
            }
            case 4,9: 
            {
                SetPlayerInterior(playerid,17);
                SetPlayerPos(playerid,488.3408, -14.8237, 1001.6078);
            }
            case 3,6,10: 
            {
                SetPlayerInterior(playerid,5);
                SetPlayerPos(playerid,770.4814, 2.5649, 1000.9934);
            }
            case 7: 
            {
                SetPlayerInterior(playerid,0);
                SetPlayerPos(playerid,-707.7855, 979.1034, 7.7314);
            } 
            case 8: 
            {
                SetPlayerInterior(playerid,0);
                SetPlayerPos(playerid,104.2438, 1045.7549, 2.3346);
            }
        }
    } 
    SetPlayerScore(playerid, PlayerInfo[playerid][accessLevel]);
    return true;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    new string[512];
    switch(dialogid)
    {
        case DIALOG_REGISTER:
        {
            if(!response) return 1;
            if(strlen(inputtext) < 4) return SPD(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "{FFCC00}Регистрация", "\n\n\t\t{FFFFFF}Введите пароль:\n\n", "Далее", "Выйти");
            format(string, sizeof(string), "Пароль: {058AFF}%s", inputtext);
            SCM(playerid, COLOR_YELLOW, string);
            format(PlayerInfo[playerid][password], 129, inputtext);
            passWrote[playerid] = 1;
            for(new i = 0; i < sizeof(registration); i++) TextDrawShowForPlayer(playerid, registration[i]);
        }
        case DIALOG_EMAIL:
        {
            new valid = false;
            if(!response) return true;
            if(strlen(inputtext) < 4) return SPD(playerid, DIALOG_EMAIL, DIALOG_STYLE_INPUT, "{FFCC00}Регистрация", "\n\n\t{FFFFFF}Введите адрес электронной почты:\n\n", "Далее", "Выйти");
            for (new i=0; i < strlen(inputtext); i++)
            {
                if(inputtext[i] == '@') valid = true;
            }
            if (!valid) return SPD(playerid, DIALOG_EMAIL, DIALOG_STYLE_INPUT, "{FFCC00}Регистрация", "\n\n\t{FFFFFF}Введите адрес электронной почты:\n\n", "Далее", "Выйти");
            format(string, sizeof(string), "Электронная почта: {058AFF}%s", inputtext);
            SCM(playerid, COLOR_YELLOW, string);
            format(PlayerInfo[playerid][mail], 129, inputtext);
            mailWrote[playerid] = 1;
            for(new i = 0; i < sizeof(registration); i++) TextDrawShowForPlayer(playerid, registration[i]);
        }
        case DIALOG_SEX:
        {
            if(response)
            {
                SCM(playerid, COLOR_YELLOW, "Пол: {058AFF}Мужской");
                PlayerInfo[playerid][sex] = 1;
                sexWrote[playerid] = 1;
                for(new i = 0; i < sizeof(registration); i++) TextDrawShowForPlayer(playerid, registration[i]);

            }
            else
            {
                SCM(playerid, COLOR_YELLOW, "Пол: {EE3B3B}Женский");
                PlayerInfo[playerid][sex] = 2;
                sexWrote[playerid] = 1;
                for(new i = 0; i < sizeof(registration); i++) TextDrawShowForPlayer(playerid, registration[i]);
            }
        }

        case DIALOG_AGE:
        {
            if(!response) return 1;
            if(response)
            {
                if(strval(inputtext) < 5 || strval(inputtext) > 99) return SPD(playerid, DIALOG_AGE, DIALOG_STYLE_INPUT, "{FFCC00}Регистрация", "\n{FFFFFF}Возраст Вашего персонажа должен быть от 16 до 90\n\n", "Далее", "Выйти");
                format(string, sizeof(string), "Возраст: {058AFF}%d", strval(inputtext));
                SCM(playerid, COLOR_YELLOW, string);
                PlayerInfo[playerid][age] = strval(inputtext);
                ageWrote[playerid] = 1;
                for(new i = 0; i < sizeof(registration); i++) TextDrawShowForPlayer(playerid, registration[i]);
            }
        }

        case DIALOG_LOGIN:
        {
            if(!response) return true;
            if(!strcmp(pass, inputtext, false, 129))
            {
                new queryInfo[512];
                mysql_format(connection, queryInfo, sizeof(queryInfo), "SELECT * FROM `account` WHERE Name = '%e'", playerName(playerid));
                mysql_tquery(connection, queryInfo, "loadAccount", "d", playerid);
            } 
            else 
            {
                PlayerInfo[playerid][wrongPass] ++;
                SCM(playerid, COLOR_RED, "Неверный пароль");
                SPD(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "{FFCC00}Авторизация", "\n\n\t\t{FFFF00}Неверный пароль\n\n\t\t{FFFFFF}Введите пароль:\n\n", "Далее", "Выйти");
                if(PlayerInfo[playerid][wrongPass] == 3)
                {
                    SCM(playerid, COLOR_RED, "{FFFF00}Связь с сервером была разорвана. Использовано максимальное количество попыток ввода пароля");
                    SetTimerEx("timerKick", 100, false, "i", playerid);
                }
            }
        }
        case DIALOG_FORUM:
        {
            if(!response) return true;
            if(response)
            {
                for(new i = 0; i < 7; i++) TextDrawShowForPlayer(playerid, login[playerid][i]);
            }
        }
        case DIALOG_CREDITS:
        {
            if(!response) return true;
            if(response)
            {
                for(new i = 0; i < 7; i++) TextDrawShowForPlayer(playerid, login[playerid][i]);
            }
        }
        case DIALOG_MENU:
        {
            if (response)
            {
                switch (listitem)
                {
                    case 0:
                    {
                        switch (PlayerInfo[playerid][member])
                        {
                            case 0:
                            {
                                format(string,sizeof(string),"{FFFFFF}Наименование:\t\tКоличество:\nАнтирадин\t\t%i\nАлкоголь\t\t%i\nСигареты\t\t%i\n\
                                Еда\t\t\t%i\nАптечка\t\t%i\nКровь зомби\t\t%i\nРем. комплект\t\t%i\nКанистра\t\t%i\nОтмычки\t\t%i\nБолты\t\t\t%i\nСпец. боеприпасы\t%i\n\
                                Металл\t\t\t%i\nДревесина\t\t%i\nЗаготовки\t\t%i\nНаркотики\t\t%i\nФрукты\t\t\t%i\nТыквa\t\t\t%i\nКрупа\t\t\t%i\nМолоко\t\t\t%i\nЯйца\t\t\t%i",
                                PlayerInventory[playerid][antiRadin],PlayerInventory[playerid][alcohol],PlayerInventory[playerid][weed],
                                PlayerInventory[playerid][food],PlayerInventory[playerid][medcine],PlayerInventory[playerid][zombiesBlood],
                                PlayerInventory[playerid][instruments],PlayerInventory[playerid][canister],PlayerInventory[playerid][masterKey],
                                PlayerInventory[playerid][bolt],PlayerInventory[playerid][specialAmmo],PlayerInventory[playerid][metal],
                                PlayerInventory[playerid][wood],PlayerInventory[playerid][blanks],PlayerInventory[playerid][drugs],
                                PlayerInventory[playerid][fruits],PlayerInventory[playerid][pumpkin],
                                PlayerInventory[playerid][croup],PlayerInventory[playerid][milk],PlayerInventory[playerid][egg]);
                                SPD(playerid,DIALOG_MENU_INVENTORY,DIALOG_STYLE_MSGBOX,"{FFCC00}Рюкзак",string,"Назад","Выйти");
                            }
                            case 1: //Мутанты
                            {
                                format(string,sizeof(string),"{FFFFFF}Наименование:\t\tКоличество:\nАнтирадин\t\t%i\nАлкоголь\t\t%i\nСигареты\t\t%i\n\
                                Еда\t\t\t%i\nАптечка\t\t%i\nКровь зомби\t\t%i\nРем. комплект\t\t%i\nКанистра\t\t%i\nОтмычки\t\t%i\nБолты\t\t\t%i\nСпец. боеприпасы\t%i\n\
                                Металл\t\t\t%i\nДревесина\t\t%i\nЗаготовки\t\t%i\nНаркотики\t\t%i\nПлоть\t\t\t%i\nФрукты\t\t\t%i\nТыквa\t\t\t%i\nКрупа\t\t\t%i\nМолоко\t\t\t%i\nЯйца\t\t\t%i",
                                PlayerInventory[playerid][antiRadin],PlayerInventory[playerid][alcohol],PlayerInventory[playerid][weed],
                                PlayerInventory[playerid][food],PlayerInventory[playerid][medcine],PlayerInventory[playerid][zombiesBlood],
                                PlayerInventory[playerid][instruments],PlayerInventory[playerid][canister],PlayerInventory[playerid][masterKey],
                                PlayerInventory[playerid][bolt],PlayerInventory[playerid][specialAmmo],PlayerInventory[playerid][metal],
                                PlayerInventory[playerid][wood],PlayerInventory[playerid][blanks],PlayerInventory[playerid][drugs],
                                PlayerInventory[playerid][flesh],PlayerInventory[playerid][fruits],PlayerInventory[playerid][pumpkin],
                                PlayerInventory[playerid][croup],PlayerInventory[playerid][milk],PlayerInventory[playerid][egg]);
                                SPD(playerid,DIALOG_MENU_INVENTORY,DIALOG_STYLE_MSGBOX,"{FFCC00}Рюкзак",string,"Назад","Выйти");
                            }
                            case 2: // Сталкеры
                            {
                                format(string,sizeof(string),"{FFFFFF}Наименование:\t\tКоличество:\nАнтирадин\t\t%i\nАлкоголь\t\t%i\nСигареты\t\t%i\n\
                                Еда\t\t\t%i\nАптечка\t\t%i\nКровь зомби\t\t%i\nРем. комплект\t\t%i\nКанистра\t\t%i\nОтмычки\t\t%i\nБолты\t\t\t%i\nСпец. боеприпасы\t%i\n\
                                Металл\t\t\t%i\nДревесина\t\t%i\nЗаготовки\t\t%i\nНаркотики\t\t%i\nАртефакты\t\t\t%i\nФрукты\t\t\t%i\nТыквa\t\t\t%i\nКрупа\t\t\t%i\nМолоко\t\t\t%i\nЯйца\t\t\t%i",
                                PlayerInventory[playerid][antiRadin],PlayerInventory[playerid][alcohol],PlayerInventory[playerid][weed],
                                PlayerInventory[playerid][food],PlayerInventory[playerid][medcine],PlayerInventory[playerid][zombiesBlood],
                                PlayerInventory[playerid][instruments],PlayerInventory[playerid][canister],PlayerInventory[playerid][masterKey],
                                PlayerInventory[playerid][bolt],PlayerInventory[playerid][specialAmmo],PlayerInventory[playerid][metal],
                                PlayerInventory[playerid][wood],PlayerInventory[playerid][blanks],PlayerInventory[playerid][drugs],
                                PlayerInventory[playerid][artefacts],PlayerInventory[playerid][fruits],PlayerInventory[playerid][pumpkin],
                                PlayerInventory[playerid][croup],PlayerInventory[playerid][milk],PlayerInventory[playerid][egg]);
                                SPD(playerid,DIALOG_MENU_INVENTORY,DIALOG_STYLE_MSGBOX,"{FFCC00}Рюкзак",string,"Назад","Выйти");
                            }
                        }
                    }
                    case 1:
                    {
                        format(string,sizeof(string),"{FFFFFF}Позывной\t\t%s\nУровень допуска\t%i\nГруппировка\t\t%s\nЗвание\t\t\t%s\nОпыт\t\t\t%i\nЗамечания\t\t%i\n\
                        Клан\t\t\t%s\nДонат очки\t\t%i\nЧасы в зоне\t\t%i\nVIP уровень\t\t%i",
                        PlayerInfo[playerid][nickName],PlayerInfo[playerid][accessLevel],memberName(playerid),
                        rankName(playerid),PlayerInfo[playerid][expLevel],PlayerInfo[playerid][warnLevel],
                        PlayerInfo[playerid][clan],PlayerInfo[playerid][donateLevel],
                        PlayerInfo[playerid][hoursLevel],PlayerInfo[playerid][vipLevel]);
                        SPD(playerid,DIALOG_MENU_INVENTORY,DIALOG_STYLE_MSGBOX,"{FFCC00}Досье",string,"Назад","Выйти");
                    }
                }
            }
        }
        case DIALOG_MENU_INVENTORY:
        {
            if (response)
            {
                SPD(playerid,DIALOG_MENU,DIALOG_STYLE_LIST,"{FFCC00}Рюкзак","{FFFFFF}Содержимое\nДосье\n\
                    Навыки\nЗаписки\nНавигатор\n\nИспользовать аптечку\n\
                    Использовать антирадин\nВыпить алкоголь\nПоесть\nПокурить\nШирнуться\n\n\
                    Использовать рем. набор\nИспользовать канистру\n\nПритвориться мирным\nАктивировать артефакт\n\
                    Спец. пт\nСтиль боя\nПангит","Выбрать","Выйти");
            }
            else return true;
        }
        case DIALOG_FIRSTJOB_HIRE:
        {
            if (response)
            {
                firstJobCP[playerid][0] = CreateDynamicCP(2825.9094,-2458.9521,12.0945, 2.0, -1, -1, -1, 500.0);
                working[playerid] = true;
            }
        }
        case DIALOG_FIRSTJOB_FIRE:
        {
            if (response) 
            {
                for (new k=0;k<10;k++) DestroyDynamicCP(firstJobCP[playerid][k]);
                working[playerid] = false;
            }
        }
    }
    return true;
}
public timerKick(playerid) return Kick(playerid);

public OnPlayerPickUpPickup(playerid, pickupid)
{
    if (pickupid == firstJobPick) 
    {
        if (working[playerid]) SPD(playerid,DIALOG_FIRSTJOB_FIRE,DIALOG_STYLE_MSGBOX,"{FFCC00}Первая работа","Уволиться?","Да","Нет");
        else SPD(playerid,DIALOG_FIRSTJOB_HIRE,DIALOG_STYLE_MSGBOX,"{FFCC00}Первая работа","Возьми, заебал","Принять","Отклонить");
    }
    return true;   
}

public OnPlayerEnterDynamicCP(playerid,checkpointid)
{
    if (checkpointid == firstJobCP[playerid][0])
    {
        TogglePlayerDynamicCP(playerid, firstJobCP[playerid][0], false);
        ClearAnimations(playerid);
        ApplyAnimation(playerid, "CARRY", "PUTDWN", 4.0, 0, 0, 0, 0, 0);
        ApplyAnimation(playerid,"CARRY","crry_prtial",4.1,0,1,1,1,1);
        new object = random(2)+0;
        switch(object)
        {
            case 0: SetPlayerAttachedObject(playerid,5,1221,1,0.135011,0.463495,-0.024351,357.460632,87.350753,88.068374,0.434164,0.491270,0.368655); 
            case 1: SetPlayerAttachedObject(playerid,5,2226,1,0.000708,0.356461,0.000000,186.670364,87.529838,0.000000,1.000000,1.000000,1.000000);
            case 2: SetPlayerAttachedObject(playerid,5,1750,1,0.013829,0.131155,0.145773,185.651550,86.201354,345.922180,0.693442,0.873942,0.577291);
        }
        new cord = random(sizeof(firstJobPoints));
        firstJobCP[playerid][cord] = CreateDynamicCP(firstJobPoints[cord][0],firstJobPoints[cord][1],firstJobPoints[cord][2], 2.0, -1, -1, -1, 500.0);
    }
    else if (checkpointid == firstJobCP[playerid][1] || checkpointid == firstJobCP[playerid][2] || checkpointid == firstJobCP[playerid][3] ||
    checkpointid == firstJobCP[playerid][4] || checkpointid == firstJobCP[playerid][5] || checkpointid == firstJobCP[playerid][6] ||
    checkpointid == firstJobCP[playerid][7] || checkpointid == firstJobCP[playerid][8] || checkpointid == firstJobCP[playerid][9])
    {
        RemovePlayerAttachedObject(playerid,5);
        ApplyAnimation(playerid,"PED","IDLE_tired",4.1,0,1,1,0,1);
        new string[20];
        format(string,sizeof(string),"[Работа] +$%i",random(30)); 
        SCM(playerid,COLOR_SYSTEM,string);
        for (new k=0;k<10;k++) TogglePlayerDynamicCP(playerid, firstJobCP[playerid][k], false);
        TogglePlayerDynamicCP(playerid, firstJobCP[playerid][0], true);
    }
    return true;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    if (PlayerInfo[playerid][adminLevel] > 0) SetPlayerPosFindZ(playerid, fX, fY, fZ);
    return true;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(clickedid == registration[2])
    {
        for(new i = 0; i < sizeof(registration); i++) TextDrawHideForPlayer(playerid, registration[i]);
        SPD(playerid, DIALOG_SEX, DIALOG_STYLE_MSGBOX, "{FFCC00}Регистрация", "\n{FFFFFF}Выберите пол Вашего персонажа:\n\n", "Мужской", "Женский");
    }
    if(clickedid == registration[3])
    {
        for(new i = 0; i < sizeof(registration); i++) TextDrawHideForPlayer(playerid, registration[i]);
        SPD(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "{FFCC00}Регистрация", "\n{FFFFFF}Введите пароль:\n\n", "Далее", "Выйти");
    }
    if(clickedid == registration[4])
    {
        for(new i = 0; i < sizeof(registration); i++) TextDrawHideForPlayer(playerid, registration[i]);
        SPD(playerid, DIALOG_AGE, DIALOG_STYLE_INPUT, "{FFCC00}Регистрация", "\n{FFFFFF}Введите возраст Вашего персонажа:\n\n", "Далее", "Выйти");
    }
    if(clickedid == registration[5])
    {
        for(new i = 0; i < sizeof(registration); i++) TextDrawHideForPlayer(playerid, registration[i]);
        SPD(playerid, DIALOG_EMAIL, DIALOG_STYLE_INPUT, "{FFCC00}Регистрация", "\n{FFFFFF}Введите адрес Вашей электронной почты:\nЭто может Вам не потерять связь с сервером\n\n", "Далее", "Выйти");
    }
    if(clickedid == registration[7]) Kick(playerid);
    if(clickedid == registration[6])
    {
        if(passWrote[playerid] == 1 && sexWrote[playerid] == 1 && ageWrote[playerid] == 1 && mailWrote[playerid] == 1)
        {
            firstSkin(playerid);
            } else {
            TextDrawShowForPlayer(playerid, registration[8]);
        }
    }
    if(clickedid == login[playerid][3])
    {
        for(new i = 0; i < 7; i++) TextDrawHideForPlayer(playerid, login[playerid][i]);
        SPD(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "{FFCC00}Авторизация", "\n\n\t\t{FFFFFF}Введите Ваш пароль:\n\n", "Далее", "Выйти");
    }
    if(clickedid == login[playerid][4])
    {
        for(new i = 0; i < 7; i++) TextDrawHideForPlayer(playerid, login[playerid][i]);
        SPD(playerid, DIALOG_FORUM, DIALOG_STYLE_MSGBOX, "{FFCC00}Сайт", "\n\n\t\t{FFFFFF}Наш сайт:\n\n\t\tServer.exe\n\n", "Назад", "");
    }
    if(clickedid == login[playerid][5])
    {
        for(new i = 0; i < 7; i++) TextDrawHideForPlayer(playerid, login[playerid][i]);
        SPD(playerid, DIALOG_CREDITS, DIALOG_STYLE_MSGBOX, "{FFCC00}Создатели", "\n\n\t{FFFFFF}Copyright (c) Server.exe 2019\n\n", "Назад", "");
    }
    if(clickedid == tSkin[0])
    {
        cskin[playerid]--;
        if(PlayerInfo[playerid][sex] == 1)
        {
            if(cskin[playerid] < 0) cskin[playerid] = 2;
            SetPlayerSkin(playerid, skinMale[cskin[playerid]][0]);
            PlayerInfo[playerid][skin] = skinMale[cskin[playerid]][0];
        }
        else if(PlayerInfo[playerid][sex] == 2)
        {
            if(cskin[playerid] < 0) cskin[playerid] = 2;
            SetPlayerSkin(playerid, skinFemale[cskin[playerid]][0]);
            PlayerInfo[playerid][skin] = skinFemale[cskin[playerid]][0];
        }
    }
    if(clickedid == tSkin[1])
    {
        cskin[playerid] ++;
        if(PlayerInfo[playerid][sex] == 1)
        {
            if(cskin[playerid] > 2) cskin[playerid] = 0;
            SetPlayerSkin(playerid, skinMale[cskin[playerid]][0]);
            PlayerInfo[playerid][skin] = skinMale[cskin[playerid]][0];
        }
        else if(PlayerInfo[playerid][sex] == 2)
        {
            if(cskin[playerid] > 2) cskin[playerid] = 0;
            SetPlayerSkin(playerid, skinFemale[cskin[playerid]][0]);
            PlayerInfo[playerid][skin] = skinFemale[cskin[playerid]][0];
        }
    }
    if(clickedid == tSkin[2])
    {
        TextDrawHideForPlayer(playerid, tSkin[0]);
        TextDrawHideForPlayer(playerid, tSkin[1]);
        TextDrawHideForPlayer(playerid, tSkin[2]);
        CancelSelectTextDraw(playerid);
        registerAccount(playerid);
    }
    return true;
}
public registerAccount(playerid)
{
    PlayerInfo[playerid][logged] = true;
    PlayerInfo[playerid][accessLevel] = 0;
    PlayerInfo[playerid][adminLevel] = 0;
    PlayerInfo[playerid][money] = 5000;
    PlayerInfo[playerid][deaths] = 0;
    PlayerInfo[playerid][centsLevel] = 50;
    PlayerInfo[playerid][wrongPass] = 0;
    PlayerInfo[playerid][member] = 0;
    PlayerInfo[playerid][rank] = 0;
    PlayerInfo[playerid][expLevel] = 0;
    PlayerInfo[playerid][radiation] = 0;
    PlayerInfo[playerid][hunger] = 0;
    PlayerInfo[playerid][thirst] = 0;
    PlayerInfo[playerid][virus] = 0;
    PlayerInfo[playerid][ban] = 0;
    PlayerInfo[playerid][mute] = 0;
    PlayerInfo[playerid][jail] = 0;
    PlayerInfo[playerid][health] = 100;
    PlayerInfo[playerid][warnLevel] = 0;
    PlayerInfo[playerid][donateLevel] = 0;
    PlayerInfo[playerid][vipLevel] = 0;
    PlayerInfo[playerid][hoursLevel] = 0;
    ResetPlayerMoney(playerid);
    new queryPlayerInfo[300];
    mysql_format(connection, queryPlayerInfo, sizeof(queryPlayerInfo), "INSERT INTO `account`\
    (`Name`, `Password`, `Email`, `Admin`, `Sex`, `Age`,`Money`, `Skin`, `Kill`, `Death`, `Cents`, `NickName`, `Clan`)\
     VALUES ('%e','%e','%e','%d','%d','%d','%d','%d','%d','%d','%d','Нет','Нет')",
    playerName(playerid),
    PlayerInfo[playerid][password],
    PlayerInfo[playerid][mail],
    PlayerInfo[playerid][adminLevel],
    PlayerInfo[playerid][sex],
    PlayerInfo[playerid][age],
    PlayerInfo[playerid][money],
    PlayerInfo[playerid][skin],
    PlayerInfo[playerid][kills],
    PlayerInfo[playerid][deaths],
    PlayerInfo[playerid][centsLevel]);
    mysql_tquery(connection, queryPlayerInfo, "", "d", playerid);
    registerInventory(playerid);
    return true;
}
public registerInventory(playerid)
{
    new queryPlayerInventory[256];
    PlayerInventory[playerid][antiRadin] = 0;
    PlayerInventory[playerid][alcohol] = 0;
    PlayerInventory[playerid][weed] = 0;
    PlayerInventory[playerid][food] = 0;
    PlayerInventory[playerid][medcine] = 0;
    PlayerInventory[playerid][zombiesBlood] = 0;
    PlayerInventory[playerid][instruments] = 0;
    PlayerInventory[playerid][canister] = 0;
    PlayerInventory[playerid][masterKey] = 0;
    PlayerInventory[playerid][bolt] = 0;
    PlayerInventory[playerid][specialAmmo] = 0;
    PlayerInventory[playerid][metal] = 0;
    PlayerInventory[playerid][wood] = 0;
    PlayerInventory[playerid][blanks] = 0;
    PlayerInventory[playerid][drugs] = 0;
    PlayerInventory[playerid][flesh] = 0;
    PlayerInventory[playerid][artefacts] = 0;
    PlayerInventory[playerid][fruits] = 0;
    PlayerInventory[playerid][pumpkin] = 0;
    PlayerInventory[playerid][croup] = 0;
    PlayerInventory[playerid][milk] = 0;
    PlayerInventory[playerid][egg] = 0;
    mysql_format(connection, queryPlayerInventory, sizeof(queryPlayerInventory), "INSERT INTO `inventory` (`Name`) VALUES ('%e')",
    playerName(playerid));
    mysql_tquery(connection, queryPlayerInventory, "", "d", playerid);
    SetPlayerScore(playerid, PlayerInfo[playerid][accessLevel]);
    GivePlayerMoney(playerid, PlayerInfo[playerid][money]);
    SetSpawnInfo(playerid, 0, PlayerInfo[playerid][skin], 613.1469,-1415.0784,13.5688, 269.15, 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid);
    hideTextDraw(playerid);
    return true;
}
public loadAccount(playerid)
{
    new strMail[32],strNick[20],strClan[20];
    cache_get_value_name(0, "Email", strMail);
    format(PlayerInfo[playerid][mail], sizeof(strMail), "%s", strMail);
    cache_get_value_name_int(0, "Admin",PlayerInfo[playerid][adminLevel]);
    cache_get_value_name_int(0, "Sex",PlayerInfo[playerid][sex]);
    cache_get_value_name_int(0, "Age",PlayerInfo[playerid][age]);
    cache_get_value_name_int(0, "Money",PlayerInfo[playerid][money]);
    cache_get_value_name_int(0, "Skin",PlayerInfo[playerid][skin]);
    cache_get_value_name_int(0, "Kill",PlayerInfo[playerid][kills]);
    cache_get_value_name_int(0, "Death",PlayerInfo[playerid][deaths]);
    cache_get_value_name_int(0, "Cents",PlayerInfo[playerid][centsLevel]);
    cache_get_value_name(0, "NickName", strNick);
    format(PlayerInfo[playerid][nickName], sizeof(strNick), "%s", strNick);
    cache_get_value_name(0, "Clan", strClan);
    format(PlayerInfo[playerid][clan], sizeof(strClan), "%s", strClan);
    cache_get_value_name_int(0, "Member",PlayerInfo[playerid][member]);
    cache_get_value_name_int(0, "Rank",PlayerInfo[playerid][rank]);
    cache_get_value_name_int(0, "AccessLevel",PlayerInfo[playerid][accessLevel]);
    cache_get_value_name_int(0, "ExpLevel",PlayerInfo[playerid][expLevel]);
    cache_get_value_name_int(0, "Radiation",PlayerInfo[playerid][radiation]);
    cache_get_value_name_int(0, "Hunger",PlayerInfo[playerid][hunger]);
    cache_get_value_name_int(0, "Thirst",PlayerInfo[playerid][thirst]);
    cache_get_value_name_int(0, "Virus",PlayerInfo[playerid][virus]);
    cache_get_value_name_int(0, "Ban",PlayerInfo[playerid][ban]);
    cache_get_value_name_int(0, "Mute",PlayerInfo[playerid][mute]);
    cache_get_value_name_int(0, "Jail",PlayerInfo[playerid][jail]);
    cache_get_value_name_float(0, "Health",PlayerInfo[playerid][health]);
    cache_get_value_name_int(0, "WarnLevel",PlayerInfo[playerid][warnLevel]);
    cache_get_value_name_int(0, "DonateLevel",PlayerInfo[playerid][donateLevel]);
    cache_get_value_name_int(0, "VipLevel",PlayerInfo[playerid][vipLevel]);
    cache_get_value_name_int(0, "HoursLevel",PlayerInfo[playerid][hoursLevel]);
    PlayerInfo[playerid][logged] = true;
    TogglePlayerSpectating(playerid, false);
    SetPlayerScore(playerid, PlayerInfo[playerid][accessLevel]);
    GivePlayerMoney(playerid, PlayerInfo[playerid][money]);
    SetSpawnInfo(playerid, 1, PlayerInfo[playerid][skin], 613.1469,-1415.0784,13.5688, 269.15, 0, 0, 0, 0, 0, 0);
    if (PlayerInfo[playerid][ban] > 0)
    {
        new stringBan[128];
        format(stringBan,sizeof(stringBan),"Срок действия бана закончится через %i час(-a/-ов)",PlayerInfo[playerid][ban]);
        SCM(playerid,COLOR_SYSTEM,stringBan);
        Kick(playerid);
    }
    new queryInventory[512];
    mysql_format(connection, queryInventory, sizeof(queryInventory), "SELECT * FROM `inventory` WHERE Name = '%e'", playerName(playerid));
    mysql_tquery(connection, queryInventory, "loadInventory", "d", playerid);
    if (PlayerInfo[playerid][adminLevel] > 0) Iter_Add(admins,playerid);
    if (PlayerInfo[playerid][member] == 3 || PlayerInfo[playerid][member] == 6 || PlayerInfo[playerid][member] == 10) Iter_Add(governments,playerid);
    if (PlayerInfo[playerid][member] == 2 || PlayerInfo[playerid][member] == 8) Iter_Add(stalkers,playerid);
    if (PlayerInfo[playerid][member] == 4 || PlayerInfo[playerid][member] == 9) Iter_Add(bandits,playerid);
    if (PlayerInfo[playerid][member] == 1 || PlayerInfo[playerid][member] == 7) Iter_Add(monolites,playerid);
    SpawnPlayer(playerid);
    hideTextDraw(playerid);
    return true;
}
public loadInventory(playerid)
{
    cache_get_value_name_int(0, "AntiRadin", PlayerInventory[playerid][antiRadin]);
    cache_get_value_name_int(0, "Alcohol",PlayerInventory[playerid][alcohol]);
    cache_get_value_name_int(0, "Weed",PlayerInventory[playerid][weed]);
    cache_get_value_name_int(0, "Food",PlayerInventory[playerid][food]);
    cache_get_value_name_int(0, "Medcine",PlayerInventory[playerid][medcine]);
    cache_get_value_name_int(0, "ZombiesBlood",PlayerInventory[playerid][zombiesBlood]);
    cache_get_value_name_int(0, "Instruments",PlayerInventory[playerid][instruments]);
    cache_get_value_name_int(0, "Canister",PlayerInventory[playerid][canister]);
    cache_get_value_name_int(0, "MasterKey",PlayerInventory[playerid][masterKey]);
    cache_get_value_name_int(0, "Bolt",PlayerInventory[playerid][bolt]);
    cache_get_value_name_int(0, "SpecialAmmo",PlayerInventory[playerid][specialAmmo]);
    cache_get_value_name_int(0, "Metal",PlayerInventory[playerid][metal]);
    cache_get_value_name_int(0, "Wood",PlayerInventory[playerid][wood]);
    cache_get_value_name_int(0, "Blanks",PlayerInventory[playerid][blanks]);
    cache_get_value_name_int(0, "Drugs",PlayerInventory[playerid][drugs]);
    cache_get_value_name_int(0, "Flesh",PlayerInventory[playerid][flesh]);
    cache_get_value_name_int(0, "Fruits",PlayerInventory[playerid][fruits]);
    cache_get_value_name_int(0, "Pumpkin",PlayerInventory[playerid][pumpkin]);
    cache_get_value_name_int(0, "Croup",PlayerInventory[playerid][croup]);
    cache_get_value_name_int(0, "Milk",PlayerInventory[playerid][milk]);
    cache_get_value_name_int(0, "Egg",PlayerInventory[playerid][egg]);
    cache_get_value_name_int(0, "Artefacts",PlayerInventory[playerid][artefacts]);
    return true;
}
public saveAccount(playerid)
{
    if (PlayerInfo[playerid][logged] == false) return true;
    PlayerInfo[playerid][money] = GetPlayerMoney(playerid);
    new Float: healthLevel;
    GetPlayerHealth(playerid, healthLevel);
    ResetPlayerMoney(playerid);
    new queryPlayerInfo[2048];
    static str[] = "UPDATE `account` SET `Name`='%e', `Password`='%e', `Email`='%e', `Admin`=%d, `Sex`=%d, `Age`=%d,`Money`=%d, `Skin`=%d, `Kill`=%d,`Death`=%d, \
    `Cents`=%d,`NickName`='%e',`Member`=%d,`Rank`=%d,`WarnLevel`=%d,`Clan`='%e',`DonateLevel`=%d,`VipLevel`=%d,`HoursLevel`=%d,`AccessLevel`=%d,`ExpLevel`=%d, \
    `Radiation`=%d, `Hunger`=%d, `Thirst`=%d, `Virus`=%d, `Health`=%f,`Ban`=%d,`Mute`=%d,`Jail`=%d WHERE `Name`='%e'";
    mysql_format(connection, queryPlayerInfo, sizeof(queryPlayerInfo), str,playerName(playerid),PlayerInfo[playerid][password],PlayerInfo[playerid][mail],PlayerInfo[playerid][adminLevel],
    PlayerInfo[playerid][sex],PlayerInfo[playerid][age],PlayerInfo[playerid][money],
    PlayerInfo[playerid][skin],PlayerInfo[playerid][kills],PlayerInfo[playerid][deaths],PlayerInfo[playerid][centsLevel],
    PlayerInfo[playerid][nickName],PlayerInfo[playerid][member],
    PlayerInfo[playerid][rank],PlayerInfo[playerid][warnLevel],
    PlayerInfo[playerid][clan],PlayerInfo[playerid][donateLevel],PlayerInfo[playerid][vipLevel],PlayerInfo[playerid][hoursLevel],
    PlayerInfo[playerid][accessLevel],PlayerInfo[playerid][expLevel],PlayerInfo[playerid][radiation],PlayerInfo[playerid][hunger],
    PlayerInfo[playerid][thirst],PlayerInfo[playerid][virus],healthLevel,
    PlayerInfo[playerid][ban],PlayerInfo[playerid][mute],PlayerInfo[playerid][jail],playerName(playerid));
    mysql_tquery(connection, queryPlayerInfo, "", "d", playerid);
    new queryPlayerInventory[1024];
    mysql_format(connection, queryPlayerInventory, sizeof(queryPlayerInventory), "UPDATE `inventory` SET `AntiRadin`=%d, \
    `Alcohol`=%d, `Weed`=%d, `Food`=%d, `Medcine`=%d, `ZombiesBlood`=%d, `Instruments`=%d, `Canister`=%d, `MasterKey`=%d \
    , `Bolt`=%d, `SpecialAmmo`=%d, `Metal`=%d,`Wood`=%d, `Blanks`=%d, `Drugs`=%d, `Flesh`=%d, `Fruits`=%d \
    , `Pumpkin`=%d, `Croup`=%d, `Milk`=%d, `Egg`=%d, `Artefacts`=%d WHERE `Name`='%e'",
    PlayerInventory[playerid][antiRadin],PlayerInventory[playerid][alcohol],PlayerInventory[playerid][weed],PlayerInventory[playerid][food],
    PlayerInventory[playerid][medcine],PlayerInventory[playerid][zombiesBlood],PlayerInventory[playerid][instruments],
    PlayerInventory[playerid][canister],PlayerInventory[playerid][masterKey],PlayerInventory[playerid][bolt],
    PlayerInventory[playerid][specialAmmo],PlayerInventory[playerid][metal],PlayerInventory[playerid][wood],PlayerInventory[playerid][blanks],
    PlayerInventory[playerid][drugs],PlayerInventory[playerid][flesh],PlayerInventory[playerid][fruits],
    PlayerInventory[playerid][pumpkin],PlayerInventory[playerid][croup],PlayerInventory[playerid][milk],
    PlayerInventory[playerid][egg],PlayerInventory[playerid][artefacts],playerName(playerid));
    mysql_tquery(connection, queryPlayerInventory, "", "d", playerid);
    PlayerInfo[playerid][logged] = false;
    return true;
}

public loadNewsTD(playerid)
{
    new cent,strCent[8];
    cache_get_value_name_int(0, "Cents", cent);
    format(strCent, sizeof(strCent), " .%i", cent);
    TextDrawSetString(cents[playerid][0], strCent);
    TextDrawShowForPlayer(playerid,cents[playerid][0]);
    new radiationTD,strRadiation[8];
    cache_get_value_name_int(0, "Radiation", radiationTD);
    format(strRadiation, sizeof(strRadiation), "%i%", radiationTD);
    TextDrawSetString(mainText[playerid][1], strRadiation);
    new hungerTD,strHunger[8];
    cache_get_value_name_int(0, "Hunger", hungerTD);
    format(strHunger, sizeof(strHunger), "%i%", hungerTD);
    TextDrawSetString(mainText[playerid][3], strHunger);
    new thirstTD,strThirst[8];
    cache_get_value_name_int(0, "Thirst", thirstTD);
    format(strThirst, sizeof(strThirst), "%i%", thirstTD);
    TextDrawSetString(mainText[playerid][5], strThirst);
    new virusTD,strVirus[8];
    cache_get_value_name_int(0, "Virus", virusTD);
    format(strVirus, sizeof(strVirus), "%i%", virusTD);
    TextDrawSetString(mainText[playerid][7], strVirus);
    for (new i = 0; i < 8; i++) TextDrawShowForPlayer(playerid,mainText[playerid][i]);
    return true;
}

public OnCheatDetected(playerid, ip_address[], type, code)
{
    if (type) BlockIpAddress(ip_address, 0);
    else if (PlayerInfo[playerid][adminLevel] < 0)
    {
        switch(code)
        {
            case 5, 6, 11, 22: return 1;
            case 14:
            {
                new a = AntiCheatGetMoney(playerid);
                ResetPlayerMoney(playerid);
                GivePlayerMoney(playerid, a);
                return true;
            }
            case 32:
            {
                if (PlayerInfo[playerid][adminLevel] > 0) return true;
                new Float:x, Float:y, Float:z;
                AntiCheatGetPos(playerid, x, y, z);
                SetPlayerPos(playerid, x, y, z);
                return 1;
            }
            case 40: SendClientMessage(playerid, -1, MAX_CONNECTS_MSG);
            case 41: SendClientMessage(playerid, -1, UNKNOWN_CLIENT_MSG);
            default:
            {
                if (PlayerInfo[playerid][adminLevel] > 0) return true;
                new strtmp[sizeof KICK_MSG];
                format(strtmp, sizeof strtmp, KICK_MSG, code);
                SCM(playerid, -1, strtmp);
            }
        }
        AntiCheatKickWithDesync(playerid, code);
    }
    return 1;
}

public IsPlayerInArea()
{
    new Float:x, Float:y, Float:z, Float: hp, str[8];
    foreach(Player, i)
    {
        if (PlayerInfo[i][mute] > 0) 
        {
            new string[64];
            format(string,sizeof(string),"До конца срока действия бана чата осталось: %i мин",PlayerInfo[i][mute]);
            SCM(i,COLOR_SYSTEM,string);
            PlayerInfo[i][mute]--;
        }
        if (PlayerInfo[i][jail] > 0) 
        {
            new string[64];
            format(string,sizeof(string),"До конца срока пребывания в КПЗ осталось: %i мин",PlayerInfo[i][jail]);
            SCM(i,COLOR_SYSTEM,string);
            PlayerInfo[i][jail]--;
        }
        if (PlayerInfo[i][jail] > 0) PlayerInfo[i][jail]--;
        if (PlayerInfo[i][hunger] <= 0 || PlayerInfo[i][hunger] <= 0 && PlayerInfo[i][thirst] <= 0 ||
        PlayerInfo[i][radiation] >= 100 && PlayerInfo[i][hunger] <= 0 || PlayerInfo[i][hunger] <= 0 && PlayerInfo[i][thirst] <= 0 ||
        PlayerInfo[i][radiation] >= 100 && PlayerInfo[i][hunger] <= 0 && PlayerInfo[i][thirst] <= 0)
        {
            PlayerInfo[i][hunger] = 30;
            SetPlayerHealth(i,0);
            SCM(i,COLOR_RED,"Вы погибли с голоду");
            format(str,sizeof(str),"%i",PlayerInfo[i][radiation]);
            TextDrawSetString(mainText[i][1], str);
            format(str,sizeof(str),"%i",PlayerInfo[i][hunger]);
            TextDrawSetString(mainText[i][3], str);
            format(str,sizeof(str),"%i",PlayerInfo[i][thirst]);
            TextDrawSetString(mainText[i][5], str);
            format(str,sizeof(str),"%i",PlayerInfo[i][virus]);
            TextDrawSetString(mainText[i][7], str);
            continue;
        }
        if (PlayerInfo[i][thirst] <= 0)
        {
            PlayerInfo[i][thirst] = 30;
            SetPlayerHealth(i,0);
            SCM(i,COLOR_RED,"Вы погибли от жажды");
            format(str,sizeof(str),"%i",PlayerInfo[i][radiation]);
            TextDrawSetString(mainText[i][1], str);
            format(str,sizeof(str),"%i",PlayerInfo[i][hunger]);
            TextDrawSetString(mainText[i][3], str);
            format(str,sizeof(str),"%i",PlayerInfo[i][thirst]);
            TextDrawSetString(mainText[i][5], str);
            format(str,sizeof(str),"%i",PlayerInfo[i][virus]);
            TextDrawSetString(mainText[i][7], str);
            continue;
        }
        if (PlayerInfo[i][radiation] >= 100)
        {
            PlayerInfo[i][radiation] = 30;
            SetPlayerHealth(i,0);
            SCM(i,COLOR_RED,"Вы погибли от радиации");
            format(str,sizeof(str),"%i",PlayerInfo[i][radiation]);
            TextDrawSetString(mainText[i][1], str);
            format(str,sizeof(str),"%i",PlayerInfo[i][hunger]);
            TextDrawSetString(mainText[i][3], str);
            format(str,sizeof(str),"%i",PlayerInfo[i][thirst]);
            TextDrawSetString(mainText[i][5], str);
            format(str,sizeof(str),"%i",PlayerInfo[i][virus]);
            TextDrawSetString(mainText[i][7], str);
            continue;
        }
        PlayerInfo[i][hunger] -= 1;
        PlayerInfo[i][thirst] -= 1;   
        GetPlayerPos(i, x, y, z);
        GetPlayerHealth(i, hp);
        if ((x >= 125 && x <= 2929.6875 && y >= -1916.015625 && y <= -415.0625) ||
        (x >= -2433.59375 && x <= -1910.15625 && y >= -2617.1875 && y <= -2167.96875) ||
        (x >= -1904.296875 && x <= -1517.578125 && y >= -650.390625 && y <= 1505.859375) ||
        (x >= 74.21875 && x <= 433.59375 && y >= -300.78125 && y <= 82.03125) ||
        (x >= 1132.8125 && x <= 1433.59375 && y >= 101.5625 && y <= 453.125) ||
        (x >= 2156.25 && x <= 2593.75 && y >= -164.0625 && y <= 164.0625) ||
        (x >= 925.78125 && x <= 2929.6875 && y >= 550.78125 && y <= 2923.828125))
        {
            hp -= 5.0;
            PlayerInfo[i][radiation] += 3;
            PlayerInfo[i][hunger] -= 2;
            PlayerInfo[i][thirst] -= 4;
            PlayerInfo[i][health] = hp;
            SetPlayerHealth(i, hp);
        } 
        else if ((x >= -1517.578125 && x <= 925.78125 && y >= 550.78125 && y <= 2923.828125) ||
        (x >= -2982.421875 && x <= -1505.859375 && y >= 1505.859375 && y <= 2923.828125))
        {
            PlayerInfo[i][radiation] += 1;
            PlayerInfo[i][hunger] -= 1;
            PlayerInfo[i][thirst] -= 2;
        }
        format(str,sizeof(str),"%i",PlayerInfo[i][radiation]);
        TextDrawSetString(mainText[i][1], str);
        format(str,sizeof(str),"%i",PlayerInfo[i][hunger]);
        TextDrawSetString(mainText[i][3], str);
        format(str,sizeof(str),"%i",PlayerInfo[i][thirst]);
        TextDrawSetString(mainText[i][5], str);
        format(str,sizeof(str),"%i",PlayerInfo[i][virus]);
        TextDrawSetString(mainText[i][7], str); 
    }
}
//Функции скрипта

stock playerName(playerid)
{
    new name[MAX_PLAYER_NAME];
    return(GetPlayerName(playerid, name, sizeof(name)), name);
}

stock playerIp(playerid)
{
    new ip[16];
    GetPlayerIp(playerid, ip, sizeof(ip));
    return ip;
}

stock hideTextDraw(playerid)
{
    CancelSelectTextDraw(playerid);
    for (new i = 0; i < sizeof(registration); i++) TextDrawHideForPlayer(playerid, registration[i]);
    for (new i = 0; i < sizeof(background); i++) TextDrawHideForPlayer(playerid, background[i]);
    for (new i = 0; i < 7; i++) TextDrawHideForPlayer(playerid, login[playerid][i]);
    return true;
}

stock memberName(playerid)
{
    new string[32];
    switch(PlayerInfo[playerid][member])
    {
        case 1: string = "Мутанты";
        case 2: string = "Сталкеры";
        case 3: string = "Военные";
        case 4: string = "Бандиты";
        case 5: string = "Псионная разведка";
        case 6: string = "Долг";
        case 7: string = "Монолит";
        case 8: string = "Свобода";
        case 9: string = "Наемники";
        case 10: string = "Ученые";
        default: string = "Нет";
    }
    return string;
}

stock adminRankName(playerid)
{
    new string[32];
    switch (PlayerInfo[playerid][adminLevel])
    {
        case 1: string = "Стажёр";
		case 2: string = "Офицер разведки";
		case 3: string = "Ст. Офицер разведки";
		case 4: string = "Зам. Директора псиопс";
		case 5: string = "Директор";
		case 6: string = "Диктатор";
    }
    return string;
}

stock rankName(playerid)
{
	new string[32];
	switch(PlayerInfo[playerid][member])
	{
        case 0: string = "Прохожий";
		case 1:
		{
			switch(PlayerInfo[playerid][rank])
			{
			 	case 1..3: string = "Заражённый";
				case 4..6: string = "Зомби";
				case 7..9: string = "Мутант";
				case 10..12: string = "Кровосос";
				case 13..14: string = "Контролёр";
                case 15: string = "Супермутант";
				case 16: string = "Люцифер";
			}
		}
		case 2:
		{
			switch(PlayerInfo[playerid][rank])
			{
			 	case 1: string = "Новичок";
			 	case 2: string = "Копатель";
		 		case 3: string = "Искатель";
				case 4: string = "Опытный";
				case 5: string = "Мастер";
				case 6: string = "Бывалый";
				case 7: string = "Адепт";
				case 8: string = "Следопыт";
				case 9: string = "Проводник";
				case 10: string = "Картограф";
				case 11: string = "Знаток";
				case 12: string = "Эксперт";
				case 13: string = "Наставник";
				case 14: string = "Гуру";
				case 15: string = "Оркаул";
				case 16: string = "Корифей";
			}
		}
		case 3:
		{
			switch(PlayerInfo[playerid][rank])
			{
			 	case 1: string = "Рядовой";
			 	case 2: string = "Ефрейтор";
				case 3: string = "Мл. Сержант";
		 		case 4: string = "Сержант";
				case 5: string = "Ст. Сержант";
				case 6: string = "Старшина";
				case 7: string = "Прапорщик";
				case 8: string = "Мл. Лейтенант";
				case 9: string = "Лейтенант";
				case 10: string = "Ст. Лейтенант";
				case 11: string = "Капитан";
				case 12: string = "Майор";
				case 13: string = "Подполковник";
				case 14: string = "Полковник";
				case 15: string = "Генерал";
				case 16: string = "Маршал";
			}
		}
		case 4:
		{
			switch(PlayerInfo[playerid][rank])
			{
			 	case 1: string = "Панк";
			 	case 2: string = "Хулиган";
		 		case 3: string = "Байкер";
				case 4: string = "Головорез";
				case 5: string = "Громила";
				case 6: string = "Вышибала";
				case 7: string = "Бригадир";
				case 8: string = "Вор";
				case 9: string = "Рейдер";
				case 10: string = "Налётчик";
				case 11: string = "Мародёр";
				case 12: string = "Разбойник";
				case 13: string = "Барыга";
				case 14: string = "Законник";
				case 15: string = "Авторитет";
				case 16: string = "Пахан";
			}
		}
		case 5:
		{
			switch(PlayerInfo[playerid][rank])
			{
			 	case 1: string = "Стажёр";
			 	case 2: string = "Офицер разведки";
		 		case 3: string = "Ст. Офицер разведки";
				case 4: string = "Зам. Директора псиопс";
				case 5: string = "Директор";
				case 6: string = "Диктатор";				
			}
		}
		case 6:
		{
			switch(PlayerInfo[playerid][rank])
			{
			 	case 1: string = "Беркут";
			 	case 2: string = "Капрал";
		 		case 3: string = "Тренер"; 
				case 4: string = "Мастер"; 
				case 5: string = "Ветеран"; 
				case 6: string = "Командир";
			}
		}
		case 7:
		{
			switch(PlayerInfo[playerid][rank])
			{
			 	case 1: string = "Послушник";
			 	case 2: string = "Адепт"; 
		 		case 3: string = "Неофит"; 
				case 4: string = "Предвестник";
				case 5: string = "Апостол"; 
				case 6: string = "О-Сознание";
			}
		}
		 case 8:
		{
			switch(PlayerInfo[playerid][rank])
			{
			 	case 1: string = "Консул";
			 	case 2: string = "Корнет"; 
		 		case 3: string = "Поручик"; 
				case 4: string = "Командор"; 
				case 5: string = "Маршал"; 
				case 6: string = "Командир";
			}
		}
		case 9:
		{
			switch(PlayerInfo[playerid][rank])
			{
			 	case 1: string = "Новобранец";
			 	case 2: string = "Боец"; 
		 		case 3: string = "Ветеран";
				case 4: string = "Мастер"; 
				case 5: string = "Агент"; 
				case 6: string = "Руководитель";
			}
		}
		case 10:
		{
			switch(PlayerInfo[playerid][rank])
			{
			 	case 1: string = "Ассистент"; 
		 		case 2: string = "Бакалавр"; 
				case 3: string = "Магистр"; 
				case 4: string = "Аспирант";
				case 5: string = "Доктор наук"; 
				case 6: string = "Ректор";
			}
		}
	}
	return string;
}
stock getRankSkin(playerid)
{
    switch (PlayerInfo[playerid][member])
    {
        case 1:
        {
            switch(PlayerInfo[playerid][rank])
            {
                case 1..3:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 163;
                    else PlayerInfo[playerid][skin] = 77;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 4..6:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 78;
                    else PlayerInfo[playerid][skin] = 77;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 7..9:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 162;
                    else PlayerInfo[playerid][skin] = 77;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 10..12:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 160;
                    else PlayerInfo[playerid][skin] = 77;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 13..14:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 213;
                    else PlayerInfo[playerid][skin] = 77;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 15:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 62;
                    else PlayerInfo[playerid][skin] = 298;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 16:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 230;
                    else PlayerInfo[playerid][skin] = 298;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }       
            }
        }
        case 2:
        {
            switch(PlayerInfo[playerid][rank])
            {
                case 1..16:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 308;
                    else PlayerInfo[playerid][skin] = 41;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
            }
        }
        case 3:
        {
            switch(PlayerInfo[playerid][rank])
            {
                case 1:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 287;
                    else PlayerInfo[playerid][skin] = 191;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                    
                }
                case 2:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 73;
                    else PlayerInfo[playerid][skin] = 191;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 3..6:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 179;
                    else PlayerInfo[playerid][skin] = 191;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 7..8:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 255;
                    else PlayerInfo[playerid][skin] = 191;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 9:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 282;
                    else PlayerInfo[playerid][skin] = 191;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 10..11:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 282;
                    else PlayerInfo[playerid][skin] = 309;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 12..13:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 302;
                    else PlayerInfo[playerid][skin] = 309;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 14..15:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 311;
                    else PlayerInfo[playerid][skin] = 309;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 16:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 288;
                    else PlayerInfo[playerid][skin] = 309;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
            }
        }
        case 4:
        {
            switch(PlayerInfo[playerid][rank])
            {
                case 1..2:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 181;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 3:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 248;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 4:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 247;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 5:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 100;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 6:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 119;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 7:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 186;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 8:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 121;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 9:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 122;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 10:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 112;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 11:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 126;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 12:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 124;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 13:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 125;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 14:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 120;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 15:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 111;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 16:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 113;
                    else PlayerInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
            }    
        }
        case 6:
        {
            switch(PlayerInfo[playerid][rank])
            {
                case 1..6:
                {
                    PlayerInfo[playerid][skin] = 279;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
            }
        }
        case 7:
        {
            switch(PlayerInfo[playerid][rank])
            {
                case 1..6:
                {
                    PlayerInfo[playerid][skin] = 285;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
            }
        }
        case 8:
        {
            switch(PlayerInfo[playerid][rank])
            {
                case 1..6:
                {
                    PlayerInfo[playerid][skin] = 29;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
            }
        }
        case 9:
        {
            switch(PlayerInfo[playerid][rank])
            {
                case 1:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 98;
                    else PlayerInfo[playerid][skin] = 76;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 2:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 163;
                    else PlayerInfo[playerid][skin] = 76;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 3:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 164;
                    else PlayerInfo[playerid][skin] = 76;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 4:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 166;
                    else PlayerInfo[playerid][skin] = 76;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 5:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 165;
                    else PlayerInfo[playerid][skin] = 76;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 6:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 171;
                    else PlayerInfo[playerid][skin] = 76;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
            }
        }
        case 10:
        {
            switch(PlayerInfo[playerid][rank])
            {
                case 1..5:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 70;
                    else PlayerInfo[playerid][skin] = 91;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
                case 6:
                {
                    if (PlayerInfo[playerid][sex] == 1) PlayerInfo[playerid][skin] = 290;
                    else PlayerInfo[playerid][skin] = 91;
                    SetPlayerSkin(playerid, PlayerInfo[playerid][skin]);
                }
            }
        }
    }
}

stock chat(playerid, Float:range, string[])
{
    new Float: x, Float: y, Float: z;
    GetPlayerPos(playerid, x, y, z);
    foreach(Player, i)
    {
        if(!IsPlayerInRangeOfPoint(i, range, x, y, z)) continue;
        SCM(i, COLOR_RED, string);
    }
    return true;
}

stock sendAdminMessage(color, const string[])
{
    foreach(admins, i) SCM(i, color, string);
    return true;
}
stock sendGovernmentMessage(frac, const string[])
{
    foreach(governments, i) 
    {
        switch (frac) 
        { 
            case 0:
            {
                SCM(i, 0x4682B4FF, string);
                continue;
            }
            case 3:
            {
                if (PlayerInfo[i][member] == 3)
                {
                    SCM(i,0x228B22FF, string);
                    continue;
                }
            }
            case 6:
            {
                if (PlayerInfo[i][member] == 6)
                {
                    SCM(i, 0x228B22FF, string);
                    continue;
                }
            }
            case 10:
            {
                if (PlayerInfo[i][member] == 10)
                {
                    SCM(i, 0x228B22FF, string);
                    continue;
                }
            }
        }       
    }
    return true;
}
stock sendStalkerMessage(frac, const string[])
{
    foreach(stalkers, i) 
    {
        switch (frac) 
        { 
            case 0:
            {
                SCM(i, 0x4682B4FF, string);
                continue;
            }
            case 2:
            {
                if (PlayerInfo[i][member] == 2)
                {
                    SCM(i,0x228B22FF, string);
                    continue;
                }
            }
            case 8:
            {
                if (PlayerInfo[i][member] == 8)
                {
                    SCM(i, 0x228B22FF, string);
                    continue;
                }
            }
        }       
    }
    return true;
}
stock sendBanditMessage(frac, const string[])
{
    foreach(bandits, i) 
    {
        switch (frac) 
        { 
            case 0:
            {
                SCM(i, 0x4682B4FF, string);
                continue;
            }
            case 2:
            {
                if (PlayerInfo[i][member] == 4)
                {
                    SCM(i,0x228B22FF, string);
                    continue;
                }
            }
            case 8:
            {
                if (PlayerInfo[i][member] == 9)
                {
                    SCM(i, 0x228B22FF, string);
                    continue;
                }
            }
        }       
    }
    return true;
}
stock sendMonolitMessage(frac, const string[])
{
    foreach(monolites, i) 
    {
        switch (frac) 
        { 
            case 0:
            {
                SCM(i, 0x4682B4FF, string);
                continue;
            }
            case 1:
            {
                if (PlayerInfo[i][member] == 1)
                {
                    SCM(i,0x228B22FF, string);
                    continue;
                }
            }
            case 7:
            {
                if (PlayerInfo[i][member] == 7)
                {
                    SCM(i, 0x228B22FF, string);
                    continue;
                }
            }
        }       
    }
    return true;
}

stock loadTextDraw()
{
    logo[0] = TextDrawCreate(594.538513, 3.333325, "__Server~n~.exe");
    TextDrawLetterSize(logo[0], 0.400000, 2.078333);
    TextDrawAlignment(logo[0], 2);
    TextDrawColor(logo[0], -1322337281);
    TextDrawSetShadow(logo[0], 1643);
    TextDrawSetOutline(logo[0], -1);
    TextDrawBackgroundColor(logo[0], 255);
    TextDrawFont(logo[0], 2);
    TextDrawSetProportional(logo[0], 1);
    TextDrawSetShadow(logo[0], 1643);
    
    background[0] = TextDrawCreate(197.000000, 2.000000, "#");
    TextDrawBackgroundColor(background[0], 255);
    TextDrawFont(background[0], 2);
    TextDrawLetterSize(background[0], 0.500000, 49.499992);
    TextDrawColor(background[0], -1);
    TextDrawSetOutline(background[0], 0);
    TextDrawSetProportional(background[0], 1);
    TextDrawSetShadow(background[0], 1);
    TextDrawUseBox(background[0], 1);
    TextDrawBoxColor(background[0], 136);
    TextDrawTextSize(background[0], 442.000000, -21.000000);
    TextDrawSetSelectable(background[0], 0);

    background[1] = TextDrawCreate(270.000000, 13.000000, "~r~~h~S~w~erver~r~~h~~r~.~w~exe");
    TextDrawBackgroundColor(background[1], 255);
    TextDrawFont(background[1], 1);
    TextDrawLetterSize(background[1], 0.600000, 2.900000);
    TextDrawColor(background[1], -1);
    TextDrawSetOutline(background[1], 0);
    TextDrawSetProportional(background[1], 1);
    TextDrawSetShadow(background[1], 1);
    TextDrawSetSelectable(background[1], 0);

    background[2] = TextDrawCreate(250.000000, 38.000000, "~w~-");
    TextDrawBackgroundColor(background[2], 255);
    TextDrawFont(background[2], 1);
    TextDrawLetterSize(background[2], 9.510000, 1.000000);
    TextDrawColor(background[2], -1);
    TextDrawSetOutline(background[2], 0);
    TextDrawSetProportional(background[2], 1);
    TextDrawSetShadow(background[2], 1);
    TextDrawSetSelectable(background[2], 0);

    background[3] = TextDrawCreate(197.000000, 97.000000, "#");
    TextDrawBackgroundColor(background[3], 255);
    TextDrawFont(background[3], 2);
    TextDrawLetterSize(background[3], 0.610000, 0.199999);
    TextDrawColor(background[3], -1);
    TextDrawSetOutline(background[3], 0);
    TextDrawSetProportional(background[3], 1);
    TextDrawSetShadow(background[3], 1);
    TextDrawUseBox(background[3], 1);
    TextDrawBoxColor(background[3], 255);
    TextDrawTextSize(background[3], 442.000000, -20.000000);
    TextDrawSetSelectable(background[3], 0);

    background[4] = TextDrawCreate(250.000000, 370.000000, "Copyright (c) Server.exe 2019");
    TextDrawBackgroundColor(background[4], 255);
    TextDrawFont(background[4], 2);
    TextDrawLetterSize(background[4], 0.290000, 1.000000);
    TextDrawColor(background[4], -1);
    TextDrawSetOutline(background[4], 1);
    TextDrawSetProportional(background[4], 1);
    TextDrawSetSelectable(background[4], 0);

    background[5] = TextDrawCreate(193.000000, 2.000000, "#");
    TextDrawBackgroundColor(background[5], 255);
    TextDrawFont(background[5], 2);
    TextDrawLetterSize(background[5], 0.610000, 51.099998);
    TextDrawColor(background[5], -1);
    TextDrawSetOutline(background[5], 0);
    TextDrawSetProportional(background[5], 1);
    TextDrawSetShadow(background[5], 1);
    TextDrawUseBox(background[5], 1);
    TextDrawBoxColor(background[5], 255);
    TextDrawTextSize(background[5], 194.000000, -20.000000);
    TextDrawSetSelectable(background[5], 0);

    background[6] = TextDrawCreate(446.000000, 2.000000, "#");
    TextDrawBackgroundColor(background[6], 255);
    TextDrawFont(background[6], 2);
    TextDrawLetterSize(background[6], 0.610000, 51.099998);
    TextDrawColor(background[6], -1);
    TextDrawSetOutline(background[6], 0);
    TextDrawSetProportional(background[6], 1);
    TextDrawSetShadow(background[6], 1);
    TextDrawUseBox(background[6], 1);
    TextDrawBoxColor(background[6], 255);
    TextDrawTextSize(background[6], 438.000000, -20.000000);
    TextDrawSetSelectable(background[6], 0);

    registration[0] = TextDrawCreate(197.000000, 80.000000, "Registration");
    TextDrawBackgroundColor(registration[0], 255);
    TextDrawFont(registration[0], 3);
    TextDrawLetterSize(registration[0], 0.540000, 1.699998);
    TextDrawColor(registration[0], -1);
    TextDrawSetOutline(registration[0], 1);
    TextDrawSetProportional(registration[0], 1);
    TextDrawUseBox(registration[0], 1);
    TextDrawBoxColor(registration[0], 255);
    TextDrawTextSize(registration[0], 314.000000, 0.000000);
    TextDrawSetSelectable(registration[0], 0);

    registration[1] = TextDrawCreate(197.000000, 332.000000, "#");
    TextDrawBackgroundColor(registration[1], 255);
    TextDrawFont(registration[1], 2);
    TextDrawLetterSize(registration[1], 0.610000, 0.199999);
    TextDrawColor(registration[1], -1);
    TextDrawSetOutline(registration[1], 0);
    TextDrawSetProportional(registration[1], 1);
    TextDrawSetShadow(registration[1], 1);
    TextDrawUseBox(registration[1], 1);
    TextDrawBoxColor(registration[1], 255);
    TextDrawTextSize(registration[1], 442.000000, -20.000000);
    TextDrawSetSelectable(registration[1], 0);

    registration[2] = TextDrawCreate(235.000000, 139.000000, "        SEX");
    TextDrawBackgroundColor(registration[2], 255);
    TextDrawFont(registration[2], 1);
    TextDrawLetterSize(registration[2], 0.500000, 1.499999);
    TextDrawColor(registration[2], -1);
    TextDrawSetOutline(registration[2], 1);
    TextDrawSetProportional(registration[2], 1);
    TextDrawUseBox(registration[2], 1);
    TextDrawBoxColor(registration[2], 102);
    TextDrawTextSize(registration[2], 403.000000, 10.000000);
    TextDrawSetSelectable(registration[2], true);

    registration[3] = TextDrawCreate(235.000000, 184.000000, "     PASSWORD");
    TextDrawBackgroundColor(registration[3], 255);
    TextDrawFont(registration[3], 1);
    TextDrawLetterSize(registration[3], 0.500000, 1.499999);
    TextDrawColor(registration[3], -1);
    TextDrawSetOutline(registration[3], 1);
    TextDrawSetProportional(registration[3], 1);
    TextDrawUseBox(registration[3], 1);
    TextDrawBoxColor(registration[3], 102);
    TextDrawTextSize(registration[3], 403.000000, 10.000000);
    TextDrawSetSelectable(registration[3], true);

    registration[4] = TextDrawCreate(235.000000, 230.000000, "        AGE");
    TextDrawBackgroundColor(registration[4], 255);
    TextDrawFont(registration[4], 1);
    TextDrawLetterSize(registration[4], 0.500000, 1.499999);
    TextDrawColor(registration[4], -1);
    TextDrawSetOutline(registration[4], 1);
    TextDrawSetProportional(registration[4], 1);
    TextDrawUseBox(registration[4], 1);
    TextDrawBoxColor(registration[4], 102);
    TextDrawTextSize(registration[4], 403.000000, 10.000000);
    TextDrawSetSelectable(registration[4], true);

    registration[5] = TextDrawCreate(235.000000, 276.000000, "        MAIL");
    TextDrawBackgroundColor(registration[5], 255);
    TextDrawFont(registration[5], 1);
    TextDrawLetterSize(registration[5], 0.500000, 1.499999);
    TextDrawColor(registration[5], -1);
    TextDrawSetOutline(registration[5], 1);
    TextDrawSetProportional(registration[5], 1);
    TextDrawUseBox(registration[5], 1);
    TextDrawBoxColor(registration[5], 102);
    TextDrawTextSize(registration[5], 403.000000, 10.000000);
    TextDrawSetSelectable(registration[5], true);

    registration[6] = TextDrawCreate(197.000000, 332.000000, "Continue");
    TextDrawBackgroundColor(registration[6], 255);
    TextDrawFont(registration[6], 3);
    TextDrawLetterSize(registration[6], 0.540000, 1.699998);
    TextDrawColor(registration[6], -1);
    TextDrawSetOutline(registration[6], 1);
    TextDrawSetProportional(registration[6], 1);
    TextDrawUseBox(registration[6], 1);
    TextDrawBoxColor(registration[6], 255);
    TextDrawTextSize(registration[6], 287.000000, 10.000000);
    TextDrawSetSelectable(registration[6], true);

    registration[7] = TextDrawCreate(326.000000, 332.000000, "       Exit");
    TextDrawBackgroundColor(registration[7], 255);
    TextDrawFont(registration[7], 3);
    TextDrawLetterSize(registration[7], 0.540000, 1.699998);
    TextDrawColor(registration[7], -1);
    TextDrawSetOutline(registration[7], 1);
    TextDrawSetProportional(registration[7], 1);
    TextDrawUseBox(registration[7], 1);
    TextDrawBoxColor(registration[7], 255);
    TextDrawTextSize(registration[7], 442.000000, 10.000000);
    TextDrawSetSelectable(registration[7], true);

    registration[8] = TextDrawCreate(210.000000, 309.000000, "        Please fill all forms");
    TextDrawBackgroundColor(registration[8], 255);
    TextDrawFont(registration[8], 2);
    TextDrawLetterSize(registration[8], 0.250000, 1.000000);
    TextDrawColor(registration[8], -16776961);
    TextDrawSetOutline(registration[8], 1);
    TextDrawSetProportional(registration[8], 1);
    TextDrawSetSelectable(registration[8], 0);
}
stock loadCents(playerid)
{
    cents[playerid][0] = TextDrawCreate(606.720214, 70.999984, "");
    TextDrawLetterSize(cents[playerid][0], 0.400000, 1.600000);
    TextDrawAlignment(cents[playerid][0], 1);
    TextDrawColor(cents[playerid][0], 11142399);
    TextDrawSetShadow(cents[playerid][0], 0);
    TextDrawSetOutline(cents[playerid][0], -255);
    TextDrawBackgroundColor(cents[playerid][0], 255);
    TextDrawFont(cents[playerid][0], 1);
    TextDrawSetProportional(cents[playerid][0], 1);
    TextDrawSetShadow(cents[playerid][0], 0);
}
stock loadMainText(playerid)
{
    mainText[playerid][0] = TextDrawCreate(484.000000, 103.000000, "RADIATION:");
    TextDrawBackgroundColor(mainText[playerid][0], 255);
    TextDrawFont(mainText[playerid][0], 1);
    TextDrawLetterSize(mainText[playerid][0], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][0], -16776961);
    TextDrawSetOutline(mainText[playerid][0], 0);
    TextDrawSetProportional(mainText[playerid][0], 1);
    TextDrawSetShadow(mainText[playerid][0], 1);
    TextDrawSetSelectable(mainText[playerid][0], 0);

    mainText[playerid][1] = TextDrawCreate(590.000000, 103.000000, "");
    TextDrawBackgroundColor(mainText[playerid][1], 255);
    TextDrawFont(mainText[playerid][1], 1);
    TextDrawLetterSize(mainText[playerid][1], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][1], -16776961);
    TextDrawSetOutline(mainText[playerid][1], 0);
    TextDrawSetProportional(mainText[playerid][1], 1);
    TextDrawSetShadow(mainText[playerid][1], 1);
    TextDrawSetSelectable(mainText[playerid][1], 0);

    mainText[playerid][2] = TextDrawCreate(484.000000, 115.000000, "SATIETY:");
    TextDrawBackgroundColor(mainText[playerid][2], 255);
    TextDrawFont(mainText[playerid][2], 1);
    TextDrawLetterSize(mainText[playerid][2], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][2], 852308735);
    TextDrawSetOutline(mainText[playerid][2], 0);
    TextDrawSetProportional(mainText[playerid][2], 1);
    TextDrawSetShadow(mainText[playerid][2], 1);
    TextDrawSetSelectable(mainText[playerid][2], 0);

    mainText[playerid][3] = TextDrawCreate(590.000000, 115.000000, "");
    TextDrawBackgroundColor(mainText[playerid][3], 255);
    TextDrawFont(mainText[playerid][3], 1);
    TextDrawLetterSize(mainText[playerid][3], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][3], 852308735);
    TextDrawSetOutline(mainText[playerid][3], 0);
    TextDrawSetProportional(mainText[playerid][3], 1);
    TextDrawSetShadow(mainText[playerid][3], 1);
    TextDrawSetSelectable(mainText[playerid][3], 0);

    mainText[playerid][4] = TextDrawCreate(521.000000, 127.000000, "THIRST:");
    TextDrawBackgroundColor(mainText[playerid][4], 255);
    TextDrawFont(mainText[playerid][4], 1);
    TextDrawLetterSize(mainText[playerid][4], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][4], 16777215);
    TextDrawSetOutline(mainText[playerid][4], 0);
    TextDrawSetProportional(mainText[playerid][4], 1);
    TextDrawSetShadow(mainText[playerid][4], 1);
    TextDrawSetSelectable(mainText[playerid][4], 0);

    mainText[playerid][5] = TextDrawCreate(590.000000, 127.000000, "");
    TextDrawBackgroundColor(mainText[playerid][5], 255);
    TextDrawFont(mainText[playerid][5], 1);
    TextDrawLetterSize(mainText[playerid][5], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][5], 16777215);
    TextDrawSetOutline(mainText[playerid][5], 0);
    TextDrawSetProportional(mainText[playerid][5], 1);
    TextDrawSetShadow(mainText[playerid][5], 1);
    TextDrawSetSelectable(mainText[playerid][5], 0);

    mainText[playerid][6] = TextDrawCreate(528.000000, 138.000000, "VIRUS:");
    TextDrawBackgroundColor(mainText[playerid][6], 255);
    TextDrawFont(mainText[playerid][6], 1);
    TextDrawLetterSize(mainText[playerid][6], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][6], -3407617);
    TextDrawSetOutline(mainText[playerid][6], 0);
    TextDrawSetProportional(mainText[playerid][6], 1);
    TextDrawSetShadow(mainText[playerid][6], 1);
    TextDrawSetSelectable(mainText[playerid][6], 0);

    mainText[playerid][7] = TextDrawCreate(590.000000, 138.000000, "");
    TextDrawBackgroundColor(mainText[playerid][7], 255);
    TextDrawFont(mainText[playerid][7], 1);
    TextDrawLetterSize(mainText[playerid][7], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][7], -3407617);
    TextDrawSetOutline(mainText[playerid][7], 0);
    TextDrawSetProportional(mainText[playerid][7], 1);
    TextDrawSetShadow(mainText[playerid][7], 1);
    TextDrawSetSelectable(mainText[playerid][7], 0);
}
stock loadTextDrawToPlayer(playerid)
{
    // == Login
    login[playerid][0] = TextDrawCreate(197.000000, 80.000000, "   Login");
    TextDrawBackgroundColor(login[playerid][0], 255);
    TextDrawFont(login[playerid][0], 3);
    TextDrawLetterSize(login[playerid][0], 0.540000, 1.699998);
    TextDrawColor(login[playerid][0], -1);
    TextDrawSetOutline(login[playerid][0], 1);
    TextDrawSetProportional(login[playerid][0], 1);
    TextDrawUseBox(login[playerid][0], 1);
    TextDrawBoxColor(login[playerid][0], 255);
    TextDrawTextSize(login[playerid][0], 299.000000, 0.000000);
    TextDrawSetSelectable(login[playerid][0], 0);

    login[playerid][1] = TextDrawCreate(220.000000, 138.000000, "Nick: ~w~");
    TextDrawBackgroundColor(login[playerid][1], 255);
    TextDrawFont(login[playerid][1], 2);
    TextDrawLetterSize(login[playerid][1], 0.260000, 1.899999);
    TextDrawColor(login[playerid][1], -1);
    TextDrawSetOutline(login[playerid][1], 0);
    TextDrawSetProportional(login[playerid][1], 1);
    TextDrawSetShadow(login[playerid][1], 1);
    TextDrawSetSelectable(login[playerid][1], 0);

    login[playerid][2] = TextDrawCreate(220.000000, 170.000000, "ID: ~w~00");
    TextDrawBackgroundColor(login[playerid][2], 255);
    TextDrawFont(login[playerid][2], 2);
    TextDrawLetterSize(login[playerid][2], 0.389999, 1.899999);
    TextDrawColor(login[playerid][2], -1);
    TextDrawSetOutline(login[playerid][2], 0);
    TextDrawSetProportional(login[playerid][2], 1);
    TextDrawSetShadow(login[playerid][2], 1);
    TextDrawSetSelectable(login[playerid][2], 0);

    login[playerid][3] = TextDrawCreate(235.000000, 248.000000, "       PLAY");
    TextDrawBackgroundColor(login[playerid][3], 255);
    TextDrawFont(login[playerid][3], 1);
    TextDrawLetterSize(login[playerid][3], 0.500000, 1.499999);
    TextDrawColor(login[playerid][3], -1);
    TextDrawSetOutline(login[playerid][3], 1);
    TextDrawSetProportional(login[playerid][3], 1);
    TextDrawUseBox(login[playerid][3], 1);
    TextDrawBoxColor(login[playerid][3], 102);
    TextDrawTextSize(login[playerid][3], 403.000000, 10.000000);
    TextDrawSetSelectable(login[playerid][3], true);

    login[playerid][4] = TextDrawCreate(235.000000, 292.000000, "      CITE");
    TextDrawBackgroundColor(login[playerid][4], 255);
    TextDrawFont(login[playerid][4], 1);
    TextDrawLetterSize(login[playerid][4], 0.500000, 1.499999);
    TextDrawColor(login[playerid][4], -1);
    TextDrawSetOutline(login[playerid][4], 1);
    TextDrawSetProportional(login[playerid][4], 1);
    TextDrawUseBox(login[playerid][4], 1);
    TextDrawBoxColor(login[playerid][4], 102);
    TextDrawTextSize(login[playerid][4], 403.000000, 10.000000);
    TextDrawSetSelectable(login[playerid][4], true);

    login[playerid][5] = TextDrawCreate(235.000000, 337.000000, "      COPYRIGHT");
    TextDrawBackgroundColor(login[playerid][5], 255);
    TextDrawFont(login[playerid][5], 1);
    TextDrawLetterSize(login[playerid][5], 0.500000, 1.499999);
    TextDrawColor(login[playerid][5], -1);
    TextDrawSetOutline(login[playerid][5], 1);
    TextDrawSetProportional(login[playerid][5], 1);
    TextDrawUseBox(login[playerid][5], 1);
    TextDrawBoxColor(login[playerid][5], 102);
    TextDrawTextSize(login[playerid][5], 403.000000, 10.000000);
    TextDrawSetSelectable(login[playerid][5], true);
}
stock giveMoney(playerid, amount)
{
    PlayerInfo[playerid][money] += amount;
    GivePlayerMoney(playerid, amount);
}
stock firstSkin(playerid)
{
    for(new i = 0; i < sizeof(registration); i++) TextDrawHideForPlayer(playerid, registration[i]);
    for(new i = 0; i < sizeof(background); i++) TextDrawHideForPlayer(playerid, background[i]);
    if(PlayerInfo[playerid][sex])
    {
        SetPlayerSkin(playerid, 26);
        PlayerInfo[playerid][skin] = 26;
    }
    else
    {
        SetPlayerSkin(playerid, 12);
        PlayerInfo[playerid][skin] = 12;
    }
    cskin[playerid] = 0;
    SetSpawnInfo(playerid, 0, PlayerInfo[playerid][skin], 1971.5336,1424.0586,76.1563, 266.4810, 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid);
    TogglePlayerControllable(playerid,false);
    SetPlayerCameraLookAt(playerid,1975.5336,1424.0586,280);
    SetPlayerCameraPos(playerid,1975.5336,1424.0586,140);
    TextDrawShowForPlayer(playerid, tSkin[0]);
    TextDrawShowForPlayer(playerid, tSkin[1]);
    TextDrawShowForPlayer(playerid, tSkin[2]);
    SelectTextDraw(playerid, 0x2641FEAA);
    return true;
}