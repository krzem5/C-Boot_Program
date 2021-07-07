#ifndef _MSC_VER
#error Non-Windows Platforms not Supported!
#endif
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <shellscalingapi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>



#define BLENDER_FILE_PATH "C:/Program Files/Blender Foundation/Blender/blender.exe"
#define CHROME_FILE_PATH "C:/Program Files/Google/Chrome Dev/Application/chrome.exe"
#define CUSTOM_ICON_FILE_PATH (__FILE_BASE_DIR__ L"rsrc/icon.ico")
#define EDITOR_FILE_PATH "C:/Program Files/Sublime Text 3/sublime_text.exe"
#define FLAG_DATA 1
#define FLAG_QUOTE 2
#define GIMP_FILE_PATH "C:/Program Files/GIMP 2/bin/gimp-2.10.exe"
#define HOTKEY_HANDLER_END_MESSAGE (WM_USER+1)
#define MINECRAFT_LAUNCHER_FILE_PATH "C:/Program Files (x86)/Minecraft Launcher/MinecraftLauncher.exe"
#define ROOT_FILE_PATH L"D:/k"



typedef void (*macro_handler_t)(void);



uint8_t _handle_macro_ig_alt=0;
char fp_bf[4096];
uint32_t fp_bfl;



uint8_t _cmp_str_len(const char* a,const char* b,uint8_t l){
	do{
		l--;
		if (*(a+l)!=*(b+l)){
			return 0;
		}
	} while (l);
	return 1;
}



void _create_process(const char* a){
	STARTUPINFOA si={
		sizeof(STARTUPINFOA)
	};
	PROCESS_INFORMATION pi;
	CreateProcessA(NULL,(char*)a,NULL,NULL,FALSE,CREATE_NEW_CONSOLE|CREATE_NEW_PROCESS_GROUP,NULL,NULL,&si,&pi);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}



HWND _console(void){
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
	return hwnd;
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
			if ((wp==WM_KEYDOWN||wp==WM_SYSKEYDOWN)&&dt->vkCode<256&&GetAsyncKeyState(VK_CONTROL)&&GetAsyncKeyState(VK_SHIFT)&&GetAsyncKeyState(VK_MENU)){
				switch (dt->vkCode){
					case VK_END:
						if (MessageBoxW(NULL,L"Shutdown?",L"Shutdown",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2|MB_SYSTEMMODAL)==IDYES){
							ExitWindowsEx(EWX_FORCE|EWX_FORCEIFHUNG|EWX_POWEROFF|EWX_SHUTDOWN,SHTDN_REASON_MAJOR_OTHER|SHTDN_REASON_MINOR_OTHER|SHTDN_REASON_FLAG_PLANNED);
						}
						break;
					case VK_HOME:
						if (MessageBoxW(NULL,L"Logout?",L"Logout",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2|MB_SYSTEMMODAL)==IDYES){
							ExitWindowsEx(EWX_LOGOFF,SHTDN_REASON_MAJOR_OTHER|SHTDN_REASON_MINOR_OTHER|SHTDN_REASON_FLAG_PLANNED);
						}
						break;
					case 'I':
					case 'Q':
					case 'R':
						{
							char tmp[4096];
							uint32_t i=0;
							for (;i<fp_bfl;i++){
								tmp[i]=fp_bf[i];
							}
							tmp[i]=(dt->vkCode=='I'?'7':(dt->vkCode=='Q'?'1':'0'));
							tmp[i+1]=0;
							_create_process(tmp);
							break;
						}
					case 'W':
						ShellExecuteW(NULL,L"open",ROOT_FILE_PATH,NULL,NULL,SW_SHOWMAXIMIZED);
						break;
				}
			}
		}
	}
	return CallNextHookEx(NULL,c,wp,lp);
}



LRESULT _screen_blocker_keyboard_handler(int c,WPARAM wp,LPARAM lp){
	KBDLLHOOKSTRUCT* dt=(KBDLLHOOKSTRUCT*)lp;
	if (dt->vkCode!=VK_ESCAPE){
		return -1;
	}
	PostMessageW(NULL,HOTKEY_HANDLER_END_MESSAGE,0,0);
	return CallNextHookEx(NULL,c,wp,lp);
}



LRESULT CALLBACK _screen_blocker_wnd_proc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
	if (msg==WM_KILLFOCUS){
		SetFocus(hwnd);
		return 0;
	}
	return DefWindowProcW(hwnd,msg,wp,lp);
}



