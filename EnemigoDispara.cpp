#include "EnemigoDispara.h"

EnemigoDispara::EnemigoDispara(float x, float y, Game* game)
	: Enemy("res/enemigo.png", x, y, 36, 40, game) {

	state = game->stateMoving;

	aDying = new Animation("res/enemigo_morir.png", width, height,
		280, 40, 1, 8, false, game);

	aHitted = new Animation("res/enemigo_hitted.png", width, height,
		36, 40, 6, 1, false, game);

	aMoving = new Animation("res/enemigo_movimiento.png", width, height,
		108, 40, 6, 3, true, game);
	animation = aMoving;

	vx = 0;
	vy = 0;

	vxIntelligence = 0;
	vyIntelligence = 0;

	vx = vxIntelligence;
	vy = vyIntelligence;

	vidas = 3;

	jugadorX = 0;
	jugadorY = 0;
}

void EnemigoDispara::update(float xPlayer, float yPlayer) {
	// Actualizar la animación
	bool endAnimation = animation->update();

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba muriendo
		if (state == game->stateDying) {
			state = game->stateDead;
		}

		if (state == game->stateHitted) {
			state = game->stateMoving;
		}
	}

	if (state == game->stateMoving) {
		animation = aMoving;
	}
	if (state == game->stateDying) {
		animation = aDying;
	}
	if (state == game->stateHitted) {
		animation = aHitted;
	}

	// Establecer velocidad
	if (state != game->stateDying) {
		if (cdCambioDireccion == 0) {
			vxIntelligence = ((rand() % 3) - 1) * 2;
			vyIntelligence = ((rand() % 3) - 1) * 2;

			vx = vxIntelligence;
			vy = vyIntelligence;

			cdCambioDireccion = 20;
		}
	}
	else {
		vx = 0;
	}

	jugadorX = xPlayer;
	jugadorY = yPlayer;

	if (shootTime > 0) {
		shootTime--;
	}

	if (cdCambioDireccion > 0) {
		cdCambioDireccion--;
	}
}

Enemy* EnemigoDispara::impacted(int damage) {
	if (state != game->stateDying) {
		if ((vidas - damage) > 0) {
			state = game->stateHitted;
			aHitted->currentFrame = 0;
			vidas = vidas - damage;
			// 100 actualizaciones 
		}
		else {
			vidas = 0;
			state = game->stateDying;
		}

		return NULL;
	}
}

void EnemigoDispara::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

ProjectileEnemigo* EnemigoDispara::shoot() {
	if (shootTime == 0 && state != game->stateDying && state != game->stateDead) {
		state = game->stateShooting;
		//aShooting->currentFrame = 0;
		shootTime = shootCadence;

		float distanciaX = abs(jugadorX - x); //a
		float distanciaY = abs(jugadorY - y); //c
		float distanciaDiagonal = sqrt(pow(distanciaX, 2) + pow(distanciaY, 2)); //b

		float anguloDisparo = asin(distanciaX / distanciaDiagonal);

		float vt = 8;

		float vy = vt * cos(anguloDisparo);
		float vx = vt * sin(anguloDisparo);

		if (x > jugadorX) { //Player a la derecha del enemigo
			vx = vx * -1;
		}
		else if (x < jugadorX) { //Player a la izquierda del enemigo
			vx = vx * 1;
		}
		else { //Player con la misma x del enemigo
			vx = 0;
		}

		if (y > jugadorY) { //Player debajo del enemigo
			vy = vy * -1;
		}
		else if (y < jugadorY) { //Player arriba del enemigo
			vy = vy * 1;
		}
		else { //Player a la misma altura del enemigo
			vy = 0;
		}

		cout << "Nuevo proyectilEnemigo. Vx = " << vx << ", Vy = " << vy << endl;
		ProjectileEnemigo* projectile = new ProjectileEnemigo(x, y, vx, vy, game);

		return projectile;
	}
	else {
		return NULL;
	}
}
