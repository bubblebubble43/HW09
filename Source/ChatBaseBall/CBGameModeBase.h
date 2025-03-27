// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CBGameModeBase.generated.h"

class ACBPlayerController;

UCLASS()
class CHATBASEBALL_API ACBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;

	void PrintChatMessageString(ACBPlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void IncreaseGuessCount(ACBPlayerController* InChattingPlayerController);

	void CheckDrawCondition();

	void ResetGame();

	void JudgeGame(ACBPlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ACBPlayerController>> AllPlayerControllers;
};
