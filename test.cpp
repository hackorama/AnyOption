#define CATCH_CONFIG_MAIN

#include "anyoption.h"
#include "catch.hpp"

#include <stdarg.h>


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

TEST_CASE("Test long options") {
  const int argc = 5;
  char **argv =
      buildArgv(argc, "test", "--long_flag_provided", "--long_option_with_value",
            "long_option_value", "--long_option_without_value");
  AnyOption *opt = new AnyOption();

  opt->setFlag("long_flag_provided");
  opt->setFlag("long_flag_not_provided");
  opt->setOption("long_option_with_value");
  opt->setOption("long_option_without-value");
  opt->setOption("long_option_not_provided");

  opt->processCommandArgs(argc, argv);

  REQUIRE(opt->getFlag("long_flag_provided") == true);
  REQUIRE(opt->getValue("long_option_without_value") == NULL);

  REQUIRE(opt->getFlag("long_flag_not_provided") == false);
  REQUIRE(opt->getValue("long_option_not_provided") == NULL);

  REQUIRE(opt->getFlag("long_flag_not_set") == false);
  REQUIRE(opt->getValue("long_option_not_set") == NULL);

  delete opt;
  clearArgv(argc, argv);
}
