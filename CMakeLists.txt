cmake_minimum_required(VERSION 2.8)

project(anyoption)

set(anyoption_SRCS
  anyoption.cpp
  )

set(anyoption_HDRS
  anyoption.h
  )

include_directories(
  .
  )

add_library(anyoption SHARED ${anyoption_SRCS} ${anyoption_HDRS})
