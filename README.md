# AnyOption

| Linux, OSX Build | Windows Build | Code Coverage |
|:--:|:--:|:--:|
| [![Linux, OSX Build Status](https://travis-ci.org/hackorama/AnyOption.svg?branch=master)](https://travis-ci.org/hackorama/AnyOption) | [![Windows Build status](https://ci.appveyor.com/api/projects/status/8e6ugxoc9nju0005?svg=true)](https://ci.appveyor.com/project/hackorama/anyoption) | [![codecov.io](https://codecov.io/github/hackorama/AnyOption/coverage.svg?branch=master)](https://codecov.io/github/hackorama/AnyOption?branch=master) |


[http://www.hackorama.com/anyoption/](http://www.hackorama.com/anyoption/)

AnyOption is a C++ class for easy parsing of complex commandline options. It also parses options from a resource file in option value pair format. 

AnyOption implements the traditional POSIX style character options ( -n ) as well as the newer GNU style long options ( --name ). Or you can use a simpler long option version ( -name ) by asking to ignore the POSIX style options. 

AnyOption supports the traditional UNIX resourcefile syntax of, any line starting with "#" is a comment and the value pairs use ":" as a delimiter. 

An option which expects a value is considered as an option value pair, while options without a value are considered flags. 

Please read the header file for the documented public interface, and demo.cpp for an example of how easy it is to use AnyOption. 

August 2004, added bug-fixes, and updates send by Michael Peters of Sandia Lab. 

September 2006, fix from Boyan Asenov for a bug in mixing up option type indexes. 

July 2011, fix from Min KJ and Costantino G for string allocation. 
