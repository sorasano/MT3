#include "Vector3.h"
#include "Matrix4.h"

#include <DxLib.h>
#include <cstring>
#include <vector>

//���̕`��
int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);


//�~���̕`��
int DrawCone3D(const Vector3& TopPos, const Vector3& BottomPos, const float r, const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);

//�����̕`��
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color);

//�J�����̈ʒu�Ǝp���̐ݒ�
int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,
	const Vector3& cameraTarget,
	const Vector3& cameraUp
);

//���f���̍��W�ϊ��p�s����Z�b�g����
int MV1SetMatrix(const int MHAndle, const Matrix4 Matrix);

//�֐��v���g�^�C�v�錾
void DrawAxis3D(const float length);//x,y,z���`��
void DrawKeyOperation();//�L�[����

//����X�̏W��(vector�R���e�i)�A��Ԃ����Ԃ̓Y�����A���Ԍo�ߗ�
Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {

	//DxLib ������
	const int WindowWidth = 1024;
	const int WindowHeight = 576;

	ChangeWindowMode(true);							//�E�B���h�E���[�h�ɂ���
	SetGraphMode(WindowWidth, WindowHeight, 32);	//��ʃ��[�h�̃Z�b�g
	SetBackgroundColor(0, 0, 64);			//�w�i�F RGB
	if (DxLib_Init() == -1) return -1;		//DxLib ����������
	SetDrawScreen(DX_SCREEN_BACK);			//�`���𗠉�ʂɃZ�b�g

	SetUseZBufferFlag(TRUE);	//Z�o�b�t�@��L���ɂ���
	SetWriteZBufferFlag(TRUE);	//Z�o�b�t�@�ւ̏������݂�L���ɂ���

	//�J����������
	Vector3 cameraPosition(-20.0f, 20.0f, -200.0f);
	Vector3 cameraTarget(0.0f, 0.0f, 0.0f);
	Vector3 cameraUp(0.0f, 1.0f, 0.0f);

	//�N���b�v��		��		��
	SetCameraNearFar(1.0f, 10000.0f);//�J�����̗L���͈͂�ݒ�
	SetCameraScreenCenter(WindowWidth / 2.0f, WindowHeight / 2.0f);	//��ʂ̒��S���J�����ɍ��킹��
	SetCameraPositionAndTargetAndUpVec(cameraPosition, cameraTarget, cameraUp);

	//TCHAR buf[256] = "fighter/fighter.mqo";
	//const char str[20] = "fighter/fighter.mqo";
	////�A�v���Ŏg�p����ϐ�
	//int model = MV1LoadModel(buf);

	//// xyz���̉�]�p�x
	//const float ROT_UNIT = 0.01f;
	//float rotX = 0.0f;
	//float rotY = 0.0f;
	//float rotZ = 0.0f;

	//���Ԍv���ɕK�v�ȃf�[�^
	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	//��ԂŎg���f�[�^
	//start -> end�@�� [s] �Ŋ���������
	//Vector3 start(-100.0f,0,0);
	//Vector3 end(+100.0f,0,0);
	Vector3 start(-100.0f, 0.0f, 0.0f);	//�X�^�[�g�n�_
	Vector3 p2(-50.0f, 50.0f, +50.0f);	//����_
	Vector3 p3(+50.0f, -30.0f, -50.0f);	//����_
	Vector3 end(+100.0f, 0.0f, 0.0f);	//�G���h�n�_

	std::vector<Vector3> points{ start,start,p2,p3,end,end };

	//p1����X�^�[�g����
	size_t startIndex = 1;

	float maxTime = 5.0f;		//�S�̎���[s]
	float timeRate;				//�������Ԃ��i�񂾂�(��)

	//���̈ʒu
	Vector3 position;

	//���s�O�ɃJ�E���^�l���擾
	startCount = GetNowHiPerformanceCount();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {


		////�X�V
		//if (CheckHitKey(KEY_INPUT_A)) rotY += ROT_UNIT;
		//if (CheckHitKey(KEY_INPUT_D)) rotY -= ROT_UNIT;

		//if (CheckHitKey(KEY_INPUT_W)) rotX += ROT_UNIT;
		//if (CheckHitKey(KEY_INPUT_S)) rotX -= ROT_UNIT;

		//if (CheckHitKey(KEY_INPUT_E)) rotZ += ROT_UNIT;
		//if (CheckHitKey(KEY_INPUT_Z)) rotZ -= ROT_UNIT;

		////�e��ϊ��s��̌v�Z
		//Matrix4 matScale = scale(Vector3(5.0f, 5.0f, 5.0f));	//���f���̊g�嗦

		//Matrix4 matRotX = rotateX(rotX);

		//Matrix4 matRotY = rotateY(rotY);

		//Matrix4 matRotZ = rotateZ(rotZ);

		//Matrix4 matRot = matRotX * matRotY * matRotZ;

		//Matrix4	matWorld = matScale * matRot;

		//MV1SetMatrix(model, matWorld);

		//[R]�Ń��Z�b�g
		if (CheckHitKey(KEY_INPUT_R)) {
			startCount = GetNowHiPerformanceCount();
			startIndex = 1;
		}

		//�o�ߎ���(elapsedTime [s])�̌v�Z
		nowCount = GetNowHiPerformanceCount();
		elapsedCount = nowCount - startCount;
		float elapsedTime = static_cast<float> (elapsedCount) / 1'000'000.0f;

		//�X�^�[�g�n�_			: start
		//�G���h�n�_			: end
		//�o�ߎ���			: elapsedTime [s]
		//�ړ������̗�(�o�ߎ���/�S�̎���) : timeRate (%)

		timeRate = elapsedTime / maxTime;
		/*	timeRate = min(elapsedTime / maxTime, 1.0f);*/

		if (timeRate >= 1.0f) {
			if (startIndex < points.size() - 3) {
				startIndex++;
				timeRate -= -1.0f;
				startCount = GetNowHiPerformanceCount();
			}
			else {
				timeRate = 1.0f;
			}
		}

		position = splinePosition(points, startIndex, timeRate);

		//Vector3 a = lerp(p0,p1, timeRate);
		//Vector3 b = lerp(p1, p2, timeRate);
		//Vector3 c = lerp(p2, p3, timeRate);

		//Vector3 d = lerp(a, b, timeRate);
		//Vector3 e = lerp(b, c, timeRate);

		//position = lerp(d, e, timeRate);

		//position = lerp(start, end, timeRate);
		//position = easeIn(start, end, timeRate);
		//position = easeOut(start, end, timeRate);
		//position = easeInOut(start, end, timeRate);

		//�`��
		ClearDrawScreen();		//��ʂ�����
		DrawAxis3D(500.0f);		//xyz���̕`��

		DrawSphere3D(start, 2.0f, 32, GetColor(0,255,  0), GetColor(255, 255, 255), TRUE);//���̕`��
		DrawSphere3D(p2, 2.0f, 32, GetColor(0,255,  0), GetColor(255, 255, 255), TRUE);//���̕`��
		DrawSphere3D(p3, 2.0f, 32, GetColor(0,255, 0), GetColor(255, 255, 255), TRUE);//���̕`��
		DrawSphere3D(end, 2.0f, 32, GetColor(0,255,  0), GetColor(255, 255, 255), TRUE);//���̕`��

		DrawSphere3D(position, 2.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);//���̕`��


		DrawFormatString(0, 0, GetColor(255, 255, 255), "position (%5.1f,%5.1f,%5.1f)", position.x, position.y, position.z);
		DrawFormatString(0, 20, GetColor(255, 255, 255), "%7.3f [s]", elapsedTime);
		DrawFormatString(0, 40, GetColor(255, 255, 255), "[R] : Restart");


		//MV1DrawModel(model);	//���f���̕`��
		//DrawKeyOperation();		//�L�[����̕`��

		ScreenFlip();			//�t���b�v����
	}
	DxLib::DxLib_End();			//DxLib �I������
	return 0;	//�\�t�g�̏I��
}

