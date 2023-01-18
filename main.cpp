#include "Vector3.h"
#include "Matrix4.h"

#include <DxLib.h>
#include <cstring>
#include <vector>

//球の描画
int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);


//円錐の描画
int DrawCone3D(const Vector3& TopPos, const Vector3& BottomPos, const float r, const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);

//線分の描画
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color);

//カメラの位置と姿勢の設定
int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,
	const Vector3& cameraTarget,
	const Vector3& cameraUp
);

//モデルの座標変換用行列をセットする
int MV1SetMatrix(const int MHAndle, const Matrix4 Matrix);

//関数プロトタイプ宣言
void DrawAxis3D(const float length);//x,y,z軸描画
void DrawKeyOperation();//キー操作

//制御店の集合(vectorコンテナ)、補間する区間の添え字、時間経過率
Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {

	//DxLib 初期化
	const int WindowWidth = 1024;
	const int WindowHeight = 576;

	ChangeWindowMode(true);							//ウィンドウモードにする
	SetGraphMode(WindowWidth, WindowHeight, 32);	//画面モードのセット
	SetBackgroundColor(0, 0, 64);			//背景色 RGB
	if (DxLib_Init() == -1) return -1;		//DxLib 初期化処理
	SetDrawScreen(DX_SCREEN_BACK);			//描画先を裏画面にセット

	SetUseZBufferFlag(TRUE);	//Zバッファを有効にする
	SetWriteZBufferFlag(TRUE);	//Zバッファへの書き込みを有効にする

	//カメラ初期化
	Vector3 cameraPosition(-20.0f, 20.0f, -200.0f);
	Vector3 cameraTarget(0.0f, 0.0f, 0.0f);
	Vector3 cameraUp(0.0f, 1.0f, 0.0f);

	//クリップ面		近		遠
	SetCameraNearFar(1.0f, 10000.0f);//カメラの有効範囲を設定
	SetCameraScreenCenter(WindowWidth / 2.0f, WindowHeight / 2.0f);	//画面の中心をカメラに合わせる
	SetCameraPositionAndTargetAndUpVec(cameraPosition, cameraTarget, cameraUp);

	//TCHAR buf[256] = "fighter/fighter.mqo";
	//const char str[20] = "fighter/fighter.mqo";
	////アプリで使用する変数
	//int model = MV1LoadModel(buf);

	//// xyz軸の回転角度
	//const float ROT_UNIT = 0.01f;
	//float rotX = 0.0f;
	//float rotY = 0.0f;
	//float rotZ = 0.0f;

	//時間計測に必要なデータ
	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	//補間で使うデータ
	//start -> end　を [s] で完了させる
	//Vector3 start(-100.0f,0,0);
	//Vector3 end(+100.0f,0,0);
	Vector3 start(-100.0f, 0.0f, 0.0f);	//スタート地点
	Vector3 p2(-50.0f, 50.0f, +50.0f);	//制御点
	Vector3 p3(+50.0f, -30.0f, -50.0f);	//制御点
	Vector3 end(+100.0f, 0.0f, 0.0f);	//エンド地点

	std::vector<Vector3> points{ start,start,p2,p3,end,end };

	//p1からスタートする
	size_t startIndex = 1;

	float maxTime = 5.0f;		//全体時間[s]
	float timeRate;				//何％時間が進んだか(率)

	//球の位置
	Vector3 position;

	//実行前にカウンタ値を取得
	startCount = GetNowHiPerformanceCount();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {


		////更新
		//if (CheckHitKey(KEY_INPUT_A)) rotY += ROT_UNIT;
		//if (CheckHitKey(KEY_INPUT_D)) rotY -= ROT_UNIT;

		//if (CheckHitKey(KEY_INPUT_W)) rotX += ROT_UNIT;
		//if (CheckHitKey(KEY_INPUT_S)) rotX -= ROT_UNIT;

		//if (CheckHitKey(KEY_INPUT_E)) rotZ += ROT_UNIT;
		//if (CheckHitKey(KEY_INPUT_Z)) rotZ -= ROT_UNIT;

		////各種変換行列の計算
		//Matrix4 matScale = scale(Vector3(5.0f, 5.0f, 5.0f));	//モデルの拡大率

		//Matrix4 matRotX = rotateX(rotX);

		//Matrix4 matRotY = rotateY(rotY);

		//Matrix4 matRotZ = rotateZ(rotZ);

		//Matrix4 matRot = matRotX * matRotY * matRotZ;

		//Matrix4	matWorld = matScale * matRot;

		//MV1SetMatrix(model, matWorld);

		//[R]でリセット
		if (CheckHitKey(KEY_INPUT_R)) {
			startCount = GetNowHiPerformanceCount();
			startIndex = 1;
		}

		//経過時間(elapsedTime [s])の計算
		nowCount = GetNowHiPerformanceCount();
		elapsedCount = nowCount - startCount;
		float elapsedTime = static_cast<float> (elapsedCount) / 1'000'000.0f;

		//スタート地点			: start
		//エンド地点			: end
		//経過時間			: elapsedTime [s]
		//移動官僚の率(経過時間/全体時間) : timeRate (%)

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

		//描画
		ClearDrawScreen();		//画面を消去
		DrawAxis3D(500.0f);		//xyz軸の描画

		DrawSphere3D(start, 2.0f, 32, GetColor(0,255,  0), GetColor(255, 255, 255), TRUE);//球の描画
		DrawSphere3D(p2, 2.0f, 32, GetColor(0,255,  0), GetColor(255, 255, 255), TRUE);//球の描画
		DrawSphere3D(p3, 2.0f, 32, GetColor(0,255, 0), GetColor(255, 255, 255), TRUE);//球の描画
		DrawSphere3D(end, 2.0f, 32, GetColor(0,255,  0), GetColor(255, 255, 255), TRUE);//球の描画

		DrawSphere3D(position, 2.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);//球の描画


		DrawFormatString(0, 0, GetColor(255, 255, 255), "position (%5.1f,%5.1f,%5.1f)", position.x, position.y, position.z);
		DrawFormatString(0, 20, GetColor(255, 255, 255), "%7.3f [s]", elapsedTime);
		DrawFormatString(0, 40, GetColor(255, 255, 255), "[R] : Restart");


		//MV1DrawModel(model);	//モデルの描画
		//DrawKeyOperation();		//キー操作の描画

		ScreenFlip();			//フリップする
	}
	DxLib::DxLib_End();			//DxLib 終了処理
	return 0;	//ソフトの終了
}

