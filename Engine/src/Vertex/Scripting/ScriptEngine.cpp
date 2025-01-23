#include <vxpch.h>

#include "ScriptEngine.h"

#include "Vertex/Core/Base.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
#include "mono/metadata/tabledefs.h"
#include "ScriptGlue.h"

#include "Vertex/Core/Buffer.h"
#include "Vertex/Core/FileSystem.h"


#include "Vertex/Core/Application.h"
#include "Vertex/Scene/Entities/Entities.h"

#include <fstream>

char Vertex::ScriptInstance::s_FieldValueBuffer[32];

namespace Vertex {

	static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
	{
		{ "System.Single", ScriptFieldType::Float },
		{ "System.Double", ScriptFieldType::Double },
		{ "System.Boolean", ScriptFieldType::Bool },
		{ "System.Char", ScriptFieldType::Char },
		{ "System.Int16", ScriptFieldType::Short },
		{ "System.Int32", ScriptFieldType::Int },
		{ "System.Int64", ScriptFieldType::Long },
		{ "System.Byte", ScriptFieldType::Byte },
		{ "System.UInt16", ScriptFieldType::UShort },
		{ "System.UInt32", ScriptFieldType::UInt },
		{ "System.UInt64", ScriptFieldType::ULong },

		{ "Vetex.Vector2", ScriptFieldType::Vector2 },
		{ "Vetex.Vector3", ScriptFieldType::Vector3 },
		{ "Vetex.Vector4", ScriptFieldType::Vector4 },
		{ "Vetex.Colour", ScriptFieldType::Vector4 },
		{ "Vetex.Entity", ScriptFieldType::Entity },
	};

	namespace Utils {

		ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
		{
			std::string typeName = mono_type_get_name(monoType);

			auto it = s_ScriptFieldTypeMap.find(typeName);
			if (it == s_ScriptFieldTypeMap.end())
			{
				VX_CORE_ERROR("Unknown type: {}", typeName);
				return ScriptFieldType::None;
			}

			return it->second;
		}

		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath, bool loadPDB = false)
		{
			if (!std::filesystem::exists(assemblyPath))
			{
				std::string msg = "Assembly file does not exist: " + assemblyPath.string();
				VX_CORE_ASSERT(false, msg.c_str());
				return nullptr;
			}

			

			ScopedBuffer fileData = FileSystem::ReadFileBinary(assemblyPath);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData.As<char>(), fileData.Size(), 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				std::string msg = std::string("Error loading image: ") + std::string(errorMessage);
				VX_CORE_ASSERT(false, msg.c_str());
				// Log some error message using the errorMessage data
				return nullptr;
			}

			if (loadPDB)
			{
				std::filesystem::path pdbPath = assemblyPath;
				pdbPath.replace_extension(".pdb");
				if (std::filesystem::exists(pdbPath))
				{
					ScopedBuffer pdbFileData = FileSystem::ReadFileBinary(assemblyPath);
					mono_debug_open_image_from_memory(image, (const mono_byte*)pdbFileData.As<char>(), pdbFileData.Size());
					VX_CORE_INFO("Loaded PDB {}", pdbPath.string().c_str());
				}
			}

