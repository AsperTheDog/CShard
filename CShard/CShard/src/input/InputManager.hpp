#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>

#include <SDL_events.h>

#define WHEEL_UP true
#define WHEEL_DOWN false

enum ShardEvent
{
	EVENT_KEYUP = SDL_KEYUP,
	EVENT_KEYDOWN = SDL_KEYDOWN,
	EVENT_MOUSEBUTTONUP = SDL_MOUSEBUTTONUP,
	EVENT_MOUSEBUTTONDOWN = SDL_MOUSEBUTTONDOWN,
	EVENT_MOUSEMOTION = SDL_MOUSEMOTION,
	EVENT_MOUSEWHEEL = SDL_MOUSEWHEEL
};

struct InputMapping
{
private:
	union Values
	{
		int32_t key;
		uint8_t mbutton;
		bool wheelDir;
	};
public:
	ShardEvent type;
	Values val;
};

class InputManager
{
public:
	InputManager() = delete;

	static void init();

	static bool addMapping(std::string id, InputMapping value);
	static void triggeredEvents(bool* shouldClose, bool isIDE);

	static void getMouseMovement(int32_t& x, int32_t& y);
	static void subscribeToEvent(ShardEvent event);
	static void unsubscribeFromEvent(ShardEvent event);

	static void removeMapping(const std::string& id);
	static void ImGuiWindowCall(bool* isOpen);

	inline static std::unordered_map<std::string, InputMapping> inputMappings;
	inline static std::unordered_set<std::string> triggeredThisFrame;
	inline static std::unordered_set<uint32_t> subscribedTypes;
	inline static int32_t x, y;
	
	inline static std::vector<ShardEvent> inputTypes;
	inline static std::unordered_map<ShardEvent, std::string> inputTypeNames;
	inline static const char* inputNames[5] = {
		"Keyboard down",
		"Keyboard up",
		"Mouse button down",
		"Mouse button up",
		"Mouse wheel"
	};
	inline static std::unordered_map<std::string, int32_t> keyboardKeys;
	inline static std::unordered_map<int32_t, std::string> keyboardKeyNames;
};

