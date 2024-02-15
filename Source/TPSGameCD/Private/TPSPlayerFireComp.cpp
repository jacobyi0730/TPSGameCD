// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerFireComp.h"
#include "Blueprint/UserWidget.h"
#include "TPSPlayer.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Enemy.h"
#include "BulletActor.h"

// Sets default values for this component's properties
UTPSPlayerFireComp::UTPSPlayerFireComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTPSPlayerFireComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	me = Cast<ATPSPlayer>( GetOwner() );

	crossHairUI = CreateWidget( GetWorld() , crossHairFactory );
	crossHairUI->AddToViewport();

	sniperUI = CreateWidget( GetWorld() , sniperFactory );
	sniperUI->AddToViewport();

	// 태어날 때 crossHiarUI만 보이게하고싶다.
	sniperUI->SetVisibility( ESlateVisibility::Hidden );

	OnIAChooseGun( FInputActionValue() );
}


// Called every frame
void UTPSPlayerFireComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	Zoom();
}

void UTPSPlayerFireComp::SetupInput( class UEnhancedInputComponent* input )
{
	if (nullptr == input)
		return;

	input->BindAction( iaFire , ETriggerEvent::Started , this , &UTPSPlayerFireComp::OnIAFire );
	input->BindAction( iaChooseGun , ETriggerEvent::Started , this , &UTPSPlayerFireComp::OnIAChooseGun );
	input->BindAction( iaChooseSniper , ETriggerEvent::Started , this , &UTPSPlayerFireComp::OnIAChooseSniper );
	input->BindAction( iaZoom , ETriggerEvent::Started , this , &UTPSPlayerFireComp::OnIAZoomIn );
	input->BindAction( iaZoom , ETriggerEvent::Completed , this , &UTPSPlayerFireComp::OnIAZoomOut );
}


void UTPSPlayerFireComp::OnIAFire( const FInputActionValue& value )
{
	// 총소리를 내고싶다.
	UGameplayStatics::PlaySound2D( GetWorld() , fireSFX );
	// 움찔 애니를 하고싶다.
	me->PlayAnimMontage( fireMontage );

	// 만약 스나이퍼가 아니라면
	if (false == bChooseSniperGun)
	{
		FTransform t = me->gunMeshComp->GetSocketTransform( TEXT( "FirePosition" ) );
		GetWorld()->SpawnActor<ABulletActor>( bulletFactory , t );
	}
	// 그렇지 않다면 LineTrace...
	else
	{
		FHitResult outHit;
		FVector start = me->cameraComp->GetComponentLocation();
		FVector end = start + me->cameraComp->GetForwardVector() * 100000;
		FCollisionQueryParams params;
		params.AddIgnoredActor( me );

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

void UTPSPlayerFireComp::OnIAChooseGun( const FInputActionValue& value )
{
	bChooseSniperGun = false;
	// 유탄총을 보이게, 스나이퍼를 안보이게
	me->gunMeshComp->SetVisibility( true );
	me->sniperMeshComp->SetVisibility( false );
	// 총을 교체하면 ZoomOut을 하고싶다.
	OnIAZoomOut( FInputActionValue() );
}

void UTPSPlayerFireComp::OnIAChooseSniper( const FInputActionValue& value )
{
	bChooseSniperGun = true;
	// 유탄총을 안보이게, 스나이퍼를 보이게
	me->gunMeshComp->SetVisibility( false );
	me->sniperMeshComp->SetVisibility( true );
}

void UTPSPlayerFireComp::OnIAZoomIn( const FInputActionValue& value )
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

void UTPSPlayerFireComp::OnIAZoomOut( const FInputActionValue& value )
{
	// ZoomOut을 하면 crossHiarUI를 보이게, sniperUI보이지않게 하고싶다.
	crossHairUI->SetVisibility( ESlateVisibility::Visible );
	sniperUI->SetVisibility( ESlateVisibility::Hidden );
	targetFOV = 90;
}




void UTPSPlayerFireComp::Zoom()
{
	// 선형보간을 이용해서 현재 FOV를 targetFOV값에 근접하게 하고싶다.
	me->cameraComp->FieldOfView = FMath::Lerp<float>( me->cameraComp->FieldOfView , targetFOV , GetWorld()->GetDeltaSeconds() * 10 );
}