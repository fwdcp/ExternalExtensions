/*
 *  player.cpp
 *  StatusSpec project
 *
 *  Copyright (c) 2014 thesupremecommander
 *  BSD 2-Clause License
 *  http://opensource.org/licenses/BSD-2-Clause
 *
 */

#include "player.h"

#include <cstdint>

#include "tier0/valve_minmax_on.h"
#include "cdll_int.h"
#include "icliententity.h"
#include "icliententitylist.h"
#include "shareddefs.h"
#include "steam/steam_api.h"
#include "toolframework/ienginetool.h"
#include "tier0/valve_minmax_off.h"

#include "common.h"
#include "entities.h"
#include "exceptions.h"
#include "ifaces.h"

Player::Player(int entindex) {
	playerEntity = Interfaces::pClientEntityList->GetClientEntity(entindex);
}

Player::Player(IClientEntity *entity) {
	playerEntity = entity;
}

Player& Player::operator=(int entindex) {
	playerEntity = Interfaces::pClientEntityList->GetClientEntity(entindex);

	return *this;
}

Player& Player::operator=(IClientEntity *entity) {
	playerEntity = entity;

	return *this;
}

Player& Player::operator=(const Player &player) {
	if (this == &player) {
		return *this;
	}

	playerEntity = player.playerEntity;

	return *this;
}

bool Player::operator==(int entindex) const {
	return IsEqualTo(Player(entindex));
}

bool Player::operator==(IClientEntity *entity) const {
	return IsEqualTo(Player(entity));
}

bool Player::operator==(const Player &player) const {
	return IsEqualTo(player);
}

bool Player::operator!=(int entindex) const {
	return IsNotEqualTo(Player(entindex));
}

bool Player::operator!=(IClientEntity *entity) const {
	return IsNotEqualTo(Player(entity));
}

bool Player::operator!=(const Player &player) const {
	return IsNotEqualTo(player);
}

bool Player::operator<(int entindex) const {
	return IsLessThan(Player(entindex));
}

bool Player::operator<(IClientEntity *entity) const {
	return IsLessThan(Player(entity));
}

bool Player::operator<(const Player &player) const {
	return IsLessThan(player);
}

bool Player::operator<=(int entindex) const {
	return IsLessThanOrEqualTo(Player(entindex));
}

bool Player::operator<=(IClientEntity *entity) const {
	return IsLessThanOrEqualTo(Player(entity));
}

bool Player::operator<=(const Player &player) const {
	return IsLessThanOrEqualTo(player);
}

bool Player::operator>(int entindex) const {
	return IsGreaterThan(Player(entindex));
}

bool Player::operator>(IClientEntity *entity) const {
	return IsGreaterThan(Player(entity));
}

bool Player::operator>(const Player &player) const {
	return IsGreaterThan(player);
}

bool Player::operator>=(int entindex) const {
	return IsGreaterThanOrEqualTo(Player(entindex));
}

bool Player::operator>=(IClientEntity *entity) const {
	return IsGreaterThanOrEqualTo(Player(entity));
}

bool Player::operator>=(const Player &player) const {
	return IsGreaterThanOrEqualTo(player);
}

bool Player::IsEqualTo(const Player &player) const {
	if (IsValid() && player.IsValid()) {
		return playerEntity == player.playerEntity;
	}

	return false;
}

bool Player::IsNotEqualTo(const Player &player) const {
	return !IsEqualTo(player);
}

bool Player::IsLessThan(const Player &player) const {
	if (!IsValid()) {
		return true;
	}

	if (!player.IsValid()) {
		return false;
	}

	if (GetTeam() < player.GetTeam()) {
		return true;
	}
	else if (GetTeam() > player.GetTeam()) {
		return false;
	}

	if (TFDefinitions::normalClassOrdinal.find(GetClass())->second < TFDefinitions::normalClassOrdinal.find(player.GetClass())->second) {
		return true;
	}
	else if (TFDefinitions::normalClassOrdinal.find(GetClass())->second > TFDefinitions::normalClassOrdinal.find(player.GetClass())->second) {
		return false;
	}

	if (this->GetEntity()->entindex() < player.GetEntity()->entindex()) {
		return true;
	}
	else if (this->GetEntity()->entindex() > player.GetEntity()->entindex()) {
		return false;
	}

	return false;
}

bool Player::IsLessThanOrEqualTo(const Player &player) const {
	return IsEqualTo(player) || IsLessThan(player);
}

