#pragma once
class GameWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGui::Begin("Game", isOpen);
		ImGui::End();
	}

private:

};

