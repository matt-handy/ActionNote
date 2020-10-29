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

