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

	OnIAChooseGun(FInputActionValue());

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
	Zoom();

	Move();

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );
	// 가로축, 세로축, 점프에대한 함수를 바인딩 하고싶다.

	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>( PlayerInputComponent );

	if (input)
	{
		input->BindAction( iaMove , ETriggerEvent::Triggered , this , &ATPSPlayer::OnIAMove );

		input->BindAction( iaLook , ETriggerEvent::Triggered , this , &ATPSPlayer::OnIALook);
		input->BindAction( iaJump , ETriggerEvent::Started , this , &ATPSPlayer::OnIAJump);
		input->BindAction( iaFire , ETriggerEvent::Started , this , &ATPSPlayer::OnIAFire);
		input->BindAction( iaChooseGun , ETriggerEvent::Started , this , &ATPSPlayer::OnIAChooseGun);
		input->BindAction( iaChooseSniper , ETriggerEvent::Started , this , &ATPSPlayer::OnIAChooseSniper);
		input->BindAction( iaZoom , ETriggerEvent::Started , this , &ATPSPlayer::OnIAZoomIn);
		input->BindAction( iaZoom , ETriggerEvent::Completed , this , &ATPSPlayer::OnIAZoomOut);

		input->BindAction( iaRun , ETriggerEvent::Started , this , &ATPSPlayer::OnIARun );
		input->BindAction( iaRun , ETriggerEvent::Completed , this , &ATPSPlayer::OnIAWalk );

		input->BindAction( iaCrouch , ETriggerEvent::Started , this , &ATPSPlayer::OnIACrouch );
		input->BindAction( iaDiveRoll , ETriggerEvent::Started , this , &ATPSPlayer::OnIADiveRoll );

	}


}

void ATPSPlayer::OnIAMove( const FInputActionValue& value )
{
	FVector2D vec = value.Get<FVector2D>();
	direction.X = vec.X; // 앞뒤
	direction.Y = vec.Y; // 좌우
}

void ATPSPlayer::OnIALook( const FInputActionValue& value )
{
	FVector2D vec = value.Get<FVector2D>();
	AddControllerYawInput( vec.X );
	AddControllerPitchInput( vec.Y );
}

void ATPSPlayer::OnIAJump( const FInputActionValue& value )
{
	Jump();
}

void ATPSPlayer::OnIAFire( const FInputActionValue& value )
{
	// 총소리를 내고싶다.
	UGameplayStatics::PlaySound2D( GetWorld() , fireSFX );
	// 움찔 애니를 하고싶다.
	this->PlayAnimMontage( fireMontage );

	// 만약 스나이퍼가 아니라면
	if (false == bChooseSniperGun)
	{
		FTransform t = gunMeshComp->GetSocketTransform( TEXT( "FirePosition" ) );
		GetWorld()->SpawnActor<ABulletActor>( bulletFactory , t );
	}
	// 그렇지 않다면 LineTrace...
	else
	{
		FHitResult outHit;
		FVector start = cameraComp->GetComponentLocation();
		FVector end = start + cameraComp->GetForwardVector() * 100000;
		FCollisionQueryParams params;
		params.AddIgnoredActor( this );

		bool bReturnValue = GetWorld()->LineTraceSingleByChannel( outHit , start , end , ECollisionChannel::ECC_Visibility , params );

		// 만약 부딪힌 것이 있다면
		if (bReturnValue)
		{
			DrawDebugLine( GetWorld() , outHit.TraceStart , outHit.ImpactPoint , FColor::Red , false , 10 );
			// 부딪힌 컴포넌트를 가져와서
			UPrimitiveComponent* hitComp = outHit.GetComponent();
			// 만약 컴포넌트가 있다 그리고 컴포넌트의 물리가 켜져있다면
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				// 그 컴포넌트에게 힘을 가하고싶다.
				FVector dir = end - start;
				hitComp->AddForce( dir.GetSafeNormal() * 500000 * hitComp->GetMass() );
			}

			// 부딪힌 곳에 expVFX를 생성해서 배치하고싶다.
			UGameplayStatics::SpawnEmitterAtLocation( GetWorld() , expVFX , outHit.ImpactPoint );

			// 만약 부딪힌것이 AEnemy라면
			// 적에게 데미지 1점을 주고싶다. 
			AEnemy* enemy = Cast<AEnemy>( outHit.GetActor() );
			if (enemy)
			{
				//auto fsm = Cast<UEnemyFSMComp>(enemy->GetDefaultSubobjectByName(TEXT("enemyFSM")));
				//fsm->TakeDamage(1);
				enemy->OnMyTakeDamage( 1 );
			}
		}
	}
}

void ATPSPlayer::OnIAChooseGun( const FInputActionValue& value )
{
	bChooseSniperGun = false;
	// 유탄총을 보이게, 스나이퍼를 안보이게
	gunMeshComp->SetVisibility( true );
	sniperMeshComp->SetVisibility( false );
	// 총을 교체하면 ZoomOut을 하고싶다.
	OnIAZoomOut(FInputActionValue());
}

void ATPSPlayer::OnIAChooseSniper( const FInputActionValue& value )
{
	bChooseSniperGun = true;
	// 유탄총을 안보이게, 스나이퍼를 보이게
	gunMeshComp->SetVisibility( false );
	sniperMeshComp->SetVisibility( true );
}

void ATPSPlayer::OnIAZoomIn( const FInputActionValue& value )
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

void ATPSPlayer::OnIAZoomOut( const FInputActionValue& value )
{
	// ZoomOut을 하면 crossHiarUI를 보이게, sniperUI보이지않게 하고싶다.
	crossHairUI->SetVisibility( ESlateVisibility::Visible );
	sniperUI->SetVisibility( ESlateVisibility::Hidden );
	targetFOV = 90;
}

void ATPSPlayer::OnIARun( const FInputActionValue& value )
{
	// 걷기의 최대 speed를 1200으로 하고싶다.
	GetCharacterMovement()->MaxWalkSpeed = 1200;
}

void ATPSPlayer::OnIAWalk( const FInputActionValue& value )
{
	// 걷기의 최대 speed를 600으로 하고싶다.
	GetCharacterMovement()->MaxWalkSpeed = 600;
}

void ATPSPlayer::OnIACrouch( const FInputActionValue& value )
{
	if (isCrouched)
	{
		// 이미 쪼그려있다면 서고싶다.
		GetCharacterMovement()->UnCrouch();
		isCrouched = false;
	}
	else
	{
		// 서있다면 쪼그리고싶다.
		GetCharacterMovement()->Crouch();
		isCrouched = true;
	}
}

void ATPSPlayer::OnIADiveRoll( const FInputActionValue& value )
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
		this->PlayAnimMontage( diveRollMontage );
	}
}

void ATPSPlayer::Move()
{
	FTransform trans = GetActorTransform();
	AddMovementInput( trans.TransformVector( direction ) );

	direction = FVector::ZeroVector;
}


void ATPSPlayer::Zoom()
{
	// 선형보간을 이용해서 현재 FOV를 targetFOV값에 근접하게 하고싶다.
	cameraComp->FieldOfView = FMath::Lerp<float>( cameraComp->FieldOfView , targetFOV , GetWorld()->GetDeltaSeconds() * 10 );
}

