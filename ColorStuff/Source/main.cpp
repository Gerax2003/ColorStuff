
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

// normal ints are read in little endian, this reads in big endian (for PNGs)
int32_t ReadInt(std::ifstream& pic)
{
	uint8_t c[4];
	pic.read((char*)c, 4);

	return static_cast<int32_t>(
		(c[3]      )|
		(c[2] << 8 )|
		(c[1] << 16)|
		(c[0] << 24));
}

int32_t IntFromData(const uint8_t* c, int offset = 0)
{
	return static_cast<int32_t>(
		(c[3+offset]) |
		(c[2+offset] << 8) |
		(c[1+offset] << 16) |
		(c[0+offset] << 24));
}

void GetSignature(std::ifstream& pic)
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

void GetChunk(std::ifstream& pic)
{
	int32_t chkLen = ReadInt(pic);
	uint8_t chkType[4];
	pic.read((char*)chkType, 4);

	std::string strType((char*)(chkType), 4);
	//strType += "\0";

	// using uint8 to avoid breaks when printing hexa values
	std::vector<uint8_t> chkData(chkLen);
	pic.read((char*)(chkData.data()), chkLen);

	int32_t chkCRC = ReadInt(pic);

	std::cout << strType << " | ";
	for (int i = 0; i < 4; i++)
	{
		int32_t val = chkType[i];
		std::cout << std::format("{:#04X} ", val) << " ";
	}
	std::cout << "\nLength: (" << chkLen << "; " << std::format("{:#010X} ", chkLen) << ")" << std::endl;

	//std::cout << chkLen << " bytes of data in this chunk" << std::endl;

	/*for (int i = 0; i < chkLen; i++)
	{
		int32_t val = chkData[i];
		std::cout << std::format("{:#04X} ", val) << " ";
	}*/
	std::cout << std::endl;


	// 1st chunk
	if (strType == "IHDR")
	{
		std::cout << "Image dimesions are " << IntFromData(chkData.data()) << "x" << IntFromData(chkData.data(), 4) << std::endl;
		std::cout << "Bits per channel: " << (int)chkData[8] << "; Color type " << (int)chkData[9] << std::endl;
		std::cout << "Compression: " << (int)chkData[10] << "; Filter: " << (int)chkData[11] << "; Interlaced: " << (int)chkData[11] << std::endl;
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
	std::cout << std::endl << "CRC: (" << chkCRC << "; " << std::format("{:#010X} ", chkCRC) << ")" << std::endl;
}

int main(int argc, char* argv[])
{
	std::ifstream pic;
	std::cout << "Current path is " << std::filesystem::current_path() << '\n';
	pic.open("Resources/zarro.png", std::ifstream::in | std::ifstream::binary);
	
	if (!pic.is_open())
		return -1;

	GetSignature(pic);
	while (pic.is_open())
		GetChunk(pic);

	/*int i = 1;
	int byte = pic.get();
	std::string str;
	while (pic.good()) 
	{
		std::cout << std::format("{:#04X} ", byte);
		str += std::format("{:#04X} ", byte);

		if (i >= 8)
		{
			std::cout << std::endl;
			str += "\n";
			i = 0;
		}

		byte = pic.get();
		i++;
	}*/

	if (pic.is_open())
		pic.close();

	/*std::ofstream txt;
	txt.open("Resources/png.txt", std::ofstream::out);
	txt << str;
	txt.close();*/

	return 0;
}
