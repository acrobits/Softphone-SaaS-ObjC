#pragma once

#define ALI_IMOVE(value) ali::move(value)
    //  Clang managed to screw up implicit move for returned values in certain cases.
