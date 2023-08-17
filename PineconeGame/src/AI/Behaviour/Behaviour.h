#pragma once

#include <Pinecone.h>

using namespace Pinecone;

enum BehaviourStatus
{
	BH_INVALID = -1,
	BH_SUCCESS,
	BH_RUNNING,
	BH_PENDING,
	BH_FAILURE,
	BH_ABORTED
};

class Behaviour
{
public:
	Behaviour() : m_Status(BH_INVALID) {}
	virtual ~Behaviour() = default;

	virtual void OnInit() {};
	virtual void OnTerminate(BehaviourStatus status) {};
	virtual BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) = 0;

	BehaviourStatus Tick(GameObject gameObject, Timestep ts);
	void Abort();
	void Reset();

	inline bool IsTerminated() const { return m_Status != BH_RUNNING; }
	inline bool IsRunning() const { return m_Status == BH_RUNNING; }

	inline BehaviourStatus GetStatus() const { return m_Status; }
private:
	BehaviourStatus m_Status;
};