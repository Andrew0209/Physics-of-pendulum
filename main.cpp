#include <iostream>
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Vec.h"

using namespace sf;
using namespace std;
#define PI acos(-1)

struct Point
{
	double x;
	double y;
	Point(double xVal, double yVal) : x(xVal), y(yVal) {}
};

void MathPendulum();
void SpringPendulum();
void drawGraph(RenderWindow& window, Point A, Point Max, Color color = Color::Red);
double MathPendulumPeriod(const double& l, const double& g, double startAngle = 0, double angleVelocity = 0);

const double FPS = 20;

int main() {
	const int ImageWidth = 1000, ImageHeight = 1000;
	RenderWindow window1(VideoMode(ImageWidth, ImageHeight), L"pendulum", Style::Default);
	window1.setVerticalSyncEnabled(true);
	window1.setPosition(Vector2<int>(0,0));
	const int R = 5; // circle radius
	CircleShape Body(2*R);
	Body.setFillColor(Color::Cyan);

	RenderWindow windowGraph(VideoMode(800, 800, 0), L"Angle Graph", Style::Default);
	windowGraph.setVerticalSyncEnabled(true);
	windowGraph.setPosition(Vector2<int>(ImageWidth, 0));

	RectangleShape fon(sf::Vector2f(windowGraph.getSize()));
	fon.setFillColor(Color::White);
	windowGraph.draw(fon);
	//windowGraph.display();

	// rewrite it so angle velocity and angle axceleration are vectors
	// 
	// math pendulum angle sol
	double Scale = 10; // scale between picture and coordinates
	double l = 40; // string length
	Vec g(0, -9.8, 0); 
	double dt = 0.0001, t = 0;
	double gMod = g.module();
	double angle = PI / 2, AngleVelocity = 0.0,AngleAxceleration= gMod / l * sin(angle);
	Vec Position = Vec(sin(angle), cos(angle), 0) * -l;
	double time = 0;

	const double finishTime = 1000;
	const double schoolPeriod = 2 * PI * sqrt(l / gMod);
	const double startAngle = angle;
	const double startEnergy = (AngleVelocity * AngleVelocity * l / 2 + gMod * (1 - cos(angle)));
	const double period = MathPendulumPeriod(l, gMod, startAngle);
	const double maxVel = 2 * sqrt(2 * gMod / l * (1 - cos(startAngle)));
	
	// period(amplitude) function
	windowGraph.setPosition(Vector2<int>(0, 0));
	window1.close();
	double step = PI / 3000;
	angle = 0;
	std::cout << "start \n";
	while (angle < PI) {
		double per = MathPendulumPeriod(l, gMod, angle);
		if(per >= schoolPeriod)
			drawGraph(windowGraph, Point(angle, per - schoolPeriod), Point(PI, 3 * schoolPeriod), Color::Blue);
		angle += step;
		//if(int(angle / step) % 50 == 0)windowGraph.display();
	}
	std::cout << "end \n";
	windowGraph.display();
	while (windowGraph.isOpen());
	return 0;

	while (t < finishTime) {
		// update angle, angle velocity, angle axceleration
		angle += AngleVelocity * dt +AngleAxceleration* dt * dt / 2;
		AngleAxceleration = -gMod / l * sin(angle) /* - 0.03 * (AngleVelocity / maxVel)*/;
		AngleVelocity = AngleVelocity + AngleAxceleration * dt;
		double energy = (AngleVelocity * AngleVelocity * l / 2 + gMod * (1 - cos(angle)));

		// energy safe
		//cout << energy / startEnergy << '\n';
		if (energy < startEnergy)
			AngleVelocity = sqrt(2 * (startEnergy - gMod * (1 - cos(angle))) / l) * (AngleVelocity < 0 ? -1 : 1);
		Position = Vec(sin(angle), cos(angle), 0) * -l;
		
		//update time
		t += dt;

		//	update image FPS times per second
		if (int(t / dt) % int(1 / FPS / dt) == 0) {
			int xPos = -Scale * Position.x + ImageWidth / 2;
			int yPos = -Scale * Position.y + ImageHeight / 2;
			Vertex line[] =
			{
				Vertex(Vector2f(ImageWidth / 2, ImageHeight / 2), Color::Blue),
				Vertex(Vector2f(xPos + 2 * R, yPos + 2 * R), Color::Red)
			};
			window1.clear(Color(0xAAAAAAFF));
			Body.setPosition(xPos, yPos);
			window1.draw(line, 4, sf::Lines);
			window1.draw(Body);
			window1.display();
			windowGraph.display();
		}
		//if (int(t / dt) % int(1 / (. * FPS) / dt) == 0) {
		//	windowGraph.display();
		//}
		double w = 2 * PI / period;
		//drawGraph(windowGraph, Point(t, startAngle * (cos(w * t) + 1)), Point(finishTime, 2 * startAngle), Color::Blue);
		//drawGraph(windowGraph, Point(t, angle + startAngle), Point(finishTime, 2 * startAngle));
		drawGraph(windowGraph, Point(angle + startAngle, AngleVelocity + maxVel), Point(2 * startAngle, 2 * maxVel), Color::Blue);
	}
}


void MathPendulum() {
	// Problems
	//  1) unfixed string length 
	//  2) no energy keeping control  
	const int ImageWidth = 1000, ImageHeight = 1000;
	RenderWindow window1(VideoMode(ImageWidth, ImageHeight), L"pendulum", Style::Default);
	window1.setVerticalSyncEnabled(true);
	const int R = 5;
	CircleShape Body(R);
	Body.setFillColor(Color(0x555555FF));
	//window1.clear(Color(0xAAAAAAFF));	
	// math pendulum regular sol
	double l = 30;
	Vec Position(0, -l, 0);
	Vec Velocity(20, 0, 0);
	Vec g(0, -9.8, 0);
	double dt = 0.001, t = 0;
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
		//window1.display();n
	}
}

// Simpliest pendulum wich has analitic solution
void SpringPendulum() {
	const int ImageWidth = 1000, ImageHeight = 1000;
	RenderWindow window1(VideoMode(ImageWidth, ImageHeight), L"pendulum", Style::Titlebar);
	window1.setVerticalSyncEnabled(true);
	CircleShape Body(5);
	Body.setFillColor(Color(0x555555FF));
	window1.clear(Color(0xAAAAAAFF));	
	Vec Position(0, 0, 0);
	Vec Velocity(30, 0, 0);
	double dt = 0.04, t = 0;
	Vec a(0, 0, 0);
	Vec Start = Position;
	// spring pendulum 
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

void drawGraph(RenderWindow& window, Point A, Point Max, Color color) {
	auto size = window.getSize();
	CircleShape point(2);
	point.setFillColor(color);
	point.setPosition(10 + (size.x - 20) / Max.x * A.x, 10 + (size.y - 20) * (1 - A.y / Max.y));
	window.draw(point);
	//window.display();
}


double MathPendulumPeriod(const double& l,const double& g, double angle, double angleVelocity) {
	// make function 
	double schoolPeriod = 2 * PI * sqrt(l / g);
	if (angle == 0 && angleVelocity == 0)return schoolPeriod;
	double startAngle = angle;
	double t = 0, dt = 0.0005, lastTime = 0;
	double AngleAxceleration = 0;
	const double startEnergy = (angleVelocity * angleVelocity * l / 2 + g * (1 - cos(angle)));
	const int count = 2;
	vector <double> period(count, 0);
	int a = 0;
	while (a < count) {
		if (abs(angle) > PI)return -1;
		if ((abs(angleVelocity) < dt / 1 && angle / startAngle > 0 && t - lastTime > schoolPeriod * 0.9)) {
			period[a] = t - lastTime;
			lastTime = t;
			a++;
		}
		// update angle, angle velocity, angle axceleration
		angle += angleVelocity * dt + AngleAxceleration * dt * dt / 2;
		AngleAxceleration = -g / l * sin(angle);
		angleVelocity = angleVelocity + AngleAxceleration * dt;
		double energy = (angleVelocity * angleVelocity * l / 2 + g * (1 - cos(angle)));
		// energy safe
		if (energy < startEnergy)
			angleVelocity = sqrt(2 * (startEnergy - g * (1 - cos(angle))) / l) * (angleVelocity < 0 ? -1 : 1);
		//update time
		t += dt;
	}
	return period[count - 1];
}