//xyz���̕`��
void DrawAxis3D(const float length) {
	//���̐��̕`��	
	DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));	//x��
	DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));	//y��
	DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));	//z��

	////���̐�̕`��
	//const float coneSize = 10.0f;
	//DrawCone3D(Vector3(length, 0, 0), Vector3(length - coneSize, 0, 0), coneSize / 2, 16, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);
	//DrawCone3D(Vector3(0, length, 0), Vector3(0, length - coneSize, 0), coneSize / 2, 16, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
	//DrawCone3D(Vector3(0, 0, length), Vector3(0, 0, length - coneSize), coneSize / 2, 16, GetColor(0, 0, 255), GetColor(255, 255, 255), TRUE);

}

//�L�[����̕`��
void DrawKeyOperation() {
	const unsigned white = GetColor(255, 255, 255);

	//DrawFormatString(10, 20 * 1, white, "  [W][E][R]  R : ���Z�b�g");
	//DrawFormatString(10, 20 * 2, white, "[A][S][D]    AD: y�����̉�]");
	//DrawFormatString(10, 20 * 3, white, " [Z]         WS: x�����̉�]");
	//DrawFormatString(10, 20 * 4, white, "             EZ: z�����̉�]");

}

//���̕`��
int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag) {
	VECTOR centerPos = { CenterPos.x, CenterPos.y, CenterPos.z };
	return DrawSphere3D(centerPos, r, DivNum, DifColor, SpcColor, FillFlag);
}


