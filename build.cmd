SET build_path=%~dp0\build
SET project_path=%~dp0

mkdir %build_path%

cmake.exe -G"Visual Studio 15 2017" -A"x64" -T"host=x64" -B"%build_path%" -H"%project_path%"
