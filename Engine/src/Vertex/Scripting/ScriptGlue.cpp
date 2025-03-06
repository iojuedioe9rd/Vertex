#include <vxpch.h>

#include "ScriptGlue.h"

#include "mono/metadata/object.h"
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <Vertex/Core/Logger.h>
#include "../Scene/Entity.h"
//#include "../../VXEntities.h"
#include <Vertex/Renderer/Renderer2D.h>
#include "ScriptEngine.h"
#include <Vertex/Core/KeyCodes.h>
#include <Vertex/Core/Input.h>

#include "mono/metadata/reflection.h"
#include "box2d/b2_body.h"

#include "Vertex/Renderer/TextureManager.h"
#include "../Scene/Entities/Entities.h"
#include <Vertex/Audio/Audio.h>
#include <Vertex/ImGui/ImGuiLink.h>



namespace Vertex
{
	

#define VX_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Vertex.InternalCalls::" #Name, Name)

#define CORE_LOG_FLAG (1 << 0)    // 0b000001 = 1
#define LOG_TRACE     (1 << 1)    // 0b000010 = 2
#define LOG_INFO      (1 << 2)    // 0b000100 = 4
#define LOG_WARN      (1 << 3)    // 0b001000 = 8
#define LOG_ERROR     (1 << 4)    // 0b010000 = 16
#define LOG_CRITICAL  (1 << 5)    // 0b100000 = 32

	AssetHandle(*getAsset_funk)(std::filesystem::path);
	AssetManagerBase*(*getAssetMan_funk)();

	static bool ImGui_Begin(MonoString* name, bool* open)
	{
		char* nameCStr = mono_string_to_utf8(name);
		bool v = ImGuiLink::Begin(nameCStr, open);
		delete nameCStr;
		return v;
	}

#include <mono/metadata/object.h>
#include <imgui.h>
#include <vector>
#include <string>
#include <cstdarg>

	static void ImGui_Text(MonoArray* fmt)
	{
		if (!fmt) return;

		int length = mono_array_length(fmt);
		if (length == 0) return;

		// Extract the first string as the format
		MonoString* formatStr = mono_array_get(fmt, MonoString*, 0);
		if (!formatStr) return;

		const char* format = mono_string_to_utf8(formatStr);

		// Collect arguments
		std::vector<const char*> args;
		for (int i = 1; i < length; i++)
		{
			MonoString* monoStr = mono_array_get(fmt, MonoString*, i);
			if (!monoStr) continue;

			args.push_back(mono_string_to_utf8(monoStr));
		}

		// Allocate buffer for formatted string
		char buffer[512]; // Adjust size as needed
		int written = snprintf(buffer, sizeof(buffer), format, args.empty() ? nullptr : args[0], args.size() > 1 ? args[1] : "", args.size() > 2 ? args[2] : "");

		if (written > 0)
		{
			ImGui::Text("%s", buffer);
		}

		// Free allocated memory
		mono_free((void*)format);
		for (const char* arg : args)
		{
			mono_free((void*)arg);
		}
	}


	static void ImGui_End()
	{
		ImGuiLink::End();
	}


	static MonoString* Entity_FindEntityByName(MonoString* name)
	{
		char* nameCStr = mono_string_to_utf8(name);
		Scene* scene = ScriptEngine::GetSceneContext();
		VX_CORE_ASSERT(scene);
		Entity* entity = scene->FindEntityByName(nameCStr);
		mono_free(nameCStr);
		if (!entity)
			return mono_string_empty(ScriptEngine::GetAppDomain());
		char* id = entity->GetID().data();
		char* newID = new char[std::strlen(id) + 1];

		std::strcpy(newID, id);
		return mono_string_new(ScriptEngine::GetAppDomain(), newID);
	}

	static MonoObject* GetScriptInstance(MonoString* entityID)
	{
		char* entityIDCStr = mono_string_to_utf8(entityID);
		return ScriptEngine::GetManagedInstance(entityIDCStr);
	}

