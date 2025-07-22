#pragma once

class BaseManager
{
public:

	virtual ~BaseManager() {};

	//�������z�֐�	
	virtual void Create() abstract;
	virtual void Initialize() abstract;
	virtual void Update() abstract;
	virtual bool Draw() abstract;

};

