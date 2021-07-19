FROM kp/nodered-dev

# NodeRED Gauge for Dashboard
RUN npm install -g node-red-contrib-ui-artless-gauge

# NodeRED Login System
RUN npm install -g node-red-contrib-users

# NodeRED MySQL Connector
RUN npm install -g node-red-node-mysql

#NodeRED Crypto.js Dynamic
RUN npm install -g node-red-contrib-crypto-js-dynamic

#WORKDIR
WORKDIR /usr/src/nodered

# Update Node.js Package
RUN npm update
