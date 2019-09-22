

#include <list>
#include <string>

using namespace std;


class FileUtils {
	
	public:
	FileUtils();
	void loadProfile(MotionProfile* mp);
	void saveProfile(MotionProfile mp);
	
	protected:
	
	private:
	void split_line(string& line, string delim, list<std::string>& values);
	
};



