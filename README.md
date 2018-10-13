![works](https://img.shields.io/badge/works%20on-my%20machine-brightgreen.svg) ![experimental](https://img.shields.io/badge/stability-experimental-red.svg)

# GCPointer
Garbage Collector which uses RAII (Experimental)
- English
- [한국어](https://github.com/kmc7468/GCPointer/blob/master/README.ko-kr.md)

## What is this?
GCPointer, Garbage collector which uses RAII, is a header-only library for C++. Note that this library is an experimental project, rather than a practical project.

## How to Install?
1. Download GCPointer.hpp
2. Include the file on your project
3. Have fun!

## Features
- Easy to install: Just download only a single header file.
- Cross-platform: You can use the same interface on any platform.
- Supports every C++ standard: You can use the same interface in every C++ standard such as  C++98 and C++17.

## Conditional Compilation Macros
- `_GCPOINTER_HAS_NAMESPACE`<br>
If this macro is defined, every class and function in GCPointer will be declared inside the namespace whose name is the value of the macro.
- `_GCPOINTER_MULTITHREADING`<br>
If this macro is defined, every function in GCPointer will be thread-safe. (With a slight performance loss.)<br>
**Note:** To use this macro in pre-C++11 standard, your operating system must be Microsoft Windows or a POSIX-based operating system. Then download GCPointer.cpp file and link it to your project.
- `_GCPOINTER_FLAGS_CPLUSPLUS98`, `_GCPOINTER_FLAGS_CPLUSPLUS03`, `_GCPOINTER_FLAGS_CPLUSPLUS11`, `_GCPOINTER_FLAGS_CPLUSPLUS14`, `_GCPOINTER_FLAGS_CPLUSPLUS17`<br>
Each indicates if the compiler supports C++98, C++03, C++11, C++14, and C++17. In most cases, you don't have to define these macros manually.

## License
```
MIT License

Copyright (c) 2018 kmc7468

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## Special Thanks
- Reviewing English README.md: [Suh Donghwi](https://github.com/suhdonghwi), [kiwiyou](https://github.com/kiwiyou)