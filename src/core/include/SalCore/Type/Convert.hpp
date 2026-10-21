#pragma once

#include <SalCore/Precompiled.hpp>

// Macros
#define Sal_Stringify_Todo(type) String ToString(type t);

#define Sal_Stringify(type, command)                                           \
    inline String ToString(type t) { command; }

#define Sal_Stringify_Prebuilt(type)                                           \
    inline String ToString(type t) { return t.ToString(); }

// Base
namespace sal {
    Sal_Stringify_Todo(float);
    Sal_Stringify_Todo(int);
    Sal_Stringify_Todo(bool);
    Sal_Stringify_Todo(double);

    bool StrToFloat(const String &string, float *out);
    bool StrToInt(const String &string, int *out);
    bool StrToDouble(const String &string, double *out);
    bool StrToBool(const String &string, bool *out);
}
