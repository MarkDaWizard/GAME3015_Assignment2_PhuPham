//******************************************//
//	GAME3015_Assignment2_PhuPham			//
//	Name:Phu Pham							//
//	ID:101250748							//
//											//
//******************************************//
#include "World.h"
#include <iostream>

World::World()
{
}

World::~World()
{
	delete m_Mesh;
	delete	m_RootNode;
}

/// <summary>
/// Set the game world objects
/// </summary>
/// <param name="Device"></param>
/// <param name="CmdList"></param>
void World::buildWorld(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList)
{
	m_RootNode = new SceneNode;

	GeometryGenerator geoGen;

	GeometryGenerator::MeshData Quad = geoGen.CreateQuad(-0.5f, 0.5f, 1.f, 1.f, 0.f);
	UINT gridVertexOffset = 0;
	UINT gridIndexOffset = 0;
	SubmeshGeometry QuadSubmesh;
	QuadSubmesh.IndexCount = (UINT)Quad.Indices32.size();
	QuadSubmesh.StartIndexLocation = gridIndexOffset;
	QuadSubmesh.BaseVertexLocation = gridVertexOffset;

	auto totalVertexCount =
		Quad.Vertices.size();

	std::vector<Vertex> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < Quad.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = Quad.Vertices[i].Position;
		vertices[k].Normal = Quad.Vertices[i].Normal;
		vertices[k].TexC = Quad.Vertices[i].TexC;
	}

	std::vector<std::uint16_t> indices;

	indices.insert(indices.end(), std::begin(Quad.GetIndices16()), std::end(Quad.GetIndices16()));

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	m_Mesh = new MeshGeometry;
	m_Mesh->Name = "Quad";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &m_Mesh->VertexBufferCPU));
	CopyMemory(m_Mesh->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &m_Mesh->IndexBufferCPU));
	CopyMemory(m_Mesh->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	m_Mesh->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(Device,
		CmdList, vertices.data(), vbByteSize, m_Mesh->VertexBufferUploader);

	m_Mesh->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(Device,
		CmdList, indices.data(), ibByteSize, m_Mesh->IndexBufferUploader);


	m_Mesh->VertexByteStride = sizeof(Vertex);
	m_Mesh->VertexBufferByteSize = vbByteSize;
	m_Mesh->IndexFormat = DXGI_FORMAT_R16_UINT;
	m_Mesh->IndexBufferByteSize = ibByteSize;

	m_Mesh->DrawArgs["Quad"] = QuadSubmesh;

	// Terrain
	Terrain1->m_Pos = XMFLOAT3(0.f, 0.f, 0.f);
	Terrain1->m_Scale = XMFLOAT3(180.f, 146.f, 1.f);
	Terrain1->m_Rot = XMFLOAT3(3.14159f / 2.f, 0.f, 0.f);
	Terrain1->m_MBIndex = 0;
	Terrain1->m_TexIndex = 0;
	m_RootNode->AddChild(Terrain1);
	m_RenderList.push_back(Terrain1);

	Terrain2->m_Pos = XMFLOAT3(0.f, 0.f, 145.f);
	Terrain2->m_Scale = XMFLOAT3(180.f, 145.f, 1.f);
	Terrain2->m_Rot = XMFLOAT3(3.14159f / 2.f, 0.f, 0.f);
	Terrain2->m_MBIndex = 0;
	Terrain2->m_TexIndex = 0;
	m_RootNode->AddChild(Terrain2);
	m_RenderList.push_back(Terrain2);


	// Player
	player->m_Pos = XMFLOAT3(0.f, 10.f, -10.f);
	player->m_Scale = XMFLOAT3(10.f, 10.f, 1.f);
	player->m_Rot = XMFLOAT3(3.14159f / 2.f, 0.f, 0.f);
	player->m_MBIndex = 1;
	player->m_TexIndex = 1;
	m_RootNode->AddChild(player);
	m_RenderList.push_back(player);


	// Enemy1
	Enemy1->m_Pos = XMFLOAT3(0.f, 10.f, 15.f);
	Enemy1->m_Scale = XMFLOAT3(8.f, 8.f, 1.f);
	Enemy1->m_Rot = XMFLOAT3(3.14159f / 1.5f, 0.f, 0.f);
	Enemy1->m_MBIndex = 2;
	Enemy1->m_TexIndex = 2;
	m_RootNode->AddChild(Enemy1);
	m_RenderList.push_back(Enemy1);

	// Enemy2
	Enemy2->m_Pos = XMFLOAT3(10.f, 15.f, 28.f);
	Enemy2->m_Scale = XMFLOAT3(8.f, 8.f, 1.f);
	Enemy2->m_Rot = XMFLOAT3(3.14159f / 1.5f, 0.f, 0.f);
	Enemy2->m_MBIndex = 2;
	Enemy2->m_TexIndex = 2;
	m_RootNode->AddChild(Enemy2);
	m_RenderList.push_back(Enemy2);

	// Wingman1
	Wingman1->m_Pos = XMFLOAT3(-10.f, 10.f, -15.f);
	Wingman1->m_Scale = XMFLOAT3(5.f, 5.f, 1.f);
	Wingman1->m_Rot = XMFLOAT3(3.14159f / 2.f, 0.f, 0.f);
	Wingman1->m_MBIndex = 3;
	Wingman1->m_TexIndex = 1;
	player->AddChild(Wingman1);
	m_RenderList.push_back(Wingman1);

	// Wingman2
	Wingman2->m_Pos = XMFLOAT3(10.f, 10.f, -15.f);
	Wingman2->m_Scale = XMFLOAT3(5.f, 5.f, 1.f);
	Wingman2->m_Rot = XMFLOAT3(3.14159f / 2.f, 0.f, 0.f);
	Wingman2->m_MBIndex = 3;
	Wingman2->m_TexIndex = 1;
	player->AddChild(Wingman2);
	m_RenderList.push_back(Wingman2);


}

void World::ProcessInput()
{
	player->HandleEvent(m_CommandQueue);
	player->HandleRealtimeInput(m_CommandQueue);
}

void World::Update(float DeltaTime, struct FrameResource* Frame)
{
	while (!m_CommandQueue.isEmpty())
	{
		m_RootNode->OnCommand(m_CommandQueue.pop(), DeltaTime);
	}

	m_RootNode->m_GlobalCBIndex = 0;
	m_RootNode->Update(DeltaTime, Frame);
}

void World::Draw(ID3D12GraphicsCommandList* CmdList, ID3D12DescriptorHeap* Desc,
	UINT DescSize, struct FrameResource* Frame, float DeltaTime)
{
	m_RootNode->Draw(CmdList, Desc, DescSize, Frame, m_Mesh, DeltaTime);
}
