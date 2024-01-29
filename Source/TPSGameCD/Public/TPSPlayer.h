// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPSGAMECD_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* cameraComp;

	FVector direction;

	void Move();

	void OnAxisVertical(float value);
	void OnAxisHorizontal(float value);

	void OnAxisTurnYaw(float value);		// Yaw
	void OnAxisLookupPitch(float value);	// Pitch

	void OnActionJump();


	// 총 메시를 추가하고싶다.
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* gunMeshComp;
};
