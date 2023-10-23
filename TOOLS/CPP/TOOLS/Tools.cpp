#pragma once
#include "Tools.h"


// in header
//template <typename T> T ReadPtr(void* ptr) { return *static_cast<const T*>(ptr); }
//void* IntADDR2VoidPtr(int _addr) { return reinterpret_cast<void*>(_addr); }



//DIRECTORY
bool DirectoryExists(std::string& folderPath)
{
	struct stat info;
	if (stat(folderPath.c_str(), &info) != 0) { return false; }
	return (info.st_mode & S_IFDIR) != 0;
}

//bool DirectoryCreate(std::string& folderPath)
//{
//	// Use the mkdir function to create the directory
//	if (mkdir(folderpath.c_str(), s_irwxu | s_irwxg | s_iroth | s_ixoth) != 0) {
//		return false; // error creating directory
//	}
//	return true; // directory created successfully
//}

std::vector<std::string> DirectoryGetAllFilesByExtention(std::string& folderPath, std::string& fileExtension)
{
	if (folderPath.back() != '\\' && folderPath.back() != '/') { folderPath += "\\"; }

	std::vector<std::string> matchingFiles;
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile((folderPath + "*." + fileExtension).c_str(), &findFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::string filePath = folderPath + findFileData.cFileName;
			matchingFiles.push_back(filePath);
		} while (FindNextFile(hFind, &findFileData) != 0);

		FindClose(hFind);
	}

	return matchingFiles;
}

std::vector<std::string> DirectoryGetAllOGGFiles(std::string& folderPath)
{
	if (folderPath.back() != '\\' && folderPath.back() != '/') { folderPath += "\\"; }

	std::vector<std::string> oggFiles; // Создаем вектор для хранения путей к .ogg файлам
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile((folderPath + "*.ogg").c_str(), &findFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::string filePath = folderPath + findFileData.cFileName;
			oggFiles.push_back(filePath);
			//std::cout << "ScanFile: " << filePath << "\n";
		} while (FindNextFile(hFind, &findFileData) != 0);

		FindClose(hFind);
	}

	return oggFiles;
}

std::vector<std::string> DirectoryGetAllFiles(std::string& directoryPath)
{
	std::vector<std::string> files;
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile((directoryPath + "\\*").c_str(), &findFileData);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				std::string filePath = directoryPath + "\\" + findFileData.cFileName;
				files.push_back(filePath);
			}
		} while (FindNextFile(hFind, &findFileData) != 0);

		FindClose(hFind);
	}
	else {
		std::cerr << "Error accessing the directory." << std::endl;
	}

	return files;
}


//FILE
bool FileExists(std::string& filePath)
{
	std::ifstream file(filePath);
	return file.good();
}

void FileWrite(std::string& filePath, std::string& text)
{
	// Open the file for writing. If it doesn't exist, create it.
	std::ofstream file(filePath, std::ios::out | std::ios::trunc);

	if (file.is_open()) {
		// Write the text to the file.
		file << text;

		// Close the file.
		file.close();
		//std::cout << "File written successfully." << std::endl;
	}
	//else {
	//	std::cerr << "Error opening or creating the file." << std::endl;
	//}
}

bool FileRemove(std::string& filePath)
{
	if (std::remove(filePath.c_str()) == 0) {
		//std::cout << "File removed successfully." << std::endl;
		return true;
	}
	else {
		//std::cerr << "Error removing the file." << std::endl;
		return false;
	}
}

std::vector<std::string> FileReadAllLines(std::string& filePath)
{
	std::vector<std::string> lines;
	std::ifstream file(filePath);

	if (!file.is_open()) { return lines; }

	std::string line;
	while (std::getline(file, line)) {
		lines.push_back(line);
	}

	file.close();
	return lines;
}

std::string FileReadAllText(std::string& filePath)
{
	std::ifstream file(filePath);
	std::string content;

	if (!file.is_open()) {
		//std::cerr << "Error opening the file." << std::endl;
		return content; // Return an empty string in case of an error.
	}

	std::string line;
	while (std::getline(file, line)) {
		content += line + '\n'; // Concatenate the line and a newline character.
	}

	file.close();
	return content;
}



