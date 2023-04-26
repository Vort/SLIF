using System;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Threading;

namespace SLIF
{
    class Program
    {
        const uint signature = 0x46494C53;
        const ushort imageWidth = 512;
        const ushort imageHeight = 512;
        const uint memorySize = 256 * 1024 * 1024;
        const int bitsPerPixel = 24;
        const int bytesPerPixel = bitsPerPixel / 8;
        const long errorThreshold = 20358302;
        const long operationCountThreshold = 100000000000;

        static byte[] Decompress(byte[] fileData, ref long operationCount)
        {
            var ms = new MemoryStream(fileData);
            var br = new BinaryReader(ms);
            if (br.ReadUInt32() != signature)
                throw new Exception();
            if (br.ReadUInt16() != imageWidth)
                throw new Exception();
            if (br.ReadUInt16() != imageHeight)
                throw new Exception();
            if (br.ReadUInt32() != memorySize)
                throw new Exception();

            // Initialized with zeros
            byte[] memory = new byte[memorySize];
            br.Read(memory, 0, fileData.Length - 12);

            ms = new MemoryStream(memory);
            br = new BinaryReader(ms);
            var bw = new BinaryWriter(ms);

            const uint lastValidIP = memorySize - 12;
            for (uint ip = 0; ip <= lastValidIP; )
            {
                ms.Seek(ip, SeekOrigin.Begin);
                uint pa = br.ReadUInt32();
                uint pb = br.ReadUInt32();
                uint pc = br.ReadUInt32();

                ms.Seek(pa, SeekOrigin.Begin);
                int a = br.ReadInt32();
                ms.Seek(pb, SeekOrigin.Begin);
                int b = br.ReadInt32();

                unchecked { b = b - a; }
                ms.Seek(pb, SeekOrigin.Begin);
                bw.Write(b);

                if (b <= 0)
                    ip = pc;
                else
                    ip += 12;

                checked { operationCount++; }
            }

            int imageDataOffset = (int)(memorySize - imageWidth * imageHeight * bytesPerPixel);
            return memory.Skip(imageDataOffset).ToArray();
        }

        static void Main(string[] args)
        {
            Thread.CurrentThread.CurrentCulture = CultureInfo.InvariantCulture;
            byte[] reference = File.ReadAllBytes("reference.bin");

            long operationCount = 0;
            byte[] fileData = File.ReadAllBytes("image.slif");
            var imageData = Decompress(fileData, ref operationCount);

            long error = 0;
            for (int i = 0; i < reference.Length; i++)
            {
                int delta = reference[i] - imageData[i];
                error += delta * delta;
            }

            bool psnrGood = error < errorThreshold;
            bool operationCountGood = operationCount < operationCountThreshold;
            bool success = psnrGood && operationCountGood;

            Console.ForegroundColor = success ? ConsoleColor.Green : ConsoleColor.Red;
            Console.WriteLine(success ? "Success" : "Failure");
            Console.ResetColor();

            double psnr = 10 * Math.Log10(65025.0 * imageWidth * imageHeight * bytesPerPixel / error);
            string psnrStr = psnr.ToString("0.0000000");
            Console.WriteLine("File size: " + fileData.Length);
            Console.Write("PSNR: ");
            Console.ForegroundColor = psnrGood ? ConsoleColor.Green : ConsoleColor.Red;
            Console.WriteLine(psnrStr);
            Console.ResetColor();
            Console.Write("Operation count: ");
            Console.ForegroundColor = operationCountGood ? ConsoleColor.Green : ConsoleColor.Red;
            Console.WriteLine(operationCount);
            Console.ResetColor();
        }
    }
}
