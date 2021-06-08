// Copyright 2020 Your Name <your_email>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstring>
#include <string>

#include "PageContainer.hpp"
#include "UsedMemory.hpp"

TEST(Example, EmptyTest) {
    EXPECT_TRUE(true);
}
TEST(File, Bad_open){
  PageContainer page{new UsedMemory(Log::Instance())};
  std::ifstream in;
  try{
    page.RawDataLoad(in);
  } catch (std::exception& e) {
    EXPECT_TRUE(std::strcmp(e.what(), "Stream is not exist"));
  }
}
TEST(File, Bad_open2){
  PageContainer page{new UsedMemory(Log::Instance())};
  std::ifstream in;
  try{
    page.Load(in, 1);
  } catch (std::exception& e) {
    EXPECT_TRUE(std::strcmp(e.what(), "Stream is not exist"));
  }
}
TEST(File, EmptyStream){
  PageContainer page{new UsedMemory(Log::Instance())};
  std::stringstream in{" "};
  try{
    page.RawDataLoad(in);
  } catch (std::exception& e) {
    EXPECT_TRUE(std::strcmp(e.what(), "Stream is empty"));
  }
}
TEST(File, EmptyStream2){
  PageContainer page{new UsedMemory(Log::Instance())};
  std::stringstream in{" "};
  try{
    page.Load(in, 1);
  } catch (std::exception& e) {
    EXPECT_TRUE(std::strcmp(e.what(), "Stream is empty"));
  }
}
TEST(File, Small){
  PageContainer page{new UsedMemory(Log::Instance())};
  std::stringstream in{"0 name 1\n"};
  EXPECT_THROW(page.RawDataLoad(in),std::runtime_error);
}
TEST(File, Rigth_fomat){
  PageContainer page{new UsedMemory(Log::Instance())};
  std::stringstream file;
  file << "0 name 10\n1 name 3\n2 name 4\n3 name 3\n4 name 10\n5 name 7\n"
          "6 name 4\n7 name 4\n8 name 5\n9 name 18\n10 name 12";
  page.RawDataLoad(file);
  EXPECT_EQ(11, page.GetRawDataSize());
}
TEST(File, Bad_fomat){
  PageContainer page{new UsedMemory(Log::Instance())};
  std::stringstream file;
  file << "0 name 10\n1 name 3\n2 name 4\n3 name 3\n4 name 10 5 name 7\n"
          "6 name 4\n7 name 4\n8 name 5\n9 name 18\n10 name  12";
  page.RawDataLoad(file);
  EXPECT_EQ(10, page.GetRawDataSize());
}

class MockStatSender : public StatSender {
 public:
  explicit MockStatSender(Log& log1):StatSender(log1){}
  MOCK_METHOD2(AsyncSend, void(const std::vector<Item>&, std::string_view));
};

TEST(Stat, Sender){
  using ::testing::_;
  using ::testing::AtLeast;
  auto* used = new UsedMemory(Log::Instance());
  auto* sender = new MockStatSender(Log::Instance());
  PageContainer page(used, sender);
  std::stringstream ss;
  ss << "0 name 10\n1 name 3\n2 name 4\n3 name 3\n4 name 10\n5 name 7\n"
        "6 name 4\n7 name 4\n8 name 5\n9 name 18\n10 name 12\n11 name 10\n"
        "12 name 17\n13 name 10\n14 name 11\n15 name 9";
  page.RawDataLoad(ss);
  EXPECT_CALL(*sender,
              AsyncSend(_, std::string_view{"/items/loaded"}))
      .Times(2);
  EXPECT_CALL(*sender,
              AsyncSend(_, std::string_view{"/items/skiped"}))
      .Times(5);
  page.Reload(1);
  page.Reload(4);
}
TEST(Memory, UsedMemory){
  std::vector<std::string> old_raw_data{ "line1" };
  std::vector<std::string> new_raw_data {"line1", "line2", "line3", "line4"};
  UsedMemory usedMemory1(Log::Instance());
  usedMemory1.OnRawDataLoad(old_raw_data, new_raw_data);
  EXPECT_EQ(45,usedMemory1.used());
  std::vector<Item> old_data{ {"0", "name", 0} };
  std::vector<Item> new_data {{"0", "name", 0},
                              {"1", "name", 0},
                              {"2", "name", 0}};
  UsedMemory usedMemory2(Log::Instance());
  usedMemory2.OnDataLoad(old_data, new_data);
  EXPECT_EQ(68,usedMemory2.used());
}
TEST(Page, Right){
  PageContainer page{new UsedMemory(Log::Instance())};
  std::stringstream file;
  file << "0 name 10\n1 name 3\n2 name 4\n3 name 3\n4 name 10\n5 name 7\n"
          "6 name 4\n7 name 4\n8 name 5\n9 name 18\n10 name 12";
  page.Load(file, 0);
  EXPECT_EQ(page.GetDataSize(), 11);
}
TEST(Page, already_seen){
  PageContainer page{new UsedMemory(Log::Instance())};
  std::stringstream file;
  file << "0 name 10\n1 name 3\n2 name 4\n3 name 3\n4 name 10\n5 name 7\n"
          "6 name 4\n1 name 4\n8 name 5\n9 name 18\n10 name 12";
  EXPECT_THROW(page.Load(file, 0),std::runtime_error);
}
TEST(Page, small_correct_data){
  PageContainer page{new UsedMemory(Log::Instance())};
  std::stringstream file;
  file << "0 name 10\n1 name 3\n2 name 4\n3 name 3\n4 name 10\n5 name 7\n"
          "6 name 4\n7 name 4\n8 name 5\n9 name 18\n10 name 12";
  EXPECT_THROW(page.Load(file, 4),std::runtime_error);
}
