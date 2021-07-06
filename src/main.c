#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>



#define CUSTOM_ICON_FILE_PATH (__FILE_BASE_DIR__ L"rsrc/icon.ico")
#define FLAG_DATA 1
#define FLAG_SPACE 2
#define FLAG_QUOTE 4



typedef void (*macro_handler_t)(void);



macro_handler_t _handle_macro_hk[256]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
uint8_t _handle_macro_ig_alt=0;



void _create_process(const char* a){
	STARTUPINFOA si={
		sizeof(STARTUPINFOA)
	};
	PROCESS_INFORMATION pi;
	CreateProcessA(NULL,(char*)a,NULL,NULL,FALSE,CREATE_NEW_CONSOLE|CREATE_NEW_PROCESS_GROUP,NULL,NULL,&si,&pi);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}



void _console(void){
	AllocConsole();
	FILE* f=_fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE),_O_TEXT),"w");
	freopen_s(&f,"CONOUT$","w",stdout);
	freopen_s(&f,"CONOUT$","w",stderr);
	f=_fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_INPUT_HANDLE),_O_TEXT),"r");
	freopen_s(&f,"CONIN$","r",stdin);
	HWND hwnd=GetConsoleWindow();
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),7);
	SetConsoleTitleW(L"");
	SendMessageW(hwnd,WM_SETICON,ICON_SMALL,(LPARAM)LoadImageW(NULL,CUSTOM_ICON_FILE_PATH,IMAGE_ICON,16,16,LR_LOADFROMFILE));
	SendMessageW(hwnd,WM_SETICON,ICON_BIG,(LPARAM)LoadImageW(NULL,CUSTOM_ICON_FILE_PATH,IMAGE_ICON,32,32,LR_LOADFROMFILE));
}



LRESULT _handle_macro(int c,WPARAM wp,LPARAM lp){
	KBDLLHOOKSTRUCT* dt=(KBDLLHOOKSTRUCT*)lp;
	if (dt->vkCode!=VK_PACKET&&(dt->flags&(LLKHF_INJECTED|LLKHF_ALTDOWN|LLKHF_UP))==0){
		if (_handle_macro_ig_alt&&dt->vkCode==VK_RMENU){
			_handle_macro_ig_alt=0;
		}
		else{
			if (dt->vkCode==VK_LCONTROL){
				_handle_macro_ig_alt=1;
			}
			if ((wp==WM_KEYDOWN||wp==WM_SYSKEYDOWN)&&dt->vkCode<256&&_handle_macro_hk[dt->vkCode]&&GetAsyncKeyState(VK_CONTROL)&&GetAsyncKeyState(VK_SHIFT)&&GetAsyncKeyState(VK_MENU)){
				_handle_macro_hk[dt->vkCode]();
			}
		}
	}
	return CallNextHookEx(NULL,c,wp,lp);
}



void _logout(void){
	if (MessageBoxW(NULL,L"Logout?",L"Logout",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2|MB_SYSTEMMODAL)==IDYES){
		HANDLE th;
		TOKEN_PRIVILEGES tkp;
		OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&th);
		LookupPrivilegeValueW(NULL,SE_SHUTDOWN_NAME,&(tkp.Privileges[0].Luid));
		tkp.PrivilegeCount=1;
		tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(th,FALSE,&tkp,0,NULL,NULL);
		ExitWindowsEx(EWX_LOGOFF,SHTDN_REASON_MAJOR_OTHER|SHTDN_REASON_MINOR_OTHER|SHTDN_REASON_FLAG_PLANNED);
	}
}



void _shutdown(void){
	if (MessageBoxW(NULL,L"Logout?",L"Logout",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2|MB_SYSTEMMODAL)==IDYES){
		HANDLE th;
		TOKEN_PRIVILEGES tkp;
		OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&th);
		LookupPrivilegeValueW(NULL,SE_SHUTDOWN_NAME,&(tkp.Privileges[0].Luid));
		tkp.PrivilegeCount=1;
		tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(th,FALSE,&tkp,0,NULL,NULL);
		ExitWindowsEx(EWX_FORCE|EWX_FORCEIFHUNG|EWX_POWEROFF|EWX_SHUTDOWN,SHTDN_REASON_MAJOR_OTHER|SHTDN_REASON_MINOR_OTHER|SHTDN_REASON_FLAG_PLANNED);
	}
}



int WinMain(HINSTANCE hi,HINSTANCE p_hi,LPSTR cmd,int sw){
	uint8_t fl=FLAG_SPACE;
	char bf[4096];
	uint32_t i=0;
	uint32_t argc=0;
	char* argv[4096];
	argv[0]=bf;
	char* str=GetCommandLineA();
	while (*str){
		if (fl&FLAG_QUOTE){
			if (*str=='\"'){
				fl=FLAG_DATA;
			}
			else{
				bf[i]=*str;
				i++;
			}
		}
		else{
			if (*str=='\"'){
				if (fl&FLAG_SPACE){
					argv[argc]=bf+i;
					argc++;
				}
				fl=FLAG_DATA|FLAG_QUOTE;
			}
			else if (*str==' '||*str=='\t'||*str=='\n'||*str=='\r'){
				if (fl&FLAG_DATA){
					bf[i]=0;
					i++;
				}
				fl=FLAG_SPACE;
			}
			else{
				if (fl&FLAG_SPACE){
					argv[argc]=bf+i;
					argc++;
				}
				bf[i]=*str;
				i++;
				fl=FLAG_DATA;
			}
		}
		str++;
	}
	bf[i]=0;
	char fp_bf[4096]={'\"'};
	uint32_t fp_bfi=GetModuleFileNameA(NULL,fp_bf+1,4095)+3;
	fp_bf[fp_bfi-2]='\"';
	fp_bf[fp_bfi-1]=' ';
	if (argc<2){
		fp_bf[fp_bfi]='0';
		fp_bf[fp_bfi+1]=0;
		_create_process(fp_bf);
		return 0;
	}
	switch (argv[1][0]-48){
		case 0:
			{
				fp_bf[fp_bfi]='7';
				fp_bf[fp_bfi+1]=0;
				_create_process(fp_bf);
				_handle_macro_hk[VK_END]=_shutdown;
				_handle_macro_hk[VK_HOME]=_logout;
				SetWindowsHookExW(WH_KEYBOARD_LL,_handle_macro,GetModuleHandleW(NULL),0);
				MSG msg;
				while (1){
					if (GetMessageW(&msg,NULL,0,0)){
						TranslateMessage(&msg);
						DispatchMessageW(&msg);
					}
				}
			}
		default:
			_console();
			for (uint32_t i=0;i<argc;i++){
				printf("%u: %s\n",i,argv[i]);
			}
			getchar();
			return 1;
	}
	return 0;
}
