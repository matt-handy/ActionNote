#include "gtest/gtest.h"
#include "../../include/action_item.h"
#include "../../include/jstring.h"

using namespace handy::action_note;

TEST(action_item, TestObjectSetup) {
	action_item test_ai = action_item("myself", "12/14/2019", "12/15/2019", "DO ALL OF THE THINGS", "", NULL);
	test_ai.add_note("12/13/2019 - I prepared to do the thing");
	test_ai.add_note("12/14/2019 - I started to do the thing");

	EXPECT_EQ(test_ai.get_action_text(), "DO ALL OF THE THINGS");
	EXPECT_EQ(test_ai.get_assignee(), "myself");
	EXPECT_EQ(test_ai.get_start_date(), "12/14/2019");
	EXPECT_EQ(test_ai.get_due_date(), "12/15/2019");
	EXPECT_EQ(test_ai.get_closed_date(), "");

	list<string> notes = test_ai.get_notes();
	EXPECT_EQ(notes.size(), 2);

	list<string>::iterator notes_itr = notes.begin();
	EXPECT_EQ(*notes_itr, "12/13/2019 - I prepared to do the thing");
	notes_itr++;
	EXPECT_EQ(*notes_itr, "12/14/2019 - I started to do the thing");

	string value = test_ai.render_text();
	jstring jvalue = jstring(value.c_str());
	vector<jstring> lines = jvalue.split(jstring(LINE_RETURN.c_str()));
	EXPECT_EQ(lines.size(), 5);

	EXPECT_TRUE(lines.at(0).equals("@@AI myself @@start 12/14/2019 @@due 12/15/2019"));
	EXPECT_TRUE(lines.at(1).equals("DO ALL OF THE THINGS"));
	EXPECT_TRUE(lines.at(2).equals("@@note 12/13/2019 - I prepared to do the thing"));
	EXPECT_TRUE(lines.at(3).equals("@@note 12/14/2019 - I started to do the thing"));
	EXPECT_TRUE(lines.at(4).equals("@@endAI"));

}

TEST(action_item, TestToText) {
	action_item test_ai = action_item("myself", "12/14/2019", "12/15/2019", "DO ALL OF THE THINGS", "2019-07-19", NULL);
	test_ai.add_note("12/13/2019 - I prepared to do the thing");
	test_ai.add_note("12/14/2019 - I started to do the thing");
	string str_val = test_ai.render_text();

	string target_val = "@@AI myself @@start 12/14/2019 @@due 12/15/2019";
	target_val.append(LINE_RETURN);
	target_val.append("DO ALL OF THE THINGS");
	target_val.append(LINE_RETURN);
	target_val.append("@@note 12/13/2019 - I prepared to do the thing");
	target_val.append(LINE_RETURN);
	target_val.append("@@note 12/14/2019 - I started to do the thing");
	target_val.append(LINE_RETURN);
	target_val.append("@@closed 2019-07-19");
	target_val.append(LINE_RETURN);
	target_val.append("@@endAI");
}