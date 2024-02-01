// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSMComp.h"
#include "Enemy.h"

// Sets default values for this component's properties
UEnemyFSMComp::UEnemyFSMComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSMComp::BeginPlay()
{
	Super::BeginPlay();

	// 내 본체를 기억하고싶다.
	me = Cast<AEnemy>( GetOwner() );

}


void UEnemyFSMComp::TickComponent( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime , TickType , ThisTickFunction );

	switch (state)
	{
	case EEnemyState::IDLE:		TickIdle();			break;
	case EEnemyState::MOVE:		TickMove();			break;
	case EEnemyState::ATTACK:	TickAttack();		break;
	case EEnemyState::DAMAGE:	TickDamage();		break;
	case EEnemyState::DIE:		TickDie();			break;
	}
}

// Idle : 주인공을 찾아서 목적지로 기억하고싶다. 만약 목적지가 있다면 이동상태로 전이하고싶다.
void UEnemyFSMComp::TickIdle()
{
	// 1. 주인공을 찾아서 목적지로 기억하고싶다.
	target = GetWorld()->GetFirstPlayerController()->GetPawn();
	//target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	// 2. 만약 목적지가 있다면
	if (target)
	{
		// 3. 이동상태로 전이하고싶다. -> state의 값을 MOVE로 바꾸고싶다.
		state = EEnemyState::MOVE;
	}
}

// Move : 목적지를 향해 이동하고싶다. 만약 목적지와의 거리가 3M이내라면 공격상태로 전이하고싶다.
void UEnemyFSMComp::TickMove()
{
	// 0. 목적지를 향하는 방향을 구하고싶다.
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	// 1. me가 그 방향으로 이동하게 하고싶다.
	me->AddMovementInput(dir.GetSafeNormal());
	// 2. 목적지와의 거리를 기억하고싶다.
	float dist = target->GetDistanceTo(me);
	// 3. 만약 목적지와의 거리가 attackDist보다 작다면
	if (dist < attackDist)
	{
		// 4. 공격상태로 전이하고싶다.
		state = EEnemyState::ATTACK;
	}
}

// Attack : 2초 대기했다가 공격을 하고 싶다. 만약 공격을 하려고할때 목적지와의 거리가 3M를 초과한다면 이동상태로 전이하고싶다.
void UEnemyFSMComp::TickAttack()
{
}

void UEnemyFSMComp::TickDamage()
{
}

void UEnemyFSMComp::TickDie()
{
}

