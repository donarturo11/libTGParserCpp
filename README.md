# About
Minimalist version of ThinkGearStreamParser rewritten in cpp

# Build
Type:
```
cd libTGStreamParser
mkdir build
cd build
cmake ..
cmake --build .
cmake --install .
```

# Usage
To use this library, add to CMakeLists.txt
```
find_package(tgparser)
```
and
```
target_link_libraries(${PROJECT_NAME} PUBLIC tgparser)
```

In header/source files in C/C++ add:
```
#include "tgparser/ThinkGearStreamParser.h"
```

In Makefile add to CFLAGS or CXXFLAGS ```-L[tgparser path] -I[tgparser path] -ltgparser```


# Contact
To contact me, please use my [contact form](https://freeshell.de/~arturwro/contact.php?lang=en)
