#include "../include/http_downloader.h"
#include <iostream>
#include <functional>

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