// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "SwitchContainer.h"

SwitchContainer::SwitchContainer(MoveSwitch& moveInterface, RotationSwitch& rotationInterface){
	this->move = moveInterface;
	this->rotation = rotationInterface;
}

SwitchContainer::~SwitchContainer(){
}

MoveSwitch SwitchContainer::GetMoveSwitch(){
	return move;
}

RotationSwitch SwitchContainer::GetRotationSwitch()
{
	return rotation;
}
