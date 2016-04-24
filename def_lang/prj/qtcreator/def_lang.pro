TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

PRECOMPILED_HEADER = ../../src/stdafx.h
CONFIG *= precompile_header

rpi {
    DEFINES+=RASPBERRY_PI
    QMAKE_MAKEFILE = "Makefile.rpi"
    MAKEFILE = "Makefile.rpi"
    CONFIG(debug, debug|release) {
        DEST_FOLDER = rpi/debug
    }
    CONFIG(release, debug|release) {
        DEST_FOLDER = rpi/release
        DEFINES += NDEBUG
    }
} else {
    QMAKE_MAKEFILE = "Makefile"
    CONFIG(debug, debug|release) {
        DEST_FOLDER = pc/debug
    }
    CONFIG(release, debug|release) {
        DEST_FOLDER = pc/release
        DEFINES += NDEBUG
    }
}

OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../lib/$${DEST_FOLDER}

QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-unused-variable -Wno-unused-parameter
QMAKE_LFLAGS += -rdynamic


INCLUDEPATH += ../../../qbase/include
INCLUDEPATH += ../../../qmath/include
INCLUDEPATH += ../../include
INCLUDEPATH += ../../src


HEADERS += \
    ../../include/def_lang/ep/Type_Template_EP.inl \
    ../../include/def_lang/ep/Value_Template_EP.inl \
    ../../include/def_lang/impl/IInitializer.inl \
    ../../include/def_lang/impl/Numeric_Type_Template.inl \
    ../../include/def_lang/impl/Numeric_Value_Template.inl \
    ../../include/def_lang/IAttribute.h \
    ../../include/def_lang/IAttribute_Container.h \
    ../../include/def_lang/IBool_Type.h \
    ../../include/def_lang/IBool_Value.h \
    ../../include/def_lang/IConstant.h \
    ../../include/def_lang/IDeclaration_Scope.h \
    ../../include/def_lang/IExpression.h \
    ../../include/def_lang/IInitializer.h \
    ../../include/def_lang/IInitializer_List.h \
    ../../include/def_lang/ILiteral.h \
    ../../include/def_lang/IMember.h \
    ../../include/def_lang/IMember_Container.h \
    ../../include/def_lang/IMember_Def.h \
    ../../include/def_lang/IMember_Def_Container.h \
    ../../include/def_lang/INumeric_Type_Template.h \
    ../../include/def_lang/INumeric_Value_Template.h \
    ../../include/def_lang/IString_Type.h \
    ../../include/def_lang/IString_Value.h \
    ../../include/def_lang/IStruct_Type.h \
    ../../include/def_lang/IStruct_Value.h \
    ../../include/def_lang/ISymbol.h \
    ../../include/def_lang/ITemplate_Argument.h \
    ../../include/def_lang/ITemplated_Type.h \
    ../../include/def_lang/ITemplated_Value.h \
    ../../include/def_lang/IType.h \
    ../../include/def_lang/IValue.h \
    ../../include/def_lang/IValue_Container.h \
    ../../include/def_lang/IVector_Type.h \
    ../../include/def_lang/IVector_Value.h \
    ../../include/def_lang/Math.h \
    ../../include/def_lang/Result.h \
    ../../include/def_lang/Source_Location.h \
    ../../include/def_lang/Symbol_Path.h \
    ../../include/def_lang/ts_assert.h \
    ../../include/def_lang/Type_System.h \
    ../../include/def_lang/Value_Selector.h \
    ../../include/def_lang/ep/Attribute_Container_EP.h \
    ../../include/def_lang/ep/Declaration_Scope_EP.h \
    ../../include/def_lang/ep/Member_Container_EP.h \
    ../../include/def_lang/ep/Member_Def_Container_EP.h \
    ../../include/def_lang/ep/Symbol_EP.h \
    ../../include/def_lang/ep/Type_Template_EP.h \
    ../../include/def_lang/ep/Value_Template_EP.h \
    ../../include/def_lang/impl/All_Numeric_Types.h \
    ../../include/def_lang/impl/All_Numeric_Values.h \
    ../../include/def_lang/impl/Bool_Type.h \
    ../../include/def_lang/impl/Bool_Value.h \
    ../../include/def_lang/impl/Decimals_Attribute.h \
    ../../include/def_lang/impl/Initializer_List.h \
    ../../include/def_lang/impl/Literal.h \
    ../../include/def_lang/impl/Literal_Expression.h \
    ../../include/def_lang/impl/Max_Attribute.h \
    ../../include/def_lang/impl/Member.h \
    ../../include/def_lang/impl/Member_Def.h \
    ../../include/def_lang/impl/Min_Attribute.h \
    ../../include/def_lang/impl/Namespace.h \
    ../../include/def_lang/impl/Numeric_Type_Template.h \
    ../../include/def_lang/impl/Numeric_Value_Template.h \
    ../../include/def_lang/impl/String_Type.h \
    ../../include/def_lang/impl/String_Value.h \
    ../../include/def_lang/impl/Struct_Type.h \
    ../../include/def_lang/impl/Struct_Value.h \
    ../../include/def_lang/impl/UI_Name_Attribute.h \
    ../../include/def_lang/impl/Vector_Type.h \
    ../../include/def_lang/impl/Vector_Value.h \
    ../../src/location.hh \
    ../../src/parser.y \
    ../../src/position.hh \
    ../../src/stack.hh \
    ../../src/tokens.l \
    ../../src/yy_parser.hpp \
    ../../include/ast/Attribute.h \
    ../../include/ast/Builder.h \
    ../../src/ast/Lexer.h \
    ../../include/ast/Node.h \
    ../../include/def_lang/impl/IValue.impl \
    ../../include/def_lang/impl/IValue.inl \
    ../../include/def_lang/All_INumeric_Types.h \
    ../../include/def_lang/All_INumeric_Values.h \
    ../../include/def_lang/Mapper.h \
    ../../include/def_lang/impl/Mapper.inl \
    ../../include/def_lang/JSON_Serializer.h \
    ../../include/def_lang/Serialization.h \
    ../../include/def_lang/impl/Serialization.inl \
    ../../include/def_lang/IPtr_Type.h \
    ../../include/def_lang/impl/Ptr_Type.h \
    ../../include/def_lang/IPtr_Value.h \
    ../../include/def_lang/impl/Ptr_Value.h

