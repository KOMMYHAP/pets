import os, sys
import subprocess

def is_proto_file(path):
	return os.path.isfile(path) and str(path).endswith(".proto")

def is_cpp_file(path):
	str_path = str(path)
	return os.path.isfile(path) and (str_path.endswith(".h") or str_path.endswith(".cc"))

def list_proto_files(dir):
	return [f for f in os.listdir(dir) if is_proto_file(os.path.join(dir, f))]

def list_cpp_files(dir):
	return [f for f in os.listdir(dir) if is_cpp_file(os.path.join(dir, f))]

# define path variables
pets_root = os.path.abspath("./../../../")
print("Root: " + os.path.normpath(pets_root))

proto_compiler = os.path.join(pets_root, "external/protobuf/protoc-3.14.0-win64/bin/protoc.exe")
print("Protoc: " + os.path.normpath(proto_compiler))

# create output directory
out_cpp_dir = os.path.abspath("./cpp-protocol/")
if not os.path.exists(out_cpp_dir):
	os.mkdir(out_cpp_dir)

# cd into protocol dir
old_dir = os.getcwd()
os.chdir("./protocol")
protocol_dir = os.getcwd()

# list input protocol files
in_files = list_proto_files(".")
print("Protocol: " + str(in_files))

# run proto compiler
result = subprocess.run([
	proto_compiler,
	"--proto_path=" + protocol_dir,
	"--cpp_out=" + out_cpp_dir]
	+ in_files,
	capture_output=True)

# cd back into script dir
os.chdir(old_dir)

# print compiler's error and exit or continue
if result.stderr:
	print("Protoc: " + result.stderr.decode().rstrip())
	exit(1)
else:
	print("Protoc: OK")

# list generated files
out_cpp_files = list_cpp_files(out_cpp_dir)

# generate pretty cmake file
cmake_file = os.path.join(out_cpp_dir, "CMakeLists.txt")
with open(str(cmake_file), 'w') as f:
	f.write(
	"set(\n"
		"\tSRCS\n"
	)
	out_cpp_dir = str(os.path.relpath(out_cpp_dir))
	for out_cpp_file in out_cpp_files:
		f.write("\t\t%s\n" % (out_cpp_dir + "/" + out_cpp_file))
	f.write(
		"\tPARENT_SCOPE\n"
	")\n"
	)
