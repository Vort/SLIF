This repository contains the challenge of creating an image codec.

In this challenge you need to create `image.slif` file, which contains compressed image. This file should be as small as you can get, decoded image should have decent quality and decoding process should not take too much time. File size specifies score (lower is better). Quality and time determines if this solution is correct or not. Valid solution to this challenge must contain file by itself. Providing description of how it was created is optional.

To understand how to create such file and to check if your file is good enough, you need to read and execute test program, which is provided in binary (`SLIF.exe`) and text (`Program.cs`) form. To execute binary, .NET Framework is required for Windows or Mono for Linux. Test program compares image from your file with reference image, which is located in `reference.bin` and should not be changed. If test program shows "Success" message, then your file is good. Otherwise it is not good.

All needed files are located in [slif_v1.zip](https://github.com/Vort/SLIF/raw/bc6269bb978802bef4738c201b78f46353b58fb4/slif_v1.zip) archive (CRC32 779BB417). Also archive contains my solution to this challenge, which produces such result when test program is launched:
```
Success
File size: 491916
PSNR: 35.7080560
Operation count: 48804196
```

To submit your solution, create issue in this repository with title in "Solution by Nickname [score]" format, put your `image.slif` file there, `PSNR` and `Operation count` values from test program and, optionally, any additional information you want to share.

_If you want to skip part with test program analysis, you can read [description with spoilers](https://github.com/Vort/SLIF/blob/spoilers/README.md) instead._