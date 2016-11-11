#include "stdafx.h"
#include "Player.h"
#include "InputManager.h"
#include "ShadowRender.h"
#include "RenderContext.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "EnemyManager.h"
#include "ParticleEmitter.h"
#include "MoveFloor.h"
#include "StageTable.h"
#include "FireJet.h"

CPlayer* g_player = NULL;

CPlayer::~CPlayer(){ 
	int bulletMax = m_bullets.size();
	for (int idx = 0; idx < bulletMax; idx++){
		SAFE_DELETE(m_bullets[idx]);
	}
	m_bullets.clear();
	m_Deletebullets.clear();

	m_CBManager.clear();
	m_DeleteChocoBall.clear();
}

void CPlayer::Initialize()

{
	m_ActiveKeyState = true;
	parent = NULL;
	g_player = this;
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/Player.X");

	CGameObject::Initialize();

	m_pInput = SINSTANCE(CInputManager)->GetCurrentInput();
	m_transform.position = PlayerTransformArray[m_StageID].pos;
	m_transform.angle = PlayerTransformArray[m_StageID].rotation;
	m_transform.scale = PlayerTransformArray[m_StageID].scale;

	// Unityから出力したポジション情報を最適化
	m_transform.position.x *= -1.0f;
	m_transform.position.z *= -1.0f;

	RV0 = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	localPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	m_moveSpeed.y = 0.0f;

	m_radius = 1.0f;

	deadTimer = 0.0f;

	SetAlive(true);
	m_pModel->m_alpha = 1.0f;
	
	LockOnflag = false;
	m_ShotFlg = false;
	m_PreviousJumpFlag = false;
	ChocoBall = false;
	
	GamaOverFlag = false;
	BusterEnemyNum = 0;
	m_size.x = 1.0f;
	m_size.y = 2.0f;
	m_size.z = 1.0f;

	//ジャンプ＆着地時のタイマー関係
	m_JumpParticleTime = 0.2f;
	m_JumpParticleTimer = 0.0f;
	
	//銃発射時のタイマー関係
	//m_GunParticleTime = 0.2f;
	//m_GunParticleTimer = 0.0f;

	BulletShotInterval = 0;

	m_GameState = GAMEEND_ID::CONTINUE;

	m_Courcedef.SetStageID(m_StageID);
	m_Courcedef.Initialize();
	m_NowCourceNo = -1;

	// ライト関連の初期化
	this->ConfigLight();
	
	m_IsIntersect.CollisitionInitialize(&m_transform.position, m_radius,CollisionType_Player);
	m_IsIntersect.SetAudio(m_pAudio);
	
	deadTimer = 0.0f;
	m_pCamera = SINSTANCE(CObjectManager)->FindGameObject<CCourceCamera>(_T("Camera"));
	CParticleEmitter::EmitterCreate(
		_T("ParticleEmitterStart"),
		PARTICLE_TYPE::STAR,
		m_transform.position,
		m_pCamera->GetCamera(),
		m_StageID,
		true
		);
	//ジャンプ＆着地時の煙
	m_pEmitter = CParticleEmitter::EmitterCreate(
		_T("ParticleEmitterSmoke"),
		PARTICLE_TYPE::SMOKE,
		m_transform.position,
		m_pCamera->GetCamera(),
		m_StageID,
		false
		);

	//銃発射時の煙
	/*m_pEmitter2 = CParticleEmitter::EmitterCreate(
		_T("ParticleEmitterGunParticle"),
		PARTICLE_TYPE::GUNPARTICLE,
		m_transform.position,
		m_pCamera->GetCamera(),
		m_StageID,
		false
		);*/

	m_pModel->SetUseBorn(true);
	m_vibration.Initialize();
	for (int idx = 0; idx < m_pModel->GetAnimation()->GetNumAnimationSet(); idx++){
		m_pModel->GetAnimation()->SetAnimationEndtime(idx, AnimationTime[idx]);
	}
	m_pModel->GetAnimation()->PlayAnimation(-1,0.0f);
	m_State = MOVE_STATE::Wait;
	m_VibrationCounter = 0.0f;
	m_VibrationInterval = 0.5f;

	m_JumpState = JUMP_STATE::J_NULL;
	m_JumpEndCounter = 0.0f;
	m_JumpEndInterval = 0.25f;
}

