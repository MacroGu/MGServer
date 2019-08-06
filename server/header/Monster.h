#pragma once

#include "CommonClass.h"
#include "defines.h"

class Monster
{
public:
	Monster();
	virtual ~Monster();

	// 玩家 Move to
	void MoveTo(const cCharacter& target);
	// 命中玩家
	void HitPlayer(cCharacter& target);
	// 伤害
	void Damaged(float damage);
	// 是否还活着
	bool IsAlive();
	// 是否受到攻击
	bool IsAttacking();
	// 是否在可跟踪范围内
	bool IsPlayerInTraceRange(const cCharacter& target);
	// 是否在攻击范围内
	bool IsPlayerInHitRange(const cCharacter& target);
	// 设置位置
	void SetLocation(float x, float y, float z);

	friend ostream& operator<<(ostream& stream, Monster& info)
	{
		stream << info.X << endl;
		stream << info.Y << endl;
		stream << info.Z << endl;
		stream << info.Health << endl;
		stream << info.Id << endl;
		stream << info.bIsAttacking << endl;

		return stream;
	}

	friend istream& operator>>(istream& stream, Monster& info)
	{
		stream >> info.X;
		stream >> info.Y;
		stream >> info.Z;
		stream >> info.Health;
		stream >> info.Id;
		stream >> info.bIsAttacking;

		return stream;
	}

	float	X;				// X坐标
	float	Y;				// Y坐标
	float	Z;				// Z坐标
	float	Health;			// 生命
	int		Id;				// 唯一 id
	float	TraceRange;		// 跟踪范围
	float	HitRange;		// 攻击范围
	float	MovePoint;		// 移动点
	float	HitPoint;		// 攻击点
	bool	bIsAttacking;	// 是否在攻击

private:
	bool	bIsTracking;	// 是否在追踪
};

class MonsterSet
{
public:
	map<int, Monster> monsters;

	friend ostream& operator<<(ostream& stream, MonsterSet& info)
	{
		stream << info.monsters.size() << endl;
		for (auto& kvp : info.monsters)
		{
			stream << kvp.first << endl;
			stream << kvp.second << endl;
		}

		return stream;
	}

	friend istream& operator>>(istream& stream, MonsterSet& info)
	{
		int nMonsters = 0;
		int PrimaryId = 0;
		Monster monster;
		info.monsters.clear();

		stream >> nMonsters;
		for (int i = 0; i < nMonsters; i++)
		{
			stream >> PrimaryId;
			stream >> monster;
			info.monsters[PrimaryId] = monster;
		}

		return stream;
	}
};