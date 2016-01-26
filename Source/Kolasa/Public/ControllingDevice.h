// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ControllingDevice.generated.h"

/**
 * 
 */
UENUM()
enum EControllingDevice
{
	Mouse UMETA(DisplayName = "Mouse"),
	Touch UMETA(DisplayName = "Touch"), 
	Gyro UMETA(DisplayName = "Gyro")
};
