#include "stdafx.h"

/* Global�ϐ� */
std::vector<cv::Point2i> AllPoints_before; // �_�Q�̕ό`�O�̍��W�̃��X�g
std::vector<cv::Point2i> AllPoints_after; // �_�Q�̕ό`��̍��W�̃��X�g
std::vector<cv::Point2f> AllPoints_uv; // �e�_�ɑΉ�����e�N�X�`�����W�̃��X�g

std::vector<cv::Point2i> ControlPoints_before; // �R���g���[���_�̕ό`�O�̍��W�̃��X�g
std::vector<cv::Point2i> ControlPoints_after; // �R���g���[���_�̕ό`�O�̍��W�̃��X�g

int ChoicedIndex = -1; // �R���g���[������_�̃C���f�b�N�X
double mousex, mousey; // �}�E�X�̍��W

bool bDrag = false; // �h���b�O
bool bTexture = false; // �e�N�X�`���̉���
bool bPoint = true; // �_�Q�̉���

/* �e�N�X�`���̓ǂݍ��� */
bool LoadGLTextures()
{
	Mat tex = imread(TexturePath);
	GLFWimage img;

	cvtColor(tex, tex, CV_BGR2RGB);
	flip(tex, tex, 0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXWIDTH, TEXHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// �e�N�X�`�����W�̎w��
	AllPoints_uv.clear();
	for (int i = 0; i < 51; ++i) {
		for (int j = 0; j < 51; ++j) {
			AllPoints_uv.push_back(Point2f(j * (float)(1.0 / 50.0), i * (float)(1.0 / 50.0)));
		}
	}
	return true;
}

/* �e�N�X�`���}�b�s���O */
void TextureMapping() {

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);

	for (int i = 0; i < 50; ++i) {

		glBegin(GL_TRIANGLES);

		for (int j = 0; j < 50; ++j) {

			glTexCoord2f(AllPoints_uv[51 * i + j].x, AllPoints_uv[51 * i + j].y);
			glVertex2i(AllPoints_after[51 * i + j].x, AllPoints_after[51 * i + j].y);

			glTexCoord2f(AllPoints_uv[51 * (i + 1) + j].x, AllPoints_uv[51 * (i + 1) + j].y);
			glVertex2i(AllPoints_after[51 * (i + 1) + j].x, AllPoints_after[51 * (i + 1) + j].y);

			glTexCoord2f(AllPoints_uv[51 * i + (j + 1)].x, AllPoints_uv[51 * i + (j + 1)].y);
			glVertex2i(AllPoints_after[51 * i + (j + 1)].x, AllPoints_after[51 * i + (j + 1)].y);


			glTexCoord2f(AllPoints_uv[51 * (i + 1) + j].x, AllPoints_uv[51 * (i + 1) + j].y);
			glVertex2i(AllPoints_after[51 * (i + 1) + j].x, AllPoints_after[51 * (i + 1) + j].y);

			glTexCoord2f(AllPoints_uv[51 * (i + 1) + (j + 1)].x, AllPoints_uv[51 * (i + 1) + (j + 1)].y);
			glVertex2i(AllPoints_after[51 * (i + 1) + (j + 1)].x, AllPoints_after[51 * (i + 1) + (j + 1)].y);

			glTexCoord2f(AllPoints_uv[51 * i + (j + 1)].x, AllPoints_uv[51 * i + (j + 1)].y);
			glVertex2i(AllPoints_after[51 * i + (j + 1)].x, AllPoints_after[51 * i + (j + 1)].y);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}


static void Draw() 
{	
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(3.0);
	for (int i = 0; i < AllPoints_after.size(); ++i) {
		glBegin(GL_POINTS);
		glVertex2i(AllPoints_after[i].x, AllPoints_after[i].y);
		glEnd();
	}

	glColor3f(0.0, 1.0, 0.0);
	glPointSize(7.0);
	for (int i = 0; i < ControlPoints_after.size(); ++i) {
		glBegin(GL_POINTS);
		glVertex2i(ControlPoints_after[i].x, ControlPoints_after[i].y);
		glEnd();
	}
}

static void init()
{
	/* �w�i�F */
	glClearColor(1.0, 1.0, 1.0, 1.0);

	AllPoints_before.clear();
	AllPoints_after.clear();
	ControlPoints_before.clear();
	ControlPoints_after.clear();

	/* �����ʒu */
	for (int i = 0; i < 51; ++i) {
		for (int j = 0; j < 51; ++j) {
			AllPoints_before.push_back(Point2i(20 + 10 * j, 20 + 10 * i));
			AllPoints_after.push_back(Point2i(20 + 10 * j, 20 + 10 * i));
		}
	}

	/* �����R���g���[���_�̎w�� */
	ControlPoints_before.push_back(AllPoints_before[0]);
	ControlPoints_before.push_back(AllPoints_before[50]);
	ControlPoints_before.push_back(AllPoints_before[2550]);
	ControlPoints_before.push_back(AllPoints_before[2600]);

	ControlPoints_after.push_back(AllPoints_before[0]);
	ControlPoints_after.push_back(AllPoints_before[50]);
	ControlPoints_after.push_back(AllPoints_before[2550]);
	ControlPoints_after.push_back(AllPoints_before[2600]);
}

static void display(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT);

	/* �h���b�O���̑��� */
	if (bDrag && ChoicedIndex != -1) 
	{
		std::cout << "�h���b�O�Ȃ�" << " >> " << std::endl;
		glfwGetCursorPos(window, &mousex, &mousey);
		mousey = Height - mousey;
		std::cout << "(" << mousex << ", " << mousey << ")" << std::endl;

		ControlPoints_after[ChoicedIndex] = Point2i(mousex, mousey);

		/* RBFT */
		RBFT *rbft = new RBFT(ControlPoints_before, ControlPoints_after);
		rbft->SetX();
		AllPoints_after = rbft->Exe(AllPoints_before);
		delete rbft;
	}

	/* ���� */
	if (bTexture) {
		TextureMapping();
	}
	if (bPoint) {
		Draw();
	}

	glFlush();
}

static void mouse(GLFWwindow* window, int button, int action, int mods) 
{
	/* �{�^�����������Ƃ� */
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		bDrag = true;

		glfwGetCursorPos(window, &mousex, &mousey);
		mousey = Height - mousey; // ���_�������ɕϊ�
		std::cout << "(" << mousex << ", " << mousey << ")" << " >> ";

		/* find nearest point */
		if (!ControlPoints_before.empty()) {
			std::vector<double> distances;
			distances.clear();
			for (int i = 0; i < ControlPoints_after.size(); ++i) {
				distances.push_back(sqrt(pow(ControlPoints_after[i].x - mousex, 2) + pow(ControlPoints_after[i].y - mousey, 2)));
			}

			double minDistance = distances[0];
			ChoicedIndex = 0;
			for (int i = 1; i < ControlPoints_after.size(); ++i) {
				if (minDistance > distances[i]) {
					minDistance = distances[i];
					ChoicedIndex = i;
				}
			}

			if (minDistance > 10) {
				ChoicedIndex = -1;
				bDrag = false;
			}
		}
	}

	/* �{�^���𗣂����Ƃ� */
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		bDrag = false;
	}

	/* �R���g���[���|�C���g�̒ǉ��܂��͍폜 */
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &mousex, &mousey);
		mousey = Height - mousey;

		/* Find nearest ControlPoint */
		if (!ControlPoints_before.empty()) {
			std::vector<double> distances;
			distances.clear();
			for (int i = 0; i < ControlPoints_after.size(); ++i) {
				distances.push_back(sqrt(pow(ControlPoints_after[i].x - mousex, 2) + pow(ControlPoints_after[i].y - mousey, 2)));
			}

			double minDistance = distances[0];
			ChoicedIndex = 0;
			for (int i = 1; i < ControlPoints_after.size(); ++i) {
				if (minDistance > distances[i]) {
					minDistance = distances[i];
					ChoicedIndex = i;
				}
			}

			if (minDistance > 10) {
				ChoicedIndex = -1;
			}
		}
		// �폜
		if (ChoicedIndex != -1) {
			ControlPoints_before.erase(ControlPoints_before.begin() + ChoicedIndex);
			ControlPoints_after.erase(ControlPoints_after.begin() + ChoicedIndex);
		}
		// �ǉ�
		else {
			ControlPoints_before.push_back(Point2i(mousex, mousey));
			ControlPoints_after.push_back(Point2i(mousex, mousey));
		}
	}

}