/*bool MkINI(std::string& pfile)
{
	std::ofstream outfile(pfile);
	if (outfile.is_open()) {

		outfile << ";shance 6 star" << std::endl;
		outfile << "40" << std::endl;
		outfile << std::endl;

		outfile.close();
		return true;
	}
	return false;
}*/



std::vector<std::string> split(std::string s, std::string delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

std::string StringToUpper(std::string strToConvert)
{
	std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);
	return strToConvert;
}

// std::mt19937
int _RandVUKL(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(min, max - 1); // Используем max - 1, чтобы исключить верхнее значение
	return distribution(gen);
}
int _RandVKL(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(gen);
}
bool Shance(int shance, int maxshance)
{
	int randomValue = _RandVKL(0, maxshance);
	return randomValue <= shance;
}
/*int _RandVUKL(int min, int max) {
	// Инициализируем генератор случайных чисел с использованием текущего времени
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Генерируем случайное число в диапазоне [min, max - 1]
	return min + std::rand() % (max - min);
}

int _RandVKL(int min, int max) {
	// Инициализируем генератор случайных чисел с использованием текущего времени
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Генерируем случайное число в диапазоне [min, max]
	return min + std::rand() % (max - min + 1);
}*/


//std::vector<std::string> parseFile(std::string& filePath) // rows to strings
//{
//	std::vector<std::string> lines;
//	std::ifstream file(filePath);
//
//	if (!file.is_open()) {
//		std::ofstream createFile(filePath); //mkfile
//		createFile.close();
//		return lines;
//	}
//
//	std::string line;
//	while (std::getline(file, line)) {
//		if (!line.empty()) {
//			lines.push_back(line);
//		}
//	}
//
//	file.close();
//	return lines;
//}


void savepDataToArray(int* pArr, void* pData, size_t dataSize)
{
	/*if (dataSize != sizeof(int) * dataSize) { //??
		std::cerr << "Error: Data size mismatch." << std::endl;
		return;
	}*/
	unsigned char* pDataBytes = static_cast<unsigned char*>(pData);

	for (size_t i = 0; i < dataSize; ++i) {
		int asciiValue = static_cast<int>(pDataBytes[i]);
		pArr[i] = asciiValue;
	}
}
void saveArrayToFile(int* pArr, std::fstream& WfileStream, size_t dataSize)
{
	if (!WfileStream.is_open()) {
		std::cerr << "Error: File stream is not open for writing." << std::endl;
		return;
	}

	for (size_t i = 0; i < dataSize; ++i) {
		WfileStream << pArr[i] << std::endl;
	}
}


bool parseFileToGlobalIntArray(int* pArr, std::fstream& RfileStream, size_t dataSize)
{
	if (!RfileStream.is_open()) {
		std::cerr << "Error: File stream is not open for reading." << std::endl;
		return false;
	}

	for (size_t i = 0; i < dataSize; ++i) {
		int value;
		if (!(RfileStream >> value)) {
			std::cerr << "Error: Failed to read an integer from the file." << std::endl;
			return false;
		}

		pArr[i] = value;
	}

	return true;
}

bool parseGlobalIntArrayTopData(int* pArr, void* pData, size_t dataSize)
{
	/*if (dataSize != sizeof(int) * dataSize) { //??
		std::cerr << "Error: Data size mismatch." << std::endl;
		return false;
	}*/
	char* pDataBytes = static_cast<char*>(pData);

	for (size_t i = 0; i < dataSize; ++i) {
		pDataBytes[i] = static_cast<char>(pArr[i]);
	}

	return true;
}




std::string savepDataToBinaryString(void* pData, size_t dataSize)
{
	unsigned char* pDataBytes = static_cast<unsigned char*>(pData);
	std::string binaryString;

	for (size_t i = 0; i < dataSize; ++i) {
		binaryString += pDataBytes[i];
	}

	return binaryString;
}

