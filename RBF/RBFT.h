#pragma once
#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
//     �@�@ RBFT��p���ĎO�p�`���b�V���̒��_�̕ԊҌ�̍��W�����߂�     �@    //
//    �@�@   �}�[�J�̓��͍��W�Əo�͍��W�̑Ή��_����W�������߂�              //
//      �@ �@ ���Ԏ��������l�����邽��3�t���[�����̑Ή��_���l����    �@      //
///////////////////////////////////////////////////////////////////////////////


using namespace std;
using namespace cv;

class RBFT {

private:
	int N; // ���_��
	vector<Point2i> A, B; // �ό`�O�E�ό`��̍��W
	Eigen::MatrixXf F, G, X; // F = GX

public:
	RBFT(std::vector<cv::Point2i> _before, std::vector<cv::Point2i> _after);
	~RBFT() {}
	double potential(const Point2i Ai, const Point2i Aj);

	/* Set�֐� */
	void SetF();
	void SetG();
	void SetX();

	/* RBF��Ԏ��s */
	std::vector<cv::Point2i> RBFT::Exe(const vector<Point2i> InputPoints);
};