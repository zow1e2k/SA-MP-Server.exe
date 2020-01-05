//Библиотеки
#include        <a_samp>
#include        <foreach>
#include        <a_mysql>
#include        <streamer>
#include        <dc_cmd>
#include        <sscanf2>
#include        <object>
#include        <removeBuildings>
#include        <nex-ac> 
//MySQL
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
//Оптимизация
#define         MAX_PLAYER_RANK                 21
#define         MAX_PLAYER_ID                   1        
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
new mainInfo[MAX_PLAYERS][Info];
new invInfo[MAX_PLAYERS][Inventory];
//Колбэки
forward loadInventory(playerid);
forward saveAccount(playerid);
forward loadAccount(playerid);
forward registerInventory(playerid);
forward registerAccount(playerid);
forward timerKick(playerid);
forward checkAccount(playerid);
forward onPlayerUpdateInMinute();
forward loadNewsTD(playerid);
forward payDay();
forward OnCheatDetected(playerid, ip_address[], type, code);
//Глобальные переменные
new MySQL:connection;
new Text:logo[1];
new Text:tSkin[3];
new cskin[MAX_PLAYERS];
new playingTime[MAX_PLAYERS];
new skinMale[3][0] = {{26},{25},{35}};
new skinFemale[3][0] = {{12},{55},{69}};
new Text:background[7];
new Text:registration[8];
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
    LoadObjects();
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
    SetTimer("onPlayerUpdateInMinute",60000, 1);
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
            mainInfo[playerid][member] = 3;
            mainInfo[playerid][rank] = 1;
            Iter_Add(governments,playerid);
            getRankSkin(playerid);
            SetPlayerInterior(playerid,5);
            SetPlayerPos(playerid,770.4814, 2.5649, 1000.9934);
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
            SCM(playerid,COLOR_SYSTEM,"Вы отказались присоединиться к Вооруженным силам");
            SetPVarInt(playerid,"accept",0);
            return true;
        } 
    }
    if ((newkeys == KEY_SECONDARY_ATTACK || newkeys == KEY_JUMP || newkeys == KEY_SECONDARY_ATTACK 
    || newkeys == KEY_FIRE || newkeys == KEY_CROUCH) && (GetPVarInt(playerid,"firstWork") == 2)) 
    { 
        RemovePlayerAttachedObject(playerid,5); 
        ApplyAnimation(playerid,"PED","IDLE_tired",4.1,0,1,1,0,1);
        SetPVarInt(playerid,"firstWork",0);
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
    TogglePlayerSpectating(playerid, true);
    SetSpawnInfo(playerid, 1, mainInfo[playerid][skin], 613.1469,-1415.0784,13.5688, 269.15, 0, 0, 0, 0, 0, 0);
    OnPlayerSpawn(playerid);
    return true;
}

