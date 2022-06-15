:: https://docs.microsoft.com/en-us/cpp/assembler/masm/directives-reference?view=msvc-170						- MASM
:: https://docs.microsoft.com/en-us/cpp/assembler/masm/ml-and-ml64-command-line-reference?view=msvc-170			- MASM
:: https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-alphabetically?view=msvc-170	- CL
:: https://riptutorial.com/cplusplus/example/5959/compiling-with-visual-cplusplus--command-line- 				- CL/LINK

mkdir Build
cl.exe /FeBuild\Example /FoBuild\main /O2 main.c

:: With debug symbols
::cl.exe /FeBuild\Example /FoBuild\main /DEBUG /Zi /O2 main.c

Build\Example.exe
