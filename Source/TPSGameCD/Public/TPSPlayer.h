// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCameraComponent* cameraComp;

	FVector direction;

	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* imcMapping;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaMove;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaLook;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaJump;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaFire;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaChooseGun;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaChooseSniper;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaZoom;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaRun;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaCrouch;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaDiveRoll;

	void OnIAMove(const FInputActionValue& value);
	void OnIALook( const FInputActionValue& value );
	void OnIAJump( const FInputActionValue& value );
	void OnIAFire( const FInputActionValue& value );
	void OnIAChooseGun( const FInputActionValue& value );
	void OnIAChooseSniper( const FInputActionValue& value );
	void OnIAZoomIn( const FInputActionValue& value );
	void OnIAZoomOut( const FInputActionValue& value );
	void OnIARun( const FInputActionValue& value );
	void OnIAWalk( const FInputActionValue& value );
	void OnIACrouch( const FInputActionValue& value );
	void OnIADiveRoll( const FInputActionValue& value );


	void Move();




	// 총 메시를 추가하고싶다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* sniperMeshComp;

	// 총알공장을 만들고싶다.
	// 마우스 왼쪽 버튼을 누르면 총알공장에서 총알을만들어서 FirePosition소켓에 배치하고싶다.

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> bulletFactory;


public:
	UPROPERTY( EditAnywhere )
	TSubclassOf<class UUserWidget> crossHairFactory;

	UPROPERTY( EditAnywhere )
	TSubclassOf<class UUserWidget> sniperFactory;

	UPROPERTY()
	class UUserWidget* crossHairUI;

	UPROPERTY()
	class UUserWidget* sniperUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float targetFOV = 90;

	void Zoom();


	/// <summary> true : sniper, false : grenade </summary>
	bool bChooseSniperGun = false;	

	// 스나이퍼 총을 쐈을 때 부딪힌곳에 자국을 남기고싶다.
	UPROPERTY(EditAnywhere)
	class UParticleSystem* expVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isCrouched;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* fireMontage;

	UPROPERTY(EditAnywhere)
	class USoundBase* fireSFX;


	UPROPERTY(EditAnywhere)
	class UAnimMontage* diveRollMontage;


};

