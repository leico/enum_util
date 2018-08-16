//
//  main.cpp
//
//
// MIT License
// 
// Copyright (c) 2018 leico
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 

#include <iostream>
#include <type_traits> 
#include "enum_util.h" 


// test case classes

enum class A : unsigned char { 
  one = 0b00000001
};

enum class B : unsigned char {
  two = 0b00001000
};

enum class C : unsigned int {
  three
};


// accepter specialization section 
// all bit operator enabled between class A and A
template <> 
struct enum_util :: accepter < A > {
  using flagger = flagger< flag :: ALL >;
};

// AND bit operator enabled between class A and B
template <> 
struct enum_util :: accepter < A, B > {
  using flagger = flagger< flag :: AND >;
};


int main(int argc, const char * argv[]) {

  A a = A :: one, aa ;
  B b = B :: two ; 
  C c;


  a  |= A :: one;
  a  &= A :: one;
  a  ^= A :: one; 
  a <<= 1;
  a >>= 1;


//  a = a | b; // error
  a = a & b;
//  a = a ^ b; // error
  a = ~a;

 
  //a = a & c; //error


  return 0;
}





