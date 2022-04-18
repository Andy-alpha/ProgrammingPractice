#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;
/*
string Warrior::names[WARRIOR_NUM] = { "dragon","ninja","iceman","lion","wolf" };
红方司令部按照 iceman、lion、wolf、ninja、dragon 的顺序制造武士。
蓝方司令部按照 lion、dragon、ninja、iceman、wolf 的顺序制造武士。
*/
int T,m,n,r,k,t;
int taken[2]{0};
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
    static bool Shoot(Warrior* me, Warrior *enemy);
    static void Bomb(Warrior* me, Warrior *enemy, const int& turn, const int& hour);
    //void Grab(const int& hour);
    static int Attack(Warrior* me,Warrior *enemy, const unsigned short& turn);
    void Yell(const int &hour) const;
    void Report(const int& hour);
};
class Camp
{
private:
    int HP;
    [[maybe_unused]] bool sign;//司令部是否存活
    int wa_num;//计数
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
        sign=true;
        wa_id=0;
        memset(warrior,0,sizeof(warrior));
    }
    //void Init(int coco,int health);
    //void HP_Plus(const int& delta);
    ~Camp();
    bool Produce(const int& hour);
    void Run_at5(const int& hour);
    void Move(const int& hour);
    //void Grab(const int& hour);
    //void Attack(const int &hour);
    void Report(const int& hour) const;
    void ReportWarrior(const int&hour);
    string Get_Color() const;
    int city() const;
};
class Attribute{
private:
    int wa_num{0};//当前要制造的武士是制造序列中的第几个
    int wa_id{0};//武士种类
    //int warrior[5];//存放每种武士的数量
    //Warrior *pWarrior[1000];

