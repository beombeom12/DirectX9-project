#include "stdafx.h"
#include "ImGuiMgr.h"
#include "ImGuizmo.h"
#include "imgui.h"

#include "Export_Function.h"
#include "Terrain.h"
#include "Texture.h"
#include "TestCube.h"
#include "DynamicCamera.h"
#include "ToolScene.h"

#include "ObjectCube.h"

IMPLEMENT_SINGLETON(CImGuiMgr)

ImGuiTextBuffer CImGuiMgr::log;

// 창을 위한 bool 변수들 이것을 통하여 껐다 켰다 할 수 있음
bool CImGuiMgr::Show_Main_Menu_Window = false;
bool CImGuiMgr::Show_Terrain_Window = true;
bool CImGuiMgr::Show_Player_Window = false;
bool CImGuiMgr::Show_Monster_Window = false;
bool CImGuiMgr::Show_Cube_Tool = false;
bool CImGuiMgr::Show_Camera_Tool = false;
bool CImGuiMgr::Show_Object_Tool = false;

_int CImGuiMgr::m_iInterval = 100;
_int CImGuiMgr::m_iWidth = 100;
_int CImGuiMgr::m_iDepth = 1;

static _int iObjectNum;

ImVec4 CImGuiMgr::clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

CImGuiMgr::CImGuiMgr()
{

}

CImGuiMgr::~CImGuiMgr()
{
	Free();
}

void CImGuiMgr::TransformEdit(CCamera* pCamera, CTransform* pTransform, _bool& Window)
{
	ImGui::Begin("Transform");
	ImGuizmo::BeginFrame();
	static float snap[3] = { 1.f, 1.f, 1.f };
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	if (ImGui::IsKeyPressed(90))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(69))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(82)) // r Key
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	if (pTransform == nullptr)
	{
		ImGui::Text("Object Delete or nullptr");
		ImGui::End();
		return;
	}

	// 이동 행렬, 자전 행렬, 스케일 행렬
	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	_matrix matWorld = pTransform->m_matWorld;

	ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Tr", matrixTranslation);
	ImGui::InputFloat3("Rt", matrixRotation);
	ImGui::InputFloat3("Sc", matrixScale);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matWorld);

	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}

	static bool useSnap(false);
	if (ImGui::IsKeyPressed(83))
		useSnap = !useSnap;
	ImGui::Checkbox("##something", &useSnap);
	ImGui::SameLine();
	switch (mCurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		ImGui::InputFloat3("Snap", &snap[0]);
		break;
	case ImGuizmo::ROTATE:
		ImGui::InputFloat("Angle Snap", &snap[0]);
		break;
	case ImGuizmo::SCALE:
		ImGui::InputFloat("Scale Snap", &snap[0]);
		break;
	}

	if (ImGui::Button("Close"))
	{
		Window = false;
	}

	_matrix matId;
	D3DXMatrixIdentity(&matId);

	ImGuiIO& io = ImGui::GetIO();
	RECT rt;
	GetClientRect(g_hWnd, &rt);
	POINT lt{ rt.left, rt.top };
	ClientToScreen(g_hWnd, &lt);
	ImGuizmo::SetRect(float(lt.x), float(lt.y), float(io.DisplaySize.x), float(io.DisplaySize.y));

	ImGuizmo::Manipulate(pCamera->GetView(), pCamera->GetProj(), mCurrentGizmoOperation, mCurrentGizmoMode, matWorld, NULL, useSnap ? &snap[0] : NULL);

	pTransform->m_matWorld = matWorld;
	ImGuizmo::DecomposeMatrixToComponents(matWorld, matrixTranslation, matrixRotation, matrixScale);
	matrixRotation[0] = D3DXToRadian(matrixRotation[0]);
	matrixRotation[1] = D3DXToRadian(matrixRotation[1]);
	matrixRotation[2] = D3DXToRadian(matrixRotation[2]);
	memcpy(&pTransform->m_vInfo[INFO_POS], matrixTranslation, sizeof(matrixTranslation));
	memcpy(&pTransform->m_vAngle, matrixRotation, sizeof(matrixRotation));
	memcpy(&pTransform->m_vScale, matrixScale, sizeof(matrixScale));

	ImGui::End();
}

void CImGuiMgr::LoggerWindow()
{
	ImGui::Begin("Example: Long text display");
	static int test_type = 0;
	static int lines = 0;
	ImGui::Text("Printing unusually long amount of text.");
	ImGui::Combo("Test type", &test_type,
		"Single call to TextUnformatted()\0"
		"Multiple calls to Text(), clipped\0"
		"Multiple calls to Text(), not clipped (slow)\0");
	ImGui::Text("Buffer contents: %d lines, %d bytes", lines, log.size());
	if (ImGui::Button("Clear")) { log.clear(); lines = 0; }
	ImGui::SameLine();
	// if (ImGui::Button("Add 1000 lines"))
	// {
	//     for (int i = 0; i < 1000; i++)
	//         log.appendf("%i The quick brown fox jumps over the lazy dog\n", lines + i);
	//     lines += 1000;
	// }
	ImGui::BeginChild("Log");
	switch (test_type)
	{
	case 0:
		// Single call to TextUnformatted() with a big buffer
		ImGui::TextUnformatted(log.begin(), log.end());
		break;
	case 1:
	{
		// Multiple calls to Text(), manually coarsely clipped - demonstrate how to use the ImGuiListClipper helper.
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGuiListClipper clipper;
		clipper.Begin(lines);
		while (clipper.Step())
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
				ImGui::Text("%i The quick brown fox jumps over the lazy dog", i);
		ImGui::PopStyleVar();
		break;
	}
	case 2:
		// Multiple calls to Text(), not clipped (slow)
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		for (int i = 0; i < lines; i++)
			ImGui::Text("%i The quick brown fox jumps over the lazy dog", i);
		ImGui::PopStyleVar();
		break;
	}
	ImGui::EndChild();
	ImGui::End();
}

// 창 생성 함수
void CImGuiMgr::WindowLayOut()
{
	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Terrain_Tool", &Show_Terrain_Window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("PlayerTool", &Show_Player_Window);
	ImGui::Checkbox("Main_MenuTool", &Show_Main_Menu_Window);
	ImGui::Checkbox("Cube_Tool", &Show_Cube_Tool);
	ImGui::Checkbox("MonsterTool", &Show_Monster_Window);
	ImGui::Checkbox("CameraTool", &Show_Camera_Tool);
	ImGui::Checkbox("ObjectTool", &Show_Object_Tool);

	if (ImGui::Button("Clear"))
	{
		Show_Player_Window = false;
		Show_Terrain_Window = false;
		Show_Main_Menu_Window = false;
		Show_Cube_Tool = false;
		Show_Monster_Window = false;
		Show_Camera_Tool = false;
		Show_Object_Tool = false;
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void CImGuiMgr::CreateWall(LPDIRECT3DDEVICE9 pGrahicDev, CScene* pScene, CCamera* pCam)
{
	if (!Show_Cube_Tool)
		return;

	ImGui::Begin("Cube Settings");

	ImGui::Text("this is Transform_ButtonMenu");

	static _int iSaveStageNum;
	static _int iLoadStageNum;

	ImGui::RadioButton("Stage1", &iSaveStageNum, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Stage2", &iSaveStageNum, 2);
	ImGui::SameLine();
	ImGui::RadioButton("Stage3", &iSaveStageNum, 3);
	if (ImGui::Button("Save"))
	{
		if (iSaveStageNum == 1)
		{
			Save_Transform(pScene, L"../../Data/Map1.dat");
		}
		if (iSaveStageNum == 2)
		{
			Save_Transform(pScene, L"../../Data/Map2.dat");
		}
		if (iSaveStageNum == 3)
		{
			Save_Transform(pScene, L"../../Data/Map3.dat");
		}
	}
	ImGui::RadioButton("Load_Stage1", &iLoadStageNum, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Load_Stage2", &iLoadStageNum, 2);
	ImGui::SameLine();
	ImGui::RadioButton("Load_Stage3", &iLoadStageNum, 3);
	if (ImGui::Button("Load"))
	{
		if (iLoadStageNum == 1)
		{
			Load_Transform(pGrahicDev, pScene, L"../../Data/Map1.dat");
		}
		if (iLoadStageNum == 2)
		{
			Load_Transform(pGrahicDev, pScene, L"../../Data/Map2.dat");
		}
		if (iLoadStageNum == 3)
		{
			Load_Transform(pGrahicDev, pScene, L"../../Data/Map3.dat");
		}
	}

	if (ImGui::CollapsingHeader("Cube Create & Chose Button", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Create"))
		{
			m_bCubeCreateCheck = true;
			m_bCubeSelectCheck = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("ChoseCube"))
		{
			m_bCubeSelectCheck = true;
			m_bCubeCreateCheck = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			CLayer* MyLayer = pScene->Get_Layer(L"Layer_Wall");
			MyLayer->Delete_GameObject(m_CurrentSelectGameObjectObjKey.c_str());
		}
	}

	CTransform * pTranscom = nullptr;
	if (m_bCubeCreateCheck)
	{
		ImGui::Text("if double click Create Cube");
		if (ImGui::IsMouseDoubleClicked(0))
		{
			ImVec2 temp = ImGui::GetMousePos();
			CGameObject *pGameObject = nullptr;

			_tchar* szObjTag = new _tchar[20];

			wstring t = L"Test%d";
			wsprintfW(szObjTag, t.c_str(), m_iIndex);

			NameList.push_back(szObjTag);

			pGameObject = CTestCube::Create(pGrahicDev, int(temp.x), int(temp.y));
			NULL_CHECK_RETURN(pGameObject, );

			CLayer* pCubelayer = pScene->Get_Layer(L"Layer_Wall");

			FAILED_CHECK_RETURN(pCubelayer->Add_GameObject(szObjTag, pGameObject), );

			++m_iIndex;
		}
	}

	if (m_bCubeSelectCheck)
	{
		if (ImGui::IsMouseDoubleClicked(0))
		{
			CLayer* MyLayer = pScene->Get_Layer(L"Layer_Wall");

			map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();

			for (auto iter = test.begin(); iter != test.end(); ++iter)
			{
				if (dynamic_cast<CTestCube*>(iter->second)->Set_SelectGizmo())
				{
					pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
					m_CurrentSelectGameObjectObjKey = iter->first;
				}
			}
		}
	}

	CTestCube* pGameObject = dynamic_cast<CTestCube*>(Engine::Get_GameObject(L"Layer_Wall", m_CurrentSelectGameObjectObjKey.c_str()));
	if (pGameObject)
	{
		ImGui::NewLine();
		if (ImGui::CollapsingHeader("Options", ImGuiTreeNodeFlags_DefaultOpen))
		{
			static _bool	bWireFrame = false;
			if (ImGui::Checkbox("WireFrame", &bWireFrame))
				pGameObject->Set_WireFrame(bWireFrame);
		}

		ImGui::NewLine();
		if (ImGui::CollapsingHeader("Tile Texture", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// 큐브 텍스처 가져와서 사용하겠다.
			CTexture* pTextureCom = dynamic_cast<CTexture*>(pGameObject->Get_Component(L"Proto_CubePlayerTexture", ID_STATIC));

			vector<IDirect3DBaseTexture9*> vecTexture = pTextureCom->Get_Texture();

			for (_uint i = 0; i < vecTexture.size(); ++i)
			{
				if (ImGui::ImageButton((void*)vecTexture[i], ImVec2(60.f, 60.f)))
				{
					pGameObject->Set_DrawTexIndex(i);
				}
				if (i == 0 || (i + 1) % 6)
					ImGui::SameLine();
			}
		}
	}
	else
	{
		ImGui::End();
		return;
	}

	TransformEdit(pCam, m_pSelectedTransform, Show_Cube_Tool);

	// 셀렉버튼을 위한것
	if (pTranscom != nullptr)
		m_pSelectedTransform = pTranscom;

	ImGui::End();
}

void CImGuiMgr::Save_Transform(CScene* pScene, wstring strDirectory)
{
	HANDLE      hFile = CreateFile(strDirectory.c_str(),
		// 파일의 경로와 이름
		GENERIC_WRITE,         // 파일 접근 모드 (GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL,               // 공유 방식(파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가)    
		NULL,               // 보안 속성(NULL을 지정하면 기본값 상태)
		CREATE_ALWAYS,         // CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING  : 파일이 있을 경우에만 열기
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성(읽기 전용, 숨김 등) : FILE_ATTRIBUTE_NORMAL : 아무런 속성이 없는 파일
		NULL);               // 생성될 파일의 속성을 제공할 템플릿 파일(안쓰니깐 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return;
	}

	CLayer* MyLayer = pScene->Get_Layer(L"Layer_Wall");
	DWORD   dwByte = 0;

	map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();
	for (auto iter = test.begin(); iter != test.end(); ++iter)
	{
		CTransform* Transcom = dynamic_cast<CTransform*>(iter->second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));

		_vec3   vRight, vUp, vLook, vPos, vScale, vAngle;
		_int	iDrawNum = 0;

		Transcom->Get_Info(INFO_RIGHT, &vRight);
		Transcom->Get_Info(INFO_UP, &vUp);
		Transcom->Get_Info(INFO_LOOK, &vLook);
		Transcom->Get_Info(INFO_POS, &vPos);
		memcpy(vScale, Transcom->m_vScale, sizeof(_vec3));
		memcpy(vAngle, Transcom->m_vAngle, sizeof(_vec3));
		iDrawNum = iter->second->Get_DrawTexIndex();

		WriteFile(hFile, &vRight, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vUp, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vLook, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vAngle, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &iDrawNum, sizeof(_int), &dwByte, nullptr);
	}

	CloseHandle(hFile);
	MSG_BOX("Save_Complete");
}

void CImGuiMgr::Load_Transform(LPDIRECT3DDEVICE9 pGrahicDev, CScene *pScene, wstring strDirectory)
{
	HANDLE      hFile = CreateFile(strDirectory.c_str(),      // 파일의 경로와 이름
		GENERIC_READ,         // 파일 접근 모드 (GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL,               // 공유 방식(파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가)    
		NULL,               // 보안 속성(NULL을 지정하면 기본값 상태)
		OPEN_EXISTING,         // CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING  : 파일이 있을 경우에만 열기
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성(읽기 전용, 숨김 등) : FILE_ATTRIBUTE_NORMAL : 아무런 속성이 없는 파일
		NULL);               // 생성될 파일의 속성을 제공할 템플릿 파일(안쓰니깐 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return;
	}

	DWORD   dwByte = 0;

	_vec3   vRight, vUp, vLook, vPos, vScale, vAngle;
	_int	iDrawIndex = 0;
	CLayer* pMyLayer = nullptr;

	while (true)
	{
		ReadFile(hFile, &vRight, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vUp, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vLook, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vAngle, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &iDrawIndex, sizeof(_int), &dwByte, nullptr);

		CGameObject *pGameObject = nullptr;
		
		_tchar* Load_Name = new _tchar[20];
		wstring t = L"Test%d";
		wsprintfW(Load_Name, t.c_str(), m_iIndex);
		NameList.push_back(Load_Name);

		pGameObject = CTestCube::Create(pGrahicDev);
		pMyLayer = pScene->Get_Layer(L"Layer_Wall");

		FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(Load_Name, pGameObject), );
		pGameObject->Set_DrawTexIndex(iDrawIndex);
		++m_iIndex;

		CTransform* Transcom = dynamic_cast<CTransform*>(pGameObject->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));

		Transcom->Set_Info(INFO_RIGHT, &vRight);
		Transcom->Set_Info(INFO_UP, &vUp);
		Transcom->Set_Info(INFO_LOOK, &vLook);
		Transcom->Set_Info(INFO_POS, &vPos);
		Transcom->Set_Angle(&vAngle);
		Transcom->Set_Scale(&vScale);

		Transcom->Update_Component(0.01f);

		//   받아온 정보 입력해줘야함
		if (0 == dwByte)
			break;
	}
	CloseHandle(hFile);
	MSG_BOX("Load_Complete");
}

void CImGuiMgr::SwitchCamera(LPDIRECT3DDEVICE9 pGrahicDev, CScene * pScene, CLayer* pLayer, CCamera * pCam)
{
	if (!Show_Camera_Tool)
		return;

	ImGui::Begin("Camera Settings");

	if (ImGui::CollapsingHeader("Camera Create & Choose Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		CGameObject *pGameObject = nullptr;
		_vec3 pEye = _vec3(0.f, 10.f, -10.f);
		_vec3 pAt = _vec3(0.f, 0.f, 0.f);
		_vec3 pUp = _vec3(0.f, 1.f, 0.f);
		_float fFov = D3DXToRadian(60.f);
		//_float fAspect, fNear, fFar;
		// LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, 
		// const _float& fFov /*= D3DXToRadian(60.f)*/, const _float& fAspect /*= (float)WINCX / WINCY*/, 
		// const _float& fNear /*= 0.1f*/, const _float& fFar /*= 1000.f*/
		ImGui::Text("EYE");	ImGui::SameLine(); ImGui::DragFloat3("EYE", pEye, 0.1f, -1000.0f, 1000.0f);
		ImGui::Text("AT");	ImGui::SameLine(); ImGui::DragFloat3("AT", pAt, 0.1f, -1000.0f, 1000.0f);
		ImGui::Text("UP");	ImGui::SameLine(); ImGui::DragFloat3("UP", pUp, 0.1f, -1000.0f, 1000.0f);
		ImGui::Text("FOV"); ImGui::SameLine(); ImGui::DragFloat("FOV", &fFov, 0.1f, 0.f, 120.f);
		if (ImGui::Button("Create"))
		{
			ImVec2 temp = ImGui::GetMousePos();
			_tchar* szObjTag = new _tchar[20];
			wstring t = L"DynamicCam_%d";
			wsprintfW(szObjTag, t.c_str(), m_iIndex);
			NameList.push_back(szObjTag);
			pGameObject = CDynamicCamera::Create(pGrahicDev, &pEye, &pAt, &pUp, fFov);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(szObjTag, pGameObject), );
			++m_iIndex;
		}
		
		for (auto& iter : pLayer->Get_GameObjectMap())
		{
			char* szCamName;
			int strSize = WideCharToMultiByte(CP_ACP, 0, iter.first, -1, NULL, 0, NULL, NULL);
			szCamName = new char[strSize];
			WideCharToMultiByte(CP_ACP, 0, iter.first, -1, szCamName, strSize, 0, 0);
			CamList.push_back(szCamName);
			
			// 더 만들어봐야함
			// 여기서 모든 맵에 있는 카메라는 꺼버린다
			if (ImGui::Button(szCamName))
			{
				dynamic_cast<CToolScene*>(pScene)->Set_Camera(dynamic_cast<CDynamicCamera*>(iter.second));
				dynamic_cast<CDynamicCamera*>(iter.second)->Set_MainCam();
			}
			string strCamRecBtn;
			string strSave = "_Rec";
			strCamRecBtn = szCamName + strSave;
			ImGui::SameLine();
			if (ImGui::Button(strCamRecBtn.c_str()))
			{
				dynamic_cast<CDynamicCamera*>(iter.second)->SaveBtn();
			}
			string strCamPlayBtn;
			string strLoad = "_Play";
			strCamPlayBtn = szCamName + strLoad;
			ImGui::SameLine();
			if (ImGui::Button(strCamPlayBtn.c_str()))
			{
				dynamic_cast<CDynamicCamera*>(iter.second)->LoadBtn();
			}
		}
	}

	CTransform * pTranscom = nullptr;
	
	if (m_bCameraSelectCheck)
	{
		map<const _tchar*, CGameObject*> mapCam = pLayer->Get_GameObjectMap();

		for (auto iter = mapCam.begin(); iter != mapCam.end(); ++iter)
		{
			pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
			m_CurrentSelectGameObjectObjKey = iter->first;
		}
	}

	TransformEdit(pCam, m_pSelectedTransform, Show_Camera_Tool);

	if (pTranscom != nullptr)
		m_pSelectedTransform = pTranscom;

	ImGui::End();
}

void CImGuiMgr::CreateObject(LPDIRECT3DDEVICE9 pGrahicDev, CScene * pScene, CCamera * pCam)
{
	if (!Show_Object_Tool)
		return;

	ImGui::Begin("Object Settings");

	ImGui::Text("this is Transform_ButtonMenu");

	static _int iSaveStageNum;
	static _int iLoadStageNum;

	ImGui::RadioButton("Gun", &iObjectNum, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Shop", &iObjectNum, 2);
	ImGui::SameLine();
	ImGui::RadioButton("Lava", &iObjectNum, 3);
	ImGui::RadioButton("Item", &iObjectNum, 4);
	ImGui::SameLine();
	ImGui::RadioButton("Throne", &iObjectNum, 5);
	ImGui::SameLine();
	ImGui::RadioButton("Slime", &iObjectNum, 6);
	ImGui::RadioButton("FireMan", &iObjectNum, 7);
	ImGui::SameLine();
	ImGui::RadioButton("Zombie", &iObjectNum, 8);
	ImGui::SameLine();
	ImGui::RadioButton("Illusioner", &iObjectNum, 9);

	if (ImGui::Button("Save"))
	{
		if (iObjectNum == 1)
		{
			Save_ObjectTransform(pScene, L"../../Data/GunPos.dat");
		}
		if (iObjectNum == 2)
		{
			Save_ObjectTransform(pScene, L"../../Data/ShopPos.dat");
		}
		if (iObjectNum == 3)
		{
			Save_ObjectTransform(pScene, L"../../Data/LavaPos.dat");
		}
		if (iObjectNum == 4)
		{
			Save_ObjectTransform(pScene, L"../../Data/ItemPos.dat");
		}
		if (iObjectNum == 5)
		{
			Save_ObjectTransform(pScene, L"../../Data/ThronePos.dat");
		}
		if (iObjectNum == 6)
		{
			Save_ObjectTransform(pScene, L"../../Data/SlimePos.dat");
		}
		if (iObjectNum == 7)
		{
			Save_ObjectTransform(pScene, L"../../Data/FireManPos.dat");
		}
		if (iObjectNum == 8)
		{
			Save_ObjectTransform(pScene, L"../../Data/ZombiePos.dat");
		}
		if (iObjectNum == 9)
		{
			Save_ObjectTransform(pScene, L"../../Data/IllusionerPos.dat");
		}
	}
	if (ImGui::Button("Load"))
	{
		if (iObjectNum == 1)
		{
			Load_ObjectTransform(pGrahicDev,pScene, L"../../Data/GunPos.dat");
		}
		if (iObjectNum == 2)
		{
			Load_ObjectTransform(pGrahicDev,pScene, L"../../Data/ShopPos.dat");
		}
		if (iObjectNum == 3)
		{
			Load_ObjectTransform(pGrahicDev,pScene, L"../../Data/LavaPos.dat");
		}
		if (iObjectNum == 4)
		{
			Load_ObjectTransform(pGrahicDev,pScene, L"../../Data/ItemPos.dat");
		}
		if (iObjectNum == 5)
		{
			Load_ObjectTransform(pGrahicDev,pScene, L"../../Data/ThronePos.dat");
		}
		if (iObjectNum == 6)
		{
			Load_ObjectTransform(pGrahicDev,pScene, L"../../Data/SlimePos.dat");
		}
		if (iObjectNum == 7)
		{
			Load_ObjectTransform(pGrahicDev,pScene, L"../../Data/FireManPos.dat");
		}
		if (iObjectNum == 8)
		{
			Load_ObjectTransform(pGrahicDev,pScene, L"../../Data/ZombiePos.dat");
		}
		if (iObjectNum == 9)
		{
			Load_ObjectTransform(pGrahicDev,pScene, L"../../Data/IllusionerPos.dat");
		}
	}

	if (ImGui::CollapsingHeader("Object Create & Choose Button", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Create"))
		{
			m_bObjectCreateCheck = true;
			m_bObjectSelectCheck = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("ChoseCube"))
		{
			m_bObjectSelectCheck = true;
			m_bObjectCreateCheck = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			CLayer* MyLayer = nullptr;
			if (iObjectNum == 1)
				MyLayer = pScene->Get_Layer(L"Layer_Gun");
			if (iObjectNum == 2)
				MyLayer = pScene->Get_Layer(L"Layer_Shop");
			if (iObjectNum == 3)
				MyLayer = pScene->Get_Layer(L"Layer_Lava");
			if (iObjectNum == 4)
				MyLayer = pScene->Get_Layer(L"Layer_ItemBox");
			if (iObjectNum == 5)
				MyLayer = pScene->Get_Layer(L"Layer_Throne");
			if (iObjectNum == 6)
				MyLayer = pScene->Get_Layer(L"Layer_Slime");
			if (iObjectNum == 7)
				MyLayer = pScene->Get_Layer(L"Layer_FireMan");
			if (iObjectNum == 8)
				MyLayer = pScene->Get_Layer(L"Layer_Zombie");
			if (iObjectNum == 9)
				MyLayer = pScene->Get_Layer(L"Layer_Illusioner");

			MyLayer->Delete_GameObject(m_CurrentSelectGameObjectObjKey.c_str());
		}
	}

	CTransform * pTranscom = nullptr;
	if (m_bObjectCreateCheck)
	{
		ImGui::Text("if double click Create Object");

		if (ImGui::IsMouseDoubleClicked(0))
		{
			if (iObjectNum == 1)
			{
				ImVec2 temp = ImGui::GetMousePos();
				CGameObject *pGameObject = nullptr;
				_tchar* szObjTag = new _tchar[20];
				wstring t = L"Gun%d";
				wsprintfW(szObjTag, t.c_str(), m_iGunIndex);
				NameList.push_back(szObjTag);
				pGameObject = CObjectCube::Create(pGrahicDev, GUN ,int(temp.x), int(temp.y));
				NULL_CHECK_RETURN(pGameObject, );
				CLayer* pCubelayer = pScene->Get_Layer(L"Layer_Gun");
				FAILED_CHECK_RETURN(pCubelayer->Add_GameObject(szObjTag, pGameObject), );
				++m_iGunIndex;
			}
			if (iObjectNum == 2)
			{
				ImVec2 temp = ImGui::GetMousePos();
				CGameObject *pGameObject = nullptr;
				_tchar* szObjTag = new _tchar[20];
				wstring t = L"Shop%d";
				wsprintfW(szObjTag, t.c_str(), m_iShopIndex);
				NameList.push_back(szObjTag);
				pGameObject = CObjectCube::Create(pGrahicDev, SHOP, int(temp.x), int(temp.y));
				NULL_CHECK_RETURN(pGameObject, );
				CLayer* pCubelayer = pScene->Get_Layer(L"Layer_Shop");
				FAILED_CHECK_RETURN(pCubelayer->Add_GameObject(szObjTag, pGameObject), );
				++m_iShopIndex;
			}
			if (iObjectNum == 3)
			{
				ImVec2 temp = ImGui::GetMousePos();
				CGameObject *pGameObject = nullptr;
				_tchar* szObjTag = new _tchar[20];
				wstring t = L"Lava%d";
				wsprintfW(szObjTag, t.c_str(), m_iLavaIndex);
				NameList.push_back(szObjTag);
				pGameObject = CObjectCube::Create(pGrahicDev, LAVA, int(temp.x), int(temp.y));
				NULL_CHECK_RETURN(pGameObject, );
				CLayer* pCubelayer = pScene->Get_Layer(L"Layer_Lava");
				FAILED_CHECK_RETURN(pCubelayer->Add_GameObject(szObjTag, pGameObject), );
				++m_iLavaIndex;
			}
			if (iObjectNum == 4)
			{
				ImVec2 temp = ImGui::GetMousePos();
				CGameObject *pGameObject = nullptr;
				_tchar* szObjTag = new _tchar[20];
				wstring t = L"Item%d";
				wsprintfW(szObjTag, t.c_str(), m_iItemIndex);
				NameList.push_back(szObjTag);
				pGameObject = CObjectCube::Create(pGrahicDev, ITEMBOX, int(temp.x), int(temp.y));
				NULL_CHECK_RETURN(pGameObject, );
				CLayer* pCubelayer = pScene->Get_Layer(L"Layer_ItemBox");
				FAILED_CHECK_RETURN(pCubelayer->Add_GameObject(szObjTag, pGameObject), );
				++m_iItemIndex;
			}
			if (iObjectNum == 5)
			{
				ImVec2 temp = ImGui::GetMousePos();
				CGameObject *pGameObject = nullptr;
				_tchar* szObjTag = new _tchar[20];
				wstring t = L"Throne%d";
				wsprintfW(szObjTag, t.c_str(), m_iThroneIndex);
				NameList.push_back(szObjTag);
				pGameObject = CObjectCube::Create(pGrahicDev, THRONE, int(temp.x), int(temp.y));
				NULL_CHECK_RETURN(pGameObject, );
				CLayer* pCubelayer = pScene->Get_Layer(L"Layer_Throne");
				FAILED_CHECK_RETURN(pCubelayer->Add_GameObject(szObjTag, pGameObject), );
				++m_iThroneIndex;
			}
			if (iObjectNum == 6)
			{
				ImVec2 temp = ImGui::GetMousePos();
				CGameObject *pGameObject = nullptr;
				_tchar* szObjTag = new _tchar[20];
				wstring t = L"Slime%d";
				wsprintfW(szObjTag, t.c_str(), m_iSlimeIndex);
				NameList.push_back(szObjTag);
				pGameObject = CObjectCube::Create(pGrahicDev, SLIME, int(temp.x), int(temp.y));
				NULL_CHECK_RETURN(pGameObject, );
				CLayer* pCubelayer = pScene->Get_Layer(L"Layer_Slime");
				FAILED_CHECK_RETURN(pCubelayer->Add_GameObject(szObjTag, pGameObject), );
				++m_iSlimeIndex;
			}
			if (iObjectNum == 7)
			{
				ImVec2 temp = ImGui::GetMousePos();
				CGameObject *pGameObject = nullptr;
				_tchar* szObjTag = new _tchar[20];
				wstring t = L"FireMan%d";
				wsprintfW(szObjTag, t.c_str(), m_iFireManIndex);
				NameList.push_back(szObjTag);
				pGameObject = CObjectCube::Create(pGrahicDev, FIREMAN, int(temp.x), int(temp.y));
				NULL_CHECK_RETURN(pGameObject, );
				CLayer* pCubelayer = pScene->Get_Layer(L"Layer_FireMan");
				FAILED_CHECK_RETURN(pCubelayer->Add_GameObject(szObjTag, pGameObject), );
				++m_iFireManIndex;
			}
			if (iObjectNum == 8)
			{
				ImVec2 temp = ImGui::GetMousePos();
				CGameObject *pGameObject = nullptr;
				_tchar* szObjTag = new _tchar[20];
				wstring t = L"Zombie%d";
				wsprintfW(szObjTag, t.c_str(), m_iZombieIndex);
				NameList.push_back(szObjTag);
				pGameObject = CObjectCube::Create(pGrahicDev, ZOMBIE, int(temp.x), int(temp.y));
				NULL_CHECK_RETURN(pGameObject, );
				CLayer* pCubelayer = pScene->Get_Layer(L"Layer_Zombie");
				FAILED_CHECK_RETURN(pCubelayer->Add_GameObject(szObjTag, pGameObject), );
				++m_iZombieIndex;
			}
			if (iObjectNum == 9)
			{
				ImVec2 temp = ImGui::GetMousePos();
				CGameObject *pGameObject = nullptr;
				_tchar* szObjTag = new _tchar[20];
				wstring t = L"Illusioner%d";
				wsprintfW(szObjTag, t.c_str(), m_iIllusionerIndex);
				NameList.push_back(szObjTag);
				pGameObject = CObjectCube::Create(pGrahicDev, ILLUSIONER, int(temp.x), int(temp.y));
				NULL_CHECK_RETURN(pGameObject, );
				CLayer* pCubelayer = pScene->Get_Layer(L"Layer_Illusioner");
				FAILED_CHECK_RETURN(pCubelayer->Add_GameObject(szObjTag, pGameObject), );
				++m_iIllusionerIndex;
			}
		}
	}

	if (m_bObjectSelectCheck)
	{
		if (ImGui::IsMouseDoubleClicked(0))
		{
			if (iObjectNum == 1)
			{
				CLayer* MyLayer = pScene->Get_Layer(L"Layer_Gun");

				map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();

				for (auto iter = test.begin(); iter != test.end(); ++iter)
				{
					if (dynamic_cast<CObjectCube*>(iter->second)->Set_SelectGizmo())
					{
						pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
						m_CurrentSelectGameObjectObjKey = iter->first;
					}
				}
			}
			if (iObjectNum == 2)
			{
				CLayer* MyLayer = pScene->Get_Layer(L"Layer_Shop");

				map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();

				for (auto iter = test.begin(); iter != test.end(); ++iter)
				{
					if (dynamic_cast<CObjectCube*>(iter->second)->Set_SelectGizmo())
					{
						pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
						m_CurrentSelectGameObjectObjKey = iter->first;
					}
				}
			}
			if (iObjectNum == 3)
			{
				CLayer* MyLayer = pScene->Get_Layer(L"Layer_Lava");

				map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();

				for (auto iter = test.begin(); iter != test.end(); ++iter)
				{
					if (dynamic_cast<CObjectCube*>(iter->second)->Set_SelectGizmo())
					{
						pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
						m_CurrentSelectGameObjectObjKey = iter->first;
					}
				}
			}
			if (iObjectNum == 4)
			{
				CLayer* MyLayer = pScene->Get_Layer(L"Layer_ItemBox");

				map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();

				for (auto iter = test.begin(); iter != test.end(); ++iter)
				{
					if (dynamic_cast<CObjectCube*>(iter->second)->Set_SelectGizmo())
					{
						pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
						m_CurrentSelectGameObjectObjKey = iter->first;
					}
				}
			}
			if (iObjectNum == 5)
			{
				CLayer* MyLayer = pScene->Get_Layer(L"Layer_Throne");

				map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();

				for (auto iter = test.begin(); iter != test.end(); ++iter)
				{
					if (dynamic_cast<CObjectCube*>(iter->second)->Set_SelectGizmo())
					{
						pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
						m_CurrentSelectGameObjectObjKey = iter->first;
					}
				}
			}
			if (iObjectNum == 6)
			{
				CLayer* MyLayer = pScene->Get_Layer(L"Layer_Slime");

				map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();

				for (auto iter = test.begin(); iter != test.end(); ++iter)
				{
					if (dynamic_cast<CObjectCube*>(iter->second)->Set_SelectGizmo())
					{
						pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
						m_CurrentSelectGameObjectObjKey = iter->first;
					}
				}
			}
			if (iObjectNum == 7)
			{
				CLayer* MyLayer = pScene->Get_Layer(L"Layer_FireMan");

				map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();

				for (auto iter = test.begin(); iter != test.end(); ++iter)
				{
					if (dynamic_cast<CObjectCube*>(iter->second)->Set_SelectGizmo())
					{
						pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
						m_CurrentSelectGameObjectObjKey = iter->first;
					}
				}
			}
			if (iObjectNum == 8)
			{
				CLayer* MyLayer = pScene->Get_Layer(L"Layer_Zombie");

				map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();

				for (auto iter = test.begin(); iter != test.end(); ++iter)
				{
					if (dynamic_cast<CObjectCube*>(iter->second)->Set_SelectGizmo())
					{
						pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
						m_CurrentSelectGameObjectObjKey = iter->first;
					}
				}
			}
			if (iObjectNum == 9)
			{
				CLayer* MyLayer = pScene->Get_Layer(L"Layer_Illusioner");

				map<const _tchar*, CGameObject*> test = MyLayer->Get_GameObjectMap();

				for (auto iter = test.begin(); iter != test.end(); ++iter)
				{
					if (dynamic_cast<CObjectCube*>(iter->second)->Set_SelectGizmo())
					{
						pTranscom = dynamic_cast<CTransform*>(iter->second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
						m_CurrentSelectGameObjectObjKey = iter->first;
					}
				}
			}
		}
	}

	TransformEdit(pCam, m_pSelectedTransform, Show_Cube_Tool);

	// 셀렉버튼을 위한것
	if (pTranscom != nullptr)
		m_pSelectedTransform = pTranscom;

	ImGui::End();
}

void CImGuiMgr::Save_ObjectTransform(CScene * pScene, wstring strDirectory)
{
	HANDLE      hFile = CreateFile(strDirectory.c_str(),
		// 파일의 경로와 이름
		GENERIC_WRITE,         // 파일 접근 모드 (GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL,               // 공유 방식(파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가)    
		NULL,               // 보안 속성(NULL을 지정하면 기본값 상태)
		CREATE_ALWAYS,         // CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING  : 파일이 있을 경우에만 열기
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성(읽기 전용, 숨김 등) : FILE_ATTRIBUTE_NORMAL : 아무런 속성이 없는 파일
		NULL);               // 생성될 파일의 속성을 제공할 템플릿 파일(안쓰니깐 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return;
	}

	CLayer* MyLayer = nullptr;
	_vec3   vRight, vUp, vLook, vPos, vScale, vAngle;
	if (iObjectNum == 1)
		MyLayer = pScene->Get_Layer(L"Layer_Gun");
	if (iObjectNum == 2)
		MyLayer = pScene->Get_Layer(L"Layer_Shop");
	if (iObjectNum == 3)
		MyLayer = pScene->Get_Layer(L"Layer_Lava");
	if (iObjectNum == 4)
		MyLayer = pScene->Get_Layer(L"Layer_ItemBox");
	if (iObjectNum == 5)
		MyLayer = pScene->Get_Layer(L"Layer_Throne");
	if (iObjectNum == 6)
		MyLayer = pScene->Get_Layer(L"Layer_Slime");
	if (iObjectNum == 7)
		MyLayer = pScene->Get_Layer(L"Layer_FireMan");
	if (iObjectNum == 8)
		MyLayer = pScene->Get_Layer(L"Layer_Zombie");
	if (iObjectNum == 9)
		MyLayer = pScene->Get_Layer(L"Layer_Illusioner");

	DWORD   dwByte = 0;

	map<const _tchar*, CGameObject*> mapLayer = MyLayer->Get_GameObjectMap();
	
	for (auto iter = mapLayer.begin(); iter != mapLayer.end(); ++iter)
	{
		CTransform* Transcom = dynamic_cast<CTransform*>(iter->second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));

		_vec3   vRight, vUp, vLook, vPos, vScale, vAngle;
		_int	iDrawNum = 0;

		Transcom->Get_Info(INFO_RIGHT, &vRight);
		Transcom->Get_Info(INFO_UP, &vUp);
		Transcom->Get_Info(INFO_LOOK, &vLook);
		Transcom->Get_Info(INFO_POS, &vPos);
		memcpy(vScale, Transcom->m_vScale, sizeof(_vec3));
		memcpy(vAngle, Transcom->m_vAngle, sizeof(_vec3));

		WriteFile(hFile, &vRight, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vUp, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vLook, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &vAngle, sizeof(_vec3), &dwByte, nullptr);
	}

	CloseHandle(hFile);
	MSG_BOX("Save_Complete");
}

void CImGuiMgr::Load_ObjectTransform(LPDIRECT3DDEVICE9 pGrahicDev, CScene * pScene, wstring strDirectory)
{
	HANDLE      hFile = CreateFile(strDirectory.c_str(),      // 파일의 경로와 이름
		GENERIC_READ,         // 파일 접근 모드 (GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL,               // 공유 방식(파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가)    
		NULL,               // 보안 속성(NULL을 지정하면 기본값 상태)
		OPEN_EXISTING,         // CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING  : 파일이 있을 경우에만 열기
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성(읽기 전용, 숨김 등) : FILE_ATTRIBUTE_NORMAL : 아무런 속성이 없는 파일
		NULL);               // 생성될 파일의 속성을 제공할 템플릿 파일(안쓰니깐 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return;
	}

	DWORD   dwByte = 0;

	_vec3   vRight, vUp, vLook, vPos, vScale, vAngle;
	CLayer* pMyLayer = nullptr;

	while (true)
	{
		ReadFile(hFile, &vRight, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vUp, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vLook, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vAngle, sizeof(_vec3), &dwByte, nullptr);

		CGameObject *pGameObject = nullptr;

		if (iObjectNum == 1)
		{
			_tchar* Load_Name = new _tchar[20];
			wstring t = L"Gun%d";
			wsprintfW(Load_Name, t.c_str(), m_iGunIndex);
			NameList.push_back(Load_Name);
			pGameObject = CObjectCube::Create(pGrahicDev);
			pMyLayer = pScene->Get_Layer(L"Layer_Gun");
			FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(Load_Name, pGameObject), );
			pGameObject->Set_DrawTexIndex(1);
			++m_iGunIndex;
		}
		if (iObjectNum == 2)
		{
			_tchar* Load_Name = new _tchar[20];
			wstring t = L"Shop%d";
			wsprintfW(Load_Name, t.c_str(), m_iShopIndex);
			NameList.push_back(Load_Name);
			pGameObject = CObjectCube::Create(pGrahicDev);
			pMyLayer = pScene->Get_Layer(L"Layer_Shop");
			FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(Load_Name, pGameObject), );
			pGameObject->Set_DrawTexIndex(2);
			++m_iShopIndex;
		}
		if (iObjectNum == 3)
		{
			_tchar* Load_Name = new _tchar[20];
			wstring t = L"Lava%d";
			wsprintfW(Load_Name, t.c_str(), m_iLavaIndex);
			NameList.push_back(Load_Name);
			pGameObject = CObjectCube::Create(pGrahicDev);
			pMyLayer = pScene->Get_Layer(L"Layer_Lava");
			FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(Load_Name, pGameObject), );
			pGameObject->Set_DrawTexIndex(3);
			++m_iLavaIndex;
		}
		if (iObjectNum == 4)
		{
			_tchar* Load_Name = new _tchar[20];
			wstring t = L"Item%d";
			wsprintfW(Load_Name, t.c_str(), m_iItemIndex);
			NameList.push_back(Load_Name);
			pGameObject = CObjectCube::Create(pGrahicDev);
			pMyLayer = pScene->Get_Layer(L"Layer_ItemBox");
			FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(Load_Name, pGameObject), );
			pGameObject->Set_DrawTexIndex(4);
			++m_iItemIndex;
		}
		if (iObjectNum == 5)
		{
			_tchar* Load_Name = new _tchar[20];
			wstring t = L"Throne%d";
			wsprintfW(Load_Name, t.c_str(), m_iThroneIndex);
			NameList.push_back(Load_Name);
			pGameObject = CObjectCube::Create(pGrahicDev);
			pMyLayer = pScene->Get_Layer(L"Layer_Throne");
			FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(Load_Name, pGameObject), );
			pGameObject->Set_DrawTexIndex(5);
			++m_iThroneIndex;
		}
		if (iObjectNum == 6)
		{
			_tchar* Load_Name = new _tchar[20];
			wstring t = L"Slime%d";
			wsprintfW(Load_Name, t.c_str(), m_iSlimeIndex);
			NameList.push_back(Load_Name);
			pGameObject = CObjectCube::Create(pGrahicDev);
			pMyLayer = pScene->Get_Layer(L"Layer_Slime");
			FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(Load_Name, pGameObject), );
			pGameObject->Set_DrawTexIndex(6);
			++m_iSlimeIndex;
		}
		if (iObjectNum == 7)
		{
			_tchar* Load_Name = new _tchar[20];
			wstring t = L"FireMan%d";
			wsprintfW(Load_Name, t.c_str(), m_iFireManIndex);
			NameList.push_back(Load_Name);
			pGameObject = CObjectCube::Create(pGrahicDev);
			pMyLayer = pScene->Get_Layer(L"Layer_FireMan");
			FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(Load_Name, pGameObject), );
			pGameObject->Set_DrawTexIndex(7);
			++m_iFireManIndex;
		}
		if (iObjectNum == 8)
		{
			_tchar* Load_Name = new _tchar[20];
			wstring t = L"Zombie%d";
			wsprintfW(Load_Name, t.c_str(), m_iGunIndex);
			NameList.push_back(Load_Name);
			pGameObject = CObjectCube::Create(pGrahicDev);
			pMyLayer = pScene->Get_Layer(L"Layer_Zombie");
			FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(Load_Name, pGameObject), );
			pGameObject->Set_DrawTexIndex(8);
			++m_iZombieIndex;
		}
		if (iObjectNum == 9)
		{
			_tchar* Load_Name = new _tchar[20];
			wstring t = L"Illusioner%d";
			wsprintfW(Load_Name, t.c_str(), m_iIllusionerIndex);
			NameList.push_back(Load_Name);
			pGameObject = CObjectCube::Create(pGrahicDev);
			pMyLayer = pScene->Get_Layer(L"Layer_Illusioner");
			FAILED_CHECK_RETURN(pMyLayer->Add_GameObject(Load_Name, pGameObject), );
			pGameObject->Set_DrawTexIndex(9);
			++m_iIllusionerIndex;
		}

		CTransform* Transcom = dynamic_cast<CTransform*>(pGameObject->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));

		Transcom->Set_Info(INFO_RIGHT, &vRight);
		Transcom->Set_Info(INFO_UP, &vUp);
		Transcom->Set_Info(INFO_LOOK, &vLook);
		Transcom->Set_Info(INFO_POS, &vPos);
		Transcom->Set_Angle(&vAngle);
		Transcom->Set_Scale(&vScale);

		Transcom->Update_Component(0.01f);

		//   받아온 정보 입력해줘야함
		if (0 == dwByte)
			break;
	}
	CloseHandle(hFile);
	MSG_BOX("Load_Complete");
}

void CImGuiMgr::Free()
{
	for (auto iter : NameList)
	{
		Safe_Delete_Array(iter);
	}

	for (auto iter : CamList)
	{
		Safe_Delete_Array(iter);
	}
}