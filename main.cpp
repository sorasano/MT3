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

//Quaternionの積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
//単位Quaternionを返す
Quaternion IdentityQuaternion();
//共役Quaternionを返す
Quaternion Cojugate(const Quaternion& quaternion);
//Quaternionのnormを返す
float Norm(const Quaternion& quaternion);
//正規化したQuaternionを返す
Quaternion Normalize(const Quaternion& quaternion);
//逆Quaternionを返す
Quaternion Inverse(const Quaternion& quaternion);

//任意軸回転を表すQuaternionの生成
Quaternion MakeAxisAngle(const Vector3& axis, float angle);
//ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
//Quaternionから回転行列を求める
Matrix4 MakeRotateMatrix(const Quaternion& quaternion);
//ベクトルをアフィン変換する
Vector3 TransformAffine(const Vector3& vector, const Matrix4& matrix);

//球面線形補間
Quaternion slerp(const Quaternion& q0, const Quaternion& q1, float t);

//カメラの位置と姿勢の設定
int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,
	const Vector3& cameraTarget,
	const Vector3& cameraUp
);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {

	//DxLib 初期化
	const int WindowWidth = 1024;
	const int WindowHeight = 576;

	ChangeWindowMode(true);							//ウィンドウモードにする
	SetGraphMode(WindowWidth, WindowHeight, 32);	//画面モードのセット
	SetBackgroundColor(32, 32, 64);			//背景色 RGB
	if (DxLib_Init() == -1) return -1;		//DxLib 初期化処理
	SetDrawScreen(DX_SCREEN_BACK);			//描画先を裏画面にセット

	SetUseZBufferFlag(TRUE);	//Zバッファを有効にする
	SetWriteZBufferFlag(TRUE);	//Zバッファへの書き込みを有効にする

	//カメラ初期化
	Vector3 cameraPosition(0.0f, 200.0f, -1.0f);
	Vector3 cameraTarget(0.0f, 0.0f, 0.0f);
	Vector3 cameraUp(0.0f, 1.0f, 0.0f);

	//クリップ面		近		遠
	SetCameraNearFar(1.0f, 10000.0f);//カメラの有効範囲を設定
	SetCameraScreenCenter(WindowWidth / 2.0f, WindowHeight / 2.0f);	//画面の中心をカメラに合わせる
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

		//Quaternion rotation = MakeAxisAngle({ 0.0f,0.0f,1.0f }, 3.141592f / 2.0f);
		//Vector3 pointY = { 0.0f,1.0f,0.0f };
		//Matrix4 rotateMatrix = MakeRotateMatrix(rotation);
		//Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
		//Vector3 rotateByMatrix = TransformAffine(pointY, rotateMatrix);

		//DrawFormatString(0, 00, GetColor(255, 255, 255), "%f %f %f : rotateByQuaternion", rotateByQuaternion.x, rotateByQuaternion.y, rotateByQuaternion.z);
		//DrawFormatString(0, 20, GetColor(255, 255, 255), "%f %f %f : rotateByMatrix", rotateByMatrix.x, rotateByMatrix.y, rotateByMatrix.z);

		Quaternion rotation0 = MakeAxisAngle({ 0.71f,0.71f,0.0f }, 0.3f);
		Quaternion rotation1 = MakeAxisAngle({ 0.71f,0.0f,0.71f }, 3.141592f);

		Quaternion interplate0 = slerp(rotation0, rotation1, 0.0f);
		Quaternion interplate1 = slerp(rotation0, rotation1, 0.3f);
		Quaternion interplate2 = slerp(rotation0, rotation1, 0.5f);
		Quaternion interplate3 = slerp(rotation0, rotation1, 0.7f);
		Quaternion interplate4 = slerp(rotation0, rotation1, 1.0f);

		DrawFormatString(0, 00, GetColor(255, 255, 255), "%.2f %.2f %.2f %.2f: interplate0 ,sleap(q0,q1,0.0f) ", interplate0.x, interplate0.y, interplate0.z, interplate0.w);
		DrawFormatString(0, 20, GetColor(255, 255, 255), "%.2f %.2f %.2f %.2f: interplate1 ,sleap(q0,q1,0.3f) ", interplate1.x, interplate1.y, interplate1.z, interplate1.w);
		DrawFormatString(0, 40, GetColor(255, 255, 255), "%.2f %.2f %.2f %.2f: interplate2 ,sleap(q0,q1,0.5f) ", interplate2.x, interplate2.y, interplate2.z, interplate2.w);
		DrawFormatString(0, 60, GetColor(255, 255, 255), "%.2f %.2f %.2f %.2f: interplate3 ,sleap(q0,q1,0.7f) ", interplate3.x, interplate3.y, interplate3.z, interplate3.w);
		DrawFormatString(0, 80, GetColor(255, 255, 255), "%.2f %.2f %.2f %.2f: interplate4 ,sleap(q0,q1,1.0f) ", interplate4.x, interplate4.y, interplate4.z, interplate4.w);




		ScreenFlip();			//フリップする
	}
	DxLib::DxLib_End();			//DxLib 終了処理
	return 0;	//ソフトの終了
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
	Vector3 vec = { matrix.m[1][0],matrix.m[1][2] ,matrix.m[1][3] };

	return vec;
}

Quaternion slerp(const Quaternion& q0, const Quaternion& q1, float t)
{

	//q1 と q2 の内積
	float dot = (q0.x * q1.x) + (q0.y * q1.y) + (q0.z * q1.z) + (q0.w * q1.w);

	//内積反転
	if (dot < 0) {
		//q0 = -q0;
		dot = -dot;
	}

	//なす角を求める
	float theta = acos(dot);

	//thetaとsinを使って補間係数scale0,scale1を求める

	float scale0, scale1, st, sut, sout;

	st = (float)sin(theta);
	sut = (float)sin(t * theta);
	sout = (float)sin((1 - t) * theta);

	scale0 = sout / st;
	scale1 = sut / st;

	//それぞれの補間関数を利用して補間後のQuaternionを求める

	Quaternion q;

	q.x = scale0 * q0.x + scale1 * q1.x;
	q.y = scale0 * q0.y + scale1 * q1.y;
	q.z = scale0 * q0.z + scale1 * q1.z;
	q.w = scale0 * q0.w + scale1 * q1.w;

	return q;
}

//カメラの位置と姿勢の設定
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
