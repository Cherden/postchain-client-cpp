## To make this sources compile:

### For Unreal Engine build system
 Add the following lines to *.Build.cs

```C#
PublicDefinitions.Add("_CRT_HAS_CXX17=0");
PublicDefinitions.Add("ECMULT_WINDOW_SIZE=15");
PublicDefinitions.Add("ECMULT_GEN_PREC_BITS=4");
```

### For other build systems
Add the following macros:
```c++
#define _CRT_HAS_CXX17 0
#define ECMULT_WINDOW_SIZE 15
#define ECMULT_GEN_PREC_BITS 4
```