public OnPlayerConnect(playerid)
{
    playingTime[playerid] = 0;
    SetPlayerColor(playerid, 0x00000000);
    killed[playerid] = false;
    removeBuildings(playerid);
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
    new queryinvInfo[512];
    mysql_format(connection, queryinvInfo, sizeof(queryinvInfo), "SELECT `AntiRadin`, `Alcohol`, `Weed`, `Food`, `Medcine`,\
    `ZombiesBlood`, `Instruments`, `Canister`, `MasterKey`, `Bolt`, `SpecialAmmo`, `Metal`,\
    `Wood`, `Blanks`, `Drugs`, `Flesh`, `Fruits`, `Pumpkin`, `Croup`, `Milk`, `Egg`, `Artefacts` FROM inventory WHERE Name='%e'",playerName(playerid));
    mysql_tquery(connection, queryinvInfo, "", "d", playerid);
    new querymainInfo[128];
    mysql_format(connection, querymainInfo, sizeof(querymainInfo), "SELECT `Name`,`ID`,`Skin`,`Password`,`Cents`,`Radiation`,`Hunger`,`Thirst`,`Virus` FROM account WHERE Name='%e'", playerName(playerid));
    mysql_tquery(connection, querymainInfo, "checkAccount", "d", playerid);
    return true;
}
public checkAccount(playerid)
{
    SelectTextDraw(playerid, 0xFF0000FF);
    for(new i = 0; i < sizeof(background); i++) TextDrawShowForPlayer(playerid, background[i]);
    if(cache_num_rows())
    {
        new skinTmp,idTmp, pass[129];
        cache_get_value_name(0, "Password", pass);
        format(mainInfo[playerid][password], 129, "%s", pass);
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
    if (mainInfo[playerid][logged] == false) return true;
    else SPD(playerid,DIALOG_MENU,DIALOG_STYLE_LIST,"{FFCC00}Рюкзак","{FFFFFF}Содержимое\nДосье\nНавыки\nЗаписки\nНавигатор\n\nИспользовать аптечку\nИспользовать антирадин\n\
    Выпить алкоголь\nПоесть\nПокурить\nШирнуться\nИспользовать рем. набор\nИспользовать канистру\n\nПритвориться мирным\n\
    Активировать артефакт\nСпец. пт\nСтиль боя\nПангит","Выбрать","Выйти");
    return true;
}
CMD:jail(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (!isLeader(playerid)) return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "iis[64]", params[0], params[1], params[2])) 
        return SCM(playerid, COLOR_SYSTEM, "Используйте: /jail [id] [минуты] [причина]");
    if (mainInfo[params[0]][member] != mainInfo[playerid][member]) return SCM(playerid, COLOR_SYSTEM, "Не в Вашей фракции");
    static str[] = "%s %s[%i] посадил %s %s[%i] в КПЗ на %i мин по причине: %s";
    new string[sizeof(str)+MAX_PLAYER_NAME*2+MAX_PLAYER_RANK*2+MAX_PLAYER_ID*2+3+64-16];
    format(string, sizeof(string), str, rankName(playerid),playerName(playerid),
        playerid,rankName(params[0]),playerName(params[0]),params[0],params[1],params[2]);
    SCM(params[0],COLOR_RED,string);
    SCM(playerid,COLOR_RED,string);
    SAM(COLOR_SYSTEM,string);
    mainInfo[params[0]][jail] = params[1];
    switch (mainInfo[params[0]][member])
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
CMD:unjail(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (!isLeader(playerid)) return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "is[64]", params[0], params[1])) return SCM(playerid, COLOR_SYSTEM, "Используйте /unjail [id] [причина]");
    if (mainInfo[params[0]][member] != mainInfo[playerid][member]) return SCM(playerid, COLOR_SYSTEM, "Не в Вашей фракции");
    static str[] = "%s %s[%i] вытащил %s %s[%i] из КПЗ по причине: %s";
    new string[sizeof(str)+MAX_PLAYER_NAME*2+MAX_PLAYER_ID*2+MAX_PLAYER+RANK*2+64-14];
    format(string, sizeof(string), str, rankName(playerid),playerName(playerid),playerid,rankName(params[0]),playerName(params[0]),params[0], params[1]);
    SCM(params[0],COLOR_RED,string);
    if (mainInfo[playerid][adminLevel] < 0) SCM(playerid,COLOR_SYSTEM,string);
    SAM(COLOR_SYSTEM,string);
    mainInfo[params[0]][jail] = -1;
    switch (mainInfo[params[0]][member])
    {
        case 1:
        {
            SetPlayerInterior(params[0],17);
            SetPlayerPos(params[0],-951.5187, 1867.1998, 5.9427);
        }
        case 2: 
        {
            SetPlayerInterior(params[0],0);
            SetPlayerPos(params[0],419.6006, 2536.7456, 10.6572);
        }
        case 4,9: 
        {
            SetPlayerInterior(params[0],17);
            SetPlayerPos(params[0],488.3408, -14.8237, 1001.6078);
        }
        case 3,10: 
        {
            SetPlayerInterior(params[0],5);
            SetPlayerPos(params[0],770.4814, 2.5649, 1000.9934);
        }
        case 6:
        {
            SetPlayerInterior(params[0],0);
            SetPlayerPos(params[0],226.5424, 1822.6515, 9.0628);
        }
        case 7: 
        {
            SetPlayerInterior(params[0],0);
            SetPlayerPos(params[0],-707.7855, 979.1034, 7.7314);
        } 
        case 8: 
        {
            SetPlayerInterior(params[0],0);
            SetPlayerPos(params[0],104.2438, 1045.7549, 2.3346);
        }   
    }   
    return true;
}
CMD:warn(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (!isLeader(playerid)) return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "is[64]", params[0], params[1])) return SCM(playerid, COLOR_SYSTEM, "Используйте /warn [id] [причина]");
    static str[] = "%s %s[%i] выдал предупреждение %s %s[%i] по причине: %s";
    new string[sizeof(str)+MAX_PLAYER_NAME*2+MAX_PLAYER_RANK*2+MAX_PLAYER_ID*2+64-14];
    format(string,sizeof(string),str,rankName(playerid),playerName(playerid),playerid,rankName(params[0]),playerName(params[0]),params[0],params[1]);
    SAM(COLOR_SYSTEM,string);
    SCM(params[0],COLOR_RED,string);
    mainInfo[params[0]][warnLevel]++;
    if (mainInfo[params[0]][warnLevel] == 2) 
    {
        mainInfo[params[0]][rank] = 1;
        SCM(params[0],COLOR_RED,"Ваш ранг был понижен до 1 (2 предупреждения)");
    }
    else if (mainInfo[params[0]][warnLevel] > 2)
    {
        mainInfo[params[0]][member] = 0;
        mainInfo[params[0]][rank] = 0;
        mainInfo[params[0]][warnLevel] = 0;
        Iter_Remove(stalkers, params[0]);
        Iter_Remove(bandits, params[0]);
        Iter_Remove(monolites, params[0]);
        Iter_Remove(governments, params[0]);
        SCM(params[0],COLOR_RED,"Вы были выгнаны из фракции (3 предупреждения)");   
    } 
    return true;
}
CMD:unwarn(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (!isLeader(playerid)) return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "is[64]", params[0], params[1])) return SCM(playerid, COLOR_SYSTEM, "Используйте /warn [id] [причина]");
    static str[] = "%s %s[%i] снял предупреждение с %s %s[%i] по причине: %s";
    new string[sizeof(str)+MAX_PLAYER_NAME*2+MAX_PLAYER_RANK*2+MAX_PLAYER_ID*2+64-14];
    format(string,sizeof(string),str,rankName(playerid),playerName(playerid),playerid,rankName(params[0]),playerName(params[0]),params[0],params[1]);
    SAM(COLOR_SYSTEM,string);
    SCM(params[0],COLOR_RED,string);
    SCM(playerid,COLOR_SYSTEM,string);
    mainInfo[params[0]][warnLevel]--;
    return true;
}       
CMD:invite(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][member] != 3 && mainInfo[playerid][rank] != 16)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "i", params[0])) return SCM(playerid, COLOR_SYSTEM, "Используйте /invite [id]");
    if (mainInfo[params[0]][member] != 0) return SCM(playerid, COLOR_SYSTEM, "Уже состоит в организации");
    static str[] = "%s %s[%i] предлагает Вам присоединиться к Вооруженным Силам";
    new string[sizeof(str)+MAX_PLAYER_NAME+MAX_PLAYER_ID+MAX_PLAYER_RANK-6];
    format(string,sizeof(string),str,rankName(playerid),playerName(playerid),playerid);
    SCM(params[0],COLOR_RED,string);
    SetPVarInt(params[0],"accept",1);
    SCM(params[0],COLOR_RED,"Нажмите Y чтобы согласиться или N чтобы отказаться");
    return true; 
}
CMD:f(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][mute] > 0) return true;
    if (mainInfo[playerid][member] < 1)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "s[64]", params[0])) return SCM(playerid, COLOR_SYSTEM, "Используйте /f [сообщение]");
    static str[] = "[F] - %s (%s %s[%i])";
    new string[sizeof(str)+MAX_PLAYER_NAME+MAX_PLAYER_ID+MAX_PLAYER_RANK-8+64];
    format(string, sizeof(string), str,params[0],rankName(playerid),playerName(playerid),playerid);
    switch (mainInfo[playerid][member])
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
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][mute] > 0) return true;
    if (mainInfo[playerid][member] < 1)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "s[64]", params[0])) return SCM(playerid, COLOR_SYSTEM, "Используйте /r [сообщение]");
    static str[] = "[R] - %s (%s %s[%i])";
    new string[sizeof(str)+MAX_PLAYER_NAME+MAX_PLAYER_ID+MAX_PLAYER_RANK-8+64];
    format(string, sizeof(string), str,params[0],rankName(playerid),playerName(playerid),playerid);
    switch (mainInfo[playerid][member])
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
CMD:setweathertime(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] < 3)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "ii", params[0], params[1])) return SCM(playerid, COLOR_SYSTEM, "Используйте: /setweather [id погоды] [время]");
    SetWeather(params[0]);
    SetWorldTime(params[1]);
    return true;
}
CMD:payday(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] < 3)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    payDay();
    return true;
}
CMD:setptime(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] < 3)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    playingTime[playerid] = 35;
    return true;
}  
CMD:gethealth(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] < 1)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "i", params[0])) return SCM(playerid, COLOR_SYSTEM, "Используйте /gethealth [id]");
    new Float: healthP;
    GetPlayerHealth(params[0], healthP);
    static str[] = "%s[%i] HP: %f и %f";
    new string[sizeof(str)+MAX_PLAYER_NAME+MAX_PLAYER_ID+2];
    format(string,sizeof(string),str,playerName(params[0]),params[0],healthP, mainInfo[params[0]][health]);
    SCM(playerid,COLOR_SYSTEM,string);
    return true;
}
CMD:setmember(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] < 3)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "iii", params[0], params[1], params[2])) return SCM(playerid, COLOR_SYSTEM, "Используйте /setmember [id] [member id] [rank]");
    mainInfo[params[0]][member] = params[1];
    mainInfo[params[0]][rank] = params[2];
    getRankSkin(params[0]);
    getPlayerColor(params[0]);
    static str[] = "[A] [%s] %s[%i] назначил игрока %s[%i] \"%s\" в организации: \"%s\"";
    new string[sizeof(str)+MAX_PLAYER_NAME*2+MAX_PLAYER_RANK*2+MAX_PLAYERID*2-14];
    format(string,sizeof(string),str, adminRankName(playerid), playerName(playerid),playerid, playerName(params[0]),params[0], rankName(params[0]),memberName(params[0]));
    SAM(COLOR_SYSTEM, string);
    switch (mainInfo[params[0]][member])
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
        default:
        {
            Iter_Remove(stalkers, params[0]);
            Iter_Remove(bandits, params[0]);
            Iter_Remove(monolites, params[0]);
            Iter_Remove(governments, params[0]);
        }
    }
    return true;
}
CMD:setskin(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] < 3)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "ii", params[0], params[1])) return SCM(playerid, COLOR_SYSTEM, "Используйте /setskin [id] [id скина]");
    SetPlayerSkin(params[0],params[1]);
    mainInfo[params[0]][skin] = params[1];
    static str[] = "[A] [%s] %s[%i] изменил облик игроку %s[%i]";
    new string[sizeof(str)+MAX_PLAYER_NAME*2+MAX_PLAYER_ID*2+MAX_PLAYER_RANK-10];
    format(string,sizeof(string),str, adminRankName(playerid), playerName(playerid),playerid, playerName(params[0]),params[0]);
    SAM(COLOR_SYSTEM, string);
    return true;
}
CMD:kick(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] < 1)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "is[64]", params[0], params[1])) return SCM(playerid, COLOR_SYSTEM, "Используйте /kick [id] [причина]");
    if (mainInfo[params[0]][adminLevel] > 0 && mainInfo[playerid][adminLevel] < 5) return true;
    static str[] = "[A] [%s] %s[%i] кикнул %s[%i] по причине: %s";
    new string[sizeof(str)+MAX_PLAYER_NAME*2+MAX_PLAYER_ID*2+MAX_PLAYER_RANK+64-12];
    format(string,sizeof(string),str, adminRankName(playerid), playerName(playerid),playerid, playerName(params[0]),params[0],params[1]);
    SCM(params[0],COLOR_RED,string);
    SAM(COLOR_SYSTEM, string);
    Kick(params[0]);   
    return true; 
}
CMD:ban(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] < 3) return SCM(playerid,COLOR_SYSTEM,"Несуществующая команда");
    if (sscanf(params,"iis[64]", params[0], params[1], params[2])) return SCM(playerid, COLOR_SYSTEM, "Используйте /ban [id] [срок в часах] [причина]");
    if (mainInfo[params[0]][adminLevel] > 0 && mainInfo[playerid][adminLevel] < 5) return SCM(playerid,COLOR_SYSTEM,"Несуществующая команда");
    static str[] = "[A] [%s] %s[%i] забанил %s[%i] на %i час(-a/-ов) по причине: %s";
    new string[sizeof(str)+MAX_PLAYER_NAME*2+MAX_PLAYER_ID*2+MAX_PLAYER_RANK+64+1-14];
    format(string,sizeof(string),str,adminRankName(playerid), playerName(playerid), playerid, playerName(params[0]),params[0],params[1],params[2]);
    SCM(params[0],COLOR_RED,string);
    SAM(COLOR_SYSTEM, string);
    mainInfo[params[0]][ban] = params[1];
    Kick(params[0]);
    return true;   
}
CMD:mute(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] < 1) return SCM(playerid,COLOR_SYSTEM,"Несуществующая команда");
    if (sscanf(params,"iis[64]",params[0],params[1],params[2])) return SCM(playerid,COLOR_SYSTEM,"Используйте: /mute [id] [срок в минутах] [причина]");
    static str[] = "[A] [%s] %s[%i] поставил затычку игроку %s[%i] на %i минут по причине: %s";
    new string[sizeof(str)+MAX_PLAYER_NAME*2+MAX_PLAYER_ID*2+MAX_PLAYER_RANK+1+64-14]; 
    format(string,sizeof(string),str,adminRankName(playerid),playerName(playerid),playerid,playerName(params[0]),params[0],params[1],params[2]);
    SCM(params[0],COLOR_RED,string);
    SAM(COLOR_SYSTEM,string);
    mainInfo[params[0]][mute] = params[1];
    return true;    
}
CMD:unmute(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] < 1) return SCM(playerid,COLOR_SYSTEM,"Несуществующая команда");
    if (sscanf(params,"i",params[0])) return SCM(playerid,COLOR_SYSTEM,"Используйте: /unmute [id]");
    static str[] = "[A] [%s] %s[%i] снял затычку с игрока %s[%i]";
    new string[sizeof(str)+MAX_PLAYER_NAME*2+MAX_PLAYER_ID*2+MAX_PLAYER_RANK-10];
    format(string,sizeof(string),str,adminRankName(playerid),playerName(playerid),playerid,playerName(params[0]),params[0],params[1],params[2]);
    SCM(params[0],COLOR_RED,string);
    SAM(COLOR_SYSTEM,string);
    mainInfo[params[0]][mute] = 0;
    return true;    
}
CMD:info(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] < 1) return SCM(playerid,COLOR_SYSTEM,"Несуществующая команда");
    if (sscanf(params,"i",params[0])) return SCM(playerid,COLOR_SYSTEM,"Используйте: /info [id]");
    new str[] = "{FFFFFF}Позывной\t\t%s\nУровень допуска\t%i\nГруппировка\t\t%s\nЗвание\t\t\t%s\n\n
        Опыт\t\t\t%i\nЗамечания\t\t%i\n\Клан\t\t\t%s\nДонат очки\t\t%i\nЧасы в зоне\t\t%i\n\
        VIP уровень\t\t%i\nIP адрес\t\t%s\nДеньги\t\t%i";
    new string[sizeof(str)+MAX_PLAYER_NAME+1+MAX_PLAYER_RANK*3+1+1+1+1+1+16+1-24];
    format(string,sizeof(string),str,mainInfo[params[0]][nickName],mainInfo[params[0]][accessLevel],memberName(params[0]),
        rankName(params[0]),mainInfo[params[0]][expLevel],mainInfo[params[0]][warnLevel],
        mainInfo[params[0]][clan],mainInfo[params[0]][donateLevel],
        mainInfo[params[0]][hoursLevel],mainInfo[params[0]][vipLevel], 
        playerIp(params[0]),mainInfo[params[0]][money]);
    SPD(playerid,DIALOG_PLAYER_INFO,DIALOG_STYLE_MSGBOX,"{FFCC00}Информация об игроке",string,"Продолжить","");
    return true;    
}
CMD:goto(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] < 1) return SCM(playerid,COLOR_SYSTEM,"Несуществующая команда");
    if (sscanf(params,"i",params[0])) return SCM(playerid,COLOR_SYSTEM,"Используйте: /goto [id]");
    static str[] = "[A] [%s] %s[%i] телепортировался к игроку %s[%i]";
    new string[sizeof(str)+MAX_PLAYER_NAME*2+MAX_PLAYER_ID*2+MAX_PLAYER_RANK-10];
    format(string,sizeof(string),str, adminRankName(playerid), playerName(playerid),playerid, playerName(params[0]),params[0]);
    SAM(COLOR_SYSTEM, string);
    new Float: x, Float: y, Float: z;
    GetPlayerPos(params[0],x,y,z);
    SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(params[0]));
    SetPlayerInterior(playerid, GetPlayerInterior(params[0]));
    SetPlayerPos(playerid,x+2.0,y,z);
    return true; 
}
CMD:gethere(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] < 1) return SCM(playerid,COLOR_SYSTEM,"Несуществующая команда");
    if (sscanf(params,"i",params[0])) return SCM(playerid,COLOR_SYSTEM,"Используйте: /gethere [id]");
    new str[] = "[A] [%s] %s[%i] телепортировал к себе игрока %s[%i]";
    new string[sizeof(str)+MAX_PLAYER_NAME*2+MAX_PLAYER_ID*2+MAX_PLAYER_RANK-10];
    format(string,sizeof(string),str, adminRankName(playerid), playerName(playerid),playerid, playerName(params[0]),params[0]);
    SAM(COLOR_SYSTEM, string);
    new Float: x, Float: y, Float: z;
    GetPlayerPos(playerid,x,y,z);
    SetPlayerVirtualWorld(params[0],GetPlayerVirtualWorld(playerid));
    SetPlayerInterior(params[0],GetPlayerInterior(playerid));
    SetPlayerPos(params[0],x+2.0,y,z);
    return true; 
}       
CMD:a(playerid, params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] < 1)  return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    if (sscanf(params, "s[64]", params[0])) return SCM(playerid, COLOR_SYSTEM, "Используйте /a [сообщение]");
    static str[] = "[A] %s (%s %s[%i])";
    new string[sizeof(str)+MAX_PLAYER_NAME+MAX_PLAYER_ID+64-8];
    format(string,sizeof(string),str, params[0], adminRankName(playerid), playerName(playerid),playerid);
    SAM(0x32CD32AA, string);
    return true;
}
CMD:veh(playerid,params[])
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] <= 3) return SCM(playerid, COLOR_SYSTEM, "Несуществующая команда");
    new Float:pX,Float:pY,Float:pZ;  
    if (sscanf(params, "iii", params[0],params[1],params[2])) 
        return SCM(playerid, COLOR_SYSTEM, "Используйте: /veh [id транспорта] [цвет 1] [цвет 2]");
    {
        if (vehID[playerid] > 0)
        {
            DestroyVehicle(vehID[playerid]);
            vehID[playerid] = 0;
        }
        if (params[1] > 126 || params[1] < 0 || params[2] > 126 || params[2] < 0) return SCM(playerid, COLOR_SYSTEM, "id цвета от 0 до 126");
        GetPlayerPos(playerid,pX,pY,pZ);
        vehID[playerid] = CreateVehicle(params[0],pX+2,pY,pZ,0.0,1,1,0,0);
        static str[] = "[A] %s %s[%i] создал т/с %i id [%i server id]";
        new string[sizeof(str)+MAX_PLAYER_NAME+MAX_PLAYER_RANK+MAX_PLAYER_ID*3-10];
        format(string,sizeof(string),str,adminRankName(playerid),playerName(playerid),playerid,params[0],vehID[playerid]);
        SAM(COLOR_SYSTEM,string);
        ChangeVehicleColor(vehID[playerid],params[1],params[2]);
    }
    return true;
}
CMD:delveh(playerid)
{
    if (mainInfo[playerid][logged] == false) return true;
    if (mainInfo[playerid][adminLevel] <= 3) return true;
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
    switch(mainInfo[playerid][member])
    {
        case 3,6,10: Iter_Remove(governments,playerid);
        case 2,8: Iter_Remove(stalkers,playerid);
        case 4,9: Iter_Remove(bandits,playerid);
        case 1,7: Iter_Remove(monolites,playerid);
    }
    if (GetPVarInt(playerid,"accept")) DeletePVar(playerid,"accept");
    if (mainInfo[playerid][adminLevel] > 0) Iter_Remove(admins, playerid);
    saveAccount(playerid);
    return true;
}

