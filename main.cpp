#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include "cmath"
#include <algorithm>


using namespace std;

// prototypes
template <class datatype> void print_bits(datatype x) ;
template <class datatype> void print_hex(datatype x) ;
template <class regtype>  void mov_reg_reg(regtype *preg1,regtype *preg2)  ;
template <class regtype>  void mov_reg_offset(regtype *preg1, int a)  ;
template <class regtype>  void mov_reg_hex(regtype *preg1,unsigned char c)  ;
template <class regtype>  void mov_reg_dec(regtype *preg1,int a)  ;
template <class regtype>  void add_reg(regtype *preg, unsigned char c)  ;
template <class regtype>  void sub_reg(regtype *preg, unsigned char c)  ;
template <class regtype>  void not_reg(regtype *preg)  ;
template <class regtype>  void or_reg_reg(regtype *preg1, regtype *preg2)  ;
template <class regtype>  void xor_reg_reg(regtype *preg1, regtype *preg2)  ;
template  <class regtype> void and_reg_reg(regtype *preg1, regtype *preg2)  ;
template <class regtype>  void mul_reg(regtype *preg)  ;
template <class regtype>  void div_reg(regtype *preg)  ;
unsigned char hex2dec(string hex);
template <class regtype>  void inc_reg(regtype *preg);
template <class regtype>  void dec_reg(regtype *preg);
void print_16bitregs() ;

// global variables ( memory, registers and flags )
unsigned char memory[2<<15];    // 64K memory
vector<string> lines;
unsigned short ax = 0 ;
unsigned short bx = 0 ;
unsigned short cx = 0 ;
unsigned short dx = 0 ;
unsigned short PC = 0 ;
unsigned short di = 0 ;
unsigned short bp = 0 ;
unsigned short si = 0 ;
unsigned short sp = (2<<15)-2 ;

bool     zf       ;              // zero flag
bool     sf       ;              // sign flag
bool     af       ;              // auxillary flag
bool     of       ;              // overflow flag
bool     cf       ;              // carry flag

// initialize pointers
unsigned short *pax = &ax ;
unsigned short *pbx = &bx ;
unsigned short *pcx = &cx ;
unsigned short *pdx = &dx ;
// note that x86 uses little endian, that is, least significat byte is stored in lowest byte
unsigned char *pah = (unsigned char *) ( ( (unsigned char *) &ax) + 1) ;
unsigned char *pal = (unsigned char *) &ax  ;
unsigned char *pbh = (unsigned char *) ( ( (unsigned char *) &bx) + 1) ;
unsigned char *pbl = (unsigned char *) &bx  ;
unsigned char *pch = (unsigned char *) ( ( (unsigned char *) &cx) + 1) ;
unsigned char *pcl = (unsigned char *) &cx  ;
unsigned char *pdh = (unsigned char *) ( ( (unsigned char *) &dx) + 1) ;
unsigned char *pdl = (unsigned char *) &dx  ;

