#include "Vector3.h"
#include "Matrix4.h"

#include <DxLib.h>
#include <cstring>

#include <Math.h>



struct Quaternion {
	float x;
	float y;
	float z;
	float w;

};

//Quaternion�̐�
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
//�P��Quaternion��Ԃ�
Quaternion IdentityQuaternion();
//����Quaternion��Ԃ�
Quaternion Cojugate(const Quaternion& quaternion);
//Quaternion��norm��Ԃ�
float Norm(const Quaternion& quaternion);
//���K������Quaternion��Ԃ�
Quaternion Normalize(const Quaternion& quaternion);
//�tQuaternion��Ԃ�
Quaternion Inverse(const Quaternion& quaternion);

//�C�ӎ���]��\��Quaternion�̐���
Quaternion MakeAxisAngle(const Vector3& axis, float angle);
//�x�N�g����Quaternion�ŉ�]���������ʂ̃x�N�g�������߂�
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
//Quaternion�����]�s������߂�
Matrix4 MakeRotateMatrix(const Quaternion& quaternion);
//�x�N�g�����A�t�B���ϊ�����
Vector3 TransformAffine(const Vector3& vector, const Matrix4& matrix);

//�J�����̈ʒu�Ǝp���̐ݒ�
int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,
	const Vector3& cameraTarget,
	const Vector3& cameraUp
);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {

	//DxLib ������
	const int WindowWidth = 1024;
	const int WindowHeight = 576;

	ChangeWindowMode(true);							//�E�B���h�E���[�h�ɂ���
	SetGraphMode(WindowWidth, WindowHeight, 32);	//��ʃ��[�h�̃Z�b�g
	SetBackgroundColor(32, 32, 64);			//�w�i�F RGB
	if (DxLib_Init() == -1) return -1;		//DxLib ����������
	SetDrawScreen(DX_SCREEN_BACK);			//�`���𗠉�ʂɃZ�b�g

	SetUseZBufferFlag(TRUE);	//Z�o�b�t�@��L���ɂ���
	SetWriteZBufferFlag(TRUE);	//Z�o�b�t�@�ւ̏������݂�L���ɂ���

	//�J����������
	Vector3 cameraPosition(0.0f, 200.0f, -1.0f);
	Vector3 cameraTarget(0.0f, 0.0f, 0.0f);
	Vector3 cameraUp(0.0f, 1.0f, 0.0f);

	//�N���b�v��		��		��
	SetCameraNearFar(1.0f, 10000.0f);//�J�����̗L���͈͂�ݒ�
	SetCameraScreenCenter(WindowWidth / 2.0f, WindowHeight / 2.0f);	//��ʂ̒��S���J�����ɍ��킹��
	SetCameraPositionAndTargetAndUpVec(cameraPosition, cameraTarget, cameraUp);


	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

		//Quaternion q1 = {2.0f,3.0f,4.0f,1.0f};
		//Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };

		//Quaternion identity = IdentityQuaternion();
		//Quaternion conj = Cojugate(q1);
		//Quaternion inv = Inverse(q1);
		//Quaternion normal = Normalize(q1);
		//Quaternion mul1 = Multiply(q1,q2);
		//Quaternion mul2 = Multiply(q2, q1);
		//float norm = Norm(q1);

		//DrawFormatString(0, 00, GetColor(255, 255, 255), "%f %f %f %f: Identity", identity.x, identity.y, identity.z, identity.w);
		//DrawFormatString(0, 20, GetColor(255, 255, 255), "%f %f %f %f: Cojugate", conj.x, conj.y, conj.z, conj.w);
		//DrawFormatString(0, 40, GetColor(255, 255, 255), "%f %f %f %f: Inverse", inv.x, inv.y, inv.z, inv.w);
		//DrawFormatString(0, 60, GetColor(255, 255, 255), "%f %f %f %f: Normalize", normal.x, normal.y, normal.z, normal.w);
		//DrawFormatString(0, 80, GetColor(255, 255, 255), "%f %f %f %f:  Multiply(q1,q2)", mul1.x, mul1.y, mul1.z, mul1.w);
		//DrawFormatString(0, 100, GetColor(255, 255, 255), "%f %f %f %f: Multiply(q2, q1)", mul2.x, mul2.y, mul2.z, mul2.w);
		//DrawFormatString(0, 120, GetColor(255, 255, 255), "%f: Norm", norm);

		Quaternion rotation = MakeAxisAngle({ 0.0f,0.0f,1.0f }, 3.141592f / 2.0f);
		Vector3 pointY = { 0.0f,1.0f,0.0f };
		Matrix4 rotateMatrix = MakeRotateMatrix(rotation);
		Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
		Vector3 rotateByMatrix = TransformAffine(pointY, rotateMatrix);

		DrawFormatString(0, 00, GetColor(255, 255, 255), "%f %f %f : rotateByQuaternion", rotateByQuaternion.x, rotateByQuaternion.y, rotateByQuaternion.z);
		DrawFormatString(0, 20, GetColor(255, 255, 255), "%f %f %f : rotateByMatrix", rotateByMatrix.x, rotateByMatrix.y, rotateByMatrix.z);



		ScreenFlip();			//�t���b�v����
	}
	DxLib::DxLib_End();			//DxLib �I������
	return 0;	//�\�t�g�̏I��
}

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion  multiply;

	multiply.x = lhs.y * rhs.z - lhs.z * rhs.y + rhs.w * lhs.x + lhs.w * rhs.x;
	multiply.y = lhs.z * rhs.x - lhs.x * rhs.z + rhs.w * lhs.y + lhs.w * rhs.y;
	multiply.z = lhs.x * rhs.y - lhs.y * rhs.x + rhs.w * lhs.z + lhs.w * rhs.z;
	multiply.w = lhs.w * rhs.w - lhs.x * rhs.x - rhs.y * lhs.y - lhs.z * rhs.z;

	return multiply;
}

