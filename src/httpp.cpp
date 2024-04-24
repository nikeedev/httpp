// httpp.cpp : Defines the entry point for the application.
//

#include "httpp.h"

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string *)userp)->append((char *)contents, size * nmemb);
	return size * nmemb;
}

namespace httpp
{

	httpRequest::httpRequest(const httpOptions &options)
	{
		if (options.url.empty())
		{
			throw std::invalid_argument("url cannot be empty");
		}

		this->options = options;

		curl = curl_easy_init();

		if (curl != nullptr)
		{
			curl_easy_setopt(curl, CURLOPT_URL, this->options.url.c_str());
			curl_easy_setopt(curl, CURLOPT_VERBOSE, options.verbose ? 1L : 0L);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res_body);

			switch (options.redirect)
			{
			case follow:
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
				break;

			default:
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);
				break;
			}
		}
		else
		{
			throw std::runtime_error("curl init failed");
		}
	};

	httpResponse httpRequest::send()
	{
		CURLcode code;
		long response_code;

		code = curl_easy_perform(curl);

		if (code != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n\nTip: Check the URL, headers or body you have been passing to httpp.\n\n",
					curl_easy_strerror(code));

		httpResponse response;

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

		response.status_code = response_code;
		response.body = res_body;

		return response;
	}

	httpResponse get(const std::string_view &url, const httpRequestOptions &options)
	{
		httpOptions ops;
		ops.url = url;
		ops.body = "";
		ops.headers = options.headers;
		ops.method = GET;
		ops.redirect = follow;

		ops.headers.insert({"Referrer", std::string{url}});

		httpRequest req(ops);

		httpResponse resp = req.send();

		return resp;
	}
}
