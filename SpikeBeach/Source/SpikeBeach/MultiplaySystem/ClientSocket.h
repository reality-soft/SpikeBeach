// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#include "CoreMinimal.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <queue>
#include <memory>
#include <utility>

#include "Packets/Packet.h"

#pragma comment(lib, "ws2_32.lib")

/**
 * 
 */
class SPIKEBEACH_API ClientSocket
{
private:
    SOCKET socketDescriptor;
    std::thread sendThread;
    std::thread receiveThread;
    std::thread syncThread;
    std::mutex dataMutex;
    std::condition_variable cv;
    bool isConnected = false;
    bool gameEntered = false;
    std::queue<std::shared_ptr<Packet>> outgoingQueue;
    std::queue<std::shared_ptr<Packet>> incomingQueue;

public:
    FString userAssignedId;
    FString token;
    FString clientVersion;
    INT32 gameId; //roomId

public:
    ClientSocket(const char* serverIP,
        int serverPort,
        FString userAssignedId,
        FString token,
        FString clientVersion,
        INT32 gameId);
    template <typename PacketType, typename... Args>
    void sendPacket(Args&&...args)
    {
        if (!isConnected) {
            UE_LOG(LogTemp, Display, TEXT("Not connected to the server."));
            return;
        }
        if (!gameEntered) {
            UE_LOG(LogTemp, Display, TEXT("Not entered to the game."));
            return;
        }

        shared_ptr<Packet> curPacket = dynamic_pointer_cast<Packet>(make_shared<PacketType>(std::forward<Args>(args)...));
        std::lock_guard<std::mutex> lock(dataMutex);
        outgoingQueue.push(curPacket);
        cv.notify_one();
    }

    ~ClientSocket() {
        CloseConnection();
    }

private:
    void SendLoop();
    void ReceiveLoop();
    void SyncLoop();
    void CloseConnection();

public:
    void ProcessPackets();
};

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"