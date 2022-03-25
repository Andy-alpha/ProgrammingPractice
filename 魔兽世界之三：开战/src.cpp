#include <iostream>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;
/*
string Warrior::names[WARRIOR_NUM] = { "dragon","ninja","iceman","lion","wolf" };
红方司令部按照 iceman、lion、wolf、ninja、dragon 的顺序制造武士。
蓝方司令部按照 lion、dragon、ninja、iceman、wolf 的顺序制造武士。
*/
int T,m,n,r,k,t;
class Camp;
class Attribute;
class City;
City *pCity[1000];
class Warrior
{
private:
    Camp *pc;
    int wa_id;//武士的种类编号 0 dragon 1 ninja 2 iceman 3 lion 4 wolf
    int no;//武士的制造编号
    int city_id;//武士所处的城市编号
    int win_times{0};//打胜仗的次数
    int win{0};//最近一次是否打赢
    bool alive = true;
public:
    static const char name[5][7];
    static int health[5];
    static int attack[5];
    friend class City;
    Warrior(Camp *p,int no_,int wa_id_);
    void Print(int hour);
    void Run(const int& hour);
    void Move(const int& hour);
    //void Grab(const int& hour);
    static int Attack(Warrior* me,Warrior *enemy, const int& hour);
    void Yell(const int &hour) const;
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
    friend class City;
    static int Seq[2][5]; //武士的制作顺序序列
    Camp() = default;
    Camp(int color,int health) : color(color),HP(health){
        wa_num=0;
        sign=false;
        wa_id=0;
        memset(warrior,0,sizeof(warrior));
    }
    //void Init(int coco,int health);
    ~Camp();
    bool Produce(const int& hour);
    void Run_at5(const int& hour);
    void Move(const int& hour);
    //void Grab(const int& hour);
    //void Attack(const int &hour);

    string Get_Color() const;
    int city() const;
};
class Attribute{
private:
    int wa_num{0};
    int wa_id{0};//当前要制造的武士是制造序列中的第几个
    //int warrior[5];//存放每种武士的数量
    //Warrior *pWarrior[1000];

    double morale=0;//士气
    int loyalty=0;//忠诚度
    int weapon_id[3]{0};//三种武器
    int elements;//生命值
    int damage=0;//武器伤害
    int damage_of_sword[11]{0};
    int to_use;//接下来要用的武器
    int to_use_num{1};//以防要用的武器重号，加以区分
    std::vector <int> durable;//储存箭的耐久
    int total{0};
public:
    static const char weapon[3][6];
    friend class Camp;
    friend class Warrior;
    friend class City;
    Attribute(Camp* pCamp, const int& wa_num, const int& wa_id){
        Attribute::wa_num=wa_num;
        Attribute::wa_id=wa_id;
        elements = Warrior::health[wa_id];
        damage = Warrior::attack[wa_id];
        damage_of_sword[0] = damage * 2 / 10;
        //编号为n的dragon降生时即获得编号为n%3 的武器。dragon在战斗结束后，如果还没有战死，就会欢呼。
        if (wa_id==0){
            to_use = wa_num%3;
            weapon_id[to_use]++;
            /**morale = double(pCamp->HP) / Warrior::health[0];*/
            total++;
        }
            //ninja可以拥有两件武器。编号为n的ninja降生时即获得编号为 n%3 和 (n+1)%3的武器。ninja 使用bomb不会让自己受伤。
        else if (wa_id==1){
            to_use = min(wa_num%3,(wa_num+1)%3);
            weapon_id[wa_num%3]++;
            weapon_id[(wa_num+1)%3]++;
            total+=2;
            if (wa_num%3 == 1) durable.push_back(2);
        }
            //编号为n的iceman降生时即获得编号为n%3 的武器。iceman每前进一步，生命值减少10%(减少的量要去尾取整)。
        else if (wa_id==2){
            to_use = wa_num%3;
            weapon_id[to_use]++;
            total++;
        }
            //编号为n的lion降生时即获得编号为n%3 的武器。lion 有“忠诚度”这个属性，其初始值等于它降生之后其司令部剩余生命元的数目。每前进一步忠诚度就降低K。忠诚度降至0或0以下，则该lion逃离战场,永远消失。但是已经到达敌人司令部的lion不会逃跑。lion在己方司令部可能逃跑。
        else if (wa_id==3){
            to_use = wa_num%3;
            weapon_id[to_use]++;
            loyalty=pCamp->HP;
            total++;
        }
        //wolf降生时没有武器，但是在战斗开始前会抢到敌人编号最小的那种武器。如果敌人有多件这样的武器，则全部抢来。Wolf手里武器也不能超过10件。如果敌人arrow太多没法都抢来，那就先抢没用过的。如果敌人也是wolf，则不抢武器。
        else{

        }
        if (to_use == 2) durable.push_back(2);
    }

