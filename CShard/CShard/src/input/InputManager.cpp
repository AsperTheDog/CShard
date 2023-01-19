#include "InputManager.hpp"

#include <algorithm>
#include <imgui.h>
#include <string>

#include "../device/window/SDLFramework.hpp"
std::unordered_map<uint32_t, InputMapping> InputManager::inputMappings;
std::unordered_set<uint32_t> InputManager::subscribedTypes;
int32_t InputManager::x, InputManager::y;
std::vector<ShardEvent> InputManager::inputTypes;
std::unordered_map<ShardEvent, std::string> InputManager::inputTypeNames;
const char* InputManager::inputNames[5] = {
		"Keyboard down",
		"Keyboard up",
		"Mouse button down",
		"Mouse button up",
		"Mouse wheel"
	};;
std::unordered_map<std::string, int32_t> InputManager::keyboardKeys;
std::unordered_map<int32_t, std::string> InputManager::keyboardKeyNames;

int InputManager::tempDeleteID = 0;
int InputManager::tempID = 0;
int InputManager::tempButton = 0;
bool InputManager::tempWheel = false;
int InputManager::tempType = 0;
char InputManager::tempValue[20] = "";


bool InputManager::addMapping(uint32_t id, InputMapping value)
{
	if (InputManager::inputMappings.contains(id)) return false;
	InputManager::subscribeToEvent((ShardEvent)value.type);
	InputManager::inputMappings.emplace(id, value);
	return true;
}

std::vector<uint32_t> InputManager::triggeredEvents(bool* shouldClose)
{
	std::vector<SDL_Event> SDLevents = SDLFramework::getEvents(subscribedTypes);
	std::vector<uint32_t> events{};
	for (auto& event : SDLevents)
	{
		if (event.type == EVENT_MOUSEMOTION)
		{
			InputManager::x = event.motion.x;
			InputManager::y = event.motion.y;
			continue;
		}
		if (event.type == SDL_QUIT)
		{
			*shouldClose = true;
			continue;
		}
		for (auto& mapping : inputMappings)
		{
			if (event.type != mapping.second.type) 
				continue;

			if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == mapping.second.val.key) events.push_back(mapping.first);
			}
			else if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == mapping.second.val.mbutton) events.push_back(mapping.first);
			}
			else if (event.type == EVENT_MOUSEWHEEL)
			{
				int isFlipped = event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED;
				bool direction = event.wheel.y * (isFlipped ? -1 : 1) > 0;
				if (direction == mapping.second.val.wheelDir) events.push_back(mapping.first);
			}
		}
	}
	return events;
}

void InputManager::getMouseMovement(int32_t& x, int32_t& y)
{
	x = InputManager::x;
	y = InputManager::y;
}

void InputManager::subscribeToEvent(ShardEvent event)
{
	InputManager::subscribedTypes.insert(event);
}

void InputManager::unsubscribeFromEvent(ShardEvent event)
{
	InputManager::subscribedTypes.erase(event);
}

void InputManager::removeMapping(uint32_t id)
{
	if (!InputManager::inputMappings.contains(id)) return;
	InputManager::inputMappings.erase(id);
}

