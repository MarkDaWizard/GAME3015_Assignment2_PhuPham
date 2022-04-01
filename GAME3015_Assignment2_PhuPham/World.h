#pragma once

#include "Entity.h"
#include "ScrollingGround.h"
#include "Player.h"
#include "AirCraft.h"

/// <summary>
/// World.h and World.cpp scripts are where the game scene is built and rendered.
/// </summary>

class World
{
public:
	World();
	~World();

public:
	/// <summary>
	/// Rootnode keeps the entities in the scene, this would be the backgrounds, characters
	/// </summary>
	SceneNode* m_RootNode;

	std::map<std::string, std::unique_ptr<Material>> mMaterials;

	/// <summary>
	/// SceneNode list keeps all entities to be rendered when Draw() is called.
	/// </summary>
	std::list<SceneNode*>	m_RenderList;
	MeshGeometry*	m_Mesh;

	CommandQueue	m_CommandQueue;

	//Terrain 
	ScrollingGround* Terrain1 = new ScrollingGround;
	ScrollingGround* Terrain2 = new ScrollingGround;
	//Wingman
	Entity* Wingman1 = new Player;
	Entity* Wingman2 = new Player;
	//Enemy
	Aircraft* Enemy1 = new Aircraft;
	Aircraft* Enemy2 = new Aircraft;
	//Player
	Player* player = new Player;

	/// <summary>
	/// buildWorld creates and sets the start transform of the entities in the game (called at initialiaztion from Game.cpp). 
	/// </summary>
	/// <param name="Device"></param>
	/// <param name="CmdList"></param>
	void buildWorld(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);

	void ProcessInput();

	/// <summary>
	/// Call all updates for each entity in the rootnode list and scroll the backgrounf
	/// </summary>
	/// <param name="DeltaTime"></param>
	/// <param name="Frame"></param>
	void Update(float DeltaTime, struct FrameResource* Frame);
	/// <summary>
	/// Render all entities in the game scene
	/// </summary>
	/// <param name="CmdList"></param>
	/// <param name="Desc"></param>
	/// <param name="DescSize"></param>
	/// <param name="Frame"></param>
	/// <param name="DeltaTime"></param>
	void Draw(ID3D12GraphicsCommandList* CmdList, ID3D12DescriptorHeap* Desc,
		UINT DescSize, struct FrameResource* Frame, float DeltaTime);
};

