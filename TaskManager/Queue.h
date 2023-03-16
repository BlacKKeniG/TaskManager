#pragma once

#include <iostream>

using std::string;

template<typename TypeContainer>
struct Queue {
	string _name;
	TypeContainer _container;

	Queue() : _name{}, _container{} {}

	Queue(const string& name) : _name{ name }, _container{} {}
	~Queue() = default;
};