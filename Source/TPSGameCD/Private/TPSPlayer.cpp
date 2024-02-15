// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BulletActor.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "EnemyFSMComp.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "TPSPlayerMoveComp.h"
#include "TPSPlayerFireComp.h"

ATPSPlayer::ATPSPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// springArmComp를 생성해서 Root에 붙이고싶다.
	springArmComp = CreateDefaultSubobject<USpringArmComponent>( TEXT( "springArmComp" ) );
	springArmComp->SetupAttachment( RootComponent );
	// Loc : (X=0.000000,Y=70.000000,Z=90.000000)
	springArmComp->SetWorldLocation( FVector( 0 , 70 , 90 ) );
	// cameraComp를 생성해서 springArmComp에 붙이고싶다.
	cameraComp = CreateDefaultSubobject<UCameraComponent>( TEXT( "cameraComp" ) );
	cameraComp->SetupAttachment( springArmComp );

	// mesh를 로드해서 적용하고싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh( TEXT( "/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'" ) );
	// 성공했다면
	if (tempMesh.Succeeded())
	{
		// Mesh에 적용하고싶다.
		GetMesh()->SetSkeletalMesh( tempMesh.Object );
		GetMesh()->SetRelativeLocationAndRotation( FVector( 0 , 0 , -90 ) , FRotator( 0 , -90 , 0 ) );
	}

	bUseControllerRotationYaw = true;
	// Use Pawn Control Rotation을 True로 하고싶다.
	springArmComp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;


	// gunMeshComp를 생성해서 로딩도하고 배치하고싶다. Mesh에 붙이고싶다.
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "gunMeshComp" ) );
	gunMeshComp->SetupAttachment( GetMesh() , TEXT( "hand_rSocket" ) );

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGunMesh( TEXT( "/Script/Engine.SkeletalMesh'/Game/Models/FPWeapon/Mesh/SK_FPGun.SK_FPGun'" ) );

	if (tempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh( tempGunMesh.Object );
		gunMeshComp->SetRelativeLocationAndRotation( FVector( -2.4f , 1.26f , 1.025f ), FRotator( 8.19f, 101.63f, -5.88f) );
		// (X=-2.400000,Y=1.260000,Z=1.025000)
		// (Pitch=8.190000,Yaw=101.630000,Roll=-5.880000)
		gunMeshComp->SetWorldScale3D( FVector( 0.8f ) );
	}

	// sniperMeshComp(UStaticMeshComponent)를 생성해서 메쉬에 붙이고싶다.
	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "sniperMeshComp" ) );
	sniperMeshComp->SetupAttachment( GetMesh() , TEXT( "hand_rSocket" ) );
	// UStaticMesh를 로드해서 적용하고싶다.
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper( TEXT( "/Script/Engine.StaticMesh'/Game/Models/SniperGun/sniper1.sniper1'" ) );

	if (tempSniper.Succeeded())
	{
		sniperMeshComp->SetStaticMesh( tempSniper.Object );
		sniperMeshComp->SetRelativeLocationAndRotation( FVector( -23.37f , -3.79f , 6.5f ), FRotator(7.58f, 101.37f, -7.7f) );
		// (X=-23.370000,Y=-3.790000,Z=6.500000)
		// (Pitch=7.580000,Yaw=101.370000,Roll=-7.700000)
		sniperMeshComp->SetWorldScale3D( FVector( 0.1f ) );
	}

	gunMeshComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	sniperMeshComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );


	// 이동, 총쏘기 컴포넌트를 생성하고싶다.
	moveComp = CreateDefaultSubobject<UTPSPlayerMoveComp>( TEXT( "moveComp" ) );
	fireComp = CreateDefaultSubobject<UTPSPlayerFireComp>( TEXT( "fireComp" ) );
}

void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* controller = Cast<APlayerController>( Controller );
	if (controller)
	{
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( controller->GetLocalPlayer() );

		if (subSystem)
		{
			subSystem->ClearAllMappings();
			subSystem->AddMappingContext( imcMapping, 0 );
		}
	}
}

// Called every frame
void ATPSPlayer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );
	// 가로축, 세로축, 점프에대한 함수를 바인딩 하고싶다.

	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>( PlayerInputComponent );

	moveComp->SetupInput( input );
	fireComp->SetupInput( input );
}
