# Chat Application
![Демо](others/animation.gif)
## Описание проекта

Это приложение чата на C++, использующее TCP для связи между клиентом и сервером. Оно позволяет пользователям подключаться к серверу, отправлять и получать сообщения в реальном времени. Приложение использует библиотеку Boost.Asio для работы с сетевыми сокетами и асинхронными операциями.

## Функциональные возможности

- Подключение нескольких пользователей к серверу.
- Отправка и получение текстовых сообщений.
- Отображение списка подключенных пользователей.
- Уведомления о подключении и отключении пользователей.
- Команды для управления чатом:
  - `:users` - получить список подключенных пользователей.
  - `:q` - выйти из чата.
  - `:clear` - очистить окно.

## Установка

### Требования

- Компилятор C++ (например, g++ или MinGW)
- Библиотека Boost (необходима для работы с Boost.Asio)

### Установка Boost

Вы можете установить Boost через пакетный менеджер вашей операционной системы или скачать с [официального сайта Boost](https://www.boost.org/).

### Сборка проекта

1. Клонируйте репозиторий:

   ```bash
   git clone https://github.com/russuran/boost_sockets
   ```

   настройте проект согласно вашей конфигурации

2. Выбор протокола:
   Для TCP:
   ```bash
   cd TCP
   ```
   Запуск сервера:
   ```bash
   ./main <port>
   ```

   Подключение к серверу
   ```bash
   ./main <adress> <port>
   ```

   
   Для UDP:

   Запуск сервера:
   ```bash
   cd /UDP/server
   ./server
   ```

   Подключение к серверу
   ```bash
   cd /UDP/client
   ./client
   ```
