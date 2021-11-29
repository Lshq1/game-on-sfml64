#include <SFML/Graphics.hpp>
#include "map.h"
#include "View.h"
#include <sstream>
#include "mission.h"
//#include <iostream>
using namespace sf;
class Player { // класс Игрока
private:
	float x, y;
public:
	float w, h, dx, dy, speed = 0; //координаты игрока х и у, высота ширина, ускорение (по х и по у), сама скорость
	int dir = 0, playerScore; //направление (direction) движения игрока
	int health; 
	bool life;
	String File; //файл с расширением
	Image image;//сфмл изображение
	Texture texture;//сфмл текстура
	Sprite sprite;//сфмл спрайт

	Player(String F, float X, float Y, float W, float H) 
	{  //Конструктор с параметрами(формальными) для класса Player. При создании объекта класса мы будем задавать имя файла, координату Х и У, ширину и высоту
		playerScore = 0;
		health = 100;
		life = true;
		File = F;//имя файла+расширение
		w = W; h = H;//высота и ширина
		image.loadFromFile("C://Users//lesha//source//repos//sfml64//x64//image//hero.png");//запихиваем в image наше изображение вместо File мы передадим то, что пропишем при создании объекта. В нашем случае "hero.png" и получится запись идентичная 	image.loadFromFile("images/hero/png");
		image.createMaskFromColor(Color(41, 33, 59));//убираем ненужный темно-синий цвет, эта тень мне показалась не красивой.
		image.createMaskFromColor(Color(255, 255, 255));
		texture.loadFromImage(image);//закидываем наше изображение в текстуру
		sprite.setTexture(texture);//заливаем спрайт текстурой
		x = X; y = Y;//координата появления спрайта
		sprite.setTextureRect(IntRect(25, 5, w - 50, h));  //Задаем спрайту один прямоугольник для вывода одного льва, а не кучи львов сразу. IntRect - приведение типов
	}



	void update(float time) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	{
		switch (dir)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
		{
		case 0: dx = speed; dy = 0;   break;//по иксу задаем положительную скорость, по игреку зануляем. получаем, что персонаж идет только вправо
		case 1: dx = -speed; dy = 0;   break;//по иксу задаем отрицательную скорость, по игреку зануляем. получается, что персонаж идет только влево
		case 2: dx = 0; dy = speed;   break;//по иксу задаем нулевое значение, по игреку положительное. получается, что персонаж идет только вниз
		case 3: dx = 0; dy = -speed;   break;//по иксу задаем нулевое значение, по игреку отрицательное. получается, что персонаж идет только вверх
		}

		x += dx * time;//то движение из прошлого урока. наше ускорение на время получаем смещение координат и как следствие движение
		y += dy * time;//аналогично по игреку

		speed = 0;//зануляем скорость, чтобы персонаж остановился.
		sprite.setPosition(x, y); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
		interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой
		if (health <= 0)
		{
			

			life = false;
			speed = 0;
		}
	}
	void interactionWithMap()//ф-ция взаимодействия с картой
	{

		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; j < (x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
			{
				if (TileMap[i][j] == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
				{
					if (dy > 0)//если мы шли вниз,
					{
						y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
					}
					if (dy < 0)
					{
						y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
					}
					if (dx > 0)
					{
						x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//аналогично идем влево
					}
				}

				if (TileMap[i][j] == 's') { //если символ равен 's' (камень)
					
					TileMap[i][j] = ' ';//убираем камень, типа взяли бонус. можем и не убирать, кстати.
					playerScore++;
					
				}
				if (TileMap[i][j] == 'f') { //если колючка то - 40 жизни

					TileMap[i][j] = ' ';//убираем камень, типа взяли бонус. можем и не убирать, кстати.

					health -= 40;
					if (health < 0)
					{
						health = 0;
					}

				}
				if (TileMap[i][j] == 'h') { //если сердечко то + 20 жизни

					TileMap[i][j] = ' ';//убираем камень, типа взяли бонус. можем и не убирать, кстати.
					health += 20;

				}
			}
	}
	float getplayercoordinateX()
	{
		return x;
	}
	float getplayercoordinateY()
	{
		return y;
	}
};
int main()
{
	RenderWindow window(sf::VideoMode(648, 480), "Lesson 8. kychka-pc.ru");
	view.reset(sf::FloatRect(0, 0, 640, 480));//размер "вида" камеры при создании объекта вида камеры. (потом можем менять как хотим) Что то типа инициализации.
	bool showMissionText = true;//логическая переменная, отвечающая за появление текста миссии на экране
	float CurrentFrame = 0;//хранит текущий кадр
	Clock clock;
	Clock gameTimeClock;
	int gameTime = 0;
	Image map_image;
	map_image.loadFromFile("C://Users//lesha//source//repos//sfml64//x64//image//map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);
	Player p("hero.png", 250, 250, 96.0, 96.0);//создаем объект p класса player,задаем "hero.png" как имя файла+расширение, далее координата Х,У, ширина, высота.
	Font font;
	font.loadFromFile("C://Users//lesha//source//repos//sfml64//x64//font//CyrilicOld.ttf");
	Text text("", font, 20);
	text.setFillColor(Color::Red);

	Image quest_image; // IMAGE
	quest_image.loadFromFile("images/missionbg.jpg");
	quest_image.createMaskFromColor(Color(0, 0, 0));
	Texture quest_texture; // TEXTURE
	quest_texture.loadFromImage(quest_image);
	Sprite s_quest; // SPRITE
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(IntRect(0, 0, 340, 510));  //приведение типов, размеры картинки исходные
	s_quest.setScale(0.6f, 0.6f);//чуть уменьшили картинку, => размер стал меньше

	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 3000;


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)//событие нажатия клавиши
				if ((event.key.code == Keyboard::Tab)) {//если клавиша ТАБ
					switch (showMissionText) {//переключатель, реагирующий на логическую переменную showMissionText

					case true: {
						std::ostringstream playerHealthString;//строка здоровья игрока
						playerHealthString << p.health; //заносим в строку здоровье 
						std::ostringstream task;//строка текста миссии
						task << getTextMission(getCurrentMission(p.getplayercoordinateX()));//вызывается функция getTextMission (она возвращает текст миссии), которая принимает в качестве аргумента функцию getCurrentMission(возвращающую номер миссии), а уже эта ф-ция принимает в качестве аргумента функцию p.getplayercoordinateX() (эта ф-ция возвращает Икс координату игрока)
						text.setString("Здоровье: " + playerHealthString.str() + "\n" + task.str());//задаем
						text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//позиция всего этого текстового блока
						s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//позиция фона для блока
						showMissionText = false;//эта строка позволяет убрать все что мы вывели на экране
						break;//выходим , чтобы не выполнить условие "false" (которое ниже)
					}
					case false: {
						text.setString("");//если не нажата клавиша таб, то весь этот текст пустой
						showMissionText = true;// а эта строка позволяет снова нажать клавишу таб и получить вывод на экран
						break;
					}
					}
				}
		}
		}

