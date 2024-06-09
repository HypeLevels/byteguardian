#pragma once
#include "../include.h"

inline constexpr uint32_t MAX_ENTITIES_IN_LIST = 512;
inline constexpr uint32_t MAX_ENTITY_LISTS = 64;
inline constexpr uint32_t MAX_TOTAL_ENTITIES = MAX_ENTITIES_IN_LIST * MAX_ENTITY_LISTS;

class EntityHandle {
public:
	static constexpr uint32_t INVALID_EHANDLE_INDEX = 0xFFFFFFFF;

	FORCEINLINE int32_t get_entry_index() const noexcept {
		return is_valid() ? static_cast<int32_t>(parts.entity_index) : -1;
	}

	FORCEINLINE bool is_valid() const noexcept {
		return index != INVALID_EHANDLE_INDEX;
	}

	FORCEINLINE bool operator==(const EntityHandle& other) const noexcept {
		return index == other.index;
	}

	FORCEINLINE bool operator!=(const EntityHandle& other) const noexcept {
		return index != other.index;
	}

protected:
	union {
		uint32_t index;

		struct {
			uint32_t entity_index : 15;
			uint32_t serial : 17;
		} parts;
	};
};

// Size: 0x78 (120 bytes)
class EntityIdentity {
public:
	FORCEINLINE int32_t get_entity_index() const noexcept {
		return handle.get_entry_index();
	}

	FORCEINLINE auto identities() const {
		return process.Read<std::array<EntityIdentity, MAX_ENTITIES_IN_LIST>>(reinterpret_cast<uint64_t>(this));
	}

public:
	uint64_t instance;      // 0x0000
	uint8_t pad_0008[0x8];  // 0x0008
	EntityHandle handle;    // 0x0010
	uint8_t pad_0014[0x64]; // 0x0014
};

// Size: 0xA78 (2680 bytes)
class ConcreteEntityList {
public:
	uint64_t get_player_controller(const int32_t index) const {
		if (index <= -1 || index >= MAX_TOTAL_ENTITIES - 1)
			return 0;

		const EntityIdentity* chunk_to_use = identity_chunks[index / MAX_ENTITIES_IN_LIST];

		if (chunk_to_use == nullptr)
			return 0;

		const EntityIdentity identity = chunk_to_use->identities()[index % MAX_ENTITIES_IN_LIST];

		return identity.get_entity_index() == index ? identity.instance : 0;
	}

	uint64_t get_player_pawn(const uint64_t player_controller) const {
		const auto pawn_handle = process.Read<EntityHandle>(player_controller + 0x5FC); // m_hPawn

		if (!pawn_handle.is_valid())
			return 0;

		const int32_t pawn_index = pawn_handle.get_entry_index();

		if (pawn_index <= -1 || pawn_index >= MAX_TOTAL_ENTITIES - 1)
			return 0;

		const EntityIdentity* chunk_to_use = identity_chunks[pawn_index / MAX_ENTITIES_IN_LIST];

		if (chunk_to_use == nullptr)
			return 0;

		const EntityIdentity identity = chunk_to_use->identities()[pawn_index % MAX_ENTITIES_IN_LIST];

		return identity.get_entity_index() == pawn_index ? identity.instance : 0;
	}

public:
	// Size: 0x18 (24 bytes)
	struct List {
		EntityIdentity* head;  // 0x0000
		EntityIdentity* tail;  // 0x0008
		uint8_t pad_0010[0x8]; // 0x0010
	};

	EntityIdentity* identity_chunks[MAX_ENTITY_LISTS]; // 0x0000
	EntityIdentity* first_active_entity;               // 0x0200
	List used_list;                                    // 0x0208
	List dormant_list;                                 // 0x0220
	List free_networkable_list;                        // 0x0238
	List free_non_networkable_list;                    // 0x0250
	uint32_t networkable_entity_limit;                 // 0x0268
	uint32_t non_networkable_entity_limit;             // 0x026C
	uint32_t max_players;                              // 0x0270
	uint8_t pad_0274[0x800];                           // 0x0274
};

// Size: 0x1508 (5384 bytes)
class EntitySystem {
public:
	uint8_t pad_0000[0x10];         // 0x0000
	ConcreteEntityList entity_list; // 0x0010
	uint8_t pad_0a88[0xA80];        // 0x0A88
};

// Size: 0x1570 (5488 bytes)
class GameEntitySystem : public EntitySystem {
public:
	uint8_t pad_1508[0x8];                    // 0x1508
	int32_t max_networked_entity_index;       // 0x1510
	int32_t networked_entity_count;           // 0x1514
	int32_t non_networked_saved_entity_count; // 0x1518
	uint8_t pad_151c[0x50];                   // 0x151C
};