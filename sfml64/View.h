#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//�������� sfml ������ "���", ������� � �������� �������
void getplayercoordinateforview(float x, float y) { //������� ��� ���������� ��������� ������


	view.setCenter(x, y); //������ �� �������, ��������� ��� ���������� ������. +100 - �������� ������ �� ���� ������. �����������������
	float tempX = x; float tempY = y;//��������� ����� ������ � ��������� ��, ����� ������ ����
	if ( x> 960)   tempX = 960;
	if (x < 320) tempX = 320;//������� �� ���� ����� �������
	if (y < 240) tempY = 240;//������� �������
	if (y > 560) tempY = 560;//������ �������	

	view.setCenter(tempX, tempY); //������ �� �������, ��������� ��� ����������. 

}
void viewmap(float time) { //������� ��� ����������� ������ �� �����. ��������� ����� sfml
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		view.move(0.1 * time, 0);//�������� ����� ������ (�� ����, ����� �� ������� ����� - �� ���� �����)
	}
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		view.move(0, 0.1 * time);//�������� ����� ���� (�� ����, ����� �� ������� ����� - �� ���� �����)
	}
	if (Keyboard::isKeyPressed(Keyboard::A)) {
		view.move(-0.1 * time, 0);//�������� ����� ����� (�� ����, ����� �� ������� ����� - �� ���� �����)
	}
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		view.move(0, -0.1 * time);//�������� ����� ������ (�� ����, ����� �� ������� ����� - �� ���� �����)
	}
}
