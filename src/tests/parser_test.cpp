#include "model/parser.h"

#include "gtest/gtest.h"

class ParserTest : public ::testing::Test {
 protected:
  void SetUp() override {
    ClearLogFile();
  }

  void TearDown() override {}

  void ClearLogFile() {
    std::ofstream clear_log("logs/debug.log", std::ios::trunc);
    clear_log.close();
  }

  std::string GetLastLogMessage() {
    std::ifstream log_file("logs/debug.log");
    std::string last_line;
    if (log_file.is_open()) {
      std::string line;
      while (std::getline(log_file, line)) {
        last_line = line;
      }
      log_file.close();
    }
    return last_line;
  }
};

TEST_F(ParserTest, non_existent_file) {
  s21::WireframeObject obj("samples/non_existent_file.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_NE(log_string.find("File not found"), std::string::npos);
  EXPECT_EQ(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "");
  EXPECT_EQ(obj.GetVertices().size(), 0);
  EXPECT_EQ(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, empty_file) {
  s21::WireframeObject obj("samples/empty.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_NE(log_string.find("Invalid file format"), std::string::npos);
  EXPECT_EQ(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "");
  EXPECT_EQ(obj.GetVertices().size(), 0);
  EXPECT_EQ(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, corrupt_sample_0) {
  s21::WireframeObject obj("samples/corrupt_sample_0.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_NE(log_string.find("Invalid format in line"), std::string::npos);
  EXPECT_EQ(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "");
  EXPECT_EQ(obj.GetVertices().size(), 0);
  EXPECT_EQ(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, corrupt_sample_1) {
  s21::WireframeObject obj("samples/corrupt_sample_1.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_NE(log_string.find("Invalid format in line"), std::string::npos);
  EXPECT_EQ(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "");
  EXPECT_EQ(obj.GetVertices().size(), 0);
  EXPECT_EQ(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, corrupt_sample_2) {
  s21::WireframeObject obj("samples/corrupt_sample_2.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_NE(log_string.find("Invalid format in line"), std::string::npos);
  EXPECT_EQ(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "");
  EXPECT_EQ(obj.GetVertices().size(), 0);
  EXPECT_EQ(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, valid_sample_0) {
  s21::WireframeObject obj("samples/simple.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_EQ(log_string, "");
  EXPECT_NE(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "simple.obj");
  EXPECT_GT(obj.GetVertices().size(), 0);
  EXPECT_GT(obj.GetFaces().size(), 0);
}
