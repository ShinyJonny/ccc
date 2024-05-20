#pragma once

#include "str.h"


#define from_mut(T) _Generic((T),     \
    str_mut: str_from_mut,            \
    StrSplitMut: str_split_from_mut,  \
)(T)
