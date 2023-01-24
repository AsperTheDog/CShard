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

	static bool addMapping(uint32_t id, InputMapping value);
	static std::vector<uint32_t> triggeredEvents(bool* shouldClose, bool isIDE);

	static void getMouseMovement(int32_t& x, int32_t& y);
	static void subscribeToEvent(ShardEvent event);
	static void unsubscribeFromEvent(ShardEvent event);

	static void removeMapping(uint32_t id);
	static void ImGuiWindowCall(bool* isOpen);

	static std::unordered_map<uint32_t, InputMapping> inputMappings;
	static std::unordered_set<uint32_t> subscribedTypes;
	static int32_t x, y;
	
	static std::vector<ShardEvent> inputTypes;
	static std::unordered_map<ShardEvent, std::string> inputTypeNames;
	static const char* inputNames[5];
	static std::unordered_map<std::string, int32_t> keyboardKeys;
	static std::unordered_map<int32_t, std::string> keyboardKeyNames;
};

