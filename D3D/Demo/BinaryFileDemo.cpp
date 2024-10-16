#include "stdafx.h"
#include "BinaryFileDemo.h"
#include "StaticMeshes/CubeMapMesh.h"
#include "Utilities/BinaryFile.h"

void BinaryFileDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(0, 36, -85);

	shader = new Shader(L"13_StaticMesh.fxo");
	sDirection = shader->AsVector("LightDirection");

	sky = new Sky(L"Environment/Mountain1024.dds");

	CreateStaticMesh();

	cubeMapShader = new Shader(L"14_CubeMap.fxo");
	cubeMapMesh = new CubeMapMesh(cubeMapShader);
	cubeMapMesh->Texture(L"Environment/DesertCube1024.dds");
	cubeMapMesh->GetTrasnform()->Position(0, 20, 0);
	cubeMapMesh->GetTrasnform()->Scale(10, 10, 10);
}

void BinaryFileDemo::Destroy()
{
	SafeDelete(shader);
	
	SafeDelete(quad);
	SafeDelete(plane);

	SafeDelete(cube);
	
	for (UINT i = 0; i < 10; i++)
	{
		SafeDelete(spheres[i]);
		SafeDelete(cylinders[i]);
	}

	SafeDelete(cubeMapShader);
	SafeDelete(cubeMapMesh);

	SafeDelete(sky);
}

void BinaryFileDemo::Update()
{
	ImGui::SliderFloat3("Light Direction", direction, -1, 1);
	sDirection->SetFloatVector(direction);

	static int pass = sky->GetShader()->PassCount() - 1;
	ImGui::InputInt("Pass", &pass);
	pass %= sky->GetShader()->PassCount();
	sky->Pass(pass);

	
	static bool bWire;
	ImGui::Checkbox("Wire Frame", &bWire);

	ImGui::SliderFloat3("Location", Location, -100, 100);
	cube->GetTransform()->Position(Location);

	if (ImGui::Button("Save Location"))
	{
		function<void(wstring)> Delegate;
		Delegate = bind(&BinaryFileDemo::OnSaveCompleted, this, placeholders::_1);
		Path::SaveFileDialog(L"", L"Binary\0*.bin", L"../../_Datas/", Delegate, D3D::GetDesc().Handle);
	}

	ImGui::SameLine();
	if (ImGui::Button("Load Location"))
	{
		function<void(wstring)> Delegate;
		Delegate = bind(&BinaryFileDemo::OnLoadCompleted, this, placeholders::_1);
		Path::OpenFileDialog(L"", L"Binary\0*.bin", L"../../_Datas/", Delegate, D3D::GetDesc().Handle);
	}

	sky->Update();

	quad->Pass(bWire ? 1 : 0);
	plane->Pass(bWire ? 1 : 0);
	cube->Pass(bWire ? 1 : 0);

	quad->Update();
	plane->Update();

	cube->Update();

	for (UINT i = 0; i < 10; i++)
	{
		cylinders[i]->Pass(bWire ? 1 : 0);
		cylinders[i]->Update();

		spheres[i]->Pass(bWire ? 1 : 0);
		spheres[i]->Update();
	}

	cubeMapMesh->Update();
}

void BinaryFileDemo::Render()
{
	sky->Render();

	for (UINT i = 0; i < 10; i++)
	{
		cylinders[i]->Render();
		spheres[i]->Render();
	}

	cube->Render();
	quad->Render();
	plane->Render();
	

	cubeMapMesh->Render();
}

void BinaryFileDemo::CreateStaticMesh()
{
	quad = new StaticMesh_Quad(shader);
	quad->DiffuseMap(L"Box.png");

	plane = new StaticMesh_Plane(shader, 2.5f, 2.5f);
	plane->GetTransform()->Scale(12, 1, 12);
	plane->DiffuseMap(L"Floor.png");

	cube = new StaticMesh_Cube(shader);
	cube->GetTransform()->Position(0, 5, 0);
	cube->GetTransform()->Scale(20, 10, 20);
	cube->DiffuseMap(L"Stones.png");

	for (UINT i = 0; i < 5; i++)
	{
		//Cylinder - Left
		cylinders[i * 2 + 0] = new StaticMesh_Cylinder(shader, 0.3f, 0.5f, 3.f, 20, 20);
		cylinders[i * 2 + 0]->GetTransform()->Position(-30, 6, (float)i * 15.f - 15.f);
		cylinders[i * 2 + 0]->GetTransform()->Scale(5, 5, 5);
		cylinders[i * 2 + 0]->DiffuseMap(L"Bricks.png");

		//Cylinder - Right
		cylinders[i * 2 + 1] = new StaticMesh_Cylinder(shader, 0.3f, 0.5f, 3.f, 20, 20);
		cylinders[i * 2 + 1]->GetTransform()->Position(+30, 6, (float)i * 15.f - 15.f);
		cylinders[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		cylinders[i * 2 + 1]->DiffuseMap(L"Bricks.png");

		//Sphere - Left
		spheres[i * 2 + 0] = new StaticMesh_Sphere(shader, 0.5f);
		spheres[i * 2 + 0]->GetTransform()->Position(-30, 15, (float)i * 15.f - 15.f);
		spheres[i * 2 + 0]->GetTransform()->Scale(5, 5, 5);
		spheres[i * 2 + 0]->DiffuseMap(L"Wall.png");

		//Sphere - Right
		spheres[i * 2 + 1] = new StaticMesh_Sphere(shader, 0.5f);
		spheres[i * 2 + 1]->GetTransform()->Position(+30, 15, (float)i * 15.f - 15.f);
		spheres[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		spheres[i * 2 + 1]->DiffuseMap(L"Wall.png");
	}
}

void BinaryFileDemo::OnSaveCompleted(wstring path)
{
	BinaryWriter* Writer = new BinaryWriter(path + L".bin");

	Writer->Vector3(Location);
	Writer->String("Cube");

	SafeDelete(Writer);
}

void BinaryFileDemo::OnLoadCompleted(wstring path)
{
	BinaryReader* Reader = new BinaryReader(path);

	Location = Reader->Vector3();
	string Name = Reader->String();

	wstring Message = path + L"파일 불러오기" + String::ToWString(Name);
	MessageBox(D3D::GetDesc().Handle, Message.c_str(), L"ME", MB_OKCANCEL);

	SafeDelete(Reader);
}
