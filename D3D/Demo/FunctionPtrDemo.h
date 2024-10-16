#pragma once

#include "Systems/IExecute.h"

class MyClass
{
public:
	static void StaticFunc()
	{
		printf(__FUNCTION__);
		printf("\n");
	}

	void MemberFunc()
	{
		printf(__FUNCTION__);
		printf("\n");
	}
};

class FunctionPtrDemo : public IExecute
{
public:
	// Inherited via IExecute
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override {};
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	void OnSaveCompleted(wstring path);
	void OnLoadCompleted(wstring path);

};