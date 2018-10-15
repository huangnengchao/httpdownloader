#ifndef __HTTPCLIENT_H__
#define __HTTPCLIENT_H__

#include <string>
#include <functional>
#include <curl/curl.h>

#ifdef _WIN32
typedef _off_t p_off_t;
#else
typedef off_t p_off_t;
#endif

//typedef void(*progress_info_callback)(void *userdata, double downloadSpeed, double remainingTime, double progressPercentage);

typedef std::function<void(void *userdata, double download_speed, double remaining_time, double progress_percentage)> progress_info_callback;


class http_client
{
public:

	~http_client();

	static http_client *getInstance();
	static void destroyInstance();

    int http_get(const std::string& requestURL, const std::string& saveTo, void *sender, progress_info_callback cb);
    static void xlog(const char *date, const char *time, const char *file, const int line, const char *func, const char* str);

private:
	http_client();

	bool init();

	static size_t write_callback(char *buffer, size_t size, size_t nmemb, void *userdata);
    static int progress_callback(void *userdata, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

	// Get the local file size
	p_off_t getLocalFileLength(std::string path);

	// Get the file size on the server
	double getDownloadFileLength(std::string url);

private:
	static http_client *instance;

	static double downloadFileLength;
	static p_off_t resumeByte;

	// Call frequency of the callback function
	static time_t lastTime;

    static bool stopCurl;

    static double current_process;
    static int count_process;

    static int retry;
};

#endif		// __HTTPCLIENT_H__
