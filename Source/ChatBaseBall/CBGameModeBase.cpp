// Fill out your copyright notice in the Description page of Project Settings.


#include "CBGameModeBase.h"
#include "CBGameStateBase.h"
#include "CBPlayerController.h"
#include "EngineUtils.h"
#include "CBPlayerState.h"

void ACBGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ACBPlayerController* CBPlayerController = Cast<ACBPlayerController>(NewPlayer);
	if (IsValid(CBPlayerController) == true)
	{
		CBPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		if (!AllPlayerControllers.Contains(CBPlayerController))
		{
			AllPlayerControllers.Add(CBPlayerController);
		}

		ACBPlayerState* CBPlayerState = CBPlayerController->GetPlayerState<ACBPlayerState>();
		if (IsValid(CBPlayerState) == true)
		{
			CBPlayerState->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ACBGameStateBase* CBGameStateBase = GetGameState<ACBGameStateBase>();
		if (IsValid(CBGameStateBase) == true)
		{
			CBGameStateBase->MulticastRPCBroadcastLoginMessage(CBPlayerState->PlayerNameString);
		}
	}
}

FString ACBGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ACBGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString ACBGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ACBGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
}

void ACBGameModeBase::PrintChatMessageString(ACBPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	FString ChatMessageString = InChatMessageString;
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);

	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		IncreaseGuessCount(InChattingPlayerController);

		int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));

		for (TActorIterator<ACBPlayerController> It(GetWorld()); It; ++It)
		{
			ACBPlayerController* CBPlayerController = *It;
			if (IsValid(CBPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				CBPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);				
			}
		}
		JudgeGame(InChattingPlayerController, StrikeCount);
	}
	else
	{
		for (TActorIterator<ACBPlayerController> It(GetWorld()); It; ++It)
		{
			ACBPlayerController* CBPlayerController = *It;
			if (IsValid(CBPlayerController) == true)
			{
				CBPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void ACBGameModeBase::IncreaseGuessCount(ACBPlayerController* InChattingPlayerController)
{
	ACBPlayerState* CBPlayerState = InChattingPlayerController->GetPlayerState<ACBPlayerState>();
	if (IsValid(CBPlayerState) == true)
	{
		CBPlayerState->CurrentGuessCount++;
	}
}

void ACBGameModeBase::CheckDrawCondition()
{
	bool bIsDraw = true;

	for (const auto& CBPlayerController : AllPlayerControllers)
	{
		ACBPlayerState* CBPlayerState = CBPlayerController->GetPlayerState<ACBPlayerState>();
		if (IsValid(CBPlayerState) && CBPlayerState->CurrentGuessCount < CBPlayerState->MaxGuessCount)
		{
			bIsDraw = false;
			break;
		}
	}

	if (bIsDraw)
	{
		for (const auto& CBPlayerController : AllPlayerControllers)
		{
			CBPlayerController->ClientRPCPrintChatMessageString(TEXT("무승부"));
		}

		ResetGame();
	}
}

void ACBGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& CBPlayerController : AllPlayerControllers)
	{
		ACBPlayerState* CBPlayerState = CBPlayerController->GetPlayerState<ACBPlayerState>();
		if (IsValid(CBPlayerState) == true)
		{
			CBPlayerState->CurrentGuessCount = 0;
		}
	}

	TSet<ACBPlayerController*> UniqueControllers(AllPlayerControllers);
	for (ACBPlayerController* CBPlayerController : UniqueControllers)
	{
		FTimerHandle TempHandle;
		FTimerDelegate TimerCallback;
		TimerCallback.BindLambda([=]()
			{
				if (IsValid(CBPlayerController))
				{
					CBPlayerController->ClientRPCPrintChatMessageString(TEXT("새 게임이 시작되었습니다."));
				}
			});

		GetWorld()->GetTimerManager().SetTimer(TempHandle, TimerCallback, 1.0f, false);
	}
}

void ACBGameModeBase::JudgeGame(ACBPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ACBPlayerState* CBPlayerState = InChattingPlayerController->GetPlayerState<ACBPlayerState>();

		FString CombinedMessageString;
		if (IsValid(CBPlayerState))
		{
			CombinedMessageString = CBPlayerState->PlayerNameString + TEXT("의 승리!");
		}
		else
		{
			CombinedMessageString = TEXT("A player has won the game.");
		}

		for (const auto& CBPlayerController : AllPlayerControllers)
		{
			CBPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
		}
		
		ResetGame();
	}
	else
	{
		CheckDrawCondition();
	}
}