void CPlayer::SetParent(MoveFloor* parent)
{
	//親が設定されたので、ワールド座標を求めるために。一旦Updateを呼び出す。
	if (parent != NULL){
		Update();
		
		//親が設定されたので、ローカル座標を親のローカル座標に変換する。
		D3DXMATRIX mParentWorldInv = parent->GetModel()->m_World;
		D3DXMatrixInverse(&mParentWorldInv, NULL, &mParentWorldInv);
		D3DXVECTOR4 pos;
		D3DXVec3Transform(&pos, &m_transform.position, &mParentWorldInv);
		localPosition.x = pos.x;
		localPosition.y = pos.y;
		localPosition.z = pos.z;
		this->parent = parent;
	}
	else if (this->parent != NULL){
		
		D3DXMATRIX mParentWorld = this->parent->GetModel()->m_World;
		D3DXVECTOR4 pos;
		D3DXVec3Transform(&pos, &localPosition, &mParentWorld);
		localPosition.x = pos.x;
		localPosition.y = pos.y;
		localPosition.z = pos.z;
		this->parent = parent;
	}
}

void CPlayer::Update()
{
	// 毎フレームの初期化。
	{
		m_ShotFlg = false;
		m_moveSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//親がいるときの処理。
	if (parent)
	{
		D3DXMATRIX mParentWorld = parent->GetModel()->m_World;
		//親のワールド行列を乗算して、ローカル座標をワールド座標に変換する。

		D3DXVECTOR4 pos;
		D3DXVec3Transform(&pos, &localPosition, &mParentWorld);
		m_transform.position.x = pos.x;
		m_transform.position.y = pos.y;
		m_transform.position.z = pos.z;
	}

	if (m_GameState == GAMEEND_ID::CONTINUE)
	{
		// メインシーンの状態を管理する処理。
		StateManaged();

		// 当たり判定。
		Collisions();

		// キー判定。
		if (m_ActiveKeyState){
			KeyState();
		}

		// キー判定の結果と現在のフラグ状況から行動を選択。
		MoveStateManaged();

		//ロックオン処理
		//if (m_MoveFlg){
		//ロックオン距離が調整できるまでは米アウト
		//LockOn();
		//}

		if (m_ShotFlg){
			BulletShot();
		}
	}

	if (m_State != MOVE_STATE::Flow && m_State != MOVE_STATE::Fly){
		// 上下移動処理。
		UpDownMove();
		SetRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_currentAngleY);
		bool IsJump = false;
		if (m_JumpState != JUMP_STATE::J_NULL) {
			IsJump = true;
		}
		//プレイヤーの位置情報更新。
		m_IsIntersect.Intersect(&m_transform.position, &m_moveSpeed, IsJump);
	}

	// アニメーション再生関数を呼び出す
	m_pModel->SetCurrentAnimNo(m_AnimState);
	m_pModel->GetAnimation()->PlayAnimation(m_pModel->GetCurrentAnimNo(), 0.1f);

	// 影カメラのポジションをプレイヤーの真上に指定。
	SINSTANCE(CShadowRender)->SetObjectPos(m_transform.position);
	SINSTANCE(CShadowRender)->SetShadowCameraPos(m_transform.position + D3DXVECTOR3(0.0f, /*2.0f*/5.0f, 0.0f));

	// ライトの更新
	static_cast<CActreLight*>(m_pLight)->Update(m_pModel->m_World);

	// 弾の更新および削除。
	int size = m_bullets.size();
	for (int idx = 0; idx < size; idx++){
		if (m_bullets[idx]->Update()){
			DeleteBullet(m_bullets[idx]);
		}
		else if (BULLET_LENG < D3DXVec3Length(&(m_bullets[idx]->GetPos() - m_transform.position))){
			//プレイヤーと弾の距離が一定値になると弾が自動でDeleteする。
			DeleteBullet(m_bullets[idx]);
		}
	}
}

