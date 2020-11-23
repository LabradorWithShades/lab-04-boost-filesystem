// Copyright 2020 LabradorWithShades
// 70892548+LabradorWithShades@users.noreply.github.com
#ifndef INCLUDE_DIRECTORYANALYZER_HPP_
#define INCLUDE_DIRECTORYANALYZER_HPP_

#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

using boost::filesystem::path;
using boost::filesystem::directory_iterator;

class DirectoryAnalyzer {
 public:
  explicit DirectoryAnalyzer(const std::string& path);

  void analyze(std::ostream& out) const;
 private:
  struct AccountData {
    uint32_t accountID;
    uint32_t lastDate;
      size_t fileCount;
  };
  struct DirectoryData {
    std::string directoryPrefix;
    std::unordered_map<uint32_t, AccountData> accountsData;
  };
  struct BrokerData {
    std::string rootDirectory;
    std::unordered_map<std::string, DirectoryData> directoriesData;
  };
  void analyzeSubDirectory(std::ostream& out,
                           const path& dirPath,
                           BrokerData& data) const;
 private:
  directory_iterator m_dirIterator;
  path m_path;
};

std::ostream& operator<<(std::ostream& out, const DirectoryAnalyzer& da);

#endif // INCLUDE_DIRECTORYANALYZER_HPP_