bool Player::IsGreaterThan(const Player &player) const {
	if (!IsValid()) {
		return false;
	}

	if (!player.IsValid()) {
		return true;
	}

	if (GetTeam() > player.GetTeam()) {
		return true;
	}
	else if (GetTeam() < player.GetTeam()) {
		return false;
	}

	if (TFDefinitions::normalClassOrdinal.find(GetClass())->second > TFDefinitions::normalClassOrdinal.find(player.GetClass())->second) {
		return true;
	}
	else if (TFDefinitions::normalClassOrdinal.find(GetClass())->second < TFDefinitions::normalClassOrdinal.find(player.GetClass())->second) {
		return false;
	}

	if (this->GetEntity()->entindex() > player.GetEntity()->entindex()) {
		return true;
	}
	else if (this->GetEntity()->entindex() < player.GetEntity()->entindex()) {
		return false;
	}

	return false;
}

bool Player::IsGreaterThanOrEqualTo(const Player &player) const {
	return IsEqualTo(player) || IsGreaterThan(player);
}

Player::operator bool() const {
	return IsValid();
}

bool Player::IsValid() const {
	return playerEntity.IsValid() && playerEntity.Get() && playerEntity->entindex() >= 1 && playerEntity->entindex() <= Interfaces::pEngineTool->GetMaxClients() && Entities::CheckEntityBaseclass(playerEntity, "TFPlayer");
}

Player::operator IClientEntity *() const {
	return playerEntity;
}

IClientEntity *Player::operator->() const {
	return playerEntity;
}

IClientEntity *Player::GetEntity() const {
	return playerEntity;
}

bool Player::CheckCondition(TFCond condition) const {
	if (IsValid()) {
		uint32_t playerCond = *Entities::GetEntityProp<uint32_t *>(playerEntity.Get(), { "m_nPlayerCond" });
		uint32_t condBits = *Entities::GetEntityProp<uint32_t *>(playerEntity.Get(), { "_condition_bits" });
		uint32_t playerCondEx = *Entities::GetEntityProp<uint32_t *>(playerEntity.Get(), { "m_nPlayerCondEx" });
		uint32_t playerCondEx2 = *Entities::GetEntityProp<uint32_t *>(playerEntity.Get(), { "m_nPlayerCondEx2" });
		uint32_t playerCondEx3 = *Entities::GetEntityProp<uint32_t *>(playerEntity.Get(), { "m_nPlayerCondEx3" });

		uint32_t conditions[4];
		conditions[0] = playerCond | condBits;
		conditions[1] = playerCondEx;
		conditions[2] = playerCondEx2;
		conditions[3] = playerCondEx3;

		if (condition < 32) {
			if (conditions[0] & (1 << condition)) {
				return true;
			}
		}
		else if (condition < 64) {
			if (conditions[1] & (1 << (condition - 32))) {
				return true;
			}
		}
		else if (condition < 96) {
			if (conditions[2] & (1 << (condition - 64))) {
				return true;
			}
		}
		else if (condition < 128) {
			if (conditions[3] & (1 << (condition - 96))) {
				return true;
			}
		}
	}

	return false;
}

TFClassType Player::GetClass() const {
	if (IsValid()) {
		return (TFClassType)*Entities::GetEntityProp<int *>(playerEntity.Get(), { "m_iClass" });
	}

	return TFClass_Unknown;
}

int Player::GetHealth() const {
	if (IsValid()) {
		return *Entities::GetEntityProp<int *>(playerEntity.Get(), { "m_iHealth" });
	}

	return 0;
}

int Player::GetMaxHealth() const {
	if (IsValid()) {
		static IClientEntity *tfPlayerResource = nullptr;

		if (!tfPlayerResource || !Entities::CheckEntityBaseclass(tfPlayerResource, "TFPlayerResource")) {
			tfPlayerResource = nullptr;

			int lastEntity = Interfaces::pClientEntityList->GetHighestEntityIndex();
			for (int i = 0; i <= lastEntity; i++) {
				IClientEntity *entity = Interfaces::pClientEntityList->GetClientEntity(i);

				if (entity && Entities::CheckEntityBaseclass(entity, "TFPlayerResource")) {
					tfPlayerResource = entity;

					break;
				}
			}
		}

		if (tfPlayerResource) {
			char offset[4];
			GetPropIndexString(playerEntity->entindex(), offset);

			return *Entities::GetEntityProp<int *>(tfPlayerResource, { "m_iMaxHealth", offset });
		}
	}

	return 0;
}

std::string Player::GetName() const {
	if (IsValid()) {
		player_info_t playerInfo;

		if (Interfaces::pEngineClient->GetPlayerInfo(playerEntity->entindex(), &playerInfo)) {
			return playerInfo.name;
		}
	}

	return "";
}

