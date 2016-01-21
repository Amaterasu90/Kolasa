// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "GameplayGameState.h"
#include "GameplayHUD.h"
#include "GameplayGameMode.h"

AGameplayGameMode::AGameplayGameMode() {
	GameStateClass = AGameplayGameState::StaticClass();
	HUDClass = AGameplayHUD::StaticClass();
}