//�~���̕`��
int DrawCone3D(const Vector3& TopPos, const Vector3& BottomPos, const float r, const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag) {
	VECTOR topPos = { TopPos.x,TopPos.y ,TopPos.z };
	VECTOR bottomPos = { BottomPos.x,BottomPos.y,BottomPos.z };

	return DrawCone3D(topPos, bottomPos, r, DivNum, DifColor, SpcColor, FillFlag);
}

//�����̕`��
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color) {
	VECTOR p1 = { Pos1.x,Pos1.y,Pos1.z };
	VECTOR p2 = { Pos2.x,Pos2.y,Pos2.z };

	return DrawLine3D(p1, p2, Color);
}

//�J�����̈ʒu�Ǝp���̐ݒ�
int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,
	const Vector3& cameraTarget,
	const Vector3& cameraUp
) {
	VECTOR position = { cameraPosition.x,cameraPosition.y,cameraPosition.z };
	VECTOR target = { cameraTarget.x,cameraTarget.y,cameraTarget.z };
	VECTOR up = { cameraUp.x,cameraUp.y,cameraUp.z };

	return SetCameraPositionAndTargetAndUpVec(position, target, up);
}

//���f���̍��W�ϊ��p�s����Z�b�g����
int MV1SetMatrix(const int MHandle, const Matrix4 Matrix) {
	MATRIX matrix;
	std::memcpy(&matrix, &Matrix, sizeof MATRIX);		//�������ԃR�s�[

	return MV1SetMatrix(MHandle, matrix);
}

//����X�̏W��(vector�R���e�i)�A��Ԃ����Ԃ̓Y�����A���Ԍo�ߗ�
Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t) {

	//��Ԃ��ׂ��_�̐�
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n];
	if (startIndex < 1)return points[1];

	Vector3 p0 = points[startIndex - 1];
	Vector3 p1 = points[startIndex];
	Vector3 p2 = points[startIndex + 1];
	Vector3 p3 = points[startIndex + 2];

	Vector3 position = 0.5 * (2 * p1 + (-p0 + p2) * t + (2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) + (-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t));

	return position;
}
