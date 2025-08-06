#pragma once
class ArrowObserver
{
public:
	virtual void OnRecoilArrow(bool flag)abstract;
	virtual ~ArrowObserver() = default;
};