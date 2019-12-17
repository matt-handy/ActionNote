#include "gtest/gtest.h"
#include "../../include/meeting_manager.h"
#include "../../include/timeutil.h"
#include <filesystem>

using namespace handy::action_note;

TEST(meeting_manager, DoesAddNote) {
	std::filesystem::remove("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_writeback\\meeting-2019-09-13-mod.mt");
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
	std::filesystem::remove("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_writeback\\meeting-2019-09-13-mod.mt");
	std::filesystem::copy("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_writeback\\meeting-2019-09-13-orig.mt", 
		"C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_writeback\\meeting-2019-09-13-mod.mt");
	meeting_manager my_man("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_writeback");
	my_man.initialize();
	vector<action_item*> open_actions = my_man.get_open_actions();
	
	EXPECT_EQ(open_actions.at(0)->get_parent_meeting()->get_original_file(), "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_writeback\\meeting-2019-09-13-mod.mt");
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
	string action_text = "TAKE RANDOM ACTION";
	ASSERT_EQ(first->get_action_text(), action_text);
	
	action_item* second = open_actions.at(1);
	ASSERT_EQ(second->get_closed_date().size(), 0);
	ASSERT_EQ(second->get_notes().size(), 2);
	action_text = "This is a followup action";
	ASSERT_EQ(second->get_action_text(), action_text);
	
	action_item* third = open_actions.at(2);
	ASSERT_EQ(third->get_closed_date().size(), 0);
	ASSERT_EQ(third->get_notes().size(), 0);
	action_text = "TAKE RANDOM ACTION";
	ASSERT_EQ(third->get_action_text(), action_text);
	
}

TEST(meeting_manager, DoesLoadOpenActionsFromActionNotes) {
	meeting_manager my_man("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_actions");
	my_man.initialize();
	vector<action_item*> open_actions = my_man.get_open_actions();
	ASSERT_EQ(open_actions.size(), 4);

	action_item* first = open_actions.at(0);
	ASSERT_EQ(first->get_closed_date().size(), 0);
	ASSERT_EQ(first->get_notes().size(), 2);
	ASSERT_EQ(first->get_notes().front(), "2019-12-07 I did stuff");
	ASSERT_EQ(first->get_notes().back(), "2019-12-08 I thought about doing stuff");
	string action_text = "Here be an action!";
	ASSERT_EQ(first->get_action_text(), action_text);
}

TEST(meeting_manager, DoesWriteBackActionNote) {
	std::filesystem::remove("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_actions_writeback\\actionnote-2019-12-06.note");
	std::filesystem::copy("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_actions_writeback\\actionnote-2019-12-06-orig.note",
		"C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_actions_writeback\\actionnote-2019-12-06.note");
	meeting_manager my_man("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_actions_writeback");
	my_man.initialize();
	vector<action_item*> open_actions = my_man.get_open_actions();

	EXPECT_EQ(open_actions.size(), 5);
	action_item* ai = open_actions.at(1);
	EXPECT_EQ(ai->get_parent_meeting(), (meeting*)NULL);
	EXPECT_EQ(ai->get_filename(), "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_actions_writeback\\actionnote-2019-12-06.note");

	my_man.addActionNote("Noted a thing!!!", 1);
	EXPECT_TRUE(my_man.save_action(ai));

	action_item* reloaded_ai = action_item::get_action_item("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_actions_writeback\\actionnote-2019-12-06.note");
	EXPECT_EQ(reloaded_ai->get_notes().size(), 3);
}

TEST(meeting_manager, DoesCreateNewActionNote) {
	string basedir = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager";
	meeting_manager my_man(basedir);
	my_man.initialize();
	my_man.create_action_note("test_creator", "myself", "2019-12-12", "2019-12-08", "This is an action I'm creating to testing stuff");

	string newFileName = basedir;
	newFileName.append(PATH_SEP);
	newFileName.append("test_creator");
	newFileName.append(timeutil::get_YYYYMMDD_current());
	newFileName.append(".note");

	EXPECT_TRUE(filesystem::exists(newFileName));
	EXPECT_TRUE(my_man.get_open_actions().size() == 4);

	std::filesystem::remove(newFileName);
}