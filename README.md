# Event notification library

This repository include the most common approaches to simplify the development of a complex project containing related components.
Among the implemented approaches, a pool of threads and an event model communication was implemented.
This library uses only components of the C++ Standard Library.

## Build

cmake -Bbuild -H.  -DBUILD_TYPE=Release -DENABLE_EXAMPLES=OFF -DENABLE_TESTS=OFF && cmake --build build -- -j4

## License

MIT License

Copyright (c) [2024] [Kirill Kazakhmedov]

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

## Maintainers

Kirill Kazakhmedov <k.kirill777@gmail.com>
