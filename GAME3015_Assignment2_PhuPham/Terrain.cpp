#include "Terrain.h"

Terrain::Terrain()
{
	scrollZBounds = -120;
	scrollZRestartPosition = 170;
	speed = -30.f;
}

Terrain::~Terrain()
{

}

void Terrain::Update(float DeltaTime, FrameResource* Frame)
{
	Entity::Update(DeltaTime, Frame);

	SetVelocity(XMFLOAT3(0.f, 0.f, speed));

	if (m_Pos.z < scrollZBounds)
	{
		m_Pos.z = scrollZRestartPosition;
	}
}

void Terrain::Draw(ID3D12GraphicsCommandList* CmdList, ID3D12DescriptorHeap* Desc,
	UINT DescSize, struct FrameResource* Frame,
	MeshGeometry* Mesh, float DeltaTime)
{
	Entity::Draw(CmdList, Desc, DescSize, Frame, Mesh, DeltaTime);
}