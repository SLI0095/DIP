// DIP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void cv1()
{
	//Gamma correction

	cv::Mat src_8uc3_img = cv::imread("images/lena.png", CV_LOAD_IMAGE_COLOR); // load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)
	float gamma_value = 2.0;
	cv::Mat gamma_32fc3_img;
	src_8uc3_img.convertTo(gamma_32fc3_img, CV_32FC3, 1.0 / 255.0);
	for (int y = 0; y < gamma_32fc3_img.rows; y++) {
		for (int x = 0; x < gamma_32fc3_img.cols; x++) {
			cv::Vec3f gamma_vec3 = gamma_32fc3_img.at<cv::Vec3f>(y, x);
			for (int i = 0; i < 3; i++)
			{
				gamma_vec3[i] = pow(gamma_vec3[i], gamma_value);
			}
			gamma_32fc3_img.at<cv::Vec3f>(y, x) = gamma_vec3;
		}
	}

	cv::imshow("Lena gamma", gamma_32fc3_img);
}

void cv2()
{

	// Convolution


	cv::Mat src_8uc1_img = cv::imread("images/lena.png", CV_LOAD_IMAGE_GRAYSCALE);

	cv::imshow("ORIGINAL IMAGE", src_8uc1_img);

	cv::Mat convolution_img = cv::Mat(src_8uc1_img.rows, src_8uc1_img.cols, CV_8UC1);
	cv::copyMakeBorder(src_8uc1_img, convolution_img, 1, 1, 1, 1, CV_HAL_BORDER_REPLICATE);

	cv::Mat box_blur = cv::Mat(3, 3, CV_8UC1, cv::Scalar(1, 1, 1));
	float box_blur_value = 1.0f / 9.0f;

	for (int y = 1; y < src_8uc1_img.rows - 1; y++) {
		for (int x = 1; x < src_8uc1_img.cols - 1; x++) {
			float sum = 0;
			for (int i = 0; i < box_blur.cols; i++) {
				for (int j = 0; j < box_blur.rows; j++)
				{
					sum += (((float)src_8uc1_img.at<uchar>(y + 1 - i, x + 1 - j)) / 255) * (float)box_blur.at<uchar>(i, j) * box_blur_value;
				}
			}
			convolution_img.at<uchar>(y, x) = sum * 255;
		}
	}

	cv::imshow("BOX BLUR", convolution_img);


	cv::copyMakeBorder(src_8uc1_img, convolution_img, 1, 1, 1, 1, CV_HAL_BORDER_REPLICATE);
	cv::Mat gaussian_blur_3x3 = (cv::Mat_<uchar>(3,3) << 1, 2, 1, 2, 4, 2, 1, 2, 1);
	float gaussian_blur_3x3_value = 1.0f / 16.0f;
	

	for (int y = 1; y < src_8uc1_img.rows - 1; y++) {
		for (int x = 1; x < src_8uc1_img.cols - 1; x++) {
			float sum = 0;
			for (int i = 0; i < gaussian_blur_3x3.cols; i++) {
				for (int j = 0; j < gaussian_blur_3x3.rows; j++)
				{
					sum += (((float)src_8uc1_img.at<uchar>(y + 1 - i, x + 1 - j)) / 255) * (float)gaussian_blur_3x3.at<uchar>(i, j) * gaussian_blur_3x3_value;
				}
			}
			convolution_img.at<uchar>(y, x) = sum * 255;
		}
	}

	cv::imshow("GAUSIAN BLUR 3x3", convolution_img);

	cv::copyMakeBorder(src_8uc1_img, convolution_img, 2,2,2,2, CV_HAL_BORDER_REPLICATE);
	cv::Mat gaussian_blur_5x5 = (cv::Mat_<uchar>(5, 5) << 1,4,6,4,1,4,16,24,16,4,6,24,36,24,6,4,16,24,16,4,1,4,6,4,1);
	float gaussian_blur_5x5_value = 1.0f / 256.0f;

	for (int y = 2; y < src_8uc1_img.rows - 2; y++) {
		for (int x = 2; x < src_8uc1_img.cols - 2; x++) {
			float sum = 0;
			for (int i = 0; i < gaussian_blur_5x5.cols; i++) {
				for (int j = 0; j < gaussian_blur_5x5.rows; j++)
				{
					sum += (((float)src_8uc1_img.at<uchar>(y + 2 - i, x + 2 - j)) / 255) * (float)gaussian_blur_5x5.at<uchar>(i, j) * gaussian_blur_5x5_value;
				}
			}
			convolution_img.at<uchar>(y, x) = sum * 255;
		}
	}

	cv::imshow("GAUSIAN BLUR 5x5", convolution_img);
}

double function_g(double delta_i, double sigma)
{
	return exp(-((delta_i * delta_i) / (sigma * sigma)));
}

