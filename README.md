# 🔐 UTXO-Based Certificate Revocation for Mozilla

This branch introduces a novel certificate revocation mechanism into Mozilla Firefox, leveraging the Bitcoin blockchain as a decentralized, transparent, and tamper-resistant infrastructure for status verification.

## 📌 About This Branch

This branch (`utxo-mods`) is a customized fork of Mozilla's source code, modified to support certificate revocation using Bitcoin's UTXO (Unspent Transaction Output) model. The standard OCSP/CRL revocation mechanisms are replaced or supplemented with a decentralized alternative that:

- Embeds a UTXO reference directly into the X.509 v3 certificate as a custom extension.
- Allows certificate revocation to be triggered by spending the associated UTXO.
- Enables verifiers (e.g., the browser) to determine certificate validity by querying a local Bitcoin node for the UTXO status.

## 💡 Motivation

Traditional certificate revocation mechanisms like CRLs and OCSP suffer from privacy leaks, single points of failure, and limited scalability. This work aims to:

- Improve user privacy by eliminating OCSP requests to third-party responders.
- Avoid centralized points of trust or failure.
- Provide verifiable and auditable certificate status checks via the blockchain.

## ⚙️ Technical Overview

- The browser’s certificate verification engine was modified to recognize a custom OID (`1.3.112.4.30.1270`) embedded in the certificate.
- When enabled via preferences, the browser extracts the `txid` and `vout` from the certificate, queries a local Bitcoin node using RPC, and validates the certificate based on whether the UTXO is still unspent.
- If the UTXO is spent, the certificate is considered revoked.

## 🧪 Status

This work is in the experimental/research phase and not intended for production use. It demonstrates the feasibility of blockchain-based revocation mechanisms within the Firefox codebase.

## 📚 Related Sections

- UTXO parsing logic: `CertVerifier.cpp`
- NSS OID registration: `secoidt.h`, `secoid.c`
- RPC querying implementation: `BitcoinHandler.cpp`

## 📎 Notes

- Tested on the Bitcoin **testnet**.
- Configuration options available in `about:config` under `security.bitcoin.utxo.enabled`.

## 👤 Author

Developed by [@imiguelrodriguez](https://github.com/imiguelrodriguez) as part of a research project on decentralized revocation methods.
