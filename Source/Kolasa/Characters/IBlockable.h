#pragma once



class IBlockable
{
public:
	virtual void ActivateMove();
	virtual void DeactivateMove();
	bool IsActiveMove();
private:
	bool bIsActive;
};

