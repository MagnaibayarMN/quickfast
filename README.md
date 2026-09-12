## QuickFAST -- An implementation of the FAST protocol for native C++ and .NET

QuickFAST is an Open Source native C++ implementation of the FAST Protocol [SM]. FAST(Fix Adapted for STreaming) protocol 
was developed by FIX Protocol Limited [FPL] (http://www.fixprotocol.org/fast/) as a way to reduce the bandwidth and network-latency 

Because FAST not specific to market data or the financial industry, there are opportunities for using FAST in a wide variety of situations.

QuickFAST is written to be portable to many platforms. It is routinely tested on Windows and Linux. The project also includes a .NET wrapper 
which supports using QuickFAST in the .NET environment. Ask if you want support for other platforms.

Instructions for [getting started with QuickFAST are here](https://github.com/objectcomputing/quickfast/wiki/GettingStarted)

## Building

QuickFAST can be built with CMake or with MPC.  Both build from the same
working copy; pick whichever suits you.

### CMake

Boost and Xerces-C are located with `find_package`, so they can come from your
platform's packages, from a package manager or from a local build.

```
cmake --preset default
cmake --build build
ctest --test-dir build --output-on-failure
```

The library alone is built by default; `-DQUICKFAST_BUILD_TESTS=ON` and
`-DQUICKFAST_BUILD_EXAMPLES=ON` add the unit tests and the example programs,
and the `default` preset above turns both on.

Installing exports a CMake package, so a project that uses QuickFAST needs
only:

```cmake
find_package(QuickFAST REQUIRED)
target_link_libraries(your_application PRIVATE QuickFAST::QuickFAST)
```

### CMake with vcpkg

If you would rather not install Boost and Xerces-C yourself, vcpkg can build
them, pinned to the baseline recorded in `vcpkg.json`.  With `VCPKG_ROOT` set:

```
cmake --preset vcpkg
cmake --build build
ctest --test-dir build --output-on-failure
```

### Docker

To build and run without installing anything on the host:

```
docker build -t quickfast .
docker run --rm quickfast -?
```

The image builds the library, runs the unit tests and keeps the example
programs; `InterpretApplication` is its entry point.

There is also a development container under `.devcontainer` with the
toolchain, vcpkg and a debugger already in place.

### MPC

The original build.  It needs [MPC](https://github.com/DOCGroup/MPC), and
`setup.sh` or `setup.cmd` has to be edited to point at your Boost and
Xerces-C installations.

```
. ./setup.sh
./m.sh
make
```


QuickFAST was developed by Object Computing Inc.(OCI) St. Louis Missouri USA. OCI has made QuickFAST available as open source software 
which may be used without payment of development or runtime license fees. OCI offers commercial support for QuickFAST.

For questions and discussion of QuickFAST, visit the [QuickFAST users mailing list](https://groups.google.com/forum/#!forum/quickfast_users)

###List Rules:
* Normal mailing list rules apply on the list. Discussions should be civil and on-topic. Offensive messages, off-topic chatter, and spam will not be tolerated.

* Messages from new members will be moderated due to the high volume of spam postings that are sent to this (and any) mailing list. For practical purposes this means there may be a delay before your first message to the list is published. Once you have est

* Messages on the list must be posted in English. It is acceptable to have the message in another language as well, but an English translation must appear first. For more information about this, click this link

## See also:

* For a open source Java implementation of FAST, see https://sourceforge.net/projects/openfast/
* For an open source C++ implementation of the FIX protocol, see http://www.quickfixengine.org/
* For an open source Java implementation of the FIX protocol, see http://www.quickfixj.org/
