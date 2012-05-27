/*
	input.cpp - routines for CInputManager DirectInput

	Copyright (c) 2002, Blaine Myers
*/
/*
	The Input class is far from complete, all three game devices have
	been implimented, but they aren't accessable in a manner of which I would approve
*/

#include "input.h"

BOOL CALLBACK EnumAxis(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef){

	LPDIRECTINPUTDEVICE8 *lpJoystick=(LPDIRECTINPUTDEVICE8*)pvRef;
	
	DIPROPRANGE diprg;
	diprg.diph.dwSize=sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize= sizeof(DIPROPHEADER);
	diprg.diph.dwHow=DIPH_BYID;
	diprg.diph.dwObj =lpddoi->dwType;
	diprg.lMin = -1000;
	diprg.lMax = 1000;

	//set range for axis
	if(FAILED((*lpJoystick)->SetProperty(DIPROP_RANGE, &diprg.diph)))
		return DIENUM_STOP;

    return DIENUM_CONTINUE;
}



BOOL CALLBACK EnumJoysticks(LPCDIDEVICEINSTANCE lpdii, LPVOID pvRef){
	
	INPUTCREATEDATA* diData;

	diData=(INPUTCREATEDATA*)pvRef;
	
	HRESULT hr;

	LPDIRECTINPUT8 *lpDI = diData->lpDI;

	//obtain interface to enumerated joysticks
	hr = (*lpDI)->CreateDevice(lpdii->guidInstance, diData->lpDevice, NULL);

	if(FAILED(hr))
		return DIENUM_CONTINUE;
	//this callback just gets the first device allowing the user to pick would be better
	return DIENUM_STOP;
}



CInputManager::CInputManager(){
	//m_lpDIObject=NULL;
	m_lpDIObject=NULL;
	m_lpKeyboard=NULL;
	m_lpJoystick=NULL;
	m_lpMouse=NULL;
	CreateInputDevice(GetModuleHandle(NULL));

	m_bKeyboard=FALSE;
	m_bMouse=FALSE;
	m_bJoystick=FALSE;
}

CInputManager::~CInputManager(){
	ShutDownDevices();
	SAFE_RELEASE(m_lpDIObject);
}



HRESULT CInputManager::ShutDownDevices(){
	if(m_lpKeyboard)m_lpKeyboard->Unacquire();
	SAFE_RELEASE(m_lpKeyboard);

	if(m_lpJoystick)m_lpJoystick->Unacquire();
	SAFE_RELEASE(m_lpJoystick);

	if(m_lpMouse)m_lpMouse->Unacquire();
	SAFE_RELEASE(m_lpMouse);

	m_bKeyboard=FALSE;
	m_bMouse=FALSE;
	m_bJoystick=FALSE;
	return S_OK;
}


HRESULT CInputManager::CreateInputDevice(HINSTANCE hInst){
	HRESULT hr;
	
	hr = DirectInput8Create(
		hInst, 
		DIRECTINPUT_VERSION,
		IID_IDirectInput8, 
		(VOID**)&m_lpDIObject, 
		NULL);

	return hr;
}


HRESULT CInputManager::InitKeyboard(HWND hWnd){
	/*
		Getting the keyboard
	*/
	if(!m_lpDIObject)return E_FAIL;
	HRESULT hr;
	//Create Keyboard device
	if(FAILED(hr=m_lpDIObject->CreateDevice(GUID_SysKeyboard, &m_lpKeyboard, NULL)))return hr;
	//set data format
	if(FAILED(hr=m_lpKeyboard->SetDataFormat(&c_dfDIKeyboard)))return hr;
	//Set cooperative level (note we don't necessarily need to specify exclusive
	if(FAILED(hr=m_lpKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE)))return hr;
	
	if (m_lpKeyboard) hr=m_lpKeyboard->Acquire();
	
	m_bKeyboard=TRUE;

	return hr;
}

