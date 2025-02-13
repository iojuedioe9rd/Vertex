#include "vxpch.h"
#include "Vertex/CustomAlloc/custom_alloc.h"
#include "LuaScripting.h"
#include "Vertex/Core/Buffer.h"
#include "Vertex/Core/FileSystem.h"
#include "Vertex/Lua/LuaGlue.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include "ArenaAllocator.h"
#include "sol/config.hpp"
#include "sol/forward.hpp"
#include "sol/sol.hpp"

/* Size for the buffer, in bytes */
#define BUFSEEDB	(sizeof(void*) + sizeof(time_t))

/* Size for the buffer in int's, rounded up */
#define BUFSEED		((BUFSEEDB + sizeof(int) - 1) / sizeof(int))

/*
** Copy the contents of variable 'v' into the buffer pointed by 'b'.
** (The '&b[0]' disguises 'b' to fix an absurd warning from clang.)
*/
#define addbuff(b,v)	(memcpy(&b[0], &(v), sizeof(v)), b += sizeof(v))


static unsigned int luai_makeseed(void) {
	unsigned int buff[BUFSEED];
	unsigned int res;
	unsigned int i;
	time_t t = time(NULL);
	char* b = (char*)buff;
	addbuff(b, b);  /* local variable's address */
	addbuff(b, t);  /* time */
	/* fill (rare but possible) remain of the buffer with zeros */
	memset(b, 0, sizeof(buff) - BUFSEEDB);
	res = buff[0];
	for (i = 1; i < BUFSEED; i++)
		res ^= (res >> 3) + (res << 7) + buff[i];
	return res;
}


namespace Vertex
{
	

	static std::function<int(lua_State*)>* g_lambda;  // Static pointer for the lambda

	// Wrapper function to invoke the lambda
	static int LuaFunctionWrapper(lua_State* L)
	{
		if (g_lambda)
		{
			return (*g_lambda)(L);  // Call the captured lambda
		}
		return 0;  // Fallback if no lambda is available
	}

	// This function converts a std::function into a lua_CFunction without using userdata
	static lua_CFunction GetTO(std::function<int(lua_State*)> func)
	{
		// Store the lambda in the static function pointer
		g_lambda = new std::function<int(lua_State*)>(func);

		// Return the static wrapper function that calls the lambda
		return LuaFunctionWrapper;
	}
	struct LuaScriptingData
	{
		lua_State* testState = nullptr;
		std::unordered_map<LuaScriptHandle, LuaScript*> scriptMap;

	};

	static LuaScriptingData* s_Data;

	void LuaScripting::Init()
	{
		s_Data = new LuaScriptingData();

		// Initialize the main Lua state
		s_Data->testState = luaL_newstate();
		luaL_openlibs(s_Data->testState);

		std::string cmd = "x = 7.53456 + 11.67558 + math.sin(23.7)";

		RunTests();
	}

	void LuaScripting::Shutdown()
	{
		if (s_Data->testState)
		{
			lua_close(s_Data->testState);
			s_Data->testState = nullptr;
		}

		delete s_Data;
		s_Data = nullptr;
	}

	LuaScriptHandle LuaScripting::NewScript(std::string name)
	{
		LuaScriptHandle handle = LuaScriptHandle();
		LuaScript* script = new LuaScript(handle);

		constexpr int POOL_SIZE = 1024 * 10;
		// Allocate memory on the heap to prevent stack corruption
		char* memory = new char[POOL_SIZE];
		ArenaAllocator* pool = new ArenaAllocator(memory, memory + POOL_SIZE);

		script->m_ArenaAllocator = pool;
		script->m_State = new sol::state(sol::default_at_panic, pool->l_alloc, pool);
		script->m_Name = name;

		s_Data->scriptMap[handle] = script;

		return handle;
	}

