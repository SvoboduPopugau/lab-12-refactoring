// Copyright 2020 Your Name <your_email>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <boost/program_options.hpp>

#include "Log.hpp"
#include "header.hpp"
#include "UsedMemory.hpp"
#include "StatSender.hpp"
#include "PageContainer.hpp"

namespace po = boost::program_options;

const char error_mes[] = "**********BAD SYNTAX**********\n"
                         "Look to --help or -h";

void CreatePo(po::options_description& desc,
              po::variables_map& vm,
              const int& argc, const char** argv){
  desc.add_options()
      ("help,h", "Help screen\n")

      ("log_debug,l", "Logger status (Release(if OFF) or Debug(if ON))\n")

      ("input,i", po::value<std::string>()->default_value("data.txt"),
       "Name of input file\n")

      ("threshold,t", po::value<float>()->default_value(1),
       "Enter number\n");
  store(parse_command_line(argc, argv, desc), vm);
  notify(vm);
}


int main(const int argc, const char* argv[]) {
  try {
    Log& the_log = Log::Instance();
    po::options_description desc{"Options"};
    po::variables_map vm;
    CreatePo(desc, vm, argc, argv);

    if(vm.count("help")){
      std::cout << desc << std::endl;
    } else if(argc > 1){
      float threshold = vm["threshold"].as<float>();
      auto* used_memory = new UsedMemory(the_log);
      the_log.setLevel(vm.count("log_debug"));
      PageContainer page(used_memory);
      std::ifstream in(vm["input"].as<std::string>());

      page.Load(in, threshold);

      the_log.Write("Used memory: " + std::to_string(used_memory->used()));
      the_log.Write("Number of skipped items: " +
                    std::to_string(Histogram::Instance().getSkipped()));
      the_log.Write("AVG score: " +
                    std::to_string(Histogram::Instance().getAvg()));

      for (size_t i = 0; i < 5; ++i) {
        const auto& item = page.ByIndex(i);
        std::cout << item.id << "\t" << item.name << ": " << item.score << std::endl;
        std::cout << "- - - - - - - - - - - - - - - - -" <<std::endl;
        const auto& item2 = page.ById(std::to_string(i));
        std::cout << item2.id << "\t" << item2.name << ": " << item2.score << std::endl;
        std::cout << "_________________________________" <<std::endl;
      }

      page.Reload(threshold + 3);

      the_log.Write("Used memory: " + std::to_string(used_memory->used()));
      the_log.Write("Number of skipped items: " +
                    std::to_string(Histogram::Instance().getSkipped()));
      the_log.Write("AVG score: " +
                    std::to_string(Histogram::Instance().getAvg()));
    } else{
      throw po::error(error_mes);
    }
  } catch (const po::error& ex) {
    Log::Instance().Write(ex.what());
  }
  return 0;
}
