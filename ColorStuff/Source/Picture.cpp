
#include "Picture.h"

#include <iostream>
#include <filesystem>
#include <vector>
#include <regex>

void Picture::Open(const char* path)
{
	pic.open(path, std::ifstream::in | std::ifstream::binary);

	if (!pic.is_open())
		return;

	GetSignature();
	while (pic.is_open())
		GetChunk();

	if (pic.is_open())
		pic.close();
}

void Picture::MakeTxt(const char* path)
{
	std::cout << "Current path is " << std::filesystem::current_path() << '\n';
	pic.open(path, std::ifstream::in | std::ifstream::binary);

	if (!pic.is_open())
		return;

	int i = 1;
	int byte = pic.get();
	std::string str;
	while (pic.good())
	{
		//std::cout << std::format("{:#04X} ", byte);
		str += std::format("{:#04X} ", byte);

		if (i >= 8)
		{
			std::cout << std::endl;
			str += "\n";
			i = 0;
		}

		byte = pic.get();
		i++;
	}

	if (pic.is_open())
		pic.close();

	std::string txtPath(path);
	txtPath = std::regex_replace(txtPath, std::regex(".png"), ".txt");;
	std::ofstream txt;
	txt.open(txtPath, std::ofstream::out);
	txt << str;
	txt.close();
}


void Picture::GetSignature()
{
	int bytes[8] = {};

	for (int i = 0; i < 8; i++)
	{
		bytes[i] = pic.get();
		if (i == 0 || i > 3)
			std::cout << std::format("{:#04X} ", bytes[i]);
		else
			std::cout << (char)(bytes[i]) << " ";
	}

	std::cout << std::endl;
}

void Picture::GetChunk()
{
	int32_t chkLen = ReadInt();
	uint8_t chkType[4];
	pic.read((char*)chkType, 4);

	std::string strType((char*)(chkType), 4);

	// using uint8 to avoid breaks when printing hexa values
	std::vector<uint8_t> chkData(chkLen);
	pic.read((char*)(chkData.data()), chkLen);

	int32_t chkCRC = ReadInt();

	std::cout << strType << " | ";
	for (int i = 0; i < 4; i++)
	{
		int32_t val = chkType[i];
		std::cout << std::format("{:#04X} ", val) << " ";
	}
	std::cout << "\nLength: (" << chkLen << "; " << std::format("{:#010X} ", chkLen) << ")" << std::endl;
	std::cout << "CRC: (" << chkCRC << "; " << std::format("{:#010X} ", chkCRC) << ")" << std::endl;

	/*for (int i = 0; i < chkLen; i++)
	{
		int32_t val = chkData[i];
		std::cout << std::format("{:#04X} ", val) << " ";
	}*/

	// 1st chunk
	if (strType == "IHDR")
	{
		dims.width = IntFromData(chkData.data());
		dims.height = IntFromData(chkData.data(), 4);
		compression = (int)chkData[10];
		filter = (int)chkData[11];
		interlaced = (int)chkData[12];


		std::cout << "Image dimesions are " << dims.width << "x" << dims.height << std::endl;
		std::cout << "Bits per channel: " << (int)chkData[8] << "; Color type " << (int)chkData[9] << std::endl;
		std::cout << "Compression: " << compression << "; Filter: " << filter << "; Interlaced: " << interlaced << std::endl;
	}
	// chunk of data
	else if (strType == "IDAT")
	{

	}
	// final chunk
	else if (strType == "IEND")
	{
		pic.close();
	}
	std::cout << "--------------" << std::endl;
}