    double morale=0;//士气
    int loyalty=0;//忠诚度
    bool weapon_id[3]{0};//三种武器及数目
    int elements;//生命值
    int damage;//武器伤害
    int damage_of_sword;
    //int to_use;//接下来要用的武器
    //int to_use_num{1};//以防要用的武器重号，加以区分
    int durable{0};//储存箭的耐久
    int total{0};//手中武器的总数
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
        damage_of_sword = damage * 2 / 10;
            //dragon可以拥有一件武器。编号为n的dragon降生时即获得编号为 n%3 的武器。dragon还有“士气”这个属性，是个浮点数，其值为它降生后其司令部剩余生命元的数量除以造dragon所需的生命元数量。dragon 在一次在它主动进攻的战斗结束后，如果还没有战死，而且士气值大于0.8，就会欢呼。dragon每取得一次战斗的胜利(敌人被杀死)，士气就会增加0.2，每经历一次未能获胜的战斗，士气值就会减少0.2。士气增减发生在欢呼之前。
        if (wa_id==0){
            weapon_id[wa_num%3]=true;
            morale = double(pCamp->HP) / Warrior::health[0];
            total++;
        }
            //ninja可以拥有两件武器。编号为n的ninja降生时即获得编号为 n%3 和 (n+1)%3的武器。ninja 挨打了也从不反击敌人。
        else if (wa_id==1) {
            weapon_id[wa_num%3]=true;
            weapon_id[(wa_num+1)%3]=true;
            total+=2;
            if (wa_num%3 == 1) durable=3;
        }
            //iceman有一件武器。编号为n的iceman降生时即获得编号为 n%3 的武器。iceman 每前进两步，在第2步完成的时候，生命值会减少9，攻击力会增加20。但是若生命值减9后会小于等于0，则生命值不减9,而是变为1。即iceman不会因走多了而死。
        else if (wa_id==2){
            weapon_id[wa_num%3]=true;
            total++;
        }
            //lion 有“忠诚度”这个属性，其初始值等于它降生之后其司令部剩余生命元的数目。每经过一场未能杀死敌人的战斗，忠诚度就降低K。忠诚度降至0或0以下，则该lion逃离战场,永远消失。但是已经到达敌人司令部的lion不会逃跑。Lion在己方司令部可能逃跑。lion 若是战死，则其战斗前的生命值就会转移到对手身上。所谓“战斗前”，就是每个小时的40分前的一瞬间。
        else if (wa_id==3){
            //weapon_id[wa_num%3]=true;
            loyalty=pCamp->HP;
            //total++;
        }
            //wolf降生时没有武器，但是在战斗中如果获胜（杀死敌人），就会缴获敌人的武器，但自己已有的武器就不缴获了。被缴获的武器当然不能算新的，已经被用到什么样了，就是什么样的。
        else{

        }
            //damage为0的剑不算拥有
        if (damage_of_sword <= 0 && weapon_id[0]) {
            weapon_id[0]=false;
            total--;
        }
        if (wa_num % 3 == 2) durable=3;
    }

    int Damage(const unsigned short& turn){//计算接下来对对手的伤害,turn=0 代表主动进攻，1 代表反击
        if (!weapon_id[0]) return damage/(1+turn);
        int damage_=damage/(1+turn)+damage_of_sword;
        damage_of_sword = damage_of_sword * 4 / 5;
        if (damage_of_sword<=0) weapon_id[0]=false,--total;
        return damage_;
    }

};
class City{
private:
    //Camp *pCamp;
    //Warrior *pWarriorRed,*pWarriorBlue;
    Warrior *pWarriorRB[2];
    //City *pCity;
    Attribute* pAttribute;
    int city_id;
    int elements{0};
    bool update[2] = {false,false};//该次是否更新了城中红（0）蓝（1）武士？
    int flag = -1;//插旗，红方为0，蓝方为1
    int streak{0};//连胜，红方为正次数，蓝方为负次数
public:
    friend class Camp;
    friend class Attribute;
    friend class Warrior;
    City() = default;
    City(Warrior* pWarriorRed, Warrior* pWarriorBlue, int city_id) : pWarriorRB{pWarriorRed,pWarriorBlue},city_id(city_id){}
    ~City() = default;
    void Element_Plus(const int& delta) { elements += delta;}
    void Element_Take(const int& hour) {
        if (pWarriorRB[0] == nullptr && pWarriorRB[1] == nullptr) return;
        if (pWarriorRB[0] == nullptr || !pWarriorRB[0]->alive && pWarriorRB[1]->alive) {
            if (elements<=0) return;
            printf("%03d:30 blue %s %d earned %d elements for his headquarter\n",hour,Warrior::name[pWarriorRB[1]->wa_id],pWarriorRB[1]->no,elements);
            pWarriorRB[1]->pc->HP+=elements,elements=0;
        }
        else if (pWarriorRB[1] == nullptr || !pWarriorRB[1]->alive && pWarriorRB[0]->alive) {
            if (elements<=0) return;
            printf("%03d:30 red %s %d earned %d elements for his headquarter\n",hour,Warrior::name[pWarriorRB[0]->wa_id],pWarriorRB[0]->no,elements);
            pWarriorRB[0]->pc->HP+=elements,elements=0;
        }
    }
    void report(const int& hour){
        if (!update[0] && !update[1]) return;
        for (int i=0;i<2;++i)
            if (update[i]) {
                if (city_id != 0 && city_id != n+1)
                    printf("%03d:10 %s %s %d marched to city %d with %d elements and force %d\n",hour,pWarriorRB[i]->pc->Get_Color().c_str(),Warrior::name[pWarriorRB[i]->wa_id],pWarriorRB[i]->no,city_id,pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no - 1]->elements,pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no - 1]->damage);
                else if (city_id == n+1) {
                    printf("%03d:10 red %s %d reached blue headquarter with %d elements and force %d\n",hour,Warrior::name[pWarriorRB[i]->wa_id],pWarriorRB[i]->no,pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no - 1]->elements,pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no - 1]->damage);
                    taken[1]++;
                    if (taken[1] == 2) printf("%03d:10 blue headquarter was taken\n",hour);
                }
                else {
                    printf("%03d:10 blue %s %d reached red headquarter with %d elements and force %d\n",hour,Warrior::name[pWarriorRB[i]->wa_id],pWarriorRB[i]->no,pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no - 1]->elements,pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no - 1]->damage);
                    taken[0]++;
                    if (taken[0] == 2) printf("%03d:10 red headquarter was taken\n",hour);
                }
            }
        update[0] = update[1] = false;
    }
    void Shoot(const int& hour) {
        for (int i = 0;i<2;++i)
            if (pWarriorRB[i] != nullptr && pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no -1]->weapon_id[2] > 0 &&
                (2 * i - 1)*city_id > pWarriorRB[i]->pc->city() - n && pCity[city_id-2*i+1]->pWarriorRB[1-i] != nullptr) {
                printf("%03d:35 %s %s %d shot",hour,pWarriorRB[i]->pc->Get_Color().c_str(),Warrior::name[pWarriorRB[i]->wa_id],pWarriorRB[i]->no);
                if (Warrior::Shoot(pWarriorRB[i],pCity[city_id-2*i+1]->pWarriorRB[1-i]))
                    printf(" and killed %s %s %d",pWarriorRB[1-i]->pc->Get_Color().c_str(),Warrior::name[pCity[city_id-2*i+1]->pWarriorRB[1-i]->wa_id],pCity[city_id-2*i+1]->pWarriorRB[1-i]->no);
                putchar('\n');

            }
    }
    void Bomb(const int& hour) {
        for (int i=0;i<2;++i)
            if (pWarriorRB[i] != nullptr && pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no -1]->weapon_id[1] > 0 && pWarriorRB[1-i] != nullptr){
                int turn;
                if (flag == -1 && city_id%2 != i || flag == i)
                Warrior::Bomb(pWarriorRB[i],pWarriorRB[1-i],flag,hour);
            }
    }
    void Grab(const int& hour) {
        if (pWarriorRB[0] == nullptr || pWarriorRB[1] == nullptr) return;
        if (pWarriorRB[0]->wa_id!=4 && pWarriorRB[1]->wa_id != 4) return;
        //if (pWarriorRB[0]->wa_id + pWarriorRB[1]->wa_id >= 8) return;
        Attribute *p[2]{pWarriorRB[0]->pc->pAttribute[pWarriorRB[0]->no - 1],pWarriorRB[1]->pc->pAttribute[pWarriorRB[1]->no - 1]};
        //int cnt = 0,category;
        for (int i=0;i<2;++i)
            if (pWarriorRB[i]->wa_id == 4 && pWarriorRB[i]->win) {//抢劫敌方武器
                if (p[i]->total >= 3) return;
                if (p[1-i]->weapon_id[0] && !p[i]->weapon_id[0]) {
                    p[1-i]->weapon_id[0]=false;
                    p[1-i]->total--;
                    p[i]->weapon_id[0]=true;
                    p[i]->total++;
                    p[i]->damage_of_sword=p[1-i]->damage_of_sword;
                    //printf("%03d:35 %s wolf %d took %d %s from %s %s %d in city %d\n",hour,pWarriorRB[i]->pc->Get_Color().c_str(),pWarriorRB[i]->no,cnt,Attribute::weapon[0],pWarriorRB[1-i]->pc->Get_Color().c_str(),Warrior::name[pWarriorRB[1-i]->wa_id],pWarriorRB[1-i]->no,city_id);
                }

                else if (p[1-i]->weapon_id[1] && !p[i]->weapon_id[1]) {
                    p[1-i]->weapon_id[1]=false;
                    p[1-i]->total--;
                    p[i]->weapon_id[1]=true;
                    p[i]->total++;
                    //printf("%03d:35 red wolf %d took %d %s from blue %s %d in city %d\n",hour,pWarriorRed->no,cnt,Attribute::weapon[1],Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id);
                }
                else if (p[1-i]->weapon_id[2] && !p[i]->weapon_id[2]){
                    p[1-i]->weapon_id[2]=false;
                    p[1-i]->total--;
                    p[i]->weapon_id[2]=true;
                    p[i]->total++;
                    p[i]->durable=p[1-i]->durable;
                    p[1-i]->durable=0;
                    //printf("%03d:35 red wolf %d took %d %s from blue %s %d in city %d\n",hour,pWarriorRed->no,cnt,Attribute::weapon[2],Warrior::name[pWarriorBlue->wa_id],pWarriorBlue->no,city_id);
                }

                //Grab要在战胜后决定
            }
    }
    void Attack_Yell(const int& hour) {
        if (pWarriorRB[0] == nullptr || pWarriorRB[1] == nullptr) return;
        int *Lion_life[2]{nullptr, nullptr};
        for (int i=0;i<2;++i)
            if (pWarriorRB[i]->wa_id == 3) Lion_life[i]=&pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no - 1]->elements;
        //Attack部分
        int red_blue[2]{pWarriorRB[0]->alive,pWarriorRB[1]->alive};//红蓝方是否存活
        int win=-1,killed,cnt=0;//同时存活为-1，红方赢为0，蓝方赢为1，平局为2
        unsigned short i;
        auto f=[&](const unsigned short&x)->string{if (x) return "blue";return "red";};
        if (flag != 1 && city_id % 2 || flag == 0)//红武士先发动进攻
            i=0;
        else i=1;//蓝武士先发动进攻
        if (red_blue[0]+red_blue[1]==0) return;
        if (red_blue[0]+red_blue[1]==1) {
            if (red_blue[i]) win = i;
            else win = 1-i;
            goto step9;
        }
        //(6) 武士主动进攻部分
        printf("%03d:40 %s %s %d attacked %s %s %d in city %d with %d elements and force %d\n",hour,f(i).c_str(),Warrior::name[pWarriorRB[i]->wa_id],pWarriorRB[i]->no,f(1-i).c_str(),Warrior::name[pWarriorRB[1-i]->wa_id],pWarriorRB[1-i]->no,city_id,pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no - 1]->elements,pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no - 1]->damage);
        killed = Warrior::Attack(pWarriorRB[i], pWarriorRB[1-i], 0);
        if (pWarriorRB[i]->wa_id==0) {
            if (killed) pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no - 1]->morale+=0.2;
            else pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no - 1]->morale-=0.2;
        }
        if (killed) {
            win=i;
            pWarriorRB[i]->win=1;
            pWarriorRB[1-i]->win=0;
            pWarriorRB[1-i]->alive=false;
            if (pWarriorRB[1-i]->wa_id == 3)
                pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no - 1]->elements+=*Lion_life[i];
        }
        else if (pWarriorRB[i]->wa_id==3)
            pWarriorRB[i]->pc->pAttribute[pWarriorRB[i]->no - 1]->loyalty-=k;
        //(7) 武士反击部分
        if (win == -1) {
            pWarriorRB[0]->win=pWarriorRB[1]->win=0;
            if (pWarriorRB[1-i]->wa_id!=1) {
                printf("%03d:40 %s %s %d fought back against %s %s %d in city %d\n",hour,f(1-i).c_str(),Warrior::name[pWarriorRB[1-i]->wa_id],pWarriorRB[1-i]->no,f(i).c_str(),Warrior::name[pWarriorRB[i]->wa_id],pWarriorRB[i]->no,city_id);
                killed = - Warrior::Attack(pWarriorRB[1-i], pWarriorRB[i], 1);
                if (killed) {
                    win = 1-i;
                    pWarriorRB[i]->alive=false;
                    pWarriorRB[1-i]->win=1;

                    if (pWarriorRB[i]->wa_id == 3)
                        pWarriorRB[1-i]->pc->pAttribute[pWarriorRB[1-i]->no - 1]->elements+=*Lion_life[1-i];
                }
                else if (pWarriorRB[1-i]->wa_id==3)
                    pWarriorRB[1-i]->pc->pAttribute[pWarriorRB[1-i]->no - 1]->loyalty-=k;
            }
        }
        //(8) 武士战死部分（被箭射死的不算）
        if (killed==1)
            printf("%03d:40 %s %s %d was killed in city %d\n",hour,f(1-i).c_str(),Warrior::name[pWarriorRB[1-i]->wa_id],pWarriorRB[1-i]->no,city_id);
        else if (killed==-1)
            printf("%03d:40 %s %s %d was killed in city %d\n",hour,f(i).c_str(),Warrior::name[pWarriorRB[i]->wa_id],pWarriorRB[i]->no,city_id);
        //(9) Yell部分
    step9:
        pWarriorRB[0]->Yell(hour);
        pWarriorRB[1]->Yell(hour);
        //(10) 武士获取elements部分
        for (int rb=0;rb<2;++rb)
            if (win == rb) {
                if (pWarriorRB[rb]->wa_id == 3) Grab(hour);
                printf("%03d:40 %s %s %d earned %d elements for his headquarter\n",hour,f(rb).c_str(),Warrior::name[pWarriorRB[rb]->wa_id],pWarriorRB[rb]->no,elements);
                pWarriorRB[rb]->pc->HP+=elements;
                elements=0;
            }
        //(11) 旗帜升起
        if (win==-1) {
            streak = 0;
            return;
        }
        if (streak*(2*win - 1)<0) {//同号
            streak+=1-2*win;
            if (streak*(1-2*win)==2 && flag != win) {
                flag=win;
                printf("%03d %s flag raised in city %d\n",hour,f(win).c_str(),city_id);
            }
        }
        else streak=0;
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

    printf("%03d:00 %s %s %d born\n",
           hour, color.c_str(), name[wa_id], no);
    if (wa_id == 0)
        printf("Its morale is %.2f\n", pc->pAttribute[no - 1]->morale);
    else if (wa_id == 3) printf("Its loyalty is %d\n", pc->pAttribute[no - 1]->loyalty);


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
            pCity[city_id]->pWarriorRB[1] = nullptr;
            pCity[--city_id]->pWarriorRB[1] = this;
            pCity[city_id]->update[1] = true;
            if (wa_id == 2 && (n+1-city_id)%2==0 && city_id > 0) {
                int& elements=pc->pAttribute[no - 1]->elements;
                if (elements > 9) elements -= 9;
                else elements = 1;
                pc->pAttribute[no - 1]->damage +=20;
            }
            /**else if (wa_id == 3) pc->pAttribute[no - 1]->loyalty-=k;*/
            //pc->pAttribute[no-1]->Damage();
        }
        //以上是蓝色阵营
        else if (!pc->color && city_id < n+1) {
            pCity[city_id]->pWarriorRB[0] = nullptr;
            pCity[++city_id]->pWarriorRB[0] = this;
            pCity[city_id]->update[0] = true;
            if (wa_id == 2 && city_id%2==0 && city_id < n+1) {
                int& elements=pc->pAttribute[no - 1]->elements;
                if (elements > 9) elements -= 9;
                else elements = 1;
                pc->pAttribute[no - 1]->damage +=20;
            }
            //不知这样有没有问题
            /**else if (wa_id == 3) pc->pAttribute[no - 1]->loyalty-=k;*/
            //pc->pAttribute[no-1]->Damage();
        }
        //以上是红色阵营
    }
}
int Warrior::Attack(Warrior* me, Warrior *enemy, const unsigned short& turn) {
    int &enemy_life = enemy->pc->pAttribute[enemy->no - 1]->elements;
    enemy_life -= me->pc->pAttribute[me->no - 1]->Damage(turn);
    return enemy_life <= 0;//有没有杀死
}
void Warrior::Yell(const int &hour) const{
    if (wa_id>0) return;
    if (alive)
        printf("%03d:40 %s dragon yelled in city %d\n",hour,pc->Get_Color().c_str(),city_id);
}
bool Warrior::Shoot(Warrior* me, Warrior *enemy) {
    int& durable=me->pc->pAttribute[me->no - 1]->durable;
    if (durable>1) --durable;
    else {
        durable=0;
        me->pc->pAttribute[me->no - 1]->total--;
        me->pc->pAttribute[me->no - 1]->weapon_id[2]=false;
        //me->pc->pAttribute[me->no - 1]->elements;
    }
    int &enemy_life = enemy->pc->pAttribute[enemy->no - 1]->elements;
    if (enemy_life <= r) {
        enemy_life = 0;
        enemy->alive=false;
        return true;
    }
    enemy_life-=r;
    return false;
}
void Warrior::Bomb(Warrior* me, Warrior *enemy, const int& turn, const int& hour) {
    //auto f=[&](const unsigned short&x)->string{if (x) return "blue";return "red";};
    int damage;
    if (turn) {//反击的一方
        damage=enemy->pc->pAttribute[enemy->no-1]->damage;
        if (enemy->pc->pAttribute[enemy->no-1]->weapon_id[0]) damage+=enemy->pc->pAttribute[enemy->no-1]->damage_of_sword;
        if (me->pc->pAttribute[me->no - 1]->elements > damage) return;
    }
    else {//主动攻击的一方
        damage=me->pc->pAttribute[me->no-1]->damage;
        if (me->pc->pAttribute[me->no-1]->weapon_id[0]) damage+=me->pc->pAttribute[me->no-1]->damage_of_sword;
        int enemy_life = enemy->pc->pAttribute[enemy->no - 1]->elements;
        if (enemy_life<=damage) return;
        damage=enemy->pc->pAttribute[enemy->no-1]->damage/2;
        if (enemy->pc->pAttribute[enemy->no-1]->weapon_id[0]) damage+=enemy->pc->pAttribute[enemy->no-1]->damage_of_sword;
        if (me->pc->pAttribute[me->no - 1]->elements > damage) return;
    }
    printf("%03d:38 %s %s %d used a bomb and killed %s %s %d\n",hour,me->pc->Get_Color().c_str(),Warrior::name[me->wa_id],me->no,enemy->pc->Get_Color().c_str(),Warrior::name[enemy->wa_id],enemy->no);
}
void Warrior::Report(const int& hour) {
    if (!alive) {
        win = win_times = 0;
        Attribute* p=pc->pAttribute[no-1];
        for (bool & i : p->weapon_id) i=false;
        p->total=0;
        return;
    }
    printf("%03d:55 %s %s %d has ",hour,pc->Get_Color().c_str(),name[wa_id],no);
    int no_weapon = 1;
    bool* Weapon = pc->pAttribute[no-1]->weapon_id;
    if (Weapon[2]) {
        no_weapon=0;
        printf("arrow(%d)",pc->pAttribute[no-1]->durable);
    }
    if (Weapon[1]) {
        if (!no_weapon)
            putchar(',');
        printf("bomb");
        no_weapon=0;
    }
    if (Weapon[0]) {
        if (!no_weapon)
            putchar(',');
        printf("sword(%d)",pc->pAttribute[no-1]->damage_of_sword);
        no_weapon = 0;
    }
    if (no_weapon) printf("no weapon");
    putchar('\n');
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
    int kind=Seq[color][wa_id];
    if( Warrior::health[kind]>HP)
        return false;//生命元不足，不制作武士
    //制作士兵：
    HP-=Warrior::health[kind];
    wa_id=(wa_id+1)%5;
    pWarrior[wa_num]=new Warrior(this,wa_num+1,kind);
    pAttribute[wa_num]=new Attribute(this,wa_num+1,kind);
    warrior[kind]++;
    pWarrior[wa_num++]->Print(time);
    return true;
}
//void Camp::HP_Plus(const int& delta) { HP+=delta;}
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
void Camp::Report(const int& hour) const {
    printf("%03d:50 %d elements in %s headquarter\n",hour,HP,Get_Color().c_str());
}
void Camp::ReportWarrior(const int& hour){
    if (color)
        for (int i=0;i<wa_num;++i)
            pWarrior[i]->Report(hour);
    else
        for (int i=wa_num - 1;i>=0;--i)
            pWarrior[i]->Report(hour);
}