			std::string pathString = assemblyPath.string();
			MonoAssembly* assembly = mono_assembly_load_from_full(image, pathString.c_str(), &status, 0);
			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				std::string msg = std::string("Error loading assembly: ") + std::string(errorMessage);
				VX_CORE_ASSERT(false, msg.c_str());
				// Log some error message using the errorMessage data
				return nullptr;
			}
			mono_image_close(image);

			return assembly;
		}

		void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
				VX_CORE_TRACE("{}.{}", nameSpace, name);
			}
		}

		ScriptFieldType ScriptFieldTypeFromString(std::string_view fieldType)
		{
			if (fieldType == "None")    return ScriptFieldType::None;
			if (fieldType == "Float")   return ScriptFieldType::Float;
			if (fieldType == "Double")  return ScriptFieldType::Double;
			if (fieldType == "Bool")    return ScriptFieldType::Bool;
			if (fieldType == "Char")    return ScriptFieldType::Char;
			if (fieldType == "Byte")    return ScriptFieldType::Byte;
			if (fieldType == "Short")   return ScriptFieldType::Short;
			if (fieldType == "Int")     return ScriptFieldType::Int;
			if (fieldType == "Long")    return ScriptFieldType::Long;
			if (fieldType == "UByte")   return ScriptFieldType::UByte;
			if (fieldType == "UShort")  return ScriptFieldType::UShort;
			if (fieldType == "UInt")    return ScriptFieldType::UInt;
			if (fieldType == "ULong")   return ScriptFieldType::ULong;
			if (fieldType == "Vector2") return ScriptFieldType::Vector2;
			if (fieldType == "Vector3") return ScriptFieldType::Vector3;
			if (fieldType == "Colour") return ScriptFieldType::Colour;
			if (fieldType == "Vector4") return ScriptFieldType::Vector4;
			if (fieldType == "Entity")  return ScriptFieldType::Entity;
			if (fieldType == "CppEntity")  return ScriptFieldType::CppEntity;
			VX_CORE_ASSERT(false, "Unknown ScriptFieldType");
			return ScriptFieldType::None;
		}

		const char* ScriptFieldTypeToString(ScriptFieldType type)
		{
			switch (type)
			{
			case ScriptFieldType::Float:   return "Float";
			case ScriptFieldType::Double:  return "Double";
			case ScriptFieldType::Bool:    return "Bool";
			case ScriptFieldType::Char:    return "Char";
			case ScriptFieldType::Byte:    return "Byte";
			case ScriptFieldType::Short:   return "Short";
			case ScriptFieldType::Int:     return "Int";
			case ScriptFieldType::Long:    return "Long";
			case ScriptFieldType::UByte:   return "UByte";
			case ScriptFieldType::UShort:  return "UShort";
			case ScriptFieldType::UInt:    return "UInt";
			case ScriptFieldType::ULong:   return "ULong";
			case ScriptFieldType::Vector2: return "Vector2";
			case ScriptFieldType::Vector3: return "Vector3";
			case ScriptFieldType::Vector4: return "Vector4";
			case ScriptFieldType::Colour: return "Colour";
			case ScriptFieldType::Entity:  return "Entity";
			case ScriptFieldType::CppEntity:  return "CppEntity";
			}

			return "<Invalid>";
		}

}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		std::filesystem::path CoreAssemblyFilepath;
		std::filesystem::path AppAssemblyFilepath;

		ScriptClass EntityClass;

		bool EnableDebugging = true;

		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;
		std::unordered_map<UUID, ScriptFieldMap> EntityScriptFields;

		// Runtime
		Scene* SceneContext = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;

	

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();

#ifdef VX_DIST
		s_Data->EnableDebugging = false;
#endif // VX_DIST


		InitMono();
		LoadAssembly("Resources/Scripts/Vertex-ScriptCore.dll");
		LoadAppAssembly("Sandbox/Assets/Scripts/Binaries/Sandbox.dll");
		LoadAssemblyClasses();

		ScriptGlue::RegisterComponents();
		ScriptGlue::RegisterFunctions();

		// Retrieve and instantiate class
		s_Data->EntityClass = ScriptClass("Vertex", "Entity", true);
