#include <thread>
#include "Monster.h"


Monster::Monster()
	:X(0), Y(0), Z(0),
	Health(0), MovePoint(5), HitPoint(0.1f),
	TraceRange(700), HitRange(180),
	bIsAttacking(false),
	bIsTracking(false)
{
	// 分配唯一 ID
}


Monster::~Monster()
{
}

void Monster::MoveTo(const cCharacter& target)
{
	if (target.X > X)
		X += MovePoint;
	if (target.X < X)
		X -= MovePoint;
	if (target.Y > Y)
		Y += MovePoint;
	if (target.Y < Y)
		Y -= MovePoint;
	if (target.Z > Z)
		Z += MovePoint;
	if (target.Z < Z)
		Z -= MovePoint;
}

void Monster::HitPlayer(cCharacter& target)
{
	std::thread t([&]() {
		// 每秒攻击一次	
		bIsAttacking = true;
		LOG_INFO("攻击");
		target.HealthValue -= HitPoint;
		std::this_thread::sleep_for(1s);
		bIsAttacking = false;
		});
	t.detach();
}

void Monster::Damaged(float damage)
{
	Health -= damage;
	LOG_INFO("当前血量 : {}", Health);
}

bool Monster::IsAlive()
{
	if (Health <= 0)
		return false;

	return true;
}

bool Monster::IsAttacking()
{
	return bIsAttacking;
}

bool Monster::IsPlayerInTraceRange(const cCharacter& target)
{
	if (abs(target.X - X) < TraceRange && abs(target.Y - Y) < TraceRange)
		return true;

	return false;
}

bool Monster::IsPlayerInHitRange(const cCharacter& target)
{
	if (abs(target.X - X) < HitRange && abs(target.Y - Y) < HitRange)
		return true;

	return false;
}

void Monster::SetLocation(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}