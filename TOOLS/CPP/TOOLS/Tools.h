#pragma once
#include "Windows.h"
#include "Tools.h"
#include "Internet.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <string>

#include <random>


// ������������� ������� ������ template
//template <typename T> T ReadPtr(void* ptr);
//void* IntADDR2VoidPtr(int _addr);

//template <typename T> T ReadPtr(void* ptr) { return *static_cast<const T*>(ptr); }
//void* IntADDR2VoidPtr(int _addr) { return reinterpret_cast<void*>(_addr); }
//int VoidPtrToInt(void* _addr) { return reinterpret_cast<int>(_addr); }


//DIRECTORY
bool DirectoryExists(std::string& folderPath);

//bool DirectoryCreate(std::string& folderPath);

std::vector<std::string> DirectoryGetAllFilesByExtention(std::string& folderPath, std::string& fileExtension);

std::vector<std::string> DirectoryGetAllOGGFiles(std::string& folderPath);

std::vector<std::string> DirectoryGetAllFiles(std::string& directoryPath);


//FILE
bool FileExists(std::string& filePath);

void FileWrite(std::string& filePath, std::string& text);

bool FileRemove(std::string& filePath);

std::vector<std::string> FileReadAllLines(std::string& filePath);

std::string FileReadAllText(std::string& filePath);





std::vector<std::string> split(std::string s, std::string delimiter);
std::string StringToUpper(std::string strToConvert);

int _RandVUKL(int min, int max);
int _RandVKL(int min, int max);
bool Shance(int shance, int maxshance);


//std::vector<std::string> parseFile(std::string& filePath); // FileReadAllLines


std::string savepDataToBinaryString(void* pData, size_t dataSize);
void parseBinaryStringToData(const std::string& binaryString, void* pData, size_t dataSize);
std::string Repeat(char character, int count);


bool parseGlobalIntArrayTopData(int* pArr, void* pData, size_t dataSize);
bool parseFileToGlobalIntArray(int* pArr, std::fstream& RfileStream, size_t dataSize);
void saveArrayToFile(int* pArr, std::fstream& WfileStream, size_t dataSize);
void savepDataToArray(int* pArr, void* pData, size_t dataSize);




std::string url_encode(const std::string& input);
std::string base64_decode(const std::string& encoded);
std::string base64_encode(const std::string& input);
std::string url_decode(const std::string& input);


static void Mbox(const char* msg, const char* title);
float _max(float a, float b);
float _min(float a, float b);

HANDLE InitConsole();
void LeaveConsole(HANDLE hConsole);

bool DKSleep(float deltaTime, float wait_time, float& sleepBuffer);

//std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
//float SleepBuffer = 0.0f;
//
//
//Events::drawHudEvent += []
//{
//	//--calc fps
//	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
//	std::chrono::duration<float> deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - previousTime);
//	previousTime = currentTime;
//
//	if (DKSleep(deltaTime.count(), delayDuration, SleepBuffer)) { canSetWantedLevel = true; } // true ����� ������ ������
//
//}