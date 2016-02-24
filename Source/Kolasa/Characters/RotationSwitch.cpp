// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "RotationSwitch.h"

void RotationSwitch::EndSmootRotation(){
	bIsEndSmoothRotation = true;
	UE_LOG(ER_Log, Display, TEXT("End Smooth Rotation"));
}

void RotationSwitch::StartSmootRotation(){
	bIsEndSmoothRotation = false;
	UE_LOG(ER_Log, Display, TEXT("Start Smooth Rotation"));
}

bool RotationSwitch::IsDurningRotation(){
	return bIsEndSmoothRotation;
}

void RotationSwitch::Activate(){
	ASwitch::Activate();
	UE_LOG(ER_Log, Display, TEXT("Activate RotationSwitch"));
}

void RotationSwitch::Deactivate(){
	ASwitch::Deactivate();
	UE_LOG(ER_Log, Display, TEXT("Dectivate RotationSwitch"));
}

RotationSwitch::RotationSwitch() : ASwitch(){
	bIsEndSmoothRotation = false;
}

RotationSwitch::~RotationSwitch(){
	ASwitch::~ASwitch();
}
