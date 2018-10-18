#include "../include/http_downloader.h"
#include <iostream>
#include <functional>


void callback(const progress_info& pinfo) {
	std::cout << pinfo.download_speed.c_str() << "\t" << pinfo.remaining_time.c_str() << "\t" << pinfo.progress_percentage << "%" << std::endl;
}


int main()
{
	http_downloader hd;
	//http
	//hd.download_file("http://ftp.sjtu.edu.cn/ubuntu-cd/18.04.1/ubuntu-18.04.1-live-server-amd64.iso", "ubuntu-18.04.1-live-server-amd64.iso");
	hd.download_file("http://mirrors.ustc.edu.cn/qtproject/archive/qt/5.11/5.11.1/single/qt-everywhere-src-5.11.1.tar.xz", "qt-everywhere-src-5.11.1.tar.xz" , std::bind(&callback, std::placeholders::_1));
	{
		http_downloader hd;
		hd.download_file("https://gtz.io/gtz_latest.run", "gtz_latest2.run" , std::bind(&callback, std::placeholders::_1));
	}
	//https  启动下载慢
	//hd.download_file("https://raw.githubusercontent.com/Genetalks/gtz/master/gtz_latest.run", "gtz_latest.run");
	return 0;
}