	static void Entity_Remove(MonoString* id)
	{
		char* entityIDCStr = mono_string_to_utf8(id);

		Scene* scene = ScriptEngine::GetSceneContext();
		VX_CORE_ASSERT(scene);
		Entity* entity = nullptr;

		for (Entity* ent : *scene)
		{

			if (ent->GetID() == std::string(entityIDCStr))
			{
				entity = ent;
				break;
			}
		}

		
		scene->RemoveEntity(*entity);

	}

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	// Texture2D_FromFilename

	void Texture2D_FromFilename(MonoString* string)
	{
		static Ref<Texture2D>* Wat = nullptr;
		const char* cStr = mono_string_to_utf8(string);
		
		if (Wat) delete Wat;
		Wat = &TextureManager2D::GetOrMakeTextureFromFilename(std::string(cStr));
	}

	// internal extern static void Renderer2D_DrawQuad(ref Vector3 pos, ref Vector3 size, string textureFilename, float tilingFactor = 1.0f, ref Colour tintColour = new Colour(1,1,1,1));
	void Renderer2D_DrawQuadTex(glm::vec3* pos, glm::vec3* size, MonoString* textureFilename, float tilingFactor, glm::vec4* tintColour)
	{
		

		const char* cStr = mono_string_to_utf8(textureFilename);

		AssetHandle Handle = getAsset_funk(cStr);

		Ref<Texture2D> tex = std::dynamic_pointer_cast<Texture2D>(getAssetMan_funk()->GetAsset(Handle));

		Renderer2D::DrawQuad(*pos, glm::vec2(size->x, size->y), tex, tilingFactor, *tintColour);
	}

	void Renderer2D_DrawQuad(glm::vec3* pos, glm::vec3* size, glm::vec4* colour)
	{
		Renderer2D::DrawQuad(*pos, glm::vec2(size->x, size->y), *colour);
	}

	std::string logBuffer;
	int logFlags;

	static void BeginLog(int flags)
	{
		logBuffer = std::string();
		logFlags = flags;
	}

	static void LogMsg(MonoString* string)
	{
		char* cStr = mono_string_to_utf8(string);
		logBuffer += cStr;
	}

	static void EndLog()
	{
		
		bool is_core_log = logFlags & CORE_LOG_FLAG;
		if (logFlags & LOG_TRACE)
		{
			if (is_core_log)
			{
				VX_CORE_TRACE("{}", logBuffer.c_str());
				return;
			}
			else
			{
				VX_TRACE("{}", logBuffer.c_str());
			}
		}
		if (logFlags & LOG_INFO)
		{
			if (is_core_log)
			{
				VX_CORE_INFO("{}", logBuffer.c_str());
				return;
			}
			else
			{
				VX_INFO("{}", logBuffer.c_str());
			}
		}
		if (logFlags & LOG_WARN)
		{
			if (is_core_log)
			{
				VX_CORE_WARN("{}", logBuffer.c_str());
				return;
			}
			else
			{
				VX_WARN("{}", logBuffer.c_str());
			}
		}
		if (logFlags & LOG_ERROR)
		{
			if (is_core_log)
			{
				VX_CORE_ERROR("{}", logBuffer.c_str());
				return;
			}
			else
			{
				VX_ERROR("{}", logBuffer.c_str());
			}
		}
		if (logFlags & LOG_CRITICAL)
		{
			if (is_core_log)
			{
				VX_CORE_CRITICAL("{}", logBuffer.c_str());
				return;
			}
			else
			{
				VX_CRITICAL("{}", logBuffer.c_str());
			}
		}

			
	}

	static void Entity_GetTranslation(MonoString* entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		VX_CORE_ASSERT(scene);
		Entity* entity = nullptr;
		char* cStr = mono_string_to_utf8(entityID);
		if (!cStr)
		{
			(*outTranslation).x = 1 << 0xDEAD1;
			(*outTranslation).y = 2 << 0xDEAD2;
			(*outTranslation).z = 3 << 0xDEAD3;
			return;
		}
		for (Entity* ent : *scene)
		{
			
			if (ent->GetID() == std::string(cStr))
			{
				entity = ent;
				break;
			}
		}
		if (!entity)
		{
			(*outTranslation).x = 1 << 0xDEAD1;
			(*outTranslation).y = 2 << 0xDEAD2;
			(*outTranslation).z = 3 << 0xDEAD3;
			return;
		}

		*outTranslation = entity->pos;
	}

