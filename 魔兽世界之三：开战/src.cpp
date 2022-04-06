#include<iostream>
#include<cstring>
#include<iomanip>
#include<cstdio>
#include<typeinfo>
using namespace std;
 
int soldier_HP[5];
char soldier_name[5][20] = {"iceman", "lion", "wolf", "ninja", "dragon"};
char weapon_name[3][20] = {"sword", "bomb", "arrow"};
int soldier_force[5];
int city_number, time_limited;
 
//different soldier class
//model class
class csoldier
{
public:
    int HP, force, locate, id;
	int weapon[4];
	int weapen_amount;
	csoldier() { memset(weapon, 0, sizeof(weapon)); }
    virtual ~csoldier(){}
 
//第n次交手应该使用的武器，n已经考虑过通过循环处理
	int getweapon(int &n)
	{
	    bool flag = false;
	    if (weapen_amount <= n) flag = true;
		if (n <= weapon[0])
		{
			++n;
			if(flag) n = 1;
			return 0;
		}
		else if (n <= weapon[0] + weapon[1])
		{
			weapon[1]--;
			weapen_amount--;
			if(flag) n = 1;
			return 1;
		}
		else if (n <= weapon[3] + weapon[0] + weapon[1])
		{
			weapon[3]--;
			weapen_amount--;
			if(flag) n = 1;
			return 2;
		}
		else if (n <= weapen_amount)
		{
 
			weapon[2]--;
			weapon[3]++;
			++n;
			if(flag) n = 1;
			return 2;
		}
		cout << "wrong!" << endl;
	}
	virtual void out_name() = 0;
    friend class battlefield;
};
 
class dragon:public csoldier
{
private:
    friend class battlefield;
    //constructor
    dragon(int n, int color):csoldier()
    {
        weapon[n % 3] = 1;
		weapen_amount = 1;
		force = soldier_force[4];
		locate = ((color == 0)? 0 : city_number + 1);
		HP = soldier_HP[4];
		id = n;
    }
    //destructor
    ~dragon(){}
	virtual void out_name()
	{
		cout << "dragon " << id;
	}
};
 
class ninja:public csoldier
{
private:
    friend class battlefield;
	ninja(int n, int color) :csoldier()
	{
		weapon[n % 3] = 1;
		weapon[(n + 1) % 3] = 1;
		weapen_amount = 2;
		force = soldier_force[3];
		locate = ((color == 0) ? 0 : city_number + 1);
		HP = soldier_HP[3];
		id = n;
	}
    ~ninja(){}
	virtual void out_name()
	{
		cout << "ninja " << id;
	}
};
 
class iceman:public csoldier
{
private:
    friend class battlefield;
	iceman(int n, int color) :csoldier()
	{
		weapon[(n % 3)] = 1;
		weapen_amount = 1;
		force = soldier_force[0];
		locate = ((color == 0) ? 0 : city_number + 1);
		HP = soldier_HP[0];
		id = n;
	}
	virtual void out_name()
	{
		cout << "iceman " << id;
	}
};
 
class lion:public csoldier
{
private:
    friend class battlefield;
    int loyalty;
	lion(int n, int color, int hp) :csoldier(), loyalty(hp)
	{
		weapon[n % 3] = 1;
		weapen_amount = 1;
		force = soldier_force[1];
		locate = ((color == 0) ? 0 : city_number + 1);
		HP = soldier_HP[1];
		id = n;
		cout << "Its loyalty is " << loyalty << endl;
	}
	virtual void out_name()
	{
		cout << "lion " << id;
	}
public:
	static int K;
};
int lion::K = 0;
 
class wolf:public csoldier
{
	wolf(int n, int color) :csoldier()
	{
		weapen_amount = 0;
		force = soldier_force[2];
		locate = ((color == 0) ? 0 : city_number + 1);
		HP = soldier_HP[2];
		id = n;
	}
	virtual void out_name()
	{
		cout << "wolf " << id;
	}
private:
    friend class battlefield;
};
 
class battlefield
{
private:
    int hour, minute, soldier_total[2], cur[2];
    int HP[2];
    char name[2][20];
    bool produce_end[2];//if production is carrying on, it is false, if else it is true
    csoldier ***head;
    int soldier[5];//in the order of iceman¡¢lion¡¢wolf¡¢ninja¡¢dragon
public:
	int arr[2][5] = { {0, 1, 2, 3, 4} ,{1, 4, 3, 0, 2} };
    //int arr[5] = {1, 4, 3, 0, 2};
	battlefield(int n) :HP{ n, n}, hour(0), minute(0), produce_end{ true,true }
    {
		soldier_total[0] = soldier_total[1] = 0;
		cur[0] = cur[1] = -1;
		strcpy(name[0], "red");
		strcpy(name[1], "blue");
        memset(soldier, 0, sizeof(soldier));
		head = new csoldier** [city_number + 2];
		for (int i = 0; i < city_number + 2; ++i)
		{
			head[i] = new csoldier*[4];
			memset(head[i], NULL, sizeof(csoldier*) * 4);
		}
    }
	//输出当前时间
    void out_time()
    {
        cout << setfill('0') << setw(3) << hour << ':' << setfill('0') << setw(2) << minute;
		return;
    }
 
