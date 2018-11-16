#include <iostream>
#include <fstream>
#include <vector>
#include "string.h"
#include <math.h>

using namespace std;

bool gcv(int argc, char* params[], const char param[], string &result){
    for(int i = 1; i != argc; i++){
        if(string(params[i]) == string(param)){
            if(params[i + 1]) {
                result = params[i + 1];
            }else{
                result = "";
            }
            return true;
        }
    }
    return false;
}

string getFormat(string val){
    if(val == "HEX"){
        return "%X";
    }else if(val == "hex"){
        return "%x";
    }else if(val == "oct"){
        return "%o";
    }else if(val == "DEC"){
        return "%u";
    }else if(val == "TAB"){
        return "\t";
    }

    return val;
}

int main(int argc, char* argv[])
{
    //анализ входных аргументов
    string val, sign0, sign1, help = "Example:hf -f filename.txt -o map\n"
                       "[-f] - file name\n"
                       "[-v] - searching value of [0..255]\n"
                       "[-from] - searching position in file, default file <BEGIN>\n"
                       "[-count] - number of sirchingchars, default file <END> \n"
                       "[-o] - output like:\n"
                       "\tmap - standart map(default)\n"
                       "\tfilesize - show file size\n"
                       "\tvalcount - show value number\n"
                       "\tnvalcount - show not value number\n"
                       "\tvalpercent - show value percent\n"
                       "\tnvalcount - show not value percent\n"
                       "\tgridcount - count grid rows\n"
                       "[-digfromat - [DEC | HEX | hex | oct | CHAR | TAB] - format of fonded value\n"
                       "[-nulfromat - format of value if not found\n"
                       "[-delimiter] - TAB default \n"
                       "[-grid] - do return every N simbols \n"
                       "[-fq] - replace each number with fqformat\n"
                       "[-fqlength] - length of fq\n"
                       "[-fqoffset] - frequency offset\n"
                       "[-fqformat = DEC | HEX | hex | oct | CHAR] - format if fq == Val\n"
                       "[-fqnformat = DEC | HEX | hex | oct | CHAR] - format if fq != Val\n";


    if(gcv(argc, argv, "--help", val)){
        printf(help.c_str());
        return 0;
    }

    if(!gcv(argc, argv, "-f", val)){
        printf(help.c_str());
    }

    ifstream hfile(val, ifstream::in);
    if(hfile.fail()){
        printf("File not found: \"%s\"\n", val.c_str());
        return 0;
    }

    vector<char> fdata;
    fdata.assign(istreambuf_iterator<char>(hfile), istreambuf_iterator<char>());

    string output = "map";
    if(!gcv(argc, argv, "-o", val)){
        printf(help.c_str());
        return 0;
    }else{
        output = val;
    }

    unsigned char c;
    int i, from, to, cnt;

    if(gcv(argc, argv, "-from", val)){
        from = stoi(val);
    }else{
        from = 0;
    }

    if(gcv(argc, argv, "-count", val)){        
        to = from + stoi(val);
        to = to > fdata.size() ?  fdata.size() : to;
    }else{
        to = fdata.size();
    }

    cnt = to - from;

    unsigned char value = 0;
    if(gcv(argc, argv, "-val", val)){
        value = static_cast<unsigned char>(stoi(string(val).c_str()));
    }

    //output format for main Chars
    string digformat = "%u";
    if(gcv(argc, argv, "-digformat", val)){
        digformat = getFormat(val);
    }

    string nulformat = "%u";
    if(gcv(argc, argv, "-nulformat", val)){
       nulformat = getFormat(val);
    }

    string delimiter = "\t";
    if(gcv(argc, argv, "-delimiter", val)){
        delimiter = getFormat(val);
    }

    int grid = 0, g = 0;
    if(gcv(argc, argv, "-grid", val)){
        grid = stoi(val);
    }

    //frequency evaluation
    int fq = 0, fqoffset = 0, fqlength = 0;
    if(gcv(argc, argv, "-fq", val)){
        if(val != ""){
            fq = stoi(string(val).c_str());
        }
    }
    if(gcv(argc, argv, "-fqoffset", val)){
        if(val != ""){
            fqoffset = stoi(string(val).c_str());
        }
    }
    if(gcv(argc, argv, "-fqlength", val)){
        if(val != ""){
            fqlength = stoi(string(val).c_str()) - 1;
            if(fqlength > fq - 1){
                fqlength = fq - 1;
            }
        }
    }
    string fqformat = " ";
    if(gcv(argc, argv, "-fqformat", val)){
         fqformat = getFormat(val);
    }

    string fqnformat = "*";
    if(gcv(argc, argv, "-fqnformat", val)){
         fqnformat = getFormat(val);
    }

    int k = 0;

    if(output == "filesize"){
         printf("%i\n", fdata.size());
         return 0;
    }
    if(output == "valcount"){
        int count = 0;
        for(int i = from; i != to; i++){
            //get char
            c = static_cast<unsigned char>(fdata.at(i));
            if(c == value){
                count++;
            }
        }
        printf("%i\n", count);
        return 0;
    }
    if(output == "nvalcount"){
        int count = 0;
        for(int i = from; i != to; i++){
            //get char
            c = static_cast<unsigned char>(fdata.at(i));
            if(c != value){
                count++;
            }
        }
        printf("%i\n", count);
        return 0;
    }
    if(output == "valpercent"){
        double result = 0;
        int count = 0;
        for(int i = from; i != to; i++){
            //get char
            c = static_cast<unsigned char>(fdata.at(i));
            if(c == value){
                count++;
            }
        }
        printf("%i/", count);
        printf("%i ", to);
        result = count;
        result /= to;
        result *=100;
        result =floor(result);
        printf("%f%%\n", result);
        return 0;
    }
    if(output == "nvalpercent"){
        double result = 0;
        int count = 0;
        for(int i = from; i != to; i++){
            //get char
            c = static_cast<unsigned char>(fdata.at(i));
            if(c != value){
                count++;
            }
        }
        printf("%i/", count);
        printf("%i ", to);
        result = count;
        result /= to;
        result *=100;
        result =floor(result);
        printf("%f%%\n", result);
        return 0;
    }

    if(output == "gridcount"){
        double result = (double)cnt / (double)max(grid, 1);
        printf("%f\n", result);
        return 0;
    }

    if(output == "map"){
        for(int i = from; i != to; i++){
            //get char
            c = static_cast<unsigned char>(fdata.at(i));

            //for each chars do
            if((fq > 0 && (i + fqoffset) % fq == 0) || (k > 0)){
                if(fqlength > 0 && (i + fqoffset) % fq == 0){
                    k = fqlength + 1;
                }
                if(c == value){
                    printf(fqformat.c_str(), c);
                }else{
                    printf(fqnformat.c_str(), c);
                }
                k--;
            }else if(c == value){
                printf(digformat.c_str(), c);
            }else{
                printf(nulformat.c_str(), c);
            }
            printf("%s", delimiter.c_str());
            if(grid > 0 && g == grid-1){
                printf("\n");
                g = 0;
            }else{
                g++;
            }
        }
    }

    printf("\n");
    return 0;

}
