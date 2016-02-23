#include "Kolasa.h"
#include "IBlockable.h"

void IBlockable::ActivateMove() {
	this->_bMoveIsActive = true;
}

void IBlockable::DeactivateMove(){
	this->_bMoveIsActive = false;
}

void IBlockable::ActivateRotation(){
	this->_bRotationIsActive = true;
}

void IBlockable::DeactivateRotation(){
	this->_bRotationIsActive = false;
}

bool IBlockable::IsActiveMove() {
	return _bMoveIsActive;
}

bool IBlockable::IsActiveRotation(){
	return this->_bRotationIsActive;
}

IBlockable::IBlockable(){
	bIsEndSmoothRotation = false;
	_bMoveIsActive = false;
	_bRotationIsActive = false;
}

IBlockable::~IBlockable(){
}
