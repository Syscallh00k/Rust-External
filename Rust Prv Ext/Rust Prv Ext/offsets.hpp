#include <Windows.h>
#include <iostream>

#define Create(name,value) uintptr_t m_##name = value

namespace ESP {
	bool box = false;
	bool FilledBox = false;
	bool CornerBox = false;
	bool Distance = false;
	bool SnapLines = false;
	bool Skeleton = false;
	bool Weapon = false;
	bool Name = false;

	float BoxColor[4] = {255,255,255,255};
}

namespace Offset {
	Create(System_Collections_Generic_List_BaseGameMode_TypeInfo, 0x3C58568);
	Create(Collision, 0x610);
	Create(CameraMode, 0x660);
	Create(eyes, 0x840);
	Create(PlayerModel, 0x38);
	Create(Position, 0x90);
	Create(ClanId, 0x5D8);
	Create(Movement, 0x608);
	Create(clActiveItem, 0x748);
	Create(ModelState, 0x778);
	Create(PlayerFlags, 0x838);
	Create(Belt, 0x870);
	Create(UserID, 0x880);
	Create(DisplayName, 0x898);
	Create(m__display_name, 0x898);
	Create(IsLocalPlayer, 0x249);
}