#pragma once
class AttributesWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGui::Begin("Attributes", isOpen);
		ImGui::End();
	}

private:

};
