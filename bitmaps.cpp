#include <cassert>
#include <fstream>
#include "bitmaps.hpp"

using namespace std;
using namespace cnv;

std::vector<cnv::Image*> Bitmaps::find (Id id) const // возвращает пак картинок по id
{
	/* Завершает программу с ошибкой, если запрошенного пака не существует.
	   Используется только в самом классе, в случаях, когда надо безопасно
	   обратиться к существующему паку.
	*/

	assert (bitmaps_.find(id) != bitmaps_.end() && "Bitmaps: ID not found");
	return bitmaps_.find(id)->second;
}

int Bitmaps::size (Id id) const // размер конкретного пака картинок по id
{
	return find(id).size();
}

void Bitmaps::draw (Id id, int frame, int x, int y, int scale) const
{
	color (1, 1, 1);
	find(id).at(frame)->draw (x, y, scale);
}

void Bitmaps::load()
{
	ifstream imglist ("img/img.txt");
	assert (imglist.good() && "Bitmaps: 'img.txt' not found");
	string img;
	int id;
	vector<Image*> pack;

	/*
	   Список картинок построен по следующему принципу:

	   # BEGIN COMMENT(string)
	   ID(int)
	   path/to/file
	   # END COMMENT

	   Комментарии после BEGIN и END обязательны, но нести смысловую нагрузку
	   не обязаны, как и совпадать. В приложенном файле они соответствуют имени
	   константы, указанной в строке ID.
	*/
	  

	while (imglist >> img)
	{
		if (img == "#")
		{
			imglist >> img;
			if (img == "END_OF_FILE") break;

			else if (img == "BEGIN")
			{
				imglist >> img; // выкидывает коммент
				imglist >> id; // и считывает id
			}

			else if (img == "END")
			{
				imglist >> img; // выкидывает коммент
				bitmaps_[id] = pack; //копирует временный пак в map	
				pack.clear();
			}
		}

		else pack.push_back (new Image (img)); // загружает картинку во временный пак
	}
}

/*** namespace bitmaps ***/

void bitmaps::load ()
{
	Bitmaps::inst().load();
}

void bitmaps::draw (Id id, int frame, int x, int y, int scale)
{
	Bitmaps::inst().draw (id, frame, x, y, scale);
}

int bitmaps::size (Id id)
{
	return Bitmaps::inst().size (id);
}
