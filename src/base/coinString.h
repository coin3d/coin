#ifndef COINSTRING_H
#define COINSTRING_H
#include <Inventor/C/tidbits.h>
#include <Inventor/SbBasic.h>
#include <Inventor/SbByteBuffer.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbTypeInfo.h>
#include <boost/lexical_cast.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <string>

template <unsigned int size, typename Target, typename Source>
struct ordered {
};

template <typename Source>
  struct ordered<2,SbByteBuffer,Source> {
  static
  SbByteBuffer repr(const Source * in) {
    uint16_t val = coin_hton_uint16(*reinterpret_cast<const uint16_t *>(in));
    return SbByteBuffer(2,reinterpret_cast<const char *>(&val));
  }
};

template <typename Source>
  struct ordered<4,SbByteBuffer,Source> {
  static
  SbByteBuffer repr(const Source * in) {
    uint32_t val = coin_hton_uint32(*reinterpret_cast<const uint32_t *>(in));
    SbByteBuffer tmp = SbByteBuffer(4,reinterpret_cast<const char *>(&val));
    return tmp;
  }
};

template <typename Target>
struct ordered<2,Target,SbByteBuffer> {
  static
    Target repr(const SbByteBuffer & in, size_t offset = 0) {
    uint16_t val = coin_ntoh_uint16(*reinterpret_cast<const uint16_t *>(&in[offset*2]));
    return *reinterpret_cast<Target*>(&val);
  }
};

template <typename Target>
struct ordered<4,Target,SbByteBuffer> {
  static
    Target repr(const SbByteBuffer & in, size_t offset = 0) {
    union {
      Target t;
      uint32_t u32;
    } val;
    val.u32 = coin_ntoh_uint32(*reinterpret_cast<const uint32_t *>(&in[offset*4]));
    return val.t;
  }
};

template<typename Type>
struct PrimitiveRepresentation {
  //The below method needs to be implemented in a specialization for any allowable type.
  //static const char * get() {}
};

template<>
struct PrimitiveRepresentation<unsigned short> {
  static const char * get() { return "(\\d+)"; }
};

template<>
struct PrimitiveRepresentation<float> {
  static const char * get() {
    return "(-?\\d+\\.\\d+?|-?\\d+)";
  }
};

template<>
struct PrimitiveRepresentation<short> {
  static const char * get() { return "(-?\\d+)"; }
};


template <typename Target, typename Source>
  struct lexical_cast_internal {
    static Target cast(const Source & arg) {
      return boost::lexical_cast<Target,Source>(arg);
    }
  };

template <typename Source>
struct lexical_cast_internal<SbString,Source> {
  static SbString cast(const Source & arg) {
    return SbString(lexical_cast_internal<std::string,Source>::cast(arg).c_str());
  }
};

template <typename Source>
struct lexical_cast_internal<SbByteBuffer,Source> {
  static SbByteBuffer cast(const Source & arg) {
    return ordered<sizeof(arg),SbByteBuffer,Source>::repr(&arg);
  }
};

template <>
struct lexical_cast_internal<SbString,SbString> {
  static SbString cast(const SbString & arg) {
    return arg;
  }
};

template <>
struct lexical_cast_internal<SbString,SbByteBuffer> {
  static SbString cast(const SbByteBuffer & arg) {
    int n = arg.size();
    if (n && arg[n-1]=='\0')
      --n;
    return SbString(arg.constData(),0,n);
  }
};

template<typename Target, typename Source>
  Target lexical_cast(const Source & arg)
{
  return lexical_cast_internal<Target,Source>::cast(arg);
}

template<typename Type>
struct OutputSeparator
{
  static
  void addLeft(const Type & source) {
  }
  static
  void addRight(const Type & source) {
  }
  static
  void addItem(const Type & source) {
  }
};

template<>
struct OutputSeparator<SbString>
{
  static
  void addLeft(SbString & source) {
    source+="<";
  }
  static
  void addRight(SbString & source) {
    source+=">";
  }
  static
  void addItem(SbString & source) {
    source+=", ";
  }
};

template<typename Type>
struct InputSeparator
{
  static
  void addLeft(const Type & source) {
  }
  static
  void addRight(const Type & source) {
  }
  static
  void addItem(const Type & source) {
  }
};

template<>
struct InputSeparator<SbString>
{
  static
  void addLeft(SbString & source) {
    source+="[<\\(]\\s*";
  }
  static
  void addRight(SbString & source) {
    source+="\\s*[>\\)]";
  }
  static
  void addItem(SbString & source) {
    source+="\\s*,\\s*";
  }
};

template<typename Type>
struct OptionalTypename {
  static void add(SbString & source)
  {
    source+="(?:";
    source+=SbTypeInfo<Type>::getTypeName();
    source+=")?";
  }
};

