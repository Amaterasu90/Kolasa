// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MoveSwitch.h"
#include "RotationSwitch.h"

/**
 * 
 */
class KOLASA_API SwitchContainer
{
public:
	SwitchContainer(MoveSwitch& moveInterface, RotationSwitch& rotationInterface);
	~SwitchContainer();
	MoveSwitch GetMoveSwitch();
	RotationSwitch GetRotationSwitch();
private:
	MoveSwitch move;
	RotationSwitch rotation;
};
