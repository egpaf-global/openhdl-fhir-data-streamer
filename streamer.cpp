#include <iostream>
#include <sstream>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <nlohmann/json.hpp>

using namespace Poco::Net;
using namespace Poco;
using json = nlohmann::json;

class FHIRRequestHandler : public HTTPRequestHandler {
public:
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override {
        std::stringstream responseStream;

        // Create a FHIR Patient resource
        json patientResource;
        patientResource["resourceType"] = "Patient";
        patientResource["id"] = "1";
        patientResource["name"] = {{"given": {"John"}, {"family": "Doe"}}};

        // Serialize the FHIR resource to JSON
        responseStream << patientResource.dump(2);

        // Send HTTP response
        response.setStatus(HTTPResponse::HTTP_OK);
        response.setContentType("application/json");
        response.send() << responseStream.str();
    }
};

int main() {
    HTTPServer server(new FHIRRequestHandlerFactory, ServerSocket(8080), new HTTPServerParams);

    std::cout << "Server starting on port 8080..." << std::endl;
    server.start();
    server.waitForTermination();

    return 0;
}