		if (p.life)
		{
			gameTime = gameTimeClock.getElapsedTime().asSeconds();//игровое время в секундах идёт вперед, пока жив игрок, перезагружать как time его не надо. оно не обновляет логику игры
			///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
			if ((Keyboard::isKeyPressed(Keyboard::Left))) {
				p.dir = 1; p.speed = 0.1;//dir =1 - направление вверх, speed =0.1 - скорость движения. Заметьте - время мы уже здесь ни на что не умножаем и нигде не используем каждый раз
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 136, 96, 56)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
				getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());//передаем координаты игрока в функцию управления камерой
			}

			if ((Keyboard::isKeyPressed(Keyboard::Right))) {
				p.dir = 0; p.speed = 0.1;//направление вправо, см выше
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 232, 96, 56));  //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
				getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());//передаем координаты игрока в функцию управления камерой
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up))) {
				p.dir = 3; p.speed = 0.1;//направление вниз, см выше
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame) + 25, 303, 46, 86));  //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
				getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());//передаем координаты игрока в функцию управления камерой

			}

			if ((Keyboard::isKeyPressed(Keyboard::Down))) { //если нажата клавиша стрелка влево или англ буква А
				p.dir = 2; p.speed = 0.1;//направление вверх, см выше
				CurrentFrame += 0.005 * time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
				if (CurrentFrame > 3) CurrentFrame -= 3; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame)+25, 0, 46, 96)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
				getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());//передаем координаты игрока в функцию управления камерой
			}
		}
		
		

		
		viewmap(time);
		p.update(time);//оживляем объект p класса Player с помощью времени sfml, передавая время в качестве параметра функции update. благодаря этому персонаж может двигаться

		window.setView(view);//"оживляем" камеру в окне sfml
		window.clear();
		/////////////////////////////Рисуем карту/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
				if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик
				if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(96, 0, 32, 32));//добавили цветок
				if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(128, 0, 32, 32));//и сердечко

				s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

				window.draw(s_map);//рисуем квадратики на экран
			}
		if (!showMissionText) { window.draw(s_quest); window.draw(text); }//рисуем спрайт свитка (фон для текста миссии). а затем и текст. все это завязано на логическую переменную, которая меняет свое состояние от нажатия клавиши ТАБ
		std::ostringstream playerhpString,gameTimeString;    // объявили переменную
		playerhpString << p.health;//занесли в нее число очков, то есть формируем строку
		gameTimeString << gameTime;
		text.setString("Жизни:" + playerhpString.str() + "\nВремя игры: " + gameTimeString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str()
		text.setPosition(view.getCenter().x + 150, view.getCenter().y-200);//задаем позицию текста, центр камеры
		
		window.draw(text);//рисую этот текст
		if (p.life == false) {
			p.sprite.setTextureRect(IntRect(20, 400, 51, 430));
		}
		window.draw(p.sprite);//рисуем спрайт объекта p класса player
		
		window.display();
	

	return 0;
}