#define CATCH_CONFIG_MAIN
/*
 * To compile get the latest version of catch2 from
 * https://github.com/catchorg/Catch2
 * $ wget https://github.com/catchorg/Catch2/releases/download/v2.2.3/catch.hpp
 * $ g++ -g -std=c++11 -Wall -Wextra -Werror test.cpp anyoption.cpp -o test
 *
 */
#include "anyoption.h"
#include "catch.hpp"

#include <fstream>
#include <stdarg.h>
#include <string>

using namespace std;
using namespace Catch::Matchers;

char **buildArgv(int size, ...) {
  va_list args;
  char **argv = (char **)malloc(size * sizeof(char *));
  va_start(args, size);
  for (int i = 0; i < size; i++) {
    const char *tmp = va_arg(args, char *);
    argv[i] = (char *)malloc((strlen(tmp) + 1) * sizeof(char));
    strcpy(argv[i], tmp);
  }
  va_end(args);
  return argv;
}

void clearArgv(int size, char **_argv) {
  for (int i = 0; i < size; i++)
    free(_argv[i]);
  free(_argv);
}

void writeOptions(string options) {
  std::ofstream out("test.options");
  out << options;
  out.close();
}

TEST_CASE("Test long options") {
  const int argc = 5;
  char **argv =
      buildArgv(argc, "test", "--long_flag_in_args", "--long_option_with_value",
                "long_option_value", "--long_option_with_out_value");

  AnyOption *opt = new AnyOption();

  opt->setFlag("long_flag_in_args");
  opt->setFlag("long_flag_not_in_args");
  opt->setOption("long_option_with_value");
  opt->setOption("long_option_with_out_value");
  opt->setOption("long_option_not_in_args");

  opt->processCommandArgs(argc, argv);

  REQUIRE(opt->getFlag("long_flag_in_args") == true);
  REQUIRE(opt->getFlag("long_flag_not_in_args") == false);
  REQUIRE(opt->getFlag("not_defined") == false);

  REQUIRE_THAT(opt->getValue("long_option_with_value"),
               Equals("long_option_value"));
  REQUIRE(opt->getValue("long_option_with_out_value") == NULL);
  REQUIRE(opt->getValue("long_option_not_in_args") == NULL);
  REQUIRE(opt->getValue("not_defined") == NULL);

  delete opt;
  clearArgv(argc, argv);
}

TEST_CASE("Test short options") {
  const int argc = 5;
  char **argv = buildArgv(argc, "test", "-x", "-a", "a_value", "-b");

  AnyOption *opt = new AnyOption();

  opt->setFlag('x'); // flag set in args
  opt->setFlag('y'); // flag not set in args

  opt->setOption('a'); // option with value in args
  opt->setOption('b'); // option with out value in args
  opt->setOption('c'); // option not in args

  opt->processCommandArgs(argc, argv);

  REQUIRE(opt->getFlag('x') == true);
  REQUIRE(opt->getFlag('y') == false);
  REQUIRE(opt->getFlag('z') == false); // not defined

  REQUIRE_THAT(opt->getValue('a'), Equals("a_value"));
  REQUIRE(opt->getValue('b') == NULL);
  REQUIRE(opt->getValue('c') == NULL);
  REQUIRE(opt->getValue('d') == NULL); // not defined

  delete opt;
  clearArgv(argc, argv);
}

TEST_CASE("Test long and short options with short provided") {

  const int argc = 4;
  char **argv = buildArgv(argc, "test", "-a", "-b", "b_value");

  AnyOption *opt = new AnyOption();

  opt->setFlag("a_long", 'a');
  opt->setOption("b_long", 'b');

  opt->processCommandArgs(argc, argv);

  REQUIRE(opt->getFlag('a') == true);
  REQUIRE(opt->getFlag("a_long") == true);
  REQUIRE_THAT(opt->getValue('b'), Equals("b_value"));
  REQUIRE_THAT(opt->getValue("b_long"), Equals("b_value"));

  delete opt;
  clearArgv(argc, argv);
}