	bool LuaScripting::LoadScript(LuaScriptHandle& handle, Buffer* buffer, LuaBufferType bufferType)
	{
		auto it = s_Data->scriptMap.find(handle);
		if (it == s_Data->scriptMap.end())
			return false;

		LuaScript* script = it->second;

		switch (bufferType)
		{
		case LuaScripting::LuaBufferType::Text:
			return LoadScript_Text(script, buffer);
		case LuaScripting::LuaBufferType::RawData:
			// Handle raw data case if needed
			return false;
		default:
			return false;
		}
	}

	bool LuaScripting::UpdateScript(LuaScriptHandle& handle, Timestep& ts)
	{
		auto it = s_Data->scriptMap.find(handle);
		if (it == s_Data->scriptMap.end())
		{
			std::cerr << "Error: Script handle not found!" << std::endl;
			return false;
		}

		LuaScript* script = it->second;
		if (!script || !script->m_State)
		{
			std::cerr << "Error: Script or state is null!" << std::endl;
			return false;
		}

		sol::state& luaState = *script->m_State;
		sol::protected_function func = luaState["update"];

		if (!func.valid())
		{
			std::cerr << "Error: 'update' function not found in Lua script!" << std::endl;
			return false;
		}

		sol::protected_function_result result = func.call(ts.GetSeconds());

		if (!result.valid())
		{
			sol::error err = result.get<sol::error>();
			std::cerr << "Lua error: " << err.what() << std::endl;
			return false;
		}

		return true;
	}

	bool LuaScripting::LoadScript_Text(LuaScript* script, Buffer* buffer)
	{
		try
		{
			PreSetupLuaState(script);
			sol::protected_function_result result = script->m_State->safe_script(buffer->As<char>(), sol::script_pass_on_error);

			if (!result.valid())
			{
				sol::error err = result.get<sol::error>();
				std::cerr << "Lua script load error: " << err.what() << std::endl;
				return false;
			}
			PostSetupLuaState(script);
			return true;
		}
		catch (const std::exception& ex)
		{
			VX_CORE_ASSERT(false, ex.what());
			return false;
		}
	}

	

	void LuaScripting::PreSetupLuaState(LuaScript* script)
	{
		script->m_State->open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table);
		LuaGlue::PreSetupLuaState(script);
		
		
	}

	void LuaScripting::PostSetupLuaState(LuaScript* script)
	{
		LuaGlue::PostSetupLuaState(script);
	}

	void LuaScripting::SetScriptBehaviour(LuaScriptHandle& handle, BEHAVIOURLuaScript* behaviour)
	{
		auto it = s_Data->scriptMap.find(handle);
		LuaScript* script = it->second;

		script->m_Behaviour = behaviour;
	}

	void LuaScripting::RunTests()
	{
		int tests = 0;
		int successfulTests = 0;
		{
			tests++;
			VX_CORE_INFO("Test 1");

			std::string cmd = "x = 7.53456 + 11.67558 + math.sin(23.7)";

			lua_State* L = luaL_newstate();
			luaL_openlibs(L);

			int r = luaL_dostring(L, cmd.c_str());

			if (r == LUA_OK)
			{
				lua_getglobal(L, "x");
				if (lua_isnumber(L, -1))
				{
					float x = (float)lua_tonumber(L, -1);

					VX_CORE_INFO("(TEST 1) X from LUA is {0}", x);

					successfulTests++;
				}
			}
			else
			{
				std::string errormsg = lua_tostring(L, -1);
				VX_CORE_INFO(errormsg);
			}

			lua_close(L);

		}

		{
			
			
			constexpr char* LUA_FILE = R"(
				function Return4()
					return 4
				end
)";

			tests++;
			bool successful = 0;
			lua_State* L = luaL_newstate();
			luaL_dostring(L, LUA_FILE);
			lua_getglobal(L, "Return4");

			if (lua_isfunction(L, -1))
			{
				lua_pcall(L, 0, 1, 0);
				int returnNum = lua_tonumber(L, -1);
				VX_CORE_INFO("(TEST 2) {0}", returnNum);
				if (returnNum == 4)
				{
					successful = true;
				}
			}

			successfulTests += successful;

			lua_close(L);
		}

		{


			constexpr char* LUA_FILE = R"(
				function Pythagoras(a, b)
					return math.sqrt((a*a) + (b*b))
				end
)";

			tests++;
			bool successful = 0;
			lua_State* L = luaL_newstate();
			luaL_openlibs(L);

			luaL_dostring(L, LUA_FILE);
			lua_getglobal(L, "Pythagoras");

			if (lua_isfunction(L, -1))
			{
				lua_pushnumber(L, 3);
				lua_pushnumber(L, 4);
				constexpr int NUM_ARGS = 2;
				constexpr int NUM_RETURNS = 3;
				lua_pcall(L, NUM_ARGS, NUM_RETURNS, 0);
				lua_Number c = lua_tonumber(L, -3);
				VX_CORE_INFO("(TEST 3) csqr = {0}", (int)c);
				lua_Number a = lua_tonumber(L, -2);
				VX_CORE_INFO("(TEST 3) a = {0}", (int)a);
				lua_Number b = lua_tonumber(L, -1);
				VX_CORE_INFO("(TEST 3) b = {0}", (int)b);

				if (c == 5)
				{
					successful = true;
				}
			}

			successfulTests += successful;

			lua_close(L);
		}

		{
			bool successful = false;
			tests++;

			auto NativePythagoras = [](lua_State* L) -> int
				{
					lua_Number a = lua_tonumber(L, -2);
					lua_Number b = lua_tonumber(L, -1);
					lua_Number csqr = (a * a) + (b * b);
					lua_pushnumber(L, sqrt(csqr));
					return 1;
				};

			constexpr char* LUA_FILE = R"(
		function Pythagoras( a, b )
			csqr = NativePythagoras( a, b )
			return csqr, a, b
		end
		)";

			lua_State* L = luaL_newstate();
			lua_pushcfunction(L, NativePythagoras);
			lua_setglobal(L, "NativePythagoras");
			luaL_dostring(L, LUA_FILE);
			lua_getglobal(L, "Pythagoras");
			if (lua_isfunction(L, -1))
			{
				lua_pushnumber(L, 3);
				lua_pushnumber(L, 4);
				constexpr int NUM_ARGS = 2;
				constexpr int NUM_RETURNS = 3;
				lua_pcall(L, NUM_ARGS, NUM_RETURNS, 0);
				lua_Number c = lua_tonumber(L, -3);
				VX_CORE_INFO("(TEST 4) csqr = {0}", (int)c);
				lua_Number a = lua_tonumber(L, -2);
				VX_CORE_INFO("(TEST 4) a = {0}", (int)a);
				lua_Number b = lua_tonumber(L, -1);
				VX_CORE_INFO("(TEST 4) b = {0}", (int)b);

				if (c == 5)
				{
					successful = true;
				}
			}
			lua_close(L);

			successfulTests += successful;
		}

		{
			tests++;

			uint8_t successful = false;

			lua_State* L = luaL_newstate();
			luaL_openlibs(L);

			struct Sprite
			{
				glm::vec2 pos;
				uint8_t* successPtr;  // Pointer to track success count
				Sprite(uint8_t* successPtr) : pos(0.0f), successPtr(successPtr) {}


				~Sprite()
				{
					(*successPtr) += 1;
				}

				void Move(glm::vec2 pos)
				{
					this->pos += pos;
				}
			};
			std::function<int(lua_State*)> CreateSpriteLambda = [&](lua_State* L) -> int
				{
					void* ptr = lua_newuserdata(L, sizeof(Sprite));
					new (ptr) Sprite(&successful);

					luaL_getmetatable(L, "SpriteMetaTable");
					lua_setmetatable(L, -2);
					return 1;
				};



			auto MoveSprite = [](lua_State* L) -> int
				{
					Sprite* sprite = (Sprite*)lua_touserdata(L, -3);
					lua_Number velX = lua_tonumber(L, -2);
					lua_Number velY = lua_tonumber(L, -1);
					if (sprite)
						sprite->Move(glm::vec2(velX, velY));
					return 0;
				};

			auto DestroySprite = [](lua_State* L) -> int
				{
					Sprite* sprite = (Sprite*)lua_touserdata(L, -1);
					sprite->~Sprite();
					return 0;
				};

			constexpr char* LUA_FILE = R"(
			sprite = Sprite.new()
			sprite:Move( 6, 7 )		-- Sprite.Move( sprite, 6, 7 )

			sprite2 = Sprite.new()
			sprite2:Move( 6, 7 )		-- Sprite.Move( sprite2, 6, 7 )
)";
			lua_newtable(L);
			int spriteTableIdx = lua_gettop(L);
			lua_pushvalue(L, spriteTableIdx);
			lua_setglobal(L, "Sprite");

			lua_pushcfunction(L, GetTO(CreateSpriteLambda));
			lua_setfield(L, -2, "new");
			lua_pushcfunction(L, MoveSprite);
			lua_setfield(L, -2, "Move");

			luaL_newmetatable(L, "SpriteMetaTable");
			lua_pushstring(L, "__gc");
			lua_pushcfunction(L, DestroySprite);
			lua_settable(L, -3);

			lua_pushstring(L, "__index");
			lua_pushvalue(L, spriteTableIdx);
			lua_settable(L, -3);

			lua_pushcfunction(L, GetTO(CreateSpriteLambda));
			lua_setglobal(L, "CreateSprite");

			lua_pushcfunction(L, MoveSprite);
			lua_setglobal(L, "MoveSprite");



			int doResult = luaL_dostring(L, LUA_FILE);
			if (doResult != LUA_OK)
			{
				VX_CORE_INFO("(TEST 5) Error: {}", lua_tostring(L, -1));
			}
			lua_getglobal(L, "sprite");
			if (lua_isuserdata(L, -1))
			{
				VX_CORE_INFO("(TEST 5) We got a sprite from Lua");
				Sprite* sprite = (Sprite*)lua_touserdata(L, -1);
				VX_CORE_INFO("(TEST 5) x = {0}, y = {0}", sprite->pos.x, sprite->pos.y);
				if (sprite->pos == glm::vec2(6, 7))
					successful += 1;
			}
			else
			{
				VX_CORE_INFO("(TEST 5) We DIDNT get a sprite from Lua");
			}
			lua_close(L);

			successfulTests += (successful >= 3) ? 1 : 0;
		}

		{
			tests++;

			struct LuaMem
			{
				static void* l_alloc(void* ud, void* ptr, size_t osize, size_t nsize) {
					(void)ud; (void)osize;  /* not used */
					if (nsize == 0) {
						free(ptr);
						return NULL;
					}
					else
						return realloc(ptr, nsize);
				}
			};

			void* ud = nullptr;
			lua_State* L = lua_newstate(LuaMem::l_alloc, ud);
			successfulTests += (L != nullptr) ? 1 : 0;
			lua_close(L);
		}

		{
			tests++;

			constexpr int POOL_SIZE = 1024 * 10;
			char memory[POOL_SIZE];
			ArenaAllocator pool(memory, &memory[POOL_SIZE - 1]);
			lua_State* L = lua_newstate(ArenaAllocator::l_alloc, &pool);
			successfulTests += (L != nullptr) ? 1 : 0;
			lua_close(L);
		}

		{
			tests++;

			constexpr int POOL_SIZE = 1024 * 10;
			char memory[POOL_SIZE];
			ArenaAllocator pool(memory, &memory[POOL_SIZE - 1]);
			lua_State* L = lua_newstate(ArenaAllocator::l_alloc, &pool);

			struct alignas(8) Thing
			{
				float x;
				float z;
			};

			Thing* t = (Thing*)lua_newuserdata(L, sizeof(Thing));
			successfulTests +=((uintptr_t)t% alignof(Thing) == 0); //are we aligned???

			
			lua_close(L);
		}

		
		{
			tests++;
			struct Sprite
			{
				int x;
				int y;

				Sprite() : x(0), y(0) {}
				~Sprite() {}

				void Move(int velX, int velY)
				{
					x += velX;
					y += velY;
				}

				void Draw()
				{
				}
			};

			struct SpriteManager
			{
				std::vector<Sprite*> m_sprites;
				int numberOfSpritesExisting = 0;
				int numberOfSpritesMade = 0;

				void LookAfterSprite(Sprite* sprite)
				{
					numberOfSpritesExisting++;
					numberOfSpritesMade++;
					m_sprites.push_back(sprite);
				}

				void ForgetSprite(Sprite* sprite)
				{
					int i = 0;
					for (auto& s : m_sprites)
					{
						if (s == sprite)
						{
							numberOfSpritesExisting--;
							m_sprites.erase(m_sprites.begin() + i);
							return;
						}
						i++;
					}
				}
			};

			SpriteManager spriteManager;

			auto CreateSprite = [](lua_State* L) -> int
				{
					SpriteManager* sm = (SpriteManager*)lua_touserdata(L, lua_upvalueindex(1));
					VX_CORE_ASSERT(sm);

					void* pointerToASprite = lua_newuserdata(L, sizeof(Sprite));
					new (pointerToASprite) Sprite();
					luaL_getmetatable(L, "SpriteMetaTable");
					VX_CORE_ASSERT(lua_istable(L, -1));
					lua_setmetatable(L, -2);

					lua_newtable(L);
					lua_setuservalue(L, 1);

					sm->LookAfterSprite((Sprite*)pointerToASprite);

					return 1;
				};

			auto DestroySprite = [](lua_State* L) -> int
				{
					SpriteManager* sm = (SpriteManager*)lua_touserdata(L, lua_upvalueindex(1));
					VX_CORE_ASSERT(sm);

					Sprite* sprite = (Sprite*)lua_touserdata(L, -1);
					sm->ForgetSprite(sprite);
					sprite->~Sprite();
					return 0;
				};

			auto MoveSprite = [](lua_State* L) -> int
				{
					Sprite* sprite = (Sprite*)lua_touserdata(L, -3);
					lua_Number velX = lua_tonumber(L, -2);
					lua_Number velY = lua_tonumber(L, -1);
					sprite->Move((int)velX, (int)velY);
					return 0;
				};

			auto DrawSprite = [](lua_State* L) -> int
				{
					Sprite* sprite = (Sprite*)lua_touserdata(L, -1);
					sprite->Draw();
					return 0;
				};

			auto SpriteIndex = [](lua_State* L) -> int
				{
					VX_CORE_ASSERT(lua_isuserdata(L, -2));	//1
					VX_CORE_ASSERT(lua_isstring(L, -1));	//2

					Sprite* sprite = (Sprite*)lua_touserdata(L, -2);
					const char* index = lua_tostring(L, -1);
					if (strcmp(index, "x") == 0)
					{
						lua_pushnumber(L, sprite->x);
						return 1;
					}
					else if (strcmp(index, "y") == 0)
					{
						lua_pushnumber(L, sprite->y);
						return 1;
					}
					else
					{
						lua_getuservalue(L, 1);
						lua_pushvalue(L, 2);
						lua_gettable(L, -2);
						if (lua_isnil(L, -1))
						{
							lua_getglobal(L, "Sprite");
							lua_pushstring(L, index);
							lua_rawget(L, -2);
						}
						return 1;
					}
				};

				auto SpriteNewIndex = [](lua_State* L) -> int
					{
						VX_CORE_ASSERT(lua_isuserdata(L, -3));  //1
						VX_CORE_ASSERT(lua_isstring(L, -2));	//2
						// -1 - value we want to set	//3

						Sprite* sprite = (Sprite*)lua_touserdata(L, -3);
						const char* index = lua_tostring(L, -2);
						if (strcmp(index, "x") == 0)
						{
							sprite->x = (int)lua_tonumber(L, -1);
						}
						else if (strcmp(index, "y") == 0)
						{
							sprite->y = (int)lua_tonumber(L, -1);
						}
						else
						{
							lua_getuservalue(L, 1);	//1
							lua_pushvalue(L, 2);	//2
							lua_pushvalue(L, 3);	//3
							lua_settable(L, -3);	//1[2] = 3
						}

						return 0;
					};

				constexpr char* LUA_FILE = R"(
		sprite = Sprite.new()
		sprite:Move( 6, 7 )		-- Sprite.Move( sprite, 6, 7 )
		sprite:Draw()
		sprite.y = 10
		sprite.zzz = 99
		sprite.x = sprite.zzz
		sprite:Draw()
		Sprite.new()
		Sprite.new()
		)";

				constexpr int POOL_SIZE = 1024 * 10;
				char memory[POOL_SIZE];
				ArenaAllocator pool(memory, &memory[POOL_SIZE - 1]);
				lua_State* L = lua_newstate(ArenaAllocator::l_alloc, &pool);

				lua_newtable(L);
				int spriteTableIdx = lua_gettop(L);
				lua_pushvalue(L, spriteTableIdx);
				lua_setglobal(L, "Sprite");

				constexpr int NUMBER_OF_UPVALUES = 1;
				lua_pushlightuserdata(L, &spriteManager);
				lua_pushcclosure(L, CreateSprite, NUMBER_OF_UPVALUES);
				lua_setfield(L, -2, "new");
				lua_pushcfunction(L, MoveSprite);
				lua_setfield(L, -2, "Move");
				lua_pushcfunction(L, DrawSprite);
				lua_setfield(L, -2, "Draw");

				luaL_newmetatable(L, "SpriteMetaTable");
				lua_pushstring(L, "__gc");
				lua_pushlightuserdata(L, &spriteManager);
				lua_pushcclosure(L, DestroySprite, NUMBER_OF_UPVALUES);
				lua_settable(L, -3);

				lua_pushstring(L, "__index");
				lua_pushcfunction(L, SpriteIndex);
				lua_settable(L, -3);

				lua_pushstring(L, "__newindex");
				lua_pushcfunction(L, SpriteNewIndex);
				lua_settable(L, -3);

				int doResult = luaL_dostring(L, LUA_FILE);
				if (doResult != LUA_OK)
				{
					VX_CORE_INFO("Error: {}", lua_tostring(L, -1));
				}
				else
				{
					successfulTests++;
				}

				lua_close(L);

				VX_CORE_ASSERT(spriteManager.numberOfSpritesExisting == 0);
				VX_CORE_ASSERT(spriteManager.numberOfSpritesMade == 3);
		}

		
		{


			tests++;
			
			sol::state state = sol::state();
			state.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table);
			state.do_string("a = (2 + 2) * 10");
			int a = state["a"].get<int>();
			VX_CORE_INFO("a is {0}", a);
			if (a == 40)
			{
				successfulTests++;
			}
		}

		{
			tests++;

			sol::state state = sol::state();
			state.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table);
			constexpr char* LUA_FILE = R"(
			function update()
				return 4.2 + 5.1
			end)";


			state.do_string(LUA_FILE);
			
			float updateCallRet = state["update"].call().get<float>();

			if ((int)updateCallRet == 9)
			{
				successfulTests++;
			}
		}

		{
			tests++;

			sol::state state = sol::state();
			state.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table);
			
			try
			{
				state.safe_script("a = 1 + 24d34t + egr43");
			}
			catch (const std::exception& ex)
			{
				successfulTests++;
			}

		}

		{
			tests++;

			sol::state state = sol::state();
			state.do_string("player = {Name = \"Test\", IQ = \"1\"}");


			sol::table a_table = state["player"];
			if (a_table)
			{
				int sfdf = 0;
				for (const auto& entry : a_table)
				{
					sol::object key = entry.first;
					sol::object value = entry.second;

					std::string sKey = key.as<std::string>();

					

					if (sKey == "IQ")
					{
						if (value.as<std::string>() == "1") { sfdf++; }
					}

					if (sKey == "Name")
					{
						if (value.as<std::string>() == "Test") { sfdf++; }
					}

					
				}
				if (sfdf == 2) { successfulTests++; }
			}
		}

		

		VX_CORE_INFO("{0} / {1} tests successful!", successfulTests, tests);
	}

	

	



	
}