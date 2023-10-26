#pragma once
#include "Windows.h" // всегда самым первым


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <TlHelp32.h>



DWORD CALLBACK ThreadEntry(LPVOID)
{

	return TRUE;
}

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL); // ускорение загрузки библиотеки

		// Получение пути к текущей DLL
		char dllPath[MAX_PATH];
		GetModuleFileName(hinstDLL, dllPath, MAX_PATH);

		// Извлечение пути до директории
		char* lastBackslash = strrchr(dllPath, '\\');
		if (lastBackslash)
			*lastBackslash = '\0'; // Убираем имя файла из пути
		std::string* dllPathStr = new std::string(dllPath);


		HANDLE ThreadEntryHandle = CreateThread(NULL, 0, ThreadEntry, dllPathStr, 0, NULL);
	}

	return TRUE;
}