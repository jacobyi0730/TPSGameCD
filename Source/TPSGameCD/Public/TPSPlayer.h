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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* sniperMeshComp;

	// 총알공장을 만들고싶다.
	// 마우스 왼쪽 버튼을 누르면 총알공장에서 총알을만들어서 FirePosition소켓에 배치하고싶다.

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> bulletFactory;

	void OnActionFire();

	void OnActionChooseGrenadeGun();
	void OnActionChooseSniperGun();

public:
	UPROPERTY( EditAnywhere )
	TSubclassOf<class UUserWidget> crossHairFactory;

	UPROPERTY( EditAnywhere )
	TSubclassOf<class UUserWidget> sniperFactory;

	UPROPERTY()
	class UUserWidget* crossHairUI;

	UPROPERTY()
	class UUserWidget* sniperUI;
};
