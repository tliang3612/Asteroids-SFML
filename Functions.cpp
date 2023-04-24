#include "Functions.h"
#include "Line.h"

std::vector<sf::SoundBuffer> buffers;
std::vector<sf::Sound> sounds;

sf::Music oddity;

void LoadResources() {
	oddity.openFromFile("res/sounds/SpaceOddity320.wav");
	oddity.setLoop(true);
	oddity.play();

	buffers.reserve(soundFileNames.size());
	sounds.reserve(soundFileNames.size());

	for (auto& name : soundFileNames) {
		buffers.push_back(sf::SoundBuffer());
		buffers.back().loadFromFile("res/sounds/" + name + ".wav");
		sounds.push_back(sf::Sound(buffers.back()));
		sounds.back().setVolume(30.f);
	}
}

void PlaySound(Sound s) {
	sounds[(int)s].play();
}

void LoopSound(Sound s) {
	sounds[(int)s].setLoop(true);
	PlaySound(s);
}

void EndSoundLoop(Sound s){
	sounds[(int)s].stop();
}

const sf::Font& GetFont() {
	bool loaded = false;
	static sf::Font font;
	if (!loaded) {
		font.loadFromFile("res/vector.ttf");
		loaded = true;
	}
	return font;
}

bool IsPointInRange(float x, float a, float b) {
	if (x == a) 
		return true;
	if (x == b) 
		return true;
	if (x > std::min(a, b) && x < std::max(a, b)) 
		return true;
	return false;
}

float VectorsModule(const sf::Vector2f& vec){
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

///Rotation using transformation matrices
///p1x = cos(theta1),		   p1y = sin(theta1)
///p2x = cos(theta1 + theta2), p2y = sin(theta1 + theta2)
/// 
/// p2x = cos(theta1) * sin(theta2) - sin(theta1) * cos(theta2)
/// p2x = p1x * cos(theta2) - p1y * sin(theta2) 
/// 
/// |p2x| = |cos(theta2)  -sin(theta2)||p1x|
/// |p2y|	|sin(theta2)   cos(theta2)||p2x|
void RotateVector(sf::Vector2f& vec, float angle, const sf::Vector2f& anchor) {
	//we do vec.x - anchor.x to get origin							
	float p2x = (vec.x - anchor.x) * cos(angle) - (vec.y - anchor.y) * sin(angle) + anchor.x;
	float p2y = (vec.x - anchor.x) * sin(angle) + (vec.y - anchor.y) * cos(angle) + anchor.y;
	vec.x = p2x;
	vec.y = p2y;
}

///Rotates the vector around the unit circle given the angle
void RotateUnitVector(sf::Vector2f& vec, float angle){
	vec.x = sin(angle);
	vec.y = -cos(angle); //-cos because 0 is the top, and screen height is at the bottom, which is the opposite of standard math convention
}


float DotProduct(sf::Vector2f& vecA, sf::Vector2f& vecB)
{
	return (vecA.x * vecB.x + vecA.y * vecB.y);
}

float CrossProduct(sf::Vector2f& vec1, sf::Vector2f& vec2){
	return vec1.x * vec2.y - vec1.y * vec2.x;
}

///returns the normal perpendicular to the vector
sf::Vector2f CalculateNormal(const sf::Vector2f pointA, sf::Vector2f pointB) {
	//swap and negate
	sf::Vector2f directionVector = pointB - pointA;
	return sf::Vector2f(directionVector.y, -directionVector.x);
}

std::vector<sf::Vector2f> CalculateAllNormals(const std::vector<sf::Vector2f>& vertices)
{
	std::vector<sf::Vector2f> normals;

	for (size_t i = 0; i < vertices.size()-1; i++)
	{
		normals.push_back(CalculateNormal(vertices[i], vertices[i + 1]));
	}
	
	normals.push_back(CalculateNormal(vertices[vertices.size() - 1], vertices[0]));
	return normals;
}

bool IsPassingVerticalBorder(const sf::Vector2f& vec){
	if (vec.x >= WIDTH)  
		return true;
	else if (vec.x <= 0) 
		return true;
	return false;
}

bool IsPassingHorizontalBorder(const sf::Vector2f& vec){
	if (vec.y >= HEIGHT) 
		return true;
	else if (vec.y <= 0) 
		return true;
	return false;
}

void HandleScreenBorder(sf::Vector2f& vec) {
	if (vec.x >= WIDTH)  
		vec.x -= WIDTH;
	else if (vec.x <= 0) 
		vec.x += WIDTH;
	if (vec.y >= HEIGHT) 
		vec.y -= HEIGHT;
	else if (vec.y <= 0) 
		vec.y += HEIGHT;
}

void FormatText(sf::Text& txt) {
	txt.setOutlineColor(sf::Color::White);
	txt.setOutlineThickness(0.5);
	txt.setFillColor(sf::Color::White);
}

void TextToCenter(sf::Text& txt, float offsetX, float offsetY) {
	txt.setPosition(WIDTH  * offsetX - txt.getGlobalBounds().width  / 2,
					HEIGHT * offsetY - txt.getGlobalBounds().height / 2);
}

float operator* (sf::Vector2f left, sf::Vector2f right){
	return (left.x * right.x) + (left.y * right.y);
}
