// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNodeBase_BasicMonster.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5_PORTFOLIO_API UBTTaskNodeBase_BasicMonster : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNodeBase_BasicMonster();

public:
	// State
	template<typename EnumType>
	EnumType GetCurState(UBehaviorTreeComponent& OwnerComp)
	{
		return static_cast<EnumType>(GetCurState(OwnerComp));
	}

	uint8 GetCurState(UBehaviorTreeComponent& OwnerComp);

	template<typename EnumType>
	void StateChange(UBehaviorTreeComponent& OwnerComp, EnumType NextState, EBTNodeResult::Type NodeResult = EBTNodeResult::Failed)
	{
		StateChange(OwnerComp, static_cast<uint8>(NextState), NodeResult);
	}

	void StateChange(UBehaviorTreeComponent& OwnerComp, uint8 NextState, EBTNodeResult::Type NodeResult = EBTNodeResult::Failed);

public:
	// SelfActor
	template<typename OwnerType>
	OwnerType* GetSelfActor(UBehaviorTreeComponent& OwnerComp)
	{
		return Cast<OwnerType>(GetSelfActor(OwnerComp));
	}

	FORCEINLINE AActor* GetSelfActor(UBehaviorTreeComponent& OwnerComp);

	// Object
	template<typename ObjectType>
	ObjectType* GetValueAsObject(UBehaviorTreeComponent& OwnerComp, FName Name)
	{
		return Cast<ObjectType>(GetValueAsObject(OwnerComp, Name));
	}

	FORCEINLINE UObject* GetValueAsObject(UBehaviorTreeComponent& OwnerComp, FName Name);
	FORCEINLINE void SetValueAsObject(UBehaviorTreeComponent& OwnerComp, FName Name, UObject* Value);

	// Vector
	FORCEINLINE FVector GetValueAsVector(UBehaviorTreeComponent& OwnerComp, FName Name);
	FORCEINLINE void SetValueAsVector(UBehaviorTreeComponent& OwnerComp, FName Name, const FVector& Value);

};
