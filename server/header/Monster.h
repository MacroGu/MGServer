#pragma once

#include "CommonClass.h"
#include "defines.h"

class Monster
{
public:
	Monster();
	virtual ~Monster();

	// ��� Move to
	void MoveTo(const cCharacter& target);
	// �������
	void HitPlayer(cCharacter& target);
	// �˺�
	void Damaged(float damage);
	// �Ƿ񻹻���
	bool IsAlive();
	// �Ƿ��ܵ�����
	bool IsAttacking();
	// �Ƿ��ڿɸ��ٷ�Χ��
	bool IsPlayerInTraceRange(const cCharacter& target);
	// �Ƿ��ڹ�����Χ��
	bool IsPlayerInHitRange(const cCharacter& target);
	// ����λ��
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

	float	X;				// X����
	float	Y;				// Y����
	float	Z;				// Z����
	float	Health;			// ����
	int		Id;				// Ψһ id
	float	TraceRange;		// ���ٷ�Χ
	float	HitRange;		// ������Χ
	float	MovePoint;		// �ƶ���
	float	HitPoint;		// ������
	bool	bIsAttacking;	// �Ƿ��ڹ���

private:
	bool	bIsTracking;	// �Ƿ���׷��
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