// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "TPSPlayerFireComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSGAMECD_API UTPSPlayerFireComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPSPlayerFireComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInput( class UEnhancedInputComponent* input );

	UPROPERTY()
	class ATPSPlayer* me;


	
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaFire;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaChooseGun;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaChooseSniper;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaZoom;

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
