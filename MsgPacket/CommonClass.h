#pragma once

#include <iostream>
#include <map>

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

	friend std::ostream& operator<<(std::ostream& stream, cCharacter& info)
	{
		stream << info.SessionId << std::endl;
		stream << info.X << std::endl;
		stream << info.Y << std::endl;
		stream << info.Z << std::endl;
		stream << info.VX << std::endl;
		stream << info.VY << std::endl;
		stream << info.VZ << std::endl;
		stream << info.Yaw << std::endl;
		stream << info.Pitch << std::endl;
		stream << info.Roll << std::endl;
		stream << info.IsAlive << std::endl;
		stream << info.HealthValue << std::endl;
		stream << info.IsAttacking << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, cCharacter& info)
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

	std::map<int, cCharacter> players;

	friend std::ostream& operator<<(std::ostream& stream, cCharactersInfo& info)
	{
		stream << info.players.size() << std::endl;
		for (auto& kvp : info.players)
		{
			stream << kvp.first << std::endl;
			stream << kvp.second << std::endl;
		}

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, cCharactersInfo& info)
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
