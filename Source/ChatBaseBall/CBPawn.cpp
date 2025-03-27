// Fill out your copyright notice in the Description page of Project Settings.


#include "CBPawn.h"
#include "ChatBaseBall.h"

void ACBPawn::BeginPlay()
{
	Super::BeginPlay();

	FString NetModeString = ChatBaseBallFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CBPawn::BeginPlay() %s [%s]"), *ChatBaseBallFunctionLibrary::GetNetModeString(this), *NetModeString);
	ChatBaseBallFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void ACBPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetModeString = ChatBaseBallFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CBPawn::PossessedBy() %s [%s]"), *ChatBaseBallFunctionLibrary::GetNetModeString(this), *NetModeString);
	ChatBaseBallFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}