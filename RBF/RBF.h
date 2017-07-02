#pragma once
#include "stdafx.h"

using namespace std;
using namespace cv;

class RBF {

private:

	int N; // �R���g���[���|�C���g�̐�
	std::vector<cv::Point2i> before; // �ό`�O
	std::vector<cv::Point2i> after; // �ό`�O

	// Y = WX
	vector<Point2i> X;
	Eigen::MatrixXf Y;
	Eigen::MatrixXf W;
	Eigen::MatrixXf Phi;

public:

	RBF(std::vector<cv::Point2i> _before, std::vector<cv::Point2i> _after);
	~RBF(){}
	void SetY(std::vector<cv::Point2i> _CenterPoints);
	void SetPhi();
	void SetW();
	cv::Point2i GetValue(const int i);
	void Exe(const std::vector<cv::Point2i> _CenterPoints, std::vector<cv::Point2i>& _AllPoints);
};