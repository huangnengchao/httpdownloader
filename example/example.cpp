#include "../include/http_downloader.h"
#include <iostream>
#include <functional>


void callback(const progress_info& pinfo) {
	std::cout << pinfo.download_speed.c_str() << "\t" << pinfo.remaining_time.c_str() << "\t" << pinfo.progress_percentage << "%" << std::endl;
}


int main()
{

	http_downloader hd;

	//set callback
	hd.set_callback(std::bind(&callback, std::placeholders::_1));

	//http
	//hd.download_file("http://ftp.sjtu.edu.cn/ubuntu-cd/18.04.1/ubuntu-18.04.1-live-server-amd64.iso", "ubuntu-18.04.1-live-server-amd64.iso");

	//https  启动下载慢
	hd.download_file("https://raw.githubusercontent.com/Genetalks/gtz/master/gtz_latest.run", "gtz_latest.run");

	//https
	//hd.download_file("https://gtz.io/gtz_latest.run", "gtz_latest.run");

	return 0;
}
