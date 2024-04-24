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
    /*
        `httpMethods`
        - Defines the HTTP method, between GET and POST
    */
    enum httpMethods
    {
        GET,
        POST
    };

    /*
        `httpRedirectOption`
        - Whether CURL should follow the connection even if it is moving permanently.
    */
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
        int status_code;
        std::string body;
    };

    struct httpOptions
    {
        std::string url;
        std::string body;
        httpMethods method = GET;
        httpRedirectOption redirect = follow;
        bool verbose = false;
        std::map<std::string, std::string> headers;
    };

    class httpRequest
    {
    public:
        CURL *curl;
        httpOptions options;
        std::string res_body;

        httpRequest(const httpOptions &options);

        httpResponse send();

        httpRequest(const httpRequest &) = delete;

        ~httpRequest()
        {
            curl_easy_cleanup(curl);
        }
    };

    httpResponse get(const std::string_view &url, const httpRequestOptions &options = httpRequestOptions{});
}

