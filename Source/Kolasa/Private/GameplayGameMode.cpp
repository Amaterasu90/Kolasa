// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "GameplayGameState.h"
#include "GameplayHUD.h"
#include "GameplayGameMode.h"
#include "GameplayCharacter.h"
#include "GameplayPlayerController.h"




AGameplayGameMode::AGameplayGameMode() {
	GameStateClass = AGameplayGameState::StaticClass();
	HUDClass = AGameplayHUD::StaticClass();
	PlayerControllerClass = AGameplayPlayerController::StaticClass();
	static ConstructorHelpers::FObjectFinder<UBlueprint> Blueprint(TEXT("Blueprint'/Game/AnimStarterPack/Ue4ASP_Character.Ue4ASP_Character'"));
	if (Blueprint.Object) {
		DefaultPawnClass = (UClass*)Blueprint.Object->GeneratedClass;
	}
}