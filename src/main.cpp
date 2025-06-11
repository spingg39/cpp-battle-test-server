#include <Game/Game.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/System/PrintDebug.hpp>
#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
	using namespace sw;
	try
	{
		if (argc != 2)
		{
			throw std::runtime_error("Error: No file specified in command line argument");
		}

		std::ifstream file(argv[1]);
		if (!file)
		{
			throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
		}

		auto eventLog = std::make_shared<EventLog>();
		Game game;
		game.SetEventLog(eventLog);

		auto ProccessIOCommand = [&game](auto& command)
		{
			printDebug(std::cout, command);
			game.AddInputCommand(command);
		};

		std::cout << "Commands:\n";
		io::CommandParser parser;
		parser.add<io::CreateMap>([&](auto command) { ProccessIOCommand(command); })
			.add<io::SpawnSwordsman>([&](auto command) { ProccessIOCommand(command); })
			.add<io::SpawnHunter>([&](auto command) { ProccessIOCommand(command); })
			.add<io::March>([&](auto command) { ProccessIOCommand(command); });

		parser.parse(file);

		std::cout << "\n\nEvents:\n";

		while (!game.IsEnded())
		{
			game.Update();
		}
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what();
	}

	return 0;
}
