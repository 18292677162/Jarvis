#include <iostream>
#include <json/json.h>
#include <string>
#include <sstream>
#include <memory>

std::string url = "www.baidu.com";
std::string api_key = "123456";
std::string user_id = "1";

void DataToJson(std::string &message)
    {
    	std::string jsonStr;
    	Json::StreamWriterBuilder writerBuilder;
    	std::ostringstream os;
         // 根 JSON 数据
        Json::Value root;
        // 一级 JSON 数据
        Json::Value item1;
        // 二级 JSON 数据
        Json::Value item2;

        // 编码二级 JSON 数据
        // 编码一级 JSON 数据
        // 编码根 JSON 数据
        root["reqType"] = 0;

        item2["text"] = message;
        item1["inputText"] = item2;
        root["perception"] = item1;

        item1.clear();
        item2.clear();

        item1["api_key"] = api_key;
        item1["user_id"] = user_id;
        root["userInfo"] = item1;

        item1.clear();

        std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    	jsonWriter->write(root, &os);
    	jsonStr = os.str();

        std::cout << "Json:\n" << jsonStr << std::endl;
    }
int main(int argc, char const *argv[])
{

	std::string s = "hello hahaha";
	DataToJson(s);
	return 0;
}