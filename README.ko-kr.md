![works](https://img.shields.io/badge/works%20on-my%20machine-brightgreen.svg) ![experimental](https://img.shields.io/badge/stability-experimental-red.svg)

# GCPointer
RAII를 사용한 쓰레기 수집기 (실험적)
- [English](https://github.com/kmc7468/GCPointer/blob/master/README.md)
- 한국어

## 이게 뭔가요?
GCPointer는 RAII를 사용한 쓰레기 수집기로, C++을 위한 헤더 파일로만 구성된 라이브러리입니다. 이 프로젝트는 실용성을 위한 프로젝트가 아닌, 실험을 위한 프로젝트임에 유의해 주십시오.

## 어떻게 설치하나요?
1. GCPointer.hpp를 다운로드 합니다.
2. 여러분의 프로젝트에 파일을 Include 합니다.
3. 즐기세요!

## 특징
- 설치가 간단합니다: 하나의 헤더 파일만 다운로드 받으면 설치가 끝납니다.
- 크로스 플랫폼입니다: 어느 플랫폼에서나 같은 인터페이스를 사용할 수 있습니다.
- 모든 C++ 표준을 지원합니다: C++98에서도, C++17에서도 같은 인터페이스로 사용할 수 있습니다.

## 조건부 컴파일 매크로
- `_GCPOINTER_HAS_NAMESPACE`<br>
이 매크로가 정의되어 있으면 GCPointer의 모든 클래스, 함수 등이 해당 매크로가 치환할 이름을 가진 네임스페이스 안에 선언됩니다.
- `_GCPOINTER_MULTITHREADING`<br>
이 매크로가 정의되어 있으면 GCPointer의 모든 함수가 쓰레드에 안전한 함수가 됩니다. (약간의 성능 저하가 발생할 수 있습니다.)<br>
**Note:** C++98 및 C++03에서 이 매크로를 사용하려면 운영체제가 Microsoft Windows 또는 POSIX 계열 운영체제여야 하며, GCPointer.cpp 파일을 다운로드 해 프로젝트에 링크해야 합니다.
- `_GCPOINTER_FLAGS_CPLUSPLUS98`, `_GCPOINTER_FLAGS_CPLUSPLUS03`, `_GCPOINTER_FLAGS_CPLUSPLUS11`, `_GCPOINTER_FLAGS_CPLUSPLUS14`, `_GCPOINTER_FLAGS_CPLUSPLUS17`<br>
각각 컴파일러가 C++98, C++03, C++11, C++14, C++17 표준을 제대로 지원함을 나타냅니다. 대부분의 경우 직접 이 매크로들을 정의할 필요가 없습니다.

## 라이선스
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
- 영문 README.md 검수: [서동휘](https://github.com/suhdonghwi), [kiwiyou](https://github.com/kiwiyou)