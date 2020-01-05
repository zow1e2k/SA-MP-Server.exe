#include <a_samp>
#include <foreach>

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
