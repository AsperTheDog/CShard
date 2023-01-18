#pragma once
#include <unordered_map>
#include <unordered_set>

#include <SDL_events.h>

enum ShardEvent
{
	EVENT_KEYUP = SDL_KEYUP,
	EVENT_KEYDOWN = SDL_KEYDOWN,
	EVENT_MOUSEBUTTONUP = SDL_MOUSEBUTTONUP,
	EVENT_MOUSEBUTTONDOWN = SDL_MOUSEBUTTONDOWN,
	EVENT_MOUSEMOTION = SDL_MOUSEMOTION,
	EVENT_MOUSEWHEEL = SDL_MOUSEWHEEL
};

struct inputMapping
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

	static bool addMapping(uint32_t id, inputMapping value);
	static std::vector<uint32_t> triggeredEvents(bool* shouldClose);

	static void getMouseMovement(int32_t& x, int32_t& y);
	static void subscribeToEvent(ShardEvent event);
	static void unsubscribeFromEvent(ShardEvent event);

	static void ImGuiWindowCall();

private:
	static std::unordered_map<uint32_t, inputMapping> inputMappings;
	static std::unordered_set<uint32_t> subscribedTypes;
	static int32_t x, y;

	static std::unordered_map<ShardEvent, std::string> inputTypes;
	static std::unordered_map<std::string, int32_t> keyboardKeys;
	static std::unordered_map<int32_t, std::string> keyboardKeyNames;
};

