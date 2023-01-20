#pragma once
class ObjectWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGui::Begin("Objects", isOpen);
		ImGui::End();
	}

private:

};

