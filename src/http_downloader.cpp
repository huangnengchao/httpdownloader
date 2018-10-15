#include "http_downloader.h"
#include "http_client.h"

#include <iostream>

http_downloader::http_downloader() {

}

http_downloader::~http_downloader() {

}


void http_downloader::progress_callback(void *userdata, double download_speed, double remaining_time, double progress_percentage)
{
	int hours = 0, minutes = 0, seconds = 0;

	if (download_speed != 0)
	{
		hours = remaining_time / 3600;
		minutes = (remaining_time - hours * 3600) / 60;
		seconds = remaining_time - hours * 3600 - minutes * 60;
	}

	std::string unit = "B";
	if (download_speed > 1024 * 1024 * 1024)
	{
		unit = "G";
		download_speed /= 1024 * 1024 * 1024;
	}
	else if (download_speed > 1024 * 1024)
	{
		unit = "M";
		download_speed /= 1024 * 1024;
	}
	else if (download_speed > 1024)
	{
		unit = "kB";
		download_speed /= 1024;
	}

	char speedFormat[15] = { 0 };
	char timeFormat[10] = { 0 };
	char progressFormat[8] = { 0 };

#ifdef _WIN32
	sprintf_s(speedFormat, sizeof(speedFormat), "%.2f%s/s", download_speed, unit.c_str());
	sprintf_s(timeFormat, sizeof(timeFormat), "%02d:%02d:%02d", hours, minutes, seconds);
	sprintf_s(progressFormat, sizeof(progressFormat), "%.2f", progress_percentage);
#else
	sprintf(speedFormat, "%.2f%s/s", download_speed, unit.c_str());
	sprintf(timeFormat, "%02d:%02d:%02d", hours, minutes, seconds);
	sprintf(progressFormat, "%.2f", progress_percentage);
#endif

	std::cout << speedFormat << "\t" << timeFormat << "\t" << progressFormat << "%" << std::endl;

	this->progress_info_.progress_percentage = progress_percentage;
	this->progress_info_.remaining_time = remaining_time;
	this->progress_info_.download_speed = download_speed;

	if (nullptr != progress_callback_) {
		progress_callback_(progress_info_);
	}
	
}


int http_downloader::download(const std::string& url, const std::string& save_path) {
	int ret = 0;
	progress_info_callback cbf = std::bind(&http_downloader::progress_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	ret = http_client::getInstance()->http_get(url, save_path, 0, cbf);

	return ret;
}