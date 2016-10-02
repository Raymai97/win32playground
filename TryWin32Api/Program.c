#include <stdio.h>
#include <Windows.h>

int main(int argc, char **argv) {
	FARPROC x = NULL;
	char *szDll = NULL, *szApi = NULL;
	if (argc > 1) { szDll = argv[1]; }
	if (argc > 2) { szApi = argv[2]; }
	if (!szDll) {
		szDll = malloc(200);
		printf("DLL? : ");
		gets(szDll);
	}
	if (!szApi) {
		szApi = malloc(200);
		printf("API? : ");
		gets(szApi);
	}
	x = GetProcAddress(LoadLibraryA(szDll), szApi);
	printf("\n%s\n", x ? "Success!" : "Failed...");
	return x ? 0 : 1;
}
