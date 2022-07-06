
#include <SFML/Window.hpp>
#include <bits/stdc++.h>
using namespace std;

//this struct creates a circle with given radius
// it alse has some properties regarding it like its current position,acceleration,velocity
// it also contains a function which updates its position and velocity whenever we call it

// struct verlet
// {
// public:
// 	sf::CircleShape* circle = new sf::CircleShape();
// 	sf::Vector2f curPos;
// 	sf::Vector2f acc;
// 	sf::Vector2f vel;
// 	float mass;
// 	verlet(float a, sf::Vector2f ini)
// 	{
// 		circle->setRadius(a);
// 		curPos = ini;
// 		vel = { 0, 0 };
// 		acc = { 0, 1 };
// 		mass = 4;
// 	}

// 	void update()
// 	{
// 		curPos = curPos + vel;
// 		vel = vel + acc;
// 	}
// };

// void collision(verlet&, verlet&, float);
// void collision(verlet& a, verlet& b, float rad)
// {
// 	auto Dist = sqrt(pow((b.curPos.x - a.curPos.x), 2) + pow((b.curPos.y - a.curPos.y), 2));
// 	if (Dist > 2 * rad)
// 		return;
// 	//relative velocity
// 	sf::Vector2f Vrel = b.vel - a.vel;

// 	//normal
// 	sf::Vector2f normal = b.curPos - a.curPos;
// 	normal.x /= Dist;
// 	normal.y /= Dist;

// 	//rel velocity along the normal
// 	auto velAlongNor = Vrel.x * normal.x + Vrel.y * normal.y;

// 	if (velAlongNor > 0)
// 		return;
// 	//impulse scalar
// 	float j = -(1 + 0.5) * velAlongNor;
// 	j /= 1 / a.mass + 1 / b.mass;

// 	// Apply impulse
// 	sf::Vector2f impulse = j * normal;

// 	a.vel -= 1 / a.mass * impulse;
// 	b.vel += 1 / b.mass * impulse;
// }
struct verlet
{
	sf::CircleShape* circle = new sf::CircleShape;
	sf::Vector2f position_current, position_old, acceleration;
	float radius;
	verlet(float rad, sf::Vector2f loc)
	{
		radius = rad;
		circle->setRadius(radius);
		position_current = loc;
		position_old = loc;
		acceleration = { 0.f, 300.f };
	}

	void update(float dt)
	{
		sf::Vector2f velo = position_current - position_old;
		//save current position
		position_old = position_current;
		//verlet integration
		position_current = position_current + velo + acceleration * dt * dt;
	}
};
void newCollision(verlet&, verlet&);
void newCollision(verlet& a, verlet& b)
{
	sf::Vector2f collaxis = a.position_current - b.position_current;
	float dist = sqrt(pow((b.position_current.x - a.position_current.x), 2) + pow((b.position_current.y - a.position_current.y), 2));
	float radDist = a.circle->getRadius() + b.circle->getRadius();
	if (dist < radDist)
	{
		sf::Vector2f n = collaxis / dist;
		float delta = radDist - dist;
		a.position_current += 0.5f * delta * n;
		b.position_current -= 0.5f * delta * n;
		// a.vel = -(0.5f) * a.vel;
		// b.vel = -(0.5f) * b.vel;
	}
}

void pusher(vector<verlet>&, sf::Vector2i, float);
void pusher(vector<verlet>& all, sf::Vector2i loc, float radius)
{
	sf::Vector2f radVec = { radius, radius };
	verlet cir(radius, (sf::Vector2f)loc - radVec);
	all.push_back(cir);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 900), "Phy6", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(0);
	sf::RectangleShape ground(sf::Vector2f(600.f, 5.f));
	ground.setPosition(sf::Vector2f(150.f, 700.f));
	sf::Font druk, product;
	druk.loadFromFile("F:\\Project stuff\\phy6\\Druk.ttf");
	product.loadFromFile("F:\\Project stuff\\phy6\\Product.ttf");
	sf::Text size, phy6, samar;
	samar.setFont(product);
	samar.setString("samartheory");
	samar.setPosition({ 370.f, 770.f });
	samar.setCharacterSize(30);

	phy6.setFont(druk);
	phy6.setString("phy6");
	phy6.setPosition({ 350.f, 700.f });
	phy6.setCharacterSize(60);

	size.setFont(druk);
	size.setString("size");
	size.setPosition({ 705.f, 200.f });
	size.setCharacterSize(30);
	//this contains all the circles
	vector<verlet> all;
	//10 - 50
	float radius = 20.f, dt = 0.05f;
	//event loop / game loop
	while (window.isOpen())
	{
		sf::Event ent;

		//if there are any pending events
		while (window.pollEvent(ent))
		{
			if (ent.type == sf::Event::Closed)
				window.close();
			//to create a circle

			else if (ent.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				{
					if (radius > 10.f)
						radius -= 1.f;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{
					if (radius < 50.f)
						radius += 1.f;
				}
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2i localPosition = sf::Mouse::getPosition(window);
				pusher(all, localPosition, radius);
			}
			//clear all circles
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				all.clear();
			}
		}
		sf::CircleShape indicator(radius);
		indicator.setFillColor(sf::Color::Red);
		indicator.setPosition({ 750.f - radius, 150.f - radius });
		//updating the position of all circles
		for (unsigned int i = 0; i < all.size(); i++)
		{
			all[i].circle->setPosition(all[i].position_current);
			all[i].update(dt);
			//constraint
			if (all[i].position_current.y > 700 - 2 * all[i].radius && all[i].position_current.x > 150.f - all[i].radius && all[i].position_current.x < 750.f - radius && all[i].position_current.y < 705.f)
			{
				all[i].position_current.y = 700 - 2 * all[i].radius;
				// all[i].acc.y = 0;
			}
		}

		for (int i = 0; i < (int)all.size() - 1; i++)
		{
			for (int j = i + 1; j < (int)all.size(); j++)
			{
				newCollision(all[i], all[j]);
			}
		}
		window.clear();
		//drawing the circles with new position
		for (unsigned int i = 0; i < all.size(); i++)
			window.draw(*all[i].circle);

		// cout << all.size() << endl;
		window.draw(indicator);
		window.draw(ground);
		window.draw(size);
		window.draw(phy6);
		window.draw(samar);
		window.display();
	}
	return 0;
}