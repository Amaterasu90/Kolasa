// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "MoveSwitch.h"

void MoveSwitch::Activate(){
	ASwitch::Activate();
	UE_LOG(ER_Log, Display, TEXT("Activate MoveSwitch"));
}

void MoveSwitch::Deactivate(){
	ASwitch::Deactivate();
	UE_LOG(ER_Log, Display, TEXT("Dectivate MoveSwitch"));
}

MoveSwitch::MoveSwitch() : ASwitch(){
}

MoveSwitch::~MoveSwitch(){
	ASwitch::~ASwitch();
}
