// Fill out your copyright notice in the Description page of Project Settings.


#include "PartDevLevel/Character/TestFPVPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedActionKeyMapping.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Global/DataAssets/InputDatas.h"
#include "TestFPVCharacter.h"

ATestFPVPlayerController::ATestFPVPlayerController()
{
	TeamId = FGenericTeamId(0);
	{
		FString RefPathString = TEXT("InputDatas'/Game/Resources/Global/DataAssets/DA_InputDatas.DA_InputDatas'");
		ConstructorHelpers::FObjectFinder<UInputDatas> ResPath(*RefPathString);

		if (false == ResPath.Succeeded())
		{
			return;
		}

		InputData = ResPath.Object;
	}
}

void ATestFPVPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	{
		UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

		InputSystem->ClearAllMappings();
		InputSystem->AddMappingContext(InputData->InputMapping, 0);

		if (nullptr != InputData->InputMapping)
		{
			EnhancedInputComponent->BindAction(InputData->Actions[0], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::MouseRotation);
			EnhancedInputComponent->BindAction(InputData->Actions[1], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::MoveFront);
			EnhancedInputComponent->BindAction(InputData->Actions[2], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::MoveBack);
			EnhancedInputComponent->BindAction(InputData->Actions[3], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::MoveRight);
			EnhancedInputComponent->BindAction(InputData->Actions[4], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::MoveLeft);
			EnhancedInputComponent->BindAction(InputData->Actions[5], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::Jump);
			EnhancedInputComponent->BindAction(InputData->Actions[5], ETriggerEvent::Completed, this, &ATestFPVPlayerController::JumpEnd);
			EnhancedInputComponent->BindAction(InputData->Actions[6], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::Fire);
			EnhancedInputComponent->BindAction(InputData->Actions[6], ETriggerEvent::Completed, this, &ATestFPVPlayerController::FireEnd);
			EnhancedInputComponent->BindAction(InputData->Actions[7], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::ChangePostureController, static_cast<EPlayerPosture>(0));
			EnhancedInputComponent->BindAction(InputData->Actions[8], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::ChangePostureController, static_cast<EPlayerPosture>(1));
			EnhancedInputComponent->BindAction(InputData->Actions[9], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::ChangePostureController, static_cast<EPlayerPosture>(2));
			EnhancedInputComponent->BindAction(InputData->Actions[10], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::ChangePostureController, static_cast<EPlayerPosture>(3));
			EnhancedInputComponent->BindAction(InputData->Actions[11], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::ChangePostureController, static_cast<EPlayerPosture>(4));
			EnhancedInputComponent->BindAction(InputData->Actions[12], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::ChangePostureController, static_cast<EPlayerPosture>(5));
			EnhancedInputComponent->BindAction(InputData->Actions[13], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::PickUpItem);
			EnhancedInputComponent->BindAction(InputData->Actions[13], ETriggerEvent::Completed, this, &ATestFPVPlayerController::PickUpItemEnd);
		}
	}
}

void ATestFPVPlayerController::MouseRotation(const FInputActionValue& Value)
{
	FVector2D MouseXY = Value.Get<FVector2D>();
	AddYawInput(MouseXY.X);
	AddPitchInput(-MouseXY.Y);
}

void ATestFPVPlayerController::MoveFront(const FInputActionValue& Value)
{
	ChangeState(EPlayerState::Walk);
	FVector Forward = GetPawn()->GetActorForwardVector();
	GetPawn()->AddMovementInput(Forward);
}

void ATestFPVPlayerController::MoveBack(const FInputActionValue& Value)
{
	ChangeState(EPlayerState::Walk);
	FVector Forward = GetPawn()->GetActorForwardVector();
	GetPawn()->AddMovementInput(-Forward);
}

void ATestFPVPlayerController::MoveRight(const FInputActionValue& Value)
{
	ChangeState(EPlayerState::Walk);
	FVector Rightward = GetPawn()->GetActorRightVector();
	GetPawn()->AddMovementInput(Rightward);
}

void ATestFPVPlayerController::MoveLeft(const FInputActionValue& Value)
{
	ChangeState(EPlayerState::Walk);
	FVector Rightward = GetPawn()->GetActorRightVector();
	GetPawn()->AddMovementInput(-Rightward);
}

void ATestFPVPlayerController::Jump(const FInputActionValue& Value)
{
	ChangeState(EPlayerState::Jump);
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	Ch->Jump();
}

void ATestFPVPlayerController::JumpEnd(const FInputActionValue& Value)
{
	ChangeState(EPlayerState::Idle);
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	Ch->StopJumping();
}

void ATestFPVPlayerController::Fire(const FInputActionValue& Value)
{
	ChangeState(EPlayerState::Fire);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Fire!"));
}

void ATestFPVPlayerController::FireEnd(const FInputActionValue& Value)
{
	ChangeState(EPlayerState::Idle);
}

void ATestFPVPlayerController::PickUpItem()
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	//Ch->PickUpItem(FName("TestMelee"));	// (for Item Test)
	Ch->SetPickUp(true);
}

void ATestFPVPlayerController::PickUpItemEnd()
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	Ch->SetPickUp(false);
}

void ATestFPVPlayerController::ChangeState(EPlayerState _State)
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	Ch->ChangeState(_State);
}

void ATestFPVPlayerController::ChangePostureController(EPlayerPosture _Posture)
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	Ch->ChangePosture(_Posture);
}

FGenericTeamId ATestFPVPlayerController::GetGenericTeamId() const
{
	return TeamId;
}
