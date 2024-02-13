// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHPWidget.h"
#include "Enemy.h"
#include "../../../../../../../Source/Runtime/UMG/Public/Components/ProgressBar.h"

void UEnemyHPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetHP( 1 , 1 );
}

void UEnemyHPWidget::NativeTick( const FGeometry& MyGeometry , float InDeltaTime )
{
	// HPBarMid의 퍼센트를 HPBar->GetPercent()값을 향해 이동하게하고싶다.
	float curPercent = HPBarMid->GetPercent();
	float targetPercent = HPBar->GetPercent();
	float lerpPercent = FMath::Lerp<float>( curPercent , targetPercent , InDeltaTime * 6 );

	HPBarMid->SetPercent( lerpPercent );
}

void UEnemyHPWidget::SetHP( int32 hp , int32 maxHP )
{
	HPBar->SetPercent( (float)hp / maxHP );
}
