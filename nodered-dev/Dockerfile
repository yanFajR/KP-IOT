FROM node:slim

# Install NodeRED
RUN npm install -g --unsafe-perm node-red

# Install NodeRED admin
RUN npm install -g node-red-admin

# NodeRED Dashboard
RUN npm install -g node-red-dashboard

# Start NodeRED
ENTRYPOINT ["node-red", "--userDir", "/data"]




