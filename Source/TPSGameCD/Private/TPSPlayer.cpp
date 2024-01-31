// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BulletActor.h"
#include "../../../../../../../Source/Runtime/UMG/Public/Blueprint/UserWidget.h"

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
	gunMeshComp->SetupAttachment( GetMesh() );

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGunMesh( TEXT( "/Script/Engine.SkeletalMesh'/Game/Models/FPWeapon/Mesh/SK_FPGun.SK_FPGun'" ) );

	if (tempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh( tempGunMesh.Object );
		gunMeshComp->SetRelativeLocation( FVector( 0 , 50 , 130 ) );
	}

	// sniperMeshComp(UStaticMeshComponent)를 생성해서 메쉬에 붙이고싶다.
	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "sniperMeshComp" ) );
	sniperMeshComp->SetupAttachment( GetMesh() );
	// UStaticMesh를 로드해서 적용하고싶다.
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper( TEXT( "/Script/Engine.StaticMesh'/Game/Models/SniperGun/sniper1.sniper1'" ) );

	if (tempSniper.Succeeded())
	{
		sniperMeshComp->SetStaticMesh( tempSniper.Object );
		sniperMeshComp->SetRelativeLocation( FVector( 0 , 80 , 130 ) );
		sniperMeshComp->SetWorldScale3D( FVector( 0.15f ) );
	}

	gunMeshComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	sniperMeshComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );
}

void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	crossHairUI = CreateWidget( GetWorld() , crossHairFactory );
	crossHairUI->AddToViewport();

	sniperUI = CreateWidget( GetWorld() , sniperFactory );
	sniperUI->AddToViewport();

	// 태어날 때 crossHiarUI만 보이게하고싶다.
	sniperUI->SetVisibility( ESlateVisibility::Hidden );

	OnActionChooseGrenadeGun();
}

// Called every frame
void ATPSPlayer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	Zoom();

	Move();

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	// 가로축, 세로축, 점프에대한 함수를 바인딩 하고싶다.

	// 주어->기능(필요한 값)
	PlayerInputComponent->BindAxis( TEXT( "Move Forward / Backward" ) , this , &ATPSPlayer::OnAxisVertical );

	PlayerInputComponent->BindAxis( TEXT( "Move Right / Left" ) , this , &ATPSPlayer::OnAxisHorizontal );

	PlayerInputComponent->BindAxis( TEXT( "Turn Right / Left Mouse" ) , this , &ATPSPlayer::OnAxisTurnYaw );

	PlayerInputComponent->BindAxis( TEXT( "Look Up / Down Mouse" ) , this , &ATPSPlayer::OnAxisLookupPitch );

	PlayerInputComponent->BindAction( TEXT( "Jump" ) , IE_Pressed , this , &ATPSPlayer::OnActionJump );

	PlayerInputComponent->BindAction( TEXT( "Fire" ) , IE_Pressed , this , &ATPSPlayer::OnActionFire );

	PlayerInputComponent->BindAction( TEXT( "ChooseGrenadeGun" ) , IE_Pressed , this , &ATPSPlayer::OnActionChooseGrenadeGun );

	PlayerInputComponent->BindAction( TEXT( "ChooseSniperGun" ) , IE_Pressed , this , &ATPSPlayer::OnActionChooseSniperGun );


	PlayerInputComponent->BindAction( TEXT( "Zoom" ) , IE_Pressed , this , &ATPSPlayer::OnActionZoomIn );

	PlayerInputComponent->BindAction( TEXT( "Zoom" ) , IE_Released , this , &ATPSPlayer::OnActionZoomOut );

}

void ATPSPlayer::Move()
{
	FTransform trans = GetActorTransform();
	AddMovementInput( trans.TransformVector( direction ) );
}

void ATPSPlayer::OnAxisVertical( float value )
{
	direction.X = value;
}

void ATPSPlayer::OnAxisHorizontal( float value )
{
	direction.Y = value;
}

void ATPSPlayer::OnAxisTurnYaw( float value )
{
	AddControllerYawInput( value );
}

void ATPSPlayer::OnAxisLookupPitch( float value )
{
	AddControllerPitchInput( value );
}

void ATPSPlayer::OnActionJump()
{
	Jump();
}

void ATPSPlayer::OnActionFire()
{
	FTransform t = gunMeshComp->GetSocketTransform( TEXT( "FirePosition" ) );
	GetWorld()->SpawnActor<ABulletActor>( bulletFactory , t );
}

void ATPSPlayer::OnActionChooseGrenadeGun()
{
	bChooseSniperGun = false;
	// 유탄총을 보이게, 스나이퍼를 안보이게
	gunMeshComp->SetVisibility( true );
	sniperMeshComp->SetVisibility( false );
	// 총을 교체하면 ZoomOut을 하고싶다.
	OnActionZoomOut();
}

void ATPSPlayer::OnActionChooseSniperGun()
{
	bChooseSniperGun = true;
	// 유탄총을 안보이게, 스나이퍼를 보이게
	gunMeshComp->SetVisibility( false );
	sniperMeshComp->SetVisibility( true );
}

void ATPSPlayer::Zoom()
{
	// 선형보간을 이용해서 현재 FOV를 targetFOV값에 근접하게 하고싶다.
	cameraComp->FieldOfView = FMath::Lerp<float>( cameraComp->FieldOfView , targetFOV , GetWorld()->GetDeltaSeconds() * 10 );
}

void ATPSPlayer::OnActionZoomIn()
{
	// 만약 손에 쥔 총이 스나이퍼가 아니라면 함수를 바로 종료하고싶다.
	if (false == bChooseSniperGun)
	{
		return;
	}

	// ZoomIn을 하면 crossHairUI를 보이지않게, sniperUI보이게 하고싶다.
	crossHairUI->SetVisibility( ESlateVisibility::Hidden );
	sniperUI->SetVisibility( ESlateVisibility::Visible );
	targetFOV = 30;
}

void ATPSPlayer::OnActionZoomOut()
{
	// ZoomOut을 하면 crossHiarUI를 보이게, sniperUI보이지않게 하고싶다.
	crossHairUI->SetVisibility( ESlateVisibility::Visible );
	sniperUI->SetVisibility( ESlateVisibility::Hidden );
	targetFOV = 90;
}