	//base produce soldiers. if base produce soldier sucessfully, return true;if else return false;
    bool produce(int color)
    {
        if(produce_end[color] == false)
            return false;
        cur[color] = (cur[color] + 1) % 5;
        int t = arr[color][cur[color]];
        if(HP[color] >= soldier_HP[t])
        {
            HP[color] -= soldier_HP[t];
            soldier[t]++;
            soldier_total[color]++;
			out_time();
			cout << ' ' << name[color] << ' ' << soldier_name[t] << ' ' << soldier_total[color] << " born"<< endl;
			int pos = ((color == 0) ? 0 : city_number + 1);
            switch(t)
            {
                case 0: head[pos][color] = new iceman(soldier_total[color], color);break;
                case 1: head[pos][color] = new lion(soldier_total[color], color, HP[color]);break;
                case 2: head[pos][color] = new wolf(soldier_total[color], color);break;
                case 3: head[pos][color] = new ninja(soldier_total[color], color);break;
                case 4: head[pos][color] = new dragon(soldier_total[color], color);break;
            }
            return true;
        }
        else
        {
			produce_end[color] = false;
			return false;
        }
    }
	//renew location
	void clear()
	{
		for (int i = 0; i <= city_number + 1; ++i)
		{
			head[i][0] = head[i][2];
			head[i][1] = head[i][3];
			head[i][2] = head[i][3] = NULL;
		}
		return;
	}
 
	//soldiers start moving
	bool march()
	{
		bool flag = true;
 
		if (head[1][1] != NULL)
		{
			if (typeid(*head[1][1]) == typeid(iceman))
				head[1][1]->HP -= head[1][1]->HP / 10;
			out_time();
			cout << " blue "; head[1][1]->out_name();
			printf(" reached red headquarter with %d elements and force %d\n", head[1][1]->HP, head[1][1]->force);
			head[0][3] = head[1][1];
			out_time();
			cout << " red headquarter was taken" << endl;
			flag = false;
		}
		for (int i = 1; i <= city_number; ++i)
		{
			if (head[i - 1][0] != NULL)
			{
				if (typeid(*head[i - 1][0]) == typeid(iceman))
					head[i - 1][0]->HP -= head[i - 1][0]->HP / 10;
				out_time();
				cout << " red "; head[i - 1][0]->out_name();
				printf(" marched to city %d with %d elements and force %d\n",i, head[i-1][0]->HP, head[i-1][0]->force);
				head[i][2] = head[i - 1][0];
			}
			if (head[i + 1][1] != NULL)
			{
				if (typeid(*head[i + 1][1]) == typeid(iceman))
					head[i + 1][1]->HP -= head[i + 1][1]->HP / 10;
				out_time();
				cout << " blue "; head[i + 1][1]->out_name();
				printf(" marched to city %d with %d elements and force %d\n", i, head[i + 1][1]->HP, head[i + 1][1]->force);
				head[i][3] = head[i + 1][1];
			}
		}
		if (head[city_number][0] != NULL)
		{
			if (typeid(*head[city_number][0]) == typeid(iceman))
				head[city_number][0]->HP -= head[city_number][0]->HP / 10;
			out_time();
			cout << " red "; head[city_number][0]->out_name();
			printf(" reached blue headquarter with %d elements and force %d\n", head[city_number][0]->HP, head[city_number][0]->force);
			head[city_number + 1][2] = head[city_number][0];
			out_time();
			cout << " blue headquarter was taken" << endl;
			flag = false;
		}
		clear();
		return flag;
	}
 
	//judge whether lion run away
	void run_away()
	{
		for (int i = 0; i <= city_number + 1; ++i)
		{
			for (int t = 0; t < 2; ++t)
			{
				if (head[i][t] != NULL && typeid(*head[i][t]) == typeid(lion))
				{
					auto p = (lion *)head[i][t];
					if (p->loyalty <= 0)
					{
						out_time();
						cout << ' ' << name[t];
						cout << " lion " << head[i][t]->id << " ran away" << endl;
						delete head[i][t];
						head[i][t] = NULL;
						continue;
					}
					p->loyalty -= lion::K;
				}
			}
 
		}
		return;
	}
 
