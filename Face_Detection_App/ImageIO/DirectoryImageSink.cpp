

#include "DirectoryImageSink.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "boost/filesystem.hpp"
#include <sstream>
#include <iostream>
#include <iomanip>

using cv::Mat;
using boost::filesystem::path;
using boost::filesystem::exists;
using boost::filesystem::is_directory;
using boost::filesystem::create_directory;
using std::string;
using std::ostringstream;
using std::setfill;
using std::setw;

namespace imageio {

DirectoryImageSink::DirectoryImageSink(string directory, string ending) :
		directory(directory), ending(ending), index(0) {
	if (this->directory[directory.length() - 1] != '/')
		this->directory += '/';
	path path(this->directory);
	if (!exists(path)) {
		if (!create_directory(path))
			std::cerr << "Could not create directory '" << directory << "'" << std::endl;
	} else if (!is_directory(path))
		std::cerr << "'" << directory << "' is no directory" << std::endl;
}

DirectoryImageSink::~DirectoryImageSink() {}

void DirectoryImageSink::add(const Mat& image) {
	ostringstream filename;
	filename << directory << setfill('0') << setw(5) << index++ << setw(0) << '.' << ending;
	if (!imwrite(filename.str(), image))
		std::cerr << "Could not write image file '" << filename.str() << "'" << std::endl;
}

} /* namespace imageio */
