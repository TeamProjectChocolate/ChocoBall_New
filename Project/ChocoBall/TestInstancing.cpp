#include "stdafx.h"
#include "TestInstancing.h"
#include "RenderContext.h"
#include "Camera.h"
#include "ShadowRender.h"
#include "Player.h"
#include "ObjectManager.h"

#define RANDOM_MARGIN 32767.0
#define MARGIN_BAIRITU 2.0
#define RANDOM_MARGIN_BAIRITU 1000.0

CTestInstancing::CTestInstancing()
{
}


CTestInstancing::~CTestInstancing()
{
	for (D3DXMATRIX* idx : pWorlds){
		SAFE_DELETE(idx);
	}
}

void CTestInstancing::CreateMatrixBuffer(int MaxNum){
	//���[���h�s��p�̃o�b�t�@�̍쐬�B
	if (m_WorldMatrixBuffer == nullptr){
		(*graphicsDevice()).CreateVertexBuffer(sizeof(D3DXMATRIX)* MaxNum, 0, 0, D3DPOOL_MANAGED, &m_WorldMatrixBuffer, 0);
	}
}

void CTestInstancing::CopyMatrixToVertexBuffer()
{
	D3DVERTEXBUFFER_DESC desc;
	m_WorldMatrixBuffer->GetDesc(&desc);
	D3DXMATRIX* pData;
	m_WorldMatrixBuffer->Lock(0, desc.Size, (void**)&pData, D3DLOCK_DISCARD);

	for (int i = 0; i < m_worldMatrix.size(); i++) {
		*pData = m_worldMatrix[i];
		pData++;
	}
	m_WorldMatrixBuffer->Unlock();
}

void CTestInstancing::Initialize(){
	
	model.SetFileName(_T("image/ball.x"));
	model.Initialize();
	model.m_alpha = 1.0f;
	model.m_luminance = 0.0f;

	m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/ModelShader.hlsl"));	// �g�p����shader�t�@�C�����w��(�f�t�H���g)
	m_hEyePosition = m_pEffect->GetParameterByName(nullptr, "EyePosition");
	m_hWorldMatrixArray = m_pEffect->GetParameterByName(nullptr, "g_WorldMatrixArray");
	m_hluminance = m_pEffect->GetParameterByName(nullptr, "g_luminance");
	m_hnumBone = m_pEffect->GetParameterByName(nullptr, "g_numBone");
	m_hAlpha = m_pEffect->GetParameterByName(nullptr, "Alpha");
	m_hRota = m_pEffect->GetParameterByName(nullptr, "Rota");
	m_hWorld = m_pEffect->GetParameterByName(nullptr, "World");
	m_hTexture = m_pEffect->GetParameterByName(nullptr, "g_Texture");
	m_hShadowMap = m_pEffect->GetParameterByName(nullptr, "g_ShadowMap");

	//�C���X�^���V���O�`��p�̏������B
	D3DVERTEXELEMENT9 declElement[MAX_FVF_DECL_SIZE];
	ANIMATION::D3DXMESHCONTAINER_DERIVED* container = model.GetImage_3D()->GetContainer();
	// ���݂̒��_�錾�I�u�W�F�N�g���擾(�����_�錾�I�u�W�F�N�g�Ƃ́A���_�f�[�^���ǂ̂悤�ȍ\���ɂȂ��Ă��邩��錾�������̂ł���)
	container->MeshData.pMesh->GetDeclaration(declElement);
	// ���_�̐錾�̏I�[��T��
	int elementIndex = 0;
	while (true) {
		if (declElement[elementIndex].Type == D3DDECLTYPE_UNUSED) {
			//�I�[�𔭌��B
			//��������C���X�^���V���O�p�̒��_���C�A�E�g�𖄂ߍ��ށB
			WORD offset = 0;
			
			for (BYTE num = 0; num < 4; num++){
				// WORLD�s���float4x4�Ȃ̂ŁA1�s����float4�Œ�`���Ă���
				declElement[elementIndex] = {
					1,	// �X�g���[���ԍ�
					offset,	/*offset�v�f(�e�X�g���[���̐擪���牽�Ԗڂɒ�`����Ă��邩��\������[sizeof(�^��) * ����]�̏�����)*/
					D3DDECLTYPE_FLOAT4,	// ���_�����ǂ̌����g�p���Đ錾���邩(�s���float4�̗v�f���l�g�p���č\������)
					D3DDECLMETHOD_DEFAULT, // �e�b�Z���[�V����(�|���S������)�̕��@���w��B���ʃe�b�Z���[�V�����͂��܂�g��Ȃ����߁A�f�t�H���g��(D3DDECLMETHOD_DEFAULT)�ŏ\��
					D3DDECLUSAGE_TEXCOORD,	// usage�v�f
					static_cast<BYTE>(num + 1) /*UsageIndex�v�f(Usage���d�����Ă�����̂ɂ��āA�ŗL�ԍ���U���Ď��ʂ������)*/
				};
				offset += sizeof(float)* 4;
				elementIndex++;
			}
			declElement[elementIndex] = D3DDECL_END();	// ���_�錾�̏I�[(�ԕ�)��ݒ�(���m�ɂ́A���_�錾�̍Ō�̗v�f�����������Ă���)
			break;
		}
		elementIndex++;
	}
	//��Ő錾�������_���Œ��_��`���쐬�B
	(*graphicsDevice()).CreateVertexDeclaration(declElement, &m_VertexDecl);
	this->CreateMatrixBuffer(MAX_INSTANCE);

	for (int idx = 0; idx < MAX_INSTANCE; idx++){
		D3DXMATRIX* pWorldMat = new D3DXMATRIX;
		D3DXMatrixIdentity(pWorldMat);
		m_worldMatrix.push_back(*pWorldMat);
		pWorlds.push_back(pWorldMat);
		float rndom = (static_cast<float>(rand()) / RANDOM_MARGIN) * MARGIN_BAIRITU;
		margin[idx].x = rndom - (1.0f * MARGIN_BAIRITU / 2.0f);
		rndom = (static_cast<float>(rand()) / RANDOM_MARGIN) * MARGIN_BAIRITU;
		margin[idx].y = rndom - (1.0f * MARGIN_BAIRITU / 2.0f);
		rndom = (static_cast<float>(rand()) / RANDOM_MARGIN) * MARGIN_BAIRITU;
		margin[idx].z = rndom - (1.0f * MARGIN_BAIRITU/ 2.0f);

		rnd[idx] = D3DXVECTOR3(0.0f,0.0f,0.0f);
		TargetRnd[idx] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		flg[idx] = false;
	}

	m_TimeCounter = 0.0f;
	m_IntervalTime = 1.0f;
}

