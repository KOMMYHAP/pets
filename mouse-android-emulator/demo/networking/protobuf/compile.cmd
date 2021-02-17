@echo off
set ROOT_DIR=%~dp0..\..\..\..
set PROTOBUF_DIR=%ROOT_DIR%\external\protobuf\protoc-3.14.0-win64\bin
set CPP_OUT_DIR=%~dp0\packets-cpp

for %%i in (packets/*.proto) do (
	%PROTOBUF_DIR%/protoc.exe --cpp_out="%CPP_OUT_DIR%" packets/%%i
)
