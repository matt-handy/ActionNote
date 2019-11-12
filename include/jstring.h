#ifndef SSTRING_HEADER
#define SSTRING_HEADER

#include <cstring>
#include <vector>

namespace handy{
	namespace action_note
	{

		class jstring {

		public:
			jstring(const char* value) {
				init((char *)value, strlen(value));
			}

			jstring(char* value) {
				init(value, strlen(value));
			}

			jstring(char* value, int len) {
				init(value, len);
			}

			jstring(const jstring& other) {
				init(other.value, other.length);
			}

			virtual ~jstring() {
				delete[] value;
			}

			size_t len();
			const char* c_str();
			bool equals(jstring other);
			bool contains(jstring other);
			bool empty();
			std::vector<jstring> split(jstring delim);

		private:
			char* value;
			size_t length;

			bool contains(jstring other, int& start, int& stop);

			void init(char* value, int len) {
				length = len;
				this->value = new char[len + 1];
				strncpy(this->value, value, len);
				this->value[len] = '\0';
			}

		};
	}
}
#endif