	// internal extern static void Input_GetMousePos(ref Vector2 pos, bool isinPix);

	static void Input_GetMousePos(glm::vec2* pos, bool isinPix)
	{
		(*pos) = Input::GetMousePositionVec2();

		if (!isinPix)
		{
			Ref<Camera> mainCamera{};
			if (ScriptEngine::GetSceneContext()->GetACameraInScene(&mainCamera, true))
			{
				// Assuming mainCamera has a method GetViewProjectionMatrix()
				glm::mat4 invVP = glm::inverse(mainCamera->GetProjection());

				// Get normalized device coordinates (NDC)
				glm::vec2 windowSize = Application::GetWindowSize(); // Replace with actual function
				glm::vec4 ndc = {
					(2.0f * (*pos).x) / windowSize.x - 1.0f,
					1.0f - (2.0f * (*pos).y) / windowSize.y,
					0.0f,  // Assuming Z = 0 for now
					1.0f
				};

				// Transform to world-space
				glm::vec4 worldPos = invVP * ndc;
				*pos = glm::vec2(worldPos.x / worldPos.w, worldPos.y / worldPos.w);
			}
		}
	}

	static MonoString* Object_GenerateUUID()
	{
		MonoString* str = mono_string_new(ScriptEngine::GetAppDomain(), get_uuid().data());
		return str;
	}

	//[MethodImplAttribute(MethodImplOptions.InternalCall)]
	//	internal extern static UInt64 Audio_CreateSound(string filename, bool looped);

	uint64_t Audio_CreateSound(MonoString* filename, bool looped)
	{
		const char* cStr = mono_string_to_utf8(filename);
		AssetHandle Handle = getAsset_funk(cStr);
		//return AudioManager::GetAudioFromFileName(std::filesystem::path(cStr), looped)->GetID();
		return Handle;
	}

	//[MethodImplAttribute(MethodImplOptions.InternalCall)]
	//	internal extern static void Audio_PlaySound(UInt64 soundID);
	void Audio_PlaySound(uint64_t soundID)
	{
		AssetHandle Handle = soundID;
		Ref<Audio> audio = std::dynamic_pointer_cast<Audio>(getAssetMan_funk()->GetAsset(Handle));
		audio->Play();
		//return AudioManager::GetAudioFromFileName(std::filesystem::path(cStr), looped)->GetID();
		
	}

	//[MethodImplAttribute(MethodImplOptions.InternalCall)]
	//	internal extern static void Audio_StopSound(UInt64 soundID);

	void Audio_StopSound(uint64_t soundID)
	{
		AssetHandle Handle = soundID;
		Ref<Audio> audio = std::dynamic_pointer_cast<Audio>(getAssetMan_funk()->GetAsset(Handle));
		audio->Stop();
		//return AudioManager::GetAudioFromFileName(std::filesystem::path(cStr), looped)->GetID();

	}

	//[MethodImplAttribute(MethodImplOptions.InternalCall)]
	//	internal extern static void Audio_SetSoundVolume(UInt64 soundID, float volume);
	void Audio_SetSoundVolume(uint64_t soundID, float volume)
	{
		AssetHandle Handle = soundID;
		Ref<Audio> audio = std::dynamic_pointer_cast<Audio>(getAssetMan_funk()->GetAsset(Handle));
		//audio->SetVolume(volume);
		//return AudioManager::GetAudioFromFileName(std::filesystem::path(cStr), looped)->GetID();

	}

