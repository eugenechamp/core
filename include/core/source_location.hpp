#pragma once
#ifndef _CORE_SOURCE_LOCATION_HPP_
#define _CORE_SOURCE_LOCATION_HPP_

#include <source_location>
#include "def.hpp"

using SrcLocation = std::source_location;

namespace core::_ {
  struct SrcLocTrackerBase {
  #if !BUILD_SHIP
  protected:
    SrcLocTrackerBase(StringView _function_name_, StringView _file_name_, u32 _line_, u32 _column_)
      : function_name_(_function_name_), file_name_{_file_name_}, line_{_line_}, column_{_column_}
    { }
    
  public:
    auto function_name() -> StringView {
      return function_name_;
    }
    auto line() -> u32 { 
      return line_;
    }
    auto file_name() -> StringView { 
      return file_name_;
    }
    auto column() -> u32 {
      return column_;
    }
  
  private:
    StringView function_name_;
    StringView file_name_;
    u32 line_;
    u32 column_;
  #else
  protected:
    SrcLocTrackerBase() = default;
  #endif
  };
}

/// TODO: improve the description
// A utility class for debugging purposes to help track the caller location of a member function. It should never be constructed explicitly inside the client code. 
/// T: specialize a class that you want to track the caller location with, this is the only class that can create SrcLocTracker object
// USAGE: add as a parameter to a member function and default initialize it with 'SrcLocTracker<T>::track()' function
/// EXAMPLE: auto YourClass::foo(..., SrcLocTracker<YourClass> loc = SrcLocTracker<YourClass>::track())
/// NOTE: calling SrcLocTracker's constructor to initialize the default parameter in a function is not portable and fails to compile on clang and Intel C++ compiler for example
template<typename Owner>
struct SrcLocTracker final : core::_::SrcLocTrackerBase {
  friend Owner;
private:
  using SrcLocTrackerBase::SrcLocTrackerBase;

  static auto track(SrcLocation loc = SrcLocation::current()) -> SrcLocTracker {
  #if !BUILD_SHIP
    return SrcLocTracker { 
      loc.function_name(), 
      loc.file_name(), 
      loc.line(), 
      loc.column() 
    };
  #else
    return {};
  #endif
  }
};

#endif