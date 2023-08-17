#pragma once

#include "../Behaviour.h"
class ControlNode : public Behaviour
{
public:
	ControlNode() = default;
	ControlNode(std::initializer_list<Behaviour*> children);
	~ControlNode();

	void OnInit();

	ControlNode* Add(Behaviour* behaviour);
	void Remove(Behaviour* behaviour);
	void Clear();
protected:
	typedef std::vector<Behaviour*> Behaviours;
	Behaviours::iterator m_Current;
	Behaviours m_Children;
};