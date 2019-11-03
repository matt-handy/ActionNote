#include "gtest/gtest.h"
#include "../../include/jstring.h";

using namespace handy::action_note;

TEST(SString, DoesEQUALITY) {
  jstring control("Control");
  jstring notControl("Not control");
  jstring otherControl("Control");
  ASSERT_TRUE(control.equals(otherControl));
  ASSERT_FALSE(control.equals(notControl));
}

TEST(SString, DoesCONTAINS){
  jstring control("Control");
  jstring notControl("Not control");
  jstring otherControl("control");
  jstring defNotControl("definitely not");

  ASSERT_FALSE(control.contains(notControl));
  ASSERT_TRUE(notControl.contains(otherControl));
  ASSERT_FALSE(defNotControl.contains(otherControl));
}

TEST(SString, DoesEMPTY){
	jstring empty("");
	jstring notEmpty("not");

  ASSERT_TRUE(empty.empty());
  ASSERT_FALSE(notEmpty.empty());
}

TEST(SString, DoesSPLIT){
	jstring base("This is my super awesome base string");
	jstring nullDelim("NULL");
	jstring white(" ");
	jstring eWhite("e ");

  std::vector<jstring> nullV = base.split(nullDelim);
  ASSERT_TRUE(nullV.size() == 1);
  ASSERT_TRUE(nullV.at(0).equals(jstring("This is my super awesome base string")));

  std::vector<jstring> whiteV = base.split(white);
  ASSERT_TRUE(whiteV.size() == 7);
  ASSERT_TRUE(whiteV.at(0).equals(jstring("This")));
  ASSERT_TRUE(whiteV.at(1).equals(jstring("is")));
  ASSERT_TRUE(whiteV.at(2).equals(jstring("my")));
  ASSERT_TRUE(whiteV.at(3).equals(jstring("super")));
  ASSERT_TRUE(whiteV.at(4).equals(jstring("awesome")));
  ASSERT_TRUE(whiteV.at(5).equals(jstring("base")));
  ASSERT_TRUE(whiteV.at(6).equals(jstring("string")));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