void CPlayer::Draw(){
	CGameObject::Draw();

	int size = m_bullets.size();
	for (int idx = 0; idx < size; idx++){
		m_bullets[idx]->Draw();
	}

	// 絶対にここで呼べよ！　絶対だぞっ！？
	ExcuteDeleteBullets();
	//ExcuteDeleteChocoBall();
}

void CPlayer::ConfigLight(){
	if (!m_pLight) {
		m_pLight = new CActreLight;
	}

	// ディフューズライト(キャラライト)の向き設定(ライト1～4)
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(0, D3DXVECTOR3(0.707f, 0.707f, 0.0f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(1, D3DXVECTOR3(1.0f, 1.0f, 0.0f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(2, D3DXVECTOR3(1.0f, -1.0f, 0.5f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(3, D3DXVECTOR3(0.0f, 0.0f, 1.0f));

	// ディフューズライト(キャラライト)の色設定(ライト1～4)
	m_pLight->SetDiffuseLightColor(0, D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f));
	m_pLight->SetDiffuseLightColor(1, D3DXVECTOR4(0.75f, 0.75f,0.75f, 1.0f));
	m_pLight->SetDiffuseLightColor(2, D3DXVECTOR4(0.75f, 0.75f,0.75f, 1.0f));
	m_pLight->SetDiffuseLightColor(3, D3DXVECTOR4(0.75f,0.75f, 0.75f, 1.0f));

	// アンビエントライト(環境光)の強さ設定
	m_pLight->SetAmbientLight(D3DXVECTOR3(0.1f, 0.1f, 0.1f));

	// リムライトの色と向き設定。
	m_pLight->SetLimLightColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pLight->SetLimLightDirection(D3DXVECTOR3(0.0f, 0.0f, -1.0f));

	CGameObject::ConfigLight();

	//// 現在共通のライトに設定。
	//SINSTANCE(CRenderContext)->SetCurrentLight(&m_light);
}

void CPlayer::MoveStateManaged(){
	switch (m_State){
	case MOVE_STATE::Wait:
		m_pCamera->SetTargetViewAngle(D3DXToRadian(30.0f));
		m_AnimState = ANIMATION_STATE::WAIT;
		m_ActiveKeyState = true;
		break;
	case MOVE_STATE::Walk:
		m_pCamera->SetTargetViewAngle(D3DXToRadian(45.0f));
		Move();
		m_AnimState = ANIMATION_STATE::WALK;
		m_ActiveKeyState = true;
		m_State = MOVE_STATE::Wait;
		break;
	case MOVE_STATE::Dash:
		m_pCamera->SetTargetViewAngle(D3DXToRadian(45.0f));
		Move();
		m_ActiveKeyState = true;
		m_State = MOVE_STATE::Wait;
		break;
	case MOVE_STATE::Vibration:
		m_vibration.Update();
		m_VibrationCounter = 0.0f;
		// 振動処理実行後、振動が終了しているか
		if (!m_vibration.GetIsVibration()){
			m_State = MOVE_STATE::Wait;
			m_ActiveKeyState = true;
			m_pCamera->SetIsTarget(true);
		}
		else{
			m_AnimState = ANIMATION_STATE::DAMAGE;
			m_ActiveKeyState = false;
		}
		break;
	case MOVE_STATE::Flow:
		m_pCamera->SetIsTarget(true);
		m_pCamera->GetCamera()->SetTarget(m_transform.position);
		m_AnimState = ANIMATION_STATE::WAIT;
		m_ActiveKeyState = false;
		m_JumpState = JUMP_STATE::J_NULL;
		//m_transform.position.y = 0.0f;
		RollingPlayer();
		break;
	case MOVE_STATE::Fly:
		m_pCamera->SetIsTarget(true);
		m_pCamera->GetCamera()->SetTarget(m_transform.position);
		m_AnimState = ANIMATION_STATE::WAIT;
		m_JumpState = JUMP_STATE::J_NULL;
		m_ActiveKeyState = false;
		RollingPlayer();
		break;
	}
}

void CPlayer::Move()
{
	isTurn = false;

	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	float X = m_pInput->GetStickL_XFloat();
	float Y = m_pInput->GetStickL_YFloat();

	//前後の動き
	if (fabs(Y) > 0.0f)
	{
		m_moveSpeed.z = Y * MOVE_SPEED;
		isTurn = true;
	}

	//左右の動き
	if (fabsf(X) > 0.0f)
	{
		m_moveSpeed.x = X * MOVE_SPEED;
		isTurn = true;
	}

	//プレイヤーの挙動をコース定義に沿ったものに補正する処理
	BehaviorCorrection();

	//D3DXToRadianの値は各自で設定する。 例　正面D3DXToRadian(0.0f)
	//isTurnはUpdateの最初でfalseにして、回転させたい時にtrueにする。
	m_currentAngleY = m_Turn.Update(isTurn, m_targetAngleY);
}

void CPlayer::UpDownMove(){

	switch (m_JumpState) {
	case JUMP_STATE::J_NULL:
		if (!m_IsIntersect.IsHitGround()) {
			m_JumpState = JUMP_STATE::J_EINS;
			m_NowJumpPower = PLAYER_JUMP_POWER / 2;
		}
		break;
	case JUMP_STATE::J_EINS:
		m_AnimState = ANIMATION_STATE::JUMP_START;
		if (m_IsIntersect.IsHitGround()){
			m_JumpState = JUMP_STATE::J_ZWEI;
			m_NowJumpPower = PLAYER_JUMP_POWER;
		}
		break;
	case JUMP_STATE::J_ZWEI:
		m_AnimState = ANIMATION_STATE::JUMP_NOW;
		//着地しているか判定(着地時はtrue,そうでなければfalse)。
		if (m_IsIntersect.IsHitGround())
		{
			// 着地した瞬間。
			m_JumpState = JUMP_STATE::J_DREI;
			m_pEmitter->SetEmitFlg(true);
			m_pAudio->PlayCue("Landing", true, this);
			m_JumpEndCounter = 0.0f;
		}
		break;
	case JUMP_STATE::J_DREI:
		m_AnimState = ANIMATION_STATE::JUMP_END;
		// 自分の周囲にパーティクル発生
		// Setすると常にプレイヤーの場所にパーティクルの発生する
		D3DXVECTOR3 pos = m_transform.position;//パーティクルのposを変えるためだけの格納
		pos.y = pos.y - 0.7f;
		m_pEmitter->SetEmitPos(pos);
		m_NowJumpPower = 0.0f;
		m_JumpEndCounter += 1.0f / 60.0f;
		if (m_JumpEndCounter >= m_JumpEndInterval) {
			m_JumpState = JUMP_STATE::J_NULL;
		}
		break;
	}
	m_moveSpeed.y = m_NowJumpPower;
	m_NowJumpPower -= GRAVITY;

	//1フレームでのカウンターの加算処理
	m_JumpParticleTimer += 1.0f / 60.0f;

	//ジャンプ＆着地時パーティクルの発生時間よりカウンターが超えたらパーティクルを消す＆カウンターも初期化
	if (m_JumpParticleTimer >= m_JumpParticleTime)
	{
		m_pEmitter->SetEmitFlg(false);
		m_JumpParticleTimer = 0.0f;
	}
}

void CPlayer::LockOn()
{
	float _X = 0.0f;

	//ロックオン状態にする。
	if (m_pInput->IsTriggerLeftShift() && LockOnflag == false)
	{
		m_LockOnEnemy = m_LockOn.FindNearEnemy(m_transform.position);
		if (m_LockOnEnemy){
			LockOnflag = true;
		}
		
	}
	//ロックオン状態の解除
	else if (m_pInput->IsTriggerLeftShift() && LockOnflag == true)
	{
		LockOnflag = false;
	}
	//ロックオン状態中の回転の計算
	if (LockOnflag)
	{
		if (!m_LockOnEnemy){
			LockOnflag = false;
		}
		else{
			_X = m_LockOn.LockOnRotation(_X, m_transform.position,m_LockOnEnemy);
		}
	}
	////ロックオン状態の時に常にプレイヤーを敵に向かせる
	//if (LockOnflag){
	//	m_targetAngleY = _X;
	//}

}

void CPlayer::BehaviorCorrection()
{
	D3DXVECTOR3		V1;
	V1 = m_pCamera->GetCamera()->GetTarget() - m_pCamera->GetCamera()->GetPos();
	
	D3DXVECTOR3		V2;
	D3DXVECTOR3		Up;

	Up.x = 0.0f;
	Up.y = 1.0f;
	Up.z = 0.0f;

	//直行するベクトルを求める。
	//COURCE_BLOCK Cource = m_Courcedef.FindCource(m_transform.position);
	//V1 = Cource.endPosition - Cource.startPosition;
	D3DXVec3Normalize(&V1, &V1);//3D ベクトルを正規化したベクトルを返す。
	D3DXVec3Cross(&V2, &V1,&Up);//2つの3Dベクトルの上方向の外積を求める→直行するV2が見つかる。
	D3DXVec3Normalize(&V2, &V2);

	//カメラにしたがってプレイヤーの進行方向と曲がり方を指定
	D3DXVECTOR3 t0, t1;
	t0 = V1 * m_moveSpeed.z;
	t1 = V2 * -m_moveSpeed.x;
	t0 += t1;
	m_moveSpeed.x = t0.x;
	m_moveSpeed.z = t0.z;

	//コース定義に従ったプレイヤーの回転の処理
	float L;
	D3DXVECTOR3		NV2;
	float			cos;
	D3DXVECTOR3		Back;
	D3DXVECTOR3     NV3;
	Back.x = 0.0f;
	Back.y = 0.0f;
	Back.z = -1.0f;

	D3DXVECTOR3 moveXZ = m_moveSpeed;
	moveXZ.y = 0.0f;
	L = D3DXVec3Length(&moveXZ);//m_moveSpeedのベクトルの大きさを返す、√の計算もしてくれる。
	if (L > 0)
	{
		D3DXVec3Normalize(&NV2, &moveXZ);
		D3DXVec3Cross(&NV3, &NV2, &Back);
		cos = D3DXVec3Dot(&NV2, &Back);//2つの3Dベクトルの上方向の内積を求める→V1とV2のなす角のcosθが見つかる。
		m_targetAngleY = acos(cos);
		if (NV3.y > 0)
		{
			m_targetAngleY = m_targetAngleY*-1;
		}
	}

}

void CPlayer::KeyState(){
	// デバイスが切り替わった場合は自動で切り替える
	SINSTANCE(CInputManager)->IsInputChanged(&m_pInput);

	if (m_JumpState == JUMP_STATE::J_NULL && m_pInput->IsTriggerSpace()){
		m_JumpState = JUMP_STATE::J_EINS;
		m_pAudio->PlayCue("Jump", true, this);//ジャンプSE
		m_JumpParticleTimer = 0.0f;
		m_NowJumpPower = PLAYER_JUMP_POWER;
		m_pEmitter->SetEmitFlg(true);
		D3DXVECTOR3 pos = m_transform.position;//パーティクルのposを変えるためだけの格納
		pos.y -= 0.5f;
		m_pEmitter->SetEmitPos(pos);
	}
	bool IsActive_X = static_cast<bool>(fabsf(m_pInput->GetStickL_XFloat()));
	bool IsActive_Y = static_cast<bool>(fabsf(m_pInput->GetStickL_YFloat()));
	if (IsActive_X || IsActive_Y){
		m_State = MOVE_STATE::Walk;
	}
	if (m_pInput->IsPressRightShift())
	{
		m_ShotFlg = true;
	}
}

void CPlayer::Collisions(){
	if (m_VibrationCounter > m_VibrationInterval){
		if (!m_vibration.GetIsVibration()){
			// エネミーとの当たり判定。
			CEnemyManager* EnemyManager = (SINSTANCE(CObjectManager)->FindGameObject<CEnemyManager>(_T("EnemyManager")));
			m_NearEnemy = m_LockOn.FindNearEnemy(m_transform.position);
			if (m_NearEnemy){
				D3DXVECTOR3 dist;
				dist = m_NearEnemy->GetPos() - m_transform.position;
				float R;
				R = D3DXVec3Length(&dist);//ベクトルの長さを計算。

				if (R <= 1)
				{
					m_State = MOVE_STATE::Vibration;
					m_ActiveKeyState = false;
					// プレイヤーを追いかけ続けていると画面が振動してしまう。
					m_pCamera->SetIsTarget(false);
					m_vibration.ThisVibration(&(m_transform.position), D3DXVECTOR3(0.002f, 0.0f, 0.0f), 0.5f, 0.01f);
					m_moveSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					m_pAudio->PlayCue("ta_ge_denki01", true, this);
					//return;
				}
			}

			// 炎のギミックとの当たり判定。
			for (int idx = 0;; idx++){
				string str = "firejet";
				char num[100];
				_itoa(idx, num, 10);
				str += num;
				CFireJet* firejet = SINSTANCE(CObjectManager)->FindGameObject<CFireJet>(_T(str.c_str()));
				if (firejet == nullptr){
					break;
					//return;
				}
				if (firejet->IsCollision(m_transform.position, 1.0f)){
					// プレイヤーを追いかけ続けていると画面が振動してしまう。
					m_State = MOVE_STATE::Vibration;
					m_ActiveKeyState = false;
					m_pCamera->SetIsTarget(false);
					m_vibration.ThisVibration(&(m_transform.position), D3DXVECTOR3(0.002f, 0.0f, 0.0f), 1.2f, 0.01f);
					m_moveSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//return;
				}
			}
		}
	}

	m_VibrationCounter += 1.0f / 60.0f;

	// チョコボールとの当たり判定。
	for (auto itr = m_CBManager.begin(); itr != m_CBManager.end();){
		if (*itr != NULL)
		{
			if (m_State != MOVE_STATE::Flow){
				//チョコボールに当たっているかの処理
				if ((*itr)->IsHit(m_transform.position, m_size))
				{
					//チョコボールに当たったらの処理
					ChocoHit();
				}
			}
		}
		if (m_NowCourceNo != -1){
			if (m_NowCourceNo - (*itr)->GetCourceNo() >= 5){
				(*itr)->NonActivate();
				SINSTANCE(CObjectManager)->DeleteGameObject(*itr);
				itr = m_CBManager.erase(itr);
				continue;
			}
		}
		itr++;
	}
}

void CPlayer::StateManaged()
{
	if (m_GameState != GAMEEND_ID::CLEAR)
	{
		CGameObject::Update();
	}

	//落下死ゲームオーバー処理。
	if (m_transform.position.y <= -15.0f)
	{
		m_GameState = GAMEEND_ID::OVER;
		return;
	}

	//ゲームクリア
	COURCE_BLOCK EndBlock = m_Courcedef.FindCource(m_Courcedef.GetCourceMax() - 1);
	COURCE_BLOCK nowBlock = m_Courcedef.FindCource(m_transform.position);
	m_NowCourceNo = nowBlock.blockNo;
	if (nowBlock.blockNo == EndBlock.blockNo){
		D3DXVECTOR3 LoadVec;
		LoadVec = EndBlock.startPosition - EndBlock.endPosition;
		D3DXVECTOR3 GoalToPlayerVec;
		GoalToPlayerVec = m_transform.position - EndBlock.endPosition;
		float Kyori = D3DXVec3Dot(&GoalToPlayerVec, &LoadVec);
		if (Kyori < 0.001f)
		{
			m_GameState = GAMEEND_ID::CLEAR;
			return;
		}
	}
}

bool CPlayer::IsVibration() 
{
	return m_vibration.GetIsVibration();
}

void CPlayer::BulletShot()
{
	BulletShotInterval++;
	if (BulletShotInterval % 5 == 0){

		//プレイヤーの向いているベクトルを計算
		D3DXVec3Normalize(&RV0, &RV0);
		D3DXMatrixRotationY(&Rot, m_currentAngleY);
		D3DXVec3Transform(&RV1, &RV0, &Rot);

		CPlayerBullet* bullet = new CPlayerBullet;
		bullet->Initialize();
		bullet->SetPos(m_transform.position);
		bullet->SetDir(RV1);
		bullet->SetBulletSpeed(0.5f);
		bullet->SetAudio(m_pAudio);
		m_bullets.push_back(bullet);
		m_pAudio->PlayCue("Laser", true, this);
	}
}

void CPlayer::DeleteBullet(CPlayerBullet* bullet){
	m_Deletebullets.push_back(bullet);
}

void CPlayer::ExcuteDeleteBullets(){
	vector<CPlayerBullet*>::iterator itr;
	int size = m_Deletebullets.size();
	for (int idx = 0; idx < size; idx++){
		for (itr = m_bullets.begin(); itr != m_bullets.end();){
			if (m_Deletebullets[idx] == *itr){
				(*itr)->OnDestroy();
				SAFE_DELETE(*itr);
				itr = m_bullets.erase(itr);
				break;
			}
			else{
				itr++;
			}
		}
	}
	m_Deletebullets.clear();
}

void CPlayer::ChocoHit()
{
	m_State = MOVE_STATE::Flow;
	m_ActiveKeyState = false;
	m_pEmitter->SetEmitFlg(false);
	m_AnimState = ANIMATION_STATE::WAIT;
	btRigidBody* rb = m_IsIntersect.GetRigidBody();//プレイヤーの剛体を取得
	m_IsIntersect.GetSphereShape()->setLocalScaling(btVector3(0.3f, 0.3f, 0.3f));//プレイヤーの球を小さく設定し、チョコボールに埋もれるようにしている。
	rb->setMassProps(1.0f, btVector3(0.1f, 0.1f, 0.1f)/*btVector3(0.1f, 0.1f, 0.1f)*/);//第一引数は質量、第二引数は回転のしやすさ
	m_pModel->GetAnimation()->SetAnimSpeed(1.0f);//アニメーション再生速度を設定
}
void CPlayer::EnemyBulletHit( D3DXVECTOR3 moveDir )
{
	m_State = MOVE_STATE::Fly;
	m_AnimState = ANIMATION_STATE::WAIT;
	m_ActiveKeyState = false;
	m_pAudio->PlayCue("スポッ１", false, this);
	btRigidBody* rb = m_IsIntersect.GetRigidBody();//プレイヤーの剛体を取得
	rb->setMassProps(1.0f, btVector3(0.01f, 0.01f, 0.01f));//第一引数は質量、第二引数は回転のしやすさ
	moveDir *= 750.0f;
	rb->applyForce(btVector3(moveDir.x, moveDir.y + 1000.0f, moveDir.z), btVector3(1.0f, 1.0f, 1.0f));//チョコボールに当たって吹っ飛ぶ力を設定
	rb->setAngularVelocity(btVector3(5.0f, 5.0f, 5.0f));
	m_pModel->GetAnimation()->SetAnimSpeed(2.0f);//アニメーション再生速度を設定
}
void CPlayer::RollingPlayer()
{
	btRigidBody* rb = m_IsIntersect.GetRigidBody();//プレイヤーの剛体を取得

	//物理エンジンで計算した移動をプレイヤーに反映
	btVector3 pos = rb->getWorldTransform().getOrigin();
	m_transform.position.x = pos.x();
	m_transform.position.y = pos.y();
	m_transform.position.z = pos.z();

	//物理エンジンで計算した回転をプレイヤーに反映
	btQuaternion rot = rb->getWorldTransform().getRotation();
	m_transform.angle.x = rot.x();
	m_transform.angle.y = rot.y();
	m_transform.angle.z = rot.z();
	m_transform.angle.w = rot.w();

	//ゲームオーバーになるまでの待機時間の設定
	deadTimer += 1.0f / 60.0f;
	if (deadTimer >= 2.0f){
		m_GameState = GAMEEND_ID::OVER;
	}
}

void CPlayer::DeleteChocoBall(CCBManager* mgr){
	m_DeleteChocoBall.push_back(mgr);
}

void CPlayer::ExcuteDeleteChocoBall(){
	//for (auto itr2 : m_DeleteChocoBall){
	//	for (list<CCBManager*>::iterator itr = m_CBManager.begin(); itr != m_CBManager.end();){
	//		if (*itr == itr2){
	//			(*itr)->NonActivate();
	//			SINSTANCE(CObjectManager)->DeleteGameObject(*itr);
	//			itr = m_CBManager.erase(itr);
	//		}
	//		else{
	//			itr++;
	//		}
	//	}
	//}
	//m_DeleteChocoBall.clear();
}