#include <iostream>
#include <fstream>
#include <cstdint>
using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cout << "usage: slif2tga input_file.slif output_file.tga" << endl;
		return 0;
	}

	ifstream ifs(argv[1], ios::in | ios::binary);
	ifs.seekg(0, ifs.end);
	int slifSize = (int)ifs.tellg();
	ifs.seekg(0, ifs.beg);

	if (slifSize < 12)
		return -1;

	uint32_t signature;
	ifs.read((char*)&signature, 4);
	if (signature != 0x46494C53)
		return -1;

	int8_t tgaHeader[18] = {0, 0, 2};
	tgaHeader[16] = 24;
	tgaHeader[17] = 32;
	ifs.read((char*)tgaHeader + 12, 4);

	uint32_t memorySize;
	ifs.read((char*)&memorySize, 4);

	uint8_t* memory = new uint8_t[memorySize]();
	ifs.read((char*)memory, slifSize - 12);

	if (memorySize >= 12)
	{
		uint32_t lastValidIP = memorySize - 12;
		for (uint32_t ip = 0; ip <= lastValidIP; )
		{
			uint32_t pa = *(uint32_t*)(memory + ip);
			uint32_t pb = *(uint32_t*)(memory + ip + 4);
			uint32_t pc = *(uint32_t*)(memory + ip + 8);
			int32_t a = *(int32_t*)(memory + pa);
			int32_t b = *(int32_t*)(memory + pb);
			b = b - a;
			*(int32_t*)(memory + pb) = b;
			if (b <= 0)
				ip = pc;
			else
				ip += 12;
		}
	}
	const int bitsPerPixel = 24;
	const int bytesPerPixel = bitsPerPixel / 8;
	uint16_t imageWidth = *(uint16_t*)(tgaHeader + 12);
	uint16_t imageHeight = *(uint16_t*)(tgaHeader + 14);
	int imagePixelCount = imageWidth * imageHeight;
	int imageDataSize = imagePixelCount * bytesPerPixel;
	int imageDataOffset = (int)(memorySize - imageDataSize);
	for (int i = 0; i < imagePixelCount; i++)
	{
		uint8_t* p = memory + imageDataOffset + i * bytesPerPixel;
		uint8_t r = p[0];
		uint8_t b = p[2];
		p[0] = b;
		p[2] = r;
	}

	ofstream ofs(argv[2], ios::out | ios::binary);
	ofs.write((char*)tgaHeader, 18);
	ofs.write((char*)memory + imageDataOffset, imageDataSize);
	return 0;
}
