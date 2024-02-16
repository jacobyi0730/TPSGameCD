// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/ProgressBar.h"

/// <summary>
/// 시작할때, curHP에 변화가 있을때 UpdateHPBar를 호출해야한다.
/// </summary>
/// <param name="curHP">현재체력</param>
/// <param name="maxHP">최대체력</param>
void UMainWidget::UpdateHPBar( float curHP , float maxHP )
{
	HPBar->SetPercent(curHP / maxHP);
}