	//winner snatch loser's weapon
	void snatch_weapon(csoldier *p, csoldier *q)
	{
		for (int i = 0; i < 4; ++i)
		{
			while (q->weapon[i] > 0 && p->weapen_amount < 10)
			{
				p->weapon[i]++;
				q->weapon[i]--;
				p->weapen_amount++;
			}
		}
		return;
	}
    void out_weapen(int t)
    {
        switch(t)
        {
            case 0: cout << weapon_name[0];break;
            case 1: cout << weapon_name[1];break;
            case 2: cout << weapon_name[2];break;
            case 3: cout << weapon_name[2];break;
        }
        return;
    }
	//wolf snatch enemy's weapon
	void w_snatch_weapon(csoldier *p, csoldier *q)
	{
		//bool flag = true;
 
		int t = 0, s = 0;
		while (q->weapon[t] == 0)++t;
		if (10 - p->weapen_amount <= q->weapon[t])
		{
			p->weapon[t] += 10 - p->weapen_amount;
			s += 10 - p->weapen_amount;
			p->weapen_amount = 10;
			q->weapen_amount -= s;
			q->weapon[t] -= s;
			return;
		}
		else
		{
			p->weapon[t] += q->weapon[t];
			s += q->weapon[t];
			p->weapen_amount += q->weapon[t];
			q->weapon[t] -= s;
			q->weapen_amount -= s;
			if (t == 2 && q->weapon[3] > 0)
			{
				t = 3;
				if (10 - p->weapen_amount <= q->weapon[t])
				{
					p->weapon[t] += 10 - p->weapen_amount;
					s += 10 - p->weapen_amount;
					p->weapon[t] -= 10 - p->weapen_amount;
					p->weapen_amount -= 10 - p->weapen_amount;
					p->weapen_amount = 10;
				}
				else
				{
					p->weapon[t] += q->weapon[t];
					s += q->weapon[t];
					q->weapen_amount -= q->weapon[t];
					p->weapen_amount += q->weapon[t];
					q->weapon[t] = 0;
				}
			}
			cout << s << ' ';
            out_weapen(t);
			return;
		}
	}
 
	//wolf snatch enemy's weapon
	void snatch()
	{
		for (int i = 1; i <= city_number; ++i)
		{
			csoldier *p[2] = { head[i][0], head[i][1] };
			if (p[0] != NULL && p[1] != NULL)
			{
 
				if (typeid(*p[0]) != typeid(wolf) && typeid(*p[1]) != typeid(wolf))
					continue;
				else if (typeid(*p[0]) != typeid(wolf))
				{
				    if(p[0]->weapen_amount == 0 || p[1]->weapen_amount == 10)
                        continue;
					out_time(); cout << " blue ";p[1]->out_name();
					cout << " took ";
					w_snatch_weapon(p[1], p[0]);
					cout << " from red ";
					p[0]->out_name();cout << " in city " << i << endl;
				}
				else if (typeid(*p[1]) != typeid(wolf))
				{
				    if(p[1]->weapen_amount == 0 || p[0]->weapen_amount == 10)
                        continue;
					out_time(); cout << " red ";p[0]->out_name();
					cout << " took ";
					w_snatch_weapon(p[0], p[1]);
					cout << " from blue ";
					p[1]->out_name();cout << " in city " << i << endl;
				}
				else
					continue;
 
			}
		}
	}
 
