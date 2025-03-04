#pragma once

#include "Include.h"
#include "eastl/unordered_map.h"

class CKeyboardInput
{
public:
	eastl::unordered_map<int32, bool8> RawInput;
	eastl::unordered_map<int32, bool8> CurrentFrameInput;
	eastl::unordered_map<int32, bool8> PreviousFrameInput;

	virtual void UpdateInputData()
	{
		PreviousFrameInput = std::move(CurrentFrameInput);
		CurrentFrameInput = std::move(RawInput);
		RawInput = eastl::unordered_map<int32, bool8>();
	}

	void InputDown(int32 Input);
	void InputUp(int32 Input);
	bool8 IsInputPressed(int32 Input) const;
	bool8 IsInputHeld(int32 Input) const;
	bool8 IsInputReleased(int32 Input) const;
};
/*
* 소숫점 단위 정밀한 인풋 추적이 필요하다면 GetRawInputData 이거 고려.
*/
class CMouseInput : public CKeyboardInput
{
public:
	Int32_2 RawMousePos;
	Int32_2 CurrentMousePos;
	Int32_2 PreviousMousePos;

	float32 MouseScrollDelta;

	void MouseMove(int32 PosX, int32 PosY);
	void MouseWheelScroll(float32 delta);

	virtual void UpdateInputData()
	{
		CKeyboardInput::UpdateInputData();

		PreviousMousePos = CurrentMousePos;
		CurrentMousePos = RawMousePos;
	}

	Int32_2 GetMouseMoveDelta() const
	{
		return CurrentMousePos - PreviousMousePos;
	}

	float32 GetMouseWhellScrollDelta() const
	{
		return MouseScrollDelta;
	}
};

class CWin32_Input
{
public:
	LRESULT WinProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void Update();

public:
	CKeyboardInput KeyboardInput;
	CMouseInput MouseInput;
};