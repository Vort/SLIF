This repository contains rules for image codec creation challenge.

Goal of this challenge is to create `image.slif` file, which contains compressed image. This file should be as small as you can get, decoded image should have decent quality and decoding process should not take too much time. File size specifies score (lower is better). Quality and time determines if solution is correct or not. Valid solution to this challenge must contain file by itself. Providing description of how it was created is optional.

To understand how to create such file and to check if your file is good enough, you need to read and execute test program, which is available in two versions: offline and online. Offline version is written in C#, binary and source code are located in [slif_v2.zip](https://github.com/Vort/SLIF/raw/a78c14980e01cf9cb0f49ccb1d2f09563d321dfa/slif_v2.zip) archive (CRC32 BAE0BA24). Online version is written in JavaScript and available [here](https://vort.github.io/SLIF/). These versions should behave the same, but in case of differences, results from C# version are preferred.

To start testing with C# version, you need to copy `image.slif` file to the same directory where binary is located and launch the binary (`SLIF.exe`). To start testing with JavaScript version, you need to select `image.slif` file with Browse button and click Check button. During the test, program compares image from your file with reference image (which is located in `reference.bin` file in C# version and should not be changed; JavaScript version have reference image embedded into source code). If test is finished and program shows "Success" message, then your file is good. Otherwise it is not good.

When test program is started with my solution, which is located inside `.zip` archive and can also be downloaded from [this](https://github.com/Vort/SLIF/blob/solutions/image_491916.slif?raw=true) link, it should produce following output:
```
Success
File size: 491916
PSNR: 35.7080560
Operation count: 48804196
```

To submit your solution, create issue in this repository with title in "Solution by Nickname [score]" format, put there your `image.slif` file, `Operation count` value from test program and, optionally, any additional information you want to share.

_If you want to skip part with test program analysis, you can read [description with spoilers](https://github.com/Vort/SLIF/blob/spoilers/README.md) instead._