#include "http_client.h"

#include <sys/stat.h>
#include <stdlib.h>



#define DEBUG_LOG__

#define XLOG(str) http_client::xlog(__DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__, str);


#ifdef _WIN32
#define XSLEEP(x) sleep(x);
#else
#include <unistd.h>
#define XSLEEP(x) sleep(x);
#endif


http_client *http_client::instance = NULL;

double http_client::downloadFileLength = -1;
p_off_t http_client::resumeByte = -1;
time_t http_client::lastTime = 0;

bool http_client::stopCurl = false;

double http_client::current_process = 0.0;
int http_client::count_process = 0;
int http_client::retry = 100;


typedef struct
{
    void *sender;
    CURL *handle;
    progress_info_callback cb;
}Progress_User_Data;

typedef enum
{
    HTTP_REQUEST_OK = CURLE_OK,

    HTTP_REQUEST_ERROR = -999,

}Http_Client_Response;


size_t http_client::write_callback(char *buffer, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = static_cast<FILE *>(userdata);
    size_t length = fwrite(buffer, size, nmemb, fp);
    if (length != nmemb)
    {
        return length;
    }

    return size * nmemb;
}

int http_client::progress_callback(void *userdata, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    (void)ultotal;
    (void)ulnow;

    if(stopCurl)
        return 1;

    time_t now = time(NULL);
    if (now - lastTime < 1)
    {
        return 0;
    }
    lastTime = now;

    Progress_User_Data *data = static_cast<Progress_User_Data *>(userdata);
    CURL *easy_handle = data->handle;

    // Defaults to bytes/second
    double speed = 0;
    curl_easy_getinfo(easy_handle, CURLINFO_SPEED_DOWNLOAD, &speed);

    // Time remaining
    double leftTime = 0;

    // Progress percentage
    double progress = 0;

    printf("dltotal=%d, speed=%d, downloadFileLength=%f\n", dltotal, speed, downloadFileLength);

    if (dltotal != 0 && speed != 0)
    {
        progress = (dlnow + resumeByte) / downloadFileLength * 100;
        leftTime = (downloadFileLength - dlnow - resumeByte) / speed;
        //printf("\t%.2f%%\tRemaing time:%s\n", progress, timeFormat);
    }

    if (http_client::current_process == progress) {
        http_client::count_process++;
    } else  {
        http_client::current_process = progress;
    }

    if (http_client::count_process > 10) {
        http_client::count_process = 0;
        return 1;
    }

    if (data->cb != NULL)
        data->cb(data->sender, speed, leftTime, progress);

    return 0;
}


http_client::http_client()
{

}


http_client::~http_client()
{
}

http_client *http_client::getInstance()
{
    if (instance == NULL)
	{
		instance = new http_client();
		instance->init();
	}

	return instance;
}

void http_client::destroyInstance()
{
    if (instance != NULL)
	{
        stopCurl = true;

        // curl_global_cleanup()    will crash on Qt windows

        delete instance;
        instance = NULL;
	}
}

bool http_client::init()
{
    if(curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
    	return false;
    }
    return true;
}

void http_client::xlog(const char *date, const char *time, const char *file, const int line, const char *func, const char *str)
{
#ifdef DEBUG_LOG__
	printf("%s | %s | %s | %d | %s | %s\n", date, time, file, line, func, str);
#endif
}

