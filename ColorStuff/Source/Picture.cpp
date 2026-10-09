
#include "Picture.h"
#include "Constants.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <filesystem>
#include <regex>

void IntToByte(char* bytes, int v)
{
	bytes[0] = v >> 0;
	bytes[1] = v >> 8;
	bytes[2] = v >> 16;
	bytes[3] = v >> 24;
}

void ColorToByte(char* bytes, RGBColor v, bool invertRGB = false)
{
	if (invertRGB)
	{
		bytes[2] = v.r >> 0;
		bytes[1] = v.g >> 0;
		bytes[0] = v.b >> 0;
		bytes[3] = v.a >> 0;
	}
	else
	{
		bytes[0] = v.r >> 0;
		bytes[1] = v.g >> 0;
		bytes[2] = v.b >> 0;
		bytes[3] = v.a >> 0;
	}
}

void Picture::Open(const char* path)
{
	// ... process data if not NULL ...
	// ... x = width, y = height, n = # 8-bit components per pixel ...
	// ... replace '0' with '1'..'4' to force that many components per pixel
	// ... but 'n' will always be the number that it would have been if you said 0
	unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

	if (data == nullptr || channels < 3)
	{
		stbi_image_free(data);
		return;
	}

	std::cout << "Image dimesions are " << width << "x" << height << " (" << channels << " channels)" << std::endl;

	pixels.resize(width * height);

	for (int i = 0; i < width * height; i++)
	{
		pixels[i].r = data[i * channels];
		pixels[i].g = data[i * channels + 1];
		pixels[i].b = data[i * channels + 2];
	
		if (channels = 4)
			pixels[i].a = data[i * channels + 3];
	}

	stbi_image_free(data);	
}

void Picture::WritePicture(const char* fileName, const PictureFormat format)
{
	switch (format)
	{
	case PictureFormat::PAM:
		WritePAM(fileName);
		break;
	case PictureFormat::BMP:
		WriteBMP(fileName);
		break;
	default:
		WritePAM(fileName);
		break;
	}
}

// Initially meant to be P6 PPM but only PAM supports alpha (PAM is barely supported LMAOOOOOOOOO)
void Picture::WritePAM(const char* fileName)
{
	std::string path = "Output/";
	path += fileName;
	path += ".pam";
	std::cout << "Writing picture at " << path << std::endl;

	std::ofstream txt;
	txt.open(path, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
	if (!txt.is_open())
	{
		std::cout << "Error writing picture at " << path << std::endl;
		return;
	}

	// header
	txt << "P7\nWIDTH " << width
		<< "\nHEIGHT " << height
		<< "\nMAXVAL 255\nDEPTH 4\nTUPLTYPE RGB_ALPHA\nENDHDR\n";

	// From now on 4 is the channels, we will always do rgba for convenience even when it's pointless
	std::vector<char> bytes;
	bytes.resize(4 * height * width);

	for (int i = 0; i < pixels.size(); i++)
		ColorToByte(&bytes[i * 4], pixels[i]);

	txt.write(&bytes[0], bytes.size() * sizeof(char));

	txt.close();

	std::cout << "Success writing picture at " << path << std::endl;
}

void Picture::WriteBMP(const char* fileName)
{
	std::string path = "Output/";
	path += fileName;
	path += ".bmp";
	std::cout << "Writing picture at " << path << std::endl;

	std::ofstream txt;
	txt.open(path, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
	if (!txt.is_open())
	{
		std::cout << "Error writing picture at " << path << std::endl;
		return;
	}

	char byte[4];

	// header based on https://cplusplus.com/forum/beginner/4307/
	txt << 'B' << 'M';

	// FORMAT HEADER
	// full file size, both headers + h*w*c for pic size in bytes
	IntToByte(byte, BMP_HDR_SIZE + BITMAPINOFHEADER_SIZE + height * width * channels);
	txt.write(byte, 4 * sizeof(char));
	IntToByte(byte, 0); // header uses 4 reserved zeros 
	txt.write(byte, 4 * sizeof(char));
	IntToByte(byte, BMP_HDR_SIZE + BITMAPINOFHEADER_SIZE); // offset where the pic starts
	txt.write(byte, 4 * sizeof(char));

	// BITMAPINOFHEADER (https://en.wikipedia.org/wiki/BMP_file_format#DIB_header)
	IntToByte(byte, BITMAPINOFHEADER_SIZE); // 40 bytes size for this header
	txt.write(byte, 4 * sizeof(char));
	IntToByte(byte, width); // width, positive because microsoft aint that stupid
	txt.write(byte, 4 * sizeof(char));
	IntToByte(byte, -height); // Negative height because somehow writing the picture bottom row first as specified by the format still makes it flipped MICROSOOOOOOOOOOOOFT (could be because I put the end of the headers as my offset, maybe positive needs the EOF?)
	txt.write(byte, 4 * sizeof(char));
	IntToByte(byte, 1); // IMPORTANT: 2 bytes set to number 1, might need to inverse order here
	txt.write(byte, 2 * sizeof(char));
	IntToByte(byte, channels * 8); // bits/pixel, since chans are 3 or 4 for now c*8 works
	txt.write(byte, 2 * sizeof(char));
	IntToByte(byte, 0); // Compression: 0 means no compression, we write RAW in this house
	txt.write(byte, 4 * sizeof(char));
	IntToByte(byte, channels * height * width * sizeof(char)); // image size in bytes
	txt.write(byte, 4 * sizeof(char));
	IntToByte(byte, 0); // PPMX, PPMY, Color Table & important colors all set to 0 for unspecified
	txt.write(byte, 4 * sizeof(char));
	txt.write(byte, 4 * sizeof(char));
	txt.write(byte, 4 * sizeof(char)); // actually this one might need a value for monochrome layers later
	txt.write(byte, 4 * sizeof(char));

	std::vector<char> bytes;
	bytes.resize(channels * height * width);

	// a single loop works but akshually bmp should be stored bottom line first, somehow it didn't work and -height was the fix have I told you how much I loathe these goofy "simple format easy to dev frfr" quirks
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			ColorToByte(&bytes[j * 4 + i * width * 4], pixels[j + i * width], true);
	
	txt.write(&bytes[0], bytes.size() * sizeof(char));

	txt.close();

	std::cout << "Success writing picture at " << path << std::endl;
}

#pragma region OLD_CLASS
void Picture_Old::Open(const char* path)
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

void Picture_Old::MakeTxt(const char* path)
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

void Picture_Old::GetSignature()
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

void Picture_Old::GetChunk()
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
#pragma endregion
