#!/bin/bash
openssl genrsa -out node-key.pem 2048
openssl req -new -sha256 -extensions v3_ca -subj '/CN=192.168.1.100' -key node-key.pem -out node-csr.pem
openssl x509 -req -in node-csr.pem -signkey node-key.pem -out node-cert.pem -days 1826
