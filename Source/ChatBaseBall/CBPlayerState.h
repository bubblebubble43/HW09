// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CBPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CHATBASEBALL_API ACBPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACBPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FString GetPlayerInfoString();

public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	//UPROPERTY(Replicated)
	int32 MaxGuessCount;
};
