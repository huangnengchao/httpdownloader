#include "http_downloader.h"
#include <iostream>
#include <functional>

void progress_callback(void *userdata, double download_speed, double remaining_time, double progress_percentage)
{
	//qDebug()<<download_speed<<remaining_time<<progress_percentage;
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
	//AnyClass *eg = static_cast<AnyClass *>(userdata);
	//eg->func(speedFormat, timeFormat, progressFormat);
}

int main()
{

	http_downloader hd;
	hd.download("http://ftp.sjtu.edu.cn/ubuntu-cd/18.04.1/ubuntu-18.04.1-live-server-amd64.iso", "ubuntu-18.04.1-live-server-amd64.iso");

	//progress_info_callback cbf = std::bind(&progress_callback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	//http_client::getInstance()->http_get("http://ftp.sjtu.edu.cn/ubuntu-cd/18.04.1/ubuntu-18.04.1-live-server-amd64.iso", "ubuntu-18.04.1-live-server-amd64.iso", NULL, cbf);
	//http_client::getInstance()->http_get("https://raw.githubusercontent.com/Genetalks/gtz/master/gtz_latest.run", "/home/nil/tmp/gtz_latest_4.run", NULL, progress_callback);
	//http_client::getInstance()->http_get("https://gtz.io/gtz_latest.run", "/home/nil/tmp/gtz_latest.run", NULL, progress_callback);
	return 0;
}