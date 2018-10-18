#ifndef __HTTPCLIENT_H__
#define __HTTPCLIENT_H__

#include <string>
#include <functional>
#include <curl/curl.h>
#include <mutex>

#ifdef _WIN32
typedef _off_t p_off_t;
#else
typedef off_t p_off_t;
#endif


typedef std::function<void(void *userdata, double download_speed, double remaining_time, double progress_percentage)> progress_info_callback;


class http_client
{
public:
	~http_client();

	static http_client *get_instance();
	static void release_instance();

    int http_get(const std::string& requesturl, const std::string& saveto, void *sender, progress_info_callback cb);
    static void xlog(const char *date, const char *time, const char *file, const int line, const char *func, const char* str);

private:
	http_client();

	bool init();

	static size_t write_callback(char *buffer, size_t size, size_t nmemb, void *userdata);
    static int progress_callback(void *userdata, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

	// Get the local file size
	p_off_t get_local_file_length(std::string path);

	// Get the file size on the server
	double get_download_file_length(std::string url);

private:
	static http_client *instance_;
	static double download_file_length_;
	static p_off_t resume_byte_;			//
	static time_t last_time_;				// Call frequency of the callback function
    volatile static bool stop_curl_;		// Stop curl
    static double current_process_;
    static int count_process_;
    static int retry_;
    static int get_file_length_retry_;		// Get File Length retry times
    static std::mutex mutex_;
};

#endif		// __HTTPCLIENT_H__
