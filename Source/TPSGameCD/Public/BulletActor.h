// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletActor.generated.h"

UCLASS()
class TPSGAMECD_API ABulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 충돌체와 외관을 만들고싶다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AAA)
	class USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AAA)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AAA)
	class UProjectileMovementComponent* movementComp;

};