HRESULT CInputManager::InitMouse(HWND hWnd){
	if(!m_lpDIObject)return E_FAIL;
	HRESULT hr;
	//create mouse object
	if(FAILED(hr=m_lpDIObject->CreateDevice(GUID_SysMouse, &m_lpMouse, NULL)))return hr;
	//set data format
	if(FAILED(hr=m_lpMouse->SetDataFormat(&c_dfDIMouse2)))return hr;
	//set the cooperative level
	if(FAILED(hr=m_lpMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND|DISCL_EXCLUSIVE)))return hr;

	if(m_lpMouse)hr=m_lpMouse->Acquire();

	m_bMouse=TRUE;

	return hr;
}

HRESULT CInputManager::InitJoystick(HWND hWnd){
	/*
		getting the joystick
	*/
	if(!m_lpDIObject)return E_FAIL;
	HRESULT hr;
	//first enumerate the devices
	INPUTCREATEDATA diData;
	diData.lpDI=&m_lpDIObject;
	diData.lpDevice=&m_lpJoystick;
	if(FAILED(hr=m_lpDIObject->EnumDevices(DI8DEVCLASS_GAMECTRL,
				EnumJoysticks, (LPVOID)&diData, DIEDFL_ATTACHEDONLY)))return hr;


	if(m_lpJoystick == NULL)return E_FAIL;
	
	//Joystick was aquired
	if(FAILED(hr=m_lpJoystick->SetDataFormat(&c_dfDIJoystick2)))return hr;
	if(FAILED(hr=m_lpJoystick->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))return hr;

	m_diJoystickCaps.dwSize=sizeof(m_diJoystickCaps);
	if(FAILED(hr=m_lpJoystick->GetCapabilities(&m_diJoystickCaps)))return hr;
	if(FAILED(hr=m_lpJoystick->EnumObjects(EnumAxis, (VOID*)&m_lpJoystick, DIDFT_AXIS)))return hr;

	m_bJoystick=TRUE;
	return S_OK;
}

HRESULT CInputManager::CreateDevices(HWND hWnd, DWORD dwCreateFlags){
	HRESULT hr=S_OK;

	if(!m_lpDIObject)return E_FAIL;
	DWORD dwFlag;

	dwFlag = (dwCreateFlags&CD_KEYBOARD);

	if(dwFlag==CD_KEYBOARD)
		hr=InitKeyboard(hWnd);

	dwFlag = (dwCreateFlags&CD_JOYSTICK);
	if(dwFlag==CD_JOYSTICK)
		hr=InitJoystick(hWnd);
	
	dwFlag = (dwCreateFlags&CD_MOUSE);
	if(dwFlag==CD_MOUSE)
		hr=InitMouse(hWnd);

	return hr;
}

HRESULT CInputManager::ProcessJoystickInput(){
	if(!m_lpJoystick)return E_FAIL;
	if(!m_bJoystick)return E_FAIL;

	HRESULT hr;

	// Poll the device to read the current state
	hr = m_lpJoystick->Poll(); 
	if (FAILED(hr)){
	// Attempt to reacquire joystick
	hr = m_lpJoystick->Acquire();
	while(hr == DIERR_INPUTLOST) 
		hr = m_lpJoystick->Acquire(); 
	}
	hr = m_lpJoystick->GetDeviceState(sizeof(DIJOYSTATE2), &m_jsState);
	if (FAILED(hr))
		return hr;

	//the joystick state will be stored in m_jsState structure until next poll
	return hr;
}

HRESULT CInputManager::ProcessMouseInput(){
	if(!m_lpMouse)return E_FAIL;
	if(!m_bMouse)return E_FAIL;

	HRESULT hr;

	if(FAILED(hr=m_lpMouse->GetDeviceState(sizeof(m_diMouseState), (LPVOID)&m_diMouseState))){
		if(hr==DIERR_INPUTLOST){
			//we lost mouse, lets reaquire it
			hr=m_lpMouse->Acquire();
			while(hr==DIERR_INPUTLOST){
				hr=m_lpMouse->Acquire();
			}
		}else if(hr==DIERR_NOTACQUIRED){
			hr = m_lpMouse->Acquire();
			while(hr == DIERR_INPUTLOST){
				hr=m_lpMouse->Acquire();
			}
		}
		else return hr;
		return hr;
	}

	return hr;
}