//xyz軸の描画
void DrawAxis3D(const float length) {
	//軸の線の描画	
	DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));	//x軸
	DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));	//y軸
	DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));	//z軸

	////軸の先の描画
	//const float coneSize = 10.0f;
	//DrawCone3D(Vector3(length, 0, 0), Vector3(length - coneSize, 0, 0), coneSize / 2, 16, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);
	//DrawCone3D(Vector3(0, length, 0), Vector3(0, length - coneSize, 0), coneSize / 2, 16, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
	//DrawCone3D(Vector3(0, 0, length), Vector3(0, 0, length - coneSize), coneSize / 2, 16, GetColor(0, 0, 255), GetColor(255, 255, 255), TRUE);

}

//キー操作の描画
void DrawKeyOperation() {
	const unsigned white = GetColor(255, 255, 255);

	//DrawFormatString(10, 20 * 1, white, "  [W][E][R]  R : リセット");
	//DrawFormatString(10, 20 * 2, white, "[A][S][D]    AD: y軸回りの回転");
	//DrawFormatString(10, 20 * 3, white, " [Z]         WS: x軸回りの回転");
	//DrawFormatString(10, 20 * 4, white, "             EZ: z軸回りの回転");

}

//球の描画
int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag) {
	VECTOR centerPos = { CenterPos.x, CenterPos.y, CenterPos.z };
	return DrawSphere3D(centerPos, r, DivNum, DifColor, SpcColor, FillFlag);
}


//円錐の描画
int DrawCone3D(const Vector3& TopPos, const Vector3& BottomPos, const float r, const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag) {
	VECTOR topPos = { TopPos.x,TopPos.y ,TopPos.z };
	VECTOR bottomPos = { BottomPos.x,BottomPos.y,BottomPos.z };

	return DrawCone3D(topPos, bottomPos, r, DivNum, DifColor, SpcColor, FillFlag);
}

//線分の描画
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color) {
	VECTOR p1 = { Pos1.x,Pos1.y,Pos1.z };
	VECTOR p2 = { Pos2.x,Pos2.y,Pos2.z };

	return DrawLine3D(p1, p2, Color);
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

//モデルの座標変換用行列をセットする
int MV1SetMatrix(const int MHandle, const Matrix4 Matrix) {
	MATRIX matrix;
	std::memcpy(&matrix, &Matrix, sizeof MATRIX);		//メモリ間コピー

	return MV1SetMatrix(MHandle, matrix);
}

//制御店の集合(vectorコンテナ)、補間する区間の添え字、時間経過率
Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t) {

	//補間すべき点の数
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
