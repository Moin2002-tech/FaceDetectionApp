/*
* LinearUtils.cpp
* Created on: 18/1/2025
* Author : Moin Shaikh
*/

#include"LinearUtils.hpp"
#include"linear.h"
#include<stdexcept>
#include"../Classification/EmptyExampleManagement.hpp"
#include"Linear.h"
using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::make_shared;
using std::vector;
using std::invalid_argument;
namespace LinearCPP
{
	linearUtils::linearUtils() : matRows(-1), matCols(-1), matType(CV_32FC1), matDepth(CV_32F), dimensions(0)
	
	{

	}

	linearUtils::~linearUtils() 
	{

	}


	size_t linearUtils::getDimensions() const
	{
		return dimensions;
	}

	std::unique_ptr<struct feature_node[]> linearUtils::createNodes(const cv::Mat& vector, bool bias)
	{

		matRows = vector.rows;
		matCols = vector.cols;
		matType = vector.type();
		matDepth = vector.depth();
		dimensions = vector.total() * vector.channels();
		std::unique_ptr<struct feature_node[]> node = std::make_unique<struct feature_node[]>(dimensions + (bias ? 2 : 1));
		if (matDepth == CV_32F)
		{
			fillNode<float>(node.get(), vector, dimensions);
		}
			
		else if (matDepth == CV_64F)
		{
			fillNode<double>(node.get(), vector, dimensions);
		}
			
		else
			throw invalid_argument("LibLinearUtils: vector has to be of depth CV_32F or CV_64F to create a node of");
		if (bias) 
		{
			node[dimensions].index = dimensions + 1;
			node[dimensions].value = 1;
			node[dimensions + 1].index = -1;
		}
		else 
		{
			node[dimensions].index = -1;
		}

		return std::move(node);
	}
	template <class T>

	void linearUtils::fillNode(struct feature_node* node, const cv::Mat& vector, size_t size) const
	{
		if (!vector.isContinuous())
			throw invalid_argument("LibLinearUtils: vector has to be continuous");
		const T* values = vector.ptr<T>();
		for (int i = 0; i < size; ++i) {
			node[i].index = i + 1;
			node[i].value = values[i];
		}
	}

	double linearUtils::computeSvmOutput(const struct model* model, const struct feature_node* x) const
	{
		double* dec_values = new double[1];
		predict_values(model, x, dec_values);
		double svmOutput = dec_values[0];
		delete[] dec_values;
		return svmOutput;
	}

	vector <cv::Mat> linearUtils::extractSupportVectors(const struct model* model) const
	{
		vector<cv::Mat> supportVectors(1);
		supportVectors.at(0).create(matRows, matCols, matType);
		if (matDepth == CV_32F)
		{
			extractWeightVector<float>(supportVectors[0], model);
		}
		else if (matDepth == CV_64F)
		{
			extractWeightVector<double>(supportVectors[0], model);
		}
			
		return supportVectors;
	}

	template <class T>
	void linearUtils::extractWeightVector(cv::Mat & vector, const struct model * model) const
	{
		T * values = vector.ptr<T>();
		if(model->nr_class == 2 && model->param.solver_type != MCSVM_CS)
		{
			for (int i = 0; i < model->nr_feature; ++i)
			{
				values[i] = static_cast<T>(model->w[i]);
			}
		}
		else
		{
			for (int i = 0; i < model->nr_feature; ++i)
			{
				values[i] = static_cast<T>(model->w[model->nr_class * i]);
			}
		}
	}

	vector<float> linearUtils::extractCoefficients(struct model* model) const
	{
		vector<float> coefficients(1);
		coefficients.at(0) = 1;
		return coefficients;
	}
	double linearUtils::extractBias(struct model* model) const
	{
		if (model->bias <= 0) 
		{
			return 0;
		}
		if(model->nr_class ==  2 && model->param.solver_type != MCSVM_CS)
		{
			return -model->w[model->nr_feature];
		}
		else
			return -model->w[model->nr_class * model->nr_feature];
	}
	void ParameterDeleter::operator()(struct parameter* param) const
	{
		destroy_param(param);
		delete param;
	}
	
	void problemDeleter::operator()(struct problem* problem) const
	{
		delete[] problem->x;
		delete[] problem->y;
		delete problem;
	}

	void modelDeleter::operator()(struct model* model) const
	{
		free_and_destroy_model(&model);
	}

}