    int Damage(){//计算接下来对对手的伤害
        if (total<=0) return 0;
        int damage_;
        switch (to_use) {
            case 0: damage_ = damage_of_sword[0];/**damage_of_sword[to_use_num] = damage_of_sword[to_use_num] * 8 /10;*/break;
            case 1: damage_ = damage * 4 / 10;--total;--to_use_num;break;
            case 2:
                damage_ = damage * 3 / 10;
                --durable[0];
                if (!durable[0]) {
                    durable.erase(durable.begin());
                    --total;
                    --weapon_id[2];
                    to_use_num--;
                }
                break;

        }
        int cnt = 0;
        if (++to_use_num > weapon_id[to_use]){//超过限度后换用下一种武器。（进位）
            to_use_num = 1;
            to_use = (to_use+1)%3;
            while (!weapon_id[to_use] && cnt++ < 3) {
                to_use = (to_use+1)%3;
            }
        }
        return damage_;
    }

};
class City{
private:
    //Camp *pCamp;
    Warrior *pWarriorRed,*pWarriorBlue;
    //City *pCity;
    Attribute* pAttribute;
    int city_id;
    bool update_r = false;//该次是否更新了城中红武士？
    bool update_b = false;//该次是否更新了城中蓝武士？
    int flag = -1;//插旗
    int streak{0};//连胜，红方为正次数，蓝方为负次数
public:
    friend class Camp;
    friend class Attribute;
    friend class Warrior;
    City() = default;
    City(Warrior* pWarriorRed, Warrior* pWarriorBlue, int city_id) : pWarriorRed(pWarriorRed),pWarriorBlue(pWarriorBlue),city_id(city_id){}
    ~City(){}

