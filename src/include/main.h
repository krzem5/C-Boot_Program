#ifndef __MAIN_H__
#define __MAIN_H__ 1
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif



#define CREATE_UPPER_KEY(x) (UPPER_KEY_MASK|((x)<<8))
#define IS_GETCH_DOUBLE_KEY(x) (!(x)||(x)==0xe0)
#define JSON_PARSER_NEXT_CHAR(p) (*((*(p))++))
#define PRINTF_TIME(t,...) \
	do{ \
		SYSTEMTIME __st; \
		GetLocalTime(&__st); \
		printf("\x1b[38;2;50;50;50m[%02u:%02u:%02u]\x1b[0m "t,__st.wHour,__st.wMinute,__st.wSecond,##__VA_ARGS__); \
	} while (0)
#define STR_LEN(x) (sizeof((x))/sizeof(char)-1)
#define _WCHAR_STR(x) L##x
#define WCHAR_STR(x) _WCHAR_STR(x)
#define BLENDER_FILE_PATH "C:/Program Files/Blender Foundation/Blender/blender.exe"
#define CHROME_FILE_PATH "C:/Program Files/Google/Chrome Dev/Application/chrome.exe"
#define CUSTOM_ICON_FILE_PATH WCHAR_STR(__FILE_BASE_DIR__)L"/rsrc/icon.ico"
#define EDITOR_FILE_PATH "C:/Program Files/Sublime Text 3/sublime_text.exe"
#define FLAG_ACCEPT_GITHUB 2
#define FLAG_ACCEPT_JSON 1
#define FLAG_ALL_INSIDE 4
#define FLAG_ASK_CREATE 2
#define FLAG_BINARY 1
#define FLAG_CHUNKED_TRANSFER 8
#define FLAG_DATA 1
#define FLAG_DIRECTORY 2
#define FLAG_EDIT_TYPE 4
#define FLAG_FROM_ROOT 8
#define FLAG_GITHUB_TOKEN 4
#define FLAG_INITIALIZE 2
#define FLAG_INVERT 1
#define FLAG_ONE_OR_MORE 2
#define FLAG_OPEN 1
#define FLAG_QUOTE 2
#define FLAG_UPDATE 1
#define FLAG_ZERO_OR_MORE 1
#define GETCH_DEL CREATE_UPPER_KEY('S')
#define GIMP_FILE_PATH "C:/Program Files/GIMP 2/bin/gimp-2.10.exe"
#define GITHUB_API_QUOTA 5000
#define GITHUB_DEFAULT_BRANCH_NAME "main"
#define GITHUB_HEADERS "application/vnd.github.v3+json"
#define GITHUB_PROJECT_BRANCH_LIST_FILE_PATH __FILE_BASE_DIR__"/data/github-branches.dt"
#define GITHUB_COMMIT_DATA_UPDATE_COUNT 0
#define GITHUB_COMMIT_DATA_SKIP_COUNT 1
#define GITHUB_COMMIT_DATA_IGNORE_COUNT 2
#define GITHUB_COMMIT_DATA_DELETE_COUNT 3
#define GITHUB_PUSHED_PROJECT_LIST_FILE_PATH __FILE_BASE_DIR__"/data/github.dt"
#define GITHUB_USERNAME "krzem5"
#define GITIGNORE_PATTERN_ELEMENT_CHAR_CLASS_CLASS_SIZE (256/(sizeof(uint64_t)*8))
#define GITIGNORE_PATTERN_ELEMENT_TYPE_ANY 0
#define GITIGNORE_PATTERN_ELEMENT_TYPE_ANY_STAR 1
#define GITIGNORE_PATTERN_ELEMENT_TYPE_CHAR 2
#define GITIGNORE_PATTERN_ELEMENT_TYPE_CHAR_CLASS 3
#define GITIGNORE_START_MATCH_REGEX "[^!]"
#define HOTKEY_HANDLER_END_MESSAGE (WM_USER+1)
#define HTTP_REQUEST_BUFFER_SIZE 4096
#define JSON_OBJECT_TYPE_ARRAY 0
#define JSON_OBJECT_TYPE_FALSE 1
#define JSON_OBJECT_TYPE_FLOAT 2
#define JSON_OBJECT_TYPE_INTEGER 3
#define JSON_OBJECT_TYPE_MAP 4
#define JSON_OBJECT_TYPE_NULL 5
#define JSON_OBJECT_TYPE_STRING 6
#define JSON_OBJECT_TYPE_TRUE 7
#define MINECRAFT_JAVA_RUNTIME_FILE_PATH "C:/Program Files/Java/jdk-16.0.1/bin/java.exe"
#define MINECRAFT_JAVA_RUNTIME_MEMORY "512M"
#define MINECRAFT_LAUNCHER_FILE_PATH "C:/Program Files (x86)/Minecraft Launcher/MinecraftLauncher.exe"
#define MINECRAFT_SERVER_FOLDER __FILE_BASE_DIR__"/mc_server/"
#define PROJECT_DIR "d:/k/code"
#define ROOT_FILE_PATH "d:/k"
#define SHA1_DATA_INIT {0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0}
#define TEMPLATES_FILE_PATH __FILE_BASE_DIR__"/templates"
#define UPPER_KEY_MASK 255
#define USER_AGENT_STRING "Request API"



struct __JSON_OBJECT;
struct __JSON_MAP_KEYPAIR;



typedef char* json_parser_state_t;



typedef struct __STRING_8BIT{
	uint8_t l;
	char v[256];
} string_8bit_t;



typedef struct __STRING_32BIT{
	uint32_t l;
	char* v;
} string_32bit_t;



typedef struct __PROJECT_TYPE{
	uint8_t l;
	char v[256];
	uint16_t el;
	string_8bit_t* e;
} project_type_t;



typedef union __EXPAND_DATA_EXTRA{
	char y[256];
	char fp[256];
} expand_data_extra_t;



typedef struct __EXPAND_DATA{
	char nm[256];
	char pt[256];
	char t[256];
	char u_nm[256];
	expand_data_extra_t e;
} expand_data_t;



typedef struct __JSON_ARRAY{
	uint32_t l;
	struct __JSON_OBJECT* dt;
} json_array_t;



typedef struct __JSON_MAP{
	uint32_t l;
	struct __JSON_MAP_KEYPAIR* dt;
} json_map_t;



typedef union __JSON_OBJECT_DATA{
	int64_t i;
	double f;
	string_32bit_t s;
	json_array_t a;
	json_map_t m;
} json_object_data_t;



typedef struct __JSON_OBJECT{
	uint8_t t;
	json_object_data_t dt;
} json_object_t;



typedef struct __JSON_MAP_KEYPAIR{
	string_32bit_t k;
	json_object_t v;
} json_map_keypair_t;



typedef struct __SHA1_DATA{
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;
	uint32_t e;
} sha1_data_t;



typedef struct __GITHUB_BRANCH{
	string_8bit_t nm;
	string_8bit_t b;
} github_branch_t;



typedef struct __PATTERN_ELEMENT_DATA_CHARA_CLASS{
	uint64_t dt[GITIGNORE_PATTERN_ELEMENT_CHAR_CLASS_CLASS_SIZE];
} gitignore_pattern_element_data_chara_class_t;



typedef union __PATTERN_ELEMENT_DATA{
	char c;
	gitignore_pattern_element_data_chara_class_t cc;
} gitignore_pattern_element_data_t;



typedef struct __PATTERN_ELEMENT{
	uint8_t t;
	gitignore_pattern_element_data_t dt;
} gitignore_pattern_element_t;



typedef struct __PATTERN{
	uint16_t sz;
	gitignore_pattern_element_t* dt;
} gitignore_pattern_t;



typedef struct __GITIGNORE_FILE_DATA_PATTERN{
	uint8_t fl;
	uint16_t sz;
	gitignore_pattern_t* dt;
} gitignore_file_data_pattern_t;



typedef struct __GITIGNORE_FILE_DATA{
	uint16_t sz;
	gitignore_file_data_pattern_t* dt;
} gitignore_file_data_t;



typedef struct __GITHUB_DIRECTORY_TREE_DATA{
	char nm[256];
	uint32_t sz;
	char sha[41];
	uint8_t rm;
} github_directory_tree_data_t;



typedef struct __GITHUB_DIRECTORY_TREE{
	uint32_t sz;
	github_directory_tree_data_t* dt;
} github_directory_tree_t;



typedef struct __GITHUB_COMMIT_DATA_FILE{
	char fp[256];
	char* dt;
} github_commit_data_file_t;



typedef struct __GITHUB_COMMIT_DATA{
	uint8_t fp_s;
	uint32_t cnt[4];
	uint32_t sz;
	github_commit_data_file_t* dt;
	const char* msg;
	const char* nm;
} github_commit_data_t;



BOOL _move_to_desktop(HWND hwnd,UINT n);



BOOL _switch_to_desktop(UINT n);



#ifdef __cplusplus
}
#endif
#endif
