#include "graphic.h"
class KEY
{
public:
	bool IsPressed();
	bool IsPressedRepeatedly();
	void SetKeyState(int action);
private:
	bool pressed = false;
	bool released = true;
	bool pressedPrevFrame = false;
};

class MOUSE
{
public:
	int GetPosX();
	int GetPosY();
	void SetMousePos(int x, int y);
private:
	int x;
	int y;
};



namespace input
{
	extern KEY
		w,
		a,
		s,
		d,
		mouseL,
		mouseR;

	extern MOUSE
		mouse;

	static void KeyCallBack(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	static void MouseButtonCallBack(GLFWwindow* pwin, int button, int action, int mod);
	static void MousePosCallBack(GLFWwindow* pwin, double xpos, double ypos);

	void init(GLFWwindow* pwin);

}