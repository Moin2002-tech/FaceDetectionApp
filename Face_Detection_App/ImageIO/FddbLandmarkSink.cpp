

#include "FddbLandmarkSink.hpp"
#include "Landmark.hpp"
#include "LandmarkCollection.hpp"

#include "boost/filesystem/path.hpp"
#include "opencv2/core/core.hpp"

using cv::Rect_;
using std::string;

namespace imageio {

FddbLandmarkSink::FddbLandmarkSink(const string& annotatedList) :
		annotatedList(annotatedList), output() {
	output.precision(8);
}

FddbLandmarkSink::~FddbLandmarkSink() {
}


bool FddbLandmarkSink::isOpen() {
	return output.is_open();
}

void FddbLandmarkSink::open(const string& filename) {
	output.open(filename);

}

void FddbLandmarkSink::close() {
	output.close();
}

void FddbLandmarkSink::add(const string imageFilename, const std::vector<RectLandmark> faceLandmarks, const std::vector<float> detectionScores) {
	// TODO: Write only to a vector/map/tuple <fn, vec<lms>, vec<scores>>, then sort by fn according to the .txt, then write at close()/d'tor
	
	size_t posOf2002 = imageFilename.find("2002/"); // NOTE: If this sequence occurs in the path somewhere else (e.g. twice), we have a problem. Same below.
	size_t posToCut;
	if (posOf2002 == std::string::npos) {
		size_t posOf2003 = imageFilename.find("2003/");
		if (posOf2003 == std::string::npos) {
			// error, end
		} else {
			// no error, '2003/' found
			posToCut = posOf2003;
		}
	} else {
		// no error, '2002/' found
		posToCut = posOf2002;
	}

	string imageIdentifier = imageFilename.substr(posToCut);
	boost::filesystem::path imageIdentifierPath(imageIdentifier);
	imageIdentifierPath.replace_extension();

	output << imageIdentifierPath.string() << std::endl;
	output << faceLandmarks.size() << std::endl;
	// check faceLandmarks.size() == detectionScores.size()
	for (int i = 0; i < faceLandmarks.size(); ++i) {
		// "tl_x, tl_y, w, h, score", score \in (-inf, inf)
		output << faceLandmarks[i].getRect().x << " " << faceLandmarks[i].getRect().y << " " << faceLandmarks[i].getWidth() << " " << faceLandmarks[i].getHeight() << " " << detectionScores[i] << std::endl;
	}

}

} /* namespace imageio */
