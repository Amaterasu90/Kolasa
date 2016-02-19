#include "Kolasa.h"
#include "IBlockable.h"

void IBlockable::ActivateMove() {
	this->bMoveIsActive = true;
}

void IBlockable::DeactivateMove(){
	this->bMoveIsActive = false;
}

void IBlockable::ActivateRotation(){
	this->bRotationIsActive = true;
}

void IBlockable::DeactivateRotation(){
	this->bRotationIsActive = false;
}

bool IBlockable::IsActiveMove() {
	return bMoveIsActive;
}

bool IBlockable::IsActiveRotation(){
	return this->bRotationIsActive;
}
