import subprocess
import sys
import os



__file__=os.path.abspath(__file__).replace("\\","/")
DEFINES={"__FILE_BASE_DIR__":"L\""+__file__[:-len(__file__.split("/")[-1])-1].rstrip("/")+"/\""}
DEFAULT_ARGS=[]



if (os.path.exists("build")):
	dl=[]
	for r,ndl,fl in os.walk("build"):
		r=r.replace("\\","/").strip("/")+"/"
		for d in ndl:
			dl.insert(0,r+d)
		for f in fl:
			os.remove(r+f)
	for k in dl:
		os.rmdir(k)
else:
	os.mkdir("build")
dl=[]
for k,v in DEFINES.items():
	dl.append("/D")
	dl.append(k+"="+v)
if (os.name=="nt"):
	cd=os.getcwd()
	os.chdir("build")
	if ("--release" in sys.argv):
		if (subprocess.run(["cl","/Wv:18","/c","/permissive-","/Zc:preprocessor","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/Oi","/MD","../src/*.c"]+dl).returncode!=0 or subprocess.run(["link","*.obj","/OUT:boot_program.exe","/DYNAMICBASE","kernel32.lib","user32.lib","gdi32.lib","winspool.lib","comdlg32.lib","advapi32.lib","shell32.lib","ole32.lib","oleaut32.lib","uuid.lib","odbc32.lib","odbccp32.lib","/MACHINE:X64","/SUBSYSTEM:WINDOWS","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/OPT:ICF"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	else:
		if (subprocess.run(["cl","/Wv:18","/c","/permissive-","/Zc:preprocessor","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/ZI","/Od","/RTC1","/MDd","../src/*.c"]+dl).returncode!=0 or subprocess.run(["link","*.obj","/OUT:boot_program.exe","/DYNAMICBASE","kernel32.lib","user32.lib","gdi32.lib","winspool.lib","comdlg32.lib","advapi32.lib","shell32.lib","ole32.lib","oleaut32.lib","uuid.lib","odbc32.lib","odbccp32.lib","/MACHINE:X64","/SUBSYSTEM:WINDOWS","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	os.chdir(cd)
	if ("--run" in sys.argv):
		subprocess.run(["build/boot_program.exe"]+DEFAULT_ARGS)
else:
	if ("--release" in sys.argv):
		fl=[]
		for r,_,cfl in os.walk("src"):
			r=r.replace("\\","/").strip("/")+"/"
			for f in cfl:
				if (f[-2:]==".c"):
					fl.append(f"build/{(r+f).replace('/','$')}.o")
					if (subprocess.run(["gcc","-Wall","-lm","-Werror","-O3","-c",r+f,"-o",f"build/{(r+f).replace('/','$')}.o"]+dl).returncode!=0):
						sys.exit(1)
		if (subprocess.run(["gcc","-o","build/boot_program"]+fl+dl).returncode!=0):
			sys.exit(1)
	else:
		fl=[]
		for r,_,cfl in os.walk("src"):
			r=r.replace("\\","/").strip("/")+"/"
			for f in cfl:
				if (f[-2:]==".c"):
					fl.append(f"build/{(r+f).replace('/','$')}.o")
					if (subprocess.run(["gcc","-Wall","-lm","-Werror","-O0","-c",r+f,"-o",f"build/{(r+f).replace('/','$')}.o"]+dl).returncode!=0):
						sys.exit(1)
		if (subprocess.run(["gcc","-o","build/boot_program"]+fl+dl).returncode!=0):
			sys.exit(1)
	if ("--run" in sys.argv):
		subprocess.run(["build/boot_program"]+DEFAULT_ARGS)
