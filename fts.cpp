#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "chronometer.hpp"
//#include "./mem/mem.cpp"
//#include "./SkelGIS/skelgis.h"
using namespace std;

#define stackOverFlawLimit 20000
float hugeNumber(10000);
const float eps(0.00000001);
int depth(0);
int nbmodif(0);


struct HEADER{
    unsigned int width;
    unsigned int height;
    float x;
    float y;
    float spacing;
    float nodata;
};

#include "neighbour.h"

struct HEADER_ASC{
    unsigned int ncols;
    unsigned int nrows;
    float xllcorner;
    float yllcorner;
    float cellsize;
    float nodata;
    // ncols 5068
    // nrows 2913
    // xllcorner 92250
    // yllcorner 2235525
    // cellsize 75.000000
    // NODATA_value -9999
};

struct HEADER_BIN{
    float cols;
    float rows;
    float west;
    float south;
    float north;
    float east;
};

struct HEADER_MAT{
    int type;
    int rows;
    int cols;
    int unUsed;
    int namelength;
    char name;
};


// struct HEADER_ESRI{
//   int cols;
//   int rows;
//   cellsize 0.050401
//   xllcorner -130.128639
//   yllcorner 20.166799
//   nodata_value 9999.000000
//   nbits 32
//   pixeltype float
//   byteorder msbfirst
// };

bool withinMatrix(const HEADER head, int i, int j){
    if(i >= 0 && i < head.height && j >= 0 && j < head.width) return true;
    else return false;
}

int maxDEM(const HEADER head, float *const* Z){
    int max(0);
    for(int i(0); i<head.height; i++)
        for(int j(0); j<head.width; j++)
            if(Z[i][j] > max)
                max = Z[i][j];
    return max;
}

bool isPixelBorder(const HEADER &head, const float *const* tab2D, int i, int j){
    if(i == 0 || j == 0){
        return true;
    }else if(i == (head.height-1)){
        return true;
    }else if(j == (head.width-1)){
        return true;
    }else return false;
}

float lowerNeighbour(const HEADER &head, const float *const* tab2D, int i, int j){
    float lowerNb(tab2D[i][j]);
    if(tab2D[i-1][j] <= lowerNb) lowerNb = tab2D[i-1][j];
    if(tab2D[i-1][j+1] <= lowerNb) lowerNb = tab2D[i-1][j+1];
    if(tab2D[i][j+1] <= lowerNb) lowerNb = tab2D[i][j+1];
    if(tab2D[i+1][j+1] <= lowerNb) lowerNb = tab2D[i+1][j+1];
    if(tab2D[i+1][j] <= lowerNb) lowerNb = tab2D[i+1][j];
    if(tab2D[i+1][j-1] <= lowerNb) lowerNb = tab2D[i+1][j-1];
    if(tab2D[i][j-1] <= lowerNb) lowerNb = tab2D[i][j-1];
    if(tab2D[i-1][j-1] <= lowerNb) lowerNb = tab2D[i-1][j-1];
    return lowerNb;
}

bool isSink(const HEADER &head, const float *const* tab2D, int i, int j){
    if(tab2D[i][j] <= lowerNeighbour(head, tab2D, i, j)){
        return true;
    }else return false;
}

float sinksCatchmentArea(const HEADER &head, float ** tab2D){
    for(int i(0); i<head.height; i++){
        for(int j(0); j<head.width; j++){
        }
    }
}

bool isCatchmentArea(const HEADER &head, float ** tab2D, int i, int j){

}

void printTab2DSinksAndBorders(HEADER &head, float ** tab2D){
    for(int i(0); i<head.height; i++){
        for(int j(0); j<head.width; j++){
            if(isPixelBorder(head, tab2D, i, j))
                cout<<tab2D[i][j]<<" ";
            else if(isSink(head, tab2D, i, j))
                cout<<tab2D[i][j]<<" ";
            else cout<<"  ";
        }
        cout<<endl;
    }
    cout<<endl;
}
float ** onlySinksDEM(const HEADER head, float ** Z){
    float ** W = NULL;
    W = new float*[head.height];
    for(int i(0); i<head.height; i++)
        W[i] = new float[head.width];

    for(int i(0); i<head.height; i++){
        for(int j(0); j<head.width; j++){
            if(isPixelBorder(head, Z, i, j))
                W[i][j] = Z[i][j];
            else if(isSink(head, Z, i, j))
                W[i][j] = Z[i][j];
            else W[i][j] = 10;
        }
    }

    return W;
}

void printTab2DtestBorder(const HEADER &head, const float *const* tab2D){
    for(int i(0); i<head.height; i++){
        for(int j(0); j<head.width; j++){
            if(isPixelBorder(head, tab2D, i, j)){
                cout<<tab2D[i][j]<<" ";
            }else cout<<"  ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void printMatrix(const HEADER &head, float ** tab2D){
    cout<<endl;
    if(tab2D != NULL){
        for(int i(0); i<head.height; i++){
            if(tab2D[i] != NULL)
                for(int j(0); j<head.width; j++){
                    cout.width(3);
                    cout<<tab2D[i][j];
                }
            cout<<endl;
        }
    }
    cout<<endl;
}
void printTab2D(const HEADER &head, float ** tab2D){
    cout<<endl<<"   ";
    for(int i(0); i<head.width; i++){
        cout.width(3);
        cout<<i;
    }
    cout<<endl<<endl;
    if(tab2D != NULL){
        for(int i(0); i<head.height; i++){
            cout<<i<<"  ";
            if(tab2D[i] != NULL)
                for(int j(0); j<head.width; j++){
                    cout.width(3);
                    cout<<tab2D[i][j];
                }
            cout<<endl;
        }
    }
    cout<<endl;
}

void writeMatrixTest(char * file){
    HEADER head_perso;
    head_perso.width = 2;
    head_perso.height = 2;
    head_perso.x = 0;
    head_perso.y = 0;
    head_perso.spacing = 0;
    head_perso.nodata = -9999;

    float tabTest[]={2,2,4,5,6,6,8,6,5,
                     3,3,5,5,5,5,7,6,4,
                     5,5,4,5,4,4,7,8,6,
                     4,4,4,3,3,3,5,7,8,
                     2,3,3,3,2,3,4,5,6,
                     4,4,3,2,2,3,4,7,7,
                     3,4,4,2,2,3,6,8,9,
                     2,3,4,4,4,4,5,9,7,
                     2,2,3,4,4,5,6,8,7,
                     1,2,3,4,5,6,7,8,9};

    float tabTestStrahler[]={0,0,0,1,0,1,0,0,
                             0,0,0,1,0,1,0,0,
                             0,0,0,0,2,0,0,0,
                             0,0,0,0,2,1,0,0,
                             0,0,0,0,2,0,2,0,
                             0,0,0,0,0,3,0,0,
                             0,0,0,0,0,3,0,0,
                             0,0,0,0,0,0,3,3,
                             0,0,0,0,0,2,0,0,
                             0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,};
    int cpt(0);
    float ** tab2D = NULL;

    tab2D = new float*[head_perso.height];
    for(int i(0); i<head_perso.height; i++)
        tab2D[i] = new float[head_perso.width];

    for(int i(0); i<head_perso.height; i++){
        for(int j(0); j<head_perso.width; j++){
            tab2D[i][j] = tabTest[cpt];
            cpt++;
        }
    }

    ofstream fout(file, ios::binary | ios::out);
    fout.write((char*)(&head_perso),sizeof(HEADER));
    for(int i(0); i<head_perso.height; i++){
        fout.write((char*)(tab2D[i]),sizeof(float)*head_perso.width);
    }
    if(tab2D != NULL){
        for(int i(0); i<head_perso.height; i++){
            if(tab2D[i] != NULL)
                delete tab2D[i];
            else cout<<endl<<"tab2D["<<i<<"] -> NULL"<<endl;
        }
        delete [] tab2D;
    }
    else cout<<"\ntab2D -> NULL"<<endl;
    fout.close();
}

float ** subDEM(const HEADER head, const float *const* Z){
    float ** W = NULL;

    cout<<endl<<"DEM immersion..."<<endl;

    W = new float*[head.height];

    for(int i(0); i<head.height; i++)
        W[i] = new float[head.width];

    for(int i(0); i<head.height; i++)
        for(int j(0); j<head.width; j++){
            if(isPixelBorder(head, Z, i, j))
                W[i][j] = Z[i][j];
            else
                W[i][j] = hugeNumber;
        }
    return W;
}

float ** streamBurning(const HEADER head,  const float *const* Z, const float *const* S, int coeff=1){
    float ** W = NULL;
    cout<<endl<<"Stream Burning..."<<endl;

    W = new float*[head.height];
    for(int i(0); i<head.height; i++)
        W[i] = new float[head.width];

    for(int i(0); i<head.height; i++)
        for(int j(0); j<head.width; j++){
            W[i][j] = Z[i][j] - (coeff*S[i][j]);
        }

    return W;
}
float ** streamBurning(const HEADER head,  const float *const* Z, const float *const* S, const float * tabCoeff){
    float ** W = NULL;
    cout<<endl<<"Stream Burning..."<<endl;

    W = new float*[head.height];
    for(int i(0); i<head.height; i++)
        W[i] = new float[head.width];

    for(int i(0); i<head.height; i++)
        for(int j(0); j<head.width; j++){
            W[i][j] = Z[i][j] - (tabCoeff[(int)(S[i][j])]);
        }

    return W;
}


void dryCell(const HEADER head, float ** Z, float ** &W, int x, int y){
    for(int i(x-1); i<=(x+1); i++){
        for(int j(y-1); j<=(y+1); j++){
            if(withinMatrix(head, i, j) && (i != x && j != y)){
                if(W[i][j] == hugeNumber){
                    if(Z[i][j] >= (W[x][y] + eps)){//(Z[x][y],W[i][j]))){
                        W[i][j] = Z[i][j];
                        nbmodif++;
                        dryCell(head, Z, W, i, j);
                    }
                }
            }
        }
    }
}


void fillTheSinks(const HEADER head, float ** Z, float ** &W){

    // Time::Chronometer chrono;
    // chrono.start();

    cout<<endl<<"Filling the sinks : "<<endl;

    cout<<"          Border processing..."<<endl;


    for(int i(0); i<head.height; i++){
        dryCell(head, Z, W, i, 0);
    }
    depth = 0;
    cout<<"nbmodif="<<nbmodif<<endl;
    for(int j(0); j<head.width; j++){
        dryCell(head, Z, W, 0, j);
    }
    depth = 0;
    cout<<"nbmodif="<<nbmodif<<endl;
    for(int i(head.height-1); i>=0; --i){
        dryCell(head, Z, W, i, head.width-1);
    }
    depth = 0;
    cout<<"nbmodif="<<nbmodif<<endl;
    for(int j(head.width-1); j>=0; --j){
        dryCell(head, Z, W, head.height-1, j);
    }
    cout<<"nbmodif="<<nbmodif<<endl;

    cout<<"          Filling matrix process."<<endl;

    bool modif(false);
    cout<<"nbmodif="<<nbmodif<<endl;

    do{
        nbmodif=0;
        modif = false;
        for(int x(0); x<head.height; ++x){
            for(int y(0); y<head.width; ++y){
                if(W[x][y] > Z[x][y]){
                    for(int i(x-1); i<=(x+1); i++){
                        for(int j(y-1); j<=(y+1); j++){
                            if(withinMatrix(head, i, j)){// && (i != x && j != y)){
                                if(Z[x][y] >= (W[i][j] + eps)){//(Z[x][y],W[i][j]))){
                                    W[x][y] = Z[x][y];
                                    modif = true;
                                    nbmodif++;
                                    //dryCell(head, Z, W, x, y);
                                }else
                                    if(W[x][y] > W[i][j] + eps){//(Z[x][y], W[i][j])){
                                        W[x][y] = W[i][j] + eps;//(Z[x][y], W[i][j]);
                                        modif = true;
                                        nbmodif++;
                                    }
                            }
                        }
                    }
                }
            }
        }
        cout<<"nbmodif="<<nbmodif<<endl;
    }while(modif);

    // chrono.stop();


    // do{
    //     depth = 0;
    //     nbmodif=0;
    //     modif = false;
    //     for(int x(head.height-1); x >=0; --x){
    //       for(int y(head.width-1); y >=0; --y){
    // 	if(W[x][y] > Z[x][y]){
    // 	  for(int i(x-1); i<=(x+1); i++){
    // 	    for(int j(y-1); j<=(y+1); j++){
    // 	      if(withinMatrix(head, i, j) && (i != x && j != y)){
    // 		if(Z[x][y] >= (W[i][j] + eps(Z[x][y],W[i][j]))){
    // 		  W[x][y] = Z[x][y];
    // 		  modif = true;
    // 		  nbmodif++;
    // 		  //dryCell(head, Z, W, x, y);
    // 		}else
    // 		  if(W[x][y] > W[i][j] + eps(Z[x][y], W[i][j])){
    // 		    W[x][y] = W[i][j] + eps(Z[x][y], W[i][j]);
    // 		    modif = true;
    // 		    nbmodif++;
    // 		  }
    // 	      }
    // 	    }
    // 	  }
    // 	}
    //       }
    //     }
    //     cout<<"nbmodif="<<nbmodif<<endl;
    //   }while(modif);

    cout<<"100%"<<endl;

    //cout<<endl<<"Time : "<<chrono<<endl;
}

float ** directions(const HEADER head, const float *const* Z){
    float ** W = NULL;
    W = new float*[head.height];
    for(int i(0); i<head.height; i++)
        W[i] = new float[head.width];

    for(int i(0); i<head.height; i++)
        for(int j(0); j<head.width; j++){

            neighbour nb(head, Z, i, j);
            W[i][j] = nb.id_min;
            nb;
        }

    return W;
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

float ** readDEM(HEADER &head, const char * file){
    float ** tab2D = NULL;
    ifstream f(file, ios::binary | ios::in);
    f.read((char*)(&head),sizeof(HEADER));

    cout<<endl;
    cout<<"Width : "<<head.width<<endl;
    cout<<"Height : "<<head.height<<endl;
    //    cout<<"x position : "<<head.x<<endl;
    //    cout<<"y position : "<<head.y<<endl;
    printf("x position : %f\n", head.x);
    printf("y position : %f\n", head.y);
    cout<<"spacing : "<<head.spacing<<endl;
    cout<<"nodata : "<<head.nodata<<endl;

    tab2D = new float*[head.height];

    for(int i(0); i<head.height; i++)
        tab2D[i] = new float[head.width];

    cout<<endl<<"Loading DEM..."<<endl;

    f.seekg(sizeof(HEADER),ios::beg);
    for(int i(0); i<head.height; i++){
        f.read((char*)(tab2D[i]),sizeof(float)*head.width);
    }
    return tab2D;
}

void readHeader(const char * file){
    ifstream f(file, ios::binary | ios::in);
    HEADER head;
    f.read((char*)(&head),sizeof(HEADER));

    cout<<endl;
    cout<<"Width : "<<head.width<<endl;
    cout<<"Height : "<<head.height<<endl;
    //    cout<<"x position : "<<head.x<<endl;
    //    cout<<"y position : "<<head.y<<endl;
    printf("x position : %f\n", head.x);
    printf("y position : %f\n", head.y);
    cout<<"spacing : "<<head.spacing<<endl;
    cout<<"nodata : "<<head.nodata<<endl;
}

void exportDEM_MAT(const HEADER head_MNT, const float *const* Z, char * file_mat){
    HEADER_MAT head_mat;

    ofstream fout(file_mat, ios::binary | ios::out);

    head_mat.type = 10;
    head_mat.rows = head_MNT.height;
    head_mat.cols = head_MNT.width;
    head_mat.unUsed = 0;
    head_mat.namelength = 1;
    head_mat.name = 0;

    cout<<endl;
    cout<<"Exporting results to "<<file_mat<<" in binary format..."<<endl;
    cout<<"Type : "<<head_mat.type<<endl;
    cout<<"Rows : "<<head_mat.rows<<endl;
    cout<<"Cols : "<<head_mat.cols<<endl;
    cout<<endl;

    fout.write((char*)(&head_mat),sizeof(HEADER_MAT));
    for(int i(0); i<head_MNT.height; i++){
        fout.write((char*)(Z[i]),sizeof(float)*head_MNT.width);
    }
}

void exportDEM_BIN(const HEADER head, const float *const* Z, char * fileOut){
    ofstream fout(fileOut, ios::binary | ios::out);

    cout<<endl<<"Exporting results to "<<fileOut<<" in binary format..."<<endl;

    fout.write((char*)(&head),sizeof(HEADER));
    for(int i(0); i<head.height; i++){
        fout.write((char*)(Z[i]),sizeof(float)*head.width);
    }
    fout.close();
}

void exportDEM_ASC(const HEADER head, float ** Z, char * file_asc){
    ofstream fout(file_asc);
    HEADER_ASC head_ASC;

    head_ASC.nrows = head.height;
    head_ASC.ncols = head.width;
    head_ASC.xllcorner = head.x;
    head_ASC.yllcorner = head.y;
    head_ASC.cellsize = head.spacing;
    head_ASC.nodata = head.nodata;

    fout<<"ncols "<<head_ASC.ncols<<endl;
    fout<<"nrows "<<head_ASC.nrows<<endl;
    fout<<"xllcorner "<<head_ASC.xllcorner<<endl;
    fout<<"yllcorner "<<head_ASC.yllcorner<<endl;
    fout<<"cellsize "<<head_ASC.cellsize<<endl;
    fout<<"NODATA_value "<<head_ASC.nodata<<endl;

    cout<<endl;
    cout<<"Exporting results to "<<file_asc<<" in ASCII format..."<<endl;
    cout<<"ncols "<<head_ASC.ncols<<endl;
    cout<<"nrows "<<head_ASC.nrows<<endl;
    //cout<<"xllcorner "<<head_ASC.xllcorner<<endl;
    printf("xllcorner %f",head_ASC.xllcorner);
    cout<<endl;
    printf("yllcorner %f",head_ASC.yllcorner);
    cout<<endl;
    //cout<<"yllcorner "<<head_ASC.yllcorner<<endl;
    cout<<"cellsize "<<head_ASC.cellsize<<endl;
    cout<<"NODATA_value "<<head_ASC.nodata<<endl;

    for(int i(0); i<head_ASC.nrows; i++){
        for(int j(0); j<head_ASC.ncols; j++){
            fout<<Z[i][j]<<" ";
        }
        fout<<endl;
    }
    fout.close();
}

float * setTabCoeff(){
    ifstream fin("./config.txt");
    if(fin.is_open()){
        int nbLines(0);
        char buffer[256];
        cout<<endl<<"Strahler coeff tab : "<<endl;
        while(fin.getline(buffer, 256)){
            ++nbLines;
            cout<<buffer<<endl;
        }
        fin.close();
        fin.open("./config.txt");

        float * tabCoeff = new float[nbLines];
        for(int i(0); fin.ignore(256, ':'); i++){
            fin.getline(buffer, 256);
            tabCoeff[i] = atof(buffer);

        }
        fin.close();

        for(int i(0); i< nbLines;++i)
            cout<<i<<"  "<<tabCoeff[i]<<endl;

        return tabCoeff;
    }else{
        cout<<endl<<"ERROR : impossible to open config.txt file."<<endl<<endl;
        exit(1);
    }
}

void abort(char* prog_name){
    cout<<endl<<endl;
    cout<<prog_name<<" has exit unespectedly !"<<endl<<endl;
    exit(1);
}

int main(int argc, char** argv){

    char * fileIn = NULL;
    char * fileSub = NULL;
    char * fileStrahler = NULL;
    char * fileOut = NULL;

    HEADER head;
    int coeff(1);

    float ** Z=NULL;
    float ** W=NULL;
    float ** S=NULL;

    int options;
    bool opt_arg_i(false); //input flag
    bool opt_arg_a(false); //save results .asc
    bool opt_arg_b(false); //save results .bin
    bool opt_arg_c(false); //stream burning coeff
    bool opt_arg_d(false); //directions flow flag
    bool opt_arg_l(false); //header reader
    bool opt_arg_m(false); //export .mat flag (doesn't work for now)
    bool opt_arg_e(false); //export flag : simply save the input file into ascii format
    bool opt_arg_u(false); //immersion flag
    bool opt_arg_s(false); //stream burning flag
    bool opt_arg_t(false); //write test matrix flag
    while((options=getopt(argc,argv,"i:a:b:c:d:e:m:u:t:s:l:"))!=-1){
        switch(options){
        case 'i':
            opt_arg_i = true;
            fileIn = optarg;
            break;
        case 'a':
            opt_arg_a = true;
            fileOut = optarg;
            break;
        case 'b':
            opt_arg_b = true;
            fileOut = optarg;
            break;
        case 'c':
            opt_arg_c = true;
            coeff = atoi(optarg);
            break;
        case 'd':
            opt_arg_d = true;
            fileOut = optarg;
            break;
        case 'l':
            opt_arg_l = true;
            fileIn = optarg;
            break;
        case 'm':
            opt_arg_m = true;
            fileOut = optarg;
            break;
        case 'e':
            opt_arg_e = true;
            fileOut = optarg;
            break;
        case 'u':
            opt_arg_u = true;
            fileSub = optarg;
            break;
        case 's':
            opt_arg_s = true;
            fileStrahler = optarg;
            break;
        case 't':
            opt_arg_t = true;
            fileOut = optarg;
            break;
        default:
            abort(argv[0]);
        }
    }



    if(opt_arg_t)
        writeMatrixTest(fileOut);
    else if(opt_arg_l)
        readHeader(fileIn);
    else if(opt_arg_i){
        Z = readDEM(head, fileIn);
        if(opt_arg_d){
            W = directions(head, Z);
            exportDEM_ASC(head, W, fileOut);
        }
        else{
            if(opt_arg_s){
                S = readDEM(head, fileStrahler);
                if(opt_arg_c)
                    W = streamBurning(head, Z, S, coeff);
                else{
                    float * tabCoeff = setTabCoeff();
                    W = streamBurning(head, Z, S, tabCoeff);
                    delete tabCoeff;
                }
            }
            if(opt_arg_e){
                // exportDEM_BIN(head, Z, fileOut);
                exportDEM_ASC(head, W, fileOut);
            }else{
                W = subDEM(head, Z);
                if(opt_arg_u){
                    exportDEM_BIN(head, W, fileSub);
                }else{
                    fillTheSinks(head, Z, W);
                    if(opt_arg_a){
                        exportDEM_ASC(head, W, fileOut);
                    }else if(opt_arg_b){
                        exportDEM_BIN(head, W, fileOut);
                    }else if(opt_arg_m){
                        exportDEM_MAT(head, W, fileOut);
                    }
                }
            }
        }
        deleteTab2D(head, Z);

        if(opt_arg_a || opt_arg_b || opt_arg_m || opt_arg_d)
            deleteTab2D(head, W);
        if(opt_arg_s){
            deleteTab2D(head, W);
            deleteTab2D(head, S);
        }
    }else
        abort(argv[0]);

    //writeMatrixTest(argv[1]);


    cout<<endl<<"Operation Done."<<endl<<endl;
    return 0;
}