void parseBinaryStringToData(std::string& binaryString, void* pData, size_t dataSize)
{
	if (binaryString.size() != dataSize) {
		std::cerr << "Error: Data size mismatch." << std::endl;
		return;
	}

	const char* binaryData = binaryString.c_str();
	unsigned char* pDataBytes = static_cast<unsigned char*>(pData);

	for (size_t i = 0; i < dataSize; ++i) {
		pDataBytes[i] = binaryData[i];
	}
}







// Function to encode a string to Base64
std::string base64_encode(const std::string& input) {
	static const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string encoded;
	int i = 0;
	int j = 0;
	uint8_t char_array_3[3];
	uint8_t char_array_4[4];

	for (char c : input) {
		char_array_3[i++] = c;
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; i < 4; i++) {
				encoded += base64_chars[char_array_4[i]];
			}
			i = 0;
		}
	}

	if (i > 0) {
		for (int k = i; k < 3; k++) {
			char_array_3[k] = '\0';
		}

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

		for (int k = 0; k < i + 1; k++) {
			encoded += base64_chars[char_array_4[k]];
		}

		while (i++ < 3) {
			encoded += '=';
		}
	}

	return encoded;
}

// Function to decode a Base64 encoded string
std::string base64_decode(const std::string& encoded) {
	static const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string decoded;
	int i = 0;
	int j = 0;
	uint8_t char_array_4[4];
	uint8_t char_array_3[3];

	for (char c : encoded) {
		if (c == '=') {
			break;
		}

		if (base64_chars.find(c) != std::string::npos) {
			char_array_4[i++] = c;
		}

		if (i == 4) {
			for (i = 0; i < 4; i++) {
				char_array_4[i] = base64_chars.find(char_array_4[i]);
			}

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0x0f) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x03) << 6) + char_array_4[3];

			for (i = 0; i < 3; i++) {
				decoded += char_array_3[i];
			}
			i = 0;
		}
	}

	if (i > 0) {
		for (int k = i; k < 4; k++) {
			char_array_4[k] = 0;
		}

		for (i = 0; i < 4; i++) {
			char_array_4[i] = base64_chars.find(char_array_4[i]);
		}

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0x0f) << 4) + ((char_array_4[2] & 0x3c) >> 2);

		for (int k = 0; k < i - 1; k++) {
			decoded += char_array_3[k];
		}
	}

	return decoded;
}

// Function to URL encode a string
std::string url_encode(const std::string& input) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (char c : input) {
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
		}
		else if (c == ' ') {
			escaped << '+';
		}
		else {
			escaped << '%' << std::setw(2) << int((unsigned char)c);
		}
	}

	return escaped.str();
}

// Function to URL decode a string
std::string url_decode(const std::string& input) {
	std::ostringstream decoded;

	for (size_t i = 0; i < input.length(); ++i) {
		if (input[i] == '%') {
			if (i + 2 < input.length()) {
				std::istringstream hex(input.substr(i + 1, 2));
				int value;
				hex >> std::hex >> value;
				decoded << static_cast<char>(value);
				i += 2;
			}
		}
		else if (input[i] == '+') {
			decoded << ' ';
		}
		else {
			decoded << input[i];
		}
	}

	return decoded.str();
}

std::string Repeat(char character, int count) {
	std::string result;
	for (int i = 0; i < count; i++) {
		result += character;
	}
	return result;
}


static void Mbox(const char* msg, const char* title)
{
	MessageBoxA(HWND_DESKTOP, msg, title, MB_SYSTEMMODAL | MB_ICONWARNING);
}


float _max(float a, float b) { return (((a) > (b)) ? (a) : (b)); }
float _min(float a, float b) { return (((a) < (b)) ? (a) : (b)); }

HANDLE InitConsole() // with proto
{
	AllocConsole();

	//SetConsoleOutputCP(866);
	setlocale(LC_ALL, "Russian");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);


	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

	return hConsole;
}

//void LeaveConsole(HANDLE hConsole = nullptr)
void LeaveConsole(HANDLE hConsole) // with proto
{
	if (hConsole != nullptr) { SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); } // Reset to default color
	FreeConsole();
}