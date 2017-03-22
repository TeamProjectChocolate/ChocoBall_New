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
#include "CBManager.h"

CPlayer* g_player = NULL;

CPlayer::~CPlayer(){ 
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

	m_GameState = GAMEEND::ID::CONTINUE;

	m_Courcedef.SetStageID(m_StageID);
	m_Courcedef.Initialize();
	m_NowCourceNo = -1;

	// ライト関連の初期化
	this->ConfigLight();

	m_radius = 1.0f;
	//// test
	m_radius = 1.0f;
	btCollisionShape* Shape = new btSphereShape(m_radius);//ここで剛体の形状を決定


	ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), Shape, CollisionType::Player, false, 1.0f, /*true*/true,true);
	m_CollisionObject->BitMask_AllOn();
	m_CollisionObject->BitMask_Off(CollisionType::Map);
	//m_CollisionObject->BitMask_Off(CollisionType::Boss);
	//m_CollisionObject->BitMask_Off(CollisionType::Wall);


	//m_CollisionObject->BitMask_On(CollisionType::Player);
	//m_CollisionObject->BitMask_On(CollisionType::Chocoball);
	//m_CollisionObject->BitMask_On(CollisionType::Enemy);
	//m_CollisionObject->BitMask_On(CollisionType::Boss_Barrier);
	//m_CollisionObject->BitMask_On(CollisionType::Boss_Cource);
	//m_CollisionObject->BitMask_On(CollisionType::Camera);
	//m_CollisionObject->BitMask_On(CollisionType::Bullet);

	m_IsIntersect.Initialize(static_cast<btRigidBody*>(this->GetCollisionObject()));
	// あたりを無視する属性をセットしていく。
	// テスト
	m_IsIntersect.OnMask(CollisionType::Player);
	m_IsIntersect.OnMask(CollisionType::Chocoball);
	m_IsIntersect.OnMask(CollisionType::Enemy);
	m_IsIntersect.OnMask(CollisionType::Boss_Barrier);
	m_IsIntersect.OnMask(CollisionType::Boss_Cource);
	m_IsIntersect.OnMask(CollisionType::Camera);
	m_IsIntersect.OnMask(CollisionType::Bullet);
	//m_IsIntersect.OnMask(CollisionType_AttackWall);
	
	deadTimer = 0.0f;
	m_pCamera = SINSTANCE(CObjectManager)->FindGameObject<CCourceCamera>(_T("Camera"));
	CParticleEmitter* StartEmitter = CParticleEmitter::EmitterCreate(
		_T("ParticleEmitterStart"),
		PARTICLE_TYPE::STAR,
		m_transform.position,
		m_pCamera->GetCamera(),
		m_StageID,
		true,
		true
		);
	StartEmitter->SetCourceLange(2);
	//ジャンプ＆着地時の煙
	m_pEmitter = CParticleEmitter::EmitterCreate(
		_T("ParticleEmitterSmoke"),
		PARTICLE_TYPE::SMOKE,
		m_transform.position,
		m_pCamera->GetCamera(),
		m_StageID,
		false,
		true
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
	m_pModel->GetAnimation()->Play(-1,0.0f,false);
	m_State = MOVE::STATE::Wait;
	m_VibrationCounter = 0.0f;
	m_VibrationInterval = 0.5f;

	m_JumpState = JUMP_STATE::J_NULL;
	m_JumpEndCounter = 0.0f;
	m_JumpEndInterval = 0.15f;
	m_JumpStartCounter = 0.0f;
	m_JumpStartInterval = 0.04f;
	m_AnimInterpolation = 0.05f;
	m_RunningCounter = 0.0f;
	m_RunningRange = 0.5f;
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
	// メインシーンの状態を管理する処理。
	StateManaged();

	if (m_GameState == GAMEEND::ID::CONTINUE)
	{
		// 当たり判定。
		Collisions();

		// キー判定。
		if (m_ActiveKeyState){
			KeyState();
		}
		else {
			m_RunningCounter = 0.0f;
			m_IsActive_Y = m_IsActive_X = false;
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

	if (m_State != MOVE::STATE::Flow && m_State != MOVE::STATE::Fly) {
		// 上下移動処理。
		UpDownMove();
		SetRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_currentAngleY);
		bool IsJump = false;
		if (m_JumpState == JUMP_STATE::J_ZWEI) {
			IsJump = true;
		}
		//プレイヤーの位置情報更新。
		//if (static_cast<CRigidbody*>(m_CollisionObject.get())->GetIsKinematic()) {
			m_IsIntersect.Intersect(&m_transform.position, &m_moveSpeed, IsJump);
		//}
		//else {
		//	static_cast<CRigidbody*>(m_CollisionObject.get())->ApplyForce(m_moveSpeed * 10.0f);
		//}
	}

	// アニメーション再生関数を呼び出す
	m_pModel->SetCurrentAnimNo(m_AnimState);
	m_pModel->GetAnimation()->Play(m_pModel->GetCurrentAnimNo(), m_AnimInterpolation,true);

	// 影カメラのポジションをプレイヤーの真上に指定。
	SINSTANCE(CShadowRender)->SetObjectPos(m_transform.position);
	SINSTANCE(CShadowRender)->SetShadowCameraPos(m_transform.position + D3DXVECTOR3(1.0f, /*2.0f*/5.0f, 0.0f));
	//static_cast<CEM_Render*>(SINSTANCE(CRenderContext)->GetEMRender())->SetCameraPos(m_transform.position + -GetDirection() * 1.5f);

	if (m_GameState != GAMEEND::ID::CLEAR)
	{
		CGameObject::Update();
	}
}

void CPlayer::Draw(){
	CGameObject::Draw();
}

void CPlayer::ConfigLight(){
	if (!m_pLight) {
		m_pLight = new CActreLight;
	}

	// ディフューズライト(キャラライト)の向き設定(ライト1～4)
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(0, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(1, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(2, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(3, D3DXVECTOR3(1.0f, 0.0f, 0.0f));

	//static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(1, D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	//static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(2, D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	//static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(3, D3DXVECTOR3(0.0f, 0.0f, 1.0f));

	// ディフューズライト(キャラライト)の色設定(ライト1～4)
	m_pLight->SetDiffuseLightColor(0, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 0.25f));
	m_pLight->SetDiffuseLightColor(1, D3DXVECTOR4(0.5f, 0.5f,0.5f, 0.25f));
	m_pLight->SetDiffuseLightColor(2, D3DXVECTOR4(0.5f, 0.5f,0.5f, 0.25f));
	m_pLight->SetDiffuseLightColor(3, D3DXVECTOR4(0.5f,0.5f, 0.5f, 0.25f));

	// アンビエントライト(環境光)の強さ設定
	m_pLight->SetAmbientLight(D3DXVECTOR3(0.1f, 0.1f, 0.1f));

	// リムライトの色と向き設定。
	m_pLight->SetLimLightColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pLight->SetLimLightDirection(D3DXVECTOR3(0.0f, 0.0f, -1.0f));

	CGameObject::ConfigLight();

	//// 現在共通のライトに設定。
	//SINSTANCE(CRenderContext)->SetCurrentLight(&m_light);
}

void CPlayer::OnTriggerStay(btCollisionObject* pCollision) {
	if (pCollision->getUserIndex() == static_cast<int>(CollisionType::ChocoballTrigger)) {
		// 当たったものがチョコボールトリガーならチョコボール生成。
		CCBManager* mgr = (CCBManager*)pCollision->getUserPointer();
		if (!mgr->GetAlive()) {
			m_pAudio->PlayCue("Chocoball", true, nullptr);//チョコ落下Audio
		}
	}
}

void CPlayer::OnCollisionStay(btCollisionObject* pCollision) {

}

void CPlayer::MoveStateManaged(){
	switch (m_State){
	case MOVE::STATE::Wait:
		//m_pCamera->SetTargetViewAngle(D3DXToRadian(30.0f));
		m_AnimState = ANIMATION_STATE::WAIT;
		m_ActiveKeyState = true;
		break;
	case MOVE::STATE::Walk:
		//m_pCamera->SetTargetViewAngle(D3DXToRadian(45.0f));
		Move();
		m_AnimState = ANIMATION_STATE::WALK;
		m_ActiveKeyState = true;
		m_State = MOVE::STATE::Wait;
		break;
	case MOVE::STATE::Dash:
		//m_pCamera->SetTargetViewAngle(D3DXToRadian(45.0f));
		Move();
		m_ActiveKeyState = true;
		m_State = MOVE::STATE::Wait;
		break;
	case MOVE::STATE::Vibration:
		m_vibration.Update();
		m_VibrationCounter = 0.0f;
		// 振動処理実行後、振動が終了しているか
		if (!m_vibration.GetIsVibration()){
			m_State = MOVE::STATE::Wait;
			m_ActiveKeyState = true;
			m_pCamera->SetIsTarget(true);
		}
		else{
			m_AnimState = ANIMATION_STATE::DAMAGE;
			m_ActiveKeyState = false;
		}
		break;
	case MOVE::STATE::Flow:
		m_pCamera->SetIsTarget(true);
		m_pCamera->GetCamera()->SetTarget(m_transform.position);
		m_AnimState = ANIMATION_STATE::WALK;
		m_ActiveKeyState = false;
		m_JumpState = JUMP_STATE::J_NULL;
		//m_transform.position.y = 0.0f;
		RollingPlayer();
		break;
	case MOVE::STATE::Fly:
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
		float work = m_RunningCounter / m_RunningRange;
		m_moveSpeed.z = Y * MOVE_SPEED * work;
		isTurn = true;
	}

	//左右の動き
	if (fabsf(X) > 0.0f)
	{
		float work = m_RunningCounter / m_RunningRange;
		m_moveSpeed.x = X * MOVE_SPEED * work;
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
		//if (!m_IsIntersect.IsHitGround()) {
		//	m_JumpState = JUMP_STATE::J_EINS;
		//	m_NowJumpPower = PLAYER_JUMP_POWER / 2;
		//}
		if (m_IsIntersect.GetIsHitGround()) {
			m_NowJumpPower = 0.0f;
			m_moveSpeed.y = 0.0f;
		}
		else {
			m_JumpState = JUMP_STATE::J_ZWEI;
			m_NowJumpPower = 0.0f;
		}
		break;
	case JUMP_STATE::J_EINS:
		if (m_IsIntersect.GetIsHitGround()) {
			m_ActiveKeyState = false;
			m_AnimState = ANIMATION_STATE::JUMP_START;
			m_JumpStartCounter += 1.0f / 60.0f;
			if (m_JumpStartCounter >= m_JumpStartInterval) {
				m_JumpStartCounter = 0.0f;
				m_JumpState = JUMP_STATE::J_ZWEI;
				m_NowJumpPower = PLAYER_JUMP_POWER;
			}
		}
		else {
			m_JumpState = JUMP_STATE::J_NULL;
		}
		break;
	case JUMP_STATE::J_ZWEI:
		m_RunningCounter = m_RunningRange;
		if (m_State != MOVE::STATE::Vibration) {
			m_ActiveKeyState = true;
			m_AnimState = ANIMATION_STATE::JUMP_NOW;
		}
		else {
			m_AnimState = ANIMATION_STATE::DAMAGE;
		}
		//着地しているか判定(着地時はtrue,そうでなければfalse)。
		if (m_IsIntersect.GetIsHitGround())
		{
			// 着地した瞬間。
			m_JumpState = JUMP_STATE::J_DREI;
			m_pEmitter->SetEmitFlg(true);
			m_pAudio->PlayCue("Landing", true, this);
			m_JumpEndCounter = 0.0f;
			if (m_IsActive_X || m_IsActive_Y) {
				m_RunningCounter = 0.5f * m_RunningRange;
			}
		}
		break;
	case JUMP_STATE::J_DREI:
		if (m_State != MOVE::STATE::Vibration) {
			m_AnimState = ANIMATION_STATE::JUMP_END;
		}
		// 自分の周囲にパーティクル発生
		// Setすると常にプレイヤーの場所にパーティクルの発生する
		D3DXVECTOR3 pos = m_transform.position;//パーティクルのposを変えるためだけの格納
		pos.y = pos.y - 0.7f;
		m_pEmitter->SetEmitPos(pos);
		m_NowJumpPower = 0.0f;
		if (m_IsActive_X || m_IsActive_Y) {
			m_AnimState = ANIMATION_STATE::WALK;
			m_JumpEndCounter = 0.0f;
			m_JumpState = JUMP_STATE::J_NULL;
			break;
		}
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

	if ((m_JumpState == JUMP_STATE::J_NULL || m_JumpState == JUMP_STATE::J_DREI) && m_pInput->IsTriggerSpace()){
		m_JumpState = JUMP_STATE::J_EINS;
		m_JumpEndCounter = 0.0f;
		m_JumpStartCounter = 0.0f;
		m_pAudio->PlayCue("Jump", true, this);//ジャンプSE
		m_JumpParticleTimer = 0.0f;
		m_pEmitter->SetEmitFlg(true);
		D3DXVECTOR3 pos = m_transform.position;//パーティクルのposを変えるためだけの格納
		pos.y -= 0.5f;
		m_pEmitter->SetEmitPos(pos);
	}
	m_IsActive_X = static_cast<bool>(fabsf(m_pInput->GetStickL_XFloat()));
	m_IsActive_Y = static_cast<bool>(fabsf(m_pInput->GetStickL_YFloat()));

	if (m_IsActive_X || m_IsActive_Y){
		if (m_RunningCounter <= m_RunningRange) {
			m_RunningCounter += 1.0f / 60.0f;
		}
		m_State = MOVE::STATE::Walk;
	}
	else {
		m_RunningCounter = 0.0f;
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
					EnemyHit();
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
				}
				if (firejet->IsCollision(m_transform.position, 1.0f)){
					// プレイヤーを追いかけ続けていると画面が振動してしまう。
					m_State = MOVE::STATE::Vibration;
					m_ActiveKeyState = false;
					m_pCamera->SetIsTarget(false);
					m_vibration.ThisVibration(&(m_transform.position), D3DXVECTOR3(0.002f, 0.0f, 0.0f), 1.2f, 0.01f);
					m_moveSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					break;
				}
			}
		}
	}

	m_VibrationCounter += 1.0f / 60.0f;
}

void CPlayer::StateManaged()
{
	//if (m_GameState != GAMEEND_ID::CLEAR)
	//{
	//	CGameObject::Update();
	//}

	//落下死ゲームオーバー処理。
	if (m_transform.position.y <= -15.0f)
	{
		m_GameState = GAMEEND::ID::OVER;
		return;
	}

	//ゲームクリア
	Cource::COURCE_BLOCK EndBlock = m_Courcedef.FindCource(m_Courcedef.GetCourceMax() - 1);
	Cource::COURCE_BLOCK nowBlock = m_Courcedef.FindCource(m_transform.position);
	m_NowCourceNo = nowBlock.blockNo;
	if (nowBlock.blockNo == EndBlock.blockNo){
		D3DXVECTOR3 LoadVec;
		LoadVec = EndBlock.startPosition - EndBlock.endPosition;
		D3DXVECTOR3 GoalToPlayerVec;
		GoalToPlayerVec = m_transform.position - EndBlock.endPosition;
		float Kyori = D3DXVec3Dot(&GoalToPlayerVec, &LoadVec);
		if (Kyori < 0.001f)
		{
			m_GameState = GAMEEND::ID::CLEAR;
			return;
		}
	}
}

void CPlayer::EnemyHit() {
	if (m_State != MOVE::STATE::Vibration && m_State != MOVE::STATE::Flow && m_State != MOVE::STATE::Fly) {
		m_State = MOVE::STATE::Vibration;
		m_ActiveKeyState = false;
		// プレイヤーを追いかけ続けていると画面が振動してしまう。
		m_pCamera->SetIsTarget(false);
		m_vibration.ThisVibration(&(m_transform.position), D3DXVECTOR3(0.002f, 0.0f, 0.0f), 0.5f, 0.01f);
		m_moveSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_pAudio->PlayCue("ta_ge_denki01", true, this);
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

		CPlayerBullet* bullet = SINSTANCE(CObjectManager)->GenerationObject<CPlayerBullet>(_T("PlayerBullet"), OBJECT::PRIORTY::OBJECT3D, false);
		bullet->Initialize();
		bullet->SetStartPos(m_transform.position);
		bullet->SetDir(static_cast<D3DXVECTOR3>(RV1));
		bullet->SetBulletSpeed(0.5f);
		bullet->SetAudio(m_pAudio);
		bullet->SetRange(20.0f);
		m_pAudio->PlayCue("Laser", true, this);
	}
}

void CPlayer::ChocoHit()
{
	if (m_State != MOVE::STATE::Flow || m_State != MOVE::STATE::Fly) {
		m_State = MOVE::STATE::Flow;
		m_ActiveKeyState = false;
		m_pEmitter->SetEmitFlg(false);
		m_AnimState = ANIMATION_STATE::WALK;
		btRigidBody* rb = m_IsIntersect.GetRigidBody();//プレイヤーの剛体を取得
		m_IsIntersect.GetSphereShape()->setLocalScaling(btVector3(0.3f, 0.3f, 0.3f));//プレイヤーの球を小さく設定し、チョコボールに埋もれるようにしている。
		rb->setMassProps(1.0f, btVector3(0.1f, 0.1f, 0.1f)/*btVector3(0.1f, 0.1f, 0.1f)*/);//第一引数は質量、第二引数は回転のしやすさ
		m_pModel->GetAnimation()->SetAnimSpeed(5.0f);//アニメーション再生速度を設定
	}
}
void CPlayer::EnemyBulletHit( D3DXVECTOR3 moveDir )
{
	m_State = MOVE::STATE::Fly;
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


	m_pModel->GetAnimation()->SetAnimSpeed(2.0f);//アニメーション再生速度を設定

	btRigidBody* rb = m_IsIntersect.GetRigidBody();//プレイヤーの剛体を取得
	m_IsIntersect.GetSphereShape()->setLocalScaling(btVector3(0.3f, 0.3f, 0.3f));//プレイヤーの球を小さく設定し、チョコボールに埋もれるようにしている。
	rb->setMassProps(1.0f, btVector3(0.1f, 0.1f, 0.1f)/*btVector3(0.1f, 0.1f, 0.1f)*/);//第一引数は質量、第二引数は回転のしやすさ

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
		m_GameState = GAMEEND::ID::OVER;
	}
}
