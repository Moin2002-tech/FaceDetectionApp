
/*
* WvmClassifier.cpp
* Created on : 19/ 12 / 2024
* Author : Moin Shaikh
*/

#include"WvmClassifier.hpp"
#include"IImg.hpp"
#include"../logging/LoggerFactory.hpp"
#include<matio.h>
#include<boost/lexical_cast.hpp>
#include<stdexcept>

using logging::LoggerFactory;
using logging::Logger;
using cv::Mat;
using boost::lexical_cast;
using std::pair;
using std::make_pair;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::invalid_argument;
using std::runtime_error;
using std::string;

namespace Classification
{
	WvmClassifier::WvmClassifier() : VectorMachineClassifier(nullptr)
	{
		linFilters = NULL;
		lin_thresholds = NULL;
		numLinFilters = 0;
		numUsedFilters = 0;
		hkWeights = NULL;

		area = NULL;
		app_rsv_convol = NULL;

		filter_output = NULL;
		u_kernel_eval = NULL;

		limitReliabilityFilter = 0.0f;

		basisParam = 0.0f;


	}

	WvmClassifier::~WvmClassifier()
	{
		if (linFilters != NULL) {
			for (int i = 0; i < numLinFilters; ++i)
				delete[] linFilters[i];
		}
		delete[] linFilters;
		if (hkWeights != NULL)
			for (int i = 0; i < numLinFilters; ++i) delete[] hkWeights[i];
		delete[] hkWeights;
		delete[] lin_thresholds;
		//delete [] hierarchicalThresholds;

		if (area != NULL) {
			for (int i = 0; i < numLinFilters; i++)
				if (area[i] != NULL)
					delete area[i];
			delete[] area;
		}
		if (app_rsv_convol != NULL) delete[] app_rsv_convol;

		if (filter_output != NULL) delete[] filter_output;
		if (u_kernel_eval != NULL) delete[] u_kernel_eval;
	}


	bool WvmClassifier::classify(const Mat& featureVector) const
	{
		return classify(hyperPlaneDistance(featureVector));
	}

	pair<bool, double> WvmClassifier::getConfidence(const Mat& featureVector) const
	{
		return getConfidence(hyperPlaneDistance(featureVector));
	}

	pair<bool, double> WvmClassifier::getConfidence(pair<int, double> levelAndDistance) const {
		if (classify(levelAndDistance))
			return make_pair(true, levelAndDistance.second);
		else
			return make_pair(false, -levelAndDistance.second);
	}

	bool WvmClassifier::classify(pair<int, double> levelAndDistance)const {
		// TODO the following todo was moved here from the end of the getHyperplaneDistance function (was in classify before)
		// TODO: filter statistics, nDropedOutAsNonFace[filter_level]++;
		// We ran till the REAL LAST filter (not just the numUsedFilters one), save the certainty
		int filterLevel = levelAndDistance.first;
		double fout = levelAndDistance.second;
		return filterLevel + 1 == this->numLinFilters && fout >= this->hierarchicalThresholds.at(filterLevel);
	}


	pair<int, double> WvmClassifier::hyperPlaneDistance(const Mat& featureVector) const
	{
		unsigned int featureVectorLength = featureVector.rows * featureVector.cols;
		unsigned char* data = new unsigned char[featureVectorLength];

		// TODO conditions: featureVector.type() == CV_8U, featureVector.isContinuous() == true, featureVectorLength == 400 (in our case)
		const uchar* values = featureVector.ptr<uchar>(0);
		for (int i = 0; i < featureVectorLength; ++i)
		{
			data[i] = values[i];
		}
		// TODO compute integral image outside of this (IntegralImageFilter), assume featureVector to be integral image,
		// work directly with given feature vector, throw away IImg

		// STAAAAAAAAAAAART

		// Check if the patch has already been classified by this detector! If yes, don't do it again.
		// OK. We can't do this here! Because we do not know/save "filter_level". So we don't know when
		// the patch dropped out. Only the fout-value is not sufficient. So we can't know if we should
		// return true or false.
		// Possible solution: Store the filter_level somewhere.

		// So: The fout value is not already computed. Go ahead.

		// patch II of fp already calc'ed?

		IImg* iimg_x = new IImg(this->filter_size_x, this->filter_size_y, 8);
		iimg_x->calIImgPatch(data, false);
		IImg* iimg_xx = new IImg(this->filter_size_x, this->filter_size_y, 8);
		iimg_xx->calIImgPatch(data, true);

		for (int n = 0; n < this->numFiltersPerLevel; n++)
		{
			u_kernel_eval[n] = 0.0f;
		}
		int filter_level = -1;
		float fout = 0.0;
		do
		{
			filter_level++;
			fout = this->linEvalWvmHisteq64(filter_level, (filter_level % this->numFiltersPerLevel), filter_output, u_kernel_eval, iimg_x, iimg_xx);
			//} while (fout >= this->hierarchicalThresholds[filter_level] && filter_level+1 < this->numLinFilters); //280
		} while (fout >= this->hierarchicalThresholds[filter_level] && filter_level + 1 < this->numUsedFilters);
		// fout = final result now!
		delete iimg_x;
		delete iimg_xx;

		// EEEEEEEEEEEEEEND

		delete[] data;
		data = NULL;
		return make_pair(filter_level, fout);
	}


