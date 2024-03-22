// httpp.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fmt/core.h>
#include <string>
#include <string_view>
#include <curl/curl.h>
#include <map>

namespace httpp
{   
    enum httpMethods
    {
        GET,
        POST
    };

    enum httpRedirectOption {
        follow,
        error
    };

    struct httpRequestOptions {
        std::string body;
        std::map<std::string, std::string> headers;
        std::string mode = "cors";
        httpRedirectOption redirect = follow;
        std::string referrer;
    };

    
    class httpResponse {
    public:
        std::map<std::string, std::string> headers;
        CURLcode status_code;
    };

    struct httpOptions
    {
        std::string url;
        std::string body;
        httpMethods method = GET;
        httpRedirectOption redirect = follow;
        std::map<std::string, std::string> headers;
    };

    class httpRequest
    {
    public:
        CURL *curl;
        httpOptions options;

        httpRequest(const httpOptions &options);

        httpResponse send();

        httpRequest(const httpRequest &) = delete;

        ~httpRequest()
        {
            curl_easy_cleanup(curl);
        }
    };

    void get(const std::string_view &url, const httpRequestOptions &options = httpRequestOptions{});
}

