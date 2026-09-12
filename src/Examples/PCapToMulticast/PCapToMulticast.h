// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//
#ifndef PCAPTOMULTICAST_H
#define PCAPTOMULTICAST_H

#define PCAP_SUPPORT_IS_HEREx
#include <Application/CommandArgParser.h>
#include <Communication/PCapReader.h>
#include <Communication/AsioCompatibility.h>
#include <boost/asio.hpp>
#include <stdio.h>
#include <chrono>

namespace QuickFAST{
  namespace Examples{

    /// @brief Multicast the contents of a FAST encoded data file.
    ///
    /// This program uses an echo file produced by the InterpretFAST program
    /// to identify the message boundaries in a FAST encoded data file.
    /// It multicasts each message in a separate datagram.
    ///
    /// Use the -? command line option for more information.
    ///
    /// This program is not really FAST-aware. It is just part of a testing
    /// framework for other programs.
    class PCapToMulticast : public Application::CommandArgHandler
    {
    public:
      PCapToMulticast();
      ~PCapToMulticast();

      /// @brief parse command line arguments, and initialize.
      /// @param argc from main
      /// @param argv from main
      /// @returns true if everything is ok.
      bool init(int argc, char * argv[]);
      /// @brief run the program
      /// @returns a value to be used as an exit code of the program (0 means all is well)
      int run();
      /// @brief do final cleanup after a run.
      void fini();

    private:
      void sendBurst();

    private:
      virtual int parseSingleArg(int argc, char * argv[]);
      virtual void usage(std::ostream & out) const;
      virtual bool applyArgs();
    private:
      unsigned short portNumber_;
      std::string sendAddress_;
      std::string dataFileName_;
      size_t sendCount_;
      size_t sendMicroseconds_;
      size_t burst_;
      bool pauseEveryPass_;
      bool pauseEveryMessage_;
      bool force32_;
      bool force64_;
      size_t packetChecksumSize_;
      bool verbose_;

      boost::asio::io_service ioService_;
      boost::asio::ip::address multicastAddress_;
      boost::asio::ip::udp::endpoint endpoint_;
      boost::asio::ip::udp::socket socket_;
      // boost::asio::strand became a template in Boost 1.66; the class this
      // example uses is spelled io_service::strand in every release.
      boost::asio::io_service::strand strand_;
#if BOOST_VERSION >= 106600
      // Asio's date_time based timers became opt-in, so the example uses the
      // chrono based timer that is always available.
      boost::asio::steady_timer timer_;
#else
      boost::asio::deadline_timer timer_;
#endif // BOOST_VERSION >= 106600

      Application::CommandArgParser commandArgParser_;
//      FILE * dataFile_;
      Communication::PCapReader pcapReader_;
      size_t nPass_;
      size_t nMsg_;
      size_t totalMessageCount_;
    };
  }
}
#endif // PCAPTOMULTICAST_H
