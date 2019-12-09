#include "gtest/gtest.h"
#include "../../include/action_item.h"
#include "../../include/jstring.h"

#include <fstream>
#include <filesystem>

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
	target_val.append(LINE_RETURN);
	EXPECT_EQ(target_val, str_val);
}

TEST(action_item, StandAloneActionRead) {
	action_item* ai = action_item::get_action_item("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\actionnote-2019-12-06.note");
	EXPECT_TRUE(ai != NULL);

	EXPECT_EQ(ai->get_assignee(), "Myself");
	EXPECT_EQ(ai->get_due_date(), "2020-12-31");
	EXPECT_EQ(ai->get_closed_date(), "");
	EXPECT_EQ(ai->get_start_date(), "2019-12-06");
	EXPECT_EQ(ai->get_action_text(), "Here be an action!");
	EXPECT_EQ(ai->get_notes().size(), 2);
	EXPECT_EQ(ai->get_notes().front(), "2019-12-07 I did stuff");
	EXPECT_EQ(ai->get_notes().back(), "2019-12-08 I thought about doing stuff");
}

TEST(action_item, StandAloneActionWrite) {
	action_item* ai = action_item::get_action_item("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\actionnote-2019-12-06.note");
	EXPECT_TRUE(ai != NULL);

	ai->save_as("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\actionnote-2019-12-06-wb.note");
	delete ai;
	ai = action_item::get_action_item("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\actionnote-2019-12-06.note");

	ifstream canon("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\actionnote-2019-12-06.note");
	ifstream rewrite("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\actionnote-2019-12-06-wb.note");

	EXPECT_TRUE(canon.is_open());
	EXPECT_TRUE(rewrite.is_open());

	string cline;
	string rline;

	while (getline(canon, cline) && getline(rewrite, rline))
	{
		EXPECT_EQ(cline, rline);
	}

	canon.close();
	rewrite.close();

	ai->add_note("2019-12-08 Taking action!!!!");
	EXPECT_TRUE(ai->save());

	action_item* ai_prime = action_item::get_action_item("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\actionnote-2019-12-06.note");
	EXPECT_EQ(ai_prime->get_action_text(), ai->get_action_text());
	EXPECT_EQ(ai_prime->get_assignee(), ai->get_assignee());
	EXPECT_EQ(ai_prime->get_due_date(), ai->get_due_date());
	EXPECT_EQ(ai_prime->get_notes().size(), 3);
	EXPECT_EQ(ai_prime->get_notes().back(), "2019-12-08 Taking action!!!!");

	std::filesystem::remove("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\actionnote-2019-12-06.note");
	std::filesystem::copy("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\actionnote-2019-12-06-wb.note",
		"C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\actionnote-2019-12-06.note");
	std::filesystem::remove("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\actionnote-2019-12-06-wb.note");
}