void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch (key) {

		case '\033':
		case 'q':
		case 'Q':
			exit(0);
			break;

		case 'S':
		case 's':
			init();
			break;

		case 'T':
		case 't':
			if (bTexture) {
				bTexture = false;
			}
			else {
				bTexture = true;
			}
			break;

		case 'P':
		case 'p':
			if (bPoint) {
				bPoint = false;
			}
			else {
				bPoint = true;
			}
			break;
		}
	}
}

static void resize(GLFWwindow* window, int width, int height) {

	float aspect = (float)width / (float)height;

	glViewport(0, 0, width, height);
}

int main(int argc, char** argv)
{
	// GLFW ������������
	if (glfwInit() == GL_FALSE)
	{
		// �������Ɏ��s����
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(Width, Height, "Radical Basis Function", NULL, NULL);

	/* �J�����g��ʂ̐ݒ�(�e�N�X�`����GPU�������Ɋi�[����Ă��邽��) */
	glfwMakeContextCurrent(window);

	/* ������ */
	LoadGLTextures();
	init();

	/* �R�[���o�b�N�֐� */
	glfwSetMouseButtonCallback(window, mouse);
	glfwSetKeyCallback(window, key);
	glfwSetWindowSizeCallback(window, resize);

	/* �ˉe */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, Width,0, Height, -1, 1);

	/* �`�� */
	while (!glfwWindowShouldClose(window)) {
		display(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}