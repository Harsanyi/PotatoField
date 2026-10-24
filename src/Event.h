#pragma once
#include <functional>
#include <vector>
#include "Utils.h"
#include "iostream"

template<typename... Args>
class Event
{
public:
	inline void subscribe(std::function<void(Args...)> action) {
		actions.push_back(action);
	};

	inline void UnSubscribe(std::function<void(Args...)> action) {
		std::cout << "Try unsubscrybe " << std::endl;
		auto iterator = std::find_if(
			actions.begin(),
			actions.end(),
			[&action](const std::function<void(Args...)>& func) {
				return action.target<void(Args...)>() == func.target<void(Args...)>();
			}
			);
		
		if (iterator != actions.end()) {
			std::cout << "function found!" << std::endl;
			actions.erase(iterator);
			return;
		}

		std::cout << "function NOT found!" << std::endl;
	};

	inline void Invoke(Args... args) {
		for (auto& action : actions) {
			if (action) {
				try {
					action(args...);
				}
				catch (std::runtime_error& err) {
					std::cout << "Error: " << err.what() << std::endl;
				}
			}
		}
	};

	inline void Clear() {
		actions.empty();
	}

private:
	std::vector<std::function<void(Args...)>> actions;
};
