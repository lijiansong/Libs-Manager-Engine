Engine Architecture Design
============================
This archive is the integral architecture design of the engine, which is like a library manager. Not only can it manager the opencv libs, but also it is able to manage any other libs, such as *face detection, voice recognition* and so on. 

## Content

* [AIDL Service Client](#aidl-service-client)
  * [Modules](#modules)
  * [Benefits](#benefits)
* [Code Generator](#code-generator)
  * [Features](#features)
* [License](#license)

## AIDL Service Client
The AIDL Service Client is based on AIDL (Android Interface Definition Language) which is similar to other IDLs you might have worked with. It allows you to define the programming interface that both the client and service agree upon in order to communicate with each other using interprocess communication (IPC) which is based on the underlying *Binder* mechanisms.

### Modules
The technical architecture of the AIDL Service Client is shown below.

![image](https://github.com/lijiansong/OpenCV-Manager-Engine/blob/master/screenshot/arch.png)

### Benefits
As we can see from the above picture, it is obvious that the AIDL Service Client has the following benefits:
- Less memory usage. All apps use the same binaries from service and do not have to keep the native libs inside themselves;
- Hardware specific optimizations for all supported platforms, such as the different CPU-Arch of Android platform;
- Once the Engine is released, all the Service Libraries will be trusted library source. All packages with OpenCV should be published on Google Play market or Apple's App Store;
- Regular updates and bug fixes.

## Code Generator
Besides the AIDL Service Client, the engine also contains a code generator. The code generator is based on the description file (config file), which is composed of infomation of the native libs. The information in the config file contains the usage of the libs, therefore the code generator is able to generate code automatically with the assistance of the config files. 

### Features
- Generating code automatically. So it can spare the trouble of learning new programing language and coding the app oneself;
- Cross-platorm. Since the description file is light-weighted, the code generator can perform normally on mutiple platforms;
- Automatical lib-packageing. Taking full advantage of the libs to develop various applications, the code generator should have the function of packaging the libiaries automatically. For example, we have class A and class B, both of them have many member functions, the code generator can *combine(package)* these functions together to get a new class for different development purposes so that developers can call these functions more easily.

## License
[The MIT License (MIT)](http://opensource.org/licenses/MIT)

Copyright (c) 2016 Json Lee

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
