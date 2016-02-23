#pragma once



class IBlockable
{
public:
	bool bIsEndSmoothRotation;
	virtual void ActivateMove();
	virtual void DeactivateMove();
	virtual void ActivateRotation();
	virtual void DeactivateRotation();
	bool IsActiveMove();
	bool IsActiveRotation();
	IBlockable();
	~IBlockable();
private:
	bool _bMoveIsActive;
	bool _bRotationIsActive;
};

