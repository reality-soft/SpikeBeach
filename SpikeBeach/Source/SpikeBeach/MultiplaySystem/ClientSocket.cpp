// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientSocket.h"
#include "Packets/PacketId.h"
#include "Packets/Requests/GameEnterReq.h"
#include "Packets/Requests/SyncReq.h"
#include "Packets/Responses/GameEnterRes.h"
#include "Packets/Responses/SyncRes.h"

ClientSocket::ClientSocket(const char* serverIP, 
    int serverPort, 
    FString userAssignedId,
    FString token,
    FString clientVersion,
    INT32 gameId) : userAssignedId(userAssignedId), token(token), clientVersion(clientVersion), gameId(gameId)
{
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            UE_LOG(LogTemp, Display, TEXT("Failed to initialize winsock2."));
            exit(EXIT_FAILURE);
        }

        socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (socketDescriptor == INVALID_SOCKET) {
            UE_LOG(LogTemp, Display, TEXT("Error creating socket."));
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(serverPort);
        if (inet_pton(AF_INET, serverIP, &serverAddress.sin_addr) <= 0) {
            UE_LOG(LogTemp, Display, TEXT("Invalid address/Address not supported"));
            closesocket(socketDescriptor);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        if (connect(socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
            UE_LOG(LogTemp, Display, TEXT("Connection failed."));
            closesocket(socketDescriptor);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        isConnected = true;
        UE_LOG(LogTemp, Display, TEXT("Connected to the server."));

        communicationThread = std::thread(&ClientSocket::communicationLoop, this);
        receiveThread = std::thread(&ClientSocket::receiveLoop, this);
    }
}

void ClientSocket::communicationLoop()
{
    while (isConnected) {
        GameEnterReq packet;
        packet.userAssignedId = userAssignedId;
        packet.token = token;
        packet.clientVersion = clientVersion;
        packet.gameId = gameId;

        TArray<char> serializedPacket = packet.Serialize();
        send(socketDescriptor, serializedPacket.GetData(), static_cast<int>(serializedPacket.Num()), 0);
        std::unique_lock<std::mutex> lock(dataMutex);
        cv.wait(lock);
        if (gameEntered) {
            break;
        }
        lock.unlock();
    }

    if (isConnected && gameEntered) {
        syncThread = std::thread(&ClientSocket::syncLoop, this);
    }

    while (isConnected && gameEntered) {
        std::unique_lock<std::mutex> lock(dataMutex);
        cv.wait(lock, [&] { return !outgoingQueue.empty(); });

        if (!outgoingQueue.empty()) {
            std::shared_ptr<Packet> packet = outgoingQueue.front();
            outgoingQueue.pop();
            lock.unlock();

            TArray<char> serializedPacket = packet->Serialize();
            send(socketDescriptor, serializedPacket.GetData(), static_cast<int>(serializedPacket.Num()), 0);
        }
    }
}

void ClientSocket::receiveLoop()
{
    while (isConnected) {
        char buffer[1024] = { 0 };
        int bytesRead = recv(socketDescriptor, buffer, sizeof(buffer), 0);
        if (bytesRead > 0) {
            PacketId packetId = Packet::GetPacketId(buffer, 1024);
            INT32 packetLength = Packet::ParsePacketLength(buffer, 1024);

            switch (packetId) {
            case PacketId::GAME_ENTER_RES:
            {
                std::shared_ptr<GameEnterRes> packet = std::make_shared<GameEnterRes>();
                packet->Deserialize(buffer, packetLength);
                UE_LOG(LogTemp, Display, TEXT("ErrorCodeL: %s"), *FString::FromInt(packet->errorCode));
                std::lock_guard<std::mutex> lock(dataMutex);
                gameEntered = true;
                cv.notify_one();
                break;
            }
            case PacketId::SYNC_RES:
            {
                std::shared_ptr<SyncRes> packet = std::make_shared<SyncRes>();
                packet->Deserialize(buffer, packetLength);
                UE_LOG(LogTemp, Display, TEXT("syncTime: %s"), *FString::FromInt(packet->syncTime));

                UE_LOG(LogTemp, Display, TEXT("user0"));
                UE_LOG(LogTemp, Display, TEXT("latency: %s"), *FString::Printf(TEXT("%lld"), packet->latency[0]));
                UE_LOG(LogTemp, Display, TEXT("position: %s"), *packet->users[0].Position.ToString());
                UE_LOG(LogTemp, Display, TEXT("velocity: %s"), *packet->users[0].Velocity.ToString());
                UE_LOG(LogTemp, Display, TEXT("acceleration: %s"), *packet->users[0].Acceleration.ToString());

                UE_LOG(LogTemp, Display, TEXT("user1"));
                UE_LOG(LogTemp, Display, TEXT("latency: %s"), *FString::Printf(TEXT("%lld"), packet->latency[1]));
                UE_LOG(LogTemp, Display, TEXT("position: %s"), *packet->users[1].Position.ToString());
                UE_LOG(LogTemp, Display, TEXT("velocity: %s"), *packet->users[1].Velocity.ToString());
                UE_LOG(LogTemp, Display, TEXT("acceleration: %s"), *packet->users[1].Acceleration.ToString());

                UE_LOG(LogTemp, Display, TEXT("user2"));
                UE_LOG(LogTemp, Display, TEXT("latency: %s"), *FString::Printf(TEXT("%lld"), packet->latency[2]));
                UE_LOG(LogTemp, Display, TEXT("position: %s"), *packet->users[2].Position.ToString());
                UE_LOG(LogTemp, Display, TEXT("velocity: %s"), *packet->users[2].Velocity.ToString());
                UE_LOG(LogTemp, Display, TEXT("acceleration: %s"), *packet->users[2].Acceleration.ToString());

                UE_LOG(LogTemp, Display, TEXT("user3"));
                UE_LOG(LogTemp, Display, TEXT("latency: %s"), *FString::Printf(TEXT("%lld"), packet->latency[3]));
                UE_LOG(LogTemp, Display, TEXT("position: %s"), *packet->users[3].Position.ToString());
                UE_LOG(LogTemp, Display, TEXT("velocity: %s"), *packet->users[3].Velocity.ToString());
                UE_LOG(LogTemp, Display, TEXT("acceleration: %s"), *packet->users[3].Acceleration.ToString());
                std::lock_guard<std::mutex> lock(dataMutex);
                gameEntered = true;
                cv.notify_one();
                break;
            }
            }
        }
    }
}

void ClientSocket::syncLoop()
{
    SyncReq syncReq;
    while (isConnected) {
        syncReq.syncReqTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        TArray<char> serializedPacket = syncReq.Serialize();
        int bSendSuccess = send(socketDescriptor, serializedPacket.GetData(), static_cast<int>(serializedPacket.Num()), 0);

        //if (bSendSuccess == SOCKET_ERROR)
        //{
        //    UE_LOG(LogTemp, Error, TEXT("Send Error: %d"), WSAGetLastError());
        //}
        //else {
        //    UE_LOG(LogTemp, Error, TEXT("Send Success"));
        //}

        Sleep(100); // 동기화 주기
    }
    
}

void ClientSocket::closeConnection()
{
    if (isConnected) {
        closesocket(socketDescriptor);
        isConnected = false;
        communicationThread.join();
        WSACleanup();
    }
}

