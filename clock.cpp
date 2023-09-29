#include <easyx.h>
#include <vector>
#include <conio.h>
#include <stdio.h>
#include <time.h>
using std::vector;

class tem_time//测试用时间
{
public:
	unsigned int hour = 0;
	unsigned int miniter = 0;
	void add()
	{
		this->miniter++;
		if (60 == this->miniter)
		{
			this->hour++;
			this->miniter = 0;
		}
		if (12 == this->hour)
		{
			this->hour = 0;
		}
	}
}tem_tm;
enum choose { r, g, b ,w};//控制颜色（详见color::change_color()）
class COLOR
{
private:
	int red = 0;
	int green = 0;
	int blue = 0;
	
public:
	
	COLOR()
	{

	}
	COLOR(int a, int b, int c)
	{
		this->red = a;
		this->green = b;
		this->blue = c;
	}
	bool change_color(choose which)
	{
		switch (which)
		{
		case r:
			this->green = 0;
			this->blue = 0;
			this->red = 255;
			break;
		case g:
			this->red = 0;
			this->blue = 0;
			this->green = 255;
			break;
		case b:
			this->green = 0;
			this->red = 0;
			this->blue = 255;
			break;
		case w:
			this->blue = 0;
			this->green = 0;
			this->red = 0;
			break;
		}
		//return true;
		return (this->blue >= 255 || this->green >= 255 || this->red >= 255);//没有用的
	}
	COLORREF get_rgb()
	{
		return RGB(this->red, this->green, this->blue);
	}
};
struct square//单个方块对象
{
private:
	POINT place = { 0,0 };
	LONG length = 100;
	COLOR color;
public:
	square()
	{

	}
	square(POINT a,LONG b,COLOR c)
	{
		this->place = a;
		this->length = b;
		this->color = c;
	}
	void set_color(choose which)
	{
		this->color.change_color(which);
	}
	void draw()
	{
		setfillcolor(this->color.get_rgb());
		fillrectangle(this->place.x, this->place.y, this->place.x + this->length, this->place.y + this->length);
	}


};
class SCREEN//时钟屏幕
{
private:
	vector<square> all_square;
public:
	SCREEN()
	{
		this->all_square.push_back({ {200,0},100,{0,0,0} });
		this->all_square.push_back({ {200,100},100,{0,0,0} });
		this->all_square.push_back({ {0,0},200,{0,0,0} });
		this->all_square.push_back({ {0,200},300,{0,0,0} });
		this->all_square.push_back({ {300,0},500,{0,0,0} });
	}
	void calculation()//计算每个方块应有的颜色（最好改一下send_seed()）或许有每次颜色结果不一样的情况，但是没体现出来。
	{
		int hour = tem_tm.hour;
		int min = tem_tm.miniter / 5;
		int judge_hour[5] = { 0,0,0,0,0 };
		int judge_min[5] = { 0,0,0,0,0 };
		auto send_seed = []()
		{
			return (rand() / 2);
		};
		auto change_judge = [send_seed](int how_many,int judge[5])
		{
			switch (how_many)
			{
			case 0:
				break;
			case 1:
				(send_seed()) ? judge[0] = 1 : judge[1] = 1;
				break;
			case 2:
				(send_seed()) ? judge[0] = judge[1] = 1 : judge[2] = 1;
				break;
			case 3:
				if (send_seed())
				{
					(send_seed()) ? judge[0] = 1 : judge[1] = 1;
					judge[2] = 1;
				}
				else
				{
					judge[3] = 1;
				}
				break;
			case 4:
				if (send_seed())
				{
					judge[3] = 1;
					(send_seed()) ? judge[0] = 1 : judge[1] = 1;
				}
				else
				{
					judge[0] = judge[1] = judge[2] = 1;
				}
				break;
			case 5:
				if (send_seed())
				{
					judge[3] = 1;
					(send_seed()) ? judge[0] = judge[1] = 1 : judge[2] = 1;
				}
				else
				{
					judge[4] = 1;
				}
				break;
			case 6:
				if (send_seed())
				{
					judge[2] = judge[3] = 1;
				}
				else
				{
					judge[4] = 1;
				}
				(send_seed()) ? judge[0] = 1 : judge[1] = 1;
				break;
			case 7:
				if (send_seed())
				{
					judge[3] = 1;
					judge[0] = judge[1] = judge[2] = 1;
				}
				else
				{
					judge[4] = 1;
					(send_seed()) ? judge[0] = judge[1] = 1 : judge[2] = 1;
				}
				break;
			case 8:
				judge[4] = 1;
				if (send_seed())
				{
					judge[2] = 1;
					(send_seed()) ? judge[0] = 1 : judge[1] = 1;
				}
				else
				{
					judge[3] = 1;
				}
				break;
			case 9:
				judge[4] = 1;
				if (send_seed())
				{
					judge[3] = 1;
					(send_seed()) ? judge[0] = 1 : judge[1] = 1;
				}
				else
				{
					judge[0] = judge[1] = judge[2] = 1;
				}
				break;
			case 10:
				judge[3] = judge[4] = 1;
				(send_seed()) ? judge[0] = judge[1] = 1 : judge[2] = 1;
				break;
			case 11:
				judge[2] = judge[3] = judge[4] = 1;
				(send_seed()) ? judge[0] = 1 : judge[1] = 1;
				break;
			case 12:
				judge[0] = judge[1] = judge[2] = judge[3] = judge[4] = 1;
				break;
			default:

				break;
			}
		};
		change_judge(hour, judge_hour);
		change_judge(min, judge_min);
		for (int i = 0; i < 5; ++i)
		{
			if (judge_hour[i] && judge_min[i])
			{
				this->all_square[i].set_color(b);
			}
			else if (judge_hour[i])
			{
				this->all_square[i].set_color(g);
			}
			else if (judge_min[i])
			{
				this->all_square[i].set_color(r);
			}
			else
			{
				this->all_square[i].set_color(w);
			}
		}
	}

	void draw()
	{
		this->calculation();
		for (auto& square : this->all_square)
		{
			square.draw();
		}
		
	}
}screen;
int main()
{
	initgraph(100 + 200 + 500, 200 + 300);
	
	while (1)
	{
		srand(time(0));
		screen.draw();
		tem_tm.add();
		printf("%d:%d\n", tem_tm.hour, tem_tm.miniter);
		_getch();
	}
	return 0;
}