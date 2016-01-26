// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Blueprint/WidgetTree.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Editor/UMGEditor/Public/WidgetBlueprint.h"
#include "GameplayPlayerController.h"


EControllingDevice AGameplayPlayerController::GetCurrentControllingDevice() {
	return CurrentControlingDevice;
}

EControllingDevice AGameplayPlayerController::SetCurrentControllingDevice(const EControllingDevice value) {
	CurrentControlingDevice = value;
	return CurrentControlingDevice;
}

float AGameplayPlayerController::GetSensivity(const EControllingDevice whichDevice, const bool currentDevice) {
	float LocalSenivity;
	if (currentDevice) {
		switch (CurrentControlingDevice)
		{
		case Mouse:
			LocalSenivity = MouseSensitivityCurrent;
			break;
		case Touch:
			LocalSenivity = TouchSensitivityCurrent;
			break;
		case Gyro:
			LocalSenivity = GyroSensitivityCurrent;
			break;
		default:
			LocalSenivity = 0.0f;
			break;
		}
	}
	else {
		switch (whichDevice)
		{
		case Mouse:
			LocalSenivity = MouseSensitivityCurrent;
			break;
		case Touch:
			LocalSenivity = TouchSensitivityCurrent;
			break;
		case Gyro:
			LocalSenivity = GyroSensitivityCurrent;
			break;
		default:
			LocalSenivity = 0.0f;
			break;
		}
	}
	return LocalSenivity;
}

void AGameplayPlayerController::SetSensivity(const EControllingDevice whichDevice, const float newSensivity) {
	switch (whichDevice)
	{
	case Mouse:
		MouseSensitivityCurrent = newSensivity;
		break;
	case Touch:
		TouchSensitivityCurrent = newSensivity;
		break;
	case Gyro:
		GyroSensitivityCurrent = newSensivity;
		break;
	default:
		break;
	}
}

void AGameplayPlayerController::SetupInputComponent(){
	Super::SetupInputComponent();
	InputComponent->BindAxis("Mouse X", this, &AGameplayPlayerController::Yaw);
	InputComponent->BindAxis("Mouse Y", this, &AGameplayPlayerController::Pitch);
	InputComponent->BindVectorAxis("Tilt", this, &AGameplayPlayerController::PitchYaw);
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGameplayPlayerController::TouchPress);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AGameplayPlayerController::TouchMove);
	InputComponent->BindKey(EKeys::O, IE_Released, this, &AGameplayPlayerController::DebugAction);
}

void AGameplayPlayerController::Pitch(const float AxisValue) {
	if (CurrentControlingDevice == EControllingDevice::Mouse) {
		float pitch = AxisValue * MouseSensitivityCurrent;
		AddPitchInput(pitch);
	}
}

void AGameplayPlayerController::Yaw(const float AxisValue) {
	if (CurrentControlingDevice == EControllingDevice::Mouse) {
		float yaw = AxisValue * MouseSensitivityCurrent;
		AddYawInput(yaw);
	}

}

void AGameplayPlayerController::PitchYaw(const FVector Value) {
	if (CurrentControlingDevice == EControllingDevice::Gyro) {
		FVector scaledValue = Value * GyroSensitivityCurrent;
		FVector substractLastTilt = scaledValue - LastTilt;
		float pitch = substractLastTilt.Z;
		AddPitchInput(pitch);
		float yaw = substractLastTilt.X * -1.0f;
		AddYawInput(yaw);
		LastTilt = scaledValue;
	}
}

void AGameplayPlayerController::TouchPress(ETouchIndex::Type type, FVector localization) {
	if (type == ETouchIndex::Touch1) {
		LastTouch = localization;
	}
}

void AGameplayPlayerController::TouchMove(ETouchIndex::Type type, FVector localization) {
	if (type == ETouchIndex::Touch1) {
		FVector currentLocation = localization;
		currentLocation = currentLocation - LastTouch;
		float yaw = currentLocation.X / TouchSensitivityCurrent;
		AddYawInput(yaw);
		float pitch = currentLocation.Y / TouchSensitivityCurrent;
		AddPitchInput(pitch);
		LastTouch = localization;
	}
}

void AGameplayPlayerController::DebugAction() {
	FInputModeUIOnly inputMode = FInputModeUIOnly();
	inputMode.SetLockMouseToViewport(true);
	SetInputMode(inputMode);

	bShowMouseCursor = true;

	UClass * sensivityWidgetClass;
	FString address = "/Game/UI_Debug_ChangeSensitivity.UI_Debug_ChangeSensitivity";
	UWidgetBlueprint * sensivityBlueprint = Cast<UWidgetBlueprint>(StaticLoadObject(UWidgetBlueprint::StaticClass(), NULL, *address, NULL, LOAD_None, NULL));
	check(sensivityBlueprint) {
		sensivityWidgetClass = sensivityBlueprint->GeneratedClass;
		UUserWidget * sensivityWidget = CreateWidget<UUserWidget>(this, sensivityWidgetClass);
		sensivityWidget->AddToViewport();
	}
}

