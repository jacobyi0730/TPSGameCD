// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerBaseComp.h"
#include "TPSPlayer.h"

// Sets default values for this component's properties
UTPSPlayerBaseComp::UTPSPlayerBaseComp()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UTPSPlayerBaseComp::InitializeComponent()
{
	me = Cast<ATPSPlayer>( GetOwner() );

	me->setupInputDelegate.AddUObject( this , &UTPSPlayerBaseComp::SetupInput );
}

// Called when the game starts
void UTPSPlayerBaseComp::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UTPSPlayerBaseComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

