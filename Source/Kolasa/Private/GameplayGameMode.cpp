// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "GameplayGameState.h"
#include "GameplayHUD.h"
#include "GameplayGameMode.h"
#include "GameplayCharacter.h"
#include "Characters/MyCharacter.h"
#include "GameplayPlayerController.h"

void AGameplayGameMode::InitDefaultPawn(TCHAR* blueprintPath) {
	static ConstructorHelpers::FObjectFinder<UBlueprint> Blueprint(blueprintPath);
	if (Blueprint.Object) {
		DefaultPawnClass = (UClass*)Blueprint.Object->GeneratedClass;
	}
	/*
	//dynamic load
	UBlueprint *characterBlueprint = LoadObject<UBlueprint>(NULL, blueprintPath);
	if (characterBlueprint != nullptr) {
		DefaultPawnClass = characterBlueprint->GetBlueprintClass();
	}*/
}

AGameplayGameMode::AGameplayGameMode() : AGameplayGameMode(TEXT("Blueprint'/Game/AnimStarterPack/Ue4ASP_Character.Ue4ASP_Character'")) {
	GameStateClass = AGameplayGameState::StaticClass();
	HUDClass = AGameplayHUD::StaticClass();
	PlayerControllerClass = AGameplayPlayerController::StaticClass();
	DefaultPawnClass = AMyCharacter::StaticClass();
}

AGameplayGameMode::AGameplayGameMode(TCHAR* pawnBlueprintPath){
	InitDefaultPawn(pawnBlueprintPath);
}