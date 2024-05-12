#pragma once
#include "Component.h"
#include "AnimationData.h"
#include "MovementStatus.h"
#include "MovementAnimationEvents.h"

//
// CAnimationComponent manages animations for a game object, handling initialization, updates, and rendering of animated sprites based on movement status.
//

class CMeshComponent;
class CGameEventManager;
class CTransformComponent;

class CAnimationComponent : public CComponent
{
public:
	using AnimationCallback = std::function<void(EAnimationID)>;
	CAnimationComponent(CObject* owner);

	void OnBeginPlay() override;
	void OnBeginDestroy() override;
	void PlayAnimationOnce(EAnimationID ID);
	void Update(float deltaTime) override;
	void Render() override;
	void ToggleInvincibility();

	void SetStartAnimationCallback(AnimationCallback callback) { m_startAnimationCallback = callback; }
	void SetEndAnimationCallback(AnimationCallback callback) { m_endAnimationCallback = callback; }

protected:

	void PlayAnimation(EAnimationID ID);
	bool CanUpdateStatus();
	void UpdatePreviousAnimation(EMovementStatus movementStatus);
	void ResetStatus();
	void UpdateAnimation(EMovementStatus movementStatus);

	EAnimationID m_currentAnimation;
	EAnimationID m_previousAnimation;
	CMeshComponent* p_meshComponent;
	CTransformComponent* p_transformComponent;
	CGameEventManager* p_eventManager;
	std::function<void(FMovementStatusChangedEvent*)> m_handlerMovementStatusChanged;
	bool b_singleAnimationPlaying;
	float m_currentAnimationDuration;
	float m_currentTime;
	bool b_debug = false;

	AnimationCallback m_startAnimationCallback;
	AnimationCallback m_endAnimationCallback;
};