	void WvmClassifier::setNumUsedFilters(int var)
	{
		if (var > this->numLinFilters || var == 0) {
			this->numUsedFilters = this->numLinFilters;
		}
		else {
			this->numUsedFilters = var;
		}
	}


	int WvmClassifier::getNumUsedFilter(void)
	{
		return this->numUsedFilters;
	}

	void WvmClassifier::setLimitReliabilityFilter(float var)
	{
		// TODO note from Peter: couldn't we add this value to the threshold in classify? 
		// Then it wouldn't be necessary to additionally store hierarchicalThresholdsFromFile,
		// as hierarchicalThresholds always remain the same.
		// lines 74 and 123 would then need to be extended by + limitReliabilityFilter (or + threshold after renaming)

		this->limitReliabilityFilter = var;
		if (var != 0.0f) {
			this->hierarchicalThresholds.clear();	// Maybe we could not use clear here and just assign the new value. But then we'd have a problem the first time we call this function.
			// But we could pre-allocate the right size.
			for (unsigned int i = 0; i < this->hierarchicalThresholdsFromFile.size(); ++i) {
				this->hierarchicalThresholds.push_back(this->hierarchicalThresholdsFromFile[i] + limitReliabilityFilter);
			}
		}
		else {
			this->hierarchicalThresholds = this->hierarchicalThresholdsFromFile;
		}

	}
	float WvmClassifier::getLimitReliabilityFilter(void)
	{
		return this->limitReliabilityFilter;
	}

