# TCP_Banking_Server
In this project, my aim was to create a banking server that should use TCP to establish communication with the client side. The banking operations requested by the client side are all sent to this server side to get checked and then confirmed. As of right now however, only signing in to the bank account is implemented. I'm looking forward to improving it in the future.

## How to use:
- Connect the server and the client to the same network.
- The client is going to connect to the server from it's IPv4 address on the same network. Because of this you need to find out your IPv4 address. To do this,
  - Press `Win + R` on your keyboard.
  - Type `cmd`.
  - When the command prompt window opens, type in `ipconfig` and press `Enter`.
  - You should be able to see your IPv4 address in the network you're connected to.
  - Copy it and change the specified location of code in the CLIENT side.
- In order for the server side to check banking information of users, you need to change it's directory to your own case. You can do this by opening the .sln file in Visual Studio, navigating to the 101st line of the TCP_Banking_Server_2.cpp file. The bankInformation.txt file is also included in the repository under the data directory.
- After all of these steps, you should be able to run the .sln file in Visual Studio.
