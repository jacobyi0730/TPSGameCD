// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"

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

}

void UEnemyAnim::AnimNotify_DieEnd()
{

}
