// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAMECD_API UEnemyHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick( const FGeometry& MyGeometry , float InDeltaTime ) override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UProgressBar* HPBarMid;

	void SetHP( int32 hp , int32 maxHP );

	UPROPERTY()
	class AEnemy* me;
};
