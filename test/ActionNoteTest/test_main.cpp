#include "gtest/gtest.h"
#include "../../include/main.h";
#include <filesystem>

namespace fs = std::filesystem;
using namespace handy::action_note;

TEST(runner, DoesBasicMeetingWork) {
	std::stringbuf buffer;
	std::ostream os(&buffer);
	std::istream is(&buffer);

	os << "minutes" << endl;
	os << "@@at Steve" << endl;
	os << "@@at That Guy" << endl;
	os << "Something happened." << endl;
	os << "More content" << endl;
	os << "additional content" << endl;
	os << "@@at Late guy" << endl;
	os << "@@save meeting" << endl;
	os << "quit" << endl;

	string content_check = "Something happened.";
	content_check.append(LINE_RETURN);
	content_check.append("More content");
	content_check.append(LINE_RETURN);
	content_check.append("additional content");
	content_check.append(LINE_RETURN);

	string dir = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\runner_test";

	std::stringbuf ret_buffer;
	std::ostream r_os(&ret_buffer);
	std::istream r_is(&ret_buffer);

	runner my_main;
	my_main.loop(is, r_os, dir);

	int fileCount = 0;
	for (auto& p : fs::directory_iterator(dir)) {
		fileCount++;
		//Assume that only files with extensions are valid meetings
		if (p.path().extension().generic_string().find("mt") != string::npos) {

			meeting* read_meeting = meeting::from_file(p.path().string());
			EXPECT_TRUE(read_meeting != NULL);
			EXPECT_TRUE(read_meeting->get_attendees().size() == 3);
			EXPECT_TRUE(read_meeting->get_attendees().front().find("Steve") != string::npos);
			EXPECT_TRUE(read_meeting->get_attendees().back().find("Late guy") != string::npos);
			
			EXPECT_TRUE(read_meeting->get_elements().size() == 1);
			EXPECT_TRUE(read_meeting->get_elements().front()->render_text().find(content_check) != string::npos);
			
			EXPECT_TRUE(remove(p.path()));
		}
		else if (p.path().extension().generic_string().find("note") != string::npos) {
			FAIL("No notes should be present");
		}
	}
	EXPECT_TRUE(fileCount == 1);

	string line;
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Enter your minutes!");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "@@at <Attendee Name> to add to attendee list");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "@@act to enter action wizard");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "@@save <Meeting Name> to close out meeting");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Meeting saved");
}

TEST(runner, DoesQuickActionWork) {
	std::stringbuf buffer;
	std::ostream os(&buffer);
	std::istream is(&buffer);

	os << "qa" << endl;
	os << "test_qa" << endl;
	os << "myself" << endl;
	os << "tomorrow" << endl;
	os << "now" << endl;
	os << "Details on the action" << endl;
	os << "More details" << endl;
	os << "do stuff!!!" << endl;
	os << "done" << endl;
	os << "quit" << endl;

	string dir = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\runner_test";

	std::stringbuf ret_buffer;
	std::ostream r_os(&ret_buffer);
	std::istream r_is(&ret_buffer);

	runner my_main;
	my_main.loop(is, r_os, dir);

	int fileCount = 0;
	for (auto& p : fs::directory_iterator(dir)) {
		fileCount++;
		//Assume that only files with extensions are valid meetings
		if (p.path().extension().generic_string().find("mt") != string::npos) {
			FAIL("No minutes should be present");
			
			
		}
		else if (p.path().extension().generic_string().find("note") != string::npos) {
			string action_content = "Details on the action";
			action_content.append(LINE_RETURN);
			action_content.append("More details");
			action_content.append(LINE_RETURN);
			action_content.append("do stuff!!!");
			
			action_item* ai = action_item::get_action_item(p.path().string());
			EXPECT_TRUE(ai->get_notes().size() == 0);
			EXPECT_EQ(ai->get_assignee(), "myself");
			EXPECT_EQ(ai->get_due_date(), "tomorrow");
			EXPECT_EQ(ai->get_start_date(), "now");
			EXPECT_TRUE(ai->get_action_text().find(action_content) != string::npos);

			EXPECT_TRUE(remove(p.path()));
		}
	}
	EXPECT_TRUE(fileCount == 1);

	string line;
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Mnemonic?");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Actionee? (default is myself)");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Due Date?");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Start Date?");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Enter the action");
}

TEST(runner, DoesMinuteWithAction) {
	std::stringbuf buffer;
	std::ostream os(&buffer);
	std::istream is(&buffer);

	os << "minutes" << endl;
	os << "@@at Steve" << endl;
	os << "@@at That Guy" << endl;
	os << "Something happened." << endl;
	os << "More content" << endl;
	os << "additional content" << endl;
	os << "@@at Late guy" << endl;
	os << "@@act" << endl;
	os << "test_qa" << endl;
	os << "myself" << endl;
	os << "tomorrow" << endl;
	os << "now" << endl;
	os << "Details on the action" << endl;
	os << "More details" << endl;
	os << "do stuff!!!" << endl;
	os << "done" << endl;
	os << "quit" << endl;
	os << "@@save meeting" << endl;
	os << "quit" << endl;

	string content_check = "Something happened.";
	content_check.append(LINE_RETURN);
	content_check.append("More content");
	content_check.append(LINE_RETURN);
	content_check.append("additional content");
	content_check.append(LINE_RETURN);

	string dir = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\runner_test";

	std::stringbuf ret_buffer;
	std::ostream r_os(&ret_buffer);
	std::istream r_is(&ret_buffer);

	runner my_main;
	my_main.loop(is, r_os, dir);

	int fileCount = 0;
	for (auto& p : fs::directory_iterator(dir)) {
		fileCount++;
		//Assume that only files with extensions are valid meetings
		if (p.path().extension().generic_string().find("mt") != string::npos) {

			meeting* read_meeting = meeting::from_file(p.path().string());
			EXPECT_TRUE(read_meeting != NULL);
			EXPECT_TRUE(read_meeting->get_attendees().size() == 3);
			EXPECT_TRUE(read_meeting->get_attendees().front().find("Steve") != string::npos);
			EXPECT_TRUE(read_meeting->get_attendees().back().find("Late guy") != string::npos);

			EXPECT_TRUE(read_meeting->get_elements().size() == 3);
			EXPECT_TRUE(read_meeting->get_elements().front()->render_text().find(content_check) != string::npos);

			EXPECT_TRUE(remove(p.path()));
		}
		else if (p.path().extension().generic_string().find("note") != string::npos) {
			FAIL("No notes should be present");
		}
	}
	EXPECT_TRUE(fileCount == 1);

	string line;
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Enter your minutes!");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "@@at <Attendee Name> to add to attendee list");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "@@act to enter action wizard");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "@@save <Meeting Name> to close out meeting");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Mnemonic?");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Actionee? (default is myself)");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Due Date?");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Start Date?");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Enter the action");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Action saved");
	EXPECT_TRUE(getline(r_is, line));
	EXPECT_EQ(line, "Meeting saved");
}