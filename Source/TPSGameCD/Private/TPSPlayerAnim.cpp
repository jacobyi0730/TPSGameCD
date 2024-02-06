// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerAnim.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTPSPlayerAnim::NativeUpdateAnimation( float DeltaSeconds )
{
	// 1. 오너 폰을 가져와서 ATPSPlayer로 Cast하고 싶다.
	ATPSPlayer* player = Cast<ATPSPlayer>( TryGetPawnOwner() );
	if (nullptr == player)
	{
		return;
	}
	// 2. 오너의 velocity, forward vector, right vector를 가져오고싶다. 
	FVector v = player->GetVelocity();
	FVector forward = player->GetActorForwardVector();
	FVector right = player->GetActorRightVector();
	// 3. speed, direction의 값을 채우고싶다.
	speed = FVector::DotProduct( v , forward );
	direction = FVector::DotProduct( v , right );
	// 4. isInAir, isCrouched를 채우고싶다.
	isInAir = player->GetCharacterMovement()->IsFalling();
	isCrouched = player->isCrouched;
	// 5. 오너의 aim rotation값을 가져와서 pitch를 채우고싶다.
	pitch = player->GetBaseAimRotation().Pitch;
}
