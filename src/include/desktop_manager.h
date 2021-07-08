#ifndef __DESKTOP_MANAGER_H__
#define __DESKTOP_MANAGER_H__ 1
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#ifdef __cplusplus
extern "C" {
#endif



BOOL _move_to_desktop(HWND hwnd,UINT n);



BOOL _switch_to_desktop(UINT n);



#ifdef __cplusplus
}
#endif
#endif