void CTestInstancing::Update(){
	CPlayer* player = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));
	if (player == nullptr){
		return;
	}
	D3DXVECTOR3 position = player->GetPos();

	for (int idx = 0; idx < MAX_INSTANCE; idx++){
		m_worldMatrix[idx].m[3][0] = position.x;
		m_worldMatrix[idx].m[3][1] = position.y;
		m_worldMatrix[idx].m[3][2] = position.z;
		m_worldMatrix[idx].m[3][3] = 1.0f;

		if (flg[idx]){
			if (D3DXVec3Length(&rnd[idx]) <= 0.0001f){
				rnd[idx] = D3DXVECTOR3(0.0f,0.0f,0.0f);
				flg[idx] = false;
				m_TimeCounter = 0.0f;
			}
			else{
				rnd[idx] -= rnd[idx] / 10.0f;
			}
		}
		else if (m_IntervalTime <= m_TimeCounter){
			if (rnd[idx].x == 0.0f && rnd[idx].y == 0.0f && rnd[idx].z == 0.0f){
				TargetRnd[idx].x = (static_cast<float>(rand()) / RANDOM_MARGIN) * RANDOM_MARGIN_BAIRITU;
				TargetRnd[idx].x = TargetRnd[idx].x - (1.0f * RANDOM_MARGIN_BAIRITU / 2.0f);
				rnd[idx].x = TargetRnd[idx].x / 10.0f;
				TargetRnd[idx].y = (static_cast<float>(rand()) / RANDOM_MARGIN) * RANDOM_MARGIN_BAIRITU;
				TargetRnd[idx].y = TargetRnd[idx].y - (1.0f * RANDOM_MARGIN_BAIRITU / 2.0f);
				rnd[idx].y = TargetRnd[idx].y / 10.0f;
				TargetRnd[idx].z = (static_cast<float>(rand()) / RANDOM_MARGIN) * RANDOM_MARGIN_BAIRITU;
				TargetRnd[idx].z = TargetRnd[idx].z - (1.0f * RANDOM_MARGIN_BAIRITU / 2.0f);
				rnd[idx].z = TargetRnd[idx].z / 10.0f;
			}
			else{
				if (fabsf(D3DXVec3Length(&TargetRnd[idx])) < fabsf(D3DXVec3Length(&rnd[idx]))){
					rnd[idx] = TargetRnd[idx];
					TargetRnd[idx] = D3DXVECTOR3(0.0f,0.0f,0.0f);
					flg[idx] = true;
				}
				else{
					rnd[idx] += rnd[idx] / 10.0f;
				}
			}
		}

		m_worldMatrix[idx].m[3][0] += margin[idx].x + (rnd[idx].x * 0.5f);
		m_worldMatrix[idx].m[3][1] += margin[idx].y + (rnd[idx].y * 0.5f);
		m_worldMatrix[idx].m[3][2] += margin[idx].z + (rnd[idx].z * 0.5f);
	}
	m_TimeCounter += 1.0f / 60.0f;
}


