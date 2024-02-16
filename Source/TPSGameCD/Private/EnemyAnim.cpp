// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"
#include "TPSPlayer.h"

void UEnemyAnim::NativeInitializeAnimation()
{
	auto owner = TryGetPawnOwner();
	if (nullptr == owner)
		return;

	AEnemy* me = Cast<AEnemy>( owner );
	enemyFSM = me->enemyFSM;
}

void UEnemyAnim::NativeUpdateAnimation( float DeltaSeconds )
{
	if (nullptr == enemyFSM)
		return;

	state = enemyFSM->state;
}

void UEnemyAnim::AnimNotify_AttackEnd()
{
	bAttack = false;
	UE_LOG(LogTemp, Warning, TEXT("UEnemyAnim::AnimNotify_AttackEnd"));
}

void UEnemyAnim::AnimNotify_DamageEnd()
{
	enemyFSM->DoDamageEnd();
}

void UEnemyAnim::AnimNotify_DieEnd()
{
	enemyFSM->isDieDone = true;
}

void UEnemyAnim::AnimNotify_Hit()
{
	if (nullptr == enemyFSM)
		return;

	// 개선한다면
	// - 타격거리를 검증하고
	// - 발차기 방향과 플레이어와의 방향이 일치

	// 타겟에게 너 맞았어 라고 알려주고싶다.
	if (ATPSPlayer* player = Cast<ATPSPlayer>( enemyFSM->target ))
	{
		player->DamageProcess( 1 , enemyFSM->me );
	}
}