public OnPlayerText(playerid, text[])
{
    if (mainInfo[playerid][logged] == false) return false;
    if (mainInfo[playerid][mute] > 0) 
    {
        SCM(playerid,COLOR_SYSTEM,"Бан чата");
        return false;
    }
    new Float:x, Float:y, Float: z;
    static str[] = "- %s (%s[%i])";
    new string[sizeof(str)+MAX_PLAYER_NAME+1+22-6];
    format(string,sizeof string,str,text,playerName(playerid),playerid);
    GetPlayerPos(playerid,x,y,z);
    foreach (Player, i)
    {
        if(!IsPlayerInRangeOfPoint(i,50.0,x,y,z)) continue;
        SCM(i,0xFFFFFFFF,string);
    }
    return false;
}

public OnPlayerSpawn(playerid)
{
    getPlayerColor(playerid);
    getRankSkin(playerid);
    switch (mainInfo[playerid][member])
    {
        case 2,8: 
        {
            if (mainInfo[playerid][jail] > 0)
            {
                SetPlayerHealth(playerid,30);
                SetPlayerInterior(playerid,0);
                SetPlayerPos(playerid,111.4851, 1047.4766, 6.2349);
                return false;
            }
        }
        case 1: 
        {
            if (mainInfo[playerid][jail] > 0)
            {
                SetPlayerHealth(playerid,30);
                SetPlayerInterior(playerid,0);
                SetPlayerPos(playerid,-699.9142, 978.1697, 7.8307);
                return false;
            }
        }
        case 7: 
        {
            if (mainInfo[playerid][jail] > 0)
            {
                SetPlayerHealth(playerid,30);
                SetPlayerInterior(playerid,0);
                SetPlayerPos(playerid,-699.9142, 978.1697, 7.8307);
                return false;
            }
        }
        case 3,6,10: 
        {
            if (mainInfo[playerid][jail] > 0)
            {
                SetPlayerHealth(playerid, 30);
                SetPlayerInterior(playerid, 5);
                SetPlayerPos(playerid,322.1140, 314.1566, 999.3682);
                return false;
            }
        }
        case 4,9: 
        {
            if (mainInfo[playerid][jail] > 0)
            {
                SetPlayerHealth(playerid,30);
                SetPlayerInterior(playerid,6);
                SetPlayerPos(playerid,347.0079, 306.2476, 999.7764);
                return false;
            }
        }     
    }
    SetPlayerInterior(playerid,0);
    if (killed[playerid] == false)
    {
        loadCents(playerid);
        loadMainText(playerid);
        SetPlayerHealth(playerid,mainInfo[playerid][health]);
        new query[512];
        mysql_format(connection, query, sizeof(query), "SELECT `Cents`,`Radiation`,`Hunger`,`Thirst`,`Virus` FROM `account` WHERE Name='%e'", playerName(playerid));
        mysql_tquery(connection, query, "loadNewsTD", "d", playerid);
    }
    SetPlayerHealth(playerid,30);
    TogglePlayerSpectating(playerid, false);
    if(mainInfo[playerid][logged] == false) return true;
    return false;
}

public OnPlayerDeath(playerid, killerid, reason)
{
    killed[playerid] = true;
    SendDeathMessage(killerid, playerid, reason);
    mainInfo[playerid][deaths]++;
    if (killerid != INVALID_PLAYER_ID) mainInfo[killerid][kills]++;
    //SetSpawnInfo(playerid, 1, mainInfo[playerid][skin], 613.1469,-1415.0784,13.5688, 269.15, 0, 0, 0, 0, 0, 0);
    //SpawnPlayer(playerid);
    return true;
}