void CTestInstancing::Draw(){
	ANIMATION::D3DXMESHCONTAINER_DERIVED* container = model.GetImage_3D()->GetContainer();

	(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pEffect->SetTechnique("NonAnimationInstancing_SFresnel");
	m_pEffect->Begin(nullptr, D3DXFX_DONOTSAVESHADERSTATE);
	m_pEffect->BeginPass(0);	//�p�X�̔ԍ����w�肵�Ăǂ̃p�X���g�p���邩�w��

	//// �����x�L����
	//(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���݂̃v���W�F�N�V�����s��ƃr���[�s����V�F�[�_�[�ɓ]��
	CCamera* camera = SINSTANCE(CRenderContext)->GetCurrentCamera();
	if (camera){
		camera->SetCamera(m_pEffect);
	}
	CLight* light = SINSTANCE(CRenderContext)->GetCurrentLight();
	if (light){
		light->SetLight(m_pEffect);
	}
	D3DXVECTOR3 pos;
	if (camera){
		pos = camera->GetPos();
	}
	else{
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	// ���_���V�F�[�_�[�ɓ]��
	m_pEffect->SetVector(m_hEyePosition, reinterpret_cast<D3DXVECTOR4*>(&pos));


	SINSTANCE(CShadowRender)->SetShadowCamera(m_pEffect);

	m_pEffect->SetFloat(m_hAlpha, model.m_alpha);
	m_pEffect->SetFloat(m_hluminance, model.m_luminance);
	m_pEffect->SetTexture(m_hShadowMap, SINSTANCE(CShadowRender)->GetTexture());	// �e�N�X�`�������Z�b�g

	m_pEffect->SetTexture(m_hTexture, container->ppTextures[0]);	// �e�N�X�`�������Z�b�g

	// ���[���h�g�����X�t�H�[��(��΍��W�ϊ�)
	// ���[���h�s�񐶐�


	// �C���X�^���V���O�`��
	{
		//DrawSubset���g�p����ƃC���X�^���V���O�`�悪�s���Ȃ��̂�
		//g_pMesh���璸�_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�����������Ă��āA���ڕ`�悷��B
		LPDIRECT3DVERTEXBUFFER9 vb;
		LPDIRECT3DINDEXBUFFER9 ib;
		container->MeshData.pMesh->GetVertexBuffer(&vb);
		container->MeshData.pMesh->GetIndexBuffer(&ib);

		DWORD fvf = container->MeshData.pMesh->GetFVF();
		DWORD stride = D3DXGetFVFVertexSize(fvf);

		(*graphicsDevice()).SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | MAX_INSTANCE);
		(*graphicsDevice()).SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

		(*graphicsDevice()).SetVertexDeclaration(m_VertexDecl);

		(*graphicsDevice()).SetStreamSource(0, vb, 0, stride);							//���_�o�b�t�@���X�g���[��0�Ԗڂɐݒ�
		(*graphicsDevice()).SetStreamSource(1, m_WorldMatrixBuffer, 0, sizeof(D3DXMATRIX));	//���[���h�s��p�̃o�b�t�@���X�g���[��1�Ԗڂɐݒ�B
		//���[���h�s��𒸓_�o�b�t�@�ɃR�s�[�B
		CopyMatrixToVertexBuffer();
		(*graphicsDevice()).SetIndices(ib);
		m_pEffect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B
		(*graphicsDevice()).DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, container->MeshData.pMesh->GetNumVertices(), 0, container->MeshData.pMesh->GetNumFaces());
		vb->Release();
		ib->Release();
	}
	m_pEffect->EndPass();
	m_pEffect->End();
}