// Return 0 if success, otherwise return error code
int http_client::http_get(const std::string& requestURL, const std::string& saveTo, void *sender, progress_info_callback cb)
{
	XLOG("http_client::HttpGet");
    std::string partPath = saveTo + ".part";

    CURL *easy_handle = NULL;
    FILE *fp = NULL;

    int ret = HTTP_REQUEST_ERROR;

    do
    {
        // Get the file size on the server
        downloadFileLength = getDownloadFileLength(requestURL);

        if (downloadFileLength < 0)
        {
            XLOG("getDownloadFileLength error");
            break;
        }

        easy_handle = curl_easy_init();
        if (!easy_handle)
        {
            XLOG("curl_easy_init error");
            break;
        }

#ifdef _WIN32
        fopen_s(&fp, partPath.c_str(), "ab+");
#else
		fp = fopen(partPath.c_str(), "ab+");
#endif
        if (fp == NULL)
        {
            XLOG("file open failed");
            XLOG(partPath.c_str());
            break;
        }

        // Set the url
        ret = curl_easy_setopt(easy_handle, CURLOPT_URL, requestURL.c_str());

        // Save data from the server
        ret |= curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, &http_client::write_callback);
        ret |= curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, fp);

		Progress_User_Data data = { sender, easy_handle, cb };

        // Get the download progress
        ret |= curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0L);
        ret |= curl_easy_setopt(easy_handle, CURLOPT_XFERINFOFUNCTION, &http_client::progress_callback);
        ret |= curl_easy_setopt(easy_handle, CURLOPT_XFERINFODATA, &data);

        // Fail the request if the HTTP code returned is equal to or larger than 400
        ret |= curl_easy_setopt(easy_handle, CURLOPT_FAILONERROR, 1L);

        // The maximum time that allow the connection parse to the server
        ret |= curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 10L);

        // The maximum time that allow to wait download
        //ret |= curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 12L);

		resumeByte = getLocalFileLength(partPath);
        if (resumeByte > 0)
        {
            // Set a point to resume transfer
            ret |= curl_easy_setopt(easy_handle, CURLOPT_RESUME_FROM_LARGE, resumeByte);
        }

        if (ret != CURLE_OK)
        {
			ret = HTTP_REQUEST_ERROR;
            XLOG("curl_easy_setopt error");
            break;
        }

        XLOG("start");
        int retry = http_client::retry;
        while (retry--) {
        	ret = curl_easy_perform(easy_handle);
        	if (ret == CURLE_OK) {
        		break;
        	} else {

    			char s[100] = { 0 };
    #ifdef _WIN32
    			sprintf_s(s, sizeof(s), "error:%d:%s", ret, curl_easy_strerror(static_cast<CURLcode>(ret)));
    #else
    			sprintf(s, "error:%d:%s", ret, curl_easy_strerror(static_cast<CURLcode>(ret)));
    #endif

                XLOG(s);
        		XSLEEP(1);

            	fflush(fp);
        		resumeByte = getLocalFileLength(partPath);
                if (resumeByte > 0)
                {
                	ret  = 0;
                    // Set a point to resume transfer
                	ret |= curl_easy_setopt(easy_handle, CURLOPT_RESUME_FROM_LARGE, resumeByte);
                    if (ret != CURLE_OK)
                    {
            			ret = HTTP_REQUEST_ERROR;
                        XLOG("curl_easy_setopt error");
                        break;
                    }
                }
        	}
        }

        XLOG("end");

        if (ret != CURLE_OK)
        {
			char s[100] = { 0 };
#ifdef _WIN32
			sprintf_s(s, sizeof(s), "error:%d:%s", ret, curl_easy_strerror(static_cast<CURLcode>(ret)));
#else
			sprintf(s, "error:%d:%s", ret, curl_easy_strerror(static_cast<CURLcode>(ret)));
#endif

            XLOG(s);
            switch (ret)
            {
                case CURLE_HTTP_RETURNED_ERROR:
                {
                    int code = 0;
                    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &code);

					char s[100] = { 0 };
#ifdef _WIN32
					sprintf_s(s, sizeof(s), "HTTP error code:%d", code);
#else
					sprintf(s, "HTTP error code:%d", code);
#endif
                    XLOG(s);
                    break;
                }
            }
        }
    } while (0);

	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}

    curl_easy_cleanup(easy_handle);
    easy_handle = NULL;

    if (ret == CURLE_OK)
    {
        remove(saveTo.c_str());
		rename(partPath.c_str(), saveTo.c_str());
    }

    return ret;
}



// Get the local file size, return -1 if failed
p_off_t http_client::getLocalFileLength(std::string path)
{
	p_off_t ret;
	struct stat fileStat;

	ret = stat(path.c_str(), &fileStat);
	if (ret == 0)
	{
		return fileStat.st_size;
	}

	return ret;
}

size_t nousecb(char *buffer, size_t x, size_t y, void *userdata)
{
	(void)buffer;
	(void)userdata;
	return x * y;
}

// Get the file size on the server
double http_client::getDownloadFileLength(std::string url)
{
    CURL *easy_handle = NULL;
	int ret = CURLE_OK;
	double size = -1;

	int retry = http_client::retry;

	do
	{
		easy_handle = curl_easy_init();
		if (!easy_handle)
		{
            XLOG("curl_easy_init error");
			break;
		}

		// Only get the header data
		ret = curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
		ret |= curl_easy_setopt(easy_handle, CURLOPT_HEADER, 1L);
		ret |= curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L);
        ret |= curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, nousecb);	// libcurl_a.lib will return error code 23 without this sentence on windows

        // The maximum time that allow to wait download
        ret |= curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 32L);


		if (ret != CURLE_OK)
		{
            XLOG("curl_easy_setopt error");
			break;
		}


		ret = curl_easy_perform(easy_handle);
		if (ret != CURLE_OK) {
			char s[100] = { 0 };
#ifdef _WIN32
			sprintf_s(s, sizeof(s), "error:%d:%s", ret, curl_easy_strerror(static_cast<CURLcode>(ret)));
#else
			sprintf(s, "error:%d:%s", ret,
					curl_easy_strerror(static_cast<CURLcode>(ret)));
#endif
			XLOG(s);
			XSLEEP(1);
			continue;
		}


		// size = -1 if no Content-Length return or Content-Length=0
		ret = curl_easy_getinfo(easy_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &size);
		if (ret != CURLE_OK)
		{
            XLOG("curl_easy_getinfo error");
			break;
		}

	} while (retry--);

	curl_easy_cleanup(easy_handle);
	return size;
}