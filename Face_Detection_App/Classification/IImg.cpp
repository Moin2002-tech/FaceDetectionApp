#include"IImg.hpp"

namespace Classification
{
	IImg::IImg(void) : data(0), w(0), h(0), colorDepth(0)
	{
		rowsize = 0;
		size = 0;
	}

	IImg::IImg(int _w, int _h, int _colordepth = 32) : data(0), w(_w), h(_h), colorDepth(_colordepth) {
		rowsize = (int)colorDepth / 8 * w;
		if (w && h)
			data = new float[rowsize * h];
		size = w * h;
	}


	IImg::~IImg(void)
	{
		delete[] data;
		data = 0;
	}


	void IImg::calIImgPatch(const unsigned char* in_img, bool sqr = false)
	{

		int c, r;
		long z, zb;
		float rowsum;

		if (sqr) {
			rowsum = 0;
			for (c = 0; c < w; c++) {
				rowsum += in_img[c] * in_img[c];
				data[c] = rowsum;
			}
			z = w; zb = 0;
			for (r = 1; r < h; r++) {
				rowsum = 0;
				for (c = 0; c < w; c++) {
					rowsum += in_img[z + c] * in_img[z + c];
					data[z + c] = data[zb + c] + rowsum;
				}
				z += w; zb += w;
			}
		}
		else {
			rowsum = 0;
			for (c = 0; c < w; c++) {
				rowsum += in_img[c];
				data[c] = rowsum;
			}

			z = w; zb = 0;
			for (r = 1; r < h; r++) {
				rowsum = 0;
				for (c = 0; c < w; c++) {
					rowsum += in_img[z + c];
					data[z + c] = data[zb + c] + rowsum;
				}
				z += w; zb += w;
			}
		}
	}

} /* namespace classification */

