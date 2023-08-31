// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
enum PacketId
{
	// Generic 0 ~ 99
	CLOSE_CONN = 1,
	ERROR_OCCUR = 11,
	DISCONN_REQ = 12,
	DISCONN_RES = 13,
	DISCONN_NTF = 14,
	GAME_TIMEOUT_NTF = 15,

	GAME_ENTER_REQ = 101,
	GAME_ENTER_NTF = 102,
	GAME_ENTER_RES = 103,
	GAME_LEAVE_REQ = 104,
	GAME_LEAVE_RES = 105,
	GAME_LEAVE_NTF = 106,
	GAME_START_NTF = 107,
	SYNC_REQ = 108,
	SYNC_RES = 109,
	CONTROLL_REQ = 110,
	CONTROLL_NTF = 111,
};
