/*=========================================================================

Program:   BlueBerry Platform
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __BERRY_MACROS_H__
#define __BERRY_MACROS_H__

#include "berryWeakPointer.h"

#define berryNameMacro(className) \
  virtual const char* GetClassName() const \
  { return #className; }\
  static const char* GetStaticClassName() \
  { return #className; }\

#define berryManifestMacro(className, namespaze) \
  static const char* GetManifestName() \
  { return #namespaze #className; } \

#define berryObjectMacro(className) \
  typedef className       Self; \
  typedef berry::SmartPointer<Self> Pointer; \
  typedef berry::SmartPointer<const Self>  ConstPointer; \
  typedef berry::WeakPointer<Self> WeakPtr; \
  typedef berry::WeakPointer<const Self> ConstWeakPtr; \
  berryNameMacro(className) \


#define berryInterfaceMacro(className, namespaze) \
  public: \
  berryObjectMacro(className) \
  berryManifestMacro(className, namespaze) \

#define berrySimpleInterfaceMacro(className, namespaze) \
  protected: className() {} \
  public: \
  berryNameMacro(className) \
  berryManifestMacro(className, namespaze) \

#define berryNewMacro(x) \
static Pointer New(void) \
{ \
  Pointer smartPtr(new x); \
  return smartPtr; \
} \

#define berryNewMacro1Param(x, type1) \
static Pointer New(type1 param1) \
{ \
  Pointer smartPtr(new x(param1)); \
  return smartPtr; \
} \

#define berryNewMacro2Param(x, type1, type2) \
static Pointer New(type1 param1, type2 param2) \
{ \
  Pointer smartPtr(new x(param1, param2)); \
  return smartPtr; \
} \

#define berryNewMacro3Param(x, type1, type2, type3) \
static Pointer New(type1 param1, type2 param2, type3 param3) \
{ \
  Pointer smartPtr (new x(param1, param2, param3)); \
  return smartPtr; \
} \

#ifndef BERRY_NO_TYPESAFE_FLAGS

#include "berryFlags.h"

#define BERRY_DECLARE_FLAGS(_Flags, _Enum)\
typedef berry::Flags<_Enum> _Flags;

#if defined _MSC_VER && _MSC_VER < 1300
# define BERRY_DECLARE_INCOMPATIBLE_FLAGS(_Flags)
#else
# define BERRY_DECLARE_INCOMPATIBLE_FLAGS(_Flags) \
inline berry::IncompatibleFlag operator|(_Flags::enum_type f1, int f2) \
{ return berry::IncompatibleFlag(int(f1) | f2); }
#endif

#define BERRY_DECLARE_OPERATORS_FOR_FLAGS(_Flags) \
inline berry::Flags<_Flags::enum_type> operator|(_Flags::enum_type f1, _Flags::enum_type f2) \
{ return berry::Flags<_Flags::enum_type>(f1) | f2; } \
inline berry::Flags<_Flags::enum_type> operator|(_Flags::enum_type f1, berry::Flags<_Flags::enum_type> f2) \
{ return f2 | f1; } BERRY_DECLARE_INCOMPATIBLE_FLAGS(_Flags)

#else /* BERRY_NO_TYPESAFE_FLAGS */

#define BERRY_DECLARE_FLAGS(_Flags, _Enum)\
typedef uint _Flags;
#define BERRY_DECLARE_OPERATORS_FOR_FLAGS(_Flags)

#endif /* BERRY_NO_TYPESAFE_FLAGS */

#define BERRY_REGISTER_EXTENSION_CLASS(_ClassType)\
qRegisterMetaType<_ClassType>(_ClassType::staticMetaObject.className());

#endif /*__BERRY_MACROS_H__*/