public OnPlayerUpdate(playerid)
{
    SetPlayerScore(playerid, mainInfo[playerid][accessLevel]);
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
            format(mainInfo[playerid][password], 129, inputtext);
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
            format(mainInfo[playerid][mail], 129, inputtext);
            mailWrote[playerid] = 1;
            for(new i = 0; i < sizeof(registration); i++) TextDrawShowForPlayer(playerid, registration[i]);
        }
        case DIALOG_SEX:
        {
            if(response)
            {
                SCM(playerid, COLOR_YELLOW, "Пол: {058AFF}Мужской");
                mainInfo[playerid][sex] = 1;
                sexWrote[playerid] = 1;
                for(new i = 0; i < sizeof(registration); i++) TextDrawShowForPlayer(playerid, registration[i]);

            }
            else
            {
                SCM(playerid, COLOR_YELLOW, "Пол: {EE3B3B}Женский");
                mainInfo[playerid][sex] = 2;
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
                mainInfo[playerid][age] = strval(inputtext);
                ageWrote[playerid] = 1;
                for(new i = 0; i < sizeof(registration); i++) TextDrawShowForPlayer(playerid, registration[i]);
            }
        }

        case DIALOG_LOGIN:
        {
            if(!response) return true;
            if(!strcmp(mainInfo[playerid][password], inputtext, false, 129))
            {
                new queryInfo[512];
                mysql_format(connection, queryInfo, sizeof(queryInfo), "SELECT * FROM `account` WHERE Name = '%e'", playerName(playerid));
                mysql_tquery(connection, queryInfo, "loadAccount", "d", playerid);
            } 
            else 
            {
                mainInfo[playerid][wrongPass] ++;
                SCM(playerid, COLOR_RED, "Неверный пароль");
                SPD(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "{FFCC00}Авторизация", "\n\n\t\t{FFFF00}Неверный пароль\n\n\t\t{FFFFFF}Введите пароль:\n\n", "Далее", "Выйти");
                if(mainInfo[playerid][wrongPass] == 3)
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
                        switch (mainInfo[playerid][member])
                        {
                            case 1,7: //Мутанты
                            {
                                format(string,sizeof(string),"{FFFFFF}Наименование:\t\tКоличество:\nАнтирадин\t\t%i\nАлкоголь\t\t%i\nСигареты\t\t%i\n\
                                Еда\t\t\t%i\nАптечка\t\t%i\nКровь зомби\t\t%i\nРем. комплект\t\t%i\nКанистра\t\t%i\nОтмычки\t\t%i\nБолты\t\t\t%i\nСпец. боеприпасы\t%i\n\
                                Металл\t\t\t%i\nДревесина\t\t%i\nЗаготовки\t\t%i\nНаркотики\t\t%i\nПлоть\t\t\t%i\nФрукты\t\t%i\nТыквa\t\t\t%i\nКрупа\t\t\t%i\nМолоко\t\t\t%i\nЯйца\t\t\t%i",
                                invInfo[playerid][antiRadin],invInfo[playerid][alcohol],invInfo[playerid][weed],
                                invInfo[playerid][food],invInfo[playerid][medcine],invInfo[playerid][zombiesBlood],
                                invInfo[playerid][instruments],invInfo[playerid][canister],invInfo[playerid][masterKey],
                                invInfo[playerid][bolt],invInfo[playerid][specialAmmo],invInfo[playerid][metal],
                                invInfo[playerid][wood],invInfo[playerid][blanks],invInfo[playerid][drugs],
                                invInfo[playerid][flesh],invInfo[playerid][fruits],invInfo[playerid][pumpkin],
                                invInfo[playerid][croup],invInfo[playerid][milk],invInfo[playerid][egg]);
                                SPD(playerid,DIALOG_MENU_INVENTORY,DIALOG_STYLE_MSGBOX,"{FFCC00}Рюкзак",string,"Назад","Выйти");
                            }
                            case 2,8: // Сталкеры
                            {
                                format(string,sizeof(string),"{FFFFFF}Наименование:\t\tКоличество:\nАнтирадин\t\t%i\nАлкоголь\t\t%i\nСигареты\t\t%i\n\
                                Еда\t\t\t%i\nАптечка\t\t%i\nКровь зомби\t\t%i\nРем. комплект\t\t%i\nКанистра\t\t%i\nОтмычки\t\t%i\nБолты\t\t\t%i\nСпец. боеприпасы\t%i\n\
                                Металл\t\t\t%i\nДревесина\t\t%i\nЗаготовки\t\t%i\nНаркотики\t\t%i\nАртефакты\t\t\t%i\nФрукты\t\t%i\nТыквa\t\t\t%i\nКрупа\t\t\t%i\nМолоко\t\t\t%i\nЯйца\t\t\t%i",
                                invInfo[playerid][antiRadin],invInfo[playerid][alcohol],invInfo[playerid][weed],
                                invInfo[playerid][food],invInfo[playerid][medcine],invInfo[playerid][zombiesBlood],
                                invInfo[playerid][instruments],invInfo[playerid][canister],invInfo[playerid][masterKey],
                                invInfo[playerid][bolt],invInfo[playerid][specialAmmo],invInfo[playerid][metal],
                                invInfo[playerid][wood],invInfo[playerid][blanks],invInfo[playerid][drugs],
                                invInfo[playerid][artefacts],invInfo[playerid][fruits],invInfo[playerid][pumpkin],
                                invInfo[playerid][croup],invInfo[playerid][milk],invInfo[playerid][egg]);
                                SPD(playerid,DIALOG_MENU_INVENTORY,DIALOG_STYLE_MSGBOX,"{FFCC00}Рюкзак",string,"Назад","Выйти");
                            }
                            default:
                            {
                                format(string,sizeof(string),"{FFFFFF}Наименование:\t\tКоличество:\nАнтирадин\t\t%i\nАлкоголь\t\t%i\nСигареты\t\t%i\n\
                                Еда\t\t\t%i\nАптечка\t\t%i\nКровь зомби\t\t%i\nРем. комплект\t\t%i\nКанистра\t\t%i\nОтмычки\t\t%i\nБолты\t\t\t%i\nСпец. боеприпасы\t%i\n\
                                Металл\t\t\t%i\nДревесина\t\t%i\nЗаготовки\t\t%i\nНаркотики\t\t%i\nФрукты\t\t%i\nТыквa\t\t\t%i\nКрупа\t\t\t%i\nМолоко\t\t\t%i\nЯйца\t\t\t%i",
                                invInfo[playerid][antiRadin],invInfo[playerid][alcohol],invInfo[playerid][weed],
                                invInfo[playerid][food],invInfo[playerid][medcine],invInfo[playerid][zombiesBlood],
                                invInfo[playerid][instruments],invInfo[playerid][canister],invInfo[playerid][masterKey],
                                invInfo[playerid][bolt],invInfo[playerid][specialAmmo],invInfo[playerid][metal],
                                invInfo[playerid][wood],invInfo[playerid][blanks],invInfo[playerid][drugs],
                                invInfo[playerid][fruits],invInfo[playerid][pumpkin],
                                invInfo[playerid][croup],invInfo[playerid][milk],invInfo[playerid][egg]);
                                SPD(playerid,DIALOG_MENU_INVENTORY,DIALOG_STYLE_MSGBOX,"{FFCC00}Рюкзак",string,"Назад","Выйти");
                            }
                        }
                    }
                    case 1:
                    {
                        format(string,sizeof(string),"{FFFFFF}Позывной\t\t%s\nУровень допуска\t%i\nГруппировка\t\t%s\nЗвание\t\t\t%s\nОпыт\t\t\t%i\nЗамечания\t\t%i\n\
                        Клан\t\t\t%s\nДонат очки\t\t%i\nЧасы в зоне\t\t%i\nVIP уровень\t\t%i",
                        mainInfo[playerid][nickName],mainInfo[playerid][accessLevel],memberName(playerid),
                        rankName(playerid),mainInfo[playerid][expLevel],mainInfo[playerid][warnLevel],
                        mainInfo[playerid][clan],mainInfo[playerid][donateLevel],
                        mainInfo[playerid][hoursLevel],mainInfo[playerid][vipLevel]);
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
                SetPVarInt(playerid,"firstWork",1);
            }
        }
        case DIALOG_FIRSTJOB_FIRE:
        {
            if (response) 
            {
                for (new k=0;k<10;k++) DestroyDynamicCP(firstJobCP[playerid][k]);
                SetPVarInt(playerid,"firstWork",0);
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
        if (GetPVarInt(playerid,"firstWork") == 1) SPD(playerid,DIALOG_FIRSTJOB_FIRE,DIALOG_STYLE_MSGBOX,"{FFCC00}Первая работа","Уволиться?","Да","Нет");
        else SPD(playerid,DIALOG_FIRSTJOB_HIRE,DIALOG_STYLE_MSGBOX,"{FFCC00}Первая работа","ЫЫЫ","Принять","Отклонить");
    }
    return true;   
}

