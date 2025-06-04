#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <sstream> // For string streams
#include <iomanip> // For std::setw and std::setfill
#include "nsString.h"

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

// Function to base64 encode a string
std::string base64_encode(const std::string& input) {
    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string output;
    int val = 0, valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            output.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) output.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (output.size() % 4) output.push_back('=');
    return output;
}

// Function to query Bitcoin for txout using txid and vout
nsresult QueryBitcoinTxOut(const std::string& txid, const std::string& vout, std::string& response) {
  // Initialize Winsock
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    return NS_ERROR_FAILURE;
  }

          // Create a socket
  SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == INVALID_SOCKET) {
    WSACleanup();
    return NS_ERROR_FAILURE;
  }

          // Define the server address
  sockaddr_in serverAddr{};
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(18332);
  inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

          // Connect to the server
  if (connect(sock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
    closesocket(sock);
    WSACleanup();
    return NS_ERROR_FAILURE;
  }

          // Username and password for authentication
  std::string username = "your_rpc_username";  // Replace with your username
  std::string password = "your_rpc_password";  // Replace with your password

          // Base64 encode the credentials
  std::string credentials = username + ":" + password;
  std::string encodedCredentials = base64_encode(credentials);

          // Prepare the RPC call
  std::string rpcCall = R"({
        "jsonrpc": "1.0",
        "id": "curltext",
        "method": "gettxout",
        "params": ["<txid>", <vout>, true]
    })";

          // Replace <txid> and <vout> with actual values
  size_t posTxid = rpcCall.find("<txid>");
  rpcCall.replace(posTxid, 6, txid);
  size_t posVout = rpcCall.find("<vout>");
  rpcCall.replace(posVout, 6, vout);

          // Add HTTP headers to the request
  std::string request = "POST / HTTP/1.1\r\n";
  request += "Host: 127.0.0.1\r\n";  // Replace with your server's host
  request += "Content-Type: application/json\r\n";
  request += "Content-Length: " + std::to_string(rpcCall.length()) + "\r\n";
  request += "Authorization: Basic " + encodedCredentials + "\r\n";  // Add Authorization header
  request += "\r\n";
  request += rpcCall;

          // Send the request to the server
  if (send(sock, request.c_str(), request.length(), 0) == SOCKET_ERROR) {
    closesocket(sock);
    WSACleanup();
    return NS_ERROR_FAILURE;
  }

          // Receive the response from the server
  char buffer[2048];  // Adjust buffer size as needed
  int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
  if (bytesReceived == SOCKET_ERROR) {
    closesocket(sock);
    WSACleanup();
    return NS_ERROR_FAILURE;
  }

          // Null-terminate the received data
  buffer[bytesReceived] = '\0';

          // You can process the response further here if needed. For now, let's assume
          // a successful transaction return.
  std::cout << "Received response: " << buffer << std::endl;
  response = std::string(buffer);

          // Cleanup
  closesocket(sock);
  WSACleanup();

          // Return success
  return NS_OK;
}  
