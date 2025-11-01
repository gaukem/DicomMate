# Develop environment
## Get vcpkg package and install
*  Use power-shell with adminstrator permission:
*  PS> git clone https://github.com/Microsoft/vcpkg.git
*  PS> cd vcpkg
*  PS> git checkout 7dc284a6ad0b9cb8ccd618c8dc78c360fbd93b83
*  PS> .\bootstrap-vcpkg.bat
*  PS> .\vcpkg.exe integrate install

## Install dcmtk, openjpeg with vcpkg 
*  PS> .\vcpkg.exe install dcmtk:x64-windows-static dcmtk:x64-windows
*  PS> .\vcpkg.exe install openjpeg:x64-windows-static openjpeg:x64-windows

## Remove packages
* PS> .\vcpkg.exe remove openjpeg-uninstall:x64-windows --recurse

## Config vcpkg
![image info](https://gitlab.com/khanhpqworking/workmonitoring/-/raw/main/Client/Resource/Vcpkg-config.PNG)

## Code style
- Commit message:
[Branch Name] Commit message
