#pragma once

struct VectorPair {
	sf::Vector2f first;
	sf::Vector2f second;
	VectorPair(sf::Vector2f f, sf::Vector2f s) : 
		first(std::move(f)), 
		second(std::move(s)) 
	{
	}
};

const float PLAYER_THRUST_FORCE			= 2.0f;

const int LEADERBOARD_MAX_SIZE			= 10;
const int NUM_OF_SPARKS					= 20;
const int BULLETS_MAX_CAPACITY			= 50;
const int ASTEROID_MAX_SIZE				= 8;
const int ASTEROIDS_MAX_QUANTITY		= 100;
const float SMALL_ASTEROID_MAX_RADIUS	= 20.f;
const float DEFAULT_ACTOR_SPEED			= 900.0f;
const float SAUCER_SPAWN_DELAY			= 7.0f;

const float SPARKS_LIFETIME				= 1.5f;

constexpr double PI						= 3.14159265358979323846f;

const std::string S_PRESS_ANY_KEY	= "press any key";
const std::string S_GAME_OVER		= "game over";
const std::string S_NEW_HIGH_SCORE	= "your score is one of the ten best\nplease enter your initials\npush rotate to select letter\npush hyperspace when letter is correct";
const std::string S_HIGH_SCORE		= "high score";

const int WIDTH = sf::VideoMode::getDesktopMode().width;
const int HEIGHT = sf::VideoMode::getDesktopMode().height;

const sf::Vector2f PLAYER_DEFAULT_POS(WIDTH / 2.0f, HEIGHT / 2.0f);

static std::random_device rd;
static std::mt19937_64 gen(rd());

const std::uniform_int_distribution<int> RAND_BOOL(0, 1);
const std::uniform_int_distribution<int> RAND_X(0, WIDTH);
const std::uniform_int_distribution<int> RAND_Y(0, HEIGHT);

const std::uniform_real_distribution<float> RAND_ASTEROID_RADIUS(SMALL_ASTEROID_MAX_RADIUS/2.f, SMALL_ASTEROID_MAX_RADIUS);
const std::uniform_real_distribution<float> RAND_DIR(-0.1f, 0.1f);
const std::uniform_real_distribution<float> RAND_ANGLE_HALF_RAD(0.5f, 1.0f);
const std::uniform_real_distribution<float> RAND_ANGLE_360(0.0f, 6.28319f);
const std::uniform_real_distribution<float> RAND_DISPERSION(-0.05f, 0.05f);

enum class Values{ASTR_B = 20, ASTR_M = 50, ASTR_S = 100, SAUC_B = 200, SAUC_S = 1000};

const std::vector<VectorPair> STARSHIP_PATTERN{
	VectorPair(sf::Vector2f(  0.0f, -20.0f),	sf::Vector2f( 10.0f,  10.0f)),
	VectorPair(sf::Vector2f( 10.0f,  10.0f),	sf::Vector2f(  6.0f,   6.0f)),
	VectorPair(sf::Vector2f(  6.0f,   6.0f),	sf::Vector2f( -6.0f,   6.0f)),
	VectorPair(sf::Vector2f( -6.0f,   6.0f),	sf::Vector2f(-10.0f,  10.0f)),
	VectorPair(sf::Vector2f(-10.0f,  10.0f),	sf::Vector2f(  0.0f, -20.0f))
};

const std::vector<VectorPair> FLAME_PATTERN{
	VectorPair(sf::Vector2f(  5.0f,   6.0f), sf::Vector2f(  0.0f,  15.0f)),
	VectorPair(sf::Vector2f(  0.0f,  15.0f), sf::Vector2f( -5.0f,   6.0f))
};

const std::vector<VectorPair> BIG_SAUCER_PATTERN{
	VectorPair(sf::Vector2f( -9.f,  -19.5f),	sf::Vector2f(  9.f, -19.5f)),
	VectorPair(sf::Vector2f(  9.f,  -19.5f),	sf::Vector2f( 15.f,  -7.5f)),
	VectorPair(sf::Vector2f( 15.f,   -7.5f),	sf::Vector2f(-15.f,  -7.5f)),
	VectorPair(sf::Vector2f(-15.f,   -7.5f),	sf::Vector2f( -9.f, -19.5f)),
	VectorPair(sf::Vector2f(-15.f,   -7.5f),	sf::Vector2f(-30.f,   4.5f)),
	VectorPair(sf::Vector2f(-30.f,    4.5f),	sf::Vector2f( 30.f,   4.5f)),
	VectorPair(sf::Vector2f( 30.f,    4.5f),	sf::Vector2f( 15.f,  -7.5f)),
	VectorPair(sf::Vector2f( 30.f,    4.5f),	sf::Vector2f( 18.f,  13.5f)),
	VectorPair(sf::Vector2f( 18.f,   13.5f),	sf::Vector2f(-18.f,  13.5f)),
	VectorPair(sf::Vector2f(-18.f,   13.5f),	sf::Vector2f(-30.f,   4.5f)) 
};

const std::vector<VectorPair> SMALL_SAUCER_PATTERN{
	VectorPair(sf::Vector2f( -6.f, -13.f),	sf::Vector2f(  6.f, -13.f)),
	VectorPair(sf::Vector2f(  6.f, -13.f),	sf::Vector2f( 10.f,  -5.f)),
	VectorPair(sf::Vector2f( 10.f,  -5.f),	sf::Vector2f(-10.f,  -5.f)),
	VectorPair(sf::Vector2f(-10.f,  -5.f),	sf::Vector2f( -6.f, -13.f)),
	VectorPair(sf::Vector2f(-10.f,  -5.f),	sf::Vector2f(-20.f,   3.f)),
	VectorPair(sf::Vector2f(-20.f,   3.f),	sf::Vector2f( 20.f,   3.f)),
	VectorPair(sf::Vector2f( 20.f,   3.f),	sf::Vector2f( 10.f,  -5.f)),
	VectorPair(sf::Vector2f( 20.f,   3.f),	sf::Vector2f( 12.f,   9.f)),
	VectorPair(sf::Vector2f( 12.f,   9.f),	sf::Vector2f(-12.f,   9.f)),
	VectorPair(sf::Vector2f(-12.f,   9.f),	sf::Vector2f(-20.f,   3.f)) 
};


const std::vector<std::string> soundFileNames{
	"fire",
	"thrust",
	"bangSmall",
	"bangMedium",
	"bangLarge",
	"saucerSmall",
	"saucerBig",
	"extraShip"
};