SOURCES += \
    ../../src/main.cpp \
    ../../src/yy_lexer.cpp \
    ../../src/yy_parser.cpp \
    ../../src/ast/Attribute.cpp \
    ../../src/ast/Builder.cpp \
    ../../src/ast/Lexer.cpp \
    ../../src/ast/Node.cpp \
    ../../src/ts/Bool_Type.cpp \
    ../../src/ts/Bool_Value.cpp \
    ../../src/ts/Decimals_Attribute.cpp \
    ../../src/ts/Initializer_List.cpp \
    ../../src/ts/Literal.cpp \
    ../../src/ts/Literal_Expression.cpp \
    ../../src/ts/Max_Attribute.cpp \
    ../../src/ts/Member.cpp \
    ../../src/ts/Member_Def.cpp \
    ../../src/ts/Min_Attribute.cpp \
    ../../src/ts/Namespace.cpp \
    ../../src/ts/Source_Location.cpp \
    ../../src/ts/String_Type.cpp \
    ../../src/ts/String_Value.cpp \
    ../../src/ts/Struct_Type.cpp \
    ../../src/ts/Struct_Value.cpp \
    ../../src/ts/Symbol_Path.cpp \
    ../../src/ts/Type_System.cpp \
    ../../src/ts/UI_Name_Attribute.cpp \
    ../../src/ts/Vector_Type.cpp \
    ../../src/ts/Vector_Value.cpp \
    ../../src/ts/ep/Attribute_Container_EP.cpp \
    ../../src/ts/ep/Declaration_Scope_EP.cpp \
    ../../src/ts/ep/Member_Container_EP.cpp \
    ../../src/ts/ep/Member_Def_Container_EP.cpp \
    ../../src/ts/Value_Selector.cpp \
    ../../src/ts/JSON_Serializer.cpp \
    ../../src/ts/Serialization.cpp \
    ../../src/ts/Ptr_Type.cpp \
    ../../src/ts/Ptr_Value.cpp \
    ../../src/ts/ep/Symbol_EP.cpp

