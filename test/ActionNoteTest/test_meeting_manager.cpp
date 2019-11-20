#include "gtest/gtest.h"
#include "../../include/meeting_manager.h"
#include <filesystem>

using namespace handy::action_note;

TEST(meeting_manager, DoesAddNote) {
	std::filesystem::remove("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_writeback\\meeting-2019-09-13-mod.txt");
	meeting_manager my_man("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_writeback");
	my_man.initialize();
	my_man.addActionNote("Adding another note!", 0);

	meeting_manager my_other_man("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_writeback");
	my_other_man.initialize();
	EXPECT_EQ(my_other_man.get_open_actions().size(), 1);
	action_item* target_ai = my_other_man.get_open_actions().at(0);
	EXPECT_EQ(target_ai->get_notes().size(), 1);
	EXPECT_TRUE((*target_ai->get_notes().begin()).find("Adding another note!") != std::string::npos);

	target_ai->remove_note(0);
	EXPECT_EQ(target_ai->get_notes().size(), 0);
	my_other_man.save_action(target_ai);
}

TEST(meeting_manager, DoesWriteBackAction) {
	std::filesystem::remove("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_writeback\\meeting-2019-09-13-mod.txt");
	std::filesystem::copy("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_writeback\\meeting-2019-09-13-orig.txt", 
		"C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_writeback\\meeting-2019-09-13-mod.txt");
	meeting_manager my_man("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_writeback");
	my_man.initialize();
	vector<action_item*> open_actions = my_man.get_open_actions();
	
	EXPECT_EQ(open_actions.at(0)->get_parent_meeting()->get_original_file(), "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_writeback\\meeting-2019-09-13-mod.txt");
	open_actions.at(0)->add_note("10/29/29 WOAH, shoulda dont this years ago!!!");
	my_man.save_action(open_actions.at(0));

	EXPECT_EQ(my_man.get_open_actions().size(), 2);
	action_item* target_ai = my_man.get_open_actions().at(0);
	EXPECT_EQ(target_ai->get_notes().size(), 1);
	EXPECT_TRUE((*target_ai->get_notes().begin()).find("10/29/29 WOAH, shoulda dont this years ago!!!") != std::string::npos);
}

TEST(meeting_manager, DoesLoadOpenActions) {
	meeting_manager my_man("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager");
	my_man.initialize();
	vector<action_item*> open_actions = my_man.get_open_actions();
	ASSERT_EQ(open_actions.size(), 3);

	action_item* first = open_actions.at(0);
	ASSERT_EQ(first->get_closed_date().size(), 0);
	ASSERT_EQ(first->get_notes().size(), 0);
	ASSERT_EQ(first->get_action_text(), "TAKE RANDOM ACTION");
	
	action_item* second = open_actions.at(1);
	ASSERT_EQ(second->get_closed_date().size(), 0);
	ASSERT_EQ(second->get_notes().size(), 2);
	ASSERT_EQ(second->get_action_text(), "This is a followup action");
	
	action_item* third = open_actions.at(2);
	ASSERT_EQ(third->get_closed_date().size(), 0);
	ASSERT_EQ(third->get_notes().size(), 0);
	ASSERT_EQ(third->get_action_text(), "TAKE RANDOM ACTION");
	
}