	//fight
	void fight()
	{
		for (int i = 1; i <= city_number; ++i)
		{
			if (head[i][0] == NULL || head[i][1] == NULL)
				continue;
			int r = i % 2, L = r ^ 1, n[2] = {1, 1}, w, flag = 5, check[4] = {0}, cnt = 0;
			while (true)
			{
				//判断是否局势改变，若不改变则为平局
				r = r ^ 1;
				L = L ^ 1;
				if (cnt == 0)
				{
					if (check[0] == head[i][0]->HP && check[1] == head[i][1]->HP && check[2] == head[i][0]->weapen_amount && check[3] == head[i][1]->weapen_amount)
					{
						flag = 0;//all alive
						break;
					}
					check[0] = head[i][0]->HP;
					check[1] = head[i][1]->HP;
					check[2] = head[i][0]->weapen_amount;
					check[3] = head[i][1]->weapen_amount;
					cnt = 10;
				}
				cnt--;
 
				if (head[i][L]->weapen_amount == 0 && head[i][r]->weapen_amount == 0)
				{
					flag = 0;//alive
					break;
				}
				if(head[i][r]->weapen_amount == 0)
                    continue;
				w = head[i][r]->getweapon(n[r]);
// if(head[i][1]->id == 10)
// cout << r << ' ' << "use weapen " << w << " in city " << i <<n[r] << endl;
				int h;
				switch (w)
				{
				case 0:
					head[i][L]->HP -= head[i][r]->force / 5; break;
				case 1:
					h = head[i][r]->force * 2 / 5;
					head[i][L]->HP -= h;
					if(typeid(*head[i][r]) != typeid(ninja))
                        head[i][r]->HP -= h / 2;
					break;
				case 2:
					head[i][L]->HP -= head[i][r]->force * 3 / 10; break;
				default:
					break;
				}
				if (head[i][L]->HP <= 0 && head[i][r]->HP <= 0)
				{
					flag = 3;//all died
					break;
				}
				else if (head[i][L]->HP <= 0)
				{
					if (L == 0)
					{
						flag = 2; //blue win
						break;
					}
					else
					{
						flag = 1; //red win
						break;
					}
				}
				else if (head[i][r]->HP <= 0)
				{
					if (r == 0)
					{
						flag = 2; //blue win
						break;
					}
					else
					{
						flag = 1; //red win
						break;
					}
				}
			}
			out_time();
			switch (flag)
			{
			case 0:
				cout << " both red ";
				head[i][0]->out_name();
				cout << " and blue ";
				head[i][1]->out_name();
				cout << " were alive in city " << i << endl;
				break;
			case 3:
				cout << " both red ";
				head[i][0]->out_name();
				cout << " and blue ";
				head[i][1]->out_name();
				cout << " died in city " << i << endl;
				delete head[i][0];
				delete head[i][1];
				head[i][0] = NULL;
				head[i][1] = NULL;
				break;
			case 1:
				cout << " red ";
				head[i][0]->out_name();
				cout << " killed blue ";
				head[i][1]->out_name();
				printf(" in city %d remaining %d elements\n", i, head[i][0]->HP);
				snatch_weapon(head[i][0], head[i][1]);
				delete head[i][1];
				head[i][1] = NULL;
				break;
			case 2:
				cout << " blue ";
				head[i][1]->out_name();
				cout << " killed red ";
				head[i][0]->out_name();
				printf(" in city %d remaining %d elements\n", i, head[i][1]->HP);
				snatch_weapon(head[i][1], head[i][0]);
				delete head[i][0];
				head[i][0] = NULL;
				break;
			default:
				break;
			}
			for (int t = 0; t < 2; ++t)
			{
				if (head[i][t] != NULL && typeid(*head[i][t]) == typeid(dragon))
				{
					out_time(); cout << ' ' << name[t] << ' ';
					head[i][t]->out_name();
					cout << " yelled in city " << i << endl;
				}
			}
 
		}
 
	}
 
	//base report condition
	void base_report()
	{
		out_time();
		printf(" %d elements in red headquarter\n", HP[0]);
		out_time();
		printf(" %d elements in blue headquarter\n", HP[1]);
		return;
	}
 
	//soldier report
	void soldier_report()
	{
		for (int i = 0; i < city_number + 1; ++i)
		{
			for(int t = 0; t < 2; ++t)
				if (head[i][t] != NULL)
				{
					out_time();
					cout << ' ' << name[t] << ' ';
					head[i][t]->out_name();
					cout << " has " << head[i][t]->weapon[0] << " sword " << head[i][t]->weapon[1] << " bomb "
						<< head[i][t]->weapon[2] + head[i][t]->weapon[3] << " arrow and " << head[i][t]->HP << " elements" << endl;
				}
		}
		return;
	}
 
	//check whether time is legal
	bool check_time()
	{
		if (hour * 60 + minute > time_limited)
			return false;
		else
			return true;
	}
 
	bool run()
	{
		minute = 0;
		if (!check_time()) return false;
 
		produce(0);
		produce(1);
 
		minute = 5;
		if (!check_time()) return false;
 
		run_away();
 
		minute = 10;
		if (!check_time()) return false;
 
		if(!march())
			return false;
 
		minute = 35;
		if (!check_time()) return false;
 
		snatch();
 
		minute = 40;
		if (!check_time()) return false;
 
		fight();
 
		minute = 50;
		if (!check_time()) return false;
		base_report();
 
		minute = 55;
		if (!check_time()) return false;
		soldier_report();
 
		hour++;
		return true;
	}
};
int main()
{
    int n, hp, cnt = 0;
    cin >> n;
    while(n--)
    {
		cout << "Case " << ++cnt <<':'<< endl;
                cin >> hp >> city_number >> lion::K >> time_limited;
                battlefield Battle(hp);
                cin >> soldier_HP[4] >> soldier_HP[3] >> soldier_HP[0] >> soldier_HP[1] >> soldier_HP[2];
		cin >> soldier_force[4] >> soldier_force[3] >> soldier_force[0] >> soldier_force[1] >> soldier_force[2];
		while (Battle.run());
 
    }
    return 0;
}
