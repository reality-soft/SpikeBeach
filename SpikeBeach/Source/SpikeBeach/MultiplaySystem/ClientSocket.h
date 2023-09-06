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

#include "GameFramework/Actor.h"

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"

#include "ClientSocket.generated.h"

UCLASS()
class SPIKEBEACH_API AClientSocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClientSocket();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    SOCKET socketDescriptor;

    std::thread sendThread;
    std::thread receiveThread;
    std::thread syncThread;

    std::mutex dataMutex;
    std::condition_variable cv;

private:
    bool isConnected = false;
    bool gameEntered = false;

private:
    std::queue<std::shared_ptr<Packet>> outgoingQueue;
    std::queue<std::shared_ptr<Packet>> incomingQueue;

private:
    INT64 RTT = 0;

public:
    FString userAssignedId_;
    FString token_;
    FString clientVersion_;
    INT32 gameId_; //roomId

public:
    void Initialize(
        const char* serverIP,
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

    ~AClientSocket() {
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