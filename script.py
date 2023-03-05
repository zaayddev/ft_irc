# This script make sure that the server is capable of 
# handling multiple clients[num_users] at the same time and never hang

import subprocess
import time

# Set up the necessary variables
host = 'localhost'
port = 8080
password = 'password'
nickname = 'zaid'
username = 'zaid'
channel = '#1337'
num_users = 100

# Connect to the IRC server using netcat
nc_process = subprocess.Popen(['nc', '-c', host, str(port)], stdin=subprocess.PIPE)

# Send the PASS command to authenticate
nc_process.stdin.write(('PASS ' + password + '\n').encode())

# Wait a moment before sending the NICK and USER commands
time.sleep(0.1)

# Send the NICK and USER commands
nc_process.stdin.write(('NICK ' + nickname + '\n').encode())
time.sleep(0.1)
nc_process.stdin.write(('USER ' + username + ' ' + username + ' ' + username + ' :My real name\n').encode())

# Wait a moment before joining the channel
time.sleep(0.1)

# Join the channel
nc_process.stdin.write(('JOIN ' + channel + '\n').encode())

# Repeat the process for the desired number of users
for i in range(num_users - 1):
    # Wait for the previous user to finish connecting
    time.sleep(0.1)

    # Connect to the IRC server using netcat
    nc_process = subprocess.Popen(['nc', '-c', host, str(port)], stdin=subprocess.PIPE)

    # Send the PASS command to authenticate
    nc_process.stdin.write(('PASS ' + password + '\n').encode())

    # Wait a moment before sending the NICK and USER commands
    time.sleep(0.1)

    # Send the NICK and USER commands
    nc_process.stdin.write(('NICK ' + nickname + str(i + 1) + '\n').encode())
    time.sleep(0.1)
    nc_process.stdin.write(('USER ' + username + str(i + 1) + ' ' + username + str(i + 1) + ' ' + username + str(i + 1) + ' :My real name\n').encode())

    # Wait a moment before joining the channel
    time.sleep(0.1)

    # Join the channel
    nc_process.stdin.write(('JOIN ' + channel + '\n').encode())

# Wait for all the users to finish connecting
time.sleep(0.1)
