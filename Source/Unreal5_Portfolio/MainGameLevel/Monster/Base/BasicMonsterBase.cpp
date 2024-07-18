// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameLevel/Monster/Base/BasicMonsterBase.h"
#include "MainGameLevel/Monster/Animation/BasicMonsterAnimInstance.h"
#include "MainGameLevel/Monster/AI/BasicMonsterAIController.h"

#include "Global/MainGameBlueprintFunctionLibrary.h"
#include "Global/MainGameInstance.h"
#include "Global/ContentsLog.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

ABasicMonsterBase::ABasicMonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackComponent = CreateDefaultSubobject<USphereComponent>("Attack Component");
	AttackComponent->SetupAttachment(RootComponent);
}

void ABasicMonsterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasicMonsterBase, AnimType);
}

void ABasicMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 데이터 세팅
	UMainGameInstance* MainGameInst = UMainGameBlueprintFunctionLibrary::GetMainGameInstance(GetWorld());
	const FMonsterDataRow* BaseData = MainGameInst->GetMonsterData(BaseDataName);

	if (nullptr == BaseData)
	{
		LOG(MonsterLog, Fatal, "BaseData Is Null");
		return;
	}

	SettingData = NewObject<UMonsterData>(this);
	SettingData->BaseData = BaseData;
	SettingData->AttackDamage = 34.0f;
	SettingData->OriginPos = GetActorLocation();

	// 애니메이션 세팅 (미완)
	AnimInst = Cast<UBasicMonsterAnimInstance>(GetMesh()->GetAnimInstance());
	
	
	// 서버 체크
	if (false == HasAuthority())
	{
		return;
	}

	// AI 컨트롤러 세팅
	AIController = GetController<ABasicMonsterAIController>();
	AIController->GetBlackboardComponent()->SetValueAsObject("BasicMonsterData", SettingData);

}

void ABasicMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimInst->ChangeAnimation(AnimType);
}