    void report(const int& hour){
        if (!update_r && !update_b) return;
        if (update_r) printf("%03d:10 red %s %d marched to city %d with %d elements and force %d\n",hour,Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,city_id,pWarriorRed->pc->pAttribute[pWarriorRed->no - 1]->elements,pWarriorRed->pc->pAttribute[pWarriorRed->no - 1]->damage);
        if (update_b) printf("%03d:10 blue %s %d marched to city %d with %d elements and force %d\n",hour,Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id,pWarriorBlue->pc->pAttribute[pWarriorBlue->no - 1]->elements,pWarriorBlue->pc->pAttribute[pWarriorBlue->no - 1]->damage);
        update_r = update_b = false;
    }
    void Grab(const int& hour){
        if (pWarriorRed == nullptr || pWarriorBlue == nullptr) return;
        if (pWarriorRed->wa_id!=4 && pWarriorBlue->wa_id != 4) return;
        if (pWarriorRed->wa_id + pWarriorBlue->wa_id >= 8) return;
        Attribute *pr = pWarriorRed->pc->pAttribute[pWarriorBlue->no - 1];
        Attribute *pb = pWarriorBlue->pc->pAttribute[pWarriorBlue->no - 1];
        int cnt = 0,category;
        if (pWarriorRed->wa_id == 4) {//抢劫蓝方武器
            if (pr->total >= 10) return;
            if (pb->weapon_id[0]) {
                while (pb->weapon_id[0] && pr->total < 10){
                    ++cnt;
                    pb->weapon_id[0]--;
                    pb->total--;
                    pr->weapon_id[0]++;
                    pr->total++;
                }
                printf("%03d:35 red wolf %d took %d %s from blue %s %d in city %d\n",hour,pWarriorRed->no,cnt,Attribute::weapon[0],Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id);
            }
            else if (pb->weapon_id[1]) {
                while (pb->weapon_id[1] && pr->total < 10){
                    ++cnt;
                    pb->weapon_id[1]--;
                    pb->total--;
                    pr->weapon_id[1]++;
                    pr->total++;
                }
                printf("%03d:35 red wolf %d took %d %s from blue %s %d in city %d\n",hour,pWarriorRed->no,cnt,Attribute::weapon[1],Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id);
            }
            else if (pb->weapon_id[2]){
                sort(pb->durable.begin(),pb->durable.end());
                while (pb->weapon_id[2] && pr->total < 10){
                    ++cnt;
                    pb->weapon_id[2]--;
                    pb->total--;
                    pr->weapon_id[2]++;
                    pr->total++;
                    pr->durable.push_back(pb->durable[pb->durable.size() - 1]);
                    pb->durable.pop_back();
                }
                printf("%03d:35 red wolf %d took %d %s from blue %s %d in city %d\n",hour,pWarriorRed->no,cnt,Attribute::weapon[2],Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id);
            }
        }
        else {
            if (pb->total >= 10) return;
            if (pr->weapon_id[0]) {
                while (pr->weapon_id[0] && pb->total < 10){
                    ++cnt;
                    pr->weapon_id[0]--;
                    pr->total--;
                    pb->weapon_id[0]++;
                    pb->total++;
                }
                printf("%03d:35 blue wolf %d took %d %s from red %s %d in city %d\n",hour,pWarriorBlue->no,cnt,Attribute::weapon[0],Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,city_id);
            }
            else if (pr->weapon_id[1]) {
                while (pr->weapon_id[1] && pb->total < 10) {
                    ++cnt;
                    pr->weapon_id[1]--;
                    pr->total--;
                    pb->weapon_id[1]++;
                    pb->total++;
                }
                printf("%03d:35 blue wolf %d took %d %s from red %s %d in city %d\n",hour,pWarriorBlue->no,cnt,Attribute::weapon[1],Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,city_id);
            }
            else if (pr->weapon_id[2]){
                while (pr->weapon_id[2] && pb->total < 10) {
                    ++cnt;
                    pr->weapon_id[2]--;
                    pr->total--;
                    pb->weapon_id[2]++;
                    pb->total++;
                    pb->durable.push_back(pr->durable[pr->durable.size() - 1]);
                    pb->durable.pop_back();
                }
                printf("%03d:35 blue wolf %d took %d %s from red %s %d in city %d\n",hour,pWarriorBlue->no,cnt,Attribute::weapon[2],Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,city_id);
            }
        }
    }
    void Attack_Yell(const int& hour){
        if (pWarriorRed == nullptr || pWarriorBlue == nullptr) return;
        //Attack部分
        int red{1},blue{1};//红蓝方是否存活
        int win=-1,a,cnt=0;//同时存活为-1，红方赢为0，蓝方赢为1，平局为2
        if (flag == 0 || (city_id % 2 && flag != 1))//红武士先发动进攻
            do{
                a=Warrior::Attack(pWarriorRed,pWarriorBlue,hour);
                red = a/10;
                blue = a%10;
                if (red + blue == 1) {
                    if (red) {
                        printf("%03d:40 red %s %d killed blue %s %d in city %d remaining %d elements\n",hour,Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id,pWarriorRed->pc->pAttribute[pWarriorRed->no - 1]->elements);
                        pWarriorRed->win=1;
                        ++pWarriorRed->win_times;
                        pWarriorBlue->win =0;
                        --pWarriorBlue->win_times;
                        pWarriorBlue->alive = false;
                        win=0;
                    }
                    else {
                        printf("%03d:40 blue %s %d killed red %s %d in city %d remaining %d elements\n",hour,Warrior::name[pWarriorBlue->wa_id],pWarriorRed->no,Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,city_id,pWarriorBlue->pc->pAttribute[pWarriorBlue->no - 1]->elements);
                        pWarriorRed->win=0;
                        --pWarriorRed->win_times;
                        pWarriorBlue->win=1;
                        ++pWarriorBlue->win_times;
                        pWarriorRed->alive = false;
                        win=1;
                    }
                    break;
                }
                else if (!red && !blue) {
                    printf("%03d:40 both red %s %d and blue %s %d died in city %d\n",hour,Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id);
                    pWarriorRed->alive = pWarriorBlue->alive = false;
                    pWarriorRed->win=0;
                    pWarriorBlue->win =0;
                    win=2;
                    break;
                }
                a=Warrior::Attack(pWarriorBlue,pWarriorRed,hour);
                red = a%10;
                blue = a/10;
                if (red + blue == 1) {
                    if (red) {
                        printf("%03d:40 red %s %d killed blue %s %d in city %d remaining %d elements\n",hour,Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id,pWarriorRed->pc->pAttribute[pWarriorRed->no - 1]->elements);
                        pWarriorRed->win=1;
                        ++pWarriorRed->win_times;
                        pWarriorBlue->win =0;
                        --pWarriorBlue->win_times;
                        pWarriorBlue->alive = false;
                        win=0;
                    }
                    else {
                        printf("%03d:40 blue %s %d killed red %s %d in city %d remaining %d elements\n",hour,Warrior::name[pWarriorBlue->wa_id],pWarriorRed->no,Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,city_id,pWarriorBlue->pc->pAttribute[pWarriorBlue->no - 1]->elements);
                        pWarriorRed->win=0;
                        --pWarriorRed->win_times;
                        pWarriorBlue->win=1;
                        ++pWarriorBlue->win_times;
                        pWarriorRed->alive = false;
                        win=1;
                    }
                    break;
                }
                else if (!red && !blue) {
                    printf("%03d:40 both red %s %d and blue %s %d died in city %d\n",hour,Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id);
                    pWarriorRed->win=0;
                    pWarriorBlue->win=0;
                    pWarriorRed->alive = pWarriorBlue->alive = false;
                    win=2;
                    break;
                }
            }while(win < 0 && cnt<20);/**还要控制Damage()都是0时不要死循环*/

        else if (flag == 1 || 1 - city_id % 2) //蓝武士先发动进攻
            do{
                a=Warrior::Attack(pWarriorBlue,pWarriorRed,hour);
                red = a%10;
                blue = a/10;
                if (red + blue == 1) {
                    if (red) {
                        printf("%03d:40 red %s %d killed blue %s %d in city %d remaining %d elements\n",hour,Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id,pWarriorRed->pc->pAttribute[pWarriorRed->no - 1]->elements);
                        pWarriorRed->win=1;
                        ++pWarriorRed->win_times;
                        pWarriorBlue->win=0;
                        --pWarriorBlue->win_times;
                        pWarriorBlue->alive = false;
                        win=0;
                    }
                    else {
                        printf("%03d:40 blue %s %d killed red %s %d in city %d remaining %d elements\n",hour,Warrior::name[pWarriorBlue->wa_id],pWarriorRed->no,Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,city_id,pWarriorBlue->pc->pAttribute[pWarriorBlue->no - 1]->elements);
                        pWarriorRed->win=0;
                        --pWarriorRed->win_times;
                        pWarriorBlue->win=1;
                        ++pWarriorBlue->win_times;
                        pWarriorRed->alive = false;
                        win=1;
                    }
                    break;
                }
                else if (!red && !blue) {
                    printf("%03d:40 both red %s %d and blue %s %d died in city %d\n",hour,Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id);
                    pWarriorRed->win=0;
                    pWarriorBlue->win=0;
                    pWarriorRed->alive = pWarriorBlue->alive = false;
                    win=2;
                    break;
                }
                a=Warrior::Attack(pWarriorRed,pWarriorBlue,hour);
                red = a/10;
                blue = a%10;
                if (red + blue == 1) {
                    if (red) {
                        printf("%03d:40 red %s %d killed blue %s %d in city %d remaining %d elements\n",hour,Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id,pWarriorRed->pc->pAttribute[pWarriorRed->no - 1]->elements);
                        pWarriorRed->win=1;
                        ++pWarriorRed->win_times;
                        pWarriorBlue->win=0;
                        --pWarriorBlue->win_times;
                        pWarriorBlue->alive = false;
                        win=0;
                    }
                    else {
                        printf("%03d:40 blue %s %d killed red %s %d in city %d remaining %d elements\n",hour,Warrior::name[pWarriorBlue->wa_id],pWarriorRed->no,Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,city_id,pWarriorBlue->pc->pAttribute[pWarriorBlue->no - 1]->elements);
                        pWarriorRed->win=0;
                        --pWarriorRed->win_times;
                        pWarriorBlue->win=1;
                        ++pWarriorBlue->win_times;
                        pWarriorRed->alive = false;
                        win=1;
                    }
                    break;
                }
                else if (!red && !blue) {
                    printf("%03d:40 both red %s %d and blue %s %d died in city %d\n",hour,Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id);
                    pWarriorRed->win=0;
                    pWarriorBlue->win=0;
                    pWarriorRed->alive = pWarriorBlue->alive = false;
                    win=2;
                    break;
                }
            }while(win < 0 && cnt<20);/**还要控制Damage()都是0时不要死循环*/
        if (win == -1) {
            pWarriorRed->win=0;
            pWarriorBlue->win=0;
            printf("%03d:40 both red %s %d and blue %s %d were alive in city %d\n",hour,Warrior::name[pWarriorRed->wa_id],pWarriorRed->no,Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id);
        }
        //Yell部分
        pWarriorRed->Yell(hour);
        pWarriorBlue->Yell(hour);
    }
};

