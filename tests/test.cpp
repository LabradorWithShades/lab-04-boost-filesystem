// Copyright 2020 LabradorWithShades
// 70892548+LabradorWithShades@users.noreply.github.com
#include <gtest/gtest.h>
#include "DirectoryAnalyzer.hpp"

TEST(General, EmptyTest) {
    EXPECT_TRUE(true);
}

TEST(General, Launch) {
  DirectoryAnalyzer analyzer("../misc/ftp");
  std::cout << analyzer;
}