void InputManager::ImGuiWindowCall(bool* isOpen)
{
	if (!*isOpen) return;
	ImGui::Begin("Input Mapper");
	ImGui::InputInt("ID", &tempID);
	tempID = std::max(0, tempID);
	ImGui::Combo("Input type", &tempType, inputNames, 5);
	bool validValue = true;
	switch(tempType)
	{
	case 0:
	case 1:
		ImGui::InputText("Value", tempValue, 30);
		validValue = keyboardKeys.contains(tempValue);
		break;
	case 2:
	case 3:
		ImGui::InputInt("Button", &tempButton);
		tempButton = std::clamp(tempButton, 0, 255);
		break;
	case 4:
		ImGui::Checkbox("Upwards", &tempWheel);
		break;
	default: 
		validValue = false;
	}

	bool isIDInUse = inputMappings.contains(tempID);
	if (isIDInUse) ImGui::Text("That ID already exists!");
	if (!validValue) ImGui::Text("Invalid keyboard key");

	ImGui::BeginDisabled(isIDInUse || !validValue);
	if (ImGui::Button("Add new mapper"))
	{
		InputMapping mapping = {inputTypes.at(tempType), 0};
		switch(tempType)
		{
		case 0:
		case 1:
			mapping.val.key = keyboardKeys.at(tempValue);
			break;
		case 2:
		case 3:
			mapping.val.mbutton = tempButton;
			break;
		case 4:
			mapping.val.wheelDir = tempWheel;
			break;
		default:
			mapping.val.key = 0;
		}
		InputManager::addMapping(tempID, mapping);
		tempID++;
	}
	ImGui::EndDisabled();

	ImGui::Separator();
	ImGui::InputInt("##DeleteID", &tempDeleteID);
	ImGui::SameLine();
	if (ImGui::Button("Delete ID"))
	{
		InputManager::removeMapping(tempDeleteID);
	}

	ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
	ImGui::BeginTable("Mappings", 3, flags);
	ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
    ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();
    for (auto& map : inputMappings)
    {
        ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("%d", map.first);
		ImGui::TableSetColumnIndex(1);
		ImGui::Text(inputTypeNames.at(map.second.type).c_str());
		ImGui::TableSetColumnIndex(2);
        if (map.second.type == EVENT_KEYDOWN || map.second.type == EVENT_KEYUP)
			ImGui::Text(keyboardKeyNames.at(map.second.val.key).c_str());
		else if (map.second.type == EVENT_MOUSEBUTTONDOWN || map.second.type == EVENT_MOUSEBUTTONUP)
			ImGui::Text("button %d", map.second.val.mbutton);
		else if (map.second.type == EVENT_MOUSEWHEEL)
			ImGui::Text(map.second.val.wheelDir ? "Up" : "Down");
    }
    ImGui::EndTable();
	ImGui::End();

	bool something = true;
	ImGui::ShowDemoWindow(&something);
}