public OnPlayerEnterDynamicCP(playerid,checkpointid)
{
    if (checkpointid == firstJobCP[playerid][0] && GetPVarInt(playerid,"firstWork") == 1)
    {
        TogglePlayerDynamicCP(playerid, firstJobCP[playerid][0], false);
        ClearAnimations(playerid);
        ApplyAnimation(playerid, "CARRY", "PUTDWN", 4.0, 0, 0, 0, 0, 0);
        ApplyAnimation(playerid,"CARRY","crry_prtial",4.1,0,1,1,1,1);
        SetPVarInt(playerid,"firstWork",2);
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
    if (GetPVarInt(playerid,"firstWork") == 2)
    {
        if (checkpointid == firstJobCP[playerid][1] || checkpointid == firstJobCP[playerid][2] || checkpointid == firstJobCP[playerid][3] ||
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
            SetPVarInt(playerid,"firstWork",1);
        }
    }
    return true;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    if (mainInfo[playerid][adminLevel] > 0) SetPlayerPosFindZ(playerid, fX, fY, fZ);
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
            firstSkin(playerid);
    }
    if(clickedid == login[playerid][3])
    {
        for(new i = 0; i < 7; i++) TextDrawHideForPlayer(playerid, login[playerid][i]);
        SPD(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "{FFCC00}Авторизация", "\n\n\t{FFFFFF}Введите Ваш пароль:\n\n", "Далее", "Выйти");
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
        if(mainInfo[playerid][sex] == 1)
        {
            if(cskin[playerid] < 0) cskin[playerid] = 2;
            SetPlayerSkin(playerid, skinMale[cskin[playerid]][0]);
            mainInfo[playerid][skin] = skinMale[cskin[playerid]][0];
        }
        else if(mainInfo[playerid][sex] == 2)
        {
            if(cskin[playerid] < 0) cskin[playerid] = 2;
            SetPlayerSkin(playerid, skinFemale[cskin[playerid]][0]);
            mainInfo[playerid][skin] = skinFemale[cskin[playerid]][0];
        }
    }
    if(clickedid == tSkin[1])
    {
        cskin[playerid] ++;
        if(mainInfo[playerid][sex] == 1)
        {
            if(cskin[playerid] > 2) cskin[playerid] = 0;
            SetPlayerSkin(playerid, skinMale[cskin[playerid]][0]);
            mainInfo[playerid][skin] = skinMale[cskin[playerid]][0];
        }
        else if(mainInfo[playerid][sex] == 2)
        {
            if(cskin[playerid] > 2) cskin[playerid] = 0;
            SetPlayerSkin(playerid, skinFemale[cskin[playerid]][0]);
            mainInfo[playerid][skin] = skinFemale[cskin[playerid]][0];
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
    mainInfo[playerid][logged] = true;
    mainInfo[playerid][accessLevel] = 0;
    mainInfo[playerid][adminLevel] = 0;
    mainInfo[playerid][money] = 5000;
    mainInfo[playerid][deaths] = 0;
    mainInfo[playerid][centsLevel] = 50;
    mainInfo[playerid][wrongPass] = 0;
    mainInfo[playerid][member] = 0;
    mainInfo[playerid][rank] = 0;
    mainInfo[playerid][expLevel] = 0;
    mainInfo[playerid][radiation] = 0;
    mainInfo[playerid][hunger] = 0;
    mainInfo[playerid][thirst] = 0;
    mainInfo[playerid][virus] = 0;
    mainInfo[playerid][ban] = 0;
    mainInfo[playerid][mute] = 0;
    mainInfo[playerid][jail] = 0;
    mainInfo[playerid][health] = 100;
    mainInfo[playerid][warnLevel] = 0;
    mainInfo[playerid][donateLevel] = 0;
    mainInfo[playerid][vipLevel] = 0;
    mainInfo[playerid][hoursLevel] = 0;
    ResetPlayerMoney(playerid);
    new querymainInfo[300];
    mysql_format(connection, querymainInfo, sizeof(querymainInfo), "INSERT INTO `account`\
    (`Name`, `Password`, `Email`, `Admin`, `Sex`, `Age`,`Money`, `Skin`, `Kill`, `Death`, `Cents`)\
     VALUES ('%e','%e','%e','%d','%d','%d','%d','%d','%d','%d','%d')",
    playerName(playerid),
    mainInfo[playerid][password],
    mainInfo[playerid][mail],
    mainInfo[playerid][adminLevel],
    mainInfo[playerid][sex],
    mainInfo[playerid][age],
    mainInfo[playerid][money],
    mainInfo[playerid][skin],
    mainInfo[playerid][kills],
    mainInfo[playerid][deaths],
    mainInfo[playerid][centsLevel]);
    mysql_tquery(connection, querymainInfo, "", "d", playerid);
    registerInventory(playerid);
    return true;
}
public registerInventory(playerid)
{
    new queryinvInfo[256];
    invInfo[playerid][antiRadin] = 0;
    invInfo[playerid][alcohol] = 0;
    invInfo[playerid][weed] = 0;
    invInfo[playerid][food] = 0;
    invInfo[playerid][medcine] = 0;
    invInfo[playerid][zombiesBlood] = 0;
    invInfo[playerid][instruments] = 0;
    invInfo[playerid][canister] = 0;
    invInfo[playerid][masterKey] = 0;
    invInfo[playerid][bolt] = 0;
    invInfo[playerid][specialAmmo] = 0;
    invInfo[playerid][metal] = 0;
    invInfo[playerid][wood] = 0;
    invInfo[playerid][blanks] = 0;
    invInfo[playerid][drugs] = 0;
    invInfo[playerid][flesh] = 0;
    invInfo[playerid][artefacts] = 0;
    invInfo[playerid][fruits] = 0;
    invInfo[playerid][pumpkin] = 0;
    invInfo[playerid][croup] = 0;
    invInfo[playerid][milk] = 0;
    invInfo[playerid][egg] = 0;
    mysql_format(connection, queryinvInfo, sizeof(queryinvInfo), "INSERT INTO `inventory` (`Name`) VALUES ('%e')",
    playerName(playerid));
    mysql_tquery(connection, queryinvInfo, "", "d", playerid);
    SetPlayerScore(playerid, mainInfo[playerid][accessLevel]);
    GivePlayerMoney(playerid, mainInfo[playerid][money]);
    SetSpawnInfo(playerid, 0, mainInfo[playerid][skin], 613.1469,-1415.0784,13.5688, 269.15, 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid);
    hideTextDraw(playerid);
    return true;
}
public loadAccount(playerid)
{
    new strMail[32],strNick[20],strClan[20];
    cache_get_value_name(0, "Email", strMail);
    format(mainInfo[playerid][mail], sizeof(strMail), "%s", strMail);
    cache_get_value_name_int(0, "Admin",mainInfo[playerid][adminLevel]);
    cache_get_value_name_int(0, "Sex",mainInfo[playerid][sex]);
    cache_get_value_name_int(0, "Age",mainInfo[playerid][age]);
    cache_get_value_name_int(0, "Money",mainInfo[playerid][money]);
    cache_get_value_name_int(0, "Skin",mainInfo[playerid][skin]);
    cache_get_value_name_int(0, "Kill",mainInfo[playerid][kills]);
    cache_get_value_name_int(0, "Death",mainInfo[playerid][deaths]);
    cache_get_value_name_int(0, "Cents",mainInfo[playerid][centsLevel]);
    cache_get_value_name(0, "NickName", strNick);
    format(mainInfo[playerid][nickName], sizeof(strNick), "%s", strNick);
    cache_get_value_name(0, "Clan", strClan);
    format(mainInfo[playerid][clan], sizeof(strClan), "%s", strClan);
    cache_get_value_name_int(0, "Member",mainInfo[playerid][member]);
    cache_get_value_name_int(0, "Rank",mainInfo[playerid][rank]);
    cache_get_value_name_int(0, "AccessLevel",mainInfo[playerid][accessLevel]);
    cache_get_value_name_int(0, "ExpLevel",mainInfo[playerid][expLevel]);
    cache_get_value_name_int(0, "Radiation",mainInfo[playerid][radiation]);
    cache_get_value_name_int(0, "Hunger",mainInfo[playerid][hunger]);
    cache_get_value_name_int(0, "Thirst",mainInfo[playerid][thirst]);
    cache_get_value_name_int(0, "Virus",mainInfo[playerid][virus]);
    cache_get_value_name_int(0, "Ban",mainInfo[playerid][ban]);
    cache_get_value_name_int(0, "Mute",mainInfo[playerid][mute]);
    cache_get_value_name_int(0, "Jail",mainInfo[playerid][jail]);
    cache_get_value_name_float(0, "Health",mainInfo[playerid][health]);
    cache_get_value_name_int(0, "WarnLevel",mainInfo[playerid][warnLevel]);
    cache_get_value_name_int(0, "DonateLevel",mainInfo[playerid][donateLevel]);
    cache_get_value_name_int(0, "VipLevel",mainInfo[playerid][vipLevel]);
    cache_get_value_name_int(0, "HoursLevel",mainInfo[playerid][hoursLevel]);
    mainInfo[playerid][logged] = true;
    TogglePlayerSpectating(playerid, false);
    SetPlayerScore(playerid, mainInfo[playerid][accessLevel]);
    GivePlayerMoney(playerid, mainInfo[playerid][money]);
    SetSpawnInfo(playerid, 1, mainInfo[playerid][skin], 613.1469,-1415.0784,13.5688, 269.15, 0, 0, 0, 0, 0, 0);
    if (mainInfo[playerid][ban] > 0)
    {
        new stringBan[128];
        format(stringBan,sizeof(stringBan),"Срок действия бана закончится через %i час(-a/-ов)",mainInfo[playerid][ban]);
        SCM(playerid,COLOR_SYSTEM,stringBan);
        Kick(playerid);
    }
    new queryInventory[512];
    mysql_format(connection, queryInventory, sizeof(queryInventory), "SELECT * FROM `inventory` WHERE Name = '%e'", playerName(playerid));
    mysql_tquery(connection, queryInventory, "loadInventory", "d", playerid);
    return true;
}
public loadInventory(playerid)
{
    cache_get_value_name_int(0, "AntiRadin", invInfo[playerid][antiRadin]);
    cache_get_value_name_int(0, "Alcohol",invInfo[playerid][alcohol]);
    cache_get_value_name_int(0, "Weed",invInfo[playerid][weed]);
    cache_get_value_name_int(0, "Food",invInfo[playerid][food]);
    cache_get_value_name_int(0, "Medcine",invInfo[playerid][medcine]);
    cache_get_value_name_int(0, "ZombiesBlood",invInfo[playerid][zombiesBlood]);
    cache_get_value_name_int(0, "Instruments",invInfo[playerid][instruments]);
    cache_get_value_name_int(0, "Canister",invInfo[playerid][canister]);
    cache_get_value_name_int(0, "MasterKey",invInfo[playerid][masterKey]);
    cache_get_value_name_int(0, "Bolt",invInfo[playerid][bolt]);
    cache_get_value_name_int(0, "SpecialAmmo",invInfo[playerid][specialAmmo]);
    cache_get_value_name_int(0, "Metal",invInfo[playerid][metal]);
    cache_get_value_name_int(0, "Wood",invInfo[playerid][wood]);
    cache_get_value_name_int(0, "Blanks",invInfo[playerid][blanks]);
    cache_get_value_name_int(0, "Drugs",invInfo[playerid][drugs]);
    cache_get_value_name_int(0, "Flesh",invInfo[playerid][flesh]);
    cache_get_value_name_int(0, "Fruits",invInfo[playerid][fruits]);
    cache_get_value_name_int(0, "Pumpkin",invInfo[playerid][pumpkin]);
    cache_get_value_name_int(0, "Croup",invInfo[playerid][croup]);
    cache_get_value_name_int(0, "Milk",invInfo[playerid][milk]);
    cache_get_value_name_int(0, "Egg",invInfo[playerid][egg]);
    cache_get_value_name_int(0, "Artefacts",invInfo[playerid][artefacts]);
    if (mainInfo[playerid][adminLevel] > 0) Iter_Add(admins,playerid);
    if (mainInfo[playerid][member] == 3 || mainInfo[playerid][member] == 6 || mainInfo[playerid][member] == 10) Iter_Add(governments,playerid);
    if (mainInfo[playerid][member] == 2 || mainInfo[playerid][member] == 8) Iter_Add(stalkers,playerid);
    if (mainInfo[playerid][member] == 4 || mainInfo[playerid][member] == 9) Iter_Add(bandits,playerid);
    if (mainInfo[playerid][member] == 1 || mainInfo[playerid][member] == 7) Iter_Add(monolites,playerid);
    getPlayerColor(playerid);
    SpawnPlayer(playerid);
    hideTextDraw(playerid);
    return true;
}
public saveAccount(playerid)
{
    if (mainInfo[playerid][logged] == false) return true;
    mainInfo[playerid][money] = GetPlayerMoney(playerid);
    new Float: healthLevel;
    GetPlayerHealth(playerid, healthLevel);
    ResetPlayerMoney(playerid);
    new querymainInfo[2048];
    static str[] = "UPDATE `account` SET `Name`='%e', `Password`='%e', `Email`='%e', `Admin`=%d, `Sex`=%d, `Age`=%d,`Money`=%d, `Skin`=%d, `Kill`=%d,`Death`=%d, \
    `Cents`=%d,`NickName`='%e',`Member`=%d,`Rank`=%d,`WarnLevel`=%d,`Clan`='%e',`DonateLevel`=%d,`VipLevel`=%d,`HoursLevel`=%d,`AccessLevel`=%d,`ExpLevel`=%d, \
    `Radiation`=%d, `Hunger`=%d, `Thirst`=%d, `Virus`=%d, `Health`=%f,`Ban`=%d,`Mute`=%d,`Jail`=%d WHERE `Name`='%e'";
    mysql_format(connection, querymainInfo, sizeof(querymainInfo), str,playerName(playerid),mainInfo[playerid][password],mainInfo[playerid][mail],mainInfo[playerid][adminLevel],
    mainInfo[playerid][sex],mainInfo[playerid][age],mainInfo[playerid][money],
    mainInfo[playerid][skin],mainInfo[playerid][kills],mainInfo[playerid][deaths],mainInfo[playerid][centsLevel],
    mainInfo[playerid][nickName],mainInfo[playerid][member],
    mainInfo[playerid][rank],mainInfo[playerid][warnLevel],
    mainInfo[playerid][clan],mainInfo[playerid][donateLevel],mainInfo[playerid][vipLevel],mainInfo[playerid][hoursLevel],
    mainInfo[playerid][accessLevel],mainInfo[playerid][expLevel],mainInfo[playerid][radiation],mainInfo[playerid][hunger],
    mainInfo[playerid][thirst],mainInfo[playerid][virus],healthLevel,
    mainInfo[playerid][ban],mainInfo[playerid][mute],mainInfo[playerid][jail],playerName(playerid));
    mysql_tquery(connection, querymainInfo, "", "d", playerid);
    new queryinvInfo[1024];
    mysql_format(connection, queryinvInfo, sizeof(queryinvInfo), "UPDATE `inventory` SET `AntiRadin`=%d, \
    `Alcohol`=%d, `Weed`=%d, `Food`=%d, `Medcine`=%d, `ZombiesBlood`=%d, `Instruments`=%d, `Canister`=%d, `MasterKey`=%d \
    , `Bolt`=%d, `SpecialAmmo`=%d, `Metal`=%d,`Wood`=%d, `Blanks`=%d, `Drugs`=%d, `Flesh`=%d, `Fruits`=%d \
    , `Pumpkin`=%d, `Croup`=%d, `Milk`=%d, `Egg`=%d, `Artefacts`=%d WHERE `Name`='%e'",
    invInfo[playerid][antiRadin],invInfo[playerid][alcohol],invInfo[playerid][weed],invInfo[playerid][food],
    invInfo[playerid][medcine],invInfo[playerid][zombiesBlood],invInfo[playerid][instruments],
    invInfo[playerid][canister],invInfo[playerid][masterKey],invInfo[playerid][bolt],
    invInfo[playerid][specialAmmo],invInfo[playerid][metal],invInfo[playerid][wood],invInfo[playerid][blanks],
    invInfo[playerid][drugs],invInfo[playerid][flesh],invInfo[playerid][fruits],
    invInfo[playerid][pumpkin],invInfo[playerid][croup],invInfo[playerid][milk],
    invInfo[playerid][egg],invInfo[playerid][artefacts],playerName(playerid));
    mysql_tquery(connection, queryinvInfo, "", "d", playerid);
    mainInfo[playerid][logged] = false;
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
    format(strRadiation, sizeof(strRadiation), "%i", radiationTD);
    TextDrawSetString(mainText[playerid][1], strRadiation);
    new hungerTD,strHunger[8];
    cache_get_value_name_int(0, "Hunger", hungerTD);
    format(strHunger, sizeof(strHunger), "%i", hungerTD);
    TextDrawSetString(mainText[playerid][3], strHunger);
    new thirstTD,strThirst[8];
    cache_get_value_name_int(0, "Thirst", thirstTD);
    format(strThirst, sizeof(strThirst), "%i", thirstTD);
    TextDrawSetString(mainText[playerid][5], strThirst);
    new virusTD,strVirus[8];
    cache_get_value_name_int(0, "Virus", virusTD);
    format(strVirus, sizeof(strVirus), "%i", virusTD);
    TextDrawSetString(mainText[playerid][7], strVirus);
    for (new i = 0; i < 8; i++) TextDrawShowForPlayer(playerid,mainText[playerid][i]);
    return true;
}

public OnCheatDetected(playerid, ip_address[], type, code)
{
    if (type) BlockIpAddress(ip_address, 0);
    else
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
                if (mainInfo[playerid][adminLevel] > 0) return true;
                new Float:x, Float:y, Float:z;
                AntiCheatGetPos(playerid, x, y, z);
                SetPlayerPos(playerid, x, y, z);
                return 1;
            }
            case 40: SendClientMessage(playerid, -1, MAX_CONNECTS_MSG);
            case 41: SendClientMessage(playerid, -1, UNKNOWN_CLIENT_MSG);
            default:
            {
                if (mainInfo[playerid][adminLevel] > 0) return true;
                new strtmp[sizeof KICK_MSG];
                format(strtmp, sizeof strtmp, KICK_MSG, code);
                SCM(playerid, -1, strtmp);
            }
        }
        AntiCheatKickWithDesync(playerid, code);
    }
    return 1;
}

