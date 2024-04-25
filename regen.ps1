$env:SOURCE=".\src\main.c"
rm .\Build\x64-debug\ -r -force 2>$null
rm .\Build\x64-release\ -r -force 2>$null
clear
cmake --preset x64-release
if ($LASTEXITCODE -eq 0) {
	clear
	cmake --build --preset x64-release
}
