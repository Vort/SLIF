This repository contains rules for image compression challenge.

Goal of this challenge is to create `image.slif` file, which contains compressed image. Such file should consist of two parts - header, which is constant for this challenge and subleq machine code, which produce image data when interpreted by test program. Valid solution to this challenge must contain file by itself. Providing description of how it was created is optional.

To win this challenge, your file should have smallest size in bytes (this is your score), produce image of decent quality and its execution should not take too much time. (See below for how these requirements are defined)

Image data is encoded as array of bytes, each byte represents intensity of red, green or blue component of pixel color. Since image size for this challenge is constant (512 x 512 pixels), resulting total size of array becomes 512 * 512 * 3 = 786432 bytes. Pixels are encoded from left to right, from top to bottom in R G B order.

Quality of image is calculated as sum of squared differences between byte values of constant reference image data (`reference.bin`) and image data, produced by subleq code. For your solution to be valid, such sum should be lower than 20358302.

Second requirement for solution is amount of executed operations during image decoding. It introduced to limit time, which is required for solution validation. For your solution to be valid, operation count should be lower than 100000000000. It corresponds to several hours of calculations with existing interpreter implementation.

Constant header of `image.slif` is 12 bytes in size and equal to 53 4C 49 46 00 02 00 02 00 00 00 10. It can be split to 4 parts: signature (4 bytes), image width (2 bytes), image height (2 bytes) and available memory size (4 bytes).

Variable data, which defines your solution, consists of subleq instructions, which also can be viewed as pure data. Each instruction have size of 12 bytes and consists of 3 addresses - `pa`, `pb`, `pc`. Each address is encoded as 32 bit little-endian byte index. Address 0 points to first byte of first instruction, which will be first operation executed. Last accessible byte have address 0xFFFFFFF (256 MiB - 1 byte). However, since variables, accessed by 32 bit pointers, are also 32 bit values, to access this byte, extra data should be read or written, which means that last valid value for pointer is 0xFFFFFFC. Reading and writing to addresses beyond it is not allowed. Transferring execution to addresses 0xFFFFFF5 and beyond is allowed and will result in program termination. Execution of each instruction consists or reading values from addresses `pa` and `pb` to variables `a` and `b`, subtraction of `a` from `b`, storing result by address `pb` and jumping to address `pc` if `b - a` is less or equal to zero or to address equal to instruction pointer + 12 otherwise. Goal of subleq program is to produce image data at addresses 0xFF40000 .. 0xFFFFFFF and terminate its execution.

To check if your file is good enough, you need to execute test program, which is available in two versions: offline and online. Offline version is written in C#, binary and source code are located in [slif.zip](https://github.com/Vort/SLIF/blob/master/slif.zip?raw=true) archive. Online version is written in JavaScript and available [here](https://vort.github.io/SLIF/). These versions should behave the same, but in case of differences, results from C# version are preferred.

To start testing with C# version, you need to copy `image.slif` file to the same directory where binary is located and launch the binary (`SLIF.exe`). To start testing with JavaScript version, you need to select `image.slif` file with Browse button and click Check button. If test is finished and program shows "Success" message, then your file is good. Otherwise it is not good.

When test program is started with example solution, which is located inside `.zip` archive and can also be downloaded from [this](https://github.com/Vort/SLIF/blob/solutions/image_667176.slif?raw=true) link, it should produce following output:
```
Success
File size: 667176
PSNR: 34.0640466
Operation count: 1342951
```

To submit your solution, create issue in this repository with title in "Solution by Nickname [score]" format, put there your `image.slif` file, `Operation count` value from test program and, optionally, any additional information you want to share.