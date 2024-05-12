#include "stdafx.h"
#include "UI.h"
#include "UIComponent.h"
#include "UIElement.h"
#include "UIElementText.h"
#include "UIElementSprite.h"
#include "Object.h"
#include "GameAuraComponent.h"
#include "GameCostants.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include "GameMode.h"
#include "GameEventManager.h"
#include "GameInstance.h"
#include "GameplayGameMode.h"
#include "TransformComponent.h"
#include "GameResourceEvents.h"

namespace UI 
{

	CUIComponent* BuildPlayerBackgroundComponent(CObject* owner)
	{
		CTransformComponent* p_transformComponent = owner->GetFirstComponentOfClass<CTransformComponent>();
		if (p_transformComponent == nullptr) return nullptr;

		CUIComponent* p_backgroundWidget = new CUIComponent( owner, FVector2D{ p_transformComponent->GetLocation().x + APP_VIRTUAL_WIDTH / 2 - s_lateralMargin / 4, APP_VIRTUAL_HEIGHT - s_topMargin });
		if (p_backgroundWidget == nullptr) return nullptr;

		CUIElementSprite* p_backgroundSpriteComponent = new CUIElementSprite(*p_backgroundWidget, FVector2D{ 0.f, 0.f }, CGameCostants::HUDbackground, 1, 1);
		p_backgroundWidget->AddUIElement(p_backgroundSpriteComponent);
		owner->AddComponent(p_backgroundWidget);
		return p_backgroundWidget;
	}

	CUIComponent* BuildPauseUIComponent(CObject* owner)
	{
		CUIComponent* p_pauseComponent = new CUIComponent(owner, FVector2D{ 0.f, 0.f });
		if (p_pauseComponent == nullptr) return nullptr;

		CUIElementSprite* p_backgroundSpriteComponent = new CUIElementSprite(*p_pauseComponent, FVector2D{ 125.f, -70.f}, CGameCostants::pauseBackgroundMesh, 1, 1);
		p_pauseComponent->AddUIElement(p_backgroundSpriteComponent);

		return p_pauseComponent;
	}

	CUIComponent* BuildPlayerScoreComponent(CObject* owner)
	{
		CTransformComponent* p_transformComponent = owner->GetFirstComponentOfClass<CTransformComponent>();
		if (p_transformComponent == nullptr) return nullptr;

		CUIComponent* p_scoreWidget = new CUIComponent(owner, FVector2D{ p_transformComponent->GetLocation().x + s_lateralMargin / 2 - APP_VIRTUAL_WIDTH / 2, APP_VIRTUAL_HEIGHT - s_topMargin + s_widgetTopOffset });
		if (p_scoreWidget == nullptr) return nullptr;

		CUIElementText* p_scoreTextComponent = new CUIElementText(*p_scoreWidget, FVector2D{ 0.f, 10.f }, "0000");
		CUIElementSprite* p_scoreSpriteComponent = new CUIElementSprite(*p_scoreWidget, FVector2D{ 80.f, -3.f }, CGameCostants::UIMoneySprite, 1, 1);
		p_scoreWidget->AddUIElement(p_scoreTextComponent);
		p_scoreWidget->AddUIElement(p_scoreSpriteComponent);

		CGameAuraComponent* p_gameAuraComponent = owner->GetFirstComponentOfClass<CGameAuraComponent>();
		if (p_gameAuraComponent == nullptr) return nullptr;
		
		CGameEventManager* p_eventManager = CGameInstance::GetGameEventManager();
		if (p_eventManager)
		{
			std::function<void(FResourceChangedEvent*)> handler = [p_scoreTextComponent](FResourceChangedEvent* event) {
				if (event->resource != EAuraResource::Coin) return;

				std::ostringstream oss{};
				oss << std::setw(4) << std::setfill('0') << static_cast<int>(round(event->newValue));
				p_scoreTextComponent->SetText(oss.str().c_str());
				};
			p_eventManager->Subscribe<FResourceChangedEvent>(p_gameAuraComponent->GetUniqueID(), handler);

			p_scoreWidget->SetBeginDestroyCallback([p_eventManager, p_gameAuraComponent, handler] {
				p_eventManager->RemoveSubscription<FResourceChangedEvent>(p_gameAuraComponent->GetUniqueID(), handler);
			});
		}

		owner->AddComponent(p_scoreWidget);
		return p_scoreWidget;
	}

	CUIComponent* BuildPlayerStatusComponent(CObject* owner)
	{
		CTransformComponent* p_transformComponent = owner->GetFirstComponentOfClass<CTransformComponent>();
		if (p_transformComponent == nullptr) return nullptr;

		CUIComponent* p_statusWidget = new CUIComponent(owner, FVector2D{ p_transformComponent->GetLocation().x - s_widgetStatusLateralOffset, p_transformComponent->GetLocation().y + APP_VIRTUAL_HEIGHT - s_topMargin + s_widgetTopOffset });
		if (p_statusWidget == nullptr) return nullptr;

		CUIElementSprite* p_statusInvulnerabilitySpriteComponent = new CUIElementSprite(*p_statusWidget, FVector2D{ 0.f, 0.f }, CGameCostants::UIInvulnerabilitySprite, 1, 1);
		p_statusInvulnerabilitySpriteComponent->SetVisibility(false);
		CUIElementSprite* p_statusSpeedBoostSpriteComponent = new CUIElementSprite(*p_statusWidget, FVector2D{ 30.f, 0.f }, CGameCostants::UISpeedBoostSprite, 1, 1);
		p_statusSpeedBoostSpriteComponent->SetVisibility(false);
		CUIElementSprite* p_statusJumpBoostSpriteComponent = new CUIElementSprite(*p_statusWidget, FVector2D{ 60.f, 0.f }, CGameCostants::UIJumpBoostSprite, 1, 1);
		p_statusJumpBoostSpriteComponent->SetVisibility(false);

		p_statusWidget->AddUIElement(p_statusInvulnerabilitySpriteComponent);
		p_statusWidget->AddUIElement(p_statusSpeedBoostSpriteComponent);
		p_statusWidget->AddUIElement(p_statusJumpBoostSpriteComponent);
	
		
		CGameAuraComponent* p_gameAuraComponent = owner->GetFirstComponentOfClass<CGameAuraComponent>();
		if (p_gameAuraComponent == nullptr) return nullptr;

		CGameEventManager* p_eventManager = CGameInstance::GetGameEventManager();
		if (p_eventManager)
		{
			std::function<void(FResourceChangedEvent*)> handler = [p_statusSpeedBoostSpriteComponent, p_statusJumpBoostSpriteComponent, p_statusInvulnerabilitySpriteComponent](FResourceChangedEvent* event) {
				switch (event->resource)
				{
				case EAuraResource::RunSpeed:
					p_statusSpeedBoostSpriteComponent->ToggleVisibility();
					break;
				case EAuraResource::JumpHeight:
					p_statusJumpBoostSpriteComponent->ToggleVisibility();
					break;
				case EAuraResource::Invulnerability:
					p_statusInvulnerabilitySpriteComponent->ToggleVisibility();
					break;
				}
			};
			p_eventManager->Subscribe<FResourceChangedEvent>(p_gameAuraComponent->GetUniqueID(), handler);

			p_statusWidget->SetBeginDestroyCallback([p_eventManager, p_gameAuraComponent, handler] {
				p_eventManager->RemoveSubscription<FResourceChangedEvent>(p_gameAuraComponent->GetUniqueID(), handler);
				});
		}
		owner->AddComponent(p_statusWidget);
		return p_statusWidget;
	}

	CUIComponent* BuildPlayerLivesComponent(CObject* owner)
	{
		CTransformComponent* p_transformComponent = owner->GetFirstComponentOfClass<CTransformComponent>();
		if (p_transformComponent == nullptr) return nullptr;

		CGameAuraComponent* p_gameAuraComponent = owner->GetFirstComponentOfClass<CGameAuraComponent>();
		if (p_gameAuraComponent == nullptr) return nullptr;

		int maxLives = static_cast<int>(p_gameAuraComponent->GetResourceMaxValue(EAuraResource::Health));
		if (maxLives == 0) return nullptr ;

		CUIComponent* p_livesWidget = new CUIComponent(owner, FVector2D{ p_transformComponent->GetLocation().x + APP_VIRTUAL_WIDTH / 2 - s_lateralMargin - s_widgetLivesLateralOffset, p_transformComponent->GetLocation().y + APP_VIRTUAL_HEIGHT - s_topMargin + s_widgetTopOffset });
		if (p_livesWidget == nullptr) return nullptr;

		for (int i = 0; i < static_cast<int>(maxLives); i++)
		{
			CUIElementSprite* p_livesSpriteComponent = new CUIElementSprite(*p_livesWidget, FVector2D{ 30.f * i, 0.f }, CGameCostants::UIHealthSprite, 1, 1);
			p_livesWidget->AddUIElement(p_livesSpriteComponent);
		}

		UpdateLivesWidgetComponentsVisibility(p_livesWidget, static_cast<int>(p_gameAuraComponent->GetResourceValue(EAuraResource::Health)), maxLives - 1, false);

		CGameEventManager* p_eventManager = CGameInstance::GetGameEventManager();

		if (p_eventManager)
		{
			std::function<void(FResourceChangedEvent*)> handler = [p_livesWidget, maxLives](FResourceChangedEvent* event) {
				if (event->resource != EAuraResource::Health) return;

				int startIdx = (event->delta < 0) ? static_cast<int>(event->newValue) : 0;
				int endIdx = (event->delta < 0) ? maxLives - 1 : static_cast<int>(event->newValue) - 1;

				UpdateLivesWidgetComponentsVisibility(p_livesWidget, startIdx, endIdx, event->delta >= 0);
			};
			p_eventManager->Subscribe<FResourceChangedEvent>(p_gameAuraComponent->GetUniqueID(), handler);

			p_livesWidget->SetBeginDestroyCallback([p_eventManager, p_gameAuraComponent, handler] {
				p_eventManager->RemoveSubscription<FResourceChangedEvent>(p_gameAuraComponent->GetUniqueID(), handler);
				});
		}

		owner->AddComponent(p_livesWidget);
		return p_livesWidget;
	}

	void UpdateLivesWidgetComponentsVisibility(CUIComponent* uiComponent, int startIdx, int endIdx, bool visibility) 
	{
		for (int i = startIdx; i <= endIdx; i++) {
			CUIElementSprite* spriteComponent = static_cast<CUIElementSprite*>(uiComponent->GetUIElements()[i]);
			if (spriteComponent != nullptr) {
				spriteComponent->SetVisibility(visibility);
			}
		}
	}
}