const char Warrior::name[5][7]={ "dragon","ninja","iceman","lion","wolf" };
const char Attribute::weapon[3][6]={"sword","bomb","arrow"};
int Warrior::health[5];
int Warrior::attack[5];
int Camp::Seq[2][5] = { { 2,3,4,1,0 },{3,0,1,2,4} }; //两个司令部武士的制作顺序序列

Warrior::Warrior( Camp *pCamp,int no_,int kindNo_) : no(no_),wa_id(kindNo_),pc(pCamp),city_id(pCamp->city()),alive(true){}
void Warrior::Print(int hour)
{
    string color=pc->Get_Color();

    printf("%03d:00 %s %s %d born with strength %d,%d %s in %s headquarter\n",
           hour, color.c_str(), name[wa_id], no, health[wa_id],
           pc->warrior[wa_id], name[wa_id], color.c_str());
    if (wa_id == 0)
        printf("It has a %s\n", Attribute::weapon[pc->pAttribute[no - 1]->weapon_id[0]]);
    else if (wa_id == 1)
        printf("It has a %s and a %s\n", Attribute::weapon[pc->pAttribute[no - 1]->weapon_id[0]],
               Attribute::weapon[pc->pAttribute[no - 1]->weapon_id[1]]);
    else if (wa_id == 2) printf("It has a %s\n", Attribute::weapon[pc->pAttribute[no - 1]->weapon_id[0]]);
    else if (wa_id == 3) printf("It's loyalty is %d\n", pc->pAttribute[no - 1]->loyalty);


}
void Warrior::Run(const int &hour) {
    if (wa_id == 3 && alive && pc->pAttribute[no - 1]->loyalty<=0){
        alive = false;
        printf("%03d:05 %s lion %d ran away\n",hour,pc->Get_Color().c_str(),no);
    }
}
void Warrior::Move(const int& hour){
    if (alive){

        if (pc->color && city_id > 0) {
            pCity[city_id]->pWarriorBlue = nullptr;
            pCity[--city_id]->pWarriorBlue = this;
            pCity[city_id]->update_b = true;
            if (wa_id == 2) pc->pAttribute[no - 1]->elements -= pc->pAttribute[no - 1]->elements/10; //不知这样有没有问题
            else if (wa_id == 3) pc->pAttribute[no - 1]->loyalty-=k;
            //pc->pAttribute[no-1]->Damage();
        }//蓝色阵营
        else if (!pc->color && city_id < n+1) {
            pCity[city_id]->pWarriorRed = nullptr;
            pCity[++city_id]->pWarriorRed = this;
            pCity[city_id]->update_r = true;
            if (wa_id == 2) pc->pAttribute[no - 1]->elements -= pc->pAttribute[no - 1]->elements/10; //不知这样有没有问题
            else if (wa_id == 3) pc->pAttribute[no - 1]->loyalty-=k;
            //pc->pAttribute[no-1]->Damage();
        }//红色阵营
    }
}
int Warrior::Attack(Warrior* me, Warrior *enemy, const int& hour) {
    int &enemy_life = enemy->pc->pAttribute[enemy->no - 1]->elements;
    int &my_life = me->pc->pAttribute[enemy->no - 1]->elements;
    int isBomb = false;
    if (me->pc->pAttribute[me->no - 1]->to_use == 1) isBomb = true;
    int damage = me->pc->pAttribute[me->no - 1]->Damage();
    enemy_life -= damage;
    if (isBomb && me->wa_id!=1) my_life -= damage/2;
    return (my_life>0)*10 + (enemy_life > 0);//十位数，十位是我方是否存活，个位是敌方是否存活
}
void Warrior::Yell(const int &hour) const{
    if (wa_id>0) return;
    printf("%03d:40 %s dragon yelled in city %d\n",hour,pc->Get_Color().c_str(),city_id);
}
/*
void Camp::Init(int coco,int health)
{
    color=coco;
    HP=health;
    wa_num=0;
    sign=false;
    wa_id=0;
    memset(warrior,0,sizeof(warrior));
}
 */
