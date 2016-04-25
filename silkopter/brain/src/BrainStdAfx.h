#if defined __cplusplus

#include <type_traits>
#include <typeinfo>
#include <boost/thread.hpp>

#include <boost/asio.hpp>
#include <boost/optional.hpp>

#define RAPIDJSON_HAS_STDSTRING 1

#include "QBase.h"
#include "QData.h"
#include "qmath.h"

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "utils/Json_Util.h"

#include "Globals.h"

#include "def_lang/IDeclaration_Scope.h"
#include "def_lang/IType.h"
#include "def_lang/IValue.h"
#include "def_lang/All_INumeric_Values.h"
#include "def_lang/IPtr_Value.h"
#include "def_lang/IVector_Value.h"
#include "def_lang/IString_Value.h"
#include "def_lang/IStruct_Value.h"
#include "def_lang/IBool_Value.h"

namespace ts
{
class Type_System;
}


#endif