public onPlayerUpdateInMinute()
{
    new Float:x, Float:y, Float:z, Float: hp, str[8], hour, minute, second;
    gettime(hour, minute, second);
    if (minute == 0) 
    {
        SetWorldTime(hour);
        payDay();
    }
    foreach(Player, i)
    {
        if (mainInfo[i][logged] == false) continue;
        playingTime[i]++;
        if (mainInfo[i][jail] == 0) 
        {
            mainInfo[i][jail] = -1;
            switch (mainInfo[i][member])
            {
                case 1:
                {
                    SetPlayerInterior(i,17);
                    SetPlayerPos(i,-951.5187, 1867.1998, 5.9427);
                }
                case 2: 
                {
                    SetPlayerInterior(i,0);
                    SetPlayerPos(i,419.6006, 2536.7456, 10.6572);
                }
                case 4,9: 
                {
                    SetPlayerInterior(i,17);
                    SetPlayerPos(i,488.3408, -14.8237, 1001.6078);
                }
                case 3,10: 
                {
                    SetPlayerInterior(i,5);
                    SetPlayerPos(i,770.4814, 2.5649, 1000.9934);
                }
                case 6:
                {
                    SetPlayerInterior(i,0);
                    SetPlayerPos(i,226.5424, 1822.6515, 9.0628);
                }
                case 7: 
                {
                    SetPlayerInterior(i,0);
                    SetPlayerPos(i,-707.7855, 979.1034, 7.7314);
                } 
                case 8: 
                {
                    SetPlayerInterior(i,0);
                    SetPlayerPos(i,104.2438, 1045.7549, 2.3346);
                }
            }
        }
        if (mainInfo[i][mute] > 0) 
        {
            new string[64];
            mainInfo[i][mute]--;
            format(string,sizeof(string),"До конца срока действия бана чата осталось: %i мин",mainInfo[i][mute]);
            SCM(i,COLOR_SYSTEM,string);
        }
        if (mainInfo[i][jail] > 0) 
        {
            new string[64];
            mainInfo[i][jail]--;
            format(string,sizeof(string),"До конца срока пребывания в КПЗ осталось: %i мин",mainInfo[i][jail]);
            SCM(i,COLOR_SYSTEM,string);
        }
        if (mainInfo[i][hunger] <= 0 || mainInfo[i][hunger] <= 0 && mainInfo[i][thirst] <= 0 ||
        mainInfo[i][radiation] >= 100 && mainInfo[i][hunger] <= 0 || mainInfo[i][hunger] <= 0 && mainInfo[i][thirst] <= 0 ||
        mainInfo[i][radiation] >= 100 && mainInfo[i][hunger] <= 0 && mainInfo[i][thirst] <= 0)
        {
            mainInfo[i][hunger] = 30;
            mainInfo[i][thirst] = 30;
            SetPlayerHealth(i,0);
            SCM(i,COLOR_RED,"Вы погибли с голоду");
            format(str,sizeof(str),"%i",mainInfo[i][radiation]);
            TextDrawSetString(mainText[i][1], str);
            format(str,sizeof(str),"%i",mainInfo[i][hunger]);
            TextDrawSetString(mainText[i][3], str);
            format(str,sizeof(str),"%i",mainInfo[i][thirst]);
            TextDrawSetString(mainText[i][5], str);
            format(str,sizeof(str),"%i",mainInfo[i][virus]);
            TextDrawSetString(mainText[i][7], str);
            continue;
        }
        if (mainInfo[i][thirst] <= 0)
        {
            mainInfo[i][hunger] = 30;
            mainInfo[i][thirst] = 30;
            SetPlayerHealth(i,0);
            SCM(i,COLOR_RED,"Вы погибли от жажды");
            format(str,sizeof(str),"%i",mainInfo[i][radiation]);
            TextDrawSetString(mainText[i][1], str);
            format(str,sizeof(str),"%i",mainInfo[i][hunger]);
            TextDrawSetString(mainText[i][3], str);
            format(str,sizeof(str),"%i",mainInfo[i][thirst]);
            TextDrawSetString(mainText[i][5], str);
            format(str,sizeof(str),"%i",mainInfo[i][virus]);
            TextDrawSetString(mainText[i][7], str);
            continue;
        }
        if (mainInfo[i][radiation] >= 100)
        {
            mainInfo[i][hunger] = 30;
            mainInfo[i][thirst] = 30;
            mainInfo[i][radiation] = 30;
            SetPlayerHealth(i,0);
            SCM(i,COLOR_RED,"Вы погибли от радиации");
            format(str,sizeof(str),"%i",mainInfo[i][radiation]);
            TextDrawSetString(mainText[i][1], str);
            format(str,sizeof(str),"%i",mainInfo[i][hunger]);
            TextDrawSetString(mainText[i][3], str);
            format(str,sizeof(str),"%i",mainInfo[i][thirst]);
            TextDrawSetString(mainText[i][5], str);
            format(str,sizeof(str),"%i",mainInfo[i][virus]);
            TextDrawSetString(mainText[i][7], str);
            continue;
        }
        mainInfo[i][hunger] -= 1;
        mainInfo[i][thirst] -= 1;   
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
            if (mainInfo[i][member] != 6 ||  mainInfo[i][member] != 7 || mainInfo[i][member] != 8) mainInfo[i][radiation] += 3;
            mainInfo[i][hunger] -= 2;
            mainInfo[i][thirst] -= 4;
            mainInfo[i][health] = hp;
            SetPlayerHealth(i, hp);
        } 
        else if ((x >= -1517.578125 && x <= 925.78125 && y >= 550.78125 && y <= 2923.828125) ||
        (x >= -2982.421875 && x <= -1505.859375 && y >= 1505.859375 && y <= 2923.828125))
        {
            if (mainInfo[i][member] != 6 ||  mainInfo[i][member] != 7 || mainInfo[i][member] != 8) mainInfo[i][radiation] += 1;
            mainInfo[i][hunger] -= 1;
            mainInfo[i][thirst] -= 2;
        }
        format(str,sizeof(str),"%i",mainInfo[i][radiation]);
        TextDrawSetString(mainText[i][1], str);
        format(str,sizeof(str),"%i",mainInfo[i][hunger]);
        TextDrawSetString(mainText[i][3], str);
        format(str,sizeof(str),"%i",mainInfo[i][thirst]);
        TextDrawSetString(mainText[i][5], str);
        format(str,sizeof(str),"%i",mainInfo[i][virus]);
        TextDrawSetString(mainText[i][7], str); 
    }
    return true;
}
public payDay()
{
    foreach(Player, i)
    {
        if (playingTime[i] < 30)
        {
            playingTime[i] = 0;
            SCM(i,COLOR_RED,"Необходимо находиться в игре минимум полчаса для получения зарплаты");
            continue;
        }
        mainInfo[i][expLevel]++;
        SCM(i,COLOR_YELLOW,"Вы провели еще один час в зоне");
        playingTime[i] = 0;
    }
    return true;
}
//Функции скрипта
stock hideTextDraw(playerid)
{
    CancelSelectTextDraw(playerid);
    for (new i = 0; i < sizeof(registration); i++) TextDrawHideForPlayer(playerid, registration[i]);
    for (new i = 0; i < sizeof(background); i++) TextDrawHideForPlayer(playerid, background[i]);
    for (new i = 0; i < 7; i++) TextDrawHideForPlayer(playerid, login[playerid][i]);
    return true;
}
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