Quaternion IdentityQuaternion()
{
	Quaternion identity = { 0.0f,0.0f,0.0f,1.0f };

	return identity;
}

Quaternion Cojugate(const Quaternion& quaternion)
{
	Quaternion cojugate = { -quaternion.x, -quaternion.y,-quaternion.z,quaternion.w };

	return cojugate;
}

float Norm(const Quaternion& quaternion)
{
	float norm = sqrt((quaternion.x * quaternion.x) + (quaternion.y * quaternion.y) + (quaternion.z * quaternion.z) + (quaternion.w * quaternion.w));

	return norm;
}

Quaternion Normalize(const Quaternion& quaternion)
{

	float norm = Norm(quaternion);

	Quaternion normalize = { quaternion.x / norm,quaternion.y / norm,quaternion.z / norm,quaternion.w / norm };

	return normalize;
}

Quaternion Inverse(const Quaternion& quaternion)
{
	float norm = Norm(quaternion);
	Quaternion cojugate = Cojugate(quaternion);

	Quaternion inverse = { cojugate.x / (norm * norm), cojugate.y / (norm * norm), cojugate.z / (norm * norm), cojugate.w / (norm * norm) };

	return inverse;
}

Quaternion MakeAxisAngle(const Vector3& axis, float angle)
{
	Quaternion axisAngle;

	axisAngle.w = cos(angle / 2.0);
	float c = sin(angle / 2.0);
	axisAngle.x = c * axis.x;
	axisAngle.y = c * axis.y;
	axisAngle.z = c * axis.z;

	return  axisAngle;
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	Vector3  RotVec;

	float ww = quaternion.w * quaternion.w;

	float xx = quaternion.x * quaternion.x;
	float yy = quaternion.y * quaternion.y;
	float zz = quaternion.z * quaternion.z;

	float wx = quaternion.w * quaternion.x;
	float wy = quaternion.w * quaternion.y;
	float wz = quaternion.w * quaternion.z;

	float xy = quaternion.x * quaternion.y;
	float xz = quaternion.x * quaternion.z;
	float yz = quaternion.y * quaternion.z;

	RotVec.x = ww * vector.x + 2 * wy * vector.z - 2 * wz * vector.y +
		xx * vector.x + 2 * xy * vector.y + 2 * xz * vector.z -
		zz * vector.x - yy * vector.x;
	RotVec.y = 2 * xy * vector.x + yy * vector.y + 2 * yz * vector.z +
		2 * wz * vector.x - zz * vector.y + ww * vector.y -
		2 * wx * vector.z - xx * vector.y;
	RotVec.z = 2 * xz * vector.x + 2 * yz * vector.y + zz * vector.z -
		2 * wy * vector.x - yy * vector.z + 2 * wx * vector.y -
		xx * vector.z + ww * vector.z;

	return  RotVec;
}

Matrix4 MakeRotateMatrix(const Quaternion& quaternion)
{
	Matrix4 m;

	float xx = quaternion.x * quaternion.x * 2.0f;
	float yy = quaternion.y * quaternion.y * 2.0f;
	float zz = quaternion.z * quaternion.z * 2.0f;
	float xy = quaternion.x * quaternion.y * 2.0f;
	float yz = quaternion.y * quaternion.z * 2.0f;
	float zx = quaternion.z * quaternion.x * 2.0f;
	float xw = quaternion.x * quaternion.w * 2.0f;
	float yw = quaternion.y * quaternion.w * 2.0f;
	float zw = quaternion.z * quaternion.w * 2.0f;
	
	m.m[0][0] = 1.0f - yy - zz;
	m.m[0][1] = xy + zw;
	m.m[0][2] = zx - yw;
	m.m[1][0] = xy - zw;
	m.m[1][1] = 1.0f - zz - xx;
	m.m[1][2] = yz + xw;
	m.m[2][0] = zx + yw;
	m.m[2][1] = yz - xw;
	m.m[2][2] = 1.0f - xx - yy;
	m.m[0][3] = m.m[1][3] = m.m[2][3] =
		m.m[3][0] = m.m[3][1] = m.m[3][2] = 0.0f;
	m.m[3][3] = 1.0f;

	return m;
}

Vector3 TransformAffine(const Vector3& vector, const Matrix4& matrix)
{
	Vector3 vec = { matrix .m[1][0],matrix.m[1][2] ,matrix.m[1][3] };

	return vec;
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
