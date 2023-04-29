#include <iostream>
#include <iomanip>
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
	int64_t slifSize = ifs.tellg();
	ifs.seekg(0, ifs.beg);

	if (slifSize < 12)
	{
		cerr << "Error: file size < 12" << endl;
		return -1;
	}

	uint32_t signature;
	ifs.read((char*)&signature, 4);
	if (signature != 0x46494C53)
	{
		cerr << "Error: incorrect signature" << endl;
		return -1;
	}

	uint8_t tgaHeader[18] = {0, 0, 2};
	tgaHeader[16] = 24;
	tgaHeader[17] = 32;
	ifs.read((char*)tgaHeader + 12, 4);

	uint32_t memorySize;
	ifs.read((char*)&memorySize, 4);

	int64_t fileDataSize = slifSize - 12;
	if (memorySize < fileDataSize)
	{
		cerr << "Error: memory size < file data size" << endl;
		return -1;
	}

	const int bitsPerPixel = 24;
	const int bytesPerPixel = bitsPerPixel / 8;
	uint16_t imageWidth = *(uint16_t*)(tgaHeader + 12);
	uint16_t imageHeight = *(uint16_t*)(tgaHeader + 14);
	uint32_t imagePixelCount = imageWidth * imageHeight;
	uint64_t imageDataSize = uint64_t(imagePixelCount) * bytesPerPixel;

	if (memorySize < imageDataSize)
	{
		cerr << "Error: memory size < image data size" << endl;
		return -1;
	}

	uint8_t* memory = new(nothrow) uint8_t[memorySize]();
	if (memory == nullptr)
	{
		cerr << "Error: not enough memory" << endl;
		return -1;
	}

	ifs.read((char*)memory, fileDataSize);

	if (memorySize >= 12)
	{
		uint32_t lastValidIP = memorySize - 12;
		uint32_t lastValidPointer = memorySize - 4;
		for (uint32_t ip = 0; ip <= lastValidIP; )
		{
			uint32_t pa = *(uint32_t*)(memory + ip);
			uint32_t pb = *(uint32_t*)(memory + ip + 4);
			uint32_t pc = *(uint32_t*)(memory + ip + 8);
			if (pa > lastValidPointer || pb > lastValidPointer)
			{
				cerr << setfill('0') << right << hex;
				cerr << "Error: out of bounds access at 0x" << setw(8) << ip << ':' << endl;
				cerr << "  pa = 0x" << setw(8) << pa << ", ";
				cerr << "pb = 0x" << setw(8) << pb << ", ";
				cerr << "pc = 0x" << setw(8) << pc << endl;
				return -1;
			}
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

	uint32_t imageDataOffset = uint32_t(memorySize - imageDataSize);
	for (uint32_t i = 0; i < imagePixelCount; i++)
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
