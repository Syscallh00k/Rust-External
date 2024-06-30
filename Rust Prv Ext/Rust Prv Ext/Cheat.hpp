#include "sdk.hpp"
#include "skcrypt.hpp"
#include <memory>

uint64_t GameAssembly;
uint64_t UnityPlayer;
uint64_t RustClient;
Matrix4x4 M;
Vector3 LpPos;
Vector2 Base, Head;
std::vector<Rust::BasePlayer*> Player;

class Cheat {
public:
	void __fastcall Do_Cheat()
	{
		SleepEx(1, false);
		uint64_t CamComponent = driver.read_chain<uint64_t>(GameAssembly, { 63001856 ,0xB8,0x00,0x10 });
		M = driver.read<Matrix4x4>(CamComponent + 0x30c);


		uint64_t BaseNetworkable = driver.read<uint64_t>(GameAssembly + Offset::m_System_Collections_Generic_List_BaseGameMode_TypeInfo);

		uint64_t Chain = driver.read_chain<uint64_t>(BaseNetworkable, { 0xB8,0x10,0x10,0x28 });

		uint64_t Array = driver.read<uint64_t>(Chain + 0x18);

		int Size = driver.read<int>(Chain + 0x10);

		for (int i = 0; i <= Size; i++)
		{
			uint64_t Actor = driver.read<uint64_t>(Array + (0x20 + (i * 8)));
			if (!Actor) continue;

			uint64_t PrefabStr = driver.read_chain<uint64_t>(Actor, { 0x10,0x30,0x60 });

			WORD Tag = driver.read_chain<WORD>(Actor, { 0x10,0x30,0x54 });

			uint64_t Object = driver.read_chain<uint64_t>(Actor, { 0x10,0x30 });

			std::string Prefab = driver.read_str(PrefabStr);
			if (!Actor || !PrefabStr || !Object) continue;

			if (Prefab.find(skCrypt("player.prefab").decrypt()) != std::string::npos ||
				Prefab.find(skCrypt("scientistnpc").decrypt()) != std::string::npos
				|| Prefab.find(skCrypt("LocalPlayer").decrypt()) != std::string::npos) // All Entitys
			{
				uint64_t Class = driver.read<uint64_t>(Object + 0x30);
				uint64_t Entity = driver.read<uint64_t>(Class + 0x18);
				uint64_t Trans = driver.read<uint64_t>(Class + 0x8);
				if (!Class || !Entity || !Trans) continue;

				std::unique_ptr<Rust::BasePlayer> MainEntity = std::make_unique<Rust::BasePlayer>(Entity, Trans);
				
				if (Prefab.find(skCrypt("LocalPlayer").decrypt()) != std::string::npos) // LocalPlayer
				{
					LpPos = MainEntity->AGetBonePosition(47);
				}
				else {
					float Distance = LpPos.DistTo(MainEntity->Trans->Get_Position());

					if (Distance >= 500) continue;

					if (LpPos.isEqual(MainEntity->Trans->Get_Position())) continue;

					Vector3 Head = MainEntity->AGetBonePosition(BoneList::head);

					Vector2 HeadPos;



					if (W22S(MainEntity->Trans->Get_Position(), Base, M) && W22S(Head, HeadPos, M))
					{
						float height = HeadPos.y - Base.y - 15;
						float width = height / 1.0;

						if (ESP::Skeleton)
						{
							std::array<BoneList, 15> Bones = {
							l_foot, l_knee, l_hip,
							r_foot, r_knee, r_hip,
							neck, spine3, head,
							l_upperarm, l_forearm, l_hand,
							r_upperarm, r_forearm, r_hand
							};
							std::array<Vector2, Bones.size()> BonesPos;

							for (int i = 0; i < BonesPos.size(); ++i) {
								if (!(W22S(MainEntity->AGetBonePosition(Bones[i]), BonesPos.at(i), M))) continue;
								if (BonesPos.at(1).Empty()) continue;
							}


							for (int j = 0; j < BonesPos.size(); j += 3) {
								ImGui::GetBackgroundDrawList()->AddLine(ImVec2{ BonesPos[j].x, BonesPos[j].y }, ImVec2{ BonesPos[j + 1].x, BonesPos[j + 1].y }, ImColor(255, 255, 255, 255), 1.0f);
								ImGui::GetBackgroundDrawList()->AddLine(ImVec2{ BonesPos[j + 1].x, BonesPos[j + 1].y }, ImVec2{ BonesPos[j + 2].x, BonesPos[j + 2].y }, ImColor(255, 255, 255, 255), 1.0f);

							
							}


							ImGui::GetBackgroundDrawList()->AddLine(ImVec2{ BonesPos[2].x, BonesPos[2].y }, ImVec2{ BonesPos[6].x, BonesPos[6].y }, ImColor(255, 255, 255, 255), 1.0f);
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2{ BonesPos[5].x, BonesPos[5].y }, ImVec2{ BonesPos[6].x, BonesPos[6].y }, ImColor(255, 255, 255, 255), 1.0f);
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2{ BonesPos[9].x, BonesPos[9].y }, ImVec2{ BonesPos[7].x, BonesPos[7].y }, ImColor(255, 255, 255, 255), 1.0f);
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2{ BonesPos[12].x, BonesPos[12].y }, ImVec2{ BonesPos[7].x, BonesPos[7].y }, ImColor(255, 255, 255, 255), 1.0f);
						}
						if (ESP::CornerBox)
						{
							DrawCorneredBox(Base.x - (height / 4), Base.y, height / 2, height, ImColor(ESP::BoxColor[0], ESP::BoxColor[1], ESP::BoxColor[2],255.f), .7f);

						}
						if (ESP::FilledBox)
						{
							DrawFilledRect(Base.x - (height / 4), Base.y, height / 2, height, ImColor(0.f,0.f,0.f, 0.5f));
						}
						if (ESP::box)
						{
							DrawBox(Base.x - (height / 4), Base.y, height / 2 , height , ImColor(0, 0, 0, 255), 2.0f); //black outline
							DrawBox(Base.x - (height / 4), Base.y, height / 2, height, ImColor(ESP::BoxColor[0], ESP::BoxColor[1], ESP::BoxColor[2], 255.f), 1.0f);
						}
						if (ESP::Distance)
						{
							std::string Text = std::to_string(int(Distance)) + "M";
							ImVec2 Size = ImGui::CalcTextSize(Text.c_str());
							draw_text_outline_font(Base.x - (Size.x / 2), Base.y - (Size.y / 2) + 10, ImColor(255, 255, 255, 255), Text.c_str());
						}
						if (ESP::Weapon)
						{
							std::string Text = MainEntity->getHeldItem().GetItemName();
							ImVec2 Size = ImGui::CalcTextSize(Text.c_str());
						
							if(ESP::Distance)
								draw_text_outline_font(Base.x - (Size.x / 2), Base.y - (Size.y / 2) + 25, ImColor(255, 255, 255, 255), Text.c_str());
							else
								draw_text_outline_font(Base.x - (Size.x / 2), Base.y - (Size.y / 2) + 10, ImColor(255, 255, 255, 255), Text.c_str());
						}
						if (ESP::Name)
						{
							std::string Text = getASCIIName(MainEntity->getPlayerName());
							ImVec2 Size = ImGui::CalcTextSize(Text.c_str());
							draw_text_outline_font(height - (Size.x / 2), height - (Size.y / 2) - 20, ImColor(255, 255, 255, 255), Text.c_str());
						}
						if (ESP::SnapLines)
						{
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(GetSystemMetrics(0) / 2 + .2, GetSystemMetrics(1) / 2 + .2),
								ImVec2(Base.x + .2, Base.y + .2),
								ImColor(0, 0, 0, 255));
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(GetSystemMetrics(0) / 2 - 1, GetSystemMetrics(1) / 2 - .2),
								ImVec2(Base.x - .2, Base.y - .2),
								ImColor(0, 0, 0, 255));
							ImGui::GetBackgroundDrawList()->AddLine(ImVec2(GetSystemMetrics(0) / 2, GetSystemMetrics(1) / 2),
								ImVec2(Base.x, Base.y),
								ImColor(255, 255, 255, 255));
						}
					}
				}
						
			}
		}
	}
	

};