#include "gtest/gtest.h"
#include "../../include/dossier_manager.h"
#include <fstream>
#include <filesystem>

using namespace handy::action_note;

TEST(dossier_manager, DoesLoadDossierSetUnencrypted) {
	dossier_manager manager("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\unencrypted_dossier_master");
	manager.initialize();

	std::stringbuf buffer;
	std::ostream os(&buffer);
	std::istream is(&buffer);
	os << "ld" << endl;
	os << "quit" << endl;

	std::stringbuf ret_buffer;
	std::ostream r_os(&ret_buffer);
	std::istream r_is(&ret_buffer);
	manager.ui_loop(is, r_os);

	string line;
	ASSERT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "1 : Gary Human");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "3 : Larry Inhuman");
	EXPECT_FALSE(getline(r_is, line));
}

TEST(dossier_manager, DoesEditDossierUnencrypted) {
	std::filesystem::copy("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\unencrypted_dossier_master",
		"C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\unencrypted_dossier_scratch");

	dossier_manager manager("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\unencrypted_dossier_scratch");
	manager.initialize();

	std::stringbuf buffer;
	std::ostream os(&buffer);
	std::istream is(&buffer);
	os << "od 1" << endl;
	os << "ufn GGary" << endl;
	os << "umn other" << endl;
	os << "uln HHuman" << endl;
	os << "udob 10/24/2001" << endl;
	os << "ainf" << endl;
	os << "I heard Gary is evil" << endl;
	os << "Steve thinks Gary is evil too" << endl;
	os << "<done>" << endl;
	os << "od 1" << endl;
	os << "quit" << endl;

	std::stringbuf ret_buffer;
	std::ostream r_os(&ret_buffer);
	std::istream r_is(&ret_buffer);
	manager.ui_loop(is, r_os);

	string line;
	ASSERT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "First Name: Gary");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "Middle Name(s): The");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "Last Name: Human");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "DOB: 10/10/1896");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "Gary is a very old human. ");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "");
	
	ASSERT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "Enter additional info. '<done>' to complete entry.");
	ASSERT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "First Name: GGary");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "Middle Name(s): other");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "Last Name: HHuman");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "DOB: 10/24/2001");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "Gary is a very old human. ");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "I heard Gary is evil");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "Steve thinks Gary is evil too");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "");


	std::filesystem::remove("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\unencrypted_dossier_scratch\\00001.txt");
	std::filesystem::remove("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\unencrypted_dossier_scratch\\00003.txt");
}

TEST(dossier_manager, AddDossier) {
	std::filesystem::copy("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\unencrypted_dossier_master",
		"C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\unencrypted_dossier_scratch");

	dossier_manager manager("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\unencrypted_dossier_scratch");
	manager.initialize();

	std::stringbuf buffer;
	std::ostream os(&buffer);
	std::istream is(&buffer);
	os << "add" << endl;
	os << "Barry" << endl;
	os << "the" << endl;
	os << "Chopper" << endl;
	os << "1989" << endl;
	os << "He likes to chop stuff" << endl;
	os << "additional info" << endl;
	os << "<done>" << endl;
	os << "quit" << endl;

	std::stringbuf ret_buffer;
	std::ostream r_os(&ret_buffer);
	std::istream r_is(&ret_buffer);
	manager.ui_loop(is, r_os);

	string line;
	ASSERT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "First name?: ");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "Middle name?: ");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "Last name?: ");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "DOB?: ");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "Enter information. '<done>' to quit");
	EXPECT_FALSE(getline(r_is, line));
	

	string dossier_file = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\unencrypted_dossier_scratch\\00002.txt";
	dossier test_dossier = dossier::from_file(dossier_file);
	ASSERT_EQ("Barry", test_dossier.get_first_name());
	ASSERT_EQ("the", test_dossier.get_middle_names());
	ASSERT_EQ("Chopper", test_dossier.get_last_name());
	ASSERT_EQ("1989", test_dossier.get_dob());
	std::stringstream reference_info;
	reference_info << "He likes to chop stuff" << LINE_RETURN;
	reference_info << "additional info" << LINE_RETURN;
	ASSERT_EQ(reference_info.str(), test_dossier.get_information());

	std::filesystem::remove("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\unencrypted_dossier_scratch\\00001.txt");
	std::filesystem::remove("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\unencrypted_dossier_scratch\\00002.txt");
	std::filesystem::remove("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\unencrypted_dossier_scratch\\00003.txt");
}

TEST(dossier_manager, EncryptedDossierSet) {
	uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	dossier_manager manager("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\encrypted_dossier_master", key);
	manager.initialize();

	std::stringbuf buffer;
	std::ostream os(&buffer);
	std::istream is(&buffer);
	os << "ld" << endl;
	os << "quit" << endl;

	std::stringbuf ret_buffer;
	std::ostream r_os(&ret_buffer);
	std::istream r_is(&ret_buffer);
	manager.ui_loop(is, r_os);

	string line;
	ASSERT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "1 : Barry Chopper");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "2 : Gary Baker");
	EXPECT_FALSE(getline(r_is, line));
}

TEST(dossier_manager, DossierImportExportTest) {
	uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	std::filesystem::copy("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\encrypted_dossier_master",
		"C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\encrypted_dossier_scratch");

	dossier_manager manager("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\encrypted_dossier_scratch", key);
	manager.initialize();

	//Write Dossier out to disk 
	std::stringbuf buffer;
	std::ostream os(&buffer);
	std::istream is(&buffer);
	os << "od 1" << endl;
	os << "exd" << endl;
	os << "y" << endl;
	os << "quit" << endl;

	std::stringbuf ret_buffer;
	std::ostream r_os(&ret_buffer);
	std::istream r_is(&ret_buffer);
	manager.ui_loop(is, r_os);

	string line;
	ASSERT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "First Name: Barry");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "Middle Name(s): the");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "Last Name: Chopper");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "DOB: 1989");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "He likes to chop stuff");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "additional info");
	EXPECT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "");
	ASSERT_TRUE(getline(r_is, line));
	ASSERT_EQ(line, "WARNING: This will write dossier unencrypted to disk. Do you wish to continue? Y/N");
	EXPECT_FALSE(getline(r_is, line));

	//Check the dossier made it unencrypted to disk, then make a change
	string dossier_file = "C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\encrypted_dossier_scratch\\tmp_dossier.txt";
	dossier test_dossier = dossier::from_file(dossier_file);
	ASSERT_EQ("Barry", test_dossier.get_first_name());
	ASSERT_EQ("the", test_dossier.get_middle_names());
	ASSERT_EQ("Chopper", test_dossier.get_last_name());
	ASSERT_EQ("1989", test_dossier.get_dob());
	std::stringstream reference_info;
	reference_info << "He likes to chop stuff" << LINE_RETURN;
	reference_info << "additional info" << LINE_RETURN;
	ASSERT_EQ(reference_info.str(), test_dossier.get_information());

	test_dossier.update_middle_names("The");
	test_dossier.to_file(dossier_file, NULL);

	//Now read the dossier back into memory and check that the master is updated
	std::stringbuf buffer2;
	std::ostream os2(&buffer2);
	std::istream is2(&buffer2);
	os2 << "imp" << endl;
	os2 << "y" << endl;
	os2 << "od 1" << endl;
	os2 << "quit" << endl;

	std::stringbuf ret_buffer2;
	std::ostream r_os2(&ret_buffer2);
	std::istream r_is2(&ret_buffer2);
	manager.ui_loop(is2, r_os2);

	ASSERT_TRUE(getline(r_is2, line));
	ASSERT_EQ(line, "WARNING: This will overwrite current dossier. Do you wish to continue? Y/N");
	ASSERT_TRUE(getline(r_is2, line));
	ASSERT_EQ(line, "First Name: Barry");
	EXPECT_TRUE(getline(r_is2, line));
	ASSERT_EQ(line, "Middle Name(s): The");
	EXPECT_TRUE(getline(r_is2, line));
	ASSERT_EQ(line, "Last Name: Chopper");
	EXPECT_TRUE(getline(r_is2, line));
	ASSERT_EQ(line, "DOB: 1989");
	EXPECT_TRUE(getline(r_is2, line));
	ASSERT_EQ(line, "He likes to chop stuff");
	EXPECT_TRUE(getline(r_is2, line));
	ASSERT_EQ(line, "additional info");
	EXPECT_TRUE(getline(r_is2, line));
	ASSERT_EQ(line, "");
	EXPECT_FALSE(getline(r_is2, line));

	ASSERT_FALSE(filesystem::exists(dossier_file));
	std::filesystem::remove("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\encrypted_dossier_scratch\\00001.txt");
	std::filesystem::remove("C:\\Users\\matte\\OneDrive\\Documents\\Software\\ActionNote\\test\\encrypted_dossier_scratch\\00002.txt");
}