Camp::~Camp()
{
    for(int i=0;i<wa_num;i++){
        delete pWarrior[i];
        delete pAttribute[i];
    }
}
bool Camp::Produce(const int& time)
{
    if(sign) return false;

    int kind=Seq[color][wa_id];
    if( Warrior::health[kind]>HP){
        sign=true;
        //if(color) printf("%03d:00 blue headquarter stops making warriors\n",time);
        //else printf("%03d:00 red headquarter stops making warriors\n",time);
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
void Camp::Run_at5(const int &hour) {
    for (int i=0;i<wa_num;++i) pWarrior[i]->Run(hour);
}
void Camp::Move(const int& hour){
    for (int i=0;i<wa_num;++i) pWarrior[i]->Move(hour);
}
//void Camp::Grab(const int& hour){}

string Camp::Get_Color() const
{
    if(color) return "blue";
    else return "red";
}
int Camp::city() const {
    if (color) return n+1;
    else return 0;
}




int main()
{

    Camp Red,Blue;
    scanf("%d",&T);
    for(int i=1;i<=T;i++){
        scanf("%d%d%d%d",&m,&n,&k,&t);
        Red=Camp(0,m),Blue=Camp(1,m);

        for (int i=0;i<n+2;++i) pCity[i] = new City(nullptr, nullptr, i);
        for(int & health : Warrior::health)
            scanf("%d",&health);
        for(int & att : Warrior::attack)
            scanf("%d",&att);
        printf("Case %d:\n",i);
        //Red.Init(0,m);
        //Blue.Init(1,m);
        int hour=0;
        while (true){
            bool sign=Red.Produce(hour);
            bool flag=Blue.Produce(hour);
            Red.Run_at5(hour);
            Blue.Run_at5(hour);
            //10分
            Red.Move(hour);
            Blue.Move(hour);
            //是否抵达敌军司令部？
            //……
            for (int i=0;i<n+2;++i) pCity[i]->report(hour);
            for (int i=1;i<n+1;++i) pCity[i]->Grab(hour);
            for (int i=1;i<n+1;++i) pCity[i]->Attack_Yell(hour);

            //报告应该放在这里
            //Red.Grab(hour);
            //Blue.Grab(hour);
            if(!sign&&!flag) break;
            ++hour;
        }
        for (int i=0;i<n+2;++i) delete pCity[i];
    }
    return 0;
}
