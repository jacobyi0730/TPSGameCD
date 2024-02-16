// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerMoveComp.h"
#include "EnhancedInputComponent.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"


// Sets default values for this component's properties
UTPSPlayerMoveComp::UTPSPlayerMoveComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTPSPlayerMoveComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UTPSPlayerMoveComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	Move();
}

void UTPSPlayerMoveComp::SetupInput( class UEnhancedInputComponent* input )
{
	if (nullptr == input)
		return;

	input->BindAction( iaMove , ETriggerEvent::Triggered , this , &UTPSPlayerMoveComp::OnIAMove );
	input->BindAction( iaLook , ETriggerEvent::Triggered , this , &UTPSPlayerMoveComp::OnIALook );
	input->BindAction( iaJump , ETriggerEvent::Started , this , &UTPSPlayerMoveComp::OnIAJump );
	input->BindAction( iaRun , ETriggerEvent::Started , this , &UTPSPlayerMoveComp::OnIARun );
	input->BindAction( iaRun , ETriggerEvent::Completed , this , &UTPSPlayerMoveComp::OnIAWalk );
	input->BindAction( iaCrouch , ETriggerEvent::Started , this , &UTPSPlayerMoveComp::OnIACrouch );
	input->BindAction( iaDiveRoll , ETriggerEvent::Started , this , &UTPSPlayerMoveComp::OnIADiveRoll );

}

void UTPSPlayerMoveComp::OnIAMove( const FInputActionValue& value )
{
	FVector2D vec = value.Get<FVector2D>();
	direction.X = vec.X; // 앞뒤
	direction.Y = vec.Y; // 좌우
}

void UTPSPlayerMoveComp::OnIALook( const FInputActionValue& value )
{
	FVector2D vec = value.Get<FVector2D>();
	me->AddControllerYawInput( vec.X );
	me->AddControllerPitchInput( vec.Y );
}

void UTPSPlayerMoveComp::OnIAJump( const FInputActionValue& value )
{
	me->Jump();
}

void UTPSPlayerMoveComp::OnIARun( const FInputActionValue& value )
{
	// 걷기의 최대 speed를 1200으로 하고싶다.
	me->GetCharacterMovement()->MaxWalkSpeed = 1200;
}

void UTPSPlayerMoveComp::OnIAWalk( const FInputActionValue& value )
{
	// 걷기의 최대 speed를 600으로 하고싶다.
	me->GetCharacterMovement()->MaxWalkSpeed = 600;
}

void UTPSPlayerMoveComp::OnIACrouch( const FInputActionValue& value )
{
	if (isCrouched)
	{
		// 이미 쪼그려있다면 서고싶다.
		me->GetCharacterMovement()->UnCrouch();
		isCrouched = false;
	}
	else
	{
		// 서있다면 쪼그리고싶다.
		me->GetCharacterMovement()->Crouch();
		isCrouched = true;
	}
}

void UTPSPlayerMoveComp::OnIADiveRoll( const FInputActionValue& value )
{
	// 액션을 하고 0.8초 동안은 막고싶다.
	double Seconds = FPlatformTime::Seconds();
	int64 curMilSec = static_cast<int64>(Seconds * 1000);

	// 정적변수
	static int64 milliseconds = 0;

	// 만약 현재시간과 기억하고있던 시간의 차이가 800ms 을 초과한다면 몽타주를 재생하고싶다.

	if (curMilSec - milliseconds > 800)
	{
		milliseconds = curMilSec;
		me->PlayAnimMontage( diveRollMontage );
	}
}

void UTPSPlayerMoveComp::Move()
{
	FTransform trans = me->GetActorTransform();
	me->AddMovementInput( trans.TransformVector( direction ) );

	direction = FVector::ZeroVector;
}