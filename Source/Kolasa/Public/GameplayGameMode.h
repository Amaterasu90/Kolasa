// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "GameplayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KOLASA_API AGameplayGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AGameplayGameMode();
protected:
	AGameplayGameMode(TCHAR* pawnBlueprintPath);
private:
	void InitDefaultPawn(TCHAR* blueprintPath);
};
