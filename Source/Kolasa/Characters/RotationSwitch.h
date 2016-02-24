// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ASwitch.h"

/**
 * 
 */
class KOLASA_API RotationSwitch : public ASwitch
{
public:
	bool bIsEndSmoothRotation;
	virtual void EndSmootRotation();
	virtual void StartSmootRotation();
	bool IsDurningRotation();
	virtual void Activate() override;
	virtual void Deactivate() override;
	RotationSwitch();
	~RotationSwitch();
};