int main(int argc, char* argv[])
{
    map <string, int> labels;
    map <string, int> vars;
    vector<string> codelines;
    int memoryIdx = 0;

    string s = "66";
    cout << hex2dec(s) << endl;


    ifstream infile(argv[1]);
    string line = "";
    while(getline(infile, line)){
        lines.push_back(line);
        /*if(label){
            labels.insert(line, lines.size()-1);
        }
         else if(variable){
            var.insert(line, lines.size()-1);
        */

        //burada biri label için diğeri variable için olmak üzere 2 tane map yap
        //key olarak string alsın value olarak da lines arrayinin indexini alsın
    }

    bool cont = false;
    //BURASI PROGRAMIN LABEL VE VARİABLE OKUDUĞU YER

    //TODO: BAŞTAKİ VE SONDAKİ FAZLA BOŞLUKLARI SİLECEK KOD YAZ!!

    for(int i = 0; i < lines.size(); i++){
        for(int j = 0; j < lines[i].size(); j++){
            if(lines[i].at(j) == ','){
                lines[i][j] = ' ';
            }
        }
    }


    for(int i = 0; i < lines.size(); i++){
        if(lines[i] == "code segment"){
            cont = true;
            continue;
        }
        else if(lines[i] == "code ends"){
            cont = false;
        }
        if(cont){
            codelines.push_back(lines[i]);
        }
        cout << lines[i] << endl;
    }

    int i = 0;
    while(codelines[i] != "int 20h") {
        if(codelines[i].substr(0,3) == "mov"){
            memoryIdx += 6;
        }
        else if(codelines[i].substr(0,3) == "add"){
            memoryIdx += 6;
        }
        else if(codelines[i].substr(0,3) == "dec"){
            memoryIdx += 6;
        }
        else if(codelines[i].substr(0,3) == "inc"){
            memoryIdx += 6;
        }
        else if(codelines[i].substr(0,3) == "add"){
            memoryIdx += 6;
        }
        else if(codelines[i].substr(0,3) == "jnz"){
            memoryIdx += 6;
        }
        else if(codelines[i] == "int 21h"){
            memoryIdx += 6;
        }
        else{
            i++;
            continue;
        }
        i++;
    }
    memoryIdx += 6;

    for(int j = i+1; j < codelines.size(); j++){
        string tmp = codelines[j];
        string var;
        string type;
        string info;
        stringstream check1(tmp);
        getline(check1, var, ' ');
        getline(check1, type, ' ');
        getline(check1, info, ' ');
        vars.insert({var, memoryIdx});
        if(type == "db"){
            info = info.substr(0, info.size()-1);
            unsigned char data = hex2dec(info);
            vars.insert(pair<string, int> (var + "1", memoryIdx));
            memory[memoryIdx] = data;
            vars.insert({var, memoryIdx});
            memoryIdx++;
        }else if(type == "dw"){
            info = info.substr(0, info.size()-1);
            unsigned char data1 = hex2dec(info.substr(0,2));
            unsigned char data2 = hex2dec(info.substr(2,2));
            vars.insert(pair<string, int> (var + "2", memoryIdx));
            memory[memoryIdx] = data1;
            memory[memoryIdx+1] = data2;
            vars.insert({var, memoryIdx});
            memoryIdx+=2;
        }else{
            cout << "error" << endl;
        }
        //memorye ASCII kodu atılacak
        //Scanner olarak al
    }

    //ASIL KOD BURADAN BAŞLIYOR
    for(int i = 0; i < codelines.size(); i++){
        string tmp = codelines[i];
        string type;
        string first;
        string sec;
        string var = "";
        stringstream check1(tmp);
        getline(check1, type, ' ');
        if(type == "mov") {
            getline(check1, first, ' ');
            getline(check1, sec, ' ');
            getline(check1, var, ' ');
            if(var != ""){
                //OFFSET İSE
                map<string, int>::iterator it ;
                it = vars.find(var);
                if(it == vars.end())
                    cout << "Key-value pair not present in map" << endl;
                else{
                    if(first == "ax"){
                        mov_reg_offset(pax, it->second);
                    }
                    else if(first == "bx"){
                        mov_reg_offset(pbx, it->second);
                    }
                    else if(first == "cx"){
                        mov_reg_offset(pcx, it->second);
                    }
                }
            }else{
                if(first == "ax"){
                    string s = sec.substr(0,sec.size()-1);
                    if(sec.at(sec.size()-1) == 'h'){
                        mov_reg_hex(pax, hex2dec(s));
                    }
                    else if(vars.count(sec)){
                        map<string, int>::iterator it ;
                        it = vars.find(sec);
                        if(it != vars.end())
                            mov_reg_hex(pax, memory[it->second]);
                    }
                    else{
                        if(sec == "bx"){
                            mov_reg_reg(pax, pbx);
                        }else if(sec == "cx"){
                            mov_reg_reg(pax, pcx);
                        }
                            //SADECE SAYI İSE
                        else{
                            mov_reg_dec(pax, stoi(sec));
                        }
                    }
                }
                else if(first == "bx"){
                }
            }

            //VAR IN MEMORYDEKİ ADRESİNİ DÖNMESİ GEREK
        }
        else if(type == "add"){
            getline(check1, first, ' ');
            getline(check1, sec, ' ');
            unsigned char c = hex2dec(sec.substr(0,sec.size()-1));
            add_reg(first.c_str(),c);
        }
        else if(type == "inc"){
            getline(check1, first, ' ');
            inc_reg(first.c_str());
        }
        else if(type == "dec"){
            getline(check1, first, ' ');
            dec_reg(first.c_str());
        }
    }
    ax = 3 ;
    bx = 4 ;

    // 1. Read instruction lines
    // 2. Place  dw and db data  and compute addresses
    // 3.  PC = line 0
    // while (PC !=  int20line) {
    //     getline(PC)
    //     parse instruction in the line  to extract opcode and operands
    //     if (strcmp(opcode,"ADD) {
    //        .... determine operands and call appropriate add function
    //     }
    //     else if (strcmp(opcode,"MOV") ) {
    //         .. determine operands call  appropriate mov function
    //print_hex(*pah) ;
    //print_hex(*pal) ;
    //add_reg(pax, 8);
    //sub_reg(pbx, 2);
    //mul_reg(pbx);
    //div_reg(pbx);
    //mov_reg_reg(pax,pbx) ;
    //*pax = 50001 ;
    //     }
    //
    // }

    //print_16bitregs() ;
    print_hex(*pah) ;
    print_hex(*pal) ;
    add_reg(pah, 5);
    //not_reg(pah);
    xor_reg_reg(pal, pah);
    //mov_reg_reg(pbx, pax);
    print_bits(*pal);
    print_bits(*pah);
    //print_bits(*pax);
}

