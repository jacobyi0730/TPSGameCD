// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerFireComp.h"

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
}


// Called every frame
void UTPSPlayerFireComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTPSPlayerFireComp::SetupInput( class UEnhancedInputComponent* input )
{

}

