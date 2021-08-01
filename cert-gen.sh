# Edit this value according your server's IP Address/Hostname
CN='/CN=104.43.104.64'
# Generate Certificate  & Key for CA
openssl req -new -x509 -days 3650 -extensions v3_ca -keyout ca.key -out ca.crt -nodes -subj $CN

# Generate Certificate  & Key for Server
openssl genrsa -out server.key 2048
openssl req -out server.csr -key server.key -new -sha256 -subj $CN
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 1826

# Generate Certificate  & Key for Client
openssl genrsa -out client.key 2048
openssl req -out client.csr -key client.key -new -sha256 -subj $CN
openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt -days 1826

# Convert to PEM
cp server.crt node-cert.pem
cp server.key node-key.pem