template <class regtype>
void mov_reg_reg(regtype *preg1, regtype *preg2)
{
    *preg1 = *preg2 ;
}

template <class regtype>
void mov_reg_hex(regtype *preg1,unsigned char c)
{
    *preg1 = c ;
}

template <class regtype>
void mov_reg_dec(regtype *preg1, int a)
{
    *preg1 = a ;
}

template <class regtype>
void mov_reg_offset(regtype *preg1, int a)
{
    *preg1 = a ;
}

template  <class regtype>
void add_reg(regtype *preg, unsigned char c)
{
    preg = preg + c;
}

template  <class regtype>
void sub_reg(regtype *preg, unsigned char c)
{
    *preg = *preg - c;
}

template <class regtype>  void inc_reg(regtype *preg){
    *preg ++;
}

template <class regtype>  void dec_reg(regtype *preg){
    *preg --;
}

template  <class regtype>
void mul_reg(regtype *preg)
{
    ax = ax * (*preg);
}

template  <class regtype>
void div_reg(regtype *preg)
{
    *pal = ax / (*preg);
    *pah = ax % (*preg);
}
template  <class regtype>
void or_reg_reg(regtype *preg1, regtype *preg2)
{
    *preg1 = (*preg1 | *preg2);
}

template  <class regtype>
void and_reg_reg(regtype *preg1, regtype *preg2)
{
    *preg1 = (*preg1 & *preg2);
}

template  <class regtype>
void xor_reg_reg(regtype *preg1, regtype *preg2)
{
    *preg1 = ((~(*preg1) & (*preg2)) | (~(*preg2) & (*preg1)));
}

template  <class regtype>
void not_reg(regtype *preg)
{
    *preg = ~(*preg);
}

template <class datatype>
void print_bits(datatype x)
{
    int i;

    for(i=8*sizeof(x)-1; i>=0; i--) {
        (x & (1 << i)) ? putchar('1') : putchar('0');
    }
    cout << "" << endl;
}

template <class datatype>
void print_hex(datatype x)
{
    if (sizeof(x) == 1)
        printf("%02x\n",x);
    else
        printf("%04x\n",x);
}

unsigned char hex2dec(string hex)
{
    unsigned char result = 0;
    for (int i=0; i<hex.length(); i++) {
        if (hex[i]>=48 && hex[i]<=57){
            result += (hex[i]-48)*pow(16,hex.length()-i-1);
        } else if (hex[i]>=65 && hex[i]<=70) {
            result += (hex[i]-55)*pow(16,hex.length( )-i-1);
        } else if (hex[i]>=97 && hex[i]<=102) {
            result += (hex[i]-87)*pow(16,hex.length()-i-1);
        }
    }
    return result;
}

void print_16bitregs()
{
    printf("AX:%04x\n",ax);
    printf("BX:%04x\n",bx);
    printf("CX:%04x\n",cx);
    printf("DX:%04x\n",dx);
    printf("BP:%04x\n",bp);
    printf("SP:%04x\n",sp);
    printf("SI:%04x\n",si);
    printf("DI:%04x\n",di);
}

