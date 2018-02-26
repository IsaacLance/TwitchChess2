# distutils: language = c++
# distutils: sources = plusOne.cpp

cdef extern from "plusOne.cpp":
    int plusOne(int var1)

cpdef plus_one(int x):
    print(plusOne(x))
