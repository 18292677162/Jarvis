#ifndef __JARVIS__
#define __JARVIS__

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <map>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
#include <fcntl.h>
#include <unordered_map>
#include "speech.h"
#include "base/http.h"
#include "curl/curl.h"

//#define SPEECH_FILE "temp_file/demo.wav"
//#define PLAY_FILE "temp_file/play.mp3"
//#define CMD_ETC "command.etc"

// 语音识别类
class SpeechRec
{
public:
    SpeechRec();
    ~SpeechRec();
};

// 图灵机器人类
class InterRobot
{
public:
    InterRobot()
    {}

    Json::Value Post_Res(Json::Value data)
    {
        // 存储 字符串 应答
        Json::Value obj;
        std::string res;
        JSONCPP_STRING errs;
        Json::CharReaderBuilder b;
        int ret = 0;
        // 发送请求
        ret = client.post(url, nullptr, data, nullptr, &res);
        //if (CURLcode::CURLE_OK != ret) {
        //  obj[aip::CURL_ERROR_CODE] = ret;
        //  return obj;
        //}

        std::unique_ptr<Json::CharReader> JsonReader(b.newCharReader());
        JsonReader->parse(res.data(), res.data() + res.size(), &obj, &errs);
        return obj;
        /*
        int post(
                 std::string url,
                 std::map<std::string, std::string> const * params,
                 const std::string & body,
                 std::map<std::string, std::string> const * headers,
                 std::string * response)
        */

    }
    
    void DataToJson( std::string &mesage)
    {
    /*
    {
        "reqType":0,
        "perception": {
            "inputText": {
                "text": "附近的酒店"
            },
            "inputImage": {
                "url": "imageUrl"
            },
            "selfInfo": {
                "location": {
                    "city": "北京",
                    "province": "北京",
                    "street": "信息路"
                }
            }
        },
        "userInfo": {
            "apiKey": "",
            "userId": ""
        }
    }
    */
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

        item2["text"] = mesage;
        item1["inputText"] = item2;
        root["perception"] = item1;

        item1.clear();
        item2.clear();

        item1["api_key"] = api_key;
        item1["user_id"] = user_id;
        root["userInfo"] = item1;

        item1.clear();
    }
    
    std::string Talk( std::string &mesage)
    {
        /*
        {
            "intent": {
                "code": 10005,
                "intentName": "",
                "actionName": "",
                "parameters": {
                    "nearby_place": "酒店"
                }
            },
            "results": [
                {
                    "groupType": 1,
                    "resultType": "url",
                    "values": {
                        "url": "http://m.elong.com/hotel/0101/nlist/#indate=2016-12-10&outdate=2016-12-11&keywords=%E4%BF%A1%E6%81%AF%E8%B7%AF"
                    }
                },
                {
                    "groupType": 1,
                    "resultType": "text",
                    "values": {
                        "text": "亲，已帮你找到相关酒店信息"
                    }
                }
            ]
        }
        */
        // 存储 Json 回复
        Json::Value res;
        Json::Value intent;
        Json::Value results;
        Json::Value values;

        // 转码 Json
        // 将发送的数据序列化
        DataToJson(mesage);
        // Json 回复
        res = Post_Res(&mesage);

        // 将收到的数据反序列化
        intent = res["intent"];
        results = res["results"];
        values = results[0]["values"];

        // 输出
        std::cout << "[Robot]:" << values["text"].asString() << std::endl;
        return values["text"].asString();
    }

    ~InterRobot()
    {}

private:
    // 图灵 url key id
    std::string url = "www.baidu.com";
    std::string api_key = "123456";
    std::string user_id = "1";

    // 百度的Http API
    aip::HttpClient client;
};

// Jarvis 业务逻辑
class Jarvis{
public:
    void Run() {
        InterRobot robot;
        std::string s;
        while(1) {        
            getline(std::cin, s);
            robot.Talk(s);
        }     
    }

};

#endif