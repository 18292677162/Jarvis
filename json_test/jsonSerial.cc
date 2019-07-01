#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <json/json.h>

int main()
{
  Json::Value root;
  Json::StreamWriterBuilder hsy;
  std::ostringstream os;

  root["Name"] = "Pinna";
  root["Age"] = 20;
  root["Lang"] = "C++";

  std::unique_ptr<Json::StreamWriter> jsonWriter(hsy.newStreamWriter());
  jsonWriter->write(root, &os);
  std::string s = os.str();

  std::cout << s <<std::endl;

  return 0;
}
