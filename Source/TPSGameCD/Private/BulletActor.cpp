// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// sphereComp를 루트로
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	SetRootComponent( sphereComp );
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComp"));

	// speed설정, 바운드설정
	movementComp->InitialSpeed = 2000.f;
	movementComp->MaxSpeed = 2000.f;
	movementComp->bShouldBounce = true;

	sphereComp->SetCollisionProfileName(TEXT("BlockAll"));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 총알액터의 수명을 5초로 하고싶다.
	SetLifeSpan(5); 
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	movementComp->SetUpdatedComponent(sphereComp);
	
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

