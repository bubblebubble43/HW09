// Fill out your copyright notice in the Description page of Project Settings.


#include "CBPlayerController.h"
#include "CBChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ChatBaseBall.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "CBGameModeBase.h"
#include "CBPlayerState.h"
#include "Net/UnrealNetwork.h"

ACBPlayerController::ACBPlayerController()
{
	bReplicates = true;
}

void ACBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UCBChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void ACBPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ACBPlayerState* CBPlayState = GetPlayerState<ACBPlayerState>();
	if (IsValid(CBPlayState) == false)
	{
		return;
	}

	// 입력 제한 검사
	if (CBPlayState->CurrentGuessCount >= CBPlayState->MaxGuessCount)
	{
		ClientRPCPrintChatMessageString(TEXT("기회 소진"));
	
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
		if (IsValid(GameMode))
		{
			ACBGameModeBase* CBGameMode = Cast<ACBGameModeBase>(GameMode);
			if (IsValid(CBGameMode))
			{
				CBGameMode->CheckDrawCondition();
			}
		}
		return;
	}
	
	ChatMessageString = InChatMessageString;
	
	if (IsLocalController() == true)
	{		
		FString CombinedMessageString = CBPlayState->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;
		ServerRPCPrintChatMessageString(CombinedMessageString);
	}
}

void ACBPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	ChatBaseBallFunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}


void ACBPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void ACBPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (IsValid(GameMode) == true)
	{
		ACBGameModeBase* CBGameMode = Cast<ACBGameModeBase>(GameMode);
		if (IsValid(CBGameMode) == true)
		{
			CBGameMode->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

void ACBPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void ACBPlayerController::ClientShowNotification_Implementation(const FString& InMessage)
{
	NotificationText = FText::FromString(InMessage);
}