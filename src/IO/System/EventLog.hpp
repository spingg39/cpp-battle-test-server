#pragma once

#include "details/PrintFieldVisitor.hpp"

#include <iostream>
#include <typeindex>
#include <unordered_map>

namespace sw
{
	class EventLog
	{
	private:
		std::ostream& _stream;

	public:
		explicit EventLog(std::ostream& stream = std::cout) :
				_stream(stream)
		{}

		template <class TEvent>
		void log(uint64_t tick, TEvent&& event)
		{
			using DecayedEventType = std::decay_t<decltype(event)>;
			_stream << "[" << tick << "] " << DecayedEventType::Name << " ";
			PrintFieldVisitor visitor(_stream);
			event.visit(visitor);
			_stream << std::endl;
		}
	};
}
