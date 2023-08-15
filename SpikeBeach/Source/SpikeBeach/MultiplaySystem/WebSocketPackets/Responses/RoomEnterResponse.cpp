// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomEnterResponse.h"
#include "../PacketIdDef.h"

TArray<uint8> RoomEnterResponse::Serialize()
{
    TArray<uint8> Bytes;
    Bytes.Append(ResponseHeader::Serialize(static_cast<int>(PacketIdDef::RoomEnterRes)));
    Bytes.Append((const uint8*)TCHAR_TO_UTF8(*roomInfoString), roomInfoString.Len() + 1);
    return Bytes;
}

int RoomEnterResponse::Deserialize(const uint8* data)
{
    int Offset = ResponseHeader::Deserialize(data);
    roomInfoString = ReadString(data, Offset);

    return Offset + sizeof(int16);
}

void RoomEnterResponse::ProcessUserInfo(TArray<FUserInRoom>& userInfo)
{
    TArray<FString> roomInfoStrings;
    roomInfoString.ParseIntoArray(roomInfoStrings, TEXT("\t"), true);

    for (const auto& curInfo : roomInfoStrings) {
        switch (curInfo[0]) {
        case 'I':
        {
            roomNum = FCString::Atoi(*curInfo.Mid(2));
            break;
        }
        case 'T':
        {
            roomName = curInfo.Mid(2);
            break;
        }
        case 'U':
        {
            TArray<FString> users;
            curInfo.Mid(2).ParseIntoArray(users, TEXT(" "), true);

            for (const auto& curUserNickname : users) {
                FUserInRoom curUser;
                curUser.nickName = curUserNickname;
                userInfo.Add(curUser);
            }
            break;
        }
        case 'H':
        {
            FString hostNickName = curInfo.Mid(2);

            for (auto& curUser : userInfo) {
                if (curUser.nickName == hostNickName) {
                    curUser.isHost = true;
                    break;
                }
            }
            break;
        }
        case 'A':
        {
            TArray<FString> users;
            curInfo.Mid(2).ParseIntoArray(users, TEXT(" "), true);

            for (const auto& curUserNickname : users) {
                for (auto& curUser : userInfo) {
                    if (curUser.nickName == curUserNickname) {
                        curUser.readyState = EReadyState::eATeam;
                        break;
                    }
                }
            }
            break;
        }
        case 'B':
        {
            TArray<FString> users;
            curInfo.Mid(1).ParseIntoArray(users, TEXT(" "), true);

            for (const auto& curUserNickname : users) {
                for (auto& curUser : userInfo) {
                    if (curUser.nickName == curUserNickname) {
                        curUser.readyState = EReadyState::eATeam;
                        break;
                    }
                }
            }
            break;
        }
        }
    }
}
