# LittleMessageBus

A sympathic implementation of a MessageBus extensively using C++ templates

## Compiling

```bash
g++ -omessagebus src/main.cpp src/MessageBus/*.cpp -Iinclude -Wall -Wextra -std=c++11
```

## Features

* Static MessageBus class
* Subscribers of any type (Lambda, Functions, Callable Objects...)
* Message of any type (primitives, structs...)
* Message Queue
* Priority of subscribers
* Possibility to prevent propagation
* Extensively using templates enabling clean code
