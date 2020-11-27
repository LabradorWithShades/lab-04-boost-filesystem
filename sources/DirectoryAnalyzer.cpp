// Copyright 2020 LabradorWithShades
// 70892548+LabradorWithShades@users.noreply.github.com
#include "DirectoryAnalyzer.hpp"

using boost::filesystem::directory_entry;
using boost::filesystem::filesystem_error;
using boost::filesystem::file_type;

DirectoryAnalyzer::DirectoryAnalyzer(const std::string& directoryPath) {
  try {
    m_path = path(directoryPath);
    m_dirIterator = directory_iterator(m_path);
  } catch (const filesystem_error& e) {
    m_path = path(".");
    m_dirIterator = directory_iterator(m_path);
    std::cerr << e.what() << std::endl;
    std::cerr << "Using \".\" directory instead" << std::endl;
  }
}

std::string getRelativePath(const path& dirPath, const path& startPath) {
  if (dirPath.string().find(startPath.string()) == std::string::npos)
    return "ERROR";
  std::string ans = dirPath.string();
  ans.erase(0, startPath.string().length() + 1);
  return ans;
}

bool isNumber(char x) {
  return ((x >= '0') && (x <= '9'));
}

bool checkFilename(const std::string& filename) {
  const size_t filenameLength = 29;
  if (filename.length() != filenameLength)
    return false;
  if (filename.find("balance") != 0)
    return false;
  const size_t balanceLength = 7;
  const size_t accountLength = 8;
  const size_t dateLength = 8;

  const size_t accountStart = 0 + balanceLength + 1;
  const size_t dateStart = accountStart + accountLength + 1;

  for (size_t i = accountStart; i < accountStart + accountStart; ++i)
    if (!isNumber(filename[i]))
      return false;

  for (size_t i = dateStart; i < dateStart + dateLength; ++i)
    if (!isNumber(filename[i]))
      return false;

  const size_t extensionStart = dateStart + dateLength + 1;
  const size_t extensionLength = 3;
  if (filename[extensionStart - 1] != '.')
    return false;

  std::string extension = filename.substr(extensionStart, extensionLength);
  if (extension != "txt")
    return false;

  return true;
}

void separateData(const std::string& name,
                        std::string& account,
                        std::string& date) {
  auto accountStart = name.find_first_of('_') + 1;
  auto accountLength = 8;

  auto dateStart = name.find_last_of('_') + 1;
  auto dateLength = 8;

  account = name.substr(accountStart, accountLength);
  date = name.substr(dateStart, dateLength);
}

uint32_t getNumber(const std::string& str) {
  uint32_t number = 0;
  const size_t base = 10;
  for (size_t i = 0; i < str.size(); ++i) {
    number *= base;
    number += static_cast<uint32_t>(str[i] - '0');
  }
  return number;
}

void DirectoryAnalyzer::analyzeSubDirectory(std::ostream& out,
                                            const path& dirPath,
                                            BrokerData& data) const {
  std::string dirPrefix = getRelativePath(dirPath, m_path);
  for (const auto& x : directory_iterator{dirPath}) {
    if ((x.status().type() == file_type::directory_file) ||
        ((x.status().type() == file_type::symlink_file) &&
         (x.symlink_status().type() == file_type::directory_file)))
      continue;
    if (checkFilename(x.path().filename().string())) {
      out << dirPrefix << " " << x.path().filename().string() << std::endl;

      std::string account_str,
                  date_str;
      separateData(x.path().filename().string(), account_str, date_str);

      uint32_t accountID = getNumber(account_str),
                lastDate = getNumber(date_str);

      auto it = data.directoriesData.find(dirPrefix);
      if (it == data.directoriesData.end()) {
        DirectoryData newDirEntry;
        newDirEntry.directoryPrefix = dirPrefix;
        AccountData newAccEntry;
        newAccEntry.accountID = accountID;
        newAccEntry.lastDate = lastDate;
        newAccEntry.fileCount = 1;
        newDirEntry.accountsData.insert(std::pair(accountID, newAccEntry));
        data.directoriesData.insert(std::pair(dirPrefix, newDirEntry));
      } else {
        auto accIt = it->second.accountsData.find(accountID);
        if (accIt == it->second.accountsData.end()) {
          AccountData newAccEntry;
          newAccEntry.accountID = accountID;
          newAccEntry.lastDate = lastDate;
          newAccEntry.fileCount = 1;
          it->second.accountsData.insert(std::pair(accountID, newAccEntry));
        } else {
          ++accIt->second.fileCount;
          if (accIt->second.lastDate < lastDate)
            accIt->second.lastDate = lastDate;
        }
      }
    }
  }
}

void DirectoryAnalyzer::analyze(std::ostream& out) const {
  BrokerData data;
  data.rootDirectory = m_path.string();
  out << "Found files in \"" << data.rootDirectory << "\" folder:" << std::endl;
  for (const auto& x : m_dirIterator) {
    if ((x.status().type() != file_type::directory_file) ||
        ((x.status().type() == file_type::symlink_file) &&
         (x.symlink_status().type() != file_type::directory_file)))
      continue;
    analyzeSubDirectory(out, x.path(), data);
  }
  if (data.directoriesData.empty()) {
    out << "No matching files found" << std::endl;
    return;
  }
  out << std::endl << "Statistics of \"" << data.rootDirectory <<
         "\" folder:" << std::endl;
  for (const auto& x : data.directoriesData) {
    for (const auto& y : x.second.accountsData) {
      out << "broker:" << x.second.directoryPrefix
          << " account:";
      out.width(8);
      out.fill('0');
      out << y.second.accountID
          << " files:" << y.second.fileCount
          << " lastdate:"
          << y.second.lastDate << std::endl;
    }
  }
}

std::ostream& operator<<(std::ostream& out, const DirectoryAnalyzer& da) {
  da.analyze(out);
  return out;
}
