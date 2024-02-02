// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSMComp.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE	UMETA(DisplayName="대기"),
	MOVE	UMETA( DisplayName = "이동" ) ,
	ATTACK	UMETA( DisplayName = "공격" ) ,
	DAMAGE	UMETA( DisplayName = "데미지" ) ,
	DIE		UMETA( DisplayName = "죽음" )
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSGAMECD_API UEnemyFSMComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSMComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 상태를 만들어서 제어하고싶다. 
	UPROPERTY(EditAnywhere)
	EEnemyState state;

	// Idle : 주인공을 찾아서 목적지로 기억하고싶다. 만약 목적지가 있다면 이동상태로 전이하고싶다.
	// Move : 목적지를 향해 이동하고싶다. 만약 목적지와의 거리가 3M이내라면 공격상태로 전이하고싶다.
	// Attack : 2초 대기했다가 공격을 하고 싶다. 만약 공격을 하려고할때 목적지와의 거리가 3M를 초과한다면 이동상태로 전이하고싶다.
	UPROPERTY(EditAnywhere)
	float attackDist = 200;

	UPROPERTY(EditAnywhere)
	class AActor* target;

	UPROPERTY(EditAnywhere)
	class AEnemy* me;



	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickDamage();
	void TickDie();

	// 현재시간
	float currentTime;
	// 공격대기시간
	float attackWaitTime = 1;

	void TakeDamage( int damage );

	void SetState(EEnemyState next);
};