void InputManager::init()
{
	inputMappings = std::unordered_map<uint32_t, InputMapping>();
	subscribedTypes = std::unordered_set<uint32_t>();
	x = 0;
	y = 0;

	subscribedTypes.insert(SDL_QUIT);

	inputTypes = std::vector<ShardEvent>();
	inputTypes.push_back(EVENT_KEYDOWN);
	inputTypes.push_back(EVENT_KEYUP);
	inputTypes.push_back(EVENT_MOUSEBUTTONDOWN);
	inputTypes.push_back(EVENT_MOUSEBUTTONUP);
	inputTypes.push_back(EVENT_MOUSEWHEEL);
	inputTypes.push_back(EVENT_MOUSEMOTION);

	inputTypeNames = std::unordered_map<ShardEvent, std::string>();
	inputTypeNames.emplace(EVENT_KEYUP, "Keyboard up");
	inputTypeNames.emplace(EVENT_KEYDOWN, "Keyboard down");
	inputTypeNames.emplace(EVENT_MOUSEBUTTONDOWN, "Mouse button down");
	inputTypeNames.emplace(EVENT_MOUSEBUTTONUP, "Mouse button up");
	inputTypeNames.emplace(EVENT_MOUSEMOTION, "Mouse movement"); 
	inputTypeNames.emplace(EVENT_MOUSEWHEEL, "Mouse wheel");



	keyboardKeys = std::unordered_map<std::string, int32_t>();
	keyboardKeys.reserve(243);

	keyboardKeys.emplace("RETURN", SDLK_RETURN);
	keyboardKeys.emplace("ESCAPE", SDLK_ESCAPE);
	keyboardKeys.emplace("BACKSPACE", SDLK_BACKSPACE);
	keyboardKeys.emplace("TAB", SDLK_TAB);
	keyboardKeys.emplace("SPACE", SDLK_SPACE);
	keyboardKeys.emplace("0", SDLK_0);
	keyboardKeys.emplace("1", SDLK_1);
	keyboardKeys.emplace("2", SDLK_2);
	keyboardKeys.emplace("3", SDLK_3);
	keyboardKeys.emplace("4", SDLK_4);
	keyboardKeys.emplace("5", SDLK_5);
	keyboardKeys.emplace("6", SDLK_6);
	keyboardKeys.emplace("7", SDLK_7);
	keyboardKeys.emplace("8", SDLK_8);
	keyboardKeys.emplace("9", SDLK_9);
	keyboardKeys.emplace("_", SDLK_UNDERSCORE);
	keyboardKeys.emplace("a", SDLK_a);
	keyboardKeys.emplace("b", SDLK_b);
	keyboardKeys.emplace("c", SDLK_c);
	keyboardKeys.emplace("d", SDLK_d);
	keyboardKeys.emplace("e", SDLK_e);
	keyboardKeys.emplace("f", SDLK_f);
	keyboardKeys.emplace("g", SDLK_g);
	keyboardKeys.emplace("h", SDLK_h);
	keyboardKeys.emplace("i", SDLK_i);
	keyboardKeys.emplace("j", SDLK_j);
	keyboardKeys.emplace("k", SDLK_k);
	keyboardKeys.emplace("l", SDLK_l);
	keyboardKeys.emplace("m", SDLK_m);
	keyboardKeys.emplace("n", SDLK_n);
	keyboardKeys.emplace("o", SDLK_o);
	keyboardKeys.emplace("p", SDLK_p);
	keyboardKeys.emplace("q", SDLK_q);
	keyboardKeys.emplace("r", SDLK_r);
	keyboardKeys.emplace("s", SDLK_s);
	keyboardKeys.emplace("t", SDLK_t);
	keyboardKeys.emplace("u", SDLK_u);
	keyboardKeys.emplace("v", SDLK_v);
	keyboardKeys.emplace("w", SDLK_w);
	keyboardKeys.emplace("x", SDLK_x);
	keyboardKeys.emplace("y", SDLK_y);
	keyboardKeys.emplace("z", SDLK_z);
	keyboardKeys.emplace("CAPSLOCK", SDLK_CAPSLOCK);
	keyboardKeys.emplace("F1", SDLK_F1);
	keyboardKeys.emplace("F2", SDLK_F2);
	keyboardKeys.emplace("F3", SDLK_F3);
	keyboardKeys.emplace("F4", SDLK_F4);
	keyboardKeys.emplace("F5", SDLK_F5);
	keyboardKeys.emplace("F6", SDLK_F6);
	keyboardKeys.emplace("F7", SDLK_F7);
	keyboardKeys.emplace("F8", SDLK_F8);
	keyboardKeys.emplace("F9", SDLK_F9);
	keyboardKeys.emplace("F10", SDLK_F10);
	keyboardKeys.emplace("F11", SDLK_F11);
	keyboardKeys.emplace("F12", SDLK_F12);
	keyboardKeys.emplace("PRINTSCREEN", SDLK_PRINTSCREEN);
	keyboardKeys.emplace("SCROLLLOCK", SDLK_SCROLLLOCK);
	keyboardKeys.emplace("PAUSE", SDLK_PAUSE);
	keyboardKeys.emplace("INSERT", SDLK_INSERT);
	keyboardKeys.emplace("HOME", SDLK_HOME);
	keyboardKeys.emplace("PAGEUP", SDLK_PAGEUP);
	keyboardKeys.emplace("DELETE", SDLK_DELETE);
	keyboardKeys.emplace("END", SDLK_END);
	keyboardKeys.emplace("PAGEDOWN", SDLK_PAGEDOWN);
	keyboardKeys.emplace("RIGHT", SDLK_RIGHT);
	keyboardKeys.emplace("LEFT", SDLK_LEFT);
	keyboardKeys.emplace("DOWN", SDLK_DOWN);
	keyboardKeys.emplace("UP", SDLK_UP);
	keyboardKeys.emplace("NUMLOCKCLEAR", SDLK_NUMLOCKCLEAR);
	keyboardKeys.emplace("KP_DIVIDE", SDLK_KP_DIVIDE);
	keyboardKeys.emplace("KP_MULTIPLY", SDLK_KP_MULTIPLY);
	keyboardKeys.emplace("KP_MINUS", SDLK_KP_MINUS);
	keyboardKeys.emplace("KP_PLUS", SDLK_KP_PLUS);
	keyboardKeys.emplace("KP_ENTER", SDLK_KP_ENTER);
	keyboardKeys.emplace("KP_1", SDLK_KP_1);
	keyboardKeys.emplace("KP_2", SDLK_KP_2);
	keyboardKeys.emplace("KP_3", SDLK_KP_3);
	keyboardKeys.emplace("KP_4", SDLK_KP_4);
	keyboardKeys.emplace("KP_5", SDLK_KP_5);
	keyboardKeys.emplace("KP_6", SDLK_KP_6);
	keyboardKeys.emplace("KP_7", SDLK_KP_7);
	keyboardKeys.emplace("KP_8", SDLK_KP_8);
	keyboardKeys.emplace("KP_9", SDLK_KP_9);
	keyboardKeys.emplace("KP_0", SDLK_KP_0);
	keyboardKeys.emplace("KP_PERIOD", SDLK_KP_PERIOD);
	keyboardKeys.emplace("APPLICATION", SDLK_APPLICATION);
	keyboardKeys.emplace("POWER", SDLK_POWER);
	keyboardKeys.emplace("KP_EQUALS", SDLK_KP_EQUALS);
	keyboardKeys.emplace("F13", SDLK_F13);
	keyboardKeys.emplace("F14", SDLK_F14);
	keyboardKeys.emplace("F15", SDLK_F15);
	keyboardKeys.emplace("F16", SDLK_F16);
	keyboardKeys.emplace("F17", SDLK_F17);
	keyboardKeys.emplace("F18", SDLK_F18);
	keyboardKeys.emplace("F19", SDLK_F19);
	keyboardKeys.emplace("F20", SDLK_F20);
	keyboardKeys.emplace("F21", SDLK_F21);
	keyboardKeys.emplace("F22", SDLK_F22);
	keyboardKeys.emplace("F23", SDLK_F23);
	keyboardKeys.emplace("F24", SDLK_F24);
	keyboardKeys.emplace("EXECUTE", SDLK_EXECUTE);
	keyboardKeys.emplace("HELP", SDLK_HELP);
	keyboardKeys.emplace("MENU", SDLK_MENU);
	keyboardKeys.emplace("SELECT", SDLK_SELECT);
	keyboardKeys.emplace("STOP", SDLK_STOP);
	keyboardKeys.emplace("AGAIN", SDLK_AGAIN);
	keyboardKeys.emplace("UNDO", SDLK_UNDO);
	keyboardKeys.emplace("CUT", SDLK_CUT);
	keyboardKeys.emplace("COPY", SDLK_COPY);
	keyboardKeys.emplace("PASTE", SDLK_PASTE);
	keyboardKeys.emplace("FIND", SDLK_FIND);
	keyboardKeys.emplace("MUTE", SDLK_MUTE);
	keyboardKeys.emplace("VOLUMEUP", SDLK_VOLUMEUP);
	keyboardKeys.emplace("VOLUMEDOWN", SDLK_VOLUMEDOWN);
	keyboardKeys.emplace("KP_COMMA", SDLK_KP_COMMA);
	keyboardKeys.emplace("KP_EQUALSAS400", SDLK_KP_EQUALSAS400);
	keyboardKeys.emplace("ALTERASE", SDLK_ALTERASE);
	keyboardKeys.emplace("SYSREQ", SDLK_SYSREQ);
	keyboardKeys.emplace("CANCEL", SDLK_CANCEL);
	keyboardKeys.emplace("CLEAR", SDLK_CLEAR);
	keyboardKeys.emplace("PRIOR", SDLK_PRIOR);
	keyboardKeys.emplace("RETURN2", SDLK_RETURN2);
	keyboardKeys.emplace("SEPARATOR", SDLK_SEPARATOR);
	keyboardKeys.emplace("OUT", SDLK_OUT);
	keyboardKeys.emplace("OPER", SDLK_OPER);
	keyboardKeys.emplace("CLEARAGAIN", SDLK_CLEARAGAIN);
	keyboardKeys.emplace("CRSEL", SDLK_CRSEL);
	keyboardKeys.emplace("EXSEL", SDLK_EXSEL);
	keyboardKeys.emplace("KP_00", SDLK_KP_00);
	keyboardKeys.emplace("KP_000", SDLK_KP_000);
	keyboardKeys.emplace("THOUSANDSSEPARATOR", SDLK_THOUSANDSSEPARATOR);
	keyboardKeys.emplace("DECIMALSEPARATOR", SDLK_DECIMALSEPARATOR);
	keyboardKeys.emplace("CURRENCYUNIT", SDLK_CURRENCYUNIT);
	keyboardKeys.emplace("CURRENCYSUBUNIT", SDLK_CURRENCYSUBUNIT);
	keyboardKeys.emplace("KP_LEFTPAREN", SDLK_KP_LEFTPAREN);
	keyboardKeys.emplace("KP_RIGHTPAREN", SDLK_KP_RIGHTPAREN);
	keyboardKeys.emplace("KP_LEFTBRACE", SDLK_KP_LEFTBRACE);
	keyboardKeys.emplace("KP_RIGHTBRACE", SDLK_KP_RIGHTBRACE);
	keyboardKeys.emplace("KP_TAB", SDLK_KP_TAB);
	keyboardKeys.emplace("KP_BACKSPACE", SDLK_KP_BACKSPACE);
	keyboardKeys.emplace("KP_A", SDLK_KP_A);
	keyboardKeys.emplace("KP_B", SDLK_KP_B);
	keyboardKeys.emplace("KP_C", SDLK_KP_C);
	keyboardKeys.emplace("KP_D", SDLK_KP_D);
	keyboardKeys.emplace("KP_E", SDLK_KP_E);
	keyboardKeys.emplace("KP_F", SDLK_KP_F);
	keyboardKeys.emplace("KP_XOR", SDLK_KP_XOR);
	keyboardKeys.emplace("KP_POWER", SDLK_KP_POWER);
	keyboardKeys.emplace("KP_PERCENT", SDLK_KP_PERCENT);
	keyboardKeys.emplace("KP_LESS", SDLK_KP_LESS);
	keyboardKeys.emplace("KP_GREATER", SDLK_KP_GREATER);
	keyboardKeys.emplace("KP_AMPERSAND", SDLK_KP_AMPERSAND);
	keyboardKeys.emplace("KP_DBLAMPERSAND", SDLK_KP_DBLAMPERSAND);
	keyboardKeys.emplace("KP_VERTICALBAR", SDLK_KP_VERTICALBAR);
	keyboardKeys.emplace("KP_DBLVERTICALBAR", SDLK_KP_DBLVERTICALBAR);
	keyboardKeys.emplace("KP_COLON", SDLK_KP_COLON);
	keyboardKeys.emplace("KP_HASH", SDLK_KP_HASH);
	keyboardKeys.emplace("KP_SPACE", SDLK_KP_SPACE);
	keyboardKeys.emplace("KP_AT", SDLK_KP_AT);
	keyboardKeys.emplace("KP_EXCLAM", SDLK_KP_EXCLAM);
	keyboardKeys.emplace("KP_MEMSTORE", SDLK_KP_MEMSTORE);
	keyboardKeys.emplace("KP_MEMRECALL", SDLK_KP_MEMRECALL);
	keyboardKeys.emplace("KP_MEMCLEAR", SDLK_KP_MEMCLEAR);
	keyboardKeys.emplace("KP_MEMADD", SDLK_KP_MEMADD);
	keyboardKeys.emplace("KP_MEMSUBTRACT", SDLK_KP_MEMSUBTRACT);
	keyboardKeys.emplace("KP_MEMMULTIPLY", SDLK_KP_MEMMULTIPLY);
	keyboardKeys.emplace("KP_MEMDIVIDE", SDLK_KP_MEMDIVIDE);
	keyboardKeys.emplace("KP_PLUSMINUS", SDLK_KP_PLUSMINUS);
	keyboardKeys.emplace("KP_CLEAR", SDLK_KP_CLEAR);
	keyboardKeys.emplace("KP_CLEARENTRY", SDLK_KP_CLEARENTRY);
	keyboardKeys.emplace("KP_BINARY", SDLK_KP_BINARY);
	keyboardKeys.emplace("KP_OCTAL", SDLK_KP_OCTAL);
	keyboardKeys.emplace("KP_DECIMAL", SDLK_KP_DECIMAL);
	keyboardKeys.emplace("KP_HEXADECIMAL", SDLK_KP_HEXADECIMAL);
	keyboardKeys.emplace("LCTRL", SDLK_LCTRL);
	keyboardKeys.emplace("LSHIFT", SDLK_LSHIFT);
	keyboardKeys.emplace("LALT", SDLK_LALT);
	keyboardKeys.emplace("LGUI", SDLK_LGUI);
	keyboardKeys.emplace("RCTRL", SDLK_RCTRL);
	keyboardKeys.emplace("RSHIFT", SDLK_RSHIFT);
	keyboardKeys.emplace("RALT", SDLK_RALT);
	keyboardKeys.emplace("RGUI", SDLK_RGUI);
	keyboardKeys.emplace("MODE", SDLK_MODE);
	keyboardKeys.emplace("AUDIONEXT", SDLK_AUDIONEXT);
	keyboardKeys.emplace("AUDIOPREV", SDLK_AUDIOPREV);
	keyboardKeys.emplace("AUDIOSTOP", SDLK_AUDIOSTOP);
	keyboardKeys.emplace("AUDIOPLAY", SDLK_AUDIOPLAY);
	keyboardKeys.emplace("AUDIOMUTE", SDLK_AUDIOMUTE);
	keyboardKeys.emplace("MEDIASELECT", SDLK_MEDIASELECT);
	keyboardKeys.emplace("WWW", SDLK_WWW);
	keyboardKeys.emplace("MAIL", SDLK_MAIL);
	keyboardKeys.emplace("CALCULATOR", SDLK_CALCULATOR);
	keyboardKeys.emplace("COMPUTER", SDLK_COMPUTER);
	keyboardKeys.emplace("AC_SEARCH", SDLK_AC_SEARCH);
	keyboardKeys.emplace("AC_HOME", SDLK_AC_HOME);
	keyboardKeys.emplace("AC_BACK", SDLK_AC_BACK);
	keyboardKeys.emplace("AC_FORWARD", SDLK_AC_FORWARD);
	keyboardKeys.emplace("AC_STOP", SDLK_AC_STOP);
	keyboardKeys.emplace("AC_REFRESH", SDLK_AC_REFRESH);
	keyboardKeys.emplace("AC_BOOKMARKS", SDLK_AC_BOOKMARKS);
	keyboardKeys.emplace("BRIGHTNESSDOWN", SDLK_BRIGHTNESSDOWN);
	keyboardKeys.emplace("BRIGHTNESSUP", SDLK_BRIGHTNESSUP);
	keyboardKeys.emplace("DISPLAYSWITCH", SDLK_DISPLAYSWITCH);
	keyboardKeys.emplace("KBDILLUMTOGGLE", SDLK_KBDILLUMTOGGLE);
	keyboardKeys.emplace("KBDILLUMDOWN", SDLK_KBDILLUMDOWN);
	keyboardKeys.emplace("KBDILLUMUP", SDLK_KBDILLUMUP);
	keyboardKeys.emplace("EJECT", SDLK_EJECT);
	keyboardKeys.emplace("SLEEP", SDLK_SLEEP);
	keyboardKeys.emplace("APP1", SDLK_APP1);
	keyboardKeys.emplace("APP2", SDLK_APP2);
	keyboardKeys.emplace("AUDIOREWIND", SDLK_AUDIOREWIND);
	keyboardKeys.emplace("AUDIOFASTFORWARD", SDLK_AUDIOFASTFORWARD);
	keyboardKeys.emplace("SOFTLEFT", SDLK_SOFTLEFT);
	keyboardKeys.emplace("SOFTRIGHT", SDLK_SOFTRIGHT);
	keyboardKeys.emplace("CALL", SDLK_CALL);
	keyboardKeys.emplace("ENDCALL", SDLK_ENDCALL);

	keyboardKeyNames = std::unordered_map<int32_t, std::string>();
	keyboardKeyNames.reserve(keyboardKeys.size());

	for (auto& key : keyboardKeys)
	{
		keyboardKeyNames.emplace(key.second, key.first);
	}
}
