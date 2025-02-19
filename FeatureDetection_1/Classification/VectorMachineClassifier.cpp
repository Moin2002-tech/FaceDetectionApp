/*
* VectorMachineClassifier.cpp
* Created on : 07/ 12 / 2024
* Author : Moin Shaikh
*/

#include"VectorMachineClassifier.hpp"
using std::shared_ptr;

namespace Classification

{
	VectorMachineClassifier::VectorMachineClassifier(std::shared_ptr<kernel> Kernel) :
		threshold(0.0), bias(0.0), Kernel(Kernel) {}

	VectorMachineClassifier::~VectorMachineClassifier() {}
	
}