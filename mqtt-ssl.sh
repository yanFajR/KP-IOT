openssl req -new -x509 -days 3650 -extensions v3_ca -keyout ca.key -out ca.crt -nodes -subj '/CN=192.168.1.100'
openssl genrsa -out server.key 2048
openssl req -out server.csr -key server.key -new -sha256 -subj '/CN=192.168.1.100'
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 1826
openssl genrsa -out client.key 2048
openssl req -out client.csr -key client.key -new -sha256 -subj '/CN=192.168.1.100'
openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt -days 1826
