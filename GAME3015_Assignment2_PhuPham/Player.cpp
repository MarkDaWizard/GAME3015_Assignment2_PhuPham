#include "Player.h"

/// <summary>
/// On construction, create the key bindings as well.
/// </summary>
Player::Player()
{
	m_Category = Category::Player;

	//Set bounds for player
	horizontalBounds = 28.f;
	verticalBoundFront = -10.f;
	verticalBoundBack = -33.f;

	//Assign keybinds for player
	m_Key[Action::MoveFront] = 'W';
	m_Key[Action::MoveBack] = 'S';
	m_Key[Action::MoveLeft] = 'A';
	m_Key[Action::MoveRight] = 'D';

	
}

Player::~Player()
{
}

void Player::Update(float DeltaTime, FrameResource* Frame)
{
	Entity::Update(DeltaTime, Frame);

}

void Player::Draw(ID3D12GraphicsCommandList* CmdList, ID3D12DescriptorHeap* Desc,
	UINT DescSize, struct FrameResource* Frame,
	MeshGeometry* Mesh, float DeltaTime)
{
	Entity::Draw(CmdList, Desc, DescSize, Frame, Mesh, DeltaTime);
}

void Player::HandleEvent(CommandQueue& Queue)
{
}

/// <summary>
/// Moves the player aircraft based on key inputs from player.
/// </summary>
/// <param name="Queue"></param>
void Player::HandleRealtimeInput(CommandQueue& Queue)
{
	//Moving forward
	if (GetAsyncKeyState(m_Key[Action::MoveFront]) & 0x8000)
	{
		if (m_Pos.z < verticalBoundFront)
		{
			Command	cmd;

			cmd.category = Category::Player;
			cmd.action = AircraftMover(0.f, 0.f, m_MoveSpeed);

			Queue.push(cmd);
		}
	}
	//Moving backward
	else if (GetAsyncKeyState(m_Key[Action::MoveBack]) & 0x8000)
	{
		if (m_Pos.z > verticalBoundBack)
		{
			Command	cmd;

			cmd.category = Category::Player;
			cmd.action = AircraftMover(0.f, 0.f, -m_MoveSpeed);

			Queue.push(cmd);
		}
	}
	//Moving left
	if (GetAsyncKeyState(m_Key[Action::MoveLeft]) & 0x8000)
	{
		if (m_Pos.x > -horizontalBounds)
		{
			Command	cmd;

			cmd.category = Category::Player;
			cmd.action = AircraftMover(-m_MoveSpeed, 0.f, 0.f);

			Queue.push(cmd);
		}
	}
	//Moving right
	else if (GetAsyncKeyState(m_Key[Action::MoveRight]) & 0x8000)
	{
		if (m_Pos.x < horizontalBounds)
		{
			Command	cmd;

			cmd.category = Category::Player;
			cmd.action = AircraftMover(m_MoveSpeed, 0.f, 0.f);

			Queue.push(cmd);
		}
	}
	
}
