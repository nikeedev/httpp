// httpp.cpp : Defines the entry point for the application.
//

#include "httpp.h"

namespace httpp {
	
	httpRequest::httpRequest(const httpOptions &options) {
		if (options.url.empty()) {
			throw std::invalid_argument("url cannot be empty");
		}
		this->options = options;

		curl = curl_easy_init();

		if (curl != nullptr) {
			curl_easy_setopt(curl, CURLOPT_URL, this->options.url);

			switch (options.redirect)
			{
			case follow:
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
				break;
			
			default:
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);
				break;
			}
		} else {
			throw std::runtime_error("curl init failed");
		}
	};

	httpResponse httpRequest::send() {
		CURLcode code;

		code = curl_easy_perform(curl);

		httpResponse response{};

		return response;
	}

	void get(const std::string_view &url, const httpRequestOptions &options) {
		httpOptions ops;
		ops.url = url;
		ops.body = options.body;
		ops.headers = options.headers;
		ops.method = GET;

		ops.headers.insert({"Referrer", options.referrer.c_str()});
		
		httpRequest req(ops);

		httpResponse resp = req.send();
		
	}
}
