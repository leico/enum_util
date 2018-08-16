//
//  enum_util.h
//  enum_util
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


#ifndef enum_util_h
#define enum_util_h

#include "operator_acception/includes/operator_acception.h"
#include "bool_checker/includes/bool_checker.h" 

namespace enum_util {


  //enum type check and returned type or underlying_type
  template < typename T >
  struct model {

    private:
      static_assert( std :: is_enum< T > :: value , "enum_cast only used to enum type" );


    public: 
      using type = T;
      using underlying_type = typename std :: underlying_type< T > :: type ;
  };



  //provides enum type to underlying_type, or underlying_type to enum type casts
  template < typename T > 
  struct cast {
    
    private: 
      using type            = typename model< T > :: type;
      using underlying_type = typename model< T > :: underlying_type;

    public:
    static constexpr underlying_type
      to_underlying ( type t ) { return static_cast< underlying_type >( t ) ; }

    static constexpr type 
      to_enum ( underlying_type t ) { return static_cast< type >( t ) ; }
  };





  //check both enum type were same underlying_type ?
  template < typename T, typename U = T  > 
    using is_same_underlying = std :: is_same<
      typename model< T > :: underlying_type 
    , typename model< U > :: underlying_type 
    >;




  //check could logical or Arithmetic using is_same_underlying and model
  template < typename... Args> 
    class is_logic_safe {

      private:

        // no args
        template < typename... InArgs > 
          struct impl : std :: false_type {};

        // single arg
        template < typename Type1, typename... Tail > 
          struct impl< Type1, Tail... > : std :: true_type {};

        // more than 2 args
        template < typename Type1, typename Type2, typename... Tail > 
          struct impl< Type1, Type2, Tail... > : 
            std :: conditional<
                is_same_underlying< Type1, Type2 > :: value 
              , typename impl< Type2, Tail... > :: type 
              , std :: false_type 
            > :: type  
          {};

      public:
        static constexpr bool value = impl< Args... > :: type :: value;
    };


  // contains bit flagger and accepter
  using operator_acception :: bit :: flagger; 
  using operator_acception :: bit :: accepter;

  // contains other flagger's utility
  using operator_acception :: bit :: is_or_accepted;
  using operator_acception :: bit :: is_and_accepted;
  using operator_acception :: bit :: is_not_accepted;
  using operator_acception :: bit :: is_xor_accepted;
  using operator_acception :: bit :: is_l_shift_accepted;
  using operator_acception :: bit :: is_r_shift_accepted;


  //also contains bit flags
  namespace flag { 
    using operator_acception :: flag :: OR ;
    using operator_acception :: flag :: AND; 
    using operator_acception :: flag :: NOT; 
    using operator_acception :: flag :: XOR; 
    using operator_acception :: flag :: L_SHIFT;
    using operator_acception :: flag :: R_SHIFT; 
    using operator_acception :: flag :: ALL;
  }

  // acception accepts bit operator function for enum
  template <
      template < typename... > class Accepter
    , typename... Args 
  >
  class acception { 
    private: 

      // exclude all 
      template <
          template < typename... > class InAccepter
        , typename... InArgs 
      >
      struct impl {
        using type = std :: false_type;
      };

      // enabled single arg
      template < 
          template < typename > class InAccepter
        , typename T 
      > 
      struct impl< InAccepter, T > { 
        using type = typename std :: conditional<
            bool_checker :: anded_t< InAccepter< T >, is_logic_safe< T > > :: value
          , std :: true_type 
          , std :: false_type
        > :: type;
      };

      // enabled double args
      template < 
          template < typename, typename > class InAccepter
        , typename T, typename U 
      > 
      struct impl< InAccepter, T, U > { 

        using type = typename std :: conditional<
            bool_checker :: anded_t< InAccepter< T, U >, is_logic_safe< T, U > > :: value 
          , std :: true_type 
          , std :: false_type 
          > :: type;
      };

    public: 
      using accepted = std :: enable_if<
          impl< Accepter, Args... > :: type :: value
        , std :: nullptr_t
      >;
  };

}

