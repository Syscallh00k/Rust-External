#pragma once
#include "offsets.hpp"
#include "vectorMath.hpp"
#include "draw.hpp"


std::string getASCIIName(std::wstring name) {
	return std::string(name.begin(), name.end());
}

class Rust {
public:
	class Address {
	public:
		uint64_t Return() {
			return (uint64_t)this;
		}
	};
	class PlayerModel : public Address {
	public:
		Vector3 Get_Position() {
			if (!this->Return())
				return { 0,0,0 };

			return driver.read<Vector3>(this->Return() + Offset::m_Position);
		}
	};
	class Model {
	public:

	};
	class BaseCollision {
	public:

	};
	class CameraMode {
	public:

	};
	class PlayerEyes {
	public:

	};
	class HeldItem
	{
	public:
		HeldItem(uintptr_t Ent)
		{
			this->ent = Ent;
		}
		std::string GetItemName()
		{
			return driver.read_str(this->ent + 0x58);
		}
		uint64_t ent;
		int WeaponID;
	};
	
	class BasePlayer : public Address {
	public:
		BasePlayer(uint64_t Entity, uint64_t Trans)
		{
			this->Player = driver.read<uint64_t>(Entity + 0x28);
			this->Trans = driver.read<PlayerModel*>(Trans + 0x38);
		}
		
		_forceinline Vector3 __fastcall AGetBonePosition(int bone)
		{
			uintptr_t playerModel = driver.read<uintptr_t>(this->Player + 0xC8); //BaseEntity -> public Model model;
			uintptr_t boneTransforms = driver.read<uintptr_t>(playerModel + 0x50); //public class Model -> public Transform[] boneTransforms;
			uintptr_t entityBone = driver.read<uintptr_t>(boneTransforms + (0x28 + (bone * 0x8))); // 0x20 = public Transform rootBone;
			return GetPosition(driver.read<uintptr_t>(entityBone + 0x10));
		}
		std::wstring getPlayerName() {

			std::wstring name = driver.read_wstr(driver.read<uint64_t>(this->Player + Offset::m_m__display_name) + 0x14); //BasePlayer -> protected string _displayName

			return name;
		}
		BaseCollision* Collision() {
			return driver.read<Rust::BaseCollision*>(this->Return() + Offset::m_Collision);

		}
		CameraMode* Camera() {
			return driver.read<Rust::CameraMode*>(this->Return() + Offset::m_CameraMode);

		}
		Vector3 Position;
		uintptr_t Player;
		PlayerModel* Trans;
		uintptr_t Object;
		HeldItem getHeldItem()
		{
			int active_weapon_id = driver.read<int>(this->Player + Offset::m_clActiveItem); // public struct ItemId--private uint clActiveItem;
			uint64_t items = driver.read_chain<uint64_t>(this->Player, { (uint64_t)0x848, (uint64_t)0x30, (uint64_t)0x48,(uint64_t)0x28}); //public PlayerInventory inventory;

			//if (Settings::debuglog)

			for (int items_on_belt = 0; items_on_belt <= 6; items_on_belt++)
			{
				uint64_t item = driver.read<uint64_t>(items + 0x20 + (items_on_belt * 0x8));

				int active_weapon = driver.read<uint32_t>(item + 0x18);

				if (active_weapon_id == active_weapon)
				{
					HeldItem item_obj = HeldItem(item);
					std::cout << "active_weapon_id: " << active_weapon_id << std::endl;
					
					return item_obj;
				}
			}

		}
		
	
	};
};