	//[MethodImplAttribute(MethodImplOptions.InternalCall)]
	//	internal extern static void Audio_SetSoundLooped(UInt64 soundID, bool looped);
	void Audio_SetSoundLooped(uint64_t soundID, bool looped)
	{
		AssetHandle Handle = soundID;
		Ref<Audio> audio = std::dynamic_pointer_cast<Audio>(getAssetMan_funk()->GetAsset(Handle));
		//audio->SetLooped(looped);
		//return AudioManager::GetAudioFromFileName(std::filesystem::path(cStr), looped)->GetID();

	}

	//[MethodImplAttribute(MethodImplOptions.InternalCall)]
	//	internal extern static void Audio_SetSoundPitch(UInt64 soundID, float pitch);
	void Audio_SetSoundPitch(uint64_t soundID, float pitch)
	{
		AssetHandle Handle = soundID;
		Ref<Audio> audio = std::dynamic_pointer_cast<Audio>(getAssetMan_funk()->GetAsset(Handle));
		//audio->SetPitch(pitch);
		//return AudioManager::GetAudioFromFileName(std::filesystem::path(cStr), looped)->GetID();

	}


	static uint64_t Object_GenerateIntUUID()
	{
		IntUUID uuid = IntUUID();
		return uuid;
	}

	static void Rigidbody2D_ApplyLinearImpulse(b2Body** body, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		(*body)->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void Rigidbody2D_ApplyLinearImpulseToCenter(b2Body** body, glm::vec2* impulse, bool wake)
	{
		(*body)->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static void Rigidbody2D_GetTransform(b2Body** body, glm::vec4* transform2D)
	{
		const auto& position = (*body)->GetPosition();
		
		transform2D->x = position.x;
		transform2D->y = position.y;
		transform2D->w = glm::degrees((*body)->GetAngle());
	}

	static void Rigidbody2D_GetVelocity(b2Body** body, glm::vec4* transform2D)
	{
		const auto& position = (*body)->GetLinearVelocity();

		transform2D->x = position.x;
		transform2D->y = position.y;
		transform2D->w = glm::degrees((*body)->GetAngle());
	}

	static void Rigidbody2D_SetTransform(b2Body** body, glm::vec4* transform2D)
	{
		(*body)->SetTransform(b2Vec2(transform2D->x, transform2D->y), glm::radians(transform2D->w));
	}

	


	static void Entity_SetTranslation(MonoString* entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		VX_CORE_ASSERT(scene);
		Entity* entity = nullptr;
		for (Entity* ent : *scene)
		{
			char* cStr = mono_string_to_utf8(entityID);
			if (ent->GetID() == std::string(cStr))
			{
				entity = ent;
				break;
			}
		}
		if (entity == nullptr) return;
		entity->pos = *translation;
	}

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);
		std::cout << str << ", " << parameter << std::endl;
	}
	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		glm::vec3 NewParameter = *parameter;
		VX_CORE_WARN("Value: {0}", NewParameter);
		*outResult = glm::normalize(NewParameter);
	}
	static float NativeLog_VectorDot(glm::vec3* parameter)
	{
		glm::vec3 NewParameter = *parameter;
		VX_CORE_WARN("Value: {0}", NewParameter);
		return glm::dot(NewParameter, NewParameter);
	}

	// internal extern static bool Input_IsMouseButtonPressed(MouseCode button);
	bool Input_IsMouseButtonPressed(MouseCode button)
	{
		return Input::IsMouseButtonPressed(button);
	}

	
	// internal extern static string Entity_Instantiate(string typename, string name, ref Vector3 translation, ref Vector3 size, ref Vector3 rotation);
	static MonoString* Entity_Instantiate(MonoString* typenameMono, MonoString* nameMono, glm::vec3* translation, glm::vec3* size, glm::vec3* rotation)
	{
		char* typenameCstr = mono_string_to_utf8(typenameMono);
		char* nameCstr = mono_string_to_utf8(nameMono);

		Scene* scene = ScriptEngine::GetSceneContext();
		VX_CORE_ASSERT(scene);

		Entity* ent = scene->CreateEntity("env_script", nameCstr);
		VX_CORE_ASSERT(ent && dynamic_cast<ENTEnvScript*>(ent));

		ENTEnvScript* script = (ENTEnvScript*)ent;
		script->pos = *translation;
		script->size = *size;
		script->rotation = *rotation;
		script->classname = typenameCstr;

		scene->Add_ENT_To_Mono(script);

		MonoString* str = mono_string_new(ScriptEngine::GetAppDomain(), script->GetID().c_str());
		return str;
	}
	
	template<typename... ENT>
	static void RegisterENT()
	{
		([]()
			{
				std::string_view typeName = typeid(ENT).name();
				size_t pos = typeName.find_last_of(':');
				std::string_view structName = typeName.substr(pos + 1);
				std::string managedTypename = fmt::format("Vertex.{}", structName);
				MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
				if (!managedType)
				{
					VX_CORE_ERROR("	Could not find ENT type {}", managedTypename);
					return;
				}
				
			}(), ...);
	}
	template<typename... ENT>
	static void RegisterENT(EntityGroup<ENT...>)
	{
		VX_CORE_INFO("Registering Entities");
		RegisterENT<ENT...>();
		VX_CORE_INFO("~Registering Entities");
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterENT(AllEntities{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		VX_ADD_INTERNAL_CALL(NativeLog);
		VX_ADD_INTERNAL_CALL(NativeLog_Vector);
		VX_ADD_INTERNAL_CALL(NativeLog_VectorDot);

		VX_ADD_INTERNAL_CALL(BeginLog);
		VX_ADD_INTERNAL_CALL(LogMsg);
		VX_ADD_INTERNAL_CALL(EndLog);

		VX_ADD_INTERNAL_CALL(Renderer2D_DrawQuad);
		VX_ADD_INTERNAL_CALL(Texture2D_FromFilename);
		VX_ADD_INTERNAL_CALL(Renderer2D_DrawQuadTex);

		VX_ADD_INTERNAL_CALL(GetScriptInstance);

		VX_ADD_INTERNAL_CALL(Entity_GetTranslation);
		VX_ADD_INTERNAL_CALL(Entity_SetTranslation);
		VX_ADD_INTERNAL_CALL(Entity_FindEntityByName);
		VX_ADD_INTERNAL_CALL(Entity_Instantiate);
		VX_ADD_INTERNAL_CALL(Entity_Remove);

		VX_ADD_INTERNAL_CALL(Input_IsKeyDown);
		VX_ADD_INTERNAL_CALL(Input_GetMousePos);
		VX_ADD_INTERNAL_CALL(Input_IsMouseButtonPressed);

		VX_ADD_INTERNAL_CALL(Rigidbody2D_ApplyLinearImpulse);
		VX_ADD_INTERNAL_CALL(Rigidbody2D_ApplyLinearImpulseToCenter);
		VX_ADD_INTERNAL_CALL(Rigidbody2D_GetTransform);
		VX_ADD_INTERNAL_CALL(Rigidbody2D_SetTransform);
		VX_ADD_INTERNAL_CALL(Rigidbody2D_GetVelocity);

		VX_ADD_INTERNAL_CALL(Object_GenerateUUID);
		VX_ADD_INTERNAL_CALL(Object_GenerateIntUUID);

		VX_ADD_INTERNAL_CALL(Audio_CreateSound);
		VX_ADD_INTERNAL_CALL(Audio_PlaySound);
		VX_ADD_INTERNAL_CALL(Audio_StopSound);
		VX_ADD_INTERNAL_CALL(Audio_SetSoundVolume);
		VX_ADD_INTERNAL_CALL(Audio_SetSoundLooped);
		VX_ADD_INTERNAL_CALL(Audio_SetSoundPitch);

		VX_ADD_INTERNAL_CALL(ImGui_Begin);
		VX_ADD_INTERNAL_CALL(ImGui_End);
		VX_ADD_INTERNAL_CALL(ImGui_Text);
	}

	void ScriptGlue::SetupGetingAssets(AssetHandle(*getAsset)(std::filesystem::path), AssetManagerBase*(*getAssetMan)())
	{
		getAsset_funk = getAsset;
		getAssetMan_funk = getAssetMan;
	}

}