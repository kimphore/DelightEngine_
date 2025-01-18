# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Depot/DelightEngine_/External/eastl/_deps/eabase-src"
  "C:/Depot/DelightEngine_/External/eastl/_deps/eabase-build"
  "C:/Depot/DelightEngine_/External/eastl/_deps/eabase-subbuild/eabase-populate-prefix"
  "C:/Depot/DelightEngine_/External/eastl/_deps/eabase-subbuild/eabase-populate-prefix/tmp"
  "C:/Depot/DelightEngine_/External/eastl/_deps/eabase-subbuild/eabase-populate-prefix/src/eabase-populate-stamp"
  "C:/Depot/DelightEngine_/External/eastl/_deps/eabase-subbuild/eabase-populate-prefix/src"
  "C:/Depot/DelightEngine_/External/eastl/_deps/eabase-subbuild/eabase-populate-prefix/src/eabase-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Depot/DelightEngine_/External/eastl/_deps/eabase-subbuild/eabase-populate-prefix/src/eabase-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Depot/DelightEngine_/External/eastl/_deps/eabase-subbuild/eabase-populate-prefix/src/eabase-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