int main()
{
    Camp Red,Blue;
    scanf("%d",&T);
    for(int i=1;i<=T;i++){
        scanf("%d%d%d%d%d",&m,&n,&r,&k,&t);
        Red=Camp(0,m),Blue=Camp(1,m);
        taken[0]=taken[1]=0;
        for (int i=0;i<n+2;++i) pCity[i] = new City(nullptr, nullptr, i);
        for(int & health : Warrior::health)
            scanf("%d",&health);
        for(int & att : Warrior::attack)
            scanf("%d",&att);
        printf("Case %d:\n",i);
        //Red.Init(0,m);
        //Blue.Init(1,m);
        int hour=0;
        while (60*hour <= t) {
            bool sign=Red.Produce(hour);
            bool flag=Blue.Produce(hour);
            //在每个小时的第5分，该逃跑的lion就在这一时刻逃跑了。
            if (60*hour+5 > t) break;
            Red.Run_at5(hour);
            Blue.Run_at5(hour);
            //在每个小时的第10分：所有的武士朝敌人司令部方向前进一步。即从己方司令部走到相邻城市，或从一个城市走到下一个城市。或从和敌军司令部相邻的城市到达敌军司令部。
            if (60*hour+10 > t) break;
            Red.Move(hour);
            Blue.Move(hour);
            //10分是否抵达敌军司令部？是否被占领？
            for (int i=0;i<n+2;++i) pCity[i]->report(hour);
            if (taken[0]>=2 || taken[1] >= 2) break;
            //在每个小时的第20分：每个城市产出10个生命元。生命元留在城市，直到被武士取走。
            if (60*hour+20 > t) break;
            for (int city=1;city<=n;++city) pCity[city]->Element_Plus(10);
            //在每个小时的第30分：如果某个城市中只有一个武士，那么该武士取走该城市中的所有生命元，并立即将这些生命元传送到其所属的司令部。
            if (60*hour+30 > t) break;
            for (int city=1;city<=n;++city) pCity[city]->Element_Take(hour);
            //在每个小时的第35分，拥有arrow的武士放箭，对敌人造成伤害。放箭事件应算发生在箭发出的城市。注意，放箭不算是战斗，因此放箭的武士不会得到任何好处。武士在没有敌人的城市被箭射死也不影响其所在城市的旗帜更换情况。
            if (60*hour+35 > t) break;
            for (int city=1;city<n+1;++city) pCity[city]->Shoot(hour);
            //在每个小时的第38分，拥有bomb的武士评估是否应该使用bomb。如果是，就用bomb和敌人同归于尽。
            if (60*hour+38 > t) break;
            for (int city=1;city<n+1;++city) pCity[city]->Bomb(hour);
            //在每个小时的第40分：在有两个武士的城市，会发生战斗。 如果敌人在5分钟前已经被飞来的arrow射死，那么仍然视为发生了一场战斗，而且存活者视为获得了战斗的胜利。此情况下不会有“武士主动攻击”，“武士反击”，“武士战死”的事件发生，但战斗胜利后应该发生的事情都会发生。如Wolf一样能缴获武器，旗帜也可能更换，等等。在此情况下,Dragon同样会通过判断是否应该轮到自己主动攻击来决定是否欢呼。
            if (60*hour+40 > t) break;
            for (int city=1;city<n+1;++city) pCity[city]->Attack_Yell(hour);
            for (int city=1;city<n+1;++city) pCity[city]->Grab(hour);
            //Red.Grab(hour);
            //Blue.Grab(hour);
            //在每个小时的第50分，司令部报告它拥有的生命元数量。
            if (60*hour+50 > t) break;
            //报告应该放在这里
            Red.Report(hour);
            Blue.Report(hour);
            //在每个小时的第55分，每个武士报告其拥有的武器情况。
            //记得删除死亡武士的武器！
            if (60*hour+55 > t) break;
            Red.ReportWarrior(hour);
            Blue.ReportWarrior(hour);
            if(!sign&&!flag) break;
            ++hour;
        }
        for (int i=0;i<n+2;++i) delete pCity[i];
    }
    return 0;
}
