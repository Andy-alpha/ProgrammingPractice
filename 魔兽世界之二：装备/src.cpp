#include <iostream>
#include <cstring>
#include <algorithm>
#include <cmath>
using namespace std;
/*
string Warrior::names[WARRIOR_NUM] = { "dragon","ninja","iceman","lion","wolf" };
红方司令部按照 iceman、lion、wolf、ninja、dragon 的顺序制造武士。
蓝方司令部按照 lion、dragon、ninja、iceman、wolf 的顺序制造武士。
*/

class Camp;
class Attribute;
class Warrior
{
private:
    Camp *pc;
    int wa_id;//武士的种类编号 0 dragon 1 ninja 2 iceman 3 lion 4 wolf
    int no;

public:
    static const char name[5][7];
    static int health[5];
    Warrior(Camp *p,int no_,int wa_id_);
    void Print(int time);
};
class Camp
{
private:
    int HP;
    bool sign;
    int wa_num;
    int color;
    int wa_id;//当前要制造的武士是制造序列中的第几个
    int warrior[5];//存放每种武士的数量
    //double morale=0;//士气
    //int loyalty=0;//忠诚度
    //int weapon_id=0;//主手武器
    //int weapon2_id=0;//ninja的副手武器
    //int damage=0;//武器伤害
    Warrior *pWarrior[1000];
    Attribute * pAttribute[1000];
public:
    friend class Warrior;
    friend class Attribute;
    static int Seq[2][5]; //武士的制作顺序序列
    void Init(int coco,int health);
    ~Camp();
    bool Produce(int time);
    string Get_Color() const;
};
class Attribute{
private:
    int wa_num{0};
    int wa_id{0};//当前要制造的武士是制造序列中的第几个
    //int warrior[5];//存放每种武士的数量
    //Warrior *pWarrior[1000];

    double morale=0;//士气
    int loyalty=0;//忠诚度
    int weapon_id=0;//主手武器
    int weapon2_id=0;//ninja的副手武器
    int damage=0;//武器伤害
public:
    static const char weapon[3][6];
    friend class Camp;
    friend class Warrior;
    Attribute(Camp* pCamp, const int& wa_num, const int& wa_id){
        Attribute::wa_num=wa_num;
        Attribute::wa_id=wa_id;
        //编号为n的dragon降生时即获得编号为 n%3 的武器。dragon还有“士气”这个属性，是个浮点数，其值为它降生后其司令部剩余生命元的数量除以造dragon所需的生命元数量。
        if (wa_id==0){
            weapon_id=wa_num%3;
            morale = double(pCamp->HP) / Warrior::health[0];
        }
        //ninja可以拥有两件武器。编号为n的ninja降生时即获得编号为 n%3 和 (n+1)%3的武器。
        else if (wa_id==1){
            weapon_id=wa_num%3;
            weapon2_id=(wa_num+1)%3;
        }
        //iceman有一件武器。编号为n的iceman降生时即获得编号为 n%3 的武器。
        else if (wa_id==2){
            weapon_id=wa_num%3;
        }
        //lion 有“忠诚度”这个属性，其值等于它降生后其司令部剩余生命元的数目。
        else if (wa_id==3){
            loyalty=pCamp->HP;
        }
        //wolf无特点。
    }
};


const char Warrior::name[5][7]={ "dragon","ninja","iceman","lion","wolf" };
const char Attribute::weapon[3][6]={"sword","bomb","arrow"};
int Warrior::health[5];
int Camp::Seq[2][5] = { { 2,3,4,1,0 },{3,0,1,2,4} }; //两个司令部武士的制作顺序序列

Warrior::Warrior( Camp *pCamp,int no_,int kindNo_) : no(no_),wa_id(kindNo_),pc(pCamp){}
void Warrior::Print(int time)
{
    string color=pc->Get_Color();
    printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n"    ,
           time, color.c_str(), name[wa_id], no, health[wa_id],
           pc->warrior[wa_id],name[wa_id],color.c_str());
    if (wa_id==0) printf("It has a %s,and it's morale is %.2lf\n",Attribute::weapon[pc->pAttribute[no-1]->weapon_id],pc->pAttribute[no-1]->morale);
    else if (wa_id==1) printf("It has a %s and a %s\n",Attribute::weapon[pc->pAttribute[no-1]->weapon_id],Attribute::weapon[pc->pAttribute[no-1]->weapon2_id]);
    else if (wa_id==2) printf("It has a %s\n",Attribute::weapon[pc->pAttribute[no-1]->weapon_id]);
    else if (wa_id==3) printf("It's loyalty is %d\n",pc->pAttribute[no-1]->loyalty);

}
void Camp::Init(int coco,int health)
{
    color=coco;
    HP=health;
    wa_num=0;
    sign=false;
    wa_id=0;
    memset(warrior,0,sizeof(warrior));
}
Camp::~Camp()
{
    for(int i=0;i<wa_num;i++){
        delete pWarrior[i];
        delete pAttribute[i];
    }
}
bool Camp::Produce(int time)
{
    if(sign) return false;
    int cnt=0;
    while(Warrior::health[Seq[color][wa_id]]>HP&&cnt<5) {
        wa_id=(wa_id+1)%5;
        cnt++;
    }
    int kind=Seq[color][wa_id];
    if( Warrior::health[kind]>HP){
        sign=true;
        if(color) printf("%03d blue headquarter stops making warriors\n",time);
        else printf("%03d red headquarter stops making warriors\n",time);
        return false;
    }
    //制作士兵：
    HP-=Warrior::health[kind];
    wa_id=(wa_id+1)%5;
    pWarrior[wa_num]=new Warrior(this,wa_num+1,kind);
    pAttribute[wa_num]=new Attribute(this,wa_num+1,kind);
    warrior[kind]++;
    pWarrior[wa_num++]->Print(time);
    return true;
}
string Camp::Get_Color() const
{
    if(color) return "blue";
    else return "red";
}

int main()
{
    int T,m;
    Camp Red,Blue;
    scanf("%d",&T);
    for(int i=1;i<=T;i++){
        scanf("%d",&m);
        for(int i=0;i<5;i++)
            scanf("%d",&Warrior::health[i]);
        printf("Case:%d\n",i);
        Red.Init(0,m);
        Blue.Init(1,m);
        int time=0;
        while (true){
            bool sign=Red.Produce(time);
            bool flag=Blue.Produce(time);
            if(!sign&&!flag) break;
            time++;
        }
    }
    return 0;
}