template<typename Target,typename Source, int Dimensions=1>
  struct DimensionalStringCast {
    static void getRegexp(SbString & regexp) {
      OptionalTypename<Target>::add(regexp);
      InputSeparator<Source>::addLeft(regexp);
      for (size_t i = 0; i < SbTypeInfo<Target>::Dimensions; ++i) {
        DimensionalStringCast<typename SbTypeInfo<Target>::PrimitiveType, Source>::getRegexp(regexp);
        if (i<SbTypeInfo<Target>::Dimensions-1)
          InputSeparator<Source>::addItem(regexp);
      }
      InputSeparator<Source>::addRight(regexp);
    }

    static Target castTo(const SbString & input, SbBool & conversionOk)
    {
      Target val;
      SbString regexp;
      getRegexp(regexp);
#ifdef COIN_DEBUG_REPR
      printf("%s\n", regexp.getString());
#endif //COIN_DEBUG_REPR
      boost::xpressive::sregex rex = boost::xpressive::sregex::compile(regexp.getString());
      boost::xpressive::smatch what;
      std::string inStr = input.getString();
      if ( ( conversionOk = regex_match( inStr , what, rex ) ) ) {
        for (size_t i = 1; i <= SbTypeInfo<Target>::Dimensions; ++i) {
          //This should never throw.
          val[i-1] = boost::lexical_cast<typename SbTypeInfo<Target>::PrimitiveType>(std::string(what[i]));
        }
      }
      return val;
    }
  };

template<typename Source>
  struct DimensionalStringCast<SbRotation,Source,4> {
    static void getRegexp(SbString & regexp) {
      OptionalTypename<SbRotation>::add(regexp);
      InputSeparator<Source>::addLeft(regexp);
      for (size_t i = 0; i < SbTypeInfo<SbRotation>::Dimensions; ++i) {
        DimensionalStringCast<typename SbTypeInfo<SbRotation>::PrimitiveType, Source>::getRegexp(regexp);
        if (i<SbTypeInfo<SbRotation>::Dimensions-1)
          InputSeparator<Source>::addItem(regexp);
      }
      InputSeparator<Source>::addRight(regexp);
    }

    static SbRotation castTo(const SbString & input, SbBool & conversionOk)
    {
      SbRotation val;
      SbString regexp;
      getRegexp(regexp);
#ifdef COIN_DEBUG_REPR
      printf("%s\n", regexp.getString());
#endif //COIN_DEBUG_REPR
      boost::xpressive::sregex rex = boost::xpressive::sregex::compile(regexp.getString());
      boost::xpressive::smatch what;
      std::string inStr = input.getString();
      if ( ( conversionOk = regex_match( inStr , what, rex ) ) ) {
        val = SbRotation(
                         boost::lexical_cast<typename SbTypeInfo<SbRotation>::PrimitiveType>(std::string(what[1])),
                         boost::lexical_cast<typename SbTypeInfo<SbRotation>::PrimitiveType>(std::string(what[2])),
                         boost::lexical_cast<typename SbTypeInfo<SbRotation>::PrimitiveType>(std::string(what[3])),
                         boost::lexical_cast<typename SbTypeInfo<SbRotation>::PrimitiveType>(std::string(what[4]))
                         );
      }
      return val;
    }
};

template<typename Target, typename Source>
  struct DimensionalStringCast<Target,Source,1> {
  static void getRegexp(SbString & regexp) {
    regexp += PrimitiveRepresentation<Target>::get();
  }
  static Target castTo(const SbString & input, SbBool & conversionOk) {
    abort();
    Target retVal;
    return retVal;
  }

};

template<typename Target,int Dimensions>
  struct DimensionalStringCast<Target,SbByteBuffer,Dimensions> {
  static size_t getSize() {
    return
      SbTypeInfo<Target>::Dimensions * sizeof(typename SbTypeInfo<Target>::PrimitiveType);
  }

  static Target castTo(const SbByteBuffer & input, SbBool & conversionOk)
  {
    Target retVal;
#ifdef COIN_DEBUG_REPR
    printf("%s has size %d\n", SbTypeInfo<Target>::getTypeName(), getSize());
    printf("Input has size %d\n", input.size());
#endif //COIN_DEBUG_REPR
    if (input.size() != getSize()) {
      conversionOk = false;
      return retVal;
    }
    conversionOk = TRUE;

    for (size_t i = 0; i < SbTypeInfo<Target>::Dimensions; ++i) {
      //This should never throw.
      retVal[i] = ordered<
        sizeof(typename SbTypeInfo<Target>::PrimitiveType),
        typename SbTypeInfo<Target>::PrimitiveType,
        SbByteBuffer
        >::repr(input,i);
    }
    return retVal;
  }
};

