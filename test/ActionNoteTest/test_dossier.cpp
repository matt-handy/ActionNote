#include "gtest/gtest.h"
#include "../../include/dossier.h"
#include <filesystem>

using namespace handy::action_note;

TEST(dossier, TestFilePacking) {
	string dossier_file = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0001.txt";
	dossier test_dossier = dossier::from_file(dossier_file);
	ASSERT_EQ("Gary", test_dossier.get_first_name());
	ASSERT_EQ("The", test_dossier.get_middle_names());
	ASSERT_EQ("Human", test_dossier.get_last_name());
	ASSERT_EQ("10/10/1896", test_dossier.get_dob());
	std::stringstream reference_info;
	reference_info << "Gary is a very old human. " << LINE_RETURN;
	ASSERT_EQ(reference_info.str(), test_dossier.get_information());
}

TEST(dossier, TestAddressNoExisting) {
	string dossier_file = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0001.txt";
	dossier test_dossier = dossier::from_file(dossier_file);
	test_dossier.update_current_addr("1234 Nowhere St");

	ASSERT_EQ(test_dossier.get_current_addr(), "1234 Nowhere St");

	//Test that there is no reference to a prior address
	std::stringstream reference_info;
	reference_info << "Gary is a very old human. " << LINE_RETURN;
	ASSERT_EQ(reference_info.str(), test_dossier.get_information());
}

TEST(dossier, TestAddressWithExisting) {
	string dossier_file = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0002.txt";
	dossier test_dossier = dossier::from_file(dossier_file);
	test_dossier.update_current_addr("1234 New St");

	ASSERT_EQ(test_dossier.get_current_addr(), "1234 New St");

	//Test that there is no reference to a prior address
	std::stringstream reference_info;
	reference_info << "Gary is a very old human. " << LINE_RETURN;
	reference_info << "Address updated, prior address" << LINE_RETURN;
	reference_info << "1234 Nowhere St" << LINE_RETURN << "America." << LINE_RETURN << LINE_RETURN;
	ASSERT_EQ(reference_info.str(), test_dossier.get_information());
}

TEST(dossier, TestPhoneNumberAdd) {
	string dossier_file = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0002.txt";
	dossier test_dossier = dossier::from_file(dossier_file);
	test_dossier.add_phone_number("800-123-4567");
	std::stringstream phone_dump;
	phone_dump << PHONE_NUMBERS << LINE_RETURN << "800-555-5555" << LINE_RETURN << "800-123-4567" << LINE_RETURN;
	ASSERT_EQ(phone_dump.str(), test_dossier.dump_phone_numbers());
}

TEST(dossier, TestPhoneNumberSubtraction) {
	string dossier_file = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0002.txt";
	dossier test_dossier = dossier::from_file(dossier_file);
	test_dossier.add_phone_number("800-123-4567");
	std::stringstream phone_dump;
	phone_dump << PHONE_NUMBERS << LINE_RETURN << "800-555-5555" << LINE_RETURN << "800-123-4567" << LINE_RETURN;
	ASSERT_EQ(phone_dump.str(), test_dossier.dump_phone_numbers());

	test_dossier.remove_phone_number("800-555-5555");
	std::stringstream phone_dump2;
	phone_dump2 << PHONE_NUMBERS << LINE_RETURN << "800-123-4567" << LINE_RETURN;
	ASSERT_EQ(phone_dump2.str(), test_dossier.dump_phone_numbers());
}

TEST(dossier, TestEmailAdd) {
	string dossier_file = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0002.txt";
	dossier test_dossier = dossier::from_file(dossier_file);

	std::stringstream email_addrs;
	email_addrs << EMAIL_ADDRESSES << LINE_RETURN << "gary.t.human@gmail.com" << LINE_RETURN << "gary.t.human@yahoo.com" << LINE_RETURN;
	ASSERT_EQ(email_addrs.str(), test_dossier.dump_email_addrs());

	test_dossier.add_email_addr("gary@human.com");
	email_addrs << "gary@human.com" << LINE_RETURN;
	ASSERT_EQ(email_addrs.str(), test_dossier.dump_email_addrs());
}

TEST(dossier, TestEmailSubtraction) {
	string dossier_file = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0002.txt";
	dossier test_dossier = dossier::from_file(dossier_file);

	test_dossier.remove_email_addr("gary.t.human@gmail.com");

	std::stringstream email_addrs;
	email_addrs << EMAIL_ADDRESSES << LINE_RETURN << "gary.t.human@yahoo.com" << LINE_RETURN;
	ASSERT_EQ(email_addrs.str(), test_dossier.dump_email_addrs());
}

