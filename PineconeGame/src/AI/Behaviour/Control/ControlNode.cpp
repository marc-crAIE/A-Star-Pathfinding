#include "ControlNode.h"

#include <algorithm>

ControlNode::~ControlNode()
{
	Clear();
}

void ControlNode::OnInit()
{
	m_Current = m_Children.begin();
}

ControlNode* ControlNode::Add(Behaviour* behaviour)
{
	m_Children.push_back(behaviour);
	return this;
}

void ControlNode::Remove(Behaviour* behaviour)
{
	m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), behaviour), m_Children.end());
	delete behaviour;
}

void ControlNode::Clear()
{
	for (Behaviours::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		delete* it;
	m_Children.clear();
}
