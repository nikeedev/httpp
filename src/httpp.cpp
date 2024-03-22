// httpp.cpp : Defines the entry point for the application.
//

#include "httpp.h"


namespace httpp {
	
	httpRequest::httpRequest(const httpRequestOptions &options) {
		if (options.url.empty()) {
			throw std::invalid_argument("url cannot be empty");
		}
		this->options = options;
	};

	void get(const httpRequestOptions &url, const httpOptions &options) {
		
	}
}