int Player::GetObserverMode() const {
	if (IsValid()) {
		return *Entities::GetEntityProp<int *>(playerEntity.Get(), { "m_iObserverMode" });
	}

	return OBS_MODE_NONE;
}

IClientEntity *Player::GetObserverTarget() const {
	if (IsValid()) {
		CBaseHandle *entityHandle = Entities::GetEntityProp<CBaseHandle *>(playerEntity.Get(), { "m_hObserverTarget" });

		if (entityHandle->IsValid()) {
			return dynamic_cast<IClientEntity *>(entityHandle->Get());
		}
	}

	return playerEntity;
}

Vector Player::GetPosition() const {
	if (IsValid()) {
		return playerEntity->GetAbsOrigin();
	}

	return Vector();
}

CSteamID Player::GetSteamID() const {
	if (IsValid()) {
		player_info_t playerInfo;

		if (Interfaces::pEngineClient->GetPlayerInfo(playerEntity->entindex(), &playerInfo)) {
			if (playerInfo.friendsID) {
				static EUniverse universe = k_EUniverseInvalid;

				if (universe == k_EUniverseInvalid) {
					if (Interfaces::pSteamAPIContext->SteamUtils()) {
						universe = Interfaces::pSteamAPIContext->SteamUtils()->GetConnectedUniverse();
					}
					else {
						// let's just assume that it's public - what are the chances that there's a Valve employee testing this on another universe without Steam?

						PRINT_TAG();
						Warning("Steam libraries not available - assuming public universe for user Steam IDs!\n");

						universe = k_EUniversePublic;
					}
				}

				return CSteamID(playerInfo.friendsID, 1, universe, k_EAccountTypeIndividual);
			}
		}
	}

	return CSteamID();
}

TFTeam Player::GetTeam() const {
	if (IsValid()) {
		return (TFTeam) *Entities::GetEntityProp<int *>(playerEntity.Get(), { "m_iTeamNum" });
	}

	return TFTeam_Unassigned;
}

int Player::GetUserID() const {
	if (IsValid()) {
		player_info_t playerInfo;

		if (Interfaces::pEngineClient->GetPlayerInfo(playerEntity->entindex(), &playerInfo)) {
			return playerInfo.userID;
		}
	}

	return 0;
}

bool Player::IsAlive() const {
	if (IsValid()) {
		return *Entities::GetEntityProp<char *>(playerEntity.Get(), { "m_lifeState" }) == LIFE_ALIVE;
	}

	return false;
}

bool Player::IsLocalPlayer() const {
	if (IsValid()) {
		return playerEntity->entindex() == Interfaces::pEngineClient->GetLocalPlayer();
	}

	return false;
}

Player::Iterator::Iterator(const Player::Iterator& old) {
	index = old.index;
}

Player::Iterator& Player::Iterator::operator=(const Player::Iterator& old) {
	index = old.index;

	return  *this;
};

Player::Iterator& Player::Iterator::operator++() {
	for (int i = index + 1; i <= Interfaces::pEngineTool->GetMaxClients(); i++) {
		if (Player(i)) {
			index = i;

			return *this;
		}
	}

	index = Interfaces::pEngineTool->GetMaxClients() + 1;

	return *this;
};

Player Player::Iterator::operator*() const {
	return Player(index);
}

void swap(Player::Iterator& lhs, Player::Iterator& rhs) {
	using std::swap;
	swap(lhs.index, rhs.index);
}

Player::Iterator Player::Iterator::operator++(int) {
	Player::Iterator current(*this);

	for (int i = index + 1; i <= Interfaces::pEngineTool->GetMaxClients(); i++) {
		if (Player(i)) {
			index = i;

			return current;
		}
	}

	index = Interfaces::pEngineTool->GetMaxClients() + 1;

	return current;
}

Player *Player::Iterator::operator->() const {
	return new Player(index);
}

bool operator==(const Player::Iterator& lhs, const Player::Iterator& rhs) {
	return lhs.index == rhs.index;
}

bool operator!=(const Player::Iterator& lhs, const Player::Iterator& rhs) {
	return lhs.index != rhs.index;
}

Player::Iterator::Iterator() {
	for (int i = 1; i <= Interfaces::pEngineTool->GetMaxClients(); i++) {
		if (Player(i)) {
			index = i;

			return;
		}
	}

	index = Interfaces::pEngineTool->GetMaxClients() + 1;

	return;
}

Player::Iterator& Player::Iterator::operator--() {
	for (int i = index - 1; i >= 1; i++) {
		if (Player(i)) {
			index = i;

			return *this;
		}
	}

	index = 0;

	return *this;
}