int WinMain(HINSTANCE hi,HINSTANCE p_hi,LPSTR cmd,int sw){
	uint8_t fl=FLAG_SPACE;
	char argv_bf[4096];
	uint32_t i=0;
	uint32_t argc=0;
	char* argv[4096];
	argv[0]=argv_bf;
	char* str=GetCommandLineA();
	while (*str){
		if (fl&FLAG_QUOTE){
			if (*str=='\"'){
				fl=FLAG_DATA;
			}
			else{
				argv_bf[i]=*str;
				i++;
			}
		}
		else{
			if (*str=='\"'){
				if (!(fl&FLAG_DATA)){
					argv[argc]=argv_bf+i;
					argc++;
				}
				fl=FLAG_DATA|FLAG_QUOTE;
			}
			else if (*str==' '||*str=='\t'||*str=='\n'||*str=='\r'){
				if (fl&FLAG_DATA){
					argv_bf[i]=0;
					i++;
				}
				fl=0;
			}
			else{
				if (!(fl&FLAG_DATA)){
					argv[argc]=argv_bf+i;
					argc++;
				}
				argv_bf[i]=*str;
				i++;
				fl=FLAG_DATA;
			}
		}
		str++;
	}
	argv_bf[i]=0;
	fp_bfl=GetModuleFileNameA(NULL,fp_bf+1,4095)+3;
	fp_bf[0]='\"';
	fp_bf[fp_bfl-2]='\"';
	fp_bf[fp_bfl-1]=' ';
	if (argc<2){
		fp_bf[fp_bfl]='_';
		fp_bf[fp_bfl+1]=0;
		_create_process(fp_bf);
		return 0;
	}
	switch (argv[1][0]-48){
		case ('_'-48):
			{
				fp_bf[fp_bfl]='4';
				fp_bf[fp_bfl+1]=0;
				_create_process(fp_bf);
				fp_bf[fp_bfl]='7';
				_create_process(fp_bf);
				TOKEN_PRIVILEGES tkp;
				LookupPrivilegeValueW(NULL,SE_SHUTDOWN_NAME,&(tkp.Privileges[0].Luid));
				tkp.PrivilegeCount=1;
				tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
				HANDLE th;
				OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&th);
				AdjustTokenPrivileges(th,FALSE,&tkp,0,NULL,NULL);
				SetWindowsHookExW(WH_KEYBOARD_LL,_handle_macro,GetModuleHandleW(NULL),0);
				MSG msg;
				while (1){
					if (GetMessageW(&msg,NULL,0,0)){
						TranslateMessage(&msg);
						DispatchMessageW(&msg);
					}
				}
			}
		case 0:
			{
				HANDLE mh=GetModuleHandleW(NULL);
				WNDCLASSEXW wc={
					sizeof(WNDCLASSEXW),
					0,
					_screen_blocker_wnd_proc,
					0,
					0,
					mh,
					NULL,
					NULL,
					CreateSolidBrush(0x00000000),
					NULL,
					L"screen_blocker_window_class",
					NULL
				};
				RegisterClassExW(&wc);
				SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
				HWND hwnd=CreateWindowExW(WS_EX_TOPMOST,L"screen_blocker_window_class",L"Screen Blocker",WS_VISIBLE,0,0,100,100,NULL,NULL,mh,NULL);
				SetFocus(hwnd);
				SetWindowLongPtrW(hwnd,GWL_STYLE,WS_VISIBLE);
				SetWindowLongPtrW(hwnd,GWL_EXSTYLE,WS_EX_TOPMOST);
				MONITORINFO mi={
					sizeof(MONITORINFO)
				};
				GetMonitorInfoW(MonitorFromWindow(hwnd,MONITOR_DEFAULTTONEAREST),&mi);
				SetWindowPos(hwnd,HWND_TOP,mi.rcMonitor.left,mi.rcMonitor.top,mi.rcMonitor.right-mi.rcMonitor.left,mi.rcMonitor.bottom-mi.rcMonitor.top,SWP_SHOWWINDOW);
				HHOOK hh=SetWindowsHookExW(WH_KEYBOARD_LL,_screen_blocker_keyboard_handler,mh,0);
				ShowCursor(0);
				MSG msg;
				while (1){
					BOOL e=GetMessageW(&msg,NULL,0,0);
					if (e==-1){
						break;
					}
					if (e!=0){
						if (msg.message==HOTKEY_HANDLER_END_MESSAGE){
							break;
						}
						TranslateMessage(&msg);
						DispatchMessageW(&msg);
					}
				}
				DestroyWindow(hwnd);
				UnhookWindowsHookEx(hh);
				UnregisterClassW(L"screen_blocker_window_class",mh);
				INPUT il[6]={
					{
						INPUT_KEYBOARD,
						.ki={
							VK_SHIFT,
							0,
							0,
							0,
							0
						}
					},
					{
						INPUT_KEYBOARD,
						.ki={
							VK_SHIFT,
							0,
							KEYEVENTF_KEYUP,
							0,
							0
						}
					},
					{
						INPUT_KEYBOARD,
						.ki={
							VK_CONTROL,
							0,
							0,
							0,
							0
						}
					},
					{
						INPUT_KEYBOARD,
						.ki={
							VK_CONTROL,
							0,
							KEYEVENTF_KEYUP,
							0,
							0
						}
					},
					{
						INPUT_KEYBOARD,
						.ki={
							VK_MENU,
							0,
							0,
							0,
							0
						}
					},
					{
						INPUT_KEYBOARD,
						.ki={
							VK_MENU,
							0,
							KEYEVENTF_KEYUP,
							0,
							0
						}
					}
				};
				SendInput(6,il,sizeof(INPUT));
				return 0;
			}
		case 1:
			{
				SetFocus(_console());
				SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),128);
				HANDLE ho=GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleMode(ho,7);
				CONSOLE_CURSOR_INFO ci;
				GetConsoleCursorInfo(ho,&ci);
				ci.bVisible=0;
				SetConsoleCursorInfo(ho,&ci);
				char bf[256];
				uint8_t bfl=0;
				uint8_t u=1;
				uint8_t ll=0;
				while (1){
					if (_kbhit()){
						char k=_getch();
						if (k==0xe0){
							_getch();
						}
						else if (k==VK_CANCEL){
							return 0;
						}
						else if (k==VK_BACK){
							if (bfl>0){
								bfl--;
								u=1;
							}
						}
						else if (k==VK_RETURN){
							if (!bfl){
								return 0;
							}
							else if (bfl==4){
								if (_cmp_str_len(bf,"exit",4)){
									return 0;
								}
								if (_cmp_str_len(bf,"gimp",4)){
									_create_process("\""GIMP_FILE_PATH"\"");
									return 0;
								}
								if (_cmp_str_len(bf,"open",4)){
									fp_bf[fp_bfl]='2';
									fp_bf[fp_bfl+1]=0;
									_create_process(fp_bf);
									return 0;
								}
							}
							else if (bfl==5){
								if (_cmp_str_len(bf,"stats",5)){
									fp_bf[fp_bfl]='6';
									fp_bf[fp_bfl+1]=0;
									_create_process(fp_bf);
									return 0;
								}
							}
							else if (bfl==6){
								if (_cmp_str_len(bf,"chrome",6)){
									_create_process("\""CHROME_FILE_PATH"\"");
									return 0;
								}
								if (_cmp_str_len(bf,"serial",6)){
									fp_bf[fp_bfl]='3';
									fp_bf[fp_bfl+1]=0;
									_create_process(fp_bf);
									return 0;
								}
							}
							else if (bfl==7){
								if (_cmp_str_len(bf,"blender",7)){
									_create_process("\""BLENDER_FILE_PATH"\"");
									return 0;
								}
								if (_cmp_str_len(bf,"sublime",7)){
									_create_process("\""EDITOR_FILE_PATH"\"");
									return 0;
								}
							}
							else if (bfl==9){
								if (_cmp_str_len(bf,"minecraft",9)){
									_create_process("\""MINECRAFT_LAUNCHER_FILE_PATH"\"");
									return 0;
								}
							}
							bfl=0;
							u=1;
						}
						else if (k>31&&k<127){
							if (bfl<255){
								bf[bfl]=k;
								bfl++;
								u=1;
							}
						}
					}
					if (u){
						u=0;
						printf("\x1b[0;0H> ");
						for (uint8_t i=0;i<bfl;i++){
							putchar(bf[i]);
						}
						while (ll>bfl+2){
							ll--;
							putchar(' ');
						}
						ll=bfl+2;
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