TEST(dossier, TestIngestWithContactInfo) {
	string dossier_file = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0002.txt";
	dossier test_dossier = dossier::from_file(dossier_file);
	ASSERT_EQ("Gary", test_dossier.get_first_name());
	ASSERT_EQ("The", test_dossier.get_middle_names());
	ASSERT_EQ("Human", test_dossier.get_last_name());
	ASSERT_EQ("10/10/1896", test_dossier.get_dob());
	std::stringstream reference_info;
	reference_info << "Gary is a very old human. " << LINE_RETURN;
	ASSERT_EQ(reference_info.str(), test_dossier.get_information());

	std::stringstream current_addr;
	current_addr << "1234 Nowhere St" << LINE_RETURN << "America." << LINE_RETURN;
	ASSERT_EQ(current_addr.str(), test_dossier.get_current_addr());

	std::stringstream phone_dump;
	phone_dump << PHONE_NUMBERS << LINE_RETURN << "800-555-5555" << LINE_RETURN;
	ASSERT_EQ(phone_dump.str(), test_dossier.dump_phone_numbers());

	std::stringstream email_addrs;
	email_addrs << EMAIL_ADDRESSES << LINE_RETURN << "gary.t.human@gmail.com" << LINE_RETURN << "gary.t.human@yahoo.com" << LINE_RETURN;
	ASSERT_EQ(email_addrs.str(), test_dossier.dump_email_addrs());
}

TEST(dossier, TestDumpWithContactInfo) {
	string dossier_file = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0002.txt";
	dossier test_dossier = dossier::from_file(dossier_file);

	string dossier_file_p = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0002_p.txt";
	test_dossier.to_file(dossier_file_p, NULL);

	dossier test_dossier_p = dossier::from_file(dossier_file_p);
	std::stringstream current_addr;
	current_addr << "1234 Nowhere St" << LINE_RETURN << "America." << LINE_RETURN;
	ASSERT_EQ(current_addr.str(), test_dossier_p.get_current_addr());

	std::stringstream phone_dump;
	phone_dump << PHONE_NUMBERS << LINE_RETURN << "800-555-5555" << LINE_RETURN;
	ASSERT_EQ(phone_dump.str(), test_dossier_p.dump_phone_numbers());

	std::stringstream email_addrs;
	email_addrs << EMAIL_ADDRESSES << LINE_RETURN << "gary.t.human@gmail.com" << LINE_RETURN << "gary.t.human@yahoo.com" << LINE_RETURN;
	ASSERT_EQ(email_addrs.str(), test_dossier_p.dump_email_addrs());

	std::filesystem::remove(dossier_file_p);
}

TEST(dossier, TestFileAmendment) {
	string dossier_file = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0001.txt";
	dossier test_dossier = dossier::from_file(dossier_file);
	
	test_dossier.append_information("Urgent news: Gary might be supernatural");
	std::stringstream reference_info;
	reference_info << "Gary is a very old human. " << LINE_RETURN;
	reference_info << "Urgent news: Gary might be supernatural" << LINE_RETURN;
	ASSERT_EQ(reference_info.str(), test_dossier.get_information());

	string dossier_file_p = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0001_p.txt";
	test_dossier.to_file(dossier_file_p, NULL);

	dossier test_dossier_p = dossier::from_file(dossier_file_p);
	ASSERT_EQ("Gary", test_dossier.get_first_name());
	ASSERT_EQ("The", test_dossier.get_middle_names());
	ASSERT_EQ("Human", test_dossier.get_last_name());
	ASSERT_EQ("10/10/1896", test_dossier.get_dob());
	ASSERT_EQ(reference_info.str(), test_dossier.get_information());

	std::filesystem::remove(dossier_file_p);
}

TEST(dossier, TestStatUpdates) {
	string dossier_file = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0001.txt";
	dossier test_dossier = dossier::from_file(dossier_file);
	test_dossier.update_first_name("GGary");
	test_dossier.update_middle_names("other");
	test_dossier.update_last_name("HHuman");
	test_dossier.update_dob("10/10/2001");

	ASSERT_EQ("GGary", test_dossier.get_first_name());
	ASSERT_EQ("other", test_dossier.get_middle_names());
	ASSERT_EQ("HHuman", test_dossier.get_last_name());
	ASSERT_EQ("10/10/2001", test_dossier.get_dob());
}

TEST(dossier, TestEncryptionIntegration) {
	string dossier_file = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0001.txt";
	dossier test_dossier = dossier::from_file(dossier_file);

	uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	string dossier_file_p = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\dossier_singles\\0001_p.txt";
	test_dossier.to_file(dossier_file_p, key);

	dossier test_dossier_p = dossier::from_file(dossier_file_p);
	ASSERT_EQ("Gary", test_dossier.get_first_name());
	ASSERT_EQ("The", test_dossier.get_middle_names());
	ASSERT_EQ("Human", test_dossier.get_last_name());
	ASSERT_EQ("10/10/1896", test_dossier.get_dob());
	std::stringstream reference_info;
	reference_info << "Gary is a very old human. " << LINE_RETURN;
	ASSERT_EQ(reference_info.str(), test_dossier.get_information());

	std::filesystem::remove(dossier_file_p);
}

