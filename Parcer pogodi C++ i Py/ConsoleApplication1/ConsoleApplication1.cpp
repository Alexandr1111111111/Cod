#include <iostream>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <math.h>
using namespace httplib;
using json = nlohmann::json;

void gen_response1(const Request& req, Response& res) {
    Client cli("http://worldtimeapi.org");
    auto clientTime = cli.Get("/api/timezone/Europe/Simferopol");
    Client cli1("http://api.openweathermap.org");
    auto clientWeather = cli1.Get("/data/2.5/onecall?lat=44.5653&lon=34.0615&lang=ru&units=metric&exclude=current,minutely,daily,alerts&appid=24f33d0dd36e14aec37d509c6d295298");
    if (clientTime) {
        int error = clientTime->status;
        if (clientTime->status == 200) {

            json time = json::parse(clientTime->body);
            json weather = json::parse(clientWeather->body);
            for (int i = 0; i < 48; i++) {

                if (weather["hourly"][i]["dt"].get<ULONG64>() > time["unixtime"].get<ULONG64>()) {
                    std::ifstream file("123.txt");
                    std::string text;
                    getline(file, text, '\0');
                    file.close();
                    std::string first = "{hourly[i].weather[0].description}";
                    std::string second = "{hourly[i].weather[0].icon}";
                    std::string third = "{hourly[i].temp}";
                    text.replace(text.find(first), first.size(), weather["hourly"][i]["weather"][0]["description"]);
                    text.replace(text.find(second), second.size(), weather["hourly"][i]["weather"][0]["icon"]);
                    text.replace(text.find(third), third.size(), std::to_string(int(round(weather["hourly"][i]["temp"].get<double>()))));
                    text.replace(text.find(third), third.size(), std::to_string(int(round(weather["hourly"][i]["temp"].get<double>()))));
                    res.set_content(text, "text/html;  charset=UTF-8");
                    break;

                }
            }


        }
        else
        {

            res.set_content(std::to_string(error), "text/plain");
        }
    }
    else  res.set_content("errorq", "text/plain");
}

void gen_response(const Request& req, Response& res)
{
    Client cli("http://worldtimeapi.org");
    auto clientTime = cli.Get("/api/timezone/Europe/Simferopol");
    Client cli1("http://api.openweathermap.org");
    auto clientWeather = cli1.Get("/data/2.5/onecall?lat=44.5653&lon=34.0615&lang=ru&units=metric&exclude=current,minutely,daily,alerts&appid=24f33d0dd36e14aec37d509c6d295298");
    if (clientTime) {
        int error = clientTime->status;
        if (clientTime->status == 200) {

            json time = json::parse(clientTime->body);
            json weather = json::parse(clientWeather->body);
            for (int i = 0; i < 48; i++) {

                if (weather["hourly"][i]["dt"].get<ULONG64>() > time["unixtime"].get<ULONG64>()) {
                    json new12;
                    new12["Temperature"] = std::to_string(int(round(weather["hourly"][i]["temp"].get<double>())));
                    new12["description_12"] = weather["hourly"][i]["weather"][0]["description"];
                    std::string output = new12.dump(4);
                    res.set_content(output, "text/json");

                    break;
                }
            }


        }
        else {

            res.set_content(std::to_string(error), "text/plain");
        }
    }
    else  res.set_content("errorq", "text/plain");
}
int main()
{
    Server svr;
    svr.Get("/", gen_response1);
    svr.Get("/raw", gen_response);
    std::cout << "Start server... OK\n";
    svr.listen("localhost", 1234);
}