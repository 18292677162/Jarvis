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

#define SPEECH_FILE "../temp_file/demo.wav"
#define PLAY_FILE "../temp_file/play.mp3"
#define CMD_ETC "../command.etc"

using namespace std;

// 语音识别类
class SpeechRec
{
public:
    SpeechRec()
    {
        client = new aip::Speech(app_id, api_key, secret_key);
    }

    /**
    * ASR语音识别示例
    */
    void ASR(int &err_code, std::string &message) {
        std::cout << std::endl << "正在识别, 请稍后...(〃'▽'〃)" << std::endl;
        
        std::map<std::string, std::string> options;
        // 中文显示
        options["dev_pid"] = "1536";
        //options["lan"] = "ZH"; 定义dev_pid会被覆盖，已废弃

        // 获取文件
        std::string file_content;
        aip::get_file_content(SPEECH_FILE, &file_content);
        // 频率16k wav格式
        Json::Value result = client->recognize(file_content, "wav", 16000, options);
        // 成功返回
        /*
        {
            "err_no": 0,
            "err_msg": "success.",
            "corpus_no": "15984125203285346378",
            "sn": "481D633F-73BA-726F-49EF-8659ACCC2F3D",
            "result": ["北京天气"]
        }

        // 失败返回
        {
            "err_no": 2000,
            "err_msg": "data empty.",
            "sn": null
        }
        */
        err_code = result["err_no"].asInt();
        if (0 == err_code) {
            message = result["result"][0].asString();
        } else {
            message = "识别失败...(╥╯^╰╥)";
        }

        std::cout << "语音识别本地文件结果:" << std::endl << result.toStyledString();
    }

    /**
    * TTS语音合成示例
    */
    /*
    void TTS(aip::Speech* client) {
        std::ofstream ofile;
        std::string file_ret;
        std::map<std::string, std::string> options;
        options["spd"] = "5";
        options["per"] = "2";
        ofile.open("./tts.mp3", std::ios::out | std::ios::binary);
        Json::Value result = client->text2audio("百度语音合成测试", options, file_ret);
        // 如果file_ret为不为空则说明合成成功，返回mp3文件内容
        if (!file_ret.empty())
        {
            // 合成成功保存文件
            ofile << file_ret;
            std::cout << "语音合成成功，打开目录下的tts.mp3文件听听看" << std::endl;
        } else {
            // 合成出错，打印错误信息
            std::cout << result.toStyledString();
        }
    }
    */

    ~SpeechRec()
    {
        delete client;
        client = nullptr;
    }
private:
    std::string app_id = "16618182";
    std::string api_key = "SzXMnD2e7x5Nv86O0Szkc9sD";
    std::string secret_key = "BV8uUFxNitDx5Hgtv7dtl3fqWirNcRPY";

    aip::Speech *client;
    std::string message;
};

// 图灵机器人类
class InterRobot
{
public:
    InterRobot()
    {}

    std::string Post_Res(std::string &data)
    {
        // 存储 Json 回复
        Json::Value res;
        Json::Value results;
        Json::Value values;
        JSONCPP_STRING errs;
        // 存储 字符串 应答
        Json::Value root;
        std::string body;
        Json::CharReaderBuilder rb;
        // 发送请求
        client.post(url, nullptr, data, nullptr, &body);

        std::unique_ptr<Json::CharReader> JsonReader(rb.newCharReader());
        bool ret = JsonReader->parse(body.data(), body.data() + body.size(), &root, &errs);
        if(!ret || !errs.empty()) {
            return "";
        }

        // 将收到的数据反序列化
        results = root["results"];
        values = results[0]["values"];

        // 输出
        std::cout << "[ヾ(๑╹◡╹)/]:" << values["text"].asString() << std::endl;
        
        return values["text"].asString();
        /*
        int post(
                 std::string url,
                 std::map<std::string, std::string> const * params,
                 const std::string & body,
                 std::map<std::string, std::string> const * headers,
                 std::string * response)
        */

    }
    
    std::string DataToJson(std::string &message)
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

        item2["text"] = message;
        item1["inputText"] = item2;
        root["perception"] = item1;

        item1.clear();
        item2.clear();

        item1["apiKey"] = api_key;
        item1["userId"] = user_id;
        root["userInfo"] = item1;

        item1.clear();

        Json::StreamWriterBuilder wb;
        ostringstream os;
        std::unique_ptr<Json::StreamWriter> jw(wb.newStreamWriter());
        jw->write(root, &os);
        return os.str();
    }
    
    std::string Talk(std::string message)
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
        string res;
        // 转码 Json
        // 将发送的数据序列化
        std::string body = DataToJson(message);
        // Json 回复
        res = Post_Res(body);
        return res;
    }

    ~InterRobot()
    {}

private:
    // 图灵 url key id
    std::string url = "http://openapi.tuling123.com/openapi/api/v2";
    std::string api_key = "e9f829191d9a453fb4dce0c3d004aa13";
    std::string user_id = "1";

    // 百度的Http API
    aip::HttpClient client;
};

// Jarvis 业务逻辑
class Jarvis{
public:
    void Run() {
        InterRobot robot;
        std::cout << "[ヾ(๑╹◡╹)/]:小娜上线啦! 主人请吩咐..." << std::endl;
        while(1) { 
            message = "";
            getline(std::cin, message);
            // bool ret = RecordASR(message);
            robot.Talk(message);
        }     
    }

    // 执行命令
    bool Exec(std::string command, bool print) {
        FILE *fp = popen(command.c_str(), "r");
        if (nullptr == fp)
        {
            std::cerr << "popen error!" << std::endl;
            return false;
        }
        if (print)
        {
            char c;
            std::cout << "-----------start-----------" << std::endl;
            while (fread(&c, 1, 1, fp) > 0) {
                std::cout << c;
            }
            std::cout << "------------end------------" << std::endl;
        }
        pclose(fp);
        return true;
    }

    bool RecordASR(std::string &message) {
        int err_code = -1;
        std::string record = "arecord -t wav -c 1 -r 16000 -d 5 -f S16_LE";
        record += SPEECH_FILE;
        // 将标准输出写入null
        record += ">/dev/null 2>$1";

        std::cout << "请讲...ヾ(๑╹◡╹)/";
        fflush(stdout);
        if (Exec(record, false))
        {
            sr.ASR(err_code, message);
            if (0 == err_code) {
                return true;
            }
            std::cout << "语音识别失败...〒▽〒" << std::endl;
        } else {
            std::cout << "录制失败...〒▽〒" << std::endl;
        }
        return false;
    }

private:
    SpeechRec sr;
    InterRobot robot;
    std::unordered_map<std::string, std::string> command_set;
    std::string message;
};

#endif