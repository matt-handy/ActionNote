#include "gtest/gtest.h"
#include "../../include/meeting.h"
#include <fstream>

using namespace handy::action_note;

TEST(meeting, DoesLoadAttendeeTest) {
	meeting* test_meeting = meeting::from_file("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting-2019-09-13.txt");
	EXPECT_EQ(test_meeting->get_attendees().size(), 3);
	list<string> attendees = test_meeting->get_attendees();
	list<string>::iterator attendee_itr = attendees.begin();
	EXPECT_EQ(*attendee_itr, "That one");
	attendee_itr++;
	EXPECT_EQ(*attendee_itr, "Another Guy");
	attendee_itr++;
	EXPECT_EQ(*attendee_itr, "Stupid head");
}

TEST(meeting, DoesLoadAiTest) {
	meeting* test_meeting = meeting::from_file("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting-2019-09-13.txt");
	list<meeting_element*> elements = test_meeting->get_elements();
	list<meeting_element *>::iterator element_itr = elements.begin();
	
	EXPECT_EQ(test_meeting->get_elements().size(), 5);

	string first_canon = "This is a test dataset";
	first_canon.append(LINE_RETURN);
	first_canon.append("it doesn't really mean anything");
	first_canon.append(LINE_RETURN);
	first_canon.append("More random words");
	first_canon.append(LINE_RETURN);
	first_canon.append("random words");
	first_canon.append(LINE_RETURN);
	first_canon.append(LINE_RETURN);
	string first_element = (*element_itr)->render_text();
	EXPECT_EQ(first_canon, first_element);

	element_itr++;
	string second_element = (*element_itr)->render_text();
	string second_canon = "@@AI Myself @@start 2019-09-14 @@due 2019-10-17";
	second_canon.append(LINE_RETURN);
	second_canon.append("TAKE RANDOM ACTION");
	second_canon.append(LINE_RETURN);
	second_canon.append("@@endAI");
	second_canon.append(LINE_RETURN);
	EXPECT_EQ(second_canon, second_element);

	element_itr++;
	string third_element = (*element_itr)->render_text();
	string third_canon = LINE_RETURN;
	third_canon.append("Here is more text of the meeting, we continue talking");
	third_canon.append(LINE_RETURN);
	third_canon.append("and talking");
	third_canon.append(LINE_RETURN);
	third_canon.append("and talking more");
	third_canon.append(LINE_RETURN);
	third_canon.append(LINE_RETURN);
	EXPECT_EQ(third_canon, third_element);

	element_itr++;
	string fourth_element = (*element_itr)->render_text();
	string fourth_canon = "@@AI Someone @@start 2019-10-18";
	fourth_canon.append(LINE_RETURN);
	fourth_canon.append("This is a followup action");
	fourth_canon.append(LINE_RETURN);
	fourth_canon.append("@@note 2019-07-08 Someone thought the action was dump");
	fourth_canon.append(LINE_RETURN);
	fourth_canon.append("@@note 2019-07-19 Someone else thought the action sound");
	fourth_canon.append(LINE_RETURN);
	fourth_canon.append("@@endAI");
	fourth_canon.append(LINE_RETURN);
	EXPECT_EQ(fourth_canon, fourth_element);
	
	element_itr++;
	string fifth_element = (*element_itr)->render_text();
	string fifth_canon = LINE_RETURN;
	fifth_canon.append("Here is the rest of the meeting");
	fifth_canon.append(LINE_RETURN);
	fifth_canon.append("Meeting");
	fifth_canon.append(LINE_RETURN);
	fifth_canon.append("meeting....");
	fifth_canon.append(LINE_RETURN);
	fifth_canon.append(LINE_RETURN);
	fifth_canon.append("end of meeting test!");
	fifth_canon.append(LINE_RETURN);
	EXPECT_EQ(fifth_canon, fifth_element);
}

TEST(meeting, DoesWriteTest) {
	meeting* test_meeting = meeting::from_file("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting-2019-09-13.txt");
	test_meeting->to_file("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting-2019-09-13-wb.txt");

	ifstream canon("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting-2019-09-13.txt");
	ifstream rewrite("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting-2019-09-13-wb.txt");

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
}

TEST(meeting, DoesWriteTestActionClosed) {
	meeting* test_meeting = meeting::from_file("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting-2019-09-10.txt");
	test_meeting->to_file("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting-2019-09-10-wb.txt");

	ifstream canon("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting-2019-09-10.txt");
	ifstream rewrite("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting-2019-09-10-wb.txt");

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
}

