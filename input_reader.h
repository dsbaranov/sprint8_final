#pragma once
#include <string>
#include <string_view>
#include <vector>

#include "geo.h"
#include "transport_catalogue.h"

struct CommandDescription {
  // Определяет, задана ли команда (поле command непустое)
  explicit operator bool() const { return !command.empty(); }

  bool operator!() const { return !operator bool(); }

  std::string command;      // Название команды
  std::string id;           // id маршрута или остановки
  std::string description;  // Параметры команды
  CommandDescription() = default;

  CommandDescription(std::string _command, std::string _id,
                     std::string _description)
      : command(_command), id(_id), description(_description) {}
  CommandDescription(const CommandDescription& other) = default;

  CommandDescription(CommandDescription&& other) {
    command = move(other.command);
    id = move(other.id);
    description = move(other.description);
  }

  CommandDescription& operator=(CommandDescription&& other) {
    command = move(other.command);
    id = move(other.id);
    description = move(other.description);
    return *this;
  }
};

class InputReader {
 public:
  /**
   * Парсит строку в структуру CommandDescription и сохраняет результат в
   * commands_
   */
  void ParseLine(std::string_view line);

  /**
   * Наполняет данными транспортный справочник, используя команды из commands_
   */
  void ApplyCommands(TransportCatalogue& catalogue) const;

 private:
  std::vector<CommandDescription> commands_;
};