	/*
	 * WVM evaluation with a histogram equalized patch (64 bins) and patch integral image
	*/
	float WvmClassifier::linEvalWvmHisteq64(
		int level, int n,  //n: n-th WSV at this apprlevel
		float* hk_kernel_eval,
		float* u_kernel_eval,
		const IImg* iimg_x/*=NULL*/,
		const IImg* iimg_xx/*=NULL*/) const
	{
		/* iimg_x and iimg_xx are now patch-integral images! */
		float* this_weight = hkWeights[level];
		float res = -lin_thresholds[level];
		double norm = 0.0F;
		int p;
		const int fx = 0;
		const int fy = 0;
		const int lx = filter_size_x - 1;
		const int ly = filter_size_y - 1;

		//###########################################
	// compute evaluation and return it to fout.Pixel(x,y)
	// eval = sum_{i=1,...,level} [ hkWeights[level][i] * exp(-basisParam*(linFilters[i]-img.data)^2) ]
	//      = sum_{i=1,...,level} [ hkWeights[level][i] * exp(-basisParam*norm[i]) ]
	//      = sum_{i=1,...,level} [ hkWeights[level][i] * hk_kernel_eval[i] ]

	//===========================================
	// first, compute this kernel and save it in hk_kernel_eval[level], 
	// because the hk_kernel_eval[0,...,level-1] are the same for each new level
	// hk_kernel_eval[level] = exp( -basisParam * (linFilters[level]-img.data)^2 )
	//                       = exp( -basisParam * norm[level] )

	//rvm_kernel_begin = clock();
	//rvm_norm_begin = clock();


	/* adjust wvm calculation to patch-II */

	//.........................................
	// calculate the norm for that kernel (hk_kernel_eval[level] = exp(-basisParam*(linFilters[level]-img.data)^2))
	//  norm[level] = ||x-z||^2 = (linFilters[level]-img.data)^2   approximated by     
	//  norm[level] = ||x-p||^2 = x*x - 2*x*p + p*p   with  (x: cur. patch img.data, p: appr. RSV linFilters[level])

		double	norm_new = 0.0F, sum_xp = 0.0F, sum_xx = 0.0F, sum_pp = 0.0F;
		float	sumv = 0.0f, sumv0 = 0.0f;
		int		r, v,/*uur,uull,dll,dr,*/ax1, ax2, ay1, ay1w, ay2w;
		PRec	rec;

		// 1st term : x'*x (integral image over x^2)'
			//sxx_begin = clock();
			//norm_new=iimg_xx->ISumV(0,0,0,399,0,0,lx,ly);
			//norm_new=iimg_xx->ISum(fx,fy,lx,ly);
			//norm_new=iimg_xx->data[dr];
			//uur=(fy-1)*20/*img.w*/ + lx; uull=(fy-1)*20/*img.w*/ + fx-1; dll=ly*20/*img.w*/ + fx-1; dr=ly*20/*img.w*/ + lx;
		const int dr = ly * filter_size_x/*img.w*/ + lx;
		/*if (fx>0 && fy>0)  {
			norm_new= iimg_xx->data[dr] - iimg_xx->data[uur] - iimg_xx->data[dll] + iimg_xx->data[uull];
			sumv0=    iimg_x->data[dr]  - iimg_x->data[uur]  - iimg_x->data[dll]  + iimg_x->data[uull];
		} else if (fx>0)   {
			norm_new= iimg_xx->data[dr] - iimg_xx->data[dll]; sumv0= iimg_x->data[dr] - iimg_x->data[dll];
		} else if (fy>0)	{
			norm_new= iimg_xx->data[dr] - iimg_xx->data[uur]; sumv0= iimg_x->data[dr] - iimg_x->data[uur];
		} else {*///if (fx==0 && fy==0)
		norm_new = iimg_xx->data[dr]; sumv0 = iimg_x->data[dr];
		//}
		sum_xx = norm_new;

		//Profiler.sxx += (double)(clock()-sxx_begin);

		//2nd term : 2x'*p 
		//    (sum 'sum_xp' over the sums 'sumv' for each gray level of the appr. RSV 
		//     over all rectangles of that gray level which are calculated by integral input image
		//     multiplied the sum_v by the gray value)
		//
		//		dh. sum_xp= sumv0*val0 - sum_{v=1}^cntval ( sum_{r=0}^cntrec_v(iimg_x->ISum(rec_{r,v})) * val_v )
		// also we can simplify 
		//		2x'*p = 2x'* ( sum_{l=0}^{lev-1}(res_{l,n}) + res_{lev,n} ) for the n-th SV at apprlevel lev
		//		      = 2x'*u_{lev-1,n} + 2x'*res_{lev,n} 
		//		      = u_kernel_eval[n] + sum_xp, with p=res_{lev,n}
		//                                        and u_kernel_eval[n]_{lev+1}=u_kernel_eval[n] + sum_xp

		//sxp_begin = clock();
		//sumv0=iimg_x->ISum(fx,fy,lx,ly);
		//sumv0=iimg_x->ISumV(0,0,0,399,0,0,lx,ly);
		//sumv0=iimg_x->data[dr];
		for (v = 1; v < area[level]->cntval; v++) {
			sumv = 0;
			for (r = 0; r < area[level]->cntrec[v]; r++) {
				rec = &area[level]->rec[v][r];
				//sxp_iimg_begin = clock();
				//if (rec->x1==rec->x2 && rec->y1==rec->y2) 
				//	sumv+=img.data[(fy+rec->y1)*img.w+(fx+rec->x1)];
				//
				////TODO: fasten up for lines 
				////else if (rec->x1==rec->x2 || rec->y1==rec->y2)
				//
				//else //if (rec->x1!=rec->x2 && rec->y1!=rec->y2)
				//	sumv+=iimg_x->ISum(fx+rec->x1,fy+rec->y1,fx+rec->x2,fy+rec->y2);
				//	sumv+=iimg_x->ISumV(rec->uull,rec->uur,rec->dll,rec->dr,rec->x1,rec->y1,rec->x2,rec->y2);
				//	sumv+=   iimg_x->data[rec->dr]                   - ((rec->y1>0)? iimg_x->data[rec->uur]:0) 
				//		   - ((rec->x1>0)? iimg_x->data[rec->dll]:0) + ((rec->x1>0 && rec->y1>0)? iimg_x->data[rec->uull]:0);
				ax1 = fx + rec->x1 - 1; ax2 = fx + rec->x2; ay1 = fy + rec->y1;
				ay1w = (ay1 - 1) * filter_size_x/*img.w*/; ay2w = (fy + rec->y2) * filter_size_x/*img.w*/;
				if (ax1 + 1 > 0 && ay1 > 0)
					sumv += iimg_x->data[ay2w + ax2] - iimg_x->data[ay1w + ax2]
					- iimg_x->data[ay2w + ax1] + iimg_x->data[ay1w + ax1];
				else if (ax1 + 1 > 0)
					sumv += iimg_x->data[ay2w + ax2] - iimg_x->data[ay2w + ax1];
				else if (ay1 > 0)
					sumv += iimg_x->data[ay2w + ax2] - iimg_x->data[ay1w + ax2];
				else //if (ax1==0 && ay1==0)
					sumv += iimg_x->data[ay2w + ax2];

				//Profiler.sxp_iimg += (double)(clock()-sxp_iimg_begin);
			}
			//sxp_mval_begin = clock();
			sumv0 -= sumv;
			sum_xp += sumv * area[level]->val[v];
			//Profiler.sxp_mval += (double)(clock()-sxp_mval_begin);
		}
		sum_xp += sumv0 * area[level]->val[0];
		sum_xp += u_kernel_eval[n];
		u_kernel_eval[n] = sum_xp;  //update u_kernel_eval[n]

		norm_new -= 2 * sum_xp;
		//Profiler.sxp += (double)(clock()-sxp_begin);

		//3rd term: p'*p (convolution of the appr. RSV - constrant calculated at the training)
		//spp_begin = clock();
		sum_pp = app_rsv_convol[level]; // Patrik: Ueberfluessig?
		norm_new += app_rsv_convol[level];
		//Profiler.spp += (double)(clock()-spp_begin);

		norm = norm_new;

		//Profiler.rvm_norm += (double)(clock()-rvm_norm_begin);

		//.........................................
		// calculate  now this kernel and save it in hk_kernel_eval[level], 
		// hk_kernel_eval[level] = exp(-basisParam*(linFilters[level]-img.data)^2) ]

		hk_kernel_eval[level] = (float)(exp(-basisParam * norm)); //save it, because they 0...level-1 the same for each new level 

		//===========================================
		// second, sum over all the kernels to get the output
		// eval = sum_{i=1,...,level} [ hkWeights[level][i] * exp(-basisParam*(linFilters[i]-img.data)^2) ]
		//      = sum_{i=1,...,level} [ hkWeights[level][i] * hk_kernel_eval[i] ]

		for (p = 0; p <= level; ++p) //sum k=0...level = b_level,k * Kernel_k
			res += this_weight[p] * hk_kernel_eval[p];

		//Profiler.rvm_kernel += (double)(clock()-rvm_kernel_begin);

		return res;

	}

#include <matio.h>

