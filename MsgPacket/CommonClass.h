#pragma once

#include <iostream>
#include <map>

using namespace std;

#define MAX_CLIENTS 100

enum EPacketType
{
	LOGIN,
	ENROLL_PLAYER,
	SEND_PLAYER,
	RECV_PLAYER,
	LOGOUT_PLAYER,
	HIT_PLAYER,
	DAMAGED_PLAYER,
	CHAT,
	ENTER_NEW_PLAYER,
	SIGNUP,
	HIT_MONSTER,
	SYNC_MONSTER,
	SPAWN_MONSTER,
	DESTROY_MONSTER
};

class cCharacter {
public:
	cCharacter();
	~cCharacter();

	// 会话 ID
	int		SessionId;
	// 位置
	float	X;
	float	Y;
	float	Z;
	// 旋转值
	float	Yaw;
	float	Pitch;
	float	Roll;
	// 速度
	float VX;
	float VY;
	float VZ;
	// 属性
	bool	IsAlive;
	float	HealthValue;
	bool	IsAttacking;

	friend ostream& operator<<(ostream& stream, cCharacter& info)
	{
		stream << info.SessionId << endl;
		stream << info.X << endl;
		stream << info.Y << endl;
		stream << info.Z << endl;
		stream << info.VX << endl;
		stream << info.VY << endl;
		stream << info.VZ << endl;
		stream << info.Yaw << endl;
		stream << info.Pitch << endl;
		stream << info.Roll << endl;
		stream << info.IsAlive << endl;
		stream << info.HealthValue << endl;
		stream << info.IsAttacking << endl;

		return stream;
	}

	friend istream& operator>>(istream& stream, cCharacter& info)
	{
		stream >> info.SessionId;
		stream >> info.X;
		stream >> info.Y;
		stream >> info.Z;
		stream >> info.VX;
		stream >> info.VY;
		stream >> info.VZ;
		stream >> info.Yaw;
		stream >> info.Pitch;
		stream >> info.Roll;
		stream >> info.IsAlive;
		stream >> info.HealthValue;
		stream >> info.IsAttacking;

		return stream;
	}
};

class cCharactersInfo
{
public:
	cCharactersInfo();
	~cCharactersInfo();

	map<int, cCharacter> players;

	friend ostream& operator<<(ostream& stream, cCharactersInfo& info)
	{
		stream << info.players.size() << endl;
		for (auto& kvp : info.players)
		{
			stream << kvp.first << endl;
			stream << kvp.second << endl;
		}

		return stream;
	}

	friend istream& operator>>(istream& stream, cCharactersInfo& info)
	{
		int nPlayers = 0;
		int SessionId = 0;
		cCharacter Player;
		info.players.clear();

		stream >> nPlayers;
		for (int i = 0; i < nPlayers; i++)
		{
			stream >> SessionId;
			stream >> Player;
			info.players[SessionId] = Player;
		}

		return stream;
	}
};

class CommonClass
{
public:
	CommonClass();
	~CommonClass();
};
