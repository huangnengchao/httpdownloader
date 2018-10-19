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
	hd.download_file("https://gtzdata.oss-cn-hangzhou.aliyuncs.com/Ailuropoda_melanoleuca_9d59370cb06760b671353b20224ec2de.rbin", "/home/nil/dir_not_exist/Ailuropoda_melanoleuca_9d59370cb06760b671353b20224ec2de.rbin", std::bind(&callback, std::placeholders::_1));
	hd.download_file("https://gtzdata.oss-cn-hangzhou.aliyuncs.com/Apis_mellifera_10a768025aad33307a53bc077b60e4c8.rbin", "Apis_mellifera_10a768025aad33307a53bc077b60e4c8.rbin" , std::bind(&callback, std::placeholders::_1));
	//hd.download_file("https://gtz.io/gtz_latest.run", "gtz_latest2.run" , std::bind(&callback, std::placeholders::_1));
	//https  启动下载慢
	//hd.download_file("https://raw.githubusercontent.com/Genetalks/gtz/master/gtz_latest.run", "gtz_latest.run");
	return 0;
}
