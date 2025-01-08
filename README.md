# Event notification library

This repository include the most common approaches to simplify the development of a complex project containing related components.
Among the implemented approaches, a pool of threads and an event model communication was implemented.
This library uses only components of the C++ Standard Library.

## Preparing

Specify the folder containing your workspace with the command:
```export HOST_WORKSPACE=<your path>```

In the $HOST_WORKSPACE folder, copy this repository using
```git clone command line https://github.com/KirillKazakhmedov/event-model.git```


## Docker

The repository uses a multi-stage build using docker compose.

Build a basic image containing the necessary dependencies:
```docker compose -f docker/docker-compose.yaml build event-model-base```

Build a docker image to launch a develop container:
```docker compose -f docker/docker-compose.yaml build event-model-develop```

Launch a develop container:
```docker compose -f docker/docker-compose.yaml up event-model-develop --detach```

Enter the container with the command:
```docker exec -it event_model_develop bash```

To stop the container, enter the command:
```docker compose -f docker/docker-compose.yaml down event-model-develop```

## Build and run tests in the container

Build:
```cd event-model && cmake -Bbuild -H.  -DBUILD_TYPE=Release -DENABLE_EXAMPLES=OFF -DENABLE_TESTS=ON && cmake --build build --```

Run tests:
```cd build && ctest --output-on-failure```

## Formatting

To provide correct code formatting use pre-commit tool:
```cd event-model && pre-commit install && pre-commit run --all-files```

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