template<>
  struct DimensionalStringCast<SbRotation,SbByteBuffer,4> {
  static size_t getSize() {
    return
      SbTypeInfo<SbRotation>::Dimensions * sizeof(SbTypeInfo<SbRotation>::PrimitiveType);
  }

  static SbRotation castTo(const SbByteBuffer & input, SbBool & conversionOk)
  {
    SbRotation retVal;
#ifdef COIN_DEBUG_REPR
    printf("%s has size %d\n", SbTypeInfo<SbRotation>::getTypeName(), getSize());
    printf("Input has size %d\n", input.size());
#endif //COIN_DEBUG_REPR
    if (input.size() != getSize()) {
      conversionOk = false;
      return retVal;
    }
    conversionOk = TRUE;

    retVal = SbRotation(
                        ordered<
                        sizeof(SbTypeInfo<SbRotation>::PrimitiveType),
                        SbTypeInfo<SbRotation>::PrimitiveType,
                        SbByteBuffer
                        >::repr(input,0),
                        ordered<
                        sizeof(SbTypeInfo<SbRotation>::PrimitiveType),
                        SbTypeInfo<SbRotation>::PrimitiveType,
                        SbByteBuffer
                        >::repr(input,1),
                        ordered<
                        sizeof(SbTypeInfo<SbRotation>::PrimitiveType),
                        SbTypeInfo<SbRotation>::PrimitiveType,
                        SbByteBuffer
                        >::repr(input,2),
                        ordered<
                        sizeof(SbTypeInfo<SbRotation>::PrimitiveType),
                        SbTypeInfo<SbRotation>::PrimitiveType,
                        SbByteBuffer
                        >::repr(input,3)
                        );
    return retVal;
  }
};

template<typename Target>
struct DimensionalStringCast<Target,SbByteBuffer,1> {
  static void getRegexp(SbString & regexp) {
    regexp += PrimitiveRepresentation<Target>::get();
  }
};

template< typename Type, unsigned int numargs, typename Source >
  struct fromArray
  {

  };

template <typename Type, typename Source>
  struct fromArray<Type,4,Source>
{
  static Type construct(const Source & src)
  {
    //return Type(const Source & src);
  }
};

template<typename Target,typename Source, int Dimensions = 1 >
  struct DimensionalCast {
    static Target castFrom(const Source & val) {
      Target ret;
      //typename Vec<typename SbTypeInfo<Source>::PrimitiveType, SbTypeInfo<Source>::Dimensions>::Type ret;
      OutputSeparator<Target>::addLeft(ret);
      for (size_t i = 0; i < SbTypeInfo<Source>::Dimensions; ++i) {
        ret+= DimensionalCast< Target, typename SbTypeInfo<Source>::PrimitiveType>::castFrom(val[i]);
        if (i<SbTypeInfo<Source>::Dimensions-1)
          OutputSeparator<Target>::addItem(ret);
      }
      OutputSeparator<Target>::addRight(ret);
      return ret;
    }
    static Target castTo(const Source & input, SbBool & conversionOk)
    {
      return DimensionalStringCast<Target,Source,Dimensions>::castTo(input,conversionOk);
    }
  };

template<typename Target, typename Source>
  struct DimensionalCast< Target, Source, 1 > {
  static Target castFrom(const Source & val) {
    return lexical_cast<Target>(val);
  }
  static Target castTo(const Source & input, SbBool & conversionOk)
  {
    return DimensionalStringCast<Target,Source,1>::castTo(input,conversionOk);
  }
};


template<typename Source>
SbString
ToString(const Source & val) {
  return DimensionalCast<SbString,Source,SbTypeInfo<Source>::Dimensions>::castFrom(val);
}

template<typename Source>
SbByteBuffer
ToByteRepr(const Source & val) {
  return DimensionalCast<SbByteBuffer,Source,SbTypeInfo<Source>::Dimensions>::castFrom(val);
}

template<typename Target>
Target
FromString(const SbString & input, SbBool * conversionOk = NULL ) {
  SbBool conversionOk_ = TRUE;
  Target val = DimensionalCast<Target,SbString,SbTypeInfo<Target>::Dimensions>::castTo(input, conversionOk_);
  if (conversionOk)
    *conversionOk = conversionOk_;
  return val;
}

template<typename Target>
Target
FromByteRepr(const SbByteBuffer & input, SbBool * conversionOk = NULL) {
  SbBool conversionOk_ = TRUE;
  Target val;
  val = DimensionalCast<Target,SbByteBuffer,SbTypeInfo<Target>::Dimensions>::castTo(input,conversionOk_);
  if (conversionOk)
    *conversionOk = conversionOk_;
  return val;
}

#endif //COINSTRING_H
