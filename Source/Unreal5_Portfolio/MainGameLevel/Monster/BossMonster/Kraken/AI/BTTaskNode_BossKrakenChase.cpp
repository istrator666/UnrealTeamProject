// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameLevel/Monster/BossMonster/Kraken/AI/BTTaskNode_BossKrakenChase.h"
#include "MainGameLevel/Monster/BossMonster/AI/BossMonsterAIController.h"
#include "MainGameLevel/Monster/BossMonster/Kraken/BossKrakenData.h"
#include "MainGameLevel/Monster/BossMonster/Kraken/BossKraken.h"
#include "MainGameLevel/Player/MainPlayerState.h"
#include "MainGameLevel/Player/MainCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

#include "Global/MainGameBlueprintFunctionLibrary.h"
#include "Global/MainGameState.h"
#include "Global/ContentsEnum.h"
#include "Global/ContentsLog.h"

#include "TestLevel/Character/TestPlayerState.h"
#include "TestLevel/Character/TestCharacter.h"

EBTNodeResult::Type UBTTaskNode_BossKrakenChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABossKraken* Kraken = GetSelfActor<ABossKraken>(OwnerComp);
	if (false == Kraken->IsValidLowLevel())
	{
		LOG(MonsterLog, Fatal, TEXT("Kraken Is Not Valid"));
		return EBTNodeResult::Type::Aborted;
	}

	UBossKrakenData* KrakenData = Kraken->GetSettingData();
	if (false == KrakenData->IsValidLowLevel())
	{
		LOG(MonsterLog, Fatal, TEXT("KrakenData Is Not Valid"));
		return EBTNodeResult::Type::Aborted;
	}

	bool IsTargetVaild = FindTarget(OwnerComp);
	if (false == IsTargetVaild)
	{
		StateChange(OwnerComp, EBossMonsterState::Idle);
		return EBTNodeResult::Type::Failed;
	}

	Kraken->GetCharacterMovement()->MaxWalkSpeed = KrakenData->BaseData->ChaseSpeed;
	Kraken->ChangeAnimation(EBossMonsterAnim::Run);

	return EBTNodeResult::Type::InProgress;
}

void UBTTaskNode_BossKrakenChase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* pNodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, pNodeMemory, DeltaSeconds);

	ABossKraken* Kraken = GetSelfActor<ABossKraken>(OwnerComp);
	UBossKrakenData* KrakenData = Kraken->GetSettingData();
	AActor* Target = GetValueAsObject<AActor>(OwnerComp, TEXT("TargetActor"));
	
	FVector LocationDiff = Kraken->GetActorLocation() - Target->GetActorLocation();
	float Diff = LocationDiff.Size();

	if (KrakenData->BaseData->MeleeAttackRange >= Diff)
	{
		StateChange(OwnerComp, EBossMonsterState::MeleeAttack);
		return;
	}
	else
	{
		StateChange(OwnerComp, EBossMonsterState::RangedAttack);
		return;
	}

	Kraken->GetAIController()->MoveToLocation(Target->GetActorLocation());
}

bool UBTTaskNode_BossKrakenChase::FindTarget(UBehaviorTreeComponent& OwnerComp)
{
	ABossKraken* Kraken = GetSelfActor<ABossKraken>(OwnerComp);

	// Find Target
	AMainGameState* CurGameState = UMainGameBlueprintFunctionLibrary::GetMainGameState(GetWorld());
	UActorGroup* PlayerGroup = CurGameState->GetActorGroup(EObjectType::Player);
	if (nullptr == PlayerGroup)
	{
		LOG(MonsterLog, Fatal, TEXT("PlayerGroup Is Nullptr"));
		return false;
	}

	float MinDist = FLT_MAX;
	AActor* TargetActor = nullptr;
	for (AActor* Actor : PlayerGroup->Actors)
	{
		// Player 상태 체크

		ATestCharacter* Player = Cast<ATestCharacter>(Actor);
		if (nullptr == Player)
		{
			LOG(MonsterLog, Fatal, TEXT("Player Is Nullptr"));
			return false;
		}

		ATestPlayerState* TestPlayerState = Cast<ATestPlayerState>(Player->GetPlayerState());
		if (nullptr == TestPlayerState)
		{
			LOG(MonsterLog, Fatal, TEXT("TestPlayerState Is Nullptr"));
			return false;
		}

		if (0.0f >= TestPlayerState->GetPlayerHp())
		{
			continue;
		}

		//AMainCharacter* Player = Cast<AMainCharacter>(Actor);
		//if (nullptr == Player)
		//{
		//	LOG(MonsterLog, Fatal, TEXT("Player Is Nullptr"));
		//	return;
		//}
		//
		//AMainPlayerState* MainPlayerState = Cast<AMainPlayerState>(Player->GetPlayerState());
		//if (nullptr == MainPlayerState)
		//{
		//	LOG(MonsterLog, Fatal, TEXT("MainPlayerState Is Nullptr"));
		//	return;
		//}
		//
		//if (0.0f >= MainPlayerState->GetPlayerHp())
		//{
		//	continue;
		//}

		// 최단 거리 Player 찾기
		FVector PlayerLocation = Player->GetActorLocation();
		PlayerLocation.Z = 0.0f;

		FVector MonsterLocation = Kraken->GetActorLocation();
		MonsterLocation.Z = 0.0f;

		FVector DiffLocation = MonsterLocation - PlayerLocation;

		float Diff = DiffLocation.Size();
		if (Diff <= MinDist)
		{
			MinDist = Diff;
			TargetActor = Player;
		}
	}

	SetValueAsObject(OwnerComp, TEXT("TargetActor"), TargetActor);

	return (TargetActor != nullptr) ? true : false;
}
