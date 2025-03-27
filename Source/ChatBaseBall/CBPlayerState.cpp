// Fill out your copyright notice in the Description page of Project Settings.


#include "CBPlayerState.h"
#include "Net/UnrealNetwork.h"

ACBPlayerState::ACBPlayerState()
	: PlayerNameString(TEXT("None"))
	, CurrentGuessCount(0)
	, MaxGuessCount(3)
{
	bReplicates = true;
}

void ACBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	//DOREPLIFETIME(ThisClass, MaxGuessCount);
}

FString ACBPlayerState::GetPlayerInfoString()
{
	int32 DisplayCount = FMath::Clamp(CurrentGuessCount + 1, 1, MaxGuessCount);
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(DisplayCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;
}
