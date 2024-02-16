// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "TPSPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam( FSetupInputDelegate , class UEnhancedInputComponent*);

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

	FSetupInputDelegate setupInputDelegate;

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

	// 체력을 관리하고싶다.
	int32 maxHP = 10;

	UPROPERTY(EditDefaultsOnly)
	int32 hp = maxHP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UMainWidget> mainUIFactory;

	UPROPERTY(BlueprintReadOnly)
	class UMainWidget* mainUI;

	void DamageProcess(int damage, class AActor* sender);

	// 블루프린트에서 구현하고, c++에서 호출하고싶은 함수
	UFUNCTION(BlueprintImplementableEvent)
	void OnMyChooseGun(bool isSniper);
};

