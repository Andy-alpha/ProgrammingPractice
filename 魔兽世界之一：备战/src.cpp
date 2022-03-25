#include<cstdio>
#include<string>
#include<cstring>
using namespace std;
/*
string Warrior::names[WARRIOR_NUM] = { "dragon","ninja","iceman","lion","wolf" };
红方司令部按照 iceman、lion、wolf、ninja、dragon 的顺序制造武士。
蓝方司令部按照 lion、dragon、ninja、iceman、wolf 的顺序制造武士。
*/

class Camp;
class Warrior
{
    private:
        Camp *pc;
        int wa_id;//武士的种类编号 0 dragon 1 ninja 2 iceman 3 lion 4 wolf
        int no;
    public:
        static string name[5];
        static int health[5];
        Warrior(Camp *p,int nono,int wa_id_);
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
        Warrior *pWarrior[1000];
    public:
        friend class Warrior;
        static int Seq[2][5]; //武士的制作顺序序列
        void Init(int coco,int lv);
        ~Camp();
        bool Produce(int time);
        string Get_Color();
};
string Warrior::name[5]={ "dragon","ninja","iceman","lion","wolf" };
int Warrior::health[5];
int Camp::Seq[2][5] = { { 2,3,4,1,0 },{3,0,1,2,4} }; //两个司令部武士的制作顺序序列

Warrior::Warrior( Camp *pCamp,int no_,int kindNo_) 
{
    no=no_;
    wa_id= kindNo_;
    pc=pCamp;
}
void Warrior::Print(int time)
{
    string color=pc->Get_Color();
    printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n"    ,
        time, color.c_str(), name[wa_id].c_str(), no, health[wa_id],
        pc->warrior[wa_id],name[wa_id].c_str(),color.c_str());
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
    for(int i=0;i<wa_num;i++)
        delete pWarrior[i];
}
bool Camp::Produce(int time)
{
    if(sign) return 0;
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
        return 0;
    }
    //制作士兵：
    HP-=Warrior::health[kind];
    wa_id=(wa_id+1)%5;
    pWarrior[wa_num]=new Warrior(this,wa_num+1,kind);
    warrior[kind]++;
    pWarrior[wa_num]->Print(time);
    wa_num++;
    return 1;
}
string Camp::Get_Color()
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
        printf("Case:%d\n",i);
        scanf("%d",&m);
        for(int i=0;i<5;i++)
            scanf("%d",&Warrior::health[i]);
        Red.Init(0,m);
        Blue.Init(1,m);
        int time=0;
        while(1){
            bool sign=Red.Produce(time);
            bool flag=Blue.Produce(time);
            if(!sign&&!flag) break;
            time++;
        }
    }
    return 0;
}
