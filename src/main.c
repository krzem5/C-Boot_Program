#ifndef _MSC_VER
#error Non-Windows Platforms not Supported!
#endif
#define WIN32_LEAN_AND_MEAN 1
#define _CRT_SECURE_NO_WARNINGS 1
#include <windows.h>
#include <shellapi.h>
#include <shellscalingapi.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <main.h>
#include <math.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



uint8_t _handle_macro_ig_alt=0;
char fp_bf[4096];
uint32_t fp_bfl;
WSADATA _ws_dt;
char _gh_token[256]={0};



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
				o+=_copy_str(d+o,e_dt->e.y);
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
		o->u_nm[i]=(c>96&&c<123?c-32:c);
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
		o->e.y[n]=bf[m];
		n++;
	}
	o->e.y[n]=0;
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



void _parse_json_str(json_parser_state_t* p,string_32bit_t* o){
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
			_parse_json_str(p,&(k->k));
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
		_parse_json_str(p,&(o->dt.s));
		return 0;
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
				dt=e_dt->e.y;
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



void _create_project_copy_data(string_8bit_t* s,string_8bit_t* d,const char* nm,expand_data_t* e_dt){
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
				_create_project_copy_data(s,d,dt.cFileName,e_dt);
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



void _create_project(const string_8bit_t* t,const string_8bit_t* nm,uint8_t fl){
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
				_create_project_copy_data(&tp,&p,dt.cFileName,&e_dt);
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



char* _request_url(const char* m,const char* url,const string_32bit_t* dt,uint8_t fl){
	if (_cmp_str_len(url,"http://",7)){
		url+=7;
	}
	else if (_cmp_str_len(url,"https://",8)){
		url+=8;
	}
	char bf[4096];
	uint16_t i=0;
	while (*(m+i)){
		bf[i]=*(m+i);
		i++;
	}
	bf[i]=' ';
	i++;
	char s[256];
	uint16_t j=0;
	while (*(url+j)!='/'){
		s[j]=*(url+j);
		j++;
	}
	s[j]=0;
	i+=_copy_str(bf+i,url+j);
	i+=_copy_str(bf+i," HTTP/1.1\r\nUser-Agent:"USER_AGENT_STRING"\r\nHost:");
	i+=_copy_str(bf+i,s);
	if (dt){
		i+=_copy_str(bf+i,"\r\nContent-Length:");
		char tmp[10];
		j=0;
		uint32_t v=dt->l;
		do{
			tmp[j]=v%10;
			v/=10;
			j++;
		} while(v);
		while (j){
			j--;
			bf[i]=tmp[j]+48;
			i++;
		}
	}
	if (fl&FLAG_ACCEPT_JSON){
		i+=_copy_str(bf+i,"\r\nAccept:application/json");
	}
	if (fl&FLAG_ACCEPT_GITHUB){
		i+=_copy_str(bf+i,"\r\nAccept:"GITHUB_HEADERS);
	}
	if (fl&FLAG_GITHUB_TOKEN){
		i+=_copy_str(bf+i,"\r\nAuthorization:token ");
		if (!_gh_token[0]){
			FILE* f=fopen(__FILE_BASE_DIR__"/data/github-secret.dt","rb");
			_gh_token[fread(_gh_token,sizeof(char),256,f)]=0;
			fclose(f);
			j=0;
			while (_gh_token[j]>31&&_gh_token[j]<127){
				j++;
			}
			_gh_token[j]=0;
		}
		i+=_copy_str(bf+i,_gh_token);
	}
	i+=_copy_str(bf+i,"\r\n\r\n");
	ADDRINFOA h={
		0,
		AF_UNSPEC,
		SOCK_STREAM,
		IPPROTO_TCP,
		0,
		NULL,
		NULL,
		NULL
	};
	ADDRINFOA* addr=NULL;
	if (getaddrinfo(s,"443",&h,&addr)){
		return NULL;
	}
	SOCKET sck;
	ADDRINFOA* c=addr;
	do{
		sck=socket(c->ai_family,c->ai_socktype,c->ai_protocol);
		if (sck==INVALID_SOCKET){
			freeaddrinfo(addr);
			return NULL;
		}
		if (connect(sck,c->ai_addr,(int)c->ai_addrlen)!=SOCKET_ERROR){
			break;
		}
		closesocket(sck);
		sck=INVALID_SOCKET;
		c=c->ai_next;
	} while (c);
	freeaddrinfo(addr);
	if (!c){
		return NULL;
	}
	SSL_CTX* ctx=SSL_CTX_new(TLS_client_method());
	if (!ctx){
		closesocket(sck);
		printf("%u\n",__LINE__);
		return NULL;
	}
	SSL* ssl=SSL_new(ctx);
	if (!ssl){
		closesocket(sck);
		printf("%u\n",__LINE__);
		return NULL;
	}
	SSL_set_fd(ssl,(int)sck);
	if (SSL_connect(ssl)!=1){
		closesocket(sck);
		return NULL;
	}
	SSL_write(ssl,bf,i);
	if (dt&&dt->l){
		SSL_write(ssl,dt->v,dt->l);
	}
	char o_bf[HTTP_REQUEST_BUFFER_SIZE];
	size_t o_bf_sz=0;
	SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
	i=0;
	uint8_t st=0;
	char hk[4096];
	uint16_t hkl=0;
	char hv[4096];
	uint16_t hvl=0;
	uint32_t ln=UINT32_MAX;
	while (1){
		if (i==o_bf_sz){
			SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
			i=0;
		}
		char c=o_bf[i];
		i++;
		if (c=='\r'||c=='\n'){
			if (st>4){
				if (hkl==17&&_cmp_str_len_lower(hk,"transfer-encoding",17)){
					if (hvl==7&&_cmp_str_len_lower(hv,"chunked",7)){
						fl|=FLAG_CHUNKED_TRANSFER;
					}
					else{
						fl&=~FLAG_CHUNKED_TRANSFER;
					}
				}
				else if (hkl==14&&_cmp_str_len_lower(hk,"content-length",14)){
					ln=0;
					for (uint16_t k=0;k<hvl;k++){
						ln=ln*10+(hv[k]-48);
					}
				}
				hkl=0;
				hvl=0;
				st=1;
			}
			else{
				st++;
				if (st==4){
					break;
				}
			}
		}
		else if (st==2){
			hk[0]=c;
			hkl=1;
			st=0;
		}
		else if (st==5&&(c==' '||c=='\t')){
			continue;
		}
		else if (st>4){
			st=6;
			hv[hvl]=c;
			hvl++;
		}
		else if (c==':'){
			st=5;
		}
		else{
			hk[hkl]=c;
			hkl++;
		}
	}
	char* o;
	if (fl&FLAG_CHUNKED_TRANSFER){
		o=malloc(sizeof(char));
		ln=0;
		if (i==o_bf_sz){
			SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
			i=0;
		}
		char c=o_bf[i];
		i++;
		while (1){
			uint32_t sz=0;
			while (c!='\r'){
				sz=(sz<<4)|(c<58?c-48:(c<71?c-55:c-87));
				if (i==o_bf_sz){
					SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
					i=0;
				}
				c=o_bf[i];
				i++;
			}
			if (i==o_bf_sz){
				SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
				i=0;
			}
			i++;
			if (!sz){
				break;
			}
			ln+=sz;
			o=realloc(o,(ln+1)*sizeof(char));
			char* ptr=o+ln-sz;
			uint32_t k=0;
			while (k<sz){
				uint32_t l=(uint32_t)(o_bf_sz-i>sz-k?sz-k:o_bf_sz-i);
				memcpy(ptr+k,o_bf+i,l);
				k+=l;
				i+=l;
				if (k<sz){
					SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
					i=0;
				}
			}
			if (i==o_bf_sz){
				SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
				i=0;
			}
			c=o_bf[i];
			i++;
			while (c=='\r'||c=='\n'){
				if (i==o_bf_sz){
					SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
					i=0;
				}
				c=o_bf[i];
				i++;
			}
		}
		*(o+ln)=0;
	}
	else if (ln!=UINT32_MAX){
		o=malloc((ln+1)*sizeof(char));
		*(o+ln)=0;
		uint32_t k=0;
		while (k<ln){
			uint32_t sz=(uint32_t)(o_bf_sz-i>ln-k?ln-k:o_bf_sz-i);
			memcpy(o+k,o_bf+i,sz);
			k+=sz;
			if (k<ln){
				SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
				i=0;
			}
		}
	}
	else{
		o=malloc(sizeof(char));
		ln=0;
		while (1){
			o=realloc(o,(ln+o_bf_sz-i+1)*sizeof(char));
			memcpy(o+ln,o_bf+i,o_bf_sz-i);
			ln+=(uint32_t)(o_bf_sz-i);
			o_bf_sz=0;
			SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
			if (!o_bf_sz){
				break;
			}
			i=0;
		}
		*(o+ln)=0;
	}
	closesocket(sck);
	SSL_free(ssl);
	SSL_CTX_free(ctx);
	return o;
}



uint8_t _download_url(const char* url,FILE* o,uint32_t t_sz){
	if (_cmp_str_len(url,"http://",7)){
		url+=7;
	}
	else if (_cmp_str_len(url,"https://",8)){
		url+=8;
	}
	char bf[4096]="GET ";
	uint16_t i=0;
	while (bf[i]){
		i++;
	}
	char s[256];
	uint16_t j=0;
	while (*(url+j)!='/'){
		s[j]=*(url+j);
		j++;
	}
	s[j]=0;
	i+=_copy_str(bf+i,url+j);
	i+=_copy_str(bf+i," HTTP/1.1\r\nAccept: */*\r\nHost:");
	i+=_copy_str(bf+i,s);
	i+=_copy_str(bf+i,"\r\n\r\n");
	ADDRINFOA h={
		0,
		AF_UNSPEC,
		SOCK_STREAM,
		IPPROTO_TCP,
		0,
		NULL,
		NULL,
		NULL
	};
	ADDRINFOA* addr=NULL;
	if (getaddrinfo(s,"443",&h,&addr)){
		return 0;
	}
	SOCKET sck;
	ADDRINFOA* c=addr;
	do{
		sck=socket(c->ai_family,c->ai_socktype,c->ai_protocol);
		if (sck==INVALID_SOCKET){
			freeaddrinfo(addr);
			return 0;
		}
		if (connect(sck,c->ai_addr,(int)c->ai_addrlen)!=SOCKET_ERROR){
			break;
		}
		closesocket(sck);
		sck=INVALID_SOCKET;
		c=c->ai_next;
	} while (c);
	freeaddrinfo(addr);
	if (!c){
		return 0;
	}
	SSL_CTX* ctx=SSL_CTX_new(TLS_client_method());
	if (!ctx){
		closesocket(sck);
		return 0;
	}
	SSL* ssl=SSL_new(ctx);
	if (!ssl){
		closesocket(sck);
		return 0;
	}
	SSL_set_fd(ssl,(int)sck);
	if (SSL_connect(ssl)!=1){
		closesocket(sck);
		return 0;
	}
	SSL_write(ssl,bf,i);
	char o_bf[HTTP_REQUEST_BUFFER_SIZE];
	size_t o_bf_sz=0;
	SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
	i=0;
	uint8_t st=0;
	char hk[4096];
	uint16_t hkl=0;
	char hv[4096];
	uint16_t hvl=0;
	uint32_t ln=UINT32_MAX;
	uint8_t fl=0;
	while (1){
		if (i==o_bf_sz){
			SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
			i=0;
		}
		char c=o_bf[i];
		i++;
		if (c=='\r'||c=='\n'){
			if (st>4){
				if (hkl==17&&_cmp_str_len_lower(hk,"transfer-encoding",17)){
					if (hvl==7&&_cmp_str_len_lower(hv,"chunked",7)){
						fl|=FLAG_CHUNKED_TRANSFER;
					}
					else{
						fl&=~FLAG_CHUNKED_TRANSFER;
					}
				}
				else if (hkl==14&&_cmp_str_len_lower(hk,"content-length",14)){
					ln=0;
					for (uint16_t k=0;k<hvl;k++){
						ln=ln*10+(hv[k]-48);
					}
				}
				hkl=0;
				hvl=0;
				st=1;
			}
			else{
				st++;
				if (st==4){
					break;
				}
			}
		}
		else if (st==2){
			hk[0]=c;
			hkl=1;
			st=0;
		}
		else if (st==5&&(c==' '||c=='\t')){
			continue;
		}
		else if (st>4){
			st=6;
			hv[hvl]=c;
			hvl++;
		}
		else if (c==':'){
			st=5;
		}
		else{
			hk[hkl]=c;
			hkl++;
		}
	}
	if (fl&FLAG_CHUNKED_TRANSFER){
		if (i==o_bf_sz){
			SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
			i=0;
		}
		char c=o_bf[i];
		i++;
		while (1){
			uint32_t sz=0;
			while (c!='\r'){
				sz=(sz<<4)|(c<58?c-48:(c<71?c-55:c-87));
				if (i==o_bf_sz){
					SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
					i=0;
				}
				c=o_bf[i];
				i++;
			}
			if (i==o_bf_sz){
				SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
				i=0;
			}
			i++;
			if (!sz){
				break;
			}
			uint32_t k=0;
			while (k<sz){
				uint32_t l=(uint32_t)(o_bf_sz-i>sz-k?sz-k:o_bf_sz-i);
				fwrite(o_bf+i,sizeof(char),l,o);
				k+=l;
				i+=l;
				if (k<sz){
					SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
					i=0;
				}
			}
			if (i==o_bf_sz){
				SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
				i=0;
			}
			c=o_bf[i];
			i++;
			while (c=='\r'||c=='\n'){
				if (i==o_bf_sz){
					SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
					i=0;
				}
				c=o_bf[i];
				i++;
			}
		}
	}
	else if (ln!=UINT32_MAX){
		uint32_t k=0;
		while (k<ln){
			uint32_t sz=(uint32_t)(o_bf_sz-i>ln-k?ln-k:o_bf_sz-i);
			fwrite(o_bf+i,sizeof(char),sz,o);
			k+=sz;
			if (k<ln){
				SSL_read_ex(ssl,o_bf,HTTP_REQUEST_BUFFER_SIZE,&o_bf_sz);
				i=0;
			}
		}
	}
	else{
		printf("Unimplemented\n");
		getchar();
		ExitProcess(1);
	}
	closesocket(sck);
	SSL_free(ssl);
	SSL_CTX_free(ctx);
	return 1;
}



void _parse_fnmatch_pattern(const char* dt,gitignore_pattern_t* o){
	o->sz=0;
	o->dt=NULL;
	while (*dt){
		o->sz++;
		o->dt=realloc(o->dt,o->sz*sizeof(gitignore_pattern_element_t));
		gitignore_pattern_element_t* e=o->dt+o->sz-1;
		if (*dt=='?'){
			e->t=GITIGNORE_PATTERN_ELEMENT_TYPE_ANY;
		}
		else if (*dt=='*'){
			e->t=GITIGNORE_PATTERN_ELEMENT_TYPE_ANY_STAR;
		}
		else if (*dt=='['){
			e->t=GITIGNORE_PATTERN_ELEMENT_TYPE_CHAR_CLASS;
			for (uint8_t i=0;i<GITIGNORE_PATTERN_ELEMENT_CHAR_CLASS_CLASS_SIZE;i++){
				e->dt.cc.dt[i]=0;
			}
			dt++;
			uint8_t inv=0;
			if (*dt=='!'){
				inv=1;
				dt++;
			}
			do{
				char c=*dt;
				dt++;
				if (*dt=='-'&&*(dt+1)!=']'){
					char d=*(dt+1);
					dt+=2;
					for (uint8_t i=c>>6;i<=d>>6;i++){
						uint8_t a=(i>(c>>6)?0:c&63);
						uint8_t b=(i<(d>>6)?63:d&63);
						e->dt.cc.dt[i]|=((1ull<<(b-a+1))-1)<<a;
					}
				}
				else{
					e->dt.cc.dt[c>>6]|=1ull<<(c&0x3f);
				}
			} while (*dt!=']');
			e->dt.cc.dt[1]|=(e->dt.cc.dt[1]&0x7fffffe00000000)>>32;
			if (inv){
				for (uint8_t i=0;i<GITIGNORE_PATTERN_ELEMENT_CHAR_CLASS_CLASS_SIZE;i++){
					e->dt.cc.dt[i]^=0xffffffffffffffff;
				}
			}
		}
		else if (*dt=='\\'){
			dt++;
			e->t=GITIGNORE_PATTERN_ELEMENT_TYPE_CHAR;
			e->dt.c=(*dt>96&&*dt<123?(*dt)-32:*dt);
		}
		else{
			e->t=GITIGNORE_PATTERN_ELEMENT_TYPE_CHAR;
			e->dt.c=(*dt>96&&*dt<123?(*dt)-32:*dt);
		}
		dt++;
	}
}



void _free_pattern(gitignore_pattern_t* p){
	if (p->dt){
		free(p->dt);
	}
}



void _parse_gitingore_file(const char* fp,gitignore_file_data_t* o){
	FILE* f=fopen(fp,"rb");
	char c=fgetc(f);
	o->sz=0;
	o->dt=NULL;
	while (c!=EOF){
		while (c==' '||c=='\t'||c=='\r'||c=='\n'){
			c=fgetc(f);
		}
		if (c==EOF){
			break;
		}
		if (c=='#'){
			while (c!='\r'&&c!='\n'){
				c=fgetc(f);
			}
			continue;
		}
		uint8_t fl=0;
		if (c=='!'){
			fl|=FLAG_INVERT;
			c=fgetc(f);
		}
		if (c==EOF){
			break;
		}
		char bf[4096];
		uint16_t i=0;
		while (c=='/'){
			c=fgetc(f);
		}
		if (c==EOF){
			break;
		}
		while (c!=EOF&&c!='\r'&&c!='\n'){
			if (i==1&&(c=='#'||c=='!')&&bf[0]=='\\'){
				i=0;
			}
			bf[i]=c;
			i++;
			c=fgetc(f);
		}
		while (i>1&&(bf[i-1]==' '||bf[i-1]=='\t')&&bf[i-2]!='\\'){
			i--;
		}
		if (bf[i-1]=='/'){
			fl|=FLAG_DIRECTORY;
			i--;
		}
		while (i>2&&bf[i-1]=='*'&&bf[i-2]=='*'&&bf[i-3]=='/'){
			i-=3;
			fl|=FLAG_ALL_INSIDE;
		}
		while (i>2&&bf[0]=='*'&&bf[1]=='*'&&bf[2]=='/'){
			i-=3;
			for (uint16_t j=0;j<i;j++){
				bf[j]=bf[j+3];
			}
		}
		while (i&&bf[0]=='/'){
			fl|=FLAG_FROM_ROOT;
			i--;
			for (uint16_t j=0;j<i;j++){
				bf[j]=bf[j+1];
			}
		}
		if (!i){
			break;
		}
		bf[i]=0;
		uint16_t j=i;
		o->sz++;
		o->dt=realloc(o->dt,o->sz*sizeof(gitignore_file_data_pattern_t));
		gitignore_file_data_pattern_t* e=o->dt+o->sz-1;
		e->fl=fl;
		e->sz=1;
		while (j){
			j--;
			if (j>1&&bf[j]=='/'&&bf[j-1]=='*'&&bf[j-2]=='*'&&(j==2||bf[j-3]!='*')){
				for (uint16_t k=j+1;k<i;k++){
					bf[k-3]=bf[k];
				}
				j-=3;
				i-=3;
				bf[i]=0;
			}
			if (bf[j]=='/'){
				bf[j]=0;
				e->sz++;
			}
			if (bf[j]=='*'){
				uint16_t s=j;
				while (j&&bf[j-1]=='*'){
					j--;
				}
				if (s-j){
					uint16_t off=s-j;
					for (uint16_t k=s+1;k<i;k++){
						bf[k-off]=bf[k];
					}
					i-=off;
					bf[i]=0;
				}
			}
		}
		e->dt=malloc(e->sz*sizeof(gitignore_pattern_t));
		gitignore_pattern_t* k=e->dt;
		j=0;
		do{
			_parse_fnmatch_pattern(bf+j,k);
			j++;
			while (bf[j-1]!=0){
				j++;
			}
			k++;
		} while (j<i);
		if (c==EOF){
			break;
		}
		c=fgetc(f);
	}
	fclose(f);
}



void _free_gitignore_data(gitignore_file_data_t* gdt){
	for (uint16_t i=0;i<gdt->sz;i++){
		for (uint16_t j=0;j<(gdt->dt+i)->sz;j++){
			_free_pattern((gdt->dt+i)->dt+j);
		}
		if ((gdt->dt+i)->dt){
			free((gdt->dt+i)->dt);
		}
	}
	if (gdt->dt){
		free(gdt->dt);
	}
}



char* _github_api_request(const char* m,const char* url,const char* dt){
	FILETIME s;
	GetSystemTimeAsFileTime(&s);
	string_32bit_t str={
		0,
		(char*)dt
	};
	if (dt){
		while (*(dt+str.l)){
			str.l++;
		}
	}
	char* o=_request_url(m,url,&str,FLAG_ACCEPT_GITHUB|FLAG_GITHUB_TOKEN);
	FILETIME e;
	GetSystemTimeAsFileTime(&e);
	uint32_t tm=(e.dwLowDateTime-s.dwLowDateTime)/10000;
	if (tm<3600000/GITHUB_API_QUOTA){
		Sleep(3600000/GITHUB_API_QUOTA-tm);
	}
	return o;
}



uint8_t _match_gitignore_pattern(const gitignore_pattern_element_t* p,uint32_t sz,const char* str){
	if (!sz){
		return !(*str);
	}
	if (p->t==GITIGNORE_PATTERN_ELEMENT_TYPE_CHAR){
		return (*str!=p->dt.c?0:_match_gitignore_pattern(p+1,sz-1,str+1));
	}
	if (p->t==GITIGNORE_PATTERN_ELEMENT_TYPE_ANY){
		return _match_gitignore_pattern(p+1,sz-1,str+1);
	}
	if (p->t==GITIGNORE_PATTERN_ELEMENT_TYPE_ANY_STAR){
		if (sz==1){
			return 1;
		}
		while (*str){
			if (_match_gitignore_pattern(p+1,sz-1,str)){
				return 1;
			}
			str++;
		}
		return 0;
	}
	return (p->dt.cc.dt[(*str)>>6]&(1ull<<((*str)&0x3f))?_match_gitignore_pattern(p+1,sz-1,str+1):0);
}



uint8_t _is_binary(uint8_t* bf,uint32_t l){
	if (!l){
		return 0;
	}
	if (l>4096){
		l=4096;
	}
	float r1=0;
	float r2=0;
	uint8_t e=0;
	for (uint16_t i=0;i<l;i++){
		if (bf[i]>127){
			r2++;
		}
		else if (bf[i]>31||bf[i]=='\n'||bf[i]=='\r'||bf[i]=='\t'){
			r1++;
		}
		if (!bf[i]){
			e|=1;
		}
		else if (bf[i]==255){
			e|=3;
		}
	}
	r1/=l;
	r2/=l;
	if (r1>=0.9f&&r2>=0.9f){
		return 1;
	}
	uint8_t enc_u=1;
	for (uint16_t i=0;i<l;i++){
		if (bf[i]<128){
			continue;
		}
		else if ((bf[i]>>5)==0b110){
			i++;
			if (i==l){
				break;
			}
			if ((bf[i]>>6)!=0b10){
				enc_u=0;
				break;
			}
		}
		else if ((bf[i]>>4)==0b1110){
			i++;
			if (i==l){
				break;
			}
			if ((bf[i]>>6)!=0b10){
				enc_u=0;
				break;
			}
			i++;
			if (i==l){
				break;
			}
			if ((bf[i]>>6)!=0b10){
				enc_u=0;
				break;
			}
		}
		else if ((bf[i]>>3)==0b11110){
			i++;
			if (i==l){
				break;
			}
			if ((bf[i]>>6)!=0b10){
				enc_u=0;
				break;
			}
			i++;
			if (i==l){
				break;
			}
			if ((bf[i]>>6)!=0b10){
				enc_u=0;
				break;
			}
			i++;
			if (i==l){
				break;
			}
			if ((bf[i]>>6)!=0b10){
				enc_u=0;
				break;
			}
		}
		else{
			enc_u=0;
			break;
		}
	}
	return (((r1>=0.3f&&r2<=0.05f)||(r1>=0.8f&&r1>=0.8f))?!enc_u:(!enc_u&&e==3));
}



void _create_commit(string_8bit_t* fp,github_directory_tree_t* r_t,const gitignore_file_data_t* gdt,github_commit_data_t* o){
	fp->v[fp->l]='*';
	fp->v[fp->l+1]=0;
	WIN32_FIND_DATAA dt;
	HANDLE* fh=FindFirstFileA(fp->v,&dt);
	if (fh!=INVALID_HANDLE_VALUE){
		do{
			if (dt.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
				continue;
			}
			char tmp[256];
			uint8_t fp_l=fp->l+_copy_str(fp->v+fp->l,dt.cFileName);
			fp->v[fp_l]=0;
			uint8_t l=1;
			for (uint8_t i=o->fp_s;i<fp_l;i++){
				if (fp->v[i]=='/'){
					tmp[i-o->fp_s]=0;
					l++;
				}
				else{
					tmp[i-o->fp_s]=(fp->v[i]>96&&fp->v[i]<123?fp->v[i]-32:fp->v[i]);
				}
			}
			tmp[fp_l-o->fp_s]=0;
			uint8_t ig=0;
			for (uint16_t i=0;i<gdt->sz;i++){
				gitignore_file_data_pattern_t* f_dt_p=gdt->dt+i;
				if ((!ig||(f_dt_p->fl&FLAG_INVERT))&&f_dt_p->sz<=l){
					if (f_dt_p->fl&FLAG_FROM_ROOT){
						char* k=tmp;
						for (uint16_t j=0;j<f_dt_p->sz;j++){
							if (j){
								while (*k){
									k++;
								}
								k++;
							}
							if (!_match_gitignore_pattern((f_dt_p->dt+j)->dt,(f_dt_p->dt+j)->sz,k)){
								goto _check_next_pattern;
							}
						}
						if (f_dt_p->fl&FLAG_INVERT){
							goto _check_file_hash;
						}
						ig=1;
					}
					else{
						for (uint16_t j=0;j<=l-f_dt_p->sz;j++){
							char* e=tmp;
							for (uint16_t k=0;k<j;k++){
								while (*e){
									e++;
								}
								e++;
							}
							for (uint16_t k=0;k<f_dt_p->sz;k++){
								if (k){
									while (*e){
										e++;
									}
									e++;
								}
								if (!_match_gitignore_pattern((f_dt_p->dt+k)->dt,(f_dt_p->dt+k)->sz,e)){
									goto _check_next_pattern_length;
								}
							}
							if (f_dt_p->fl&FLAG_INVERT){
								goto _check_file_hash;
							}
							ig=1;
							break;
_check_next_pattern_length:;
						}
					}
				}
_check_next_pattern:;
			}
			if (ig){
				o->cnt[GITHUB_COMMIT_DATA_IGNORE_COUNT]++;
				PRINTF_TIME("\x1b[38;2;190;0;220m! %s/%s\n",o->nm,fp->v+o->fp_s);
				continue;
			}
_check_file_hash:
			HANDLE sz_fh=CreateFileA(fp->v,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			uint32_t sz=GetFileSize(sz_fh,NULL);
			CloseHandle(sz_fh);
			uint8_t* dt=malloc(sz*sizeof(uint8_t));
			FILE* f=fopen(fp->v,"rb");
			fread(dt,sizeof(uint8_t),sz,f);
			fclose(f);
			uint8_t bn=_is_binary(dt,sz);
			if (!bn){
				uint32_t i=0;
				uint32_t k=0;
				for (uint32_t j=0;j<sz;j++){
					if (i){
						*(dt+j-i)=*(dt+j);
					}
					if (*(dt+j)=='\r'){
						i++;
						k++;
					}
				}
				sz-=k;
			}
			for (uint32_t i=0;i<r_t->sz;i++){
				github_directory_tree_data_t* e=r_t->dt+i;
				if (e->rm&&_cmp_str_len(e->nm,fp->v+o->fp_s,fp->l-o->fp_s+1)){
					e->rm=0;
					uint8_t bf[64]="blob ";
					uint8_t bfl=0;
					while (bf[bfl]){
						bfl++;
					}
					char tmp[20];
					uint8_t tmpi=0;
					uint64_t v=sz;
					do{
						tmp[tmpi]=v%10;
						tmpi++;
						v/=10;
					} while (v);
					while (tmpi){
						tmpi--;
						bf[bfl]=tmp[tmpi]+48;
						bfl++;
					}
					bf[bfl]=0;
					bfl++;
					uint64_t sha_sz=bfl;
					uint32_t k=(sz>(uint32_t)(64-bfl)?64-bfl:sz);
					uint32_t off=bfl;
					memcpy(bf+bfl,dt+sha_sz-off,k);
					sha_sz+=k;
					sha1_data_t h=SHA1_DATA_INIT;
					if (sz>=(uint32_t)(64-bfl)){
						uint32_t j=sz-k;
						_sha1_chunk(&h,bf);
						bfl=0;
						while (j){
							k=(j>64?64:j);
							memcpy(bf,dt+sha_sz-off,k);
							sha_sz+=k;
							if (k!=64){
								bfl=k;
								break;
							}
							_sha1_chunk(&h,bf);
							j-=64;
							if (!j){
								bfl=0;
								break;
							}
						}
					}
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
					sha_sz<<=3;
					for (uint8_t k=63;k>=56;k--){
						bf[k]=sha_sz&0xff;
						sha_sz>>=8;
					}
					_sha1_chunk(&h,bf);
					if (_cmp_hash(&h,e->sha)){
						o->cnt[GITHUB_COMMIT_DATA_SKIP_COUNT]++;
						PRINTF_TIME("\x1b[38;2;230;210;40m? %s/%s\n",o->nm,fp->v+o->fp_s);
						goto _skip_file;
					}
					break;
				}
			}
			o->cnt[GITHUB_COMMIT_DATA_UPDATE_COUNT]++;
			PRINTF_TIME("\x1b[38;2;70;210;70m+ %s/%s\n",o->nm,fp->v+o->fp_s);
_skip_file:
			free(dt);
		} while (FindNextFileA(fh,&dt));
		FindClose(fh);
	}
	fp->v[fp->l]='*';
	fp->v[fp->l+1]=0;
	fh=FindFirstFileA(fp->v,&dt);
	if (fh!=INVALID_HANDLE_VALUE){
		do{
			if (dt.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
				if (*(dt.cFileName)=='.'&&(*(dt.cFileName+1)==0||(*(dt.cFileName+1)=='.'&&*(dt.cFileName+2)==0))){
					continue;
				}
				uint8_t fpl=fp->l;
				fp->l+=_copy_str(fp->v+fp->l,dt.cFileName);
				fp->v[fp->l]='/';
				fp->l++;
				_create_commit(fp,r_t,gdt,o);
				fp->l=fpl;
			}
		} while (FindNextFileA(fh,&dt));
		FindClose(fh);
	}
}



void _push_github_project(string_8bit_t* fp,const expand_data_t* e_dt){
	if (fp->v[fp->l-1]!='/'){
		fp->v[fp->l]='/';
		fp->l++;
	}
	github_branch_t* bl=NULL;
	uint16_t bll=0;
	if (GetFileAttributesA(GITHUB_PROJECT_BRANCH_LIST_FILE_PATH)!=INVALID_FILE_ATTRIBUTES){
		FILE* f=fopen(GITHUB_PROJECT_BRANCH_LIST_FILE_PATH,"rb");
		bll=(fgetc(f)<<8)|fgetc(f);
		bl=malloc(bll*sizeof(github_branch_t));
		for (uint16_t i=0;i<bll;i++){
			(bl+i)->nm.l=fgetc(f);
			(bl+i)->b.l=fgetc(f);
			fread((bl+i)->nm.v,sizeof(char),(bl+i)->nm.l,f);
			fread((bl+i)->b.v,sizeof(char),(bl+i)->b.l,f);
			(bl+i)->nm.v[(bl+i)->nm.l]=0;
			(bl+i)->b.v[(bl+i)->b.l]=0;
		}
		fclose(f);
	}
	uint8_t cr=1;
	char* br=GITHUB_DEFAULT_BRANCH_NAME;
	for (uint16_t i=0;i<bll;i++){
		if (_cmp_str_len_lower((bl+i)->nm.v,e_dt->e.fp,(bl+i)->nm.l+1)){
			br=(bl+i)->b.v;
			cr=0;
			break;
		}
	}
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	char msg[256]="Push Update ";
	uint16_t i=0;
	while (msg[i]){
		i++;
	}
	msg[i]=tm.wDay/10+48;
	msg[i+1]=(tm.wDay%10)+48;
	msg[i+2]='/';
	msg[i+3]=tm.wMonth/10+48;
	msg[i+4]=(tm.wMonth%10)+48;
	msg[i+5]='/';
	msg[i+6]=tm.wYear/1000+48;
	msg[i+7]=((tm.wYear/100)%10)+48;
	msg[i+8]=((tm.wYear/10)%10)+48;
	msg[i+9]=(tm.wYear%10)+48;
	msg[i+10]=' ';
	msg[i+11]=tm.wHour/10+48;
	msg[i+12]=(tm.wHour%10)+48;
	msg[i+13]=':';
	msg[i+14]=tm.wMinute/10+48;
	msg[i+15]=(tm.wMinute%10)+48;
	msg[i+16]=':';
	msg[i+17]=tm.wSecond/10+48;
	msg[i+18]=(tm.wSecond%10)+48;
	msg[i+19]=0;
	char url[4096]="https://api.github.com/repos/"GITHUB_USERNAME"/";
	uint16_t url_i=0;
	while (url[url_i]){
		url_i++;
	}
	url_i+=_copy_str(url+url_i,e_dt->e.fp);
	url[url_i]='/';
	url_i++;
	if (cr){
		PRINTF_TIME("\x1b[38;2;100;100;100mCreating Project \x1b[38;2;65;118;46m'%s'\x1b[38;2;100;100;100m...\n",e_dt->e.fp);
		char bf[4096]="{\"name\":\"";
		i=0;
		while (bf[i]){
			i++;
		}
		uint8_t sz=_copy_str(bf+i,e_dt->e.fp);
		i+=sz;
		i+=_copy_str(bf+i,"\",\"description\":\"");
		i+=_copy_str(bf+i,e_dt->t);
		bf[i]='\"';
		bf[i+1]='}';
		bf[i+2]=0;
		free(_github_api_request("POST","https://api.github.com/user/repos",bf));
		bll++;
		bl=realloc(bl,bll*sizeof(github_branch_t));
		(bl+bll-1)->nm.l=sz;
		_copy_str((bl+bll-1)->nm.v,e_dt->e.fp);
		(bl+bll-1)->nm.v[sz]=0;
		(bl+bll-1)->b.l=_copy_str((bl+bll-1)->b.v,GITHUB_DEFAULT_BRANCH_NAME);
		(bl+bll-1)->b.v[(bl+bll-1)->b.l]=0;
		url[url_i+_copy_str(url+url_i,"contents/_")]=0;
		i=_copy_str(bf,"{\"content\":\"\",\"message\":\"");
		i+=_copy_str(bf+i,msg);
		bf[i]='\"';
		bf[i+1]='}';
		bf[i+2]=0;
		free(_github_api_request("PUT",url,bf));
	}
	PRINTF_TIME("\x1b[38;2;100;100;100mParsing Gitignore File...\n");
	fp->v[fp->l+_copy_str(fp->v+fp->l,".gitignore")]=0;
	gitignore_file_data_t gdt;
	_parse_gitingore_file(fp->v,&gdt);
	PRINTF_TIME("\x1b[38;2;100;100;100mCommiting to Branch \x1b[38;2;65;118;46m'%s/%s'\x1b[38;2;100;100;100m with Message \x1b[38;2;65;118;46m'%s'\x1b[38;2;100;100;100m...\n",e_dt->e.fp,br,msg);
	i=url_i+_copy_str(url+url_i,"git/ref/heads/");
	url[i+_copy_str(url+i,br)];
	char* dt=_github_api_request("GET",url,NULL);
	json_object_t json;
	json_parser_state_t p=dt;
	char bt_sha[256];
	if (_parse_json(&p,&json)){
		free(dt);
		_free_gitignore_data(&gdt);
		free(bl);
		return;
	}
	free(dt);
	bt_sha[_copy_str(bt_sha,_get_by_key(_get_by_key(&json,"object"),"sha")->dt.s.v)]=0;
	_free_json(&json);
	PRINTF_TIME("\x1b[38;2;100;100;100mReading Recursive Tree...\n");
	i=url_i+_copy_str(url+url_i,"git/trees/");
	i+=_copy_str(url+i,bt_sha);
	url[i+_copy_str(url+i,"?recursive=true")]=0;
	dt=_github_api_request("GET",url,NULL);
	p=dt;
	if (_parse_json(&p,&json)){
		free(dt);
		_free_gitignore_data(&gdt);
		free(bl);
		return;
	}
	free(dt);
	github_directory_tree_t r_t={
		0,
		NULL
	};
	if (_get_by_key(&json,"truncated")->t!=JSON_OBJECT_TYPE_FALSE){
		printf("Unimplemented!\n");
		getchar();
		ExitProcess(1);
	}
	else{
		PRINTF_TIME("\x1b[38;2;100;100;100mFound Tree \x1b[38;2;65;118;46m'.'\x1b[38;2;100;100;100m...\n");
		json_array_t tl=_get_by_key(&json,"tree")->dt.a;
		for (uint32_t j=0;j<tl.l;j++){
			json_object_t* k=tl.dt+j;
			if (_cmp_str_len(_get_by_key(k,"type")->dt.s.v,"blob",5)){
				r_t.sz++;
				r_t.dt=realloc(r_t.dt,r_t.sz*sizeof(github_directory_tree_data_t));
				github_directory_tree_data_t* e=r_t.dt+r_t.sz-1;
				e->nm[_copy_str(e->nm,_get_by_key(k,"path")->dt.s.v)]=0;
				e->sz=(uint32_t)_get_by_key(k,"size")->dt.i;
				e->sha[_copy_str(e->sha,_get_by_key(k,"sha")->dt.s.v)]=0;
				e->rm=1;
			}
			else{
				PRINTF_TIME("\x1b[38;2;100;100;100mFound Tree \x1b[38;2;65;118;46m'./%s'\x1b[38;2;100;100;100m...\n",_get_by_key(k,"path")->dt.s.v);
			}
		}
	}
	_free_json(&json);
	PRINTF_TIME("\x1b[38;2;100;100;100mCreating Commit...\n");
	github_commit_data_t cm={
		fp->l,
		{
			0,
			0,
			0,
			0
		},
		0,
		NULL,
		msg,
		e_dt->e.fp
	};
	_create_commit(fp,&r_t,&gdt,&cm);
	for (uint32_t j=0;j<r_t.sz;j++){
		if ((r_t.dt+j)->rm){
			cm.cnt[GITHUB_COMMIT_DATA_DELETE_COUNT]++;
			PRINTF_TIME("\x1b[38;2;210;40;40m- %s/%s\n",e_dt->e.fp,(r_t.dt+j)->nm);
		}
	}
	PRINTF_TIME("\x1b[38;2;40;210;190m%s => \x1b[38;2;70;210;70m+%u\x1b[38;2;40;210;190m, \x1b[38;2;230;210;40m?%u\x1b[38;2;40;210;190m, \x1b[38;2;190;0;220m!%u\x1b[38;2;40;210;190m, \x1b[38;2;210;40;40m-%u\n",e_dt->e.fp,cm.cnt[GITHUB_COMMIT_DATA_UPDATE_COUNT],cm.cnt[GITHUB_COMMIT_DATA_SKIP_COUNT],cm.cnt[GITHUB_COMMIT_DATA_IGNORE_COUNT],cm.cnt[GITHUB_COMMIT_DATA_DELETE_COUNT]);
	if (r_t.dt){
		free(r_t.dt);
	}
	_free_gitignore_data(&gdt);
	if (cr){
		FILE* f=fopen(GITHUB_PROJECT_BRANCH_LIST_FILE_PATH,"wb");
		fputc(bll>>8,f);
		fputc(bll&0xff,f);
		for (uint16_t j=0;j<bll;j++){
			fputc((bl+j)->nm.l,f);
			fputc((bl+j)->b.l,f);
			fwrite((bl+j)->nm.v,sizeof(char),(bl+j)->nm.l,f);
			fwrite((bl+j)->b.v,sizeof(char),(bl+j)->b.l,f);
		}
		fclose(f);
	}
	free(bl);
	getchar();ExitProcess(1);
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
	if (GetFileAttributesA(__FILE_BASE_DIR__"/data/")==INVALID_FILE_ATTRIBUTES){
		CreateDirectoryA(__FILE_BASE_DIR__"/data/",NULL);
	}
	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	if (WSAStartup(MAKEWORD(2,2),&_ws_dt)){
		_console();
		printf("WinSock Fail!\n");
		getchar();
		ExitProcess(1);
	}
	atexit(WSACleanup);
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
				project_type_t tl[256];
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
								_create_project(&p_t,&p_nm,FLAG_OPEN);
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
							project_type_t pt={0};
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
								_create_project(NULL,NULL,FLAG_OPEN);
								break;
							}
							for (uint8_t i=0;i<tll;i++){
								project_type_t e=tl[i];
								if (e.l!=p_t.l||!_cmp_str_len_lower(e.v,p_t.v,p_t.l)){
									continue;
								}
								for (uint16_t j=0;j<e.el;j++){
									if ((e.e+j)->l!=p_nm.l||!_cmp_str_len_lower((e.e+j)->v,p_nm.v,p_nm.l)){
										continue;
									}
									_create_project(&p_t,&p_nm,FLAG_OPEN);
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
						project_type_t pt={0};
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
			{
				HWND hwnd=_console();
				if (argc==2||(argv[2][0]=='*'&&!argv[2][1])){
					// _move_to_desktop(hwnd,2);
					if (getenv("DISABLE_BULK_PROJECT_PUSH")){
						PRINTF_TIME("\x1b[38;2;200;40;20mProject Push Diabled.\n");
						getchar();
						return 0;
					}
					string_8bit_t* fl=NULL;
					uint16_t fll=0;
					if (argc==2){
						if (GetFileAttributesA(GITHUB_PUSHED_PROJECT_LIST_FILE_PATH)!=INVALID_FILE_ATTRIBUTES){
							FILE* f=fopen(GITHUB_PUSHED_PROJECT_LIST_FILE_PATH,"rb");
							fll=(fgetc(f)<<8)|fgetc(f);
							fl=malloc(fll*sizeof(string_8bit_t));
							for (uint16_t i=0;i<fll;i++){
								(fl+i)->l=fgetc(f);
								fread((fl+i)->v,sizeof(char),(fl+i)->l,f);
								(fl+i)->v[(fl+i)->l]=0;
							}
							fclose(f);
						}
					}
					WIN32_FIND_DATAA dt;
					HANDLE fh=FindFirstFileA(PROJECT_DIR"/*",&dt);
					if (fh!=INVALID_HANDLE_VALUE){
						do{
							if (dt.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
								if (*(dt.cFileName)=='.'&&(*(dt.cFileName+1)==0||(*(dt.cFileName+1)=='.'&&*(dt.cFileName+2)==0))){
									continue;
								}
								for (uint16_t i=0;i<fll;i++){
									if (_cmp_str_len_lower((fl+i)->v,dt.cFileName,(fl+i)->l+1)){
										goto _push_next_project;
									}
								}
								char t[256];
								uint8_t i=0;
								while (*(dt.cFileName+i)!='-'){
									if (!(*(dt.cFileName+i))){
										goto _push_next_project;
									}
									t[i]=*(dt.cFileName+i);
									i++;
								}
								t[i]=0;
								expand_data_t e_dt;
								_generate_expand_data(&e_dt,t,dt.cFileName+i+1);
								string_8bit_t fp={
									0,
									PROJECT_DIR"/"
								};
								while (fp.v[fp.l]){
									fp.l++;
								}
								uint8_t sz=_copy_str(fp.v+fp.l,dt.cFileName);
								fp.l+=sz;
								fp.v[fp.l]='/';
								fp.l++;
								fp.v[fp.l]=0;
								_copy_str(e_dt.e.fp,dt.cFileName);
								e_dt.e.fp[sz]=0;
								_push_github_project(&fp,&e_dt);
								fll++;
								fl=realloc(fl,fll*sizeof(string_8bit_t));
								(fl+fll-1)->l=sz;
								_copy_str((fl+fll-1)->v,dt.cFileName);
								(fl+fll-1)->v[(fl+fll-1)->l]=0;
_push_next_project:;
							}
						} while (FindNextFileA(fh,&dt));
						FindClose(fh);
						FILE* f=fopen(GITHUB_PUSHED_PROJECT_LIST_FILE_PATH,"wb");
						fputc(fll>>8,f);
						fputc(fll&0xff,f);
						for (uint16_t i=0;i<fll;i++){
							fputc((fl+i)->l,f);
							fwrite((fl+i)->v,sizeof(char),(fl+i)->l,f);
						}
						fclose(f);
						free(fl);
					}
				}
				else{
					SetFocus(hwnd);
					if (getenv("DISABLE_BULK_PROJECT_PUSH")){
						PRINTF_TIME("\x1b[38;2;200;40;20mProject Push Diabled.\n");
						getchar();
						return 0;
					}
					string_8bit_t fp={
						0
					};
					while (argv[2][fp.l]){
						fp.v[fp.l]=(argv[2][fp.l]=='\\'?'/':argv[2][fp.l]);
						fp.l++;
					}
					while (fp.v[fp.l-1]=='/'){
						fp.l--;
					}
					fp.v[fp.l]=0;
					uint8_t i=fp.l;
					while (fp.v[i-1]!='/'){
						i--;
					}
					if (_cmp_str_len_lower(fp.v,PROJECT_DIR,STR_LEN(PROJECT_DIR))){
						char t[256];
						uint8_t j=0;
						while (fp.v[i+j]!='-'){
							if (!(fp.v[i+j])){
								return 1;
							}
							t[j]=fp.v[i+j];
							j++;
						}
						t[j]=0;
						expand_data_t e_dt;
						_generate_expand_data(&e_dt,t,fp.v+i+j+1);
						e_dt.e.fp[_copy_str(e_dt.e.fp,fp.v+i)]=0;
						fp.v[fp.l]='/';
						fp.l++;
						fp.v[fp.l]=0;
						_push_github_project(&fp,&e_dt);
					}
				}
				getchar();
				return 0;
			}
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
				char* dt;
				if (!(dt=_request_url("GET","launchermeta.mojang.com/mc/game/version_manifest.json",NULL,FLAG_ACCEPT_JSON))){
					goto _skip_update;
				}
				json_parser_state_t p=dt;
				if (_parse_json(&p,&json)){
					free(dt);
					goto _skip_update;
				}
				free(dt);
				string_32bit_t url=_get_by_key(_get_by_key(&json,"versions")->dt.a.dt,"url")->dt.s;
				PRINTF_TIME("Downloading Release Data From URL \x1b[38;2;91;216;38m'%s'\x1b[38;2;100;100;100m...\n",url.v);
				if (!(dt=_request_url("GET",url.v,NULL,FLAG_ACCEPT_JSON))){
					goto _skip_update;
				}
				_free_json(&json);
				json.t=JSON_OBJECT_TYPE_NULL;
				p=dt;
				if (_parse_json(&p,&json)){
					free(dt);
					goto _skip_update;
				}
				free(dt);
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