Player::Iterator Player::Iterator::operator--(int) {
	Player::Iterator current(*this);

	for (int i = index - 1; i >= 1; i++) {
		if (Player(i)) {
			index = i;

			return current;
		}
	}

	index = 0;

	return current;
}

Player::Iterator::Iterator(int startIndex) {
	index = startIndex;
}

Player::Iterator Player::begin() {
	return Player::Iterator();
}

Player::Iterator Player::end() {
	return Player::Iterator(Interfaces::pEngineTool->GetMaxClients() + 1);
}

Player::Iterator Player::Iterable::begin() {
	return Player::begin();
}

Player::Iterator Player::Iterable::end() {
	return Player::end();
}

bool Player::CheckDependencies() {
	bool ready = true;

	if (!Interfaces::pClientEntityList) {
		PRINT_TAG();
		Warning("Required interface IClientEntityList for player helper class not available!\n");

		ready = false;
	}

	if (!Interfaces::pEngineClient) {
		PRINT_TAG();
		Warning("Interface IVEngineClient for player helper class not available!\n");

		ready = false;
	}

	if (!Interfaces::pEngineTool) {
		PRINT_TAG();
		Warning("Interface IEngineTool for player helper class not available!\n");

		ready = false;
	}

	if (!Interfaces::steamLibrariesAvailable) {
		PRINT_TAG();
		Warning("Steam libraries for player helper class not available (required for accuracy in retrieving Steam IDs)!\n");
	}

	if (!Entities::RetrieveClassPropOffset("CTFPlayer", { "m_nPlayerCond" })) {
		PRINT_TAG();
		Warning("Required property m_nPlayerCond for CTFPlayer for player helper class not available!\n");

		ready = false;
	}

	if (!Entities::RetrieveClassPropOffset("CTFPlayer", { "_condition_bits" })) {
		PRINT_TAG();
		Warning("Required property _condition_bits for CTFPlayer for player helper class not available!\n");

		ready = false;
	}

	if (!Entities::RetrieveClassPropOffset("CTFPlayer", { "m_nPlayerCondEx" })) {
		PRINT_TAG();
		Warning("Required property m_nPlayerCondEx for CTFPlayer for player helper class not available!\n");

		ready = false;
	}

	if (!Entities::RetrieveClassPropOffset("CTFPlayer", { "m_nPlayerCondEx2" })) {
		PRINT_TAG();
		Warning("Required property m_nPlayerCondEx2 for CTFPlayer for player helper class not available!\n");

		ready = false;
	}

	if (!Entities::RetrieveClassPropOffset("CTFPlayer", { "m_nPlayerCondEx3" })) {
		PRINT_TAG();
		Warning("Required property m_nPlayerCondEx3 for CTFPlayer for player helper class not available!\n");

		ready = false;
	}

	if (!Entities::RetrieveClassPropOffset("CTFPlayer", { "m_iClass" })) {
		PRINT_TAG();
		Warning("Required property m_iClass for CTFPlayer for player helper class not available!\n");

		ready = false;
	}

	if (!Entities::RetrieveClassPropOffset("CTFPlayer", { "m_lifeState" })) {
		PRINT_TAG();
		Warning("Required property m_lifeState for CTFPlayer for player helper class not available!\n");

		ready = false;
	}

	if (!Entities::RetrieveClassPropOffset("CTFPlayer", { "m_iObserverMode" })) {
		PRINT_TAG();
		Warning("Required property m_iObserverMode for CTFPlayer for player helper class not available!\n");

		ready = false;
	}

	if (!Entities::RetrieveClassPropOffset("CTFPlayer", { "m_hObserverTarget" })) {
		PRINT_TAG();
		Warning("Required property m_hObserverTarget for CTFPlayer for player helper class not available!\n");

		ready = false;
	}

	if (!Entities::RetrieveClassPropOffset("CTFPlayer", { "m_iHealth" })) {
		PRINT_TAG();
		Warning("Required property m_iHealth for CTFPlayer for player helper class not available!\n");

		ready = false;
	}

	if (!Entities::RetrieveClassPropOffset("CTFPlayer", { "m_iTeamNum" })) {
		PRINT_TAG();
		Warning("Required property m_iHealth for CTFPlayer for player helper class not available!\n");

		ready = false;
	}

	for (int i = 0; i <= Interfaces::pEngineTool->GetMaxClients(); i++) {
		char offset[4];
		GetPropIndexString(i, offset);

		if (!Entities::RetrieveClassPropOffset("CTFPlayerResource", { "m_iMaxHealth", offset })) {
			PRINT_TAG();
			Warning("Required property array m_iMaxHealth for CTFPlayerResource for player helper class not available!\n");

			ready = false;
		}
	}

	return ready;
}
