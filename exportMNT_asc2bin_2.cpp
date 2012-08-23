#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//#include "./SkelGIS/skelgis.h"
//#include "./mem/mem.cpp"
using namespace std;

struct HEADER{
    unsigned int width;
    unsigned int height;
    float x;
    float y;
    float spacing;
    float nodata;
};

float ** readDEM(HEADER &head, const char * fileIn){
    float ** tab2D = NULL;
    ifstream fin(fileIn, ios::in);
    // ofstream fout(fileOut, ios::binary | ios::out);

    if(fin.is_open()){

        for(int i(0); i<6; i++){
            char c('a');
            char buffer[256];
            float f;
            while((c < '0') || (c > '9')){
                c = fin.get();
                c = fin.peek();
            }
            fin.getline(buffer, 256);
            switch(i){
            case 0:
                head.width = atof(buffer);
                break;
            case 1:
                head.height = atof(buffer);
                break;
            case 2:
                head.x = atof(buffer);
                break;
            case 3:
                head.y = atof(buffer);
                break;
            case 4:
                head.spacing = atof(buffer);
                break;
            case 5:
                head.nodata = -9999;
                break;
            }

        }

        //    head.width = 5068;
        //    head.height = 2913;
        //    head.x = 92250;
        //    head.y = 2235525;
        //    head.spacing = 75;
        //    head.nodata = -9999;

        //    head.width = 9;
        //    head.height = 10;
        //    head.x = 92250;
        //    head.y = 2235525;
        //    head.spacing = 75;
        //    head.nodata = -9999;

        //    for(int i(0); i<6; i++){
        ////        getline(fin, line);

        //        getline(fin, line);
        //        istringstream buffer_width(line);
        //        buffer_width.seekg(14, ios::cur);
        //        buffer_width>>head.width;
        //    }

        cout<<endl;
        cout<<"Width : "<<head.width<<endl;
        cout<<"Height : "<<head.height<<endl;
        //cout<<"x position : "<<head.x<<endl;
        printf("x position : %f\n", head.x);
        //cout<<"y position : "<<head.y<<endl;
        printf("y position : %f\n", head.y);
        cout<<"spacing : "<<head.spacing<<endl;
        cout<<"NODATA_value : "<<head.nodata<<endl;

        tab2D = new float*[head.height];

        for(int i(0); i<head.height; i++)
            tab2D[i] = new float[head.width];

        cout<<endl<<"Loading DEM..."<<endl;

        for(int i(0); i<head.height; i++){
            for(int j(0); j<head.width; j++){
                fin>>tab2D[i][j];
            }
        }
        //    cout<<endl;
        //    for(int i(0); i<head.height; i++){
        //        for(int j(0); j<head.width; j++){
        //            printf("%f ", tab2D[i][j]);
        //        }
        //        cout<<endl;
        //    }
        return tab2D;
    }else{
        cout<<endl<<"ERROR : impossible to open "<<fileIn<<endl<<endl;
        exit(1);
    }

}

void exportDEM_BIN(const HEADER head, const float *const* Z, char * fileIn, char * fileOut){

    ofstream fout(fileOut, ios::binary | ios::out);


    cout<<endl;
    cout<<"Exporting "<<fileIn<<" to "<<fileOut<<" in binary format..."<<endl;
    cout<<"Rows : "<<head.height<<endl;
    cout<<"Cols : "<<head.width<<endl;
    cout<<endl<<"0%...";

    fout.write((char*)(&head),sizeof(HEADER));
    for(int i(0); i<head.height; i++){
        if(i == head.height/2)
            cout<<"50%...";
        else if(i == head.height/4)
            cout<<"25%...";
        else if(i == 3*(head.height/4))
            cout<<"75%...";
        fout.write((char*)(Z[i]),sizeof(float)*head.width);
    }
}

void deleteTab2D(const HEADER &head, float ** &tab2D){
    if(tab2D != NULL){
        for(int i(0); i<head.height; i++){
            if(tab2D[i] != NULL)
                delete tab2D[i];
            else cout<<endl<<"tab2D["<<i<<"] -> NULL"<<endl;
        }
        delete [] tab2D;
    }
    else cout<<"\ntab2D -> NULL"<<endl;
}

void abort(char* prog_name){
    cout<<endl<<endl;
    cout<<prog_name<<" has exit unexpectedly !"<<endl<<endl;
    exit(1);
}

int main(int argc, char** argv){

    char * fileIn = NULL;
    char * fileOut = NULL;

    HEADER head;

    float ** Z=NULL;
    float ** W=NULL;
    float ** WS=NULL;

    int options;
    bool opt_arg_i(false); //input file
    bool opt_arg_o(false); //output file .bin
    while((options=getopt(argc,argv,"i:o:"))!=-1){
        switch(options){
        case 'i':
            opt_arg_i = true;
            fileIn = optarg;
            break;
        case 'o':
            opt_arg_o = true;
            fileOut = optarg;
            break;
        default:
            abort(argv[0]);
        }
    }

    if(opt_arg_i){
        Z = readDEM(head, fileIn);
        if(opt_arg_o){
            exportDEM_BIN(head, Z, fileIn, fileOut);
        }
        deleteTab2D(head, Z);
    }else
        abort(argv[0]);

    cout<<endl<<"Export Done."<<endl<<endl;
    return 0;
}