HRESULT CInputManager::ProcessKeyboardInput(){
	if(!m_lpKeyboard)return E_FAIL;
	if(!m_bKeyboard)return E_FAIL;
	
	HRESULT hr;

	if(FAILED(hr=m_lpKeyboard->GetDeviceState(sizeof(m_cKeyboardBuffer), (LPVOID)&m_cKeyboardBuffer))){
		if(hr==DIERR_INPUTLOST){
			//we lost the keyboard, lets reaquire it
			hr = m_lpKeyboard->Acquire();
			while(hr == DIERR_INPUTLOST){
				hr = m_lpKeyboard->Acquire();
			}
				
		}else if(hr==DIERR_NOTACQUIRED){
			hr = m_lpKeyboard->Acquire();
			while(hr == DIERR_INPUTLOST){
				hr = m_lpKeyboard->Acquire();
			}
		}
		else return hr;
		return hr; //return no mater what as data needs to be gotten again any way
	}
	//now keyboard data is in buffer: Act appropriately

	return hr;
}

HRESULT CInputManager::UpdateInputValues(){
	if(!m_lpDIObject)return E_FAIL;
	HRESULT hr=S_OK;
	if(m_bKeyboard)if(FAILED(ProcessKeyboardInput()))hr=E_FAIL;
	if(m_bJoystick)if(FAILED(ProcessJoystickInput()))hr=E_FAIL;
	if(m_bMouse)if(FAILED(ProcessMouseInput()))hr=E_FAIL;
	
	return hr;
}

BOOL CInputManager::GetEscapeStatus(){
	if(!m_lpDIObject)return E_FAIL;
	if(KEYDOWN(m_cKeyboardBuffer, DIK_ESCAPE))return TRUE;
	else return FALSE;
}


DPAD CInputManager::GetDPad(){
	if(!m_lpDIObject)return DP_CENTER;
	BOOL up=FALSE, down=FALSE, left=FALSE, right=FALSE;

	//we check the joystick first if it is active we return it's value
	//if it is centered the function will then check the keyboard
	{
		if(m_jsState.lX>500)right=TRUE;
		if(m_jsState.lX<-500)left=TRUE;
		if(m_jsState.lY<-500)up=TRUE;
		if(m_jsState.lY>500)down=TRUE;

		if(up && !left && !right)return DP_UP;
		if(up && left)return DP_UPLEFT;
		if(up && right)return DP_UPRIGHT;
		if(down && !left && !right)return DP_DOWN;
		if(down && left)return DP_DOWNLEFT;
		if(down && right)return DP_DOWNRIGHT;
		if(left && !up && !down)return DP_LEFT;
		if(right && !up && !down)return DP_RIGHT;
	}
	
	if(KEYDOWN(m_cKeyboardBuffer, DIK_UP))up=TRUE;
	if(KEYDOWN(m_cKeyboardBuffer, DIK_DOWN))down=TRUE;
	if(KEYDOWN(m_cKeyboardBuffer, DIK_LEFT))left=TRUE;
	if(KEYDOWN(m_cKeyboardBuffer, DIK_RIGHT))right=TRUE;

	//if both up and down then none
	if(up&&down){
		up=FALSE;
		down=FALSE;
	}
	//if both left and righ then neigher
	if(left&&right){
		left=FALSE;
		right=FALSE;
	}

	if(up && !left && !right)return DP_UP;
	if(up && left)return DP_UPLEFT;
	if(up && right)return DP_UPRIGHT;
	if(down && !left && !right)return DP_DOWN;
	if(down && left)return DP_DOWNLEFT;
	if(down && right)return DP_DOWNRIGHT;
	if(left && !up && !down)return DP_LEFT;
	if(right && !up && !down)return DP_RIGHT;
	//should manage joystick too
	return DP_CENTER;
}

BOOL CInputManager::GetKeyState(int nKey){
	if(!m_lpDIObject)return FALSE;
	if(!m_lpKeyboard)return FALSE;
	if(KEYDOWN(m_cKeyboardBuffer, nKey))return TRUE;
	else return FALSE;
}

BOOL CInputManager::GetButtonState(BYTE nButton){
	if(!m_lpDIObject)return FALSE;
	if(!m_lpJoystick)return FALSE;
	if((m_jsState.rgbButtons[nButton]&0x80)==0x80)return TRUE;
	else return FALSE;
}