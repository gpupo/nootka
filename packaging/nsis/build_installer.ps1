# Powershell

echo "--- Getting ZLIB"
wget -q "https://www.opencode.net/seelook/nootka-build/-/raw/master/3rdParty/zlib.zip"
Expand-Archive -Path zlib.zip -DestinationPath zlib
Move-Item -Path .\zlib\zlib\*.* -Destination .\zlib\

echo "--- Getting 3rd party libraries (FFTW, OGG, VORBIS)"
wget -q "https://www.opencode.net/seelook/nootka-build/-/raw/master/3rdParty/win32-fftw-ogg-vorbis.tar.gz"
tar -xzf .\win32-fftw-ogg-vorbis.tar.gz

mkdir build
cd build

cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=installs ../

make -j2

mkdir installs
make install
make deploy

echo "--- Building installer"
makensis installs/nootka-utf16.nsi

Move-Item -Path .\installs\Nootka-*-Windows-Installer.exe -Destination ..\Nootka-Installer.exe

cd ..
ls