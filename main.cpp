#include <iostream>
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Vec.h"

using namespace sf;
using namespace std;
#define PI acos(-1)

void MathPendum();
void SpringPendum();

int main() {
	//MathPendum();
	const int ImageWidth = 1000, ImageHeight = 1000;
	RenderWindow window1(VideoMode(ImageWidth, ImageHeight), L"pendulum", Style::Default);
	window1.setVerticalSyncEnabled(true);
	const int R = 5;
	CircleShape Body(R);
	Body.setFillColor(Color::Cyan);
	//window1.clear(Color(0xFFFFFFFF));	
	// math pendum angle sol
	double Scale = 10;
	double l = 40;
	Vec g(0, -9.8, 0);
	double dt = 0.00001, t = 0;
	double gMod = g.module();
	double angle = PI / 2, AngleVelocity = 0.0, b = gMod / l * sin(angle);
	Vec Position = Vec(sin(angle), cos(angle), 0) * -l;
	double time = 0;
	double startEnergy = (AngleVelocity * AngleVelocity * l / 2 + gMod * (1 - cos(angle)));
	while (true) {
		if (abs(AngleVelocity) < dt / 5 && Position.x < 0 && t - time > 10 * dt) {
			cout << "Period: " << t - time << '\n';
			time = t;
		}
		//cout << "Energy: " << (AngleVelocity * AngleVelocity * l / 2 + gMod * (1 - cos(angle))) << '\n';
		angle += AngleVelocity * dt + b * dt * dt / 2;
		b = -gMod / l * sin(angle);
		AngleVelocity = AngleVelocity + b * dt;
		double energy = (AngleVelocity * AngleVelocity * l / 2 + gMod * (1 - cos(angle)));
		if (energy < startEnergy)
			AngleVelocity = sqrt(2 * (startEnergy - gMod * (1 - cos(angle))) / l) * (AngleVelocity < 0 ? -1 : 1);
		Position = Vec(sin(angle), cos(angle), 0) * -l;
		t += dt;
		int xPos = -Scale * Position.x + ImageWidth / 2;
		int yPos = -Scale * Position.y + ImageHeight / 2 - l * Scale / 2;
		Vertex line[] =
		{
			Vertex(Vector2f(ImageWidth / 2, ImageHeight / 2 - l * Scale / 2), Color::Blue),
			Vertex(Vector2f(xPos + R, yPos + R), Color::Red)
		};
		if (int(t / dt) % int(0.05 / dt) == 0) {
			window1.clear(Color(0xAAAAAAFF));
			Body.setPosition(xPos, yPos);
			window1.draw(line, 4, sf::Lines);
			window1.draw(Body);
			window1.display();
		}
	}


}
void MathPendum() {
	const int ImageWidth = 1000, ImageHeight = 1000;
	RenderWindow window1(VideoMode(ImageWidth, ImageHeight), L"pendulum", Style::Default);
	window1.setVerticalSyncEnabled(true);
	const int R = 5;
	CircleShape Body(R);
	Body.setFillColor(Color(0x555555FF));
	//window1.clear(Color(0xAAAAAAFF));	
	// math pendum regular sol
	double l = 30;
	Vec Position(0, -l, 0);
	Vec Velocity(20, 0, 0);
	Vec g(0, -9.8, 0);
	double dt = 0.01, t = 0;
	Vec aT(0, 0, 0);
	Vec aN = g;
	Vec Start = Position;
	double gMod = g.module();
	while (true) {
		double angleSin = angleVecSin(Position, Start);
		double angleCos = angleVecCos(Position, Start);
		if (Position.x < Start.x) angleSin = -angleSin;
		aT = Vec(angleSin * angleCos, angleSin * angleSin, 0.0) * -gMod;
		Position = (Position + Velocity * dt + ((aT + aN) * dt * dt) / 2);
		aN = Vec(-angleSin, angleCos, 0) * Velocity.module2() / l;

		if (Position.module() > l)Velocity = (Velocity + (aN + aT) * dt);
		else Velocity = Velocity + g * dt;
		t += dt;
		// view object
		double Scale = 10;
		int xPos = -Scale * Position.x + ImageWidth / 2;
		int yPos = -Scale * Position.y + ImageHeight / 2;
		Vertex line[] =
		{
			Vertex(Vector2f(ImageWidth / 2, ImageHeight / 2 - l * Scale / 2), Color::Blue),
			Vertex(Vector2f(xPos + R, yPos + R), Color::Red)
		};
		window1.clear(Color(0xAAAAAAFF));
		Body.setPosition(xPos, yPos);
		window1.draw(line, 4, sf::Lines);
		window1.draw(Body);
		window1.display();
	}
}

void SpringPendum() {
	const int ImageWidth = 1000, ImageHeight = 1000;
	RenderWindow window1(VideoMode(ImageWidth, ImageHeight), L"pendulum", Style::Default);
	window1.setVerticalSyncEnabled(true);
	CircleShape Body(5);
	Body.setFillColor(Color(0x555555FF));
	window1.clear(Color(0xAAAAAAFF));	
	Vec Position(0, 0, 0);
	Vec Velocity(30, 0, 0);
	double dt = 0.04, t = 0;
	Vec a(0, 0, 0);
	Vec Start = Position;
	// spring pendum 
	while (true) {
		a = (Start - Position) * 4;
		Position = Position + Velocity * dt + (a * dt * dt) / 2;
		Velocity = Velocity + a * dt;
		t += dt;
		// view object
		Body.setPosition(10 * Position.x + 300, Position.y + 200);
		window1.clear(Color(0xAAAAAAFF));
		window1.draw(Body);
		window1.display();
	}
}