//
// Created by User on 22/11/2024.
//

#ifndef BITCOINHANDLER_H
#define BITCOINHANDLER_H

#include <string>

//nsresult QueryBitcoin(const std::string& txid, const std::string& vout, nsCString& response);
nsresult QueryBitcoinTxOut(const std::string& txid, const std::string& vout, std::string& response);


#endif //BITCOINHANDLER_H
