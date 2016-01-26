// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "ControllingDevice.h"
#include "GameplayPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KOLASA_API AGameplayPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayPlayerController")
	EControllingDevice GetCurrentControllingDevice();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayPlayerController")
	EControllingDevice SetCurrentControllingDevice(EControllingDevice value);
	UFUNCTION(BlueprintCallable, Category = "GameplayPlayerController")
	float GetSensivity(EControllingDevice whichDevice, bool currentDevice);
	UFUNCTION(BlueprintCallable,  Category = "GameplayPlayerController")
	void SetSensivity(EControllingDevice whichDevice, float newSensivity);
	virtual void SetupInputComponent() override;
	UPROPERTY(BlueprintReadOnly)
	float MouseSensitivityMin = 0.1f;
	UPROPERTY(BlueprintReadOnly)
	float MouseSensitivityMax = 10.0f;
	UPROPERTY(BlueprintReadOnly)
	float TouchSensitivityMin = 15.0f;
	UPROPERTY(BlueprintReadOnly)
	float TouchSensitivityMax = 5.0f;
	UPROPERTY(BlueprintReadOnly)
	float GyroSensitivityMin = 20.0f;
	UPROPERTY(BlueprintReadOnly)
	float GyroSensitivityMax = 60.0f;
	
private:
	EControllingDevice CurrentControlingDevice = EControllingDevice::Mouse;
	float MouseSensitivityCurrent = 1.0f;
	float TouchSensitivityCurrent = 10.0f;
	float GyroSensitivityCurrent = 40.0f;
	FVector LastTilt;
	FVector LastTouch;

	void Pitch(float AxisValue);
	void Yaw(float AxisValue);
	void PitchYaw(FVector Value);
	void TouchPress(ETouchIndex::Type type, FVector loc);
	void TouchMove(ETouchIndex::Type type, FVector loc);
	void DebugAction();
};