#if 0

		MonoObject* instance = s_Data->EntityClass.Instantiate();

		// Call method
		MonoMethod* printMessageFunc = s_Data->EntityClass.GetMethod("PrintMessage", 0);
		s_Data->EntityClass.InvokeMethod(instance, printMessageFunc);

		// Call method with param
		MonoMethod* printIntFunc = s_Data->EntityClass.GetMethod("PrintInt", 1);

		int value = 5;
		void* param = &value;

		s_Data->EntityClass.InvokeMethod(instance, printIntFunc, &param);

		MonoMethod* printIntsFunc = s_Data->EntityClass.GetMethod("PrintInts", 2);
		int value2 = 508;
		void* params[2] =
		{
			&value,
			&value2
		};
		s_Data->EntityClass.InvokeMethod(instance, printIntsFunc, params);

		MonoString* monoString = mono_string_new(s_Data->AppDomain, "Hello World from C++!");
		MonoMethod* printCustomMessageFunc = s_Data->EntityClass.GetMethod("PrintCustomMessage", 1);
		void* stringParam = monoString;
		s_Data->EntityClass.InvokeMethod(instance, printCustomMessageFunc, &stringParam);

		VX_CORE_ASSERT(false);
#endif
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("assets/mono/lib");

		if (s_Data->EnableDebugging)
		{
			const char* argv[2] = {
				"--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n,loglevel=3,logfile=MonoDebugger.log",
				"--soft-breakpoints"
			};
			mono_jit_parse_options(2, (char**)argv);
			mono_debug_init(MONO_DEBUG_FORMAT_MONO);
		}

		MonoDomain* rootDomain = mono_jit_init("VertexJITRuntime");
		VX_CORE_ASSERT(rootDomain);

		// Store the root domain pointer
		s_Data->RootDomain = rootDomain;

		if (s_Data->EnableDebugging)
			mono_debug_domain_create(s_Data->RootDomain);
		mono_thread_set_main(mono_thread_current());

		
	}

	void ScriptEngine::ShutdownMono()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;

		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}

	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		// Move this maybe
		s_Data->AppAssemblyFilepath = filepath;
		s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath, s_Data->EnableDebugging);
		auto assemb = s_Data->AppAssembly;
		s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);
		auto assembi = s_Data->AppAssemblyImage;
		// Utils::PrintAssemblyTypes(s_Data->AppAssembly);

		
	}

	void ScriptEngine::ReloadAssembly()
	{
		mono_domain_set(mono_get_root_domain(), false);
		mono_domain_unload(s_Data->AppDomain);
		LoadAssembly(s_Data->CoreAssemblyFilepath);
		LoadAppAssembly(s_Data->AppAssemblyFilepath);
		LoadAssemblyClasses();

		ScriptGlue::RegisterComponents();

		// Retrieve and instantiate class
		s_Data->EntityClass = ScriptClass("Vertex", "Entity", true);
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		// Create an App Domain
		s_Data->AppDomain = mono_domain_create_appdomain("VertexScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		// Move this maybe
		s_Data->CoreAssemblyFilepath = filepath;
		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath, s_Data->EnableDebugging);
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		// Utils::PrintAssemblyTypes(s_Data->CoreAssembly);
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_Data->SceneContext = scene;
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullClassName)
	{
		return s_Data->EntityClasses.find(fullClassName) != s_Data->EntityClasses.end();
	}

	void ScriptEngine::OnCreateEntity(Entity* entity, std::function<bool(ENTEnvScript*)> func)
	{
		
		if (auto sc = dynamic_cast<ENTEnvScript*>(entity))
		{
			if (ScriptEngine::EntityClassExists(sc->classname))
			{
				UUID entityID = entity->GetID();

				Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_Data->EntityClasses[sc->classname], entity);
				s_Data->EntityInstances[entityID] = instance;
				// Copy field values
				if (s_Data->EntityScriptFields.find(entityID) != s_Data->EntityScriptFields.end())
				{
					const ScriptFieldMap& fieldMap = s_Data->EntityScriptFields.at(entityID);
					for (const auto& [name, fieldInstance] : fieldMap)
					{
						

						instance->SetFieldValueInternal(name, fieldInstance.m_Buffer);
					}
						
				}


				VX_CORE_ASSERT(func(sc));
				instance->InvokeOnCreate();
			}
		}
		
	}

	void ScriptEngine::OnUpdateEntity(Entity* entity, Timestep ts)
	{
		UUID entityUUID = entity->GetID();
		VX_CORE_ASSERT(s_Data->EntityInstances.find(entityUUID) != s_Data->EntityInstances.end());

		Ref<ScriptInstance> instance = s_Data->EntityInstances[entityUUID];
		instance->InvokeOnUpdate((float)ts);
	}

	void ScriptEngine::OnPhysUpdateEntity(Entity* entity, Timestep ts)
	{
		UUID entityUUID = entity->GetID();
		VX_CORE_ASSERT(s_Data->EntityInstances.find(entityUUID) != s_Data->EntityInstances.end());

		Ref<ScriptInstance> instance = s_Data->EntityInstances[entityUUID];
		instance->InvokeOnPhysUpdate((float)ts);
	}

	void ScriptEngine::OnDrawEntity(Entity* entity)
	{
		UUID entityUUID = entity->GetID();
		VX_CORE_ASSERT(s_Data->EntityInstances.find(entityUUID) != s_Data->EntityInstances.end());

		Ref<ScriptInstance> instance = s_Data->EntityInstances[entityUUID];
		instance->InvokeOnDraw();
	}

	MonoClass* ScriptEngine::GetMonoClassFromName(MonoImage* image, std::string nameSpace, std::string name)
	{
		return mono_class_from_name(image, nameSpace.c_str(), name.c_str());
	}

	bool ScriptEngine::IsSubclassOf(MonoClass* monoClass, MonoClass* subclass, bool checkInterfaces)
	{
		return mono_class_is_subclass_of(monoClass, subclass, checkInterfaces);
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(UUID entityID)
	{
		auto it = s_Data->EntityInstances.find(entityID);
		if (it == s_Data->EntityInstances.end())
			return nullptr;

		return it->second;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_Data->SceneContext = nullptr;

		s_Data->EntityInstances.clear();
	}

	Ref<ScriptClass> ScriptEngine::GetEntityClass(const std::string& name)
	{
		if (s_Data->EntityClasses.find(name) == s_Data->EntityClasses.end())
			return nullptr;

		return s_Data->EntityClasses.at(name);
	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetEntityClasses()
	{
		return s_Data->EntityClasses;
	}

	ScriptFieldMap& ScriptEngine::GetScriptFieldMap(Entity* entity)
	{
		VX_CORE_ASSERT(entity);
		UUID entityID = entity->GetID();
		return s_Data->EntityScriptFields[entityID];
	}

	Ref<ScriptInstance> ScriptEngine::GetEntityInstance(UUID uuid)
	{
		VX_CORE_ASSERT(s_Data->EntityInstances.find(uuid) != s_Data->EntityInstances.end());

		Ref<ScriptInstance> instance = s_Data->EntityInstances[uuid];
		return instance;
	}

	void ScriptEngine::LoadAssemblyClasses()
	{
		s_Data->EntityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Vertex", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, className);
			else
				fullName = className;

			if (fullName == "<Module>")
				continue;

			MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, className);

			if (monoClass == entityClass)
				continue;

			if (monoClass == nullptr)
				continue;

			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
			if (!isEntity)
				continue;

			Ref<ScriptClass> scriptClass = CreateRef<ScriptClass>(nameSpace, className);
			s_Data->EntityClasses[fullName] = scriptClass;

			// This routine is an iterator routine for retrieving the fields in a class.
			// You must pass a gpointer that points to zero and is treated as an opaque handle
			// to iterate over all of the elements. When no more values are available, the return value is NULL.

			int fieldCount = mono_class_num_fields(monoClass);
			VX_CORE_WARN("{} has {} fields:", className, fieldCount);

			void* iterator = nullptr;
			while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
			{
				const char* fieldName = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);
				if (flags & FIELD_ATTRIBUTE_PUBLIC)
				{
					MonoType* type = mono_field_get_type(field);
					ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(type);
					VX_CORE_WARN("  {} ({})", fieldName, Utils::ScriptFieldTypeToString(fieldType));
					scriptClass->m_Fields[fieldName] = { fieldType, fieldName, field };
				}
			}

			auto& entityClasses = s_Data->EntityClasses;
			//mono_field_get_value()
		}
	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_Data->CoreAssemblyImage;
	}

	MonoObject* ScriptEngine::GetManagedInstance(UUID uuid)
	{
		VX_CORE_ASSERT(s_Data->EntityInstances.find(uuid) != s_Data->EntityInstances.end());
		return s_Data->EntityInstances.at(uuid)->GetManagedObject();
	}

	MonoDomain* ScriptEngine::GetAppDomain()
	{
		return s_Data->AppDomain;
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore)
		: m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(isCore ? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		MonoObject* exception = nullptr;
		MonoObject* result = mono_runtime_invoke(method, instance, params, &exception);


		if (exception != nullptr) {
			// Handle the exception, perhaps by printing or logging it
			// Example: Retrieve exception message
			MonoString* exceptionMessage = mono_object_to_string(exception, nullptr);
			char* exceptionText = mono_string_to_utf8(exceptionMessage);
#ifndef VX_DIST

			if (exception != nullptr) {
				// Handle the exception, perhaps by printing or logging it
				// Example: Retrieve exception message
				MonoString* exceptionMessage = mono_object_to_string(exception, nullptr);
				char* exceptionText = mono_string_to_utf8(exceptionMessage);
				VX_ASSERT(false, exceptionText);
				mono_free(exceptionText);
			}
#else
			if (exception != nullptr) {
				// Handle the exception, perhaps by printing or logging it
				// Example: Retrieve exception message
				MonoString* exceptionMessage = mono_object_to_string(exception, nullptr);
				char* exceptionText = mono_string_to_utf8(exceptionMessage);
				VX_CRITICAL(exceptionText);
				Application::Get().Close();
				mono_free(exceptionText);
		}
#endif // !VX_DIST

			mono_free(exceptionText);
		}


		return result;
	}

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity* entity)
		: m_ScriptClass(scriptClass)
	{
		m_Instance = scriptClass->Instantiate();

		m_Constructor = s_Data->EntityClass.GetMethod(".ctor", 1);
		m_OnCreateMethod = scriptClass->GetMethod("OnCreate", 0);
		m_OnUpdateMethod = scriptClass->GetMethod("OnUpdate", 1);
		m_OnDrawMethod = scriptClass->GetMethod("OnDraw", 0);
		m_OnPhysUpdateMethod = scriptClass->GetMethod("OnPhysUpdate", 1);

		// Call Entity constructor
		{
			UUID entityID = entity->GetID();
			MonoString* idMono = mono_string_new(s_Data->AppDomain, entityID.c_str());
			void* param = idMono;
			m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &param);
		}
	}

	void ScriptInstance::InvokeOnCreate()
	{
		if (m_OnCreateMethod)
			m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod);
	}

	void ScriptInstance::InvokeOnUpdate(float ts)
	{
		if (m_OnUpdateMethod)
		{
			void* param = &ts;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &param);
		}
	}

	void ScriptInstance::InvokeOnPhysUpdate(float ts)
	{
		if (m_OnPhysUpdateMethod)
		{
			void* param = &ts;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnPhysUpdateMethod, &param);
		}
	}

	void ScriptInstance::InvokeOnDraw()
	{
		if (m_OnDrawMethod)
		{
			m_ScriptClass->InvokeMethod(m_Instance, m_OnDrawMethod, nullptr);
		}
	}

	bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
	{
		const auto& fields = m_ScriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_get_value(m_Instance, field.ClassField, buffer);
		return true;
	}

	bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
	{
		const auto& fields = m_ScriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_set_value(m_Instance, field.ClassField, (void*)value);
		return true;
	}
	

	
}


