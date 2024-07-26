// Fill out your copyright notice in the Description page of Project Settings.


#include "PartDevLevel/UI/HUD/WeaponSlot_UserWidget.h"
#include "TestLevel/UI/TestPlayHUD.h"
#include "Global/ContentsLog.h"

#include "kismet/GameplayStatics.h"

void UWeaponSlot_UserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* MyController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (nullptr == MyController)
	{
		LOG(UILog, Fatal, "Controller is Null");
	}

	CurHUD = Cast<ATestPlayHUD>(MyController->GetHUD());
	if (nullptr == CurHUD)
	{
		LOG(UILog, Fatal, "HUD is Null");
	}
}

void UWeaponSlot_UserWidget::UpdateReloadComment(int _bullet) // MouseLeftCall 이 안타져서 테스트 못해봤음(연두)
{
	if (_bullet > 0 && false == IsChanged)
	{
		IsChanged = false;
		CurHUD->UIOff(EUserWidgetType::ReloadComment);
	}
	else if (_bullet <= 0)
	{
		IsChanged = true;
		CurHUD->UIOn(EUserWidgetType::ReloadComment);
	}
}

