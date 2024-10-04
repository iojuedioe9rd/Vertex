#pragma once
#include "../base_npc/base_npc.h"
#include "../info_player/info_player.h"
#include "Vertex/Audio/AudioManager.h"
#include <random>
#include "../json_static_tilemap/json_static_tilemap.h"

namespace Vertex {
	class ENTNpcEnemy : public ENTBaseNPC
	{
	public:
		ENTNpcEnemy()
		{

		}
		ENTNpcEnemy(std::string name, Scene* scene) : ENTBaseNPC(name, scene)
		{
			m_Audio = AudioManager::GetAudioFromFileName("assets/music/Tormentor.wav");
			m_Audio2 = AudioManager::GetAudioFromFileName("assets/music/Tormentor 01.wav");
			Init(15, 25);
		}
		~ENTNpcEnemy();

		virtual void Update(Timestep& ts) override;
		virtual void Draw(Timestep& ts)   override;
		float t = 0.0f;

		float maxTime = 5.0f;
		float initMaxTime = 5.0f;
		virtual std::string GetEntName() override
		{
			return "npc_enemy";
		}

		ENTJsonStaticTilemap* m_tilemap;
		void Setup(ENTInfoPlayer* player, Ref<Texture2D> tex, ENTJsonStaticTilemap* m_tilemap, int seed = 0)
		{
			// Seed the RNG with the current time
			std::mt19937 rng(static_cast<int>(seed));

			// Generate a random number
			std::uniform_int_distribution<int> dist(5, 25);
			maxTime = dist(rng) ;
			initMaxTime = dist(rng);


			m_player = player;
			m_tex = tex;
			this->m_tilemap = m_tilemap;
			InIt(m_tilemap->GetAllWalls());
		}
		virtual void ImGuiRender() override {};
		
		virtual void OnDeath() override;
	private:
		
		Ref<Texture2D> m_tex;
		Audio* m_Audio;
		Audio* m_Audio2;
		ENTInfoPlayer* m_player = nullptr;
	};

	
}