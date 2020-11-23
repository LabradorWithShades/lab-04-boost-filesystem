// Copyright 2020 LabradorWithShades
// 70892548+LabradorWithShades@users.noreply.github.com
#include <iostream>
#include "DirectoryAnalyzer.hpp"

int main(int argc, char** argv) {
  std::string pathToLookupDirectory = "";
  if (argc == 1) {
    pathToLookupDirectory = ".";
  } else if (argc == 2) {
    pathToLookupDirectory = argv[1];
  } else {
    std::cout << "Error! Incorrect argument count" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "lab-04-filesystem <path to directory>" << std::endl;
    std::cout << "OR" << std::endl;
    std::cout << "lab-04-filesystem" << std::endl;
    std::cout << "which is equivalent to:" << std::endl;
    std::cout << "lab-04-filesystem ." << std::endl;
    return 1;
  }
  DirectoryAnalyzer analyzer(pathToLookupDirectory);
  std::cout << analyzer;
  return 0;
}

