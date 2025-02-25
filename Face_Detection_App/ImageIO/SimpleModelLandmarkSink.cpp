
#include "SimpleModelLandmarkSink.hpp"
#include "Landmark.hpp"
#include "LandmarkCollection.hpp"

#include <fstream>

using boost::filesystem::path;

namespace imageio {

void SimpleModelLandmarkSink::add(const LandmarkCollection& collection, path filename)
{
	//filename.replace_extension(".txt");
	std::ofstream outputFile(filename.string());

	if (!outputFile.is_open()) {
		// TODO log
		throw std::runtime_error("SimpleModelLandmarkSink: Error creating the output file " + filename.string());
	}

	const auto& landmarks = collection.getLandmarks();
	for (const auto& lm : landmarks) {
		outputFile << lm->getName() << " " << lm->getX() << " " << lm->getY() << std::endl;
	}

	outputFile.close();
	return;
}

} /* namespace imageio */