TEST_CASE("Test file options") {

  writeOptions("x\nlong_flag\na : a_value\nlong_option : long_option_value\n");

  AnyOption *opt = new AnyOption();

  opt->setFlag('x');                 // short flag set in file
  opt->setFlag('y');                 // short flag not set in file
  opt->setFlag("long_flag");         // long flag set in file
  opt->setFlag("long_flag_not_set"); // long flag not set in file

  opt->setOption('a');                       // short option with value in file
  opt->setOption('b');                       // short option not in file
  opt->setOption("long_option");             // long option with value in file
  opt->setOption("long_option_not_in_file"); // long option not file

  opt->useFiileName("test.options");
  opt->processFile();

  REQUIRE(opt->getFlag('x') == true);
  REQUIRE(opt->getFlag('y') == false);
  REQUIRE(opt->getFlag('z') == false); // not defined
  REQUIRE(opt->getFlag("long_flag") == true);
  REQUIRE(opt->getFlag("not_defined") == false);

  REQUIRE_THAT(opt->getValue('a'), Equals("a_value"));
  REQUIRE(opt->getValue('b') == NULL);
  REQUIRE(opt->getValue('c') == NULL); // not defined
  REQUIRE_THAT(opt->getValue("long_option"), Equals("long_option_value"));
  REQUIRE(opt->getValue("not_defined") == NULL);

  delete opt;
}

TEST_CASE("Test set variations for long option") {

  const int argc = 7;
  char **argv =
      buildArgv(argc, "test", "--long_flag_set", "--long_flag_not_set",
                "--long_option_set", "value_set", "--long_option_not_set",
                "value_not_set");

  AnyOption *opt = new AnyOption();

  opt->setCommandFlag("long_flag_set");
  opt->setCommandOption("long_option_set");
  opt->setFileFlag("long_flag_not_set");
  opt->setFileOption("long_option_not_set");

  opt->processCommandArgs(argc, argv);

  REQUIRE(opt->getFlag("long_flag_set") == true);
  REQUIRE(opt->getFlag("long_flag_not_set") ==
          false); // should not match even if set in args

  REQUIRE_THAT(opt->getValue("long_option_set"), Equals("value_set"));
  REQUIRE(opt->getValue("long_option_not_set") ==
          NULL); // should not match even if set in args

  delete opt;
  clearArgv(argc, argv);
}

TEST_CASE("Test set variations for short option") {

  const int argc = 7;
  char **argv =
      buildArgv(argc, "test", "-x", "-y", "-a", "a_value", "-b", "b_value");

  AnyOption *opt = new AnyOption();

  opt->setCommandFlag('x');
  opt->setCommandOption('a');
  opt->setFileFlag('y');
  opt->setFileOption('b');

  opt->processCommandArgs(argc, argv);

  REQUIRE(opt->getFlag('x') == true);
  REQUIRE(opt->getFlag('y') == false); // should not match even if set in args

  REQUIRE_THAT(opt->getValue('a'), Equals("a_value"));
  REQUIRE(opt->getValue('b') == NULL); // should not match even if set in args

  delete opt;
  clearArgv(argc, argv);
}

TEST_CASE("Test option storage allocation") {

  AnyOption *opt = new AnyOption();

  opt->setOption("option_1");
  opt->setOption("option_2");
  opt->setOption("option_3");
  opt->setOption("option_4");
  opt->setOption("option_5");
  opt->setOption("option_6");
  opt->setOption("option_7");
  opt->setOption("option_8");
  opt->setOption("option_9");
  opt->setOption("option_10");
  opt->setOption("option_11"); // exceed DEFAULT_MAXOPTS of 10

  delete opt;
}

TEST_CASE("Test option storage allocation for char options") {

  AnyOption *opt = new AnyOption();

  opt->setOption('1');
  opt->setOption('2');
  opt->setOption('3');
  opt->setOption('4');
  opt->setOption('5');
  opt->setOption('6');
  opt->setOption('7');
  opt->setOption('8');
  opt->setOption('9');
  opt->setOption('a');
  opt->setOption('b'); // exceed DEFAULT_MAXOPTS of 10

  delete opt;
}

TEST_CASE("Test usage storage allocation") {

  AnyOption *opt = new AnyOption();

  opt->addUsage("line 1");
  opt->addUsage("line 2");
  opt->addUsage("line 3");
  opt->addUsage("line 4"); // exceed DEFAULT_MAXUSAGE of 3

  delete opt;
}

TEST_CASE("Test usage") {

  AnyOption *opt = new AnyOption();

  opt->addUsage("line 1");
  opt->addUsage("line 2");
  opt->addUsage("line 3");
  opt->printUsage();
  opt->printAutoUsage();

  delete opt;
}

TEST_CASE("Test util set functions") {

  AnyOption *opt = new AnyOption();

  opt->setCommandPrefixChar('=');
  opt->setCommandLongPrefix("++");
  opt->setCommandLongPrefix(
      "##---"); // try more than MAX_LONG_PREFIX_LENGTH of 2
  opt->setFileCommentChar('.');
  opt->setFileDelimiterChar('=');
  opt->noPOSIX();
  opt->setVerbose();

  delete opt;
}
