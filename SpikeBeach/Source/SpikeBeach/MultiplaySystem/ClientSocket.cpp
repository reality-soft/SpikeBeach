// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"
#include "Packets/PacketId.h"
#include "Packets/Requests/GameEnterReq.h"
#include "Packets/Requests/SyncReq.h"
#include "Packets/Responses/GameEnterRes.h"
#include "Packets/Responses/SyncRes.h"

// Sets default values
AClientSocket::AClientSocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AClientSocket::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClientSocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    ProcessPackets();
}

void AClientSocket::Initialize(
    const char* serverIP,
    int serverPort,
    FString userAssignedId,
    FString token,
    FString clientVersion,
    INT32 gameId)
{
    {
        this->userAssignedId_ = userAssignedId;
        this->token_ = token;
        this->clientVersion_ = clientVersion;
        this->gameId_ = gameId;

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

        sendThread = std::thread(&AClientSocket::SendLoop, this);
        receiveThread = std::thread(&AClientSocket::ReceiveLoop, this);
    }
}

void AClientSocket::SendLoop()
{
    while (isConnected) {
        GameEnterReq packet;
        packet.userAssignedId = userAssignedId_;
        packet.token = token_;
        packet.clientVersion = clientVersion_;
        packet.gameId = gameId_;

        TArray<char> serializedPacket = packet.Serialize();
        send(socketDescriptor, serializedPacket.GetData(), static_cast<int>(serializedPacket.Num()), 0);
        std::unique_lock<std::mutex> lock(dataMutex);
        cv.wait(lock, [&]{ return gameEntered; });
        if (gameEntered) {
            break;
        }
        lock.unlock();
    }

    if (isConnected && gameEntered) {
        syncThread = std::thread(&AClientSocket::SyncLoop, this);
    }

    while (isConnected && gameEntered) {
        std::unique_lock<std::mutex> lock(dataMutex);
        cv.wait(lock, [&] { return !outgoingQueue.empty() || isConnected == false; });

        if (!outgoingQueue.empty()) {
            std::shared_ptr<Packet> packet = outgoingQueue.front();
            outgoingQueue.pop();
            lock.unlock();

            TArray<char> serializedPacket = packet->Serialize();
            send(socketDescriptor, serializedPacket.GetData(), static_cast<int>(serializedPacket.Num()), 0);
        }
    }
}

void AClientSocket::ReceiveLoop()
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

                std::lock_guard<std::mutex> lock(dataMutex);
                incomingQueue.push(packet);
                gameEntered = true;
                cv.notify_one();
                break;
            }
            case PacketId::SYNC_RES:
            {
                std::shared_ptr<SyncRes> packet = std::make_shared<SyncRes>();
                packet->Deserialize(buffer, packetLength);
                
                INT64 curTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                RTT = curTime - packet->syncReqTime;
                otherClientsAvgRTT = 0;
                
                for (const int & curRTT : packet->RTT) {
                    otherClientsAvgRTT += curRTT;
                }
                otherClientsAvgRTT /= 4;

                std::lock_guard<std::mutex> lock(dataMutex);
                incomingQueue.push(packet);
                break;
            }
            }
        }
    }
}

void AClientSocket::SyncLoop()
{
    SyncReq syncReq;
    while (isConnected) {
        syncReq.syncReqTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        syncReq.RTT = RTT;
        TArray<char> serializedPacket = syncReq.Serialize();
        int bSendSuccess = send(socketDescriptor, serializedPacket.GetData(), static_cast<int>(serializedPacket.Num()), 0);
        Sleep(100); // 동기화 주기
    }

}

void AClientSocket::CloseConnection()
{
    if (isConnected) {
        closesocket(socketDescriptor);
        isConnected = false;
        cv.notify_all();
        sendThread.join();
        receiveThread.join();
        syncThread.join();
        WSACleanup();
    }
}

void AClientSocket::ProcessPackets()
{
    while (!incomingQueue.empty()) {
        std::shared_ptr<Packet> packet = incomingQueue.front();
        packet->Process(GetWorld());
        incomingQueue.pop();
    }
}

