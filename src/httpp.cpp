// httpp.cpp : Defines the entry point for the application.
//

#include "httpp.h"

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string *)userp)->append((char *)contents, size * nmemb);
	return size * nmemb;
}

size_t ReadCallback(char *buffer, size_t size, size_t nitems, std::string *data)
{
	size_t len = size * nitems;
	size_t total_size = data->size();
	if (total_size > 0)
	{
		size_t copy_size = min(len, total_size);
		memcpy(buffer, data->c_str(), copy_size);
		data->erase(0, copy_size);
		return copy_size;
	}
	return 0;
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
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

			switch (this->options.method)
			{
			case GET:
				curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
				break;
			case POST:
				curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
				break;
			case PUT:
				curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
				break;
			case PATCH:
				curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
				break;
			}
			
			if (!this->options.cookies.empty())
				curl_easy_setopt(curl, CURLOPT_COOKIE, this->options.cookies.c_str());

			// headers:

			struct curl_slist *list = NULL;

			auto headers = this->options.headers;

			for (const auto &[key, value] : headers)
			{
				list = curl_slist_append(list, std::format("{}: {}", key, value).c_str());
				fmt::println("HERE: {}", std::format("{}: {}", key, value));
			}

			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

			if (this->options.url.rfind("https://", 0) == 0)
				curl_easy_setopt(curl, CURLOPT_PORT, 443L);

			if (this->options.method == POST || this->options.method == PATCH || this->options.method == PUT)
			{
				curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadCallback);
				curl_easy_setopt(curl, CURLOPT_READDATA, &this->options.body);
			}

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

		httpResponse response;

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

		if (code != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n\nTip: Check the URL, headers or body you have been passing to httpp.\n\n",
					curl_easy_strerror(code));

		response.status_code = response_code;
		response.body = response_body;

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

	httpResponse post(const std::string_view &url, const std::string_view &body, const httpRequestOptions &options)
	{
		httpOptions ops;
		ops.url = url;
		ops.body = options.body.empty() ? body : options.body;
		ops.headers = options.headers;
		ops.method = POST;
		ops.redirect = follow;

		ops.headers.insert({"Referrer", std::string{url}});

		httpRequest req(ops);

		httpResponse resp = req.send();

		return resp;
	}
}
