import os
import subprocess
import sys
import zipfile



if (os.path.exists("build")):
	dl=[]
	for r,ndl,fl in os.walk("build"):
		dl=[os.path.join(r,k) for k in ndl]+dl
		for f in fl:
			os.remove(os.path.join(r,f))
	for k in dl:
		os.rmdir(k)
else:
	os.mkdir("build")
jfl=[]
ml=[]
for r,_,fl in os.walk("src"):
	r=r.replace("\\","/").strip("/")+"/"
	for f in fl:
		if (f[-5:]==".java"):
			jfl.append(r+f)
		if (f[-4:]==".jar"):
			ml.append(r+f)
if (subprocess.run(["javac","-encoding","utf8","-cp",os.path.pathsep.join(ml),"-d","build"]+jfl).returncode!=0):
	sys.exit(1)
with zipfile.ZipFile("build/$NAME$.jar","w") as zf:
	print("Writing: META-INF/MANIFEST.MF")
	zf.write("manifest.mf",arcname="META-INF/MANIFEST.MF")
	for r,_,fl in os.walk("build"):
		r=r.replace("\\","/").strip("/")+"/"
		for f in fl:
			if (f[-6:]==".class"):
				print(f"Writing: {(r+f)[6:]}")
				zf.write(r+f,arcname=(r+f)[6:])
	for k in ml:
		with zipfile.ZipFile(k,"r") as jf:
			for e in jf.namelist():
				if (e[-1] not in "\\/" and e.lower()[:8]!="meta-inf"):
					print(f"Writing: {e}")
					zf.writestr(e,jf.read(e))
if ("--run" in sys.argv):
	subprocess.run(["java","-Dfile.encoding=UTF8","-jar","build/$NAME$.jar"])