	shared_ptr<WvmClassifier> WvmClassifier::loadFromMATLAB(const std::string& classifierFilename, const std::string& thresholdsFilename)
	{
		Logger logger = LoggerFactory::Instance()->getLogger("Classification");


		logger.info("Loading WVM classifier from matlab file: " + classifierFilename);

		shared_ptr<WvmClassifier> wvm = make_shared<WvmClassifier>();

		//Number filters to use
		wvm->numUsedFilters = 280;    // Todo make dynamic (from script)

		//Grenze der Zuverlaesigkeit ab der Gesichter aufgenommen werden (Diffwert fr W-RSV's-Schwellen)
		// zB. +0.1 => weniger patches drueber(mehr rejected, langsamer),    dh. mehr fn(FRR), weniger fp(FAR)  
		// zB. -0.1 => mehr patches drueber(mehr nicht rejected, schneller), dh. weniger fn(FRR), mehr fp(FAR)
		wvm->limitReliabilityFilter = 0.0f;    // FD.limit_reliability_filter (for WVM) (without _filter, it's for the SVM)

		mat_t* pmatfile;
		matvar_t* pmxarray; // =mat
		double* matdata;

		// Open the MAT file
		pmatfile = Mat_Open(classifierFilename.c_str(), MAT_ACC_RDONLY);
		if (!pmatfile) {
			throw invalid_argument("WvmClassifier: Could not open the provided classifier filename: " + classifierFilename);
		}

		// Read num_hk variable
		pmxarray = Mat_VarRead(pmatfile, "num_hk");
		if (!pmxarray) {
			throw runtime_error("WvmClassifier: There is no num_hk in the classifier file.");
		}
		matdata = (double*)pmxarray->data;
		int nfilter = (int)matdata[0];
		Mat_VarFree(pmxarray);
		logger.debug("Found " + lexical_cast<std::string>(nfilter) + " WVM filters.");

		// Check for 'wrvm' structure
		pmxarray = Mat_VarRead(pmatfile, "wrvm");
		if (pmxarray) {
			logger.error("Found a structure 'wrvm' and trying to read the " + lexical_cast<std::string>(nfilter) + " non-linear filters support_hk* and weight_hk* at once. However, this is not implemented yet.");
			throw runtime_error("WvmClassifier: Reading all wvm filters at once using the structure 'wrvm' is not (yet) supported.");
		}
		else {
			logger.debug("No structure 'wrvm' found, thus reading the " + lexical_cast<string>(nfilter) + " non-linear filters support_hk* and weight_hk* sequentially (slower).");
			char str[100];
			sprintf_s(str, "support_hk%d", 1);
			pmxarray = Mat_VarRead(pmatfile, str);
			if (!pmxarray) {
				throw runtime_error("WvmClassifier: Unable to find the matrix 'support_hk1' in the classifier file.");
			}
			const size_t* dim = pmxarray->dims;
			int h = (int)dim[0];
			int w = (int)dim[1];
			wvm->filter_size_x = w;
			wvm->filter_size_y = h;

			wvm->numLinFilters = nfilter;
			wvm->linFilters = new float* [wvm->numLinFilters];
			for (int i = 0; i < wvm->numLinFilters; ++i) {
				wvm->linFilters[i] = new float[w * h];
			}

			wvm->hkWeights = new float* [wvm->numLinFilters];
			for (int i = 0; i < wvm->numLinFilters; ++i) {
				wvm->hkWeights[i] = new float[wvm->numLinFilters];
			}

			Mat_VarFree(pmxarray);

			for (int i = 0; i < wvm->numLinFilters; i++) {
				sprintf_s(str, "support_hk%d", i + 1);
				pmxarray = Mat_VarRead(pmatfile, str);
				if (!pmxarray) {
					throw runtime_error("WvmClassifier: Unable to find the matrix 'support_hk" + lexical_cast<string>(i + 1) + "' in the classifier file.");
				}
				matdata = (double*)pmxarray->data;
				int k = 0;
				for (int x = 0; x < wvm->filter_size_x; ++x) {
					for (int y = 0; y < wvm->filter_size_y; ++y) {
						wvm->linFilters[i][y * wvm->filter_size_x + x] = 255.0f * (float)matdata[k++];
					}
				}
				Mat_VarFree(pmxarray);

				sprintf_s(str, "weight_hk%d", i + 1);
				pmxarray = Mat_VarRead(pmatfile, str);
				if (pmxarray) {
					const size_t* dim = pmxarray->dims;
					if (dim[1] != i + 1 && dim[0] != i + 1) {
						throw runtime_error("WvmClassifier: The matrix " + lexical_cast<string>(str) + " in the classifier file should have a dimensions 1x" + lexical_cast<string>(i + 1) + " or " + lexical_cast<string>(i + 1) + "x1");
					}
					matdata = (double*)pmxarray->data;
					for (int j = 0; j <= i; ++j) {
						wvm->hkWeights[i][j] = (float)matdata[j];
					}
					Mat_VarFree(pmxarray);
				}
			}
			logger.debug("Vectors and weights successfully read.");
		}

		// Reading other classifier parameters
		pmxarray = Mat_VarRead(pmatfile, "param_nonlin1_rvm");
		if (pmxarray) {
			matdata = (double*)pmxarray->data;
			wvm->bias = (float)matdata[0];
			int nonLinType = (int)matdata[1];
			wvm->basisParam = (float)(matdata[2] / 65025.0); // because the training images gray level values were divided by 255
			int polyPower = (int)matdata[3];
			float divisor = (float)matdata[4];
			Mat_VarFree(pmxarray);
		}

		// Close the MAT file
		Mat_Close(pmatfile);

		// Read thresholds from another file
		logger.info("Loading WVM thresholds from matlab file: " + thresholdsFilename);
		pmatfile = Mat_Open(thresholdsFilename.c_str(), MAT_ACC_RDONLY);
		if (pmatfile == 0) {
			throw runtime_error("WvmClassifier: Unable to open the thresholds file (wrong format?):" + thresholdsFilename);
		}
		pmxarray = Mat_VarRead(pmatfile, "hierar_thresh");
		if (pmxarray == 0) {
			throw runtime_error("WvmClassifier: Unable to find the matrix hierar_thresh in the thresholds file.");
		}
		else {
			double* matdata = (double*)pmxarray->data;
			const size_t* dim = pmxarray->dims;
			for (int o = 0; o < (int)dim[1]; ++o) {
				// Process the thresholds here
			}
			Mat_VarFree(pmxarray);
		}

		Mat_Close(pmatfile);

		logger.info("WVM successfully read.");

		return wvm;
	}
	WvmClassifier::Area::Area(void)
	{
		cntval = 0;
		val = NULL;
		rec = NULL;
		cntrec = NULL;
		cntallrec = 0;
	}

	WvmClassifier::Area::Area(int cv, int* cr) : cntval(cv)
	{
		int r, v;

		val = new double[cntval];
		cntrec = new int[cntval];
		cntallrec = 0;
		for (v = 0; v < cntval; v++) {
			cntrec[v] = cr[v];
			cntallrec += cr[v];
		}
		rec = new TRec * [cntval];
		for (v = 0; v < cntval; v++) {
			rec[v] = new TRec[cntrec[v]];
			for (r = 0; r < cntrec[v]; r++) { rec[v][r].x1 = rec[v][r].y1 = rec[v][r].x2 = rec[v][r].y2 = 0; }
		}
	}

	WvmClassifier::Area::~Area(void)
	{
		if (rec != NULL) for (int v = 0; v < cntval; v++) delete[] rec[v];
		if (rec != NULL) delete[] rec;
		if (cntrec != NULL) delete[] cntrec;
		if (val != NULL) delete[] val;
	}

	void WvmClassifier::Area::dump(char* name) {
		int r, v;

		Logger logger = LoggerFactory::Instance()->getLogger("Classification");
		// NOTE: This code with the logger is not tested because we haven't used Area::dump for a long time. But it should be ok.
		logger.trace("area" + lexical_cast<string>(name) + ": cntval:" + lexical_cast<string>(cntval) + ", cntallrec:" + lexical_cast<string>(cntallrec) + ", val:");

		for (v = 0; v < cntval; v++)
			logger.trace(" " + lexical_cast<string>(val[v]));
		logger.trace("");

		for (v = 0; v < cntval; v++) {
			for (r = 0; r < cntrec[v]; r++) {
				//printf("r[%d][%d]:(%d,%d,%d,%d) ",v,r,rec[v][r].x1,rec[v][r].y1,rec[v][r].x2,rec[v][r].y2);
				logger.trace("r[" + lexical_cast<string>(v) + "][" + lexical_cast<string>(r) + "]:(" + lexical_cast<string>(rec[v][r].x1) + "," + lexical_cast<string>(rec[v][r].y1) + "," + lexical_cast<string>(rec[v][r].x2) + "," + lexical_cast<string>(rec[v][r].y2) + ") ");
				if ((r % 5) == 4)
					logger.trace("");
			}
			logger.trace("");
		}

	}
}
