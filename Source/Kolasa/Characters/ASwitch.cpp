// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "ASwitch.h"

bool ASwitch::IsActive()
{
	return bIsActive;
}

void ASwitch::Activate() {
	ASwitch::bIsActive = true;
}

void ASwitch::Deactivate() {
	ASwitch::bIsActive = false;
}

ASwitch::ASwitch(){
	bIsActive = false;
}

ASwitch::~ASwitch()
{
}
