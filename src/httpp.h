// httpp.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fmt/core.h>
#include <string>
#include <curl/curl.h>
#include <vector>

namespace httpp
{   
    struct httpHeader {
        std::string key;
        std::string value;
    };

    enum httpMethods
    {
        GET,
        POST
    };

    struct httpRequestOptions
    {
        std::string url;
        httpMethods method = GET;
        std::vector<httpHeader> headers;
    };

    class httpRequest
    {
        CURL *curl;
        CURLcode status_code;
        httpRequestOptions options;

    public:
        httpRequest(const httpRequestOptions &options);

        httpRequest(const httpRequest &) = delete;

    private:
        ~httpRequest()
        {
            curl_easy_cleanup(curl);
        }
    };

    enum httpRedirectOption {
        follow,
        error
    };

    struct httpOptions {
        std::vector<httpHeader> headers;
        std::string mode = "cors";
        httpRedirectOption redirect = follow;
        std::string referrer;  
    };

    void get(const std::string_view &url, const httpOptions &options = httpOptions{});
}

