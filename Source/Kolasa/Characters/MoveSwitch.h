// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ASwitch.h"
/**
 * 
 */
class KOLASA_API MoveSwitch : public ASwitch
{
public:
	virtual void Activate() override;
	virtual void Deactivate() override;
	MoveSwitch();
	~MoveSwitch();
};
