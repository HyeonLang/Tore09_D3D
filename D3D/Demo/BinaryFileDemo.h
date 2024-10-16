#pragma once

#include "Systems/IExecute.h"

class BinaryFileDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	void CreateStaticMesh();

	void OnSaveCompleted(wstring path);
	void OnLoadCompleted(wstring path);

private:
	struct Vertex
	{
		Vector3 Position;
	};

private:
	Shader* shader;

	Sky* sky;

	Vector3 direction = Vector3(-1, -1, 1);
	ID3DX11EffectVectorVariable* sDirection;

	StaticMesh_Quad* quad;
	StaticMesh_Plane* plane;

	StaticMesh_Cube* cube;
	StaticMesh_Cylinder* cylinders[10];
	StaticMesh_Sphere* spheres[10];

	Shader* cubeMapShader;
	class CubeMapMesh* cubeMapMesh;

	Vector3 Location = Vector3(0, 0, 0);
};