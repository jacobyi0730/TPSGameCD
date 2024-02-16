// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "TPSPlayerBaseComp.h"
#include "TPSPlayerMoveComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSGAMECD_API UTPSPlayerMoveComp : public UTPSPlayerBaseComp
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPSPlayerMoveComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInput( class UEnhancedInputComponent* input ) override;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaMove;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaLook;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaJump;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaRun;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaCrouch;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* iaDiveRoll;

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isCrouched;
	
	UPROPERTY(EditAnywhere)
	class UAnimMontage* diveRollMontage;

	FVector direction;

	void OnIAMove( const FInputActionValue& value );
	void OnIALook( const FInputActionValue& value );
	void OnIAJump( const FInputActionValue& value );
	void OnIARun( const FInputActionValue& value );
	void OnIAWalk( const FInputActionValue& value );
	void OnIACrouch( const FInputActionValue& value );
	void OnIADiveRoll( const FInputActionValue& value );

	void Move();

};
