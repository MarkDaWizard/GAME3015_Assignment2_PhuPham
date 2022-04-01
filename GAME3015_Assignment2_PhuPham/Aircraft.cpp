#include "Aircraft.h"


Aircraft::Aircraft()
{
	m_velocity = XMFLOAT3(20.f,0.f,0.f);
	m_Dir = 1.f;
	m_Category = Category::Enemy;
	offsetLimit = 25;
}

Aircraft::~Aircraft()
{
}

void Aircraft::Update(float DeltaTime, FrameResource* Frame)
{
	Entity::Update(DeltaTime, Frame);

	m_Pos.x += m_MoveSpeed * m_Dir * DeltaTime;

	//Set movement direction depending on position
	if (m_Pos.x >= startingPos.x + offsetLimit)
	{
		m_Pos.x = startingPos.x + offsetLimit;
		m_Dir = -1.f;
	}

	else if (m_Pos.x <= -startingPos.x - offsetLimit)
	{
		m_Pos.x = startingPos.x - offsetLimit;
		m_Dir = 1.f;
	}

	//Rotate aircraft depending on movement direction
	if (m_Dir < 0.f)
	{
		m_Rot.z = 10.f;
	}

	else if (m_Dir > 0.f)
	{
		m_Rot.z = -10.f;
	}

	else
	{
		m_Rot.z = 0.f;
	}
}

void Aircraft::Draw(ID3D12GraphicsCommandList* CmdList, ID3D12DescriptorHeap* Desc,
	UINT DescSize, struct FrameResource* Frame,
	MeshGeometry* Mesh, float DeltaTime)
{
	Entity::Draw(CmdList, Desc, DescSize, Frame, Mesh, DeltaTime);
}


