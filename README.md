This repository contains the challenge of creating an image codec.

In this challenge you need to create `image.slif` file, which contains compressed image. Such file should consist of two parts - header, which is constant for this challenge and subleq machine code, which produce image data when interpreted by test program. Valid solution to this challenge must contain file by itself. Providing description of how it was created is optional.

To win this challenge, your file should have smallest size in bytes (this is your score), produce image of decent quality and its execution should not take too much time. (See below for how these requirements are defined)

Image data is encoded as array of bytes, each byte represents intensity of red, green or blue component of pixel color. Since image size for this challenge is constant (512 x 512 pixels), resulting total size of array becomes 512 * 512 * 3 = 786432 bytes. Pixels are encoded from left to right, from top to bottom in R G B order.

Quality of image is calculated as sum of squared differences between byte values of constant reference image data (`reference.bin`) and image data, produced by subleq code. For your solution to be valid, such sum should be lower than 20358302.

Second requirement for solution is amount of executed operations during image decoding. It introduced to limit time, which is required for solution validation. For your solution to be valid, operation count should be lower than 100000000000. It corresponds to several hours of calculations with existing interpreter implementation.

Constant header of `image.slif` is 12 bytes in size and equal to 53 4C 49 46 00 02 00 02 00 00 00 10. It can be split to 4 parts: signature (4 bytes), image width (2 bytes), image height (2 bytes) and available memory size (4 bytes).

Variable data, which defines your solution, consists of subleq instructions, which also can be viewed as pure data. Each instruction have size of 12 bytes and consists of 3 addresses - `pa`, `pb`, `pc`. Each address is encoded as 32 bit little-endian byte index. Address 0 points to first byte of first instruction, which will be first operation executed. Last accessible byte have address 0xFFFFFFF (256 MiB - 1 byte). However, since variables, accessed by 32 bit pointers, are also 32 bit values, to access this byte, extra data should be read or written, which means that last valid value for pointer is 0xFFFFFFC. Reading and writing to addresses beyond it is not allowed. Transferring execution to addresses 0x10000000 and beyond is allowed and will result in program termination. Execution of each instruction consists or reading values from addresses `pa` and `pb` to variables `a` and `b`, subtraction of `a` from `b`, storing result by address `pb` and jumping to address `pc` if `b - a` is less or equal to zero or to address equal to instruction pointer + 12 otherwise. Goal of subleq program is to produce image data at addresses 0xFF40000 .. 0xFFFFFFF and terminate its execution.

Information needed to validate your solution is stored in [slif_v1.zip](https://github.com/Vort/SLIF/raw/bc6269bb978802bef4738c201b78f46353b58fb4/slif_v1.zip) archive (CRC32 779BB417). It consists of test program, which is provided in binary (`SLIF.exe`) and text (`Program.cs`) form, constant reference image data (`reference.bin`) and my solution to this challenge (`image.slif`).

To execute binary, .NET Framework is required for Windows or Mono for Linux. Test program compares image from your file with reference image. If test program shows "Success" message, then your file is good. Otherwise it is not good.

Launching test program with my solution should produce following result:
```
Success
File size: 491916
PSNR: 35.7080560
Operation count: 48804196
```

To submit your solution, create issue in this repository with title in "Solution by Nickname [score]" format, put your `image.slif` file there, `PSNR` and `Operation count` values from test program and, optionally, any additional information you want to share.