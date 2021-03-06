#include "Bullet.h"
#include "MoveComponent.h"
#include "Enemy.h"
#include "Engine.h"
#include <Vector2.h>
#include "Transform2D.h"
#include <raylib.h>

Bullet::Bullet(Actor* owner) 
	: Actor(0, 0, "Bullet") {
	m_owner = owner;
	m_enabled = false;
	m_moveComponent = nullptr;
	m_timer = 0;
}

Bullet::~Bullet() {
	m_owner = nullptr;
}

void Bullet::shoot() {
	resetPosition();
	getMoveComponent()->setVelocity(m_owner->getTransform()->getForward() * 100);
	enable();
}

void Bullet::enable() {
	m_enabled = true;
	m_timer = 0;
}

void Bullet::resetPosition() {
	getTransform()->setWorldPostion(m_owner->getTransform()->getWorldPosition());
}

void Bullet::start() {
	getTransform()->setScale({ 2, 2 });
	m_moveComponent = new MoveComponent(500, "MoveComponent");
	addComponent(m_moveComponent);
	Actor::start();
}

void Bullet::update(float deltaTime) {
	m_timer += deltaTime;

	if (m_enabled && (m_timer > 5)) {
		m_enabled = false;
	}

	if (m_enabled)
		Actor::update(deltaTime);
}

void Bullet::draw() {
	if (m_enabled) {
		//Draws this bullet to the screen
		MathLibrary::Vector2 position = getTransform()->getWorldPosition();
		RAYLIB_H::DrawCircle(position.x, position.y, getTransform()->getScale().x, BLACK);
		Actor::draw();
	}
}

void Bullet::onCollision(Actor* actor) {
	//If this actor is a bullet, remove it from the scene
	Enemy* enemy = dynamic_cast<Enemy*>(actor);

	if (enemy) {
		Engine::getCurrentScene()->removeActor(enemy);
	}
	Actor::onCollision(actor);
}
