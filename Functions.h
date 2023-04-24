#pragma once
#include "Constants.h"

enum class Sound { FIRE, THRUST, BANG_S, BANG_M, BANG_L, SOUC_S, SOUC_B, EXTRA };

struct Shot {
	sf::Vector2f from;
	float angle;
	Shot(sf::Vector2f f, float a) : from(f), angle(a) {}
};

void LoadResources();

void PlaySound(Sound sound);
void LoopSound(Sound sound);
void EndSoundLoop(Sound sound);

const sf::Font& GetFont();

bool IsPointInRange(float x, float a, float b);

float VectorsModule(const sf::Vector2f& vec);

void RotateVector(sf::Vector2f& vec, float angle, const sf::Vector2f& anchor = sf::Vector2f(0.f, 0.f));
void RotateUnitVector(sf::Vector2f& vec, float angle);

float DotProduct(sf::Vector2f& vec1, sf::Vector2f& vec2);
float CrossProduct(sf::Vector2f& vec1, sf::Vector2f& vec2);


sf::Vector2f CalculateNormal(const sf::Vector2f pointA, sf::Vector2f pointB);
std::vector<sf::Vector2f> CalculateAllNormals(const std::vector<sf::Vector2f>& vertices);

bool IsPointInTriangle(sf::Vector2f p, sf::Vector2f a, sf::Vector2f b, sf::Vector2f c);


bool IsPassingVerticalBorder(const sf::Vector2f& vec);
bool IsPassingHorizontalBorder(const sf::Vector2f& vec);
void HandleScreenBorder(sf::Vector2f& vec);

void FormatText(sf::Text& txt);
void TextToCenter(sf::Text& text, float offsetX = 0.5f, float offsetY = 0.5f);

float operator* (sf::Vector2f left, sf::Vector2f right);
