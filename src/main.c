#ifndef _MSC_VER
#error Non-Windows Platforms not Supported!
#endif
#define WIN32_LEAN_AND_MEAN 1
#define _CRT_SECURE_NO_WARNINGS 1
#include <windows.h>
#include <shellapi.h>
#include <shellscalingapi.h>
#include <winhttp.h>
#include <conio.h>
#include <desktop_manager.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



#define CREATE_UPPER_KEY(x) (UPPER_KEY_MASK|((x)<<8))
#define IS_GETCH_DOUBLE_KEY(x) (!(x)||(x)==0xe0)
#define JSON_PARSER_NEXT_CHAR(p) (*((*(p))++))
#define PRINTF_TIME(t,...) \
	do{ \
		SYSTEMTIME __st; \
		GetLocalTime(&__st); \
		printf("\x1b[38;2;50;50;50m[%02u:%02u:%02u]\x1b[0m "t,__st.wHour,__st.wMinute,__st.wSecond,##__VA_ARGS__); \
	} while (0)
#define _WCHAR_STR(x) L##x
#define WCHAR_STR(x) _WCHAR_STR(x)
#define BLENDER_FILE_PATH "C:/Program Files/Blender Foundation/Blender/blender.exe"
#define CHROME_FILE_PATH "C:/Program Files/Google/Chrome Dev/Application/chrome.exe"
#define CUSTOM_ICON_FILE_PATH WCHAR_STR(__FILE_BASE_DIR__)L"/rsrc/icon.ico"
#define EDITOR_FILE_PATH "C:/Program Files/Sublime Text 3/sublime_text.exe"
#define FLAG_ASK_CREATE 2
#define FLAG_DATA 1
#define FLAG_EDIT_TYPE 4
#define FLAG_INITIALIZE 2
#define FLAG_OPEN 1
#define FLAG_QUOTE 2
#define FLAG_UPDATE 1
#define GETCH_DEL CREATE_UPPER_KEY('S')
#define GIMP_FILE_PATH "C:/Program Files/GIMP 2/bin/gimp-2.10.exe"
#define HOTKEY_HANDLER_END_MESSAGE (WM_USER+1)
#define HTTP_REQUEST_BUFFER_SIZE 65536
#define JSON_OBJECT_TYPE_NULL 0
#define JSON_OBJECT_TYPE_FALSE 1
#define JSON_OBJECT_TYPE_TRUE 2
#define JSON_OBJECT_TYPE_INTEGER 3
#define JSON_OBJECT_TYPE_STRING 4
#define JSON_OBJECT_TYPE_FLOAT 5
#define JSON_OBJECT_TYPE_ARRAY 6
#define JSON_OBJECT_TYPE_MAP 7
#define MINECRAFT_JAVA_RUNTIME_FILE_PATH "C:/Program Files/Java/jdk-16.0.1/bin/java.exe"
#define MINECRAFT_JAVA_RUNTIME_MEMORY "512M"
#define MINECRAFT_LAUNCHER_FILE_PATH "C:/Program Files (x86)/Minecraft Launcher/MinecraftLauncher.exe"
#define MINECRAFT_SERVER_FOLDER __FILE_BASE_DIR__"/mc_server/"
#define PROJECT_DIR "d:/k/code"
#define ROOT_FILE_PATH "d:/k"
#define SHA1_DATA_INIT {0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0}
#define TEMPLATES_FILE_PATH __FILE_BASE_DIR__"/templates"
#define UPPER_KEY_MASK 255
#define USER_AGENT_STRING WCHAR_STR("Boot Program Request API")



struct __JSON_OBJECT;
struct __JSON_MAP_KEYPAIR;



typedef uint16_t wchar_t;



typedef char* json_parser_state_t;



typedef struct __STRING_8BIT{
	uint8_t l;
	char v[256];
} string_8bit_t;



typedef struct __STRING_32BIT{
	uint32_t l;
	char* v;
} string_32bit_t;



typedef struct __PROGRAM_TYPE{
	uint8_t l;
	char v[256];
	uint16_t el;
	string_8bit_t* e;
} program_type_t;



typedef struct __EXPAND_DATA{
	char nm[256];
	char pt[256];
	char t[256];
	char u_nm[256];
	char y[256];
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



uint8_t _cmp_str_len_lower(const char* a,const char* b,uint8_t l){
	do{
		l--;
		char c=*(a+l);
		char d=*(b+l);
		if (c>96&&c<123){
			c-=32;
		}
		if (d>96&&d<123){
			d-=32;
		}
		if (c!=d){
			return 0;
		}
	} while (l);
	return 1;
}



uint8_t _copy_str(char* d,const char* s){
	uint8_t o=0;
	while (*(s+o)){
		*(d+o)=*(s+o);
		o++;
	}
	return o;
}



uint8_t _copy_str_expand(char* d,const char* s,expand_data_t* e_dt){
	uint8_t i=0;
	uint8_t o=0;
	while (*(s+i)){
		char c=*(s+i);
		if (c=='$'){
			i++;
			uint8_t j=i;
			while (*(s+i)!='$'){
				i++;
			}
			if (i-j==4&&_cmp_str_len_lower(s+j,"name",4)){
				o+=_copy_str(d+o,e_dt->nm);
			}
			else if (i-j==12&&_cmp_str_len_lower(s+j,"pretty_title",12)){
				o+=_copy_str(d+o,e_dt->pt);
			}
			else if (i-j==10&&_cmp_str_len_lower(s+j,"title_name",10)){
				o+=_copy_str(d+o,e_dt->t);
			}
			else if (i-j==14&&_cmp_str_len_lower(s+j,"uppercase_name",14)){
				o+=_copy_str(d+o,e_dt->u_nm);
			}
			else if (i-j==4&&_cmp_str_len_lower(s+j,"year",4)){
				o+=_copy_str(d+o,e_dt->y);
			}
			i++;
		}
		else{
			i++;
			*(d+o)=c;
			o++;
		}
	}
	return o;
}



uint8_t _str_title_case(char* d,const char* s,uint8_t l){
	uint8_t o=0;
	uint8_t st=1;
	for (uint8_t i=0;i<l;i++){
		char c=*(s+i);
		if (c=='_'&&!st){
			*(d+o)='_';
			o++;
			st=1;
		}
		else if (!st){
			if (c>64&&c<91){
				c+=32;
			}
			*(d+o)=c;
			o++;
		}
		else if (st){
			if (c>96&&c<123){
				c-=32;
			}
			*(d+o)=c;
			o++;
			st=0;
		}
	}
	return o;
}



wchar_t* _expand_to_wide(const char* s){
	uint32_t sz=0;
	while (*(s+sz)){
		sz++;
	}
	wchar_t* o=malloc((sz+1)*sizeof(wchar_t));
	for (uint32_t i=0;i<sz;i++){
		*(o+i)=*(s+i);
	}
	*(o+sz)=0;
	return o;
}



void _generate_expand_data(expand_data_t* o,const char* t,const char* nm){
	uint8_t j=0;
	uint8_t st=1;
	while (*t){
		char c=*t;
		if (c=='_'&&!st){
			o->pt[j]=' ';
			j++;
			st=1;
		}
		else if (!st){
			o->pt[j]=(c>64&&c<91?c+32:c);
			j++;
		}
		else if (st){
			o->pt[j]=(c>96&&c<123?c-32:c);
			j++;
			st=0;
		}
		t++;
	}
	if (!st){
		o->pt[j]=' ';
		j++;
	}
	o->pt[j]='-';
	o->pt[j+1]=' ';
	j+=2;
	st=1;
	uint8_t i=0;
	uint8_t k=0;
	while (*nm){
		char c=*nm;
		o->nm[i]=(c>64&&c<91?c+32:c);
		o->u_nm[i]=(c>96&&c<123?c+32:c);
		if (c=='_'&&!st){
			o->pt[j]=' ';
			o->t[k]=' ';
			j++;
			k++;
			st=1;
		}
		else if (!st){
			o->pt[j]=(c>64&&c<91?c+32:c);
			o->t[k]=(c>64&&c<91?c+32:c);
			j++;
			k++;
		}
		else if (st){
			o->pt[j]=(c>96&&c<123?c-32:c);
			o->t[k]=(c>96&&c<123?c-32:c);
			j++;
			k++;
			st=0;
		}
		nm++;
		i++;
	}
	o->nm[i]=0;
	o->pt[j]=0;
	o->t[k]=0;
	o->u_nm[i]=0;
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	char bf[5];
	uint8_t m=0;
	uint32_t v=tm.wYear;
	while (v){
		bf[m]=(v%10)+48;
		m++;
		v/=10;
	}
	uint8_t n=0;
	while (m){
		m--;
		o->y[n]=bf[m];
		n++;
	}
	o->y[n]=0;
}



void _sha1_chunk(sha1_data_t* h,uint8_t* bf){
	uint32_t w[80];
	uint32_t a=h->a;
	uint32_t b=h->b;
	uint32_t c=h->c;
	uint32_t d=h->d;
	uint32_t e=h->e;
	for (uint8_t i=0;i<64;i+=4){
		w[i>>2]=(((*(bf+i))<<24)|((*(bf+i+1))<<16)|((*(bf+i+2))<<8)|(*(bf+i+3)));
		uint32_t na=((a<<5)|(a>>27))+(d^(b&c)^(b&d))+e+0x5a827999+w[i>>2];
		e=d;
		d=c;
		c=(b<<30)|(b>>2);
		b=a;
		a=na;
	}
	for (uint8_t i=16;i<80;i++){
		uint32_t v=w[i-3]^w[i-8]^w[i-14]^w[i-16];
		w[i]=(v<<1)|(v>>31);
		uint32_t na=((a<<5)|(a>>27))+e+w[i];
		if (i<20){
			na+=(d^(b&c)^(b&d))+0x5a827999;
		}
		else if (i<40){
			na+=(b^c^d)+0x6ed9eba1;
		}
		else if (i<60){
			na+=((b&c)|(b&d)|(c&d))+0x8f1bbcdc;
		}
		else{
			na+=(b^c^d)+0xca62c1d6;
		}
		e=d;
		d=c;
		c=(b<<30)|(b>>2);
		b=a;
		a=na;
	}
	h->a+=a;
	h->b+=b;
	h->c+=c;
	h->d+=d;
	h->e+=e;
}



uint8_t _cmp_hash(sha1_data_t* h,const char* s){
	for (uint8_t i=0;i<8;i++){
		uint32_t j=32-((i+1)<<2);
		char a=*(s+i);
		char b=*(s+i+8);
		char c=*(s+i+16);
		char d=*(s+i+24);
		char e=*(s+i+32);
		if (((h->a>>j)&0xf)!=a-(a>47&&a<58?48:(a>64&&a<91?55:87))||((h->b>>j)&0xf)!=b-(b>47&&b<58?48:(b>64&&b<91?55:87))||((h->c>>j)&0xf)!=c-(c>47&&c<58?48:(c>64&&c<91?55:87))||((h->d>>j)&0xf)!=d-(d>47&&d<58?48:(d>64&&d<91?55:87))||((h->e>>j)&0xf)!=e-(e>47&&e<58?48:(e>64&&e<91?55:87))){
			return 0;
		}
	}
	return 1;
}



uint8_t _parse_json_str(json_parser_state_t* p,string_32bit_t* o){
	o->l=1;
	o->v=malloc(sizeof(char));
	*(o->v)=0;
	char c=JSON_PARSER_NEXT_CHAR(p);
	while (c!='\"'){
		if (c!='\\'){
			o->l++;
			o->v=realloc(o->v,o->l*sizeof(char));
			*(o->v+o->l-2)=c;
		}
		else{
			c=JSON_PARSER_NEXT_CHAR(p);
			if (c=='/'||c=='\\'||c=='\''||c=='\"'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=c;
			}
			else if (c=='b'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=8;
			}
			else if (c=='f'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=12;
			}
			else if (c=='n'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=10;
			}
			else if (c=='r'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=13;
			}
			else if (c=='t'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=9;
			}
			else if (c=='v'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=11;
			}
			else if (c=='x'){
				char a=JSON_PARSER_NEXT_CHAR(p);
				char b=JSON_PARSER_NEXT_CHAR(p);
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=((a>47&&a<58?a-48:(a>64&&a<71?a-55:a-87))<<4)|(b>47&&b<58?b-48:(b>64&&b<71?b-55:b-87));
			}
			else{
				printf("Unknown Escape: \\%c\n",c);
				getchar();
				ExitProcess(1);
			}
		}
		c=JSON_PARSER_NEXT_CHAR(p);
	}
	o->l--;
	*(o->v+o->l)=0;
	return 0;
}



uint8_t _parse_json(json_parser_state_t* p,json_object_t* o){
	char c=JSON_PARSER_NEXT_CHAR(p);
	while (c==' '||c=='\t'||c=='\n'||c=='\r'){
		c=JSON_PARSER_NEXT_CHAR(p);
	}
	if (c=='{'){
		o->t=JSON_OBJECT_TYPE_MAP;
		o->dt.m.l=0;
		o->dt.m.dt=NULL;
		while (1){
			while (c!='\"'){
				c=JSON_PARSER_NEXT_CHAR(p);
			}
			o->dt.m.l++;
			o->dt.m.dt=realloc(o->dt.m.dt,o->dt.m.l*sizeof(json_map_keypair_t));
			json_map_keypair_t* k=o->dt.m.dt+o->dt.m.l-1;
			if (_parse_json_str(p,&(k->k))){
				return 1;
			}
			c=JSON_PARSER_NEXT_CHAR(p);
			while (c!=':'){
				c=JSON_PARSER_NEXT_CHAR(p);
			}
			if (_parse_json(p,&(k->v))){
				return 1;
			}
			c=JSON_PARSER_NEXT_CHAR(p);
			while (c!=','){
				if (c=='}'){
					return 0;
				}
				c=JSON_PARSER_NEXT_CHAR(p);
			}
		}
	}
	if (c=='['){
		o->t=JSON_OBJECT_TYPE_ARRAY;
		o->dt.a.l=0;
		o->dt.a.dt=NULL;
		while (1){
			o->dt.a.l++;
			o->dt.a.dt=realloc(o->dt.a.dt,o->dt.a.l*sizeof(json_object_t));
			if (_parse_json(p,o->dt.a.dt+o->dt.a.l-1)){
				return 1;
			}
			c=JSON_PARSER_NEXT_CHAR(p);
			while (c!=','){
				if (c==']'){
					return 0;
				}
				c=JSON_PARSER_NEXT_CHAR(p);
			}
		}
	}
	if (c=='\"'){
		o->t=JSON_OBJECT_TYPE_STRING;
		return _parse_json_str(p,&(o->dt.s));
	}
	if (c=='t'&&_cmp_str_len(*p,"rue",3)){
		(*p)+=3;
		o->t=JSON_OBJECT_TYPE_TRUE;
		return 0;
	}
	if (c=='f'&&_cmp_str_len(*p,"alse",4)){
		(*p)+=4;
		o->t=JSON_OBJECT_TYPE_FALSE;
		return 0;
	}
	if (c=='n'&&_cmp_str_len(*p,"ull",3)){
		(*p)+=3;
		o->t=JSON_OBJECT_TYPE_NULL;
		return 0;
	}
	uint8_t s=1;
	if (c=='-'){
		s=-1;
		c=JSON_PARSER_NEXT_CHAR(p);
	}
	double v=0;
	while (c>47&&c<58){
		v=v*10+(c-48);
		c=JSON_PARSER_NEXT_CHAR(p);
	}
	if (c!='.'&&c!='e'&&c!='E'){
		o->t=JSON_OBJECT_TYPE_INTEGER;
		o->dt.i=(int64_t)(v*s);
		(*p)--;
		return 0;
	}
	if (c=='.'){
		double pw=0.1;
		c=JSON_PARSER_NEXT_CHAR(p);
		while (c>47&&c<58){
			v+=pw*(c-47);
			pw*=0.1;
			c=JSON_PARSER_NEXT_CHAR(p);
		}
	}
	if (c=='e'||c=='E'){
		c=JSON_PARSER_NEXT_CHAR(p);
		int8_t pw_s=1;
		if (c=='+'){
			c=JSON_PARSER_NEXT_CHAR(p);
		}
		else if (c=='-'){
			c=JSON_PARSER_NEXT_CHAR(p);
			pw_s=-1;
		}
		int64_t pw=0;
		while (c>4&&c<58){
			pw=pw*10+(c-48);
			c=JSON_PARSER_NEXT_CHAR(p);
		}
		pw*=pw_s;
		v*=pow(2,(double)pw)*pow(5,(double)pw);
	}
	(*p)--;
	o->t=JSON_OBJECT_TYPE_FLOAT;
	o->dt.f=v*s;
	return 0;
}



json_object_t* _get_by_key(json_object_t* json,const char* k){
	for (uint32_t i=0;i<json->dt.m.l;i++){
		json_map_keypair_t* e=json->dt.m.dt+i;
		if (_cmp_str_len(e->k.v,k,e->k.l+1)){
			return &(e->v);
		}
	}
	return NULL;
}



void _free_json(json_object_t* json){
	if (json->t=JSON_OBJECT_TYPE_STRING){
		if (json->dt.s.v){
			free(json->dt.s.v);
		}
	}
	else if (json->t==JSON_OBJECT_TYPE_ARRAY){
		for (uint32_t i=0;i<json->dt.a.l;i++){
			_free_json(json->dt.a.dt+i);
		}
		if (json->dt.a.dt){
			free(json->dt.a.dt);
		}
	}
	else if (json->t==JSON_OBJECT_TYPE_MAP){
		for (uint32_t i=0;i<json->dt.m.l;i++){
			json_map_keypair_t* e=json->dt.m.dt+i;
			if (e->k.v){
				free(e->k.v);
			}
			_free_json(&(e->v));
		}
		if (json->dt.m.dt){
			free(json->dt.m.dt);
		}
	}
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



void _copy_file(const char* d,const char* s,expand_data_t* e_dt){
	FILE* df=fopen(d,"wb");
	if (!df){
		return;
	}
	FILE* sf=fopen(s,"rb");
	if (!sf){
		fclose(df);
	}
	char c=fgetc(sf);
	while (c!=EOF){
		if (c=='$'){
			char bf[256];
			uint8_t i=0;
			c=fgetc(sf);
			while (c!='$'){
				bf[i]=c;
				i++;
				c=fgetc(sf);
			}
			char* dt=NULL;
			if (i==4&&_cmp_str_len_lower(bf,"name",4)){
				dt=e_dt->nm;
			}
			else if (i==12&&_cmp_str_len_lower(bf,"pretty_title",12)){
				dt=e_dt->pt;
			}
			else if (i==10&&_cmp_str_len_lower(bf,"title_name",10)){
				dt=e_dt->t;
			}
			else if (i==14&&_cmp_str_len_lower(bf,"uppercase_name",14)){
				dt=e_dt->u_nm;
			}
			else if (i==4&&_cmp_str_len_lower(bf,"year",4)){
				dt=e_dt->y;
			}
			if (dt){
				while (*dt){
					fputc(*dt,df);
					dt++;
				}
			}
		}
		else{
			fputc(c,df);
		}
		c=fgetc(sf);
	}
	fclose(df);
	fclose(sf);
}



void _create_program_copy_data(string_8bit_t* s,string_8bit_t* d,const char* nm,expand_data_t* e_dt){
	uint8_t sl=s->l;
	uint8_t dl=d->l;
	s->l+=_copy_str(s->v+s->l,nm);
	d->l+=_copy_str_expand(d->v+d->l,nm,e_dt);
	s->v[s->l]='/';
	d->v[d->l]='/';
	s->l++;
	d->l++;
	s->v[s->l]='*';
	s->v[s->l+1]=0;
	d->v[d->l]=0;
	CreateDirectoryA(d->v,NULL);
	WIN32_FIND_DATAA dt;
	HANDLE fh=FindFirstFileA(s->v,&dt);
	if (fh!=INVALID_HANDLE_VALUE){
		do{
			if (!(dt.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){
				s->v[_copy_str(s->v+s->l,dt.cFileName)+s->l]=0;
				d->v[_copy_str_expand(d->v+d->l,dt.cFileName,e_dt)+d->l]=0;
				_copy_file(d->v,s->v,e_dt);
				if (*(dt.cFileName)=='.'){
					SetFileAttributesA(s->v,FILE_ATTRIBUTE_HIDDEN);
				}
			}
			else{
				if (*(dt.cFileName)=='.'&&(*(dt.cFileName+1)==0||(*(dt.cFileName+1)=='.'&&*(dt.cFileName+2)==0))){
					continue;
				}
				_create_program_copy_data(s,d,dt.cFileName,e_dt);
			}
		} while (FindNextFileA(fh,&dt));
		FindClose(fh);
	}
	s->l=sl;
	d->l=dl;
}



uint8_t _open_first(string_8bit_t* r,const char* ext,char* bf,uint16_t bfi){
	WIN32_FIND_DATAA dt;
	r->v[r->l]='*';
	r->v[r->l+1]=0;
	HANDLE fh=FindFirstFileA(r->v,&dt);
	if (fh!=INVALID_HANDLE_VALUE){
		do{
			if (dt.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
				if (*(dt.cFileName)=='.'&&(*(dt.cFileName+1)==0||(*(dt.cFileName+1)=='.'&&*(dt.cFileName+2)==0))){
					continue;
				}
				uint8_t rl=r->l;
				r->l+=_copy_str(r->v+r->l,dt.cFileName);
				r->v[r->l]='/';
				r->l++;
				if (_open_first(r,ext,bf,bfi)){
					return 1;
				}
				r->l=rl;
			}
			else{
				r->v[r->l]=0;
				uint8_t i=0;
				uint8_t j=0;
				while (*(dt.cFileName+i)){
					if (*(dt.cFileName+i)=='.'){
						j=i+1;
					}
					i++;
				}
				if (_cmp_str_len(ext,dt.cFileName+j,i-j+1)){
					bfi+=_copy_str(bf+bfi,r->v);
					bf[bfi+_copy_str(bf+bfi,dt.cFileName)]=0;
					_create_process(bf);
					return 1;
				}
			}
		} while (FindNextFileA(fh,&dt));
		FindClose(fh);
	}
	return 0;
}



void _create_program(const string_8bit_t* t,const string_8bit_t* nm,uint8_t fl){
	string_8bit_t p;
	string_8bit_t tp;
	if (!t){
		p.l=_copy_str(p.v,__FILE_BASE_DIR__"/");
		tp.l=_copy_str(tp.v,TEMPLATES_FILE_PATH"/c/");
	}
	else{
		p.l=_copy_str(p.v,PROJECT_DIR"/");
		p.l+=_str_title_case(p.v+p.l,t->v,t->l);
		p.v[p.l]='-';
		p.l++;
		p.l+=_str_title_case(p.v+p.l,nm->v,nm->l);
		p.v[p.l]='/';
		p.l++;
		tp.l=_copy_str(tp.v,TEMPLATES_FILE_PATH"/");
		for (uint8_t i=0;i<t->l;i++){
			tp.v[tp.l]=*(t->v+i);
			tp.l++;
		}
		tp.v[tp.l]='/';
		tp.l++;
		tp.v[tp.l]=0;
		if (GetFileAttributesA(tp.v)==INVALID_FILE_ATTRIBUTES){
			return;
		}
	}
	p.v[p.l]=0;
	if (GetFileAttributesA(p.v)==INVALID_FILE_ATTRIBUTES){
		fl|=FLAG_INITIALIZE;
		CreateDirectoryA(p.v,NULL);
	}
	tp.v[tp.l]='*';
	tp.v[tp.l+1]=0;
	WIN32_FIND_DATAA dt;
	HANDLE fh=FindFirstFileA(tp.v,&dt);
	expand_data_t e_dt;
	_generate_expand_data(&e_dt,(t?t->v:"Python"),(nm?nm->v:"Boot_Program"));
	uint16_t pl=p.l;
	if (fh!=INVALID_HANDLE_VALUE){
		do{
			if (!(dt.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){
				p.v[_copy_str_expand(p.v+p.l,dt.cFileName,&e_dt)+p.l]=0;
				if (GetFileAttributesA(p.v)==INVALID_FILE_ATTRIBUTES){
					tp.v[_copy_str(tp.v+tp.l,dt.cFileName)+tp.l]=0;
					_copy_file(p.v,tp.v,&e_dt);
					if (*(dt.cFileName)=='.'){
						SetFileAttributesA(p.v,FILE_ATTRIBUTE_HIDDEN);
					}
				}
			}
			else if (fl&FLAG_INITIALIZE){
				if (*(dt.cFileName)=='.'&&(*(dt.cFileName+1)==0||(*(dt.cFileName+1)=='.'&&*(dt.cFileName+2)==0))){
					continue;
				}
				_create_program_copy_data(&tp,&p,dt.cFileName,&e_dt);
			}
		} while (FindNextFileA(fh,&dt));
		FindClose(fh);
	}
	p.v[pl]=0;
	if (!(fl&FLAG_OPEN)){
		return;
	}
	char bf[4096]="\""EDITOR_FILE_PATH"\" ";
	uint16_t i=0;
	while (bf[i]){
		i++;
	}
	bf[i]='-';
	bf[i+1]='-';
	bf[i+2]='a';
	bf[i+3]='d';
	bf[i+4]='d';
	bf[i+5]=' ';
	bf[i+6]='\"';
	uint16_t j=i+7;
	j+=_copy_str(bf+j,p.v);
	bf[j]='\"';
	bf[j+1]=0;
	_create_process(bf);
	char* ext=NULL;
	char* fp=NULL;
	if (_cmp_str_len_lower(t->v,"arduino",7)){
		ext="ino";
		fp="src/main.ino";
	}
	else if (_cmp_str_len_lower(t->v,"assembly",8)){
		ext="asm";
		fp="src/main.asm";
	}
	else if (_cmp_str_len_lower(t->v,"c",1)){
		ext="c";
		fp="src/main.c";
	}
	else if (_cmp_str_len_lower(t->v,"cpp",3)){
		ext="cpp";
		fp="src/main.cpp";
	}
	else if (_cmp_str_len_lower(t->v,"css",3)){
		ext="css";
		fp="src/style/main.css";
	}
	else if (_cmp_str_len_lower(t->v,"java",4)){
		ext="java";
		char tmp[4096]="src/com/krzem/";
		j=0;
		while (tmp[j]){
			j++;
		}
		j+=_copy_str(tmp+j,e_dt.nm);
		tmp[j+_copy_str(tmp+j,"/Main.java")]=0;
		fp=tmp;
	}
	else if (_cmp_str_len_lower(t->v,"js",2)){
		ext="js";
		fp="src/script/main.js";
	}
	else if (_cmp_str_len_lower(t->v,"php",3)){
		ext="php";
		fp="src/main.php";
	}
	else if (_cmp_str_len_lower(t->v,"pde",3)){
		ext="pde";
		fp="src/main.pde";
	}
	else{
		ext="py";
		fp="src/main.py";
	}
	bf[i]='\"';
	i++;
	j=i+_copy_str(bf+i,p.v);
	j+=_copy_str(bf+j,fp);
	bf[j]='\"';
	bf[j+1]=0;
	char tmp[4096]={0};
	_copy_str(tmp+_copy_str(tmp,p.v),fp);
	if (GetFileAttributesA(tmp)!=INVALID_FILE_ATTRIBUTES){
		_create_process(bf);
	}
	else{
		_open_first(&p,ext,bf,i);
	}
}



uint8_t _request(const char* s,const char* m,const char* p,string_32bit_t* o){
	wchar_t* ws=_expand_to_wide(s);
	wchar_t* wm=_expand_to_wide(m);
	wchar_t* wp=_expand_to_wide(p);
	HINTERNET sh=NULL;
	HINTERNET ch=NULL;
	HINTERNET rh=NULL;
	o->v=malloc(1);
	if (!(sh=WinHttpOpen(USER_AGENT_STRING,WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS,0))){
		goto _error;
	}
	if (!(ch=WinHttpConnect(sh,ws,INTERNET_DEFAULT_HTTPS_PORT,0))){
		goto _error;
	}
	free(ws);
	ws=NULL;
	if (!(rh=WinHttpOpenRequest(ch,wm,wp,NULL,WINHTTP_NO_REFERER,WINHTTP_DEFAULT_ACCEPT_TYPES,WINHTTP_FLAG_SECURE))){
		goto _error;
	}
	free(wm);
	free(wp);
	wm=NULL;
	wp=NULL;
	if (!WinHttpSendRequest(rh,WINHTTP_NO_ADDITIONAL_HEADERS,0,WINHTTP_NO_REQUEST_DATA,0,0,0)||!WinHttpReceiveResponse(rh,NULL)){
		goto _error;
	}
	o->l=0;
	while (1){
		uint32_t sz;
		if (!WinHttpQueryDataAvailable(rh,&sz)){
			goto _error;
		}
		if (!sz){
			break;
		}
		o->l+=sz;
		o->v=realloc(o->v,(o->l+1)*sizeof(char));
		uint32_t tmp;
		if (!WinHttpReadData(rh,(LPVOID)(o->v+o->l-sz),sz,&tmp)){
			goto _error;
		}
	}
	*(o->v+o->l)=0;
	WinHttpCloseHandle(sh);
	WinHttpCloseHandle(ch);
	WinHttpCloseHandle(rh);
	return 1;
_error:
	if (wm){
		free(wm);
	}
	if (ws){
		free(ws);
	}
	if (wp){
		free(wp);
	}
	if (sh){
		WinHttpCloseHandle(sh);
	}
	if (ch){
		WinHttpCloseHandle(ch);
	}
	if (rh){
		WinHttpCloseHandle(rh);
	}
	if (o->v){
		free(o->v);
		o->v=NULL;
	}
	o->l=0;
	return 0;
}



uint8_t _request_url(const char* m,const char* url,string_32bit_t* o){
	if (_cmp_str_len(url,"http://",7)){
		url+=7;
	}
	else if (_cmp_str_len(url,"https://",8)){
		url+=8;
	}
	uint32_t i=0;
	while (*(url+i)!='/'){
		i++;
	}
	char* s=malloc((i+1)*sizeof(char));
	for (uint32_t j=0;j<i;j++){
		*(s+j)=*(url+j);
	}
	*(s+i)=0;
	uint8_t rc=_request(s,m,url+i,o);
	free(s);
	return rc;
}



uint8_t _download_url(const char* url,FILE* o,uint32_t t_sz){
	if (_cmp_str_len(url,"http://",7)){
		url+=7;
	}
	else if (_cmp_str_len(url,"https://",8)){
		url+=8;
	}
	uint32_t i=0;
	while (*(url+i)!='/'){
		i++;
	}
	wchar_t* ws=malloc((i+1)*sizeof(wchar_t));
	for (uint32_t j=0;j<i;j++){
		*(ws+j)=*(url+j);
	}
	*(ws+i)=0;
	url+=i;
	i=0;
	while (*(url+i)){
		i++;
	}
	wchar_t* wp=malloc((i+1)*sizeof(wchar_t));
	i=0;
	while (*url){
		*(wp+i)=*url;
		url++;
		i++;
	}
	*(wp+i)=0;
	HINTERNET sh=NULL;
	HINTERNET ch=NULL;
	HINTERNET rh=NULL;
	if (!(sh=WinHttpOpen(USER_AGENT_STRING,WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS,0))){
		goto _error;
	}
	if (!(ch=WinHttpConnect(sh,ws,INTERNET_DEFAULT_HTTPS_PORT,0))){
		goto _error;
	}
	free(ws);
	ws=NULL;
	if (!(rh=WinHttpOpenRequest(ch,L"GET",wp,NULL,WINHTTP_NO_REFERER,WINHTTP_DEFAULT_ACCEPT_TYPES,WINHTTP_FLAG_SECURE))){
		goto _error;
	}
	free(wp);
	wp=NULL;
	if (!WinHttpSendRequest(rh,WINHTTP_NO_ADDITIONAL_HEADERS,0,WINHTTP_NO_REQUEST_DATA,0,0,0)||!WinHttpReceiveResponse(rh,NULL)){
		goto _error;
	}
	uint8_t lp=0;
	char bf[HTTP_REQUEST_BUFFER_SIZE];
	uint32_t t=0;
	if (t_sz){
		PRINTF_TIME("\x1b[38;2;100;100;100m0%% Downloaded...\n");
	}
	while (1){
		uint32_t sz;
		if (!WinHttpQueryDataAvailable(rh,&sz)){
			goto _error;
		}
		if (!sz){
			break;
		}
		if (sz>HTTP_REQUEST_BUFFER_SIZE){
			sz=HTTP_REQUEST_BUFFER_SIZE;
		}
		t+=sz;
		if (t_sz&&t*100/t_sz>lp){
			lp=t*100/t_sz;
			PRINTF_TIME("\x1b[38;2;100;100;100m%u%% Downloaded...\n",lp);
		}
		uint32_t tmp;
		if (!WinHttpReadData(rh,(LPVOID)bf,sz,&tmp)||fwrite(bf,sizeof(char),sz,o)!=sz){
			goto _error;
		}
	}
	if (t_sz){
		PRINTF_TIME("\x1b[38;2;100;100;100m100%% Downloaded...\n");
	}
	WinHttpCloseHandle(sh);
	WinHttpCloseHandle(ch);
	WinHttpCloseHandle(rh);
	return 1;
_error:
	if (ws){
		free(ws);
	}
	if (wp){
		free(wp);
	}
	if (sh){
		WinHttpCloseHandle(sh);
	}
	if (ch){
		WinHttpCloseHandle(ch);
	}
	if (rh){
		WinHttpCloseHandle(rh);
	}
	return 0;
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
					case '1':
					case '2':
					case '3':
						{
							char tmp[4096];
							uint16_t i=0;
							for (;i<fp_bfl;i++){
								tmp[i]=fp_bf[i];
							}
							tmp[i]='8';
							tmp[i+1]=' ';
							tmp[i+2]=(char)(dt->vkCode-1);
							tmp[i+3]=0;
							_create_process(tmp);
							break;
						}
					case 'I':
					case 'Q':
					case 'R':
						{
							char tmp[4096];
							uint16_t i=0;
							for (;i<fp_bfl;i++){
								tmp[i]=fp_bf[i];
							}
							tmp[i]=(dt->vkCode=='I'?'7':(dt->vkCode=='Q'?'1':'0'));
							tmp[i+1]=0;
							_create_process(tmp);
							break;
						}
					case 'W':
						ShellExecuteW(NULL,L"open",WCHAR_STR(ROOT_FILE_PATH),NULL,NULL,SW_SHOWMAXIMIZED);
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
	uint8_t fl=0;
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
				_switch_to_desktop(0);
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
						int k=_getch();
						if (IS_GETCH_DOUBLE_KEY(k)){
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
							if (bfl<UINT8_MAX){
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
		case 2:
			{
				SetFocus(_console());
				SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),128);
				HANDLE ho=GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleMode(ho,7);
				CONSOLE_SCREEN_BUFFER_INFO sbi;
				GetConsoleScreenBufferInfo(ho,&sbi);
				CONSOLE_CURSOR_INFO ci;
				GetConsoleCursorInfo(ho,&ci);
				ci.bVisible=0;
				SetConsoleCursorInfo(ho,&ci);
				program_type_t tl[256];
				uint8_t tll=0;
				WIN32_FIND_DATAA dt;
				HANDLE fh=FindFirstFileA(PROJECT_DIR"/*",&dt);
				if (fh!=INVALID_HANDLE_VALUE){
					do{
						if (*(dt.cFileName)=='.'){
							continue;
						}
						uint8_t i=0;
						while (*(dt.cFileName+i)!='-'){
							if (!(*(dt.cFileName+i))){
								goto _nxt_dir;
							}
							i++;
						}
						char* nm=dt.cFileName+i+1;
						uint8_t sz=0;
						while (*(nm+sz)){
							sz++;
						}
						for (uint8_t j=0;j<tll;j++){
							if (tl[j].l!=i||!_cmp_str_len_lower(tl[j].v,dt.cFileName,i)){
								continue;
							}
							tl[j].el++;
							void* tmp=realloc(tl[j].e,tl[j].el*sizeof(string_8bit_t));
							if (!tmp){
								return 1;
							}
							tl[j].e=tmp;
							(tl[j].e+tl[j].el-1)->l=sz;
							char* d=(tl[j].e+tl[j].el-1)->v;
							for (uint8_t k=0;k<sz+1;k++){
								*(d+k)=*(nm+k);
							}
							goto _nxt_dir;
						}
						tl[tll].l=i;
						*(tl[tll].v+i)=0;
						while (i){
							i--;
							*(tl[tll].v+i)=*(dt.cFileName+i);
						}
						tl[tll].el=1;
						tl[tll].e=malloc(sizeof(string_8bit_t));
						tl[tll].e->l=sz;
						for (uint8_t k=0;k<sz+1;k++){
							*(tl[tll].e->v+k)=*(nm+k);
						}
						tll++;
_nxt_dir:;
					} while (FindNextFileA(fh,&dt));
					FindClose(fh);
				}
				const char* b_str="Boot";
				if (!tll){
					tl[0].l=4;
					tl[0].e=0;
					for (uint8_t i=0;i<5;i++){
						*(tl[0].v+i)=*(b_str+i);
					}
				}
				else{
					uint8_t i=tll-1;
					while (i){
						uint8_t cmp=1;
						uint8_t j=0;
						for (;j<tl[i].l;j++){
							char b=*(tl[i].v+j);
							if (cmp&&j<4){
								char a=*(b_str+j);
								if ((!a&&!b)||a>b){
									goto _insert_elem;
								}
								if (a<b){
									cmp=0;
								}
							}
							*(tl[i+1].v+j)=b;
						}
						*(tl[i+1].v+j)=0;
						tl[i+1].l=tl[i].l;
						tl[i+1].el=tl[i].el;
						tl[i+1].e=tl[i].e;
						i--;
					}
_insert_elem:;
					tl[i+1].l=4;
					tl[i+1].el=0;
					for (uint8_t j=0;j<5;j++){
						*(tl[i+1].v+j)=*(b_str+j);
					}
				}
				tll++;
				uint8_t fl=FLAG_UPDATE|FLAG_EDIT_TYPE;
				uint16_t ll=0;
				string_8bit_t p_t={
					0,
					{
						0
					}
				};
				string_8bit_t p_nm={
					0,
					{
						0
					}
				};
				while (1){
					if (_kbhit()){
						int k=_getch();
						if (IS_GETCH_DOUBLE_KEY(k)){
							k=CREATE_UPPER_KEY(_getch());
						}
						if (k==VK_CANCEL){
							break;
						}
						else if (fl&FLAG_ASK_CREATE){
							if (k=='Y'||k=='y'){
								_create_program(&p_t,&p_nm,FLAG_OPEN);
								break;
							}
							fl^=FLAG_UPDATE|FLAG_ASK_CREATE;
						}
						else if (k==VK_BACK){
							if (fl&FLAG_EDIT_TYPE){
								if (p_t.l){
									p_t.l--;
									p_t.v[p_t.l]=0;
									fl|=FLAG_UPDATE;
								}
							}
							else{
								if (p_nm.l){
									p_nm.l--;
									p_nm.v[p_nm.l]=0;
									fl|=FLAG_UPDATE;
								}
							}
						}
						else if (k==VK_TAB){
							program_type_t pt={0};
							if (!p_t.l){
								for (uint8_t i=0;i<tl[0].l+1;i++){
									*(p_t.v+i)=*(tl[0].v+i);
								}
								p_t.l=tl[0].l;
								fl|=FLAG_UPDATE;
							}
							else{
								for (uint8_t i=0;i<tll;i++){
									if (tl[i].l>=p_t.l&&_cmp_str_len_lower(tl[i].v,p_t.v,p_t.l)){
										if (tl[i].l==p_t.l){
											pt=tl[i];
										}
										if (fl&FLAG_EDIT_TYPE){
											for (uint8_t j=0;j<tl[i].l+1;j++){
												*(p_t.v+j)=*(tl[i].v+j);
											}
											p_t.l=tl[i].l;
											fl|=FLAG_UPDATE;
										}
										break;
									}
								}
							}
							if (pt.l&&!(fl&FLAG_EDIT_TYPE)){
								if (!p_nm.l&&pt.el){
									for (uint8_t i=0;i<pt.e->l+1;i++){
										*(p_nm.v+i)=*(pt.e->v+i);
									}
									p_nm.l=pt.e->l;
									fl|=FLAG_UPDATE;
								}
								else{
									for (uint16_t i=0;i<pt.el;i++){
										if ((pt.e+i)->l>=p_nm.l&&_cmp_str_len_lower((pt.e+i)->v,p_nm.v,p_nm.l)){
											for (uint8_t j=0;j<(pt.e+i)->l+1;j++){
												*(p_nm.v+j)=*((pt.e+i)->v+j);
											}
											p_nm.l=(pt.e+i)->l;
											fl|=FLAG_UPDATE;
											break;
										}
									}
								}
							}
						}
						else if (k==VK_RETURN){
							if (p_t.l==4&&_cmp_str_len_lower(p_t.v,b_str,4)){
								_create_program(NULL,NULL,FLAG_OPEN);
								break;
							}
							for (uint8_t i=0;i<tll;i++){
								program_type_t e=tl[i];
								if (e.l!=p_t.l||!_cmp_str_len_lower(e.v,p_t.v,p_t.l)){
									continue;
								}
								for (uint16_t j=0;j<e.el;j++){
									if ((e.e+j)->l!=p_nm.l||!_cmp_str_len_lower((e.e+j)->v,p_nm.v,p_nm.l)){
										continue;
									}
									_create_program(&p_t,&p_nm,FLAG_OPEN);
									goto _cleanup_project_list;
								}
								fl|=FLAG_UPDATE|FLAG_ASK_CREATE;
								break;
							}
						}
						else if (k=='-'){
							fl^=FLAG_UPDATE|FLAG_EDIT_TYPE;
						}
						else if ((k>47&&k<58)||(k>64&&k<91)||k=='_'||(k>96&&k<123)){
							if (fl&FLAG_EDIT_TYPE){
								if (p_t.l<UINT8_MAX){
									p_t.v[p_t.l]=k;
									p_t.l++;
									p_t.v[p_t.l]=0;
									fl|=FLAG_UPDATE;
								}
							}
							else{
								if (p_nm.l<UINT8_MAX){
									p_nm.v[p_nm.l]=k;
									p_nm.l++;
									p_nm.v[p_nm.l]=0;
									fl|=FLAG_UPDATE;
								}
							}
						}
						else if (k==GETCH_DEL){
							if (fl&FLAG_EDIT_TYPE){
								p_t.l=0;
								*(p_t.v)=0;
							}
							else{
								p_nm.l=0;
								*(p_nm.v)=0;
							}
							fl|=FLAG_UPDATE;
						}
					}
					if (fl&FLAG_UPDATE){
						char* pr_t="";
						uint8_t pr_tl=0;
						program_type_t pt={0};
						if (!p_t.l){
							pr_t=tl[0].v;
							pr_tl=tl[0].l;
						}
						else{
							for (uint8_t i=0;i<tll;i++){
								if (tl[i].l>=p_t.l&&_cmp_str_len_lower(tl[i].v,p_t.v,p_t.l)){
									if (tl[i].l==p_t.l){
										pt=tl[i];
									}
									pr_t=tl[i].v+p_t.l;
									pr_tl=tl[i].l-p_t.l;
									break;
								}
							}
						}
						char* pr_nm="";
						uint8_t pr_nml=0;
						if (pt.l){
							if (!p_nm.l&&pt.el){
								pr_nm=pt.e->v;
								pr_nml=pt.e->l;
							}
							else{
								for (uint16_t i=0;i<pt.el;i++){
									if ((pt.e+i)->l>=p_nm.l&&_cmp_str_len_lower((pt.e+i)->v,p_nm.v,p_nm.l)){
										pr_nm=(pt.e+i)->v+p_nm.l;
										pr_nml=(pt.e+i)->l-p_nm.l;
										break;
									}
								}
							}
						}
						fl&=~FLAG_UPDATE;
						printf("\x1b[0;0H\x1b[38;2;98;145;22mProject\x1b[38;2;59;59;59m: \x1b[38;2;255;255;255m%s\x1b[38;2;139;139;139m%s\x1b[38;2;59;59;59m-\x1b[38;2;255;255;255m%s\x1b[38;2;139;139;139m%s",p_t.v,pr_t,p_nm.v,pr_nm);
						uint16_t ln=10+p_t.l+pr_tl+p_nm.l+pr_nml;
						if (fl&FLAG_ASK_CREATE){
							printf("\n\x1b[38;2;50;155;204mCreate Project?");
							ln+=sbi.dwMaximumWindowSize.X+16;
						}
						while (ll>ln){
							ll--;
							putchar(' ');
						}
						ll=ln;
					}
				}
_cleanup_project_list:
				for (uint8_t i=0;i<tll;i++){
					if (tl[i].el){
						free(tl[i].e);
					}
				}
				return 0;
			}
		case 4:
			_move_to_desktop(_console(),2);
			for (uint32_t i=0;i<argc;i++){
				printf("%u: %s\n",i,argv[i]);
			}
			getchar();
			return 0;
		case 7:
			{
				_move_to_desktop(_console(),2);
				PRINTF_TIME("Starting Minecraft Server in Folder \x1b[38;2;91;216;38m'%s'\x1b[38;2;100;100;100m...\n",MINECRAFT_SERVER_FOLDER);
				if (GetFileAttributesA(MINECRAFT_SERVER_FOLDER)==INVALID_FILE_ATTRIBUTES){
					PRINTF_TIME("\x1b[38;2;200;40;20mMinecraft Server Folder Missing.\x1b[0m Quitting\x1b[38;2;100;100;100m...\n");
					getchar();
					return 1;
				}
				PRINTF_TIME("Downloading Metadata\x1b[38;2;100;100;100m...\n");
				json_object_t json={
					JSON_OBJECT_TYPE_NULL
				};
				string_32bit_t dt;
				if (!_request("launchermeta.mojang.com","GET","/mc/game/version_manifest.json",&dt)){
					goto _skip_update;
				}
				json_parser_state_t p=dt.v;
				if (_parse_json(&p,&json)){
					free(dt.v);
					goto _skip_update;
				}
				free(dt.v);
				string_32bit_t url=_get_by_key(_get_by_key(&json,"versions")->dt.a.dt,"url")->dt.s;
				PRINTF_TIME("Downloading Release Data From URL \x1b[38;2;91;216;38m'%s'\x1b[38;2;100;100;100m...\n",url.v);
				if (!_request_url("GET",url.v,&dt)){
					goto _skip_update;
				}
				_free_json(&json);
				json.t=JSON_OBJECT_TYPE_NULL;
				p=dt.v;
				if (_parse_json(&p,&json)){
					free(dt.v);
					goto _skip_update;
				}
				free(dt.v);
				json_object_t* s_dt=_get_by_key(_get_by_key(&json,"downloads"),"server");
				uint32_t n_sz=(uint32_t)_get_by_key(s_dt,"size")->dt.i;
				if (GetFileAttributesA(MINECRAFT_SERVER_FOLDER"server.jar")!=INVALID_FILE_ATTRIBUTES){
					PRINTF_TIME("Inspecting Current Version\x1b[38;2;100;100;100m...\n");
					HANDLE fh=CreateFileA(MINECRAFT_SERVER_FOLDER"server.jar",GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
					uint32_t sz=GetFileSize(fh,NULL);
					CloseHandle(fh);
					PRINTF_TIME("File Size: \x1b[38;2;48;109;206m%u\x1b[0m, New Size: \x1b[38;2;48;109;206m%u\n",sz,n_sz);
					if (sz==n_sz){
						uint32_t i=sz;
						uint8_t bf[64];
						sha1_data_t h=SHA1_DATA_INIT;
						uint8_t bfl;
						uint8_t lp=0;
						FILE* f=fopen(MINECRAFT_SERVER_FOLDER"server.jar","rb");
						PRINTF_TIME("\x1b[38;2;100;100;100m0%% Hashed...\n");
						while (1){
							uint32_t j=(i>64?64:i);
							if (fread(bf,sizeof(uint8_t),j,f)!=j){
								fclose(f);
								goto _skip_update;
							}
							if (j!=64){
								bfl=j;
								break;
							}
							_sha1_chunk(&h,bf);
							i-=64;
							if (!i){
								bfl=0;
							}
							if ((sz-i)*100/sz>lp){
								lp=(sz-i)*100/sz;
								PRINTF_TIME("\x1b[38;2;100;100;100m%u%% Hashed...\n",lp);
							}
						}
						fclose(f);
						bf[bfl]=0x80;
						bfl++;
						if (bfl>=56){
							while (bfl<64){
								bf[bfl]=0;
								bfl++;
							}
							_sha1_chunk(&h,bf);
							bfl=0;
						}
						while (bfl<56){
							bf[bfl]=0;
							bfl++;
						}
						uint64_t v=sz<<3;
						for (uint8_t j=63;j>=56;j--){
							bf[j]=v&0xff;
							v>>=8;
						}
						_sha1_chunk(&h,bf);
						PRINTF_TIME("\x1b[38;2;100;100;100m100%% Hashed...\n");
						if (_cmp_hash(&h,_get_by_key(s_dt,"sha1")->dt.s.v)){
							goto _skip_update;
						}
					}
				}
				string_32bit_t id=_get_by_key(&json,"id")->dt.s;
				url=_get_by_key(s_dt,"url")->dt.s;
				PRINTF_TIME("Downloading Server For \x1b[38;2;48;109;206m%s\x1b[0m (\x1b[38;2;91;216;38m'%s'\x1b[0m)\x1b[38;2;100;100;100m...\n",id.v,url.v);
				FILE* f=fopen(MINECRAFT_SERVER_FOLDER"server.jar","wb");
				_download_url(url.v,f,n_sz);
				fclose(f);
_skip_update:;
				_free_json(&json);
				printf("\x1b[0m");
				CONSOLE_SCREEN_BUFFER_INFO sbi;
				HANDLE ho=GetStdHandle(STD_OUTPUT_HANDLE);
				GetConsoleScreenBufferInfo(ho,&sbi);
				DWORD tmp;
				COORD z={
					0,
					0
				};
				FillConsoleOutputCharacterA(ho,' ',sbi.dwSize.X*sbi.dwSize.Y,z,&tmp);
				FillConsoleOutputAttribute(ho,7,sbi.dwSize.X*sbi.dwSize.Y,z,&tmp);
				SetConsoleCursorPosition(ho,z);
				STARTUPINFOA si={
					sizeof(STARTUPINFOA)
				};
				PROCESS_INFORMATION pi;
				CreateProcessA(NULL,"\""MINECRAFT_JAVA_RUNTIME_FILE_PATH"\" -Xms"MINECRAFT_JAVA_RUNTIME_MEMORY" -Xmx"MINECRAFT_JAVA_RUNTIME_MEMORY" -jar \""MINECRAFT_SERVER_FOLDER"server.jar\" --nogui",NULL,NULL,FALSE,0,NULL,MINECRAFT_SERVER_FOLDER,&si,&pi);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				return 0;
			}
		case 8:
			return _switch_to_desktop(argv[2][0]-48);
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
