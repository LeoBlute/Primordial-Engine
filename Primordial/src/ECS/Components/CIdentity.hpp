#pragma once

#include <string>

class CIdentity
{
public:
	CIdentity() = delete;
	CIdentity(const CIdentity&) = delete;
	CIdentity(const std::string& name, unsigned int layer)
		: Name(name), Layer(layer) {}
	~CIdentity() = default;
public:
	std::string Name;
	unsigned int Layer;
};