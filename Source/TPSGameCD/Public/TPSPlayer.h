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

	UPROPERTY(EditDefaultsOnly)
	class UTPSPlayerMoveComp* moveComp;

	UPROPERTY(EditDefaultsOnly)
	class UTPSPlayerFireComp* fireComp;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCameraComponent* cameraComp;

		UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* imcMapping;

	



	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaFire;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaChooseGun;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaChooseSniper;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaZoom;

	
	// 총 메시를 추가하고싶다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* sniperMeshComp;



	// 이동
	





	// 총쏘기
	void OnIAFire( const FInputActionValue& value );
	void OnIAChooseGun( const FInputActionValue& value );
	void OnIAChooseSniper( const FInputActionValue& value );
	void OnIAZoomIn( const FInputActionValue& value );
	void OnIAZoomOut( const FInputActionValue& value );

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> bulletFactory;

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


	UPROPERTY(EditAnywhere)
	class UAnimMontage* fireMontage;

	UPROPERTY(EditAnywhere)
	class USoundBase* fireSFX;
};

