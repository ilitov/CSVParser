#ifndef _COMMAND_HANDLERS_HPP_
#define _COMMAND_HANDLERS_HPP_

#include <map>
#include <cstdint>
#include <algorithm>
#include <execution>
#include <functional>

template <typename DataType>
class CommandHandlers {
public:
	using HandlerType = std::function<void(DataType &, const DataType &)>;

public:
	void registerHandler(char op, HandlerType handler) {
		m_handlers[op] = std::move(handler);
	}

	HandlerType resolveHandler(char op) const {
		const auto it = m_handlers.find(op);
		if (it == m_handlers.end()) {
			return nullptr;
		}

		return it->second;
	}

private:
	std::map<char, HandlerType> m_handlers;
};

#endif // !_COMMAND_HANDLERS_HPP_
