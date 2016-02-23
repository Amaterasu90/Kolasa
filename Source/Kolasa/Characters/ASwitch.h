// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class KOLASA_API ASwitch
{
public:
	virtual void Activate();
	virtual void Deactivate();
	bool IsActive();
	ASwitch();
	~ASwitch();
protected:
	bool bIsActive;
};