// provide | operation for enum class
template < 
    typename T
  , typename U = T
  , typename enum_util :: acception< 
      enum_util :: is_or_accepted, T, U 
    > :: accepted :: type  = nullptr 
> 
static constexpr T operator| ( T t, U u ) { 

  using namespace enum_util;

  using cast_t = cast< T >;
  using cast_u = cast< U >;

  return cast_t :: to_enum( 
      cast_t :: to_underlying( t ) | 
      cast_u :: to_underlying( u ) 
  ) ;
}


// provide & operation for enum class
template < 
    typename T
  , typename U = T
  , typename enum_util :: acception< 
      enum_util :: is_and_accepted, T, U  
    > :: accepted :: type = nullptr 
> 
static constexpr T operator& ( T t, U u ) {

  using namespace enum_util;

  using cast_t = cast< T >;
  using cast_u = cast< U >;

  return cast_t :: to_enum(
      cast_t :: to_underlying( t ) &
      cast_u :: to_underlying( u )
  );
}

// provide ~ operation for enum class
template < 
    typename T
  , typename enum_util :: acception< 
      enum_util :: is_not_accepted, T 
    > :: accepted :: type = nullptr 
> 
static constexpr T operator~ ( T t ) { 
  using namespace enum_util;
  using cast = cast< T >;

  return cast :: to_enum( 
    ~( cast :: to_underlying( t ) )
  );
}

// provide ^ operation for enum class
template < 
    typename T
  , typename U = T 
  , typename enum_util :: acception< 
      enum_util :: is_xor_accepted, T, U 
    > :: accepted :: type = nullptr 
>
static constexpr T operator^ ( T t, U u ) { 
  using namespace enum_util;
  using cast_t = cast< T >;
  using cast_u = cast< U >;

  return cast_t :: to_enum( 
    cast_t :: to_underlying( t ) ^
    cast_u :: to_underlying( u )
  );
}


// provide << operation for enum class
template < 
    typename T
  , typename enum_util :: acception< 
      enum_util :: is_l_shift_accepted, T 
    > :: accepted :: type = nullptr
> 
static constexpr T operator<< ( T t, std :: size_t digits ) { 

  using namespace enum_util;
  using cast = cast< T >;

  return cast :: to_enum( cast :: to_underlying( t ) << digits );
}

// provide >> operation for enum class
template < 
    typename T
  , typename enum_util :: acception< 
      enum_util :: is_r_shift_accepted, T 
    > :: accepted :: type = nullptr 
>
static constexpr T operator>> ( T t, std :: size_t digits ) { 

  using namespace enum_util;
  using cast = cast< T >;

  return cast :: to_enum( cast :: to_underlying( t ) >> digits );
}

// provide |= operation for enum class
template < 
    typename T
  , typename U = T
  , typename enum_util :: acception< 
      enum_util :: is_or_accepted, T, U 
    > :: accepted :: type = nullptr 
> 
static constexpr T& operator|= ( T& t, U u ) {
  return t = t | u;
}

// provide &= operation for enum class
template < 
    typename T
  , typename U = T 
  , typename enum_util :: acception< 
      enum_util :: is_and_accepted, T, U 
    > :: accepted :: type = nullptr 
>
static constexpr T& operator&= ( T& t, U u ) { 
  return t = t & u;
}


// provide ^= operation for enum class
template < 
    typename T
  , typename U = T
  , typename enum_util :: acception< 
      enum_util :: is_xor_accepted, T, U 
    > :: accepted :: type = nullptr 
> 
static constexpr T& operator^= ( T& t, U u ) { 
  return t = t ^ u;
}

// provide  <<= operation for enum class
template < 
    typename T
  , typename enum_util :: acception< 
      enum_util :: is_l_shift_accepted, T 
    > :: accepted :: type = nullptr 
>
static constexpr T& operator<<= ( T& t, std :: size_t digits ) { 
  return t = t << digits ;
}

// provide >>= operation for enum class
template < 
    typename T
  , typename enum_util :: acception< 
      enum_util :: is_r_shift_accepted, T 
    > :: accepted :: type = nullptr 
> 
static constexpr T& operator>>= ( T& t, std :: size_t digits ) { 
  return t = t >> digits ;
}



#endif /* enum_util_h */
