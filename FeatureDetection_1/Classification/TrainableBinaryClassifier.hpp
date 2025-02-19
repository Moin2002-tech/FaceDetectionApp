#pragma once
/*
* TrainableBinaryClassifier.hpp
* Created on : 14/ 11 / 2024
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_CLASSIFICATION_TRAINABLE_BINARYCLASSIFIER_HPP_
#define FEATUREDETECTION_CLASSIFICATION_TRAINABLE_BINARYCLASSIFIER_HPP_
#include"BinaryClassifier.hpp"
#include"TrainableClassifier.hpp"

namespace Classification {

	/* 
		*Binary classifier that may be re - trained using new examples.Re - training is an incremental procedure
		* that adds new examples and refines the classifier.Nevertheless, it may be possible that previous
		* training examples are forgotten to ensure the classifier stays relatively small and efficient.
	*/
	class TrainableBinaryClassifier : public TrainableClassifier ,public BinaryClassifier
	{
	public:
		virtual ~TrainableBinaryClassifier() {};
	};

}


#endif //FEATUREDETECTION_CLASSIFICATION_TRAINABLE_BINARYCLASSIFIER_HPP_