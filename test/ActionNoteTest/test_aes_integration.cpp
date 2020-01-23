#include "gtest/gtest.h"
#include "aes.hpp"
#include "../../include/meeting.h"
#include "../../include/action_item.h"
#include "../../include/meeting_manager.h"
#include <filesystem>

using namespace handy::action_note;

TEST(aes_integration, TestAesBasicLink) {

	uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	uint8_t out[] = { 0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
					  0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
					  0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
					  0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7 };

	uint8_t iv[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	uint8_t in[] = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
					  0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
					  0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
					  0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10 };

	uint8_t in_org[] = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
					  0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
					  0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
					  0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10 };
	struct AES_ctx ctx;

	AES_init_ctx_iv(&ctx, key, iv);
	AES_CBC_encrypt_buffer(&ctx, in, 64);

	ASSERT_TRUE(0 == memcmp((char*)out, (char*)in, 64));

	AES_init_ctx_iv(&ctx, key, iv);
	AES_CBC_decrypt_buffer(&ctx, in, 64);

	ASSERT_TRUE(0 == memcmp((char*)in_org, (char*)in, 64));

	
	std::string testString = "This is my test. It should be encrypted";
	unsigned char* workingBuff = new unsigned char[testString.length() + 1];
	memcpy(workingBuff, testString.c_str(), testString.length() + 1);

	AES_init_ctx_iv(&ctx, key, iv);
	AES_CBC_encrypt_buffer(&ctx, (uint8_t *)workingBuff, testString.length() + 1);
	
	AES_init_ctx_iv(&ctx, key, iv);
	AES_CBC_decrypt_buffer(&ctx, (uint8_t*)workingBuff, testString.length() + 1);
	
	ASSERT_TRUE(0 == memcmp((char*)testString.c_str(), (char*)workingBuff, 40));

}

TEST(aes_integration, TestActionNoteInOut) {
	action_item* test_ai = action_item::get_action_item("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\actionnote-2019-12-06.note");
	string cypher_file_name = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\actionnote-2019-12-06_cypher.note";
	const char* key = "testtesttesttest";
	test_ai->save_as(cypher_file_name, (uint8_t *)key);

	action_item* test_ai_rebuilt = action_item::get_action_item(cypher_file_name, (uint8_t*)key);
	ASSERT_EQ(test_ai_rebuilt->get_notes().size(), test_ai->get_notes().size());
	ASSERT_EQ(test_ai_rebuilt->get_action_text(), test_ai->get_action_text());
	ASSERT_EQ(test_ai_rebuilt->render_text(), test_ai->render_text());
}

TEST(aes_integration, TestMeetingManagerCreateEncryptedActionNote) {
	string basedir = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting_manager_enc";
	const char* key = "testtesttesttest";
	meeting_manager my_man(basedir, (uint8_t*)key);
	my_man.initialize();
	my_man.create_action_note("test_creator_enc", "myself", "2019-12-12", "2019-12-08", "This is an action I'm creating to testing stuff");

	string newFileName = basedir;
	newFileName.append(PATH_SEP);
	newFileName.append("test_creator_enc");
	newFileName.append(timeutil::get_YYYYMMDD_current());
	newFileName.append(".note");

	EXPECT_TRUE(filesystem::exists(newFileName));
	EXPECT_TRUE(my_man.get_open_actions().size() == 1);

	action_item* test_ai_rebuilt = action_item::get_action_item(newFileName, (uint8_t*)key);
	EXPECT_EQ(test_ai_rebuilt->get_assignee(), "myself");

	std::filesystem::remove(newFileName);
}

TEST(aes_integration, TestMeetingInOut) {
	meeting* test_meeting = meeting::from_file("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting-2019-09-13.mt");
	string cypher_file_name = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\meeting-2019-09-13_cypher.mt";
	const char * key = "testtesttesttest";
	test_meeting->to_file(cypher_file_name, (uint8_t*) key);

	meeting* test_meeting_rebuilt = meeting::from_file(cypher_file_name, (uint8_t*) key);
	ASSERT_EQ(test_meeting_rebuilt->get_actions().size(), test_meeting->get_actions().size());
	ASSERT_EQ(test_meeting_rebuilt->get_attendees().size(), test_meeting->get_attendees().size());
	ASSERT_EQ(test_meeting_rebuilt->get_elements().size(), test_meeting->get_elements().size());
	
	list<meeting_element*> elements = test_meeting->get_elements();
	list<meeting_element*>::iterator element_itr = elements.begin();
	list<meeting_element*> elements_r = test_meeting_rebuilt->get_elements();
	list<meeting_element*>::iterator element_itr_r = elements_r.begin();

	for (int idx = 0; idx < test_meeting_rebuilt->get_elements().size(); idx++) {
		string element = (*element_itr)->render_text();
		string element_r = (*element_itr_r)->render_text();
		EXPECT_EQ(element, element_r);

		element_itr++;
		element_itr_r++;
	}

	

}