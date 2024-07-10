/**
 * @file request.h
 * @brief Заголовочный файл для обработки HTTP-запросов.
 *
 * Этот файл содержит определения структур и функций для разбора, логирования и уничтожения HTTP-запросов.
 */

#ifndef REQUEST_H
#define REQUEST_H

#include <winsock2.h>
#include "stringmap.h"
#include "url.h"
#include "method.h"

/** @brief Утилита для выделения памяти и инициализации объектов. */
#define new(x) ((x*)malloc(sizeof(x)))

/** @brief Строка для неизвестного метода HTTP. */
static const char* METHOD_UNKNOWN_ALIAS = "UNKNOWN";

/**
 * @struct Protocol
 * @brief Структура для хранения информации о версии протокола HTTP.
 *
 * Хранит основную информацию о протоколе и его версии.
 */
typedef struct Protocol {
    const char* content;  /**< Строка, содержащая полное название протокола (например, "HTTP/1.1"). */
    const char* protocol; /**< Название протокола (например, "HTTP"). */
    size_t      major;    /**< Основная версия протокола (например, 1 для "HTTP/1.1"). */
    size_t      minor;    /**< Минорная версия протокола (например, 1 для "HTTP/1.1"). */
} Protocol;

/**
 * @struct HttpRequest
 * @brief Структура для хранения данных HTTP-запроса.
 *
 * Содержит метод HTTP-запроса, путь, версию протокола и содержимое запроса.
 */
typedef struct HttpRequest {
    Method       method;       /**< Метод HTTP-запроса (GET, POST и т.д.). */
    Path         path;         /**< Путь запроса (например, "/index.html"). */
    Protocol*    protocol;     /**< Указатель на структуру Protocol с информацией о версии протокола. */
    const char * content;      /**< Содержимое запроса. */
} HttpRequest;

/**
 * @struct Request
 * @brief Основная структура для хранения данных HTTP-запроса.
 *
 * Хранит HTTP-запрос, заголовки, параметры GET и POST-запросов, cookies, файлы и данные сессии.
 */
typedef struct Request {
    HttpRequest*   httpRequest; /**< Указатель на структуру HttpRequest с данными запроса. */
    StringMap*     headers;     /**< Заголовки запроса. */
    SOCKET         client;      /**< Сокет клиента. */
    const char*    clientIp;    /**< IP-адрес клиента. */
    unsigned short clientPort;  /**< Порт клиента. */
    StringMap*     queryParams; /**< Параметры GET-запроса. */
    StringMap*     postParams;  /**< Параметры POST-запроса. */
    StringMap*     cookies;     /**< Cookies. */
    StringMap*     files;       /**< Загруженные файлы. */
    StringMap*     session;     /**< Данные сессии. */
    const char*    body;        /**< Тело запроса. */
} Request;

/**
 * @struct FileData
 * @brief Структура для хранения данных загруженного файла.
 *
 * Хранит информацию о файле, загруженном в запросе.
 */
typedef struct FileData {
    const char*  fileName;       /**< Имя файла. */
    const char*  fileType;       /**< MIME-тип файла. */
    size_t       fileSize;       /**< Размер файла в байтах. */
    const char*  fileContent;    /**< Содержимое файла. */
} FileData;

/** @brief Список всех возможных методов HTTP. */
extern const unsigned int METHOD_ALL;

/**
 * @brief Разбирает данные запроса и создает структуру Request.
 *
 * Парсит строку запроса, извлекает информацию о методе, заголовках, параметрах и теле запроса.
 *
 * @param data Строка с данными HTTP-запроса.
 * @return Указатель на структуру Request с данными запроса.
 */
Request* ParseRequest(const char *data);

/**
 * @brief Логирует информацию о запросе.
 *
 * Выводит информацию о запросе для целей отладки и мониторинга.
 *
 * @param request Указатель на структуру Request с данными запроса.
 */
void LogRequest(const Request* request);

/**
 * @brief Освобождает ресурсы, связанные с запросом.
 *
 * Освобождает память, выделенную для структуры Request и ее полей.
 *
 * @param request Указатель на структуру Request для освобождения.
 */
void RequestDestroy(Request* request);

#endif // REQUEST_H
