
# enum_util

provide enum operator overload acceptor for enum class 
and more utilities for enum class

depends 

* leico/operator_acception
* leico/bool_checker

## functions

### `enum_util :: model< EnumClass >`

asserts when `EnumClass` is not enum class.

provide 2 returned type

* ` :: type`
    * returned `EnumClass` s type
* ` :: underlying_type`
    * returned `EnumClass` s underlying_type

### `enum_util :: cast< EnumClass >`

provide 2 types cast

* ` :: to_enum`
    * cast to `EnumClass` type
* ` :: to_underlying`
    * cast to `EnumClass` s underlying_type

### `enum_util :: is_same_underlying< T, U > :: value`

`value` returned `true` if enum class `T` and `U` are same underlying_type,
otherwise `false`

### `enum_util :: is_logic_safe< Args... > :: value`

Checking enum classes `Args...` are same underlying_type or not.
`value` returned `true` when all enum class's underlying_type are same.
`false`, or not.


### alias of other meta functions


#### from operator_acception :: bit structs

* `enum_util :: flagger`
* `enum_util :: accepter`

#### from operator_acception :: bit utilities

* `enum_util :: is_or_accepted`
* `enum_util :: is_and_accepted`
* `enum_util :: is_not_accepted`
* `enum_util :: is_xor_accepted`
* `enum_util :: is_l_shift_accepted`
* `enum_util :: is_r_shift_accepted`

#### from operator_acception :: flags


* `enum_util :: flag :: OR`
* `enum_util :: flag :: AND`
* `enum_util :: flag :: NOT`
* `enum_util :: flag :: XOR`
* `enum_util :: flag :: L_SHIFT`
* `enum_util :: flag :: R_SHIFT`
* `enum_util :: flag :: ALL`


### `enum_util :: acception`

allowed below 2 patterns

* `enum_util :: acception< T, U, Accepter<typename, typename> > :: accepted`
* `enum_util :: acception< T,    Accepter<typename>           > :: accepted` 

`accepted :: type` sets `std :: nullptr_t` when `Accepter<T> :: value` and `is_logic_safe<T> :: value` is `true`
or `Accepter<T, U> :: value` and `is_logic_safe<T, U> :: value` is `true`



### operators

enable when correspond flag is `true`

* `accepter<A, B> :: flagger :: OR`
    * `A |  B`
    * `A |= B`
* `accepter<A, B> :: flagger :: AND`
    * `A &  B`
    * `A &= B`
* `accepter<A>    :: flagger :: NOT`
    * `~A` 
* `accepter<A, B> :: flagger :: XOR`
    * `A ^  B`
    * `A ^= B`
* `accepter<A>    :: flagger :: L_SHIFT`
    * `A <<  digits`
    * `A <<= digits`
* `accepter<A>    :: flagger :: R_SHIFT`
    * `A >>  digits`
    * `A >>= digits`

flagger is checked `is_xxx_accepted` each operators above this section

```cpp
...
, typename enum_util :: acception< 
enum_util :: is_xxx_accepted, T, U 
> :: accepted :: type  = nullptr 
...
```

## example 

```cpp
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
```