void cv3(int time, double lambda, double sigma)
{
	//Anisotropic filtration

	cv::Mat src = cv::imread("images/valve.png", CV_LOAD_IMAGE_GRAYSCALE);
	
	cv::Mat double_graycsale_original;
	src.convertTo(double_graycsale_original, CV_64FC1, 1.0 / 255.0);

	cv::Mat anisotropic_filtration_img = cv::Mat(double_graycsale_original.rows, double_graycsale_original.cols, CV_64FC1);
	cv::copyMakeBorder(double_graycsale_original, anisotropic_filtration_img, 1, 1, 1, 1, CV_HAL_BORDER_REPLICATE);

	cv::imshow("original image", double_graycsale_original);

	for (int t = 0; t < time; t++)
	{
		for (int y = 1; y < double_graycsale_original.rows - 1; y++)
		{
			for (int x = 1; x < double_graycsale_original.cols - 1; x++)
			{
				double i_n = double_graycsale_original.at<double>(y, x - 1);
				double i_s = double_graycsale_original.at<double>(y, x + 1);
				double i_w = double_graycsale_original.at<double>(y - 1, x);
				double i_e = double_graycsale_original.at<double>(y + 1, x);

				double i = double_graycsale_original.at<double>(y, x);
				double c_n = function_g(i_n - i, sigma);
				double c_s = function_g(i_s - i, sigma);
				double c_w = function_g(i_w - i, sigma);
				double c_e = function_g(i_e - i, sigma);

				anisotropic_filtration_img.at<double>(y, x) = double_graycsale_original.at<double>(y, x) * 
					(1 - lambda * (c_n + c_s + c_e + c_w)) + 
					lambda * (c_n * i_n + c_s * i_s + c_e * i_e + c_w * i_w);
			}
		}

		double_graycsale_original = anisotropic_filtration_img;
	}

	cv::imshow("anisotropic filtration", anisotropic_filtration_img);
}

cv::Mat dft(cv::Mat src)
{
	// Discrete Fourier Transform

	cv::Mat src_double_grayscale;
	src.convertTo(src_double_grayscale, CV_64FC1, 1.0 / 255.0);

	int M = src.rows;
	int N = src.cols;
	double constant = 1 / sqrt(M * N);

	src_double_grayscale.convertTo(src_double_grayscale, CV_64FC1, constant);

	cv::Mat DFT(M, N, CV_64FC2);

	for (int k = 0; k < M; k++)
	{
		for (int l = 0; l < N; l++)
		{
			double real_sum = 0.0;
			double complex_sum = 0.0;

			for (int m = 0; m < M; m++)
			{
				for (int n = 0; n < N; n++)
				{

					double x = -2.0 * CV_PI * ((((double)m * (double)k) / (double)M) + (((double)n * (double)l) / (double)N));
					double img_value = src_double_grayscale.at<double>(m, n);
					real_sum += (img_value * cos(x));
					complex_sum += (img_value * sin(x));
				}
			}
			DFT.at<cv::Vec2d>(k, l) = cv::Vec2d(real_sum, complex_sum);
		}
	}

	return DFT;
}

