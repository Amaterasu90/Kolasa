#pragma once
class IBlockable
{
public:
	void BlockMove();
	void UnlockMove();
	bool IsActive();
private:
	bool bIsActive = false;
};

