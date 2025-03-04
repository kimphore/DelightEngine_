#include "Win32_Input.h"

void CKeyboardInput::InputDown(int32 Input)
{
	RawInput[Input] = true;
}

void CKeyboardInput::InputUp(int32 Input)
{
	RawInput[Input] = false;
}

bool8 CKeyboardInput::IsInputPressed(int32 Input) const
{
	return CurrentFrameInput.at(Input) && !PreviousFrameInput.at(Input);
}

bool8 CKeyboardInput::IsInputHeld(int32 Input) const
{
	return CurrentFrameInput.at(Input) && PreviousFrameInput.at(Input);
}

bool8 CKeyboardInput::IsInputReleased(int32 Input) const
{
	return !CurrentFrameInput.at(Input) && PreviousFrameInput.at(Input);
}

void CMouseInput::MouseMove(int32 deltaX, int32 deltaY)
{
	RawMousePos = Int32_2(deltaX, deltaY);
}

void CMouseInput::MouseWheelScroll(float32 delta)
{
	MouseScrollDelta = delta;
}

LRESULT CWin32_Input::WinProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_KEYDOWN:
			KeyboardInput.InputDown(static_cast<int32>(wParam));
			break;

		case WM_KEYUP:
			KeyboardInput.InputUp(static_cast<int32>(wParam));
			break;

		case WM_MOUSEMOVE:
			MouseInput.MouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;

		case WM_LBUTTONDOWN:
			MouseInput.InputDown(Mouse_LBT);
			break;

		case WM_LBUTTONUP:
			MouseInput.InputUp(Mouse_LBT);
			break;

		case WM_RBUTTONDOWN:
			MouseInput.InputDown(Mouse_RBT);
			break;

		case WM_RBUTTONUP:
			MouseInput.InputUp(Mouse_RBT);
			break;
	}

	return false;
}

void CWin32_Input::Update()
{
	KeyboardInput.UpdateInputData();
	MouseInput.UpdateInputData();
}

