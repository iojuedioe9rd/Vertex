//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by Resource.rc
//

#define GET_Texture2D_AssetManager(filepath)\
 std::dynamic_pointer_cast<Texture2D>(AssetManager::GetAsset(\
	std::dynamic_pointer_cast<EditorAssetManager>(\
	AssetManager::GetAssetManager()) \
	->ImportAsset(filepath)))\
\

#define IDB_PNG1                        101
#define IDB_PNG2                        102
#define IDB_PNG3                        103
#define IDB_PNG4                        104
#define IDB_PNG5                        105

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        106
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
