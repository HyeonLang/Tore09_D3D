#pragma once

#include "Systems/IExecute.h"

class TextureBufferDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override {};
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	Shader* shader;
	Texture* texture;

	TextureBuffer* textureBuffer;
	Canvas* canvas;

};