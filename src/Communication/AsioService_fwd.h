// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ASIOSERVICE_FWD_H
#define ASIOSERVICE_FWD_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

// Boost 1.66 turned io_service into a typedef for io_context, so the class
// declaration QuickFAST used to make here now conflicts with the one Asio
// provides.  From that release on, take the declaration from Boost itself;
// io_service.hpp only pulls in io_context, not all of Asio.
#include <boost/version.hpp>
#if BOOST_VERSION >= 108700
// Boost 1.87 removed the io_service name altogether.
# include <boost/asio/io_context.hpp>
namespace boost
{
  namespace asio
  {
    typedef io_context io_service;
  }
}
#elif BOOST_VERSION >= 106600
# include <boost/asio/io_service.hpp>
#else
// forward declare io_service without including
// boost header
namespace boost
{
  namespace asio
  {
    class io_service;
  }
}
#endif // BOOST_VERSION >= 106600

namespace QuickFAST
{
  namespace Communication
  {
    class AsioService;
  }
}
#endif // ASIOSERVICE_FWD_H