stock memberName(playerid)
{
    new string[22];
    switch(mainInfo[playerid][member])
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
    switch (mainInfo[playerid][adminLevel])
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
	switch(mainInfo[playerid][member])
	{
        case 0: string = "Одиночка";
		case 1:
		{
			switch(mainInfo[playerid][rank])
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
			switch(mainInfo[playerid][rank])
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
			switch(mainInfo[playerid][rank])
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
			switch(mainInfo[playerid][rank])
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
			switch(mainInfo[playerid][rank])
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
			switch(mainInfo[playerid][rank])
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
			switch(mainInfo[playerid][rank])
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
			switch(mainInfo[playerid][rank])
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
			switch(mainInfo[playerid][rank])
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
			switch(mainInfo[playerid][rank])
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
    switch (mainInfo[playerid][member])
    {
        case 1:
        {
            switch(mainInfo[playerid][rank])
            {
                case 1..3:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 161;
                    else mainInfo[playerid][skin] = 77;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 4..6:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 78;
                    else mainInfo[playerid][skin] = 77;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 7..9:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 162;
                    else mainInfo[playerid][skin] = 77;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 10..12:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 160;
                    else mainInfo[playerid][skin] = 77;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 13..14:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 213;
                    else mainInfo[playerid][skin] = 77;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 15:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 62;
                    else mainInfo[playerid][skin] = 298;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 16:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 230;
                    else mainInfo[playerid][skin] = 298;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }       
            }
        }
        case 2:
        {
            switch(mainInfo[playerid][rank])
            {
                case 1..3:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 60;
                    else mainInfo[playerid][skin] = 41;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 4..7:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 101;
                    else mainInfo[playerid][skin] = 41;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 8..13:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 206;
                    else mainInfo[playerid][skin] = 41;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 14..15:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 29;
                    else mainInfo[playerid][skin] = 41;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
            }
        }
        case 3:
        {
            switch(mainInfo[playerid][rank])
            {
                case 1:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 287;
                    else mainInfo[playerid][skin] = 191;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                    
                }
                case 2:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 73;
                    else mainInfo[playerid][skin] = 191;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 3..6:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 179;
                    else mainInfo[playerid][skin] = 191;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 7..8:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 255;
                    else mainInfo[playerid][skin] = 191;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 9:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 282;
                    else mainInfo[playerid][skin] = 191;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 10..11:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 282;
                    else mainInfo[playerid][skin] = 309;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 12..13:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 302;
                    else mainInfo[playerid][skin] = 309;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 14..15:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 311;
                    else mainInfo[playerid][skin] = 309;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 16:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 288;
                    else mainInfo[playerid][skin] = 309;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
            }
        }
        case 4:
        {
            switch(mainInfo[playerid][rank])
            {
                case 1..2:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 181;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 3:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 248;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 4:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 247;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 5:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 100;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 6:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 119;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 7:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 186;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 8:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 121;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 9:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 122;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 10:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 112;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 11:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 126;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 12:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 124;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 13:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 125;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 14:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 120;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 15:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 111;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 16:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 113;
                    else mainInfo[playerid][skin] = 64;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
            }    
        }
        case 6:
        {
            SetPlayerAttachedObject(playerid, 0, 19163, 2, 0.027000, -0.005999, 0.004000, -0.999999, 85.600036, -5.299999, 1.279006, 1.000000, 1.000000);
            SetPlayerAttachedObject(playerid, 1, 368, 2, -0.029000, -0.020999, 0.000000, -4.400000, -4.599998, -1.900000, 1.000000, 1.861003, 0.921000);
            SetPlayerAttachedObject(playerid, 2, 19163, 2, -0.005999, 0.013000, 0.005999, 0.000000, 88.400001, -7.199999, 1.051998, 1.108999, 1.902001);
            SetPlayerAttachedObject(playerid, 3, 19472, 2, 0.000000, 0.165000, 0.004000, 105.199806, 78.900024, 0.000000, 1.000000, 1.000000, 1.000000);
            SetPlayerAttachedObject(playerid, 4, 1010, 1, 0.000000, -0.045000, 0.000000, 88.900047, -2.099987, -177.900039, 1.000000, 1.000000, 1.000000);
            switch(mainInfo[playerid][rank])
            {
                case 1..6:
                {
                    mainInfo[playerid][skin] = 279;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
            }
        }
        case 7:
        {
            SetPlayerAttachedObject(playerid, 0, 19472, 2, 0.026999, 0.151000, 0.005000, 85.299995, 76.299980, 0.000000, 1.000000, 0.889001, 1.000000);
            SetPlayerAttachedObject(playerid, 1, 368, 2, 0.000000, 0.087000, -0.005000, -4.400000, -4.599998, -1.900000, 1.000000, 0.676000, 0.916000);
            SetPlayerAttachedObject(playerid, 2, 19163, 2, 0.047999, 0.057999, 0.017000, 0.000000, 88.400001, -7.199999, 0.937000, 0.832999, 1.000000);
            SetPlayerAttachedObject(playerid, 3, 3026, 1, -0.130999, -0.084000, 0.008000, 5.300004, 0.099998, -0.899996, 1.000000, 1.000000, 0.807001);
            switch(mainInfo[playerid][rank])
            {
                case 1..6:
                {
                    mainInfo[playerid][skin] = 285;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
            }
        }
        case 8:
        {
            switch(mainInfo[playerid][rank])
            {
                case 1..6:
                {
                    mainInfo[playerid][skin] = 29;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
            }
        }
        case 9:
        {
            switch(mainInfo[playerid][rank])
            {
                case 1:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 98;
                    else mainInfo[playerid][skin] = 76;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 2:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 163;
                    else mainInfo[playerid][skin] = 76;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 3:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 164;
                    else mainInfo[playerid][skin] = 76;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 4:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 166;
                    else mainInfo[playerid][skin] = 76;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 5:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 165;
                    else mainInfo[playerid][skin] = 76;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 6:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 171;
                    else mainInfo[playerid][skin] = 76;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
            }
        }
        case 10:
        {
            switch(mainInfo[playerid][rank])
            {
                case 1..5:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 70;
                    else mainInfo[playerid][skin] = 91;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
                case 6:
                {
                    if (mainInfo[playerid][sex] == 1) mainInfo[playerid][skin] = 290;
                    else mainInfo[playerid][skin] = 91;
                    SetPlayerSkin(playerid, mainInfo[playerid][skin]);
                }
            }
        }
    }
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
                if (mainInfo[i][member] == 3)
                {
                    SCM(i,0x228B22FF, string);
                    continue;
                }
            }
            case 6:
            {
                if (mainInfo[i][member] == 6)
                {
                    SCM(i, 0x228B22FF, string);
                    continue;
                }
            }
            case 10:
            {
                if (mainInfo[i][member] == 10)
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
                if (mainInfo[i][member] == 2)
                {
                    SCM(i,0x228B22FF, string);
                    continue;
                }
            }
            case 8:
            {
                if (mainInfo[i][member] == 8)
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
                if (mainInfo[i][member] == 4)
                {
                    SCM(i,0x228B22FF, string);
                    continue;
                }
            }
            case 8:
            {
                if (mainInfo[i][member] == 9)
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
                if (mainInfo[i][member] == 1)
                {
                    SCM(i,0x228B22FF, string);
                    continue;
                }
            }
            case 7:
            {
                if (mainInfo[i][member] == 7)
                {
                    SCM(i, 0x228B22FF, string);
                    continue;
                }
            }
        }       
    }
    return true;
}

stock getPlayerColor(playerid)
{
    switch(mainInfo[playerid][member])
    {
        case 0: SetPlayerColor(playerid, 0xFFFFFFAA);
        case 1,7: SetPlayerColor(playerid, 0xB22222AA);
        case 2,8: SetPlayerColor(playerid, 0xEE7600AA);
        case 3: SetPlayerColor(playerid, 0x006400AA);
        case 4,9: SetPlayerColor(playerid, 0x0000CDAA);
        case 5: SetPlayerColor(playerid, 0x1C1C1CAA);
        case 6,10: SetPlayerColor(playerid, 0x006400AA);
    }
    return true;
}

stock isLeader(playerid)
{
    switch (mainInfo[playerid][member])
    {
        case 1..4: if (mainInfo[playerid][rank] == 16) return true;
        case 6..10: if (mainInfo[playerid][rank] == 6) return true;
    }
    return false;
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

    background[4] = TextDrawCreate(250.000000, 370.000000, "(c) Server.exe 2019");
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
    mainText[playerid][0] = TextDrawCreate(550.000000, 100.000000, "hud:radar_girlfriend");
    TextDrawBackgroundColor(mainText[playerid][0], 255);
    TextDrawFont(mainText[playerid][0], 4);
    TextDrawLetterSize(mainText[playerid][0], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][0], -16776961);
    TextDrawSetOutline(mainText[playerid][0], 0);
    TextDrawSetProportional(mainText[playerid][0], 1);
    TextDrawSetShadow(mainText[playerid][0], 1);
    TextDrawTextSize(mainText[playerid][0], 10.000000, 10.000000);
    TextDrawSetSelectable(mainText[playerid][0], 0);

    mainText[playerid][1] = TextDrawCreate(550.000000, 110.000000, "");
    TextDrawBackgroundColor(mainText[playerid][1], 255);
    TextDrawFont(mainText[playerid][1], 1);
    TextDrawLetterSize(mainText[playerid][1], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][1], -16776961);
    TextDrawSetOutline(mainText[playerid][1], 0);
    TextDrawSetProportional(mainText[playerid][1], 1);
    TextDrawTextSize(mainText[playerid][1], 5.000000, 5.000000);
    TextDrawSetShadow(mainText[playerid][1], 1);
    TextDrawSetSelectable(mainText[playerid][1], 0);
    
    
    mainText[playerid][2] = TextDrawCreate(500.000000, 100.000000, "hud:radar_burgerShot");
    TextDrawBackgroundColor(mainText[playerid][2], 255);
    TextDrawFont(mainText[playerid][2], 4);
    TextDrawLetterSize(mainText[playerid][2], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][2], -1);
    TextDrawSetOutline(mainText[playerid][2], 0);
    TextDrawSetProportional(mainText[playerid][2], 1);
    TextDrawSetShadow(mainText[playerid][2], 1);
    TextDrawTextSize(mainText[playerid][2], 10.000000, 10.000000);
    TextDrawSetSelectable(mainText[playerid][2], 0);

    mainText[playerid][3] = TextDrawCreate(500.000000, 110.000000, "");
    TextDrawBackgroundColor(mainText[playerid][3], 255);
    TextDrawFont(mainText[playerid][3], 1);
    TextDrawLetterSize(mainText[playerid][3], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][3], 852308735);
    TextDrawSetOutline(mainText[playerid][3], 0);
    TextDrawSetProportional(mainText[playerid][3], 1);
    TextDrawTextSize(mainText[playerid][3], 5.000000, 5.000000);
    TextDrawSetShadow(mainText[playerid][3], 1);
    TextDrawSetSelectable(mainText[playerid][3], 0);
    
    mainText[playerid][4] = TextDrawCreate(520.000000, 100.000000, "hud:radar_diner");
    TextDrawBackgroundColor(mainText[playerid][4], 255);
    TextDrawFont(mainText[playerid][4], 4);
    TextDrawLetterSize(mainText[playerid][4], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][4], -1);
    TextDrawSetOutline(mainText[playerid][4], 0);
    TextDrawSetProportional(mainText[playerid][4], 1);
    TextDrawSetShadow(mainText[playerid][4], 1);
    TextDrawTextSize(mainText[playerid][4], 10.000000, 10.000000);
    TextDrawSetSelectable(mainText[playerid][4], 0);

    mainText[playerid][5] = TextDrawCreate(520.000000, 110.000000, "");
    TextDrawBackgroundColor(mainText[playerid][5], 255);
    TextDrawFont(mainText[playerid][5], 1);
    TextDrawLetterSize(mainText[playerid][5], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][5], 16777215);
    TextDrawSetOutline(mainText[playerid][5], 0);
    TextDrawSetProportional(mainText[playerid][5], 1);
    TextDrawTextSize(mainText[playerid][5], 5.000000, 5.000000);
    TextDrawSetShadow(mainText[playerid][5], 1);
    TextDrawSetSelectable(mainText[playerid][5], 0);

    mainText[playerid][6] = TextDrawCreate(540.000000, 100.000000, "hud:radar_LocoSyndicate");
    TextDrawBackgroundColor(mainText[playerid][6], 255);
    TextDrawFont(mainText[playerid][6], 4);
    TextDrawLetterSize(mainText[playerid][6], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][6], -3407617);
    TextDrawSetOutline(mainText[playerid][6], 0);
    TextDrawSetProportional(mainText[playerid][6], 1);
    TextDrawSetShadow(mainText[playerid][6], 1);
    TextDrawTextSize(mainText[playerid][6], 10.000000, 10.000000);
    TextDrawSetSelectable(mainText[playerid][6], 0);

    mainText[playerid][7] = TextDrawCreate(540.000000, 110.000000, "");
    TextDrawBackgroundColor(mainText[playerid][7], 255);
    TextDrawFont(mainText[playerid][7], 1);
    TextDrawLetterSize(mainText[playerid][7], 0.500000, 1.000000);
    TextDrawColor(mainText[playerid][7], -3407617);
    TextDrawSetOutline(mainText[playerid][7], 0);
    TextDrawSetProportional(mainText[playerid][7], 1);
    TextDrawTextSize(mainText[playerid][7], 5.000000, 5.000000);
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
    mainInfo[playerid][money] += amount;
    GivePlayerMoney(playerid, amount);
}
stock firstSkin(playerid)
{
    for(new i = 0; i < sizeof(registration); i++) TextDrawHideForPlayer(playerid, registration[i]);
    for(new i = 0; i < sizeof(background); i++) TextDrawHideForPlayer(playerid, background[i]);
    if(mainInfo[playerid][sex])
    {
        SetPlayerSkin(playerid, 26);
        mainInfo[playerid][skin] = 26;
    }
    else
    {
        SetPlayerSkin(playerid, 12);
        mainInfo[playerid][skin] = 12;
    }
    cskin[playerid] = 0;
    SetSpawnInfo(playerid, 0, mainInfo[playerid][skin], 1971.5336,1424.0586,76.1563, 266.4810, 0, 0, 0, 0, 0, 0);
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

