/*
	input.h - header of CInputManager class

	Copyright (c) 2002, Blaine Myers
*/
#ifndef __INPUT_H__
#define __INPUT_H__

#include <dinput.h>
#include <windows.h>
#include "defines.h"



#define KEYDOWN(name, key) (name[key] & 0x80)

#define CD_MOUSE 0x10000000L
#define CD_JOYSTICK 0x20000000L
#define CD_KEYBOARD 0x40000000L

typedef enum tagDPAD{
	DP_CENTER=0,
	DP_UP, 
	DP_DOWN, 
	DP_LEFT, 
	DP_RIGHT, 
	DP_UPLEFT, 
	DP_UPRIGHT, 
	DP_DOWNRIGHT, 
	DP_DOWNLEFT
}DPAD;

BOOL CALLBACK EnumAxis(LPCDIDEVICEOBJECTINSTANCE lpddoi, void* pvRef);
BOOL CALLBACK EnumJoysticks(LPCDIDEVICEINSTANCE lpdii, void* pvRef);

typedef struct tagINPUTCREATEDATA{
	LPDIRECTINPUT8 *lpDI;
	LPDIRECTINPUTDEVICE8 *lpDevice;
}INPUTCREATEDATA;


class CInputManager{
private:
	LPDIRECTINPUT8 m_lpDIObject; //direct input interface
	LPDIRECTINPUTDEVICE8 m_lpKeyboard; //keyboard device
	char m_cKeyboardBuffer[256]; //buffer for keyboard data
	BOOL m_bKeyboard; //whether or not the keyboard was created

	LPDIRECTINPUTDEVICE8 m_lpJoystick; //joystick device
	DIDEVCAPS m_diJoystickCaps; //capabilities of joystick
	DIJOYSTATE2 m_jsState; //state of joystick
	BOOL m_bJoystick; //whether or not the joystick was created

	LPDIRECTINPUTDEVICE8 m_lpMouse; //mouse device
	DIMOUSESTATE2	m_diMouseState; //state of mouse
	BOOL m_bMouse;  //whether or not the mosue was created

	HRESULT ProcessKeyboardInput(); //processes keyboard input
	HRESULT ProcessJoystickInput(); //processes joystick input
	HRESULT ProcessMouseInput(); //processes mouse input
	//BOOL CALLBACK EnumJoysticks(LPCDIDEVICEINSTANCE lpdii, void* pvRef);
	//BOOL CALLBACK EnumAxis(LPCDIDEVICEOBJECTINSTANCE lpddoi, void* pvRef);

	HRESULT InitKeyboard(HWND hWnd); //initializes the keyboard
	HRESULT InitJoystick(HWND hWnd); //initializes the joystick
	HRESULT InitMouse(HWND hWnd); //initializes the mouse
public:
	CInputManager(); //constructor
	~CInputManager(); //destructor
	HRESULT CreateInputDevice(HINSTANCE hInst); //creates input device
	HRESULT ShutDownDevices(); //shuts down devices
	HRESULT CreateDevices(HWND hWnd, DWORD dwCreateFlags); //creates the devices, specify which deviceswith createflags
	HRESULT UpdateInputValues(); //updates input values
	DPAD GetDPad(); //gets the direction of the d-pad
	BOOL GetEscapeStatus(); //This function is only for development
	BOOL GetKeyState(int nKey); //gets state of key
	BOOL GetButtonState(BYTE nButton);
};

#endif //__input_h__