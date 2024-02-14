// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSMComp.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "EnemyAnim.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"


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

	enemyAnim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	ai = Cast<AAIController>( me->GetController() );

	GetRandomChasePoint( chaseRadius , chasePoint );

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
		SetState( EEnemyState::MOVE );
	}
}

// Move : 목적지를 향해 이동하고싶다. 만약 목적지와의 거리가 3M이내라면 공격상태로 전이하고싶다.
void UEnemyFSMComp::TickMove()
{
	// 순찰 Patrol
	// 추적 Chase
	FVector destination = target->GetActorLocation();

	// 추적할 대상을 길찾기를 하고싶다.
	auto ns = UNavigationSystemV1::GetNavigationSystem( GetWorld() );
	FAIMoveRequest req;
	req.SetAcceptanceRadius( 50 );
	req.SetGoalLocation( destination );
	FPathFindingQuery query;
	ai->BuildPathfindingQuery( req , query );
	FPathFindingResult r = ns->FindPathSync( query );
	// 만약 목적지가 길 위라면
	if (r.Result == ENavigationQueryResult::Success )
	{
		// (추적) AI가 목적지를 향해 이동하게 하고싶다.
		ai->MoveToLocation( destination , 50 );
	}
	// 그렇지 않다면
	else
	{
		// (순찰) 랜덤한 위치를 향해 이동
		EPathFollowingRequestResult::Type chaseResult = ai->MoveToLocation( chasePoint , 50 );

		if (chaseResult == EPathFollowingRequestResult::AlreadyAtGoal ||
			chaseResult == EPathFollowingRequestResult::Failed)
		{
			GetRandomChasePoint( chaseRadius , chasePoint );
		}
	}

	// 0. 목적지를 향하는 방향을 구하고싶다.
	//FVector dir = target->GetActorLocation() - me->GetActorLocation();
	// 1. me가 그 방향으로 이동하게 하고싶다.
	//me->AddMovementInput( dir.GetSafeNormal() );
	
	
	
	// 2. 목적지와의 거리를 기억하고싶다.
	float dist = target->GetDistanceTo( me );
	// 3. 만약 목적지와의 거리가 attackDist보다 작다면
	if (dist < attackDist)
	{
		// 4. 공격상태로 전이하고싶다.
		SetState( EEnemyState::ATTACK );
		enemyAnim->bAttack = true;
	}
}

void UEnemyFSMComp::TickAttack()
{
	// 1. 시간이 흐르다가 
	currentTime += GetWorld()->GetDeltaSeconds();
	// 2. 만약 현재시간이 > 공격대기시간을 초과하면
	if (currentTime > attackWaitTime)
	{
		// 3. 현재시간을 초기화 하고싶다.
		currentTime = 0;
		// 4. 목적지와의 거리를 재고
		float distance = target->GetDistanceTo( me );
		// 5. 만약 그 거리가 공격가능거리를 초과한다면
		if (distance > attackDist)
		{
			// 6.   이동상태로 전이하고싶다.
			SetState( EEnemyState::MOVE );
		}
		else // 7. 그렇지 않다면
		{
			// 8.   공격을 하고싶다.
			enemyAnim->bAttack = true;
		}
	}
}

void UEnemyFSMComp::TickDamage()
{
	//// 시간이 흐르다가
	//currentTime += GetWorld()->GetDeltaSeconds();
	//// 현재시간이 1초가 되면 
	//if (currentTime > 1)
	//{
	//	// 이동상태로 전이하고싶다.
	//	SetState( EEnemyState::MOVE );
	//	me->GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	//}
}

void UEnemyFSMComp::TickDie()
{
	// 만약 죽음 애니메이션이 끝났다면
	if (false == isDieDone)
		return;

	// 아래로 이동하고싶다.
	float deltaTime = GetWorld()->GetDeltaSeconds();
	FVector P0 = me->GetActorLocation();
	FVector velocity = FVector::DownVector * 500;
	me->SetActorLocation( P0 + velocity * deltaTime );
	
	// 시간이 흐르다가
	currentTime += deltaTime;
	// 2초가 되면
	if (currentTime > 2)
	{
		// 스스로 파괴하고싶다.
		me->Destroy();
	}
}

void UEnemyFSMComp::DoDamageEnd()
{
	// 이동상태로 전이하고싶다.
	SetState( EEnemyState::MOVE );
	me->GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
}

void UEnemyFSMComp::TakeDamage( int damage )
{
	// 체력을 damage만큼 줄이고싶다.
	me->hp -= damage;
	if (me->hp < 0)
	{
		me->hp = 0;
	}
	// 만약 체력이 0보다 크다면 Damage상태로 전이하고싶다.
	if (me->hp > 0)
	{
		SetState( EEnemyState::DAMAGE );
		// 데미지 애니메이션 몽타주 재생
		int index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf( TEXT( "Damage%d" ), index ) ;
		me->PlayAnimMontage( enemyMontage, 1, FName(*sectionName) );
	}
	// 그렇지않다면(체력이 0이하라면) Die상태로 전이하고싶다.
	else
	{
		SetState( EEnemyState::DIE );
		// 죽음 애니메이션 몽타주 재생
		me->PlayAnimMontage( enemyMontage, 1, TEXT("Die") );
		isDieDone = false;
	}
	// 충돌체를 끄고싶다.
	me->GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
}

void UEnemyFSMComp::SetState( EEnemyState next )
{
	state = next;
	currentTime = 0;
}

bool UEnemyFSMComp::GetRandomChasePoint(float radius, FVector& outLoc)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem( GetWorld() );
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius( me->GetActorLocation() , radius , loc );
	if (result)
	{
		outLoc = loc.Location;
	}
	return result;
}

