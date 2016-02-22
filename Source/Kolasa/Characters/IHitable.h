// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class KOLASA_API IHitable
{
public:
	virtual FHitResult GetRayHit() = 0;
};
