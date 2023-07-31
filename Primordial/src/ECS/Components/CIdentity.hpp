#pragma once

#include <string>
#include <vector>
#include "System/UUID.hpp"

class CIdentity
{
public:
	CIdentity() = delete;
	CIdentity(const CIdentity&) = delete;
	CIdentity(const std::string& _name, const UUID _uuid)
		: name(_name), uuid(_uuid) {}
	~CIdentity() = default;
public:
	std::string name;
	const UUID uuid;
};