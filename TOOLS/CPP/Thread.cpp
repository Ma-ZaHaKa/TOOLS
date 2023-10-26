#pragma once
#include "Windows.h" // ������ ����� ������


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
		DisableThreadLibraryCalls(hinstDLL); // ��������� �������� ����������

		// ��������� ���� � ������� DLL
		char dllPath[MAX_PATH];
		GetModuleFileName(hinstDLL, dllPath, MAX_PATH);

		// ���������� ���� �� ����������
		char* lastBackslash = strrchr(dllPath, '\\');
		if (lastBackslash)
			*lastBackslash = '\0'; // ������� ��� ����� �� ����
		std::string* dllPathStr = new std::string(dllPath);


		HANDLE ThreadEntryHandle = CreateThread(NULL, 0, ThreadEntry, dllPathStr, 0, NULL);
	}

	return TRUE;
}