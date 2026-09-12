// Copyright (c) 2009, 2010, 2011 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifdef _MSC_VER
# pragma once
#endif
#ifndef ASIOCOMPATIBILITY_H
#define ASIOCOMPATIBILITY_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

#include "AsioService_fwd.h"
#include <boost/version.hpp>
#include <boost/asio.hpp>

namespace QuickFAST
{
  namespace Communication
  {
    // Boost 1.66 replaced several of the Asio interfaces QuickFAST is written
    // against, and Boost 1.87 removed the originals.  Each helper below spells
    // its operation the way the Boost release being compiled against expects,
    // so that the call sites do not have to.

    /// @brief Convert a dotted IP address to an asio address
    /// @param address is the address in dotted notation
    inline boost::asio::ip::address makeAddress(const std::string & address)
    {
#if BOOST_VERSION >= 106600
      return boost::asio::ip::make_address(address);
#else
      return boost::asio::ip::address::from_string(address);
#endif
    }

    /// @brief Post a completion handler to an io service
    /// @param ioService is the service that will run the handler
    /// @param handler is the handler to be posted
    template<typename CompletionHandler>
    inline void postHandler(
      boost::asio::io_service & ioService,
      CompletionHandler handler)
    {
#if BOOST_VERSION >= 106600
      boost::asio::post(ioService, handler);
#else
      ioService.post(handler);
#endif
    }

    /// @brief Keeps an io service running while it has no work to do
    ///
    /// io_service::work was replaced by executor_work_guard in Boost 1.66 and
    /// removed in Boost 1.87.
#if BOOST_VERSION >= 106600
    typedef boost::asio::executor_work_guard<
      boost::asio::io_context::executor_type> WorkGuard;
#else
    typedef boost::asio::io_service::work WorkGuard;
#endif

    /// @brief Create a work guard for an io service
    /// @param ioService is the service to be kept alive
    /// @returns a new work guard; the caller owns it
    inline WorkGuard * makeWorkGuard(boost::asio::io_service & ioService)
    {
#if BOOST_VERSION >= 106600
      return new WorkGuard(boost::asio::make_work_guard(ioService));
#else
      return new WorkGuard(ioService);
#endif
    }

    /// @brief Prepare an io service to be run again after it has stopped
    /// @param ioService is the service to be restarted
    inline void restartService(boost::asio::io_service & ioService)
    {
#if BOOST_VERSION >= 106600
      ioService.restart();
#else
      ioService.reset();
#endif
    }
  }
}
#endif // ASIOCOMPATIBILITY_H