void cv4()
{
	cv::Mat src = cv::imread("images/lena64.png", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat src_double_grayscale;
	src.convertTo(src_double_grayscale, CV_64FC1, 1.0 / 255.0);
	cv::imshow("original image", src);

	int M = src.rows;
	int N = src.cols;
	double constant = 1 / sqrt(M * N);

	src_double_grayscale.convertTo(src_double_grayscale, CV_64FC1, constant);

	cv::Mat DFT(M, N, CV_64FC2);

	for (int k = 0; k < M; k++)
	{
		for (int l = 0; l < N; l++)
		{
			double real_sum = 0.0;
			double complex_sum = 0.0;

			for (int m = 0; m < M; m++)
			{
				for (int n = 0; n < N; n++)
				{

					double x = -2.0 * CV_PI * ((((double)m * (double)k) / (double)M) + (((double)n * (double)l) / (double)N));
					double img_value = src_double_grayscale.at<double>(m, n);
					real_sum += (img_value * cos(x));
					complex_sum += (img_value * sin(x));
				}
			}
			DFT.at<cv::Vec2d>(k, l) = cv::Vec2d(real_sum, complex_sum);
		}
	}

	// visualisation
	cv::Mat powerImg(DFT.rows, DFT.cols, CV_64FC1);
	cv::Mat phaseImg;
	powerImg.copyTo(phaseImg);

	for (int k = 0; k < M; k++)
	{
		for (int l = 0; l < N; l++)
		{
			cv::Vec2d coef = DFT.at<cv::Vec2d>(k, l);
			double real = coef[0];
			double comp = coef[1];

			double spectrum = sqrt(real * real + comp * comp);
			double power = spectrum * spectrum;
			double phase = atan(comp / real);
			double lg = log2(power);
			phaseImg.at<double>(k, l) = phase;
			powerImg.at<double>(k, l) = lg;
		}
	}

	double min, max;
	cv::minMaxLoc(powerImg, &min, &max);
	double scale = max - min;

	for (int k = 0; k < M / 2; k++)
	{
		for (int l = 0; l < N; l++)
		{
			int x = (l < N / 2) ? (l + N / 2) : (l - N / 2);
			int y = k + M / 2;
			std::swap(powerImg.at<double>(k, l), powerImg.at<double>(y, x));
			powerImg.at<double>(k, l) = (powerImg.at<double>(k, l) - min) / scale;
			powerImg.at<double>(y, x) = (powerImg.at<double>(y, x) - min) / scale;
		}
	}

	cv::imshow("Phase", phaseImg);
	cv::imshow("Power", powerImg);
}

cv::Mat idft(cv::Mat DFT)
{
	cv::Mat IDFT(DFT.rows, DFT.cols, CV_64FC1);

	int M = DFT.rows;
	int N = DFT.cols;
	double sqr = 1 / sqrt(M * N);

	for (int m = 0; m < M; m++)
	{
		for (int n = 0; n < N; n++)
		{
			double real_sum = 0.0;
			double complex_sum = 0.0;

			for (int k = 0; k < M; k++)
			{
				for (int l = 0; l < N; l++)
				{
					cv::Vec2d coefficients = DFT.at<cv::Vec2d>(k, l);

					double x = 2.0 * CV_PI * ((((double)m * (double)k) / (double)M) + (((double)n * (double)l) / (double)N));
					real_sum += coefficients[0] * cos(x);
					complex_sum += -1 * coefficients[1] * sin(x);
				}
			}

			IDFT.at<double>(m, n) = sqr * (real_sum + complex_sum);
		}
	}

	return IDFT;
}

void cv5()
{
	cv::Mat src = cv::imread("images/lena64.png", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat DFT = dft(src);

	cv::Mat reversed = idft(DFT);

	cv::imshow("result of IDFT", reversed);

}


void cv6(int rad)
{
	//filtration
	cv::Mat noise = cv::imread("images/lena64_noise.png", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat bars_src = cv::imread("images/lena64_bars.png", CV_LOAD_IMAGE_GRAYSCALE);

	cv::Mat noise_fouriere = dft(noise);
	cv::Mat bars_fouriere = dft(bars_src);


	int M = noise_fouriere.rows;
	int N = noise_fouriere.cols;

	//switching quadrants
	for (int k = 0; k < M / 2; k++)
	{
		for (int l = 0; l < N; l++)
		{
			int x = (l < N / 2) ? (l + N / 2) : (l - N / 2);
			int y = k + M / 2;
			std::swap(noise_fouriere.at<double>(k, l), noise_fouriere.at<double>(y, x));
			std::swap(bars_fouriere.at<double>(k, l), bars_fouriere.at<double>(y, x));
		}
	}




	cv::Mat mask = cv::Mat(M, N, CV_64FC1);
	cv::Mat bars_mask = cv::Mat(M, N, CV_64FC1);
	cv::Mat high = cv::Mat(M, N, CV_64FC2);
	cv::Mat low = cv::Mat(M, N, CV_64FC2);
	cv::Mat bars = cv::Mat(M, N, CV_64FC2);

	int lineWidth = 3;
	int radius = 2;

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (sqrt(pow(i - M / 2, 2) + pow(j - N / 2, 2)) < rad) {
				mask.at<double>(i, j) = 1;
			}
			else {
				mask.at<double>(i, j) = 0;
			}
		}
	}

	cv::rectangle(bars_mask, cv::Rect(0, 0, bars_mask.cols, bars_mask.rows), cv::Scalar(255), -1);
	cv::rectangle(bars_mask, cv::Rect(0, (bars_mask.rows / 2) - (lineWidth / 2), bars_mask.cols, lineWidth), cv::Scalar(0), -1);
	cv::circle(bars_mask, cv::Point(bars_mask.cols / 2, bars_mask.rows / 2), radius, cv::Scalar(255), -1);

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < M; j++)
		{
			bars.at<cv::Vec2d>(i, j)[0] = bars_mask.at<double>(i, j) * bars_fouriere.at<cv::Vec2d>(i, j)[0];
			bars.at<cv::Vec2d>(i, j)[1] = bars_mask.at<double>(i, j) * bars_fouriere.at<cv::Vec2d>(i, j)[1];
		}
	}

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < M; j++)
		{
			high.at<cv::Vec2d>(i, j)[0] = mask.at<double>(i, j) * noise_fouriere.at<cv::Vec2d>(i, j)[0];
			high.at<cv::Vec2d>(i, j)[1] = mask.at<double>(i, j) * noise_fouriere.at<cv::Vec2d>(i, j)[1];
			
			if (mask.at<double>(i, j) == 1)
			{
				low.at<cv::Vec2d>(i, j)[0] = 0;
				low.at<cv::Vec2d>(i, j)[1] = 0;
			}
			else
			{
				low.at<cv::Vec2d>(i, j)[0] = noise_fouriere.at<cv::Vec2d>(i, j)[0];
				low.at<cv::Vec2d>(i, j)[1] = noise_fouriere.at<cv::Vec2d>(i, j)[1];
			}
		}
	}

	for (int k = 0; k < M / 2; k++)
	{
		for (int l = 0; l < N; l++)
		{
			int x = (l < N / 2) ? (l + N / 2) : (l - N / 2);
			int y = k + M / 2;
			std::swap(high.at<double>(k, l), high.at<double>(y, x));
			
			std::swap(low.at<double>(k, l), low.at<double>(y, x));
			
			std::swap(bars.at<double>(k, l), bars.at<double>(y, x));
		}
	}

	cv::namedWindow("HIGH", CV_WINDOW_NORMAL);
	cv::imshow("HIGH", idft(high));
	cv::namedWindow("LOW", CV_WINDOW_NORMAL);
	cv::imshow("LOW", idft(low));
	//cv::namedWindow("BARS", CV_WINDOW_NORMAL);
	//cv::imshow("BARS", idft(bars));
	cv::namedWindow("ORIGINAL", CV_WINDOW_NORMAL);
	cv::imshow("ORIGINAL", noise);

}

struct RLDUserData {
	cv::Mat& src_8uc3_img;
	cv::Mat& undistorted_8uc3_img;
	int K1;
	int K2;

	RLDUserData(const int K1, const int K2, cv::Mat& src_8uc3_img, cv::Mat& undistorted_8uc3_img) : K1(K1), K2(K2), src_8uc3_img(src_8uc3_img), undistorted_8uc3_img(undistorted_8uc3_img) {

	}
};


void geom_dist(cv::Mat& src_8uc3_img, cv::Mat& dst_8uc3_img, bool bili, double K1 = 1.0, double K2 = 1.0)
{

	int width = src_8uc3_img.rows;
	int height = src_8uc3_img.cols;

	float Cu = float(width) / 2.0f;
	float Cv = float(height) / 2.0f;
	float R = sqrt(pow(Cu, 2) + pow(Cv, 2));

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			float x_strip = (x - Cu) / R;
			float y_strip = (y - Cv) / R;
			float r2 = pow(x_strip, 2) + pow(y_strip, 2);

			float x_d = (x - Cu) * (1 / (1 + K1 * r2 + K2 * pow(r2, 2))) + Cu;
			float y_d = (y - Cv) * (1 / (1 + K1 * r2 + K2 * pow(r2, 2))) + Cv;

			dst_8uc3_img.at<cv::Vec3b>(x, y) = src_8uc3_img.at<cv::Vec3b>(x_d, y_d);
		}
	}
}



void apply_rld(int id, void* user_data)
{
	RLDUserData* rld_user_data = (RLDUserData*)user_data;

	geom_dist(rld_user_data->src_8uc3_img, rld_user_data->undistorted_8uc3_img, !false, rld_user_data->K1 / 100.0, rld_user_data->K2 / 100.0);
	cv::imshow("Geom Dist", rld_user_data->undistorted_8uc3_img);
}

int cv7()
{
	//geometry distortion

	cv::Mat src_8uc3_img, geom_8uc3_img;
	RLDUserData rld_user_data(3.0, 1.0, src_8uc3_img, geom_8uc3_img);

	src_8uc3_img = cv::imread("images/distorted_window.jpg", cv::IMREAD_COLOR);
	if (src_8uc3_img.empty())
	{
		printf("Unable to load image!\n");
		exit(-1);
	}

	cv::namedWindow("Original Image");
	cv::imshow("Original Image", src_8uc3_img);

	geom_8uc3_img = src_8uc3_img.clone();
	apply_rld(0, (void*)&rld_user_data);

	cv::namedWindow("Geom Dist");
	cv::imshow("Geom Dist", geom_8uc3_img);

	cv::createTrackbar("K1", "Geom Dist", &rld_user_data.K1, 100, apply_rld, &rld_user_data);
	cv::createTrackbar("K2", "Geom Dist", &rld_user_data.K2, 100, apply_rld, &rld_user_data);

	cv::waitKey(0);

	return 0;
}

void cv8()
{
	//histogram
	cv::Mat src = cv::imread("images/uneq.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	int histogram[256] = { 0 };

	int width = src.rows;
	int height = src.cols;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int k = src.at<uchar>(i, j);
			histogram[k]++;
		}
	}

	/*int max = 0;
	for (int i = 0; i < 256; i++)
	{
		if (histogram[i] >= max)
		{
			max = histogram[i];
		}
	}
	cv::Mat histogram_visual(max, 256, CV_8UC1);
	for (int y = 0; y < histogram_visual.cols; y++) {
		for (int x = 0; x < histogram[y]; x++) {
			histogram_visual.at<uchar>(y, x) = 255;
		}
	}*/


	int cd_f[256];
	for (int i = 0; i < 256; i++)
	{
		int sum = 0;
		for (int j = 0; j < i; j++)
		{
			sum += histogram[j];
		}
		cd_f[i] = sum;
	}
	
	int cd_f_min;
	for (int i = 0; i < 256; i++)
	{
		if (cd_f[i] != 0) {
			cd_f_min = i;
			break;
		}
	}

	cv::Mat histogram_equalization = cv::Mat(width, height, CV_8UC1);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{

			int v = src.at<uchar>(i, j);
			histogram_equalization.at<uchar>(i, j) = round((((cd_f[v] - cd_f_min) * (256 - 1)) / ((width * height) - cd_f_min)));
		}
	}

	cv::imshow("original", src);
	//cv::imshow("original histogram", histogram_visual);
	cv::imshow("equalized image", histogram_equalization);
}

void cv9()
{
	//projective transform

	cv::Mat school = cv::imread("images/vsb.jpg", CV_LOAD_IMAGE_COLOR);
	cv::Mat flag = cv::imread("images/flag.png", CV_LOAD_IMAGE_COLOR);
	cv::Mat final_result = school.clone();
	cv::imshow("school", school);
	cv::imshow("flag", flag);

	int flag_width = flag.rows;
	int flag_height = flag.cols;

	int flag_coordinates_x[] = { 0,323,323,0 };
	int flag_coordinates_y[] = { 0,0,215,215 };
	int school_coordinates_x[] = { 69,227,228,66 };
	int school_coordinates_y[] = { 107,76,122,134 };

	cv::Mat a = cv::Mat(8, 8, CV_64FC1);
	cv::Mat b = cv::Mat(8, 1, CV_64FC1);
	b = (cv::Mat_<double>(8, 1) << 
		school_coordinates_x[0],school_coordinates_y[0], school_coordinates_x[1], school_coordinates_y[1], school_coordinates_x[2], school_coordinates_y[2], school_coordinates_x[3], school_coordinates_y[3]);

	a = (cv::Mat_<double>(8, 8) <<
		flag_coordinates_x[0], flag_coordinates_y[0], 1, 0, 0, 0, -school_coordinates_x[0] * flag_coordinates_x[0], -school_coordinates_x[0] * flag_coordinates_y[0],
		0, 0, 0, flag_coordinates_x[0], flag_coordinates_y[0], 1, -school_coordinates_y[0] * flag_coordinates_x[0], -school_coordinates_y[0] * flag_coordinates_y[0],

		flag_coordinates_x[1], flag_coordinates_y[1], 1, 0, 0, 0, -school_coordinates_x[1] * flag_coordinates_x[1], -school_coordinates_x[1] * flag_coordinates_y[1],
		0, 0, 0, flag_coordinates_x[1], flag_coordinates_y[1], 1, -school_coordinates_y[1] * flag_coordinates_x[1], -school_coordinates_y[1] * flag_coordinates_y[1],

		flag_coordinates_x[2], flag_coordinates_y[2], 1, 0, 0, 0, -school_coordinates_x[2] * flag_coordinates_x[2], -school_coordinates_x[2] * flag_coordinates_y[2],
		0, 0, 0, flag_coordinates_x[2], flag_coordinates_y[2], 1, -school_coordinates_y[2] * flag_coordinates_x[2], -school_coordinates_y[2] * flag_coordinates_y[2],

		flag_coordinates_x[3], flag_coordinates_y[3], 1, 0, 0, 0, -school_coordinates_x[3] * flag_coordinates_x[3], -school_coordinates_x[3] * flag_coordinates_y[3],
		0, 0, 0, flag_coordinates_x[3], flag_coordinates_y[3], 1, -school_coordinates_y[3] * flag_coordinates_x[3], -school_coordinates_y[3] * flag_coordinates_y[3]);

	cv::Mat result = cv::Mat(8, 1, CV_64FC1);

	cv::solve(a, b, result);

	for (int i = 0; i < flag_height; i++)
	{
		for (int j = 0; j < flag_width; j++)
		{
			int x = (i * result.at<double>(0, 0) + j * result.at<double>(1, 0) + result.at<double>(2, 0)) / (i * result.at<double>(6, 0) + j * result.at<double>(7, 0) + 1);
			int y = (i * result.at<double>(3, 0) + j * result.at<double>(4, 0) + result.at<double>(5, 0)) / (i * result.at<double>(6, 0) + j * result.at<double>(7, 0) + 1);

			final_result.at<cv::Vec3b>(y, x) = flag.at<cv::Vec3b>(j, i);
		}

	}

	cv::imshow("result", final_result);

}

void cv10()
{
	//edge detection
	cv::Mat source = cv::imread("images/valve.png", CV_LOAD_IMAGE_GRAYSCALE);

	cv::Mat G_x = (cv::Mat_<uchar>(3, 3) << -1, 0, 1, -2, 0, -2, -1, 0, 1);
	cv::Mat G_y = (cv::Mat_<uchar>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);

	int width = source.rows;
	int height = source.cols;
	cv::Mat result = cv::Mat(width, height, CV_8UC1);

	for (int y = 1; y < width - 1; y++)
	{
		for (int x = 1; x < height - 1; x++)
		{
			double sum_x = 0, sum_y = 0;

			sum_x = source.at<uchar>(y - 1, x - 1) +
				2 * source.at<uchar>(y, x - 1) +
				source.at<uchar>(y + 1, x - 1) -
				source.at<uchar>(y - 1, x + 1) -
				2 * source.at<uchar>(y, x + 1) -
				source.at<uchar>(y + 1, x + 1);

			sum_y = source.at<uchar>(y - 1, x - 1) +
				2 * source.at<uchar>(y - 1, x) +
				source.at<uchar>(y - 1, x + 1) -
				source.at<uchar>(y + 1, x - 1) -
				2 * source.at<uchar>(y + 1, x) -
				source.at<uchar>(y + 1, x + 1);

			double sum = sqrt(pow(sum_x, 2) + pow(sum_y, 2));

			if (sum > 255)
			{
				sum = 255;
			}
			if (sum < 0)
			{
				sum = 0;
			}

			result.at<uchar>(y, x) = sum;
		}
	}
	cv::imshow("original", source);
	cv::imshow("edge_detection", result);
}

void cv100(int lower_thr, int upper_thr)
{
	//edge thinning

	if (lower_thr > upper_thr)
		return;

	cv::Mat source = cv::imread("images/valve.png", CV_LOAD_IMAGE_GRAYSCALE);

	int width = source.rows;
	int height = source.cols;
	
	cv::Mat result = cv::Mat(width, height, CV_8UC1);
	cv::Mat magnitudes = cv::Mat(width, height, CV_64FC2);

	for (int x = 1; x < width - 1; x++)
	{
		for (int y = 1; y < height - 1; y++)
		{
			double fx = (source.at<uchar>(x - 1, y) - source.at<uchar>(x + 1, y)) / 2.0;
			double fy = (source.at<uchar>(x, y - 1) - source.at<uchar>(x, y + 1)) / 2.0;
			
			double strenght = sqrt(fx * fx + fy * fy);
			double angle = atan2(fy, fx) * (180 / CV_PI);
			
			if (angle >= -22.5 && angle <= 22.5 || angle >= 157.5 && angle <= 180 || angle >= -180 && angle <= -157.5) {
				angle = 0;
			}
			else if (angle >= 22.5 && angle <= 67.5 || angle >= -157.5 && angle <= -112.5) {
				angle = 45;
			}
			else if (angle >= 67.5 && angle <= 112.5 || angle >= -112.5 && angle <= -67.5) {
				angle = 90;
			}
			else if (angle >= 112.5 && angle <= 157.5 || angle >= -67.5 && angle <= -22.5) {
				angle = 135;
			}
			magnitudes.at<cv::Vec2d>(x, y)(0) = angle;
			magnitudes.at<cv::Vec2d>(x, y)(1) = strenght;
		}
	}

	for (int x = 1; x < width - 1; x++)
	{
		for (int y = 1; y < height - 1; y++)
		{
			double angle = magnitudes.at<cv::Vec2d>(x, y)(0);
			double strenght = magnitudes.at<cv::Vec2d>(x, y)(1);
			
			if (strenght < lower_thr) {
				result.at<uchar>(x, y) = 0;
			}
			else if (strenght > upper_thr) {
				result.at<uchar>(x, y) = 255;
			}
			else if (angle == 0) {
				if (strenght >= magnitudes.at<cv::Vec2d>(x - 1, y)(1) && strenght >= magnitudes.at<cv::Vec2d>(x + 1, y)(1)) {
					result.at<uchar>(x, y) = 255;
				}
				else {
					result.at<uchar>(x, y) = 0;
				}
			}
			else if (angle == 45) {
				if (strenght >= magnitudes.at<cv::Vec2d>(x + 1, y - 1)(1) && strenght >= magnitudes.at<cv::Vec2d>(x - 1, y + 1)(1)) {
					result.at<uchar>(x, y) = 255;
				}
				else {
					result.at<uchar>(x, y) = 0;
				}
			}
			else if (angle == 90) {
				if (strenght >= magnitudes.at<cv::Vec2d>(x, y - 1)(1) && strenght >= magnitudes.at<cv::Vec2d>(x, y + 1)(1)) {
					result.at<uchar>(x, y) = 255;
				}
				else {
					result.at<uchar>(x, y) = 0;
				}
			}
			else if (angle == 135) {
				if (strenght >= magnitudes.at<cv::Vec2d>(x - 1, y - 1)(1) && strenght >= magnitudes.at<cv::Vec2d>(x + 1, y + 1)(1)) {
					result.at<uchar>(x, y) = 255;
				}
				else {
					result.at<uchar>(x, y) = 0;
				}
			}
		}
	}
	
	cv::imshow("edge thining", result);
}

void edge_thining(cv::Mat& src_8uc3_img, cv::Mat& dst_8uc3_img, bool bili, double lower_thr = 15, double upper_thr = 50)
{
	int width = src_8uc3_img.rows;
	int height = src_8uc3_img.cols;

	cv::Mat result = cv::Mat(width, height, CV_8UC1);
	cv::Mat magnitudes = cv::Mat(width, height, CV_64FC2);

	for (int x = 1; x < width - 1; x++)
	{
		for (int y = 1; y < height - 1; y++)
		{
			double fx = (src_8uc3_img.at<uchar>(x - 1, y) - src_8uc3_img.at<uchar>(x + 1, y)) / 2.0;
			double fy = (src_8uc3_img.at<uchar>(x, y - 1) - src_8uc3_img.at<uchar>(x, y + 1)) / 2.0;

			double strenght = sqrt(fx * fx + fy * fy);
			double angle = atan2(fy, fx) * (180 / CV_PI);

			if (angle >= -22.5 && angle <= 22.5 || angle >= 157.5 && angle <= 180 || angle >= -180 && angle <= -157.5) {
				angle = 0;
			}
			else if (angle >= 22.5 && angle <= 67.5 || angle >= -157.5 && angle <= -112.5) {
				angle = 45;
			}
			else if (angle >= 67.5 && angle <= 112.5 || angle >= -112.5 && angle <= -67.5) {
				angle = 90;
			}
			else if (angle >= 112.5 && angle <= 157.5 || angle >= -67.5 && angle <= -22.5) {
				angle = 135;
			}
			magnitudes.at<cv::Vec2d>(x, y)(0) = angle;
			magnitudes.at<cv::Vec2d>(x, y)(1) = strenght;
		}
	}

	for (int x = 1; x < width - 1; x++)
	{
		for (int y = 1; y < height - 1; y++)
		{
			double angle = magnitudes.at<cv::Vec2d>(x, y)(0);
			double strenght = magnitudes.at<cv::Vec2d>(x, y)(1);

			if (strenght < lower_thr) {
				dst_8uc3_img.at<uchar>(x, y) = 0;
			}
			else if (strenght > upper_thr) {
				dst_8uc3_img.at<uchar>(x, y) = 255;
			}
			else if (angle == 0) {
				if (strenght >= magnitudes.at<cv::Vec2d>(x - 1, y)(1) && strenght >= magnitudes.at<cv::Vec2d>(x + 1, y)(1)) {
					dst_8uc3_img.at<uchar>(x, y) = 255;
				}
				else {
					dst_8uc3_img.at<uchar>(x, y) = 0;
				}
			}
			else if (angle == 45) {
				if (strenght >= magnitudes.at<cv::Vec2d>(x + 1, y - 1)(1) && strenght >= magnitudes.at<cv::Vec2d>(x - 1, y + 1)(1)) {
					dst_8uc3_img.at<uchar>(x, y) = 255;
				}
				else {
					dst_8uc3_img.at<uchar>(x, y) = 0;
				}
			}
			else if (angle == 90) {
				if (strenght >= magnitudes.at<cv::Vec2d>(x, y - 1)(1) && strenght >= magnitudes.at<cv::Vec2d>(x, y + 1)(1)) {
					dst_8uc3_img.at<uchar>(x, y) = 255;
				}
				else {
					dst_8uc3_img.at<uchar>(x, y) = 0;
				}
			}
			else if (angle == 135) {
				if (strenght >= magnitudes.at<cv::Vec2d>(x - 1, y - 1)(1) && strenght >= magnitudes.at<cv::Vec2d>(x + 1, y + 1)(1)) {
					dst_8uc3_img.at<uchar>(x, y) = 255;
				}
				else {
					dst_8uc3_img.at<uchar>(x, y) = 0;
				}
			}
		}
	}
}

void apply_rld2(int id, void* user_data)
{
	RLDUserData* rld_user_data = (RLDUserData*)user_data;

	edge_thining(rld_user_data->src_8uc3_img, rld_user_data->undistorted_8uc3_img, !false, rld_user_data->K1*2, rld_user_data->K2*2);
	cv::imshow("Edges", rld_user_data->undistorted_8uc3_img);
}



int cv11()
{
	cv::Mat src_8uc3_img, geom_8uc3_img;
	RLDUserData rld_user_data(5, 5, src_8uc3_img, geom_8uc3_img);

	src_8uc3_img = cv::imread("images/valve.png", cv::IMREAD_GRAYSCALE);
	if (src_8uc3_img.empty())
	{
		printf("Unable to load image!\n");
		exit(-1);
	}

	cv::namedWindow("Original Image");
	cv::imshow("Original Image", src_8uc3_img);

	geom_8uc3_img = src_8uc3_img.clone();
	apply_rld2(0, (void*)&rld_user_data);

	cv::namedWindow("Edges");
	cv::imshow("Edges", geom_8uc3_img);

	cv::createTrackbar("Thr1", "Edges", &rld_user_data.K1, 20, apply_rld2, &rld_user_data);
	cv::createTrackbar("Thr2", "Edges", &rld_user_data.K2, 20, apply_rld2, &rld_user_data);

	cv::waitKey(0);

	return 0;
}

void cv12()
{
	//Laplace

	cv::Mat source = cv::imread("images/valve.png", CV_LOAD_IMAGE_GRAYSCALE);

	int width = source.rows;
	int height = source.cols;

	cv::Mat derivation1 = cv::Mat(width, height, CV_64FC2);
	cv::Mat derivation2 = cv::Mat(width, height, CV_64FC2);
	cv::Mat result = cv::Mat(width, height, CV_8UC1);

	for (int x = 1; x < width - 1; x++)
	{
		for (int y = 1; y < height - 1; y++)
		{
			derivation1.at<cv::Vec2d>(x, y)(0) = (source.at<uchar>(x - 1, y) - source.at<uchar>(x + 1, y)) / 2.0;
			derivation1.at<cv::Vec2d>(x, y)(1) = (source.at<uchar>(x, y - 1) - source.at<uchar>(x, y + 1)) / 2.0;
		}
	}

	for (int x = 1; x < width - 1; x++)
	{
		for (int y = 1; y < height - 1; y++)
		{
			derivation2.at<cv::Vec2d>(x, y)(0) = (derivation1.at<cv::Vec2d>(x - 1, y)(0) - derivation1.at<cv::Vec2d>(x + 1, y)(0)) / 2.0;
			derivation2.at<cv::Vec2d>(x, y)(1) = (derivation1.at<cv::Vec2d>(x, y - 1)(1) - derivation1.at<cv::Vec2d>(x, y + 1)(1)) / 2.0;

			result.at<uchar>(x, y) = sqrt(derivation2.at<cv::Vec2d>(x, y)(0) * derivation2.at<cv::Vec2d>(x, y)(0) + derivation2.at<cv::Vec2d>(x, y)(1) * derivation2.at<cv::Vec2d>(x, y)(1));
		}
	}

	cv::imshow("Laplace", result);
}


int main()
{
	cv::Mat src_8uc3_img = cv::imread("images/lena.png", CV_LOAD_IMAGE_COLOR); // load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)

	if (src_8uc3_img.empty()) {
		printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
	}

	//cv::imshow( "LENA", src_8uc3_img );

	cv::Mat gray_8uc1_img; // declare variable to hold grayscale version of img variable, gray levels wil be represented using 8 bits (uchar)
	cv::Mat gray_32fc1_img; // declare variable to hold grayscale version of img variable, gray levels wil be represented using 32 bits (float)

	cv::cvtColor(src_8uc3_img, gray_8uc1_img, CV_BGR2GRAY); // convert input color image to grayscale one, CV_BGR2GRAY specifies direction of conversion
	gray_8uc1_img.convertTo(gray_32fc1_img, CV_32FC1, 1.0 / 255.0); // convert grayscale image from 8 bits to 32 bits, resulting values will be in the interval 0.0 - 1.0
	int x = 10, y = 15; // pixel coordinates

	uchar p1 = gray_8uc1_img.at<uchar>(y, x); // read grayscale value of a pixel, image represented using 8 bits
	float p2 = gray_32fc1_img.at<float>(y, x); // read grayscale value of a pixel, image represented using 32 bits
	cv::Vec3b p3 = src_8uc3_img.at<cv::Vec3b>(y, x); // read color value of a pixel, image represented using 8 bits per color channel
	

	// print values of pixels
	printf("p1 = %d\n", p1);
	printf("p2 = %f\n", p2);
	printf("p3[ 0 ] = %d, p3[ 1 ] = %d, p3[ 2 ] = %d\n", p3[0], p3[1], p3[2]);

	gray_8uc1_img.at<uchar>(y, x) = 0; // set pixel value to 0 (black)

	// draw a rectangle
	cv::rectangle(gray_8uc1_img, cv::Point(65, 84), cv::Point(75, 94),
		cv::Scalar(50), CV_FILLED);

	// declare variable to hold gradient image with dimensions: width= 256 pixels, height= 50 pixels.
	// Gray levels wil be represented using 8 bits (uchar)
	cv::Mat gradient_8uc1_img(50, 256, CV_8UC1);

	// For every pixel in image, assign a brightness value according to the x coordinate.
	// This wil create a horizontal gradient.
	for (int y = 0; y < gradient_8uc1_img.rows; y++) {
		for (int x = 0; x < gradient_8uc1_img.cols; x++) {
			gradient_8uc1_img.at<uchar>(y, x) = x;
		}
	}

	//cv1();
	//cv2();
	//cv3(1000,0.1, 0.015);
	//cv4();
	//cv5();
	//cv6(20);
	//cv7();
	//cv8();
	//cv9();
	//cv10();
	//cv11();
	//cv12();




	// diplay images
	//cv::imshow("Gradient", gradient_8uc1_img);
	//cv::imshow("Lena gray", gray_8uc1_img);
	//cv::imshow("Lena gray 32f", gray_32fc1_img);
	

	cv::waitKey(0); // wait until keypressed

	return 0;
}


