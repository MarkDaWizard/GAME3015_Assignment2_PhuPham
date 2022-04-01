#include "SceneNode.h"
#include "FrameResource.h"

int SceneNode::m_GlobalCBIndex = 0;

SceneNode::SceneNode()	:
	m_Parent(nullptr),
	m_Render(false),
	m_MBIndex(-1),
	m_CBIndex(-1),
	m_TexIndex(0),
	m_Category(Category::Scene)
{
}

SceneNode::~SceneNode()
{
	auto	iter = m_ChildList.begin();
	auto	iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		delete* iter;
	}
}

void SceneNode::AddChild(SceneNode* Child)
{
	Child->m_Parent = this;

	m_ChildList.push_back(Child);
}

void SceneNode::OnCommand(const Command& Cmd, float DeltaTime)
{
	if (Cmd.category == m_Category)
		Cmd.action(*this, DeltaTime);

	auto	iter = m_ChildList.begin();
	auto	iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->OnCommand(Cmd, DeltaTime);
	}
}

void SceneNode::Update(float DeltaTime, FrameResource* Frame)
{
	if (m_Render)
	{
		XMMATRIX	matScale, matRot, matPos;

		matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		matRot = XMMatrixRotationX(m_Rot.x) * XMMatrixRotationY(m_Rot.y) * XMMatrixRotationZ(m_Rot.z);
		matPos = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);

		XMMATRIX	matWorld = matScale * matRot * matPos;
		XMStoreFloat4x4(&m_matWorld, matWorld);

		m_CBIndex = m_GlobalCBIndex;
		++m_GlobalCBIndex;

		matWorld = XMMatrixTranspose(matWorld);

		auto currObjectCB = Frame->ObjectCB.get();

		ObjectConstants objConstants;
		XMStoreFloat4x4(&objConstants.World, matWorld);

		currObjectCB->CopyData(m_CBIndex, objConstants);
	}

	UpdateChild(DeltaTime, Frame);
}

void SceneNode::Draw(ID3D12GraphicsCommandList* CmdList, ID3D12DescriptorHeap* Desc,
	UINT DescSize, struct FrameResource* Frame,
	MeshGeometry* Mesh, float DeltaTime)
{
	if (m_CBIndex != -1 && m_MBIndex != -1)
	{
		UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
		UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

		auto objectCB = Frame->ObjectCB->Resource();
		auto matCB = Frame->MaterialCB->Resource();

		CmdList->IASetVertexBuffers(0, 1, &Mesh->VertexBufferView());
		CmdList->IASetIndexBuffer(&Mesh->IndexBufferView());
		CmdList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		CD3DX12_GPU_DESCRIPTOR_HANDLE tex(
			Desc->GetGPUDescriptorHandleForHeapStart()
			);

		tex.Offset(m_TexIndex, DescSize);

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + m_CBIndex * objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + m_MBIndex * matCBByteSize;

		CmdList->SetGraphicsRootDescriptorTable(0, tex);
		CmdList->SetGraphicsRootConstantBufferView(1, objCBAddress);
		CmdList->SetGraphicsRootConstantBufferView(3, matCBAddress);

		CmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);
	}

	auto	iter = m_ChildList.begin();
	auto	iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Draw(CmdList, Desc, DescSize, Frame, Mesh, DeltaTime);
	}
}

void SceneNode::UpdateChild(float DeltaTime, FrameResource* Frame)
{
	auto	iter = m_ChildList.begin();
	auto	iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Update(DeltaTime, Frame);
	}
}
