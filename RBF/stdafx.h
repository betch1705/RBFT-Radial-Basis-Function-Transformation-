#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <glfw3.h>
#include <Eigen/Dense>
#include "RBFT.h"


#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"GLFW3.lib")
#pragma warning(disable:4996)


/* �ݒ� */
const int Width = 540; // �E�B���h�E�̕�
const int Height = 540; // �E�B���h�E�̍���

const int TEXWIDTH = 500; /* �e�N�X�`���̕� */
const int TEXHEIGHT = 500; /* �e�N�X�`���̍��� */
const std::string TexturePath = "texture.jpg"; // �ǂݍ��ރe�N�X�`���̃p�X

const double lamda = 0; // �A�t�B���ϊ��̒��x

