// Fill out your copyright notice in the Description page of Project Settings.


#include "CBGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "CBPlayerController.h"

void ACBGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ACBPlayerController* CBPC = Cast<ACBPlayerController>(PC);
			if (IsValid(CBPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				CBPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}