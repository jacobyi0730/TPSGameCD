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

	



	

	
	// 총 메시를 추가하고싶다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* sniperMeshComp;



	// 이동
	





	// 총쏘기

};

