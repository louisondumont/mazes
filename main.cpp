/** ------------------------------------------------ */
/** Developed by Louison Dumont                      */
/** http://louison.me/generate-all-mazes-of-size-nm/ */
/** (2014-05)                                        */
/** Feel free to contact me if you have any question */
/** ------------------------------------------------ */
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>     /* atoi */
#include <math.h>       /* pow */
#include <conio.h>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;


/** INIT */
ofstream logfile;

int n, N, M, p=4; // p=4 because 4 directions (left, top, right, bott)
int colonne=0;
std::vector< std::vector<int> > matrice; // will store current maze
std::vector<int> composition; // will store nodes values of current maze
std::vector<int> pathsLength; // will store the shortest path of each maze
int nbr1;
int nbrZeros = 0;
int lowestLevel; // used to store current shortest path
int counter = 0;
int invalidCounter = 0;
int level = 0;


/** FUNCTIONS used to Display the maze + some counters */
void dispMurHoriz() { cout << "---"; logfile << "----"; nbr1++; }
void dispMurVerti(){ cout << "|"; logfile << "|"; nbr1++; }
void dispBlank() { cout << "   "; logfile << "     "; }
void dispBlankH() { cout << "   "; logfile << "    "; }



void fillin(int pixelx, int pixely, int coltarget, int colrep) // fill in algo to search for closed spaces
{
    if(matrice[pixely][pixelx] == coltarget)
    {
        matrice[pixely][pixelx] = colrep; nbrZeros--;

        if(pixely>0) fillin(pixelx, pixely-1, coltarget, colrep); // top
        if(pixely<M*2) fillin(pixelx, pixely+1, coltarget, colrep); // bottom
        if(pixelx>0) fillin(pixelx-1, pixely, coltarget, colrep); // left
        if(pixelx<N*2) fillin(pixelx+1, pixely, coltarget, colrep); //right
    }
}


void searchPath(int pixelx, int pixely, int coltarget, int colrep, int level, int destX, int destY)
{
    if(pixelx == destX && pixely == destY && ((level)/2 < lowestLevel || lowestLevel==0)) { lowestLevel = (level)/2; }
    else if(matrice[pixely][pixelx] == coltarget)
    {
        matrice[pixely][pixelx] = colrep;

        if(pixely>0) searchPath(pixelx, pixely-1, coltarget, colrep, level+1, destX, destY); // top
        if(pixely<M*2) searchPath(pixelx, pixely+1, coltarget, colrep, level+1, destX, destY); // bottom
        if(pixelx>0) searchPath(pixelx-1, pixely, coltarget, colrep, level+1, destX, destY); // left
        if(pixelx<N*2) searchPath(pixelx+1, pixely, coltarget, colrep, level+1, destX, destY); // right

        matrice[pixely][pixelx] = coltarget;
    }
}


void fillAndCalc(std::vector<int> thevector)
{
    /** init the matrice */
    nbr1=0;

    for(int i=0; i<=2*N; i++){
        for(int j=0; j<=2*M; j++){
            matrice[j][i] = 0;
            }
    } // fill matrice with 0

    /** disp the maze and fill the matrice */
    for(int line=1;line<=(M*2+1);line++)
  {
      cout << endl; logfile << endl;

    for(int col=1;col<=(N*2+1);col++)
    {
        if(line%2 == 1) // disp horiz walls
            {
        int uppersquareNbr = (((line-1)/2)-1)*N+(col/2);
        int bottomRightNode = (uppersquareNbr%N==0)?0:(uppersquareNbr%N)+(N-1)*(((line-1)/2)-1); // if square on right wall, no node bottomright
        int bottomLeftNode = (uppersquareNbr%N==1)?0:((uppersquareNbr-1)%N)+(N-1)*(((line-1)/2)-1);
                if(col%2 == 1) { cout << "+"; logfile << "+"; matrice[line-1][col-1] = 1; }
                else
                {
                    if(line == 1 || line == (M*2+1)) { dispMurHoriz(); matrice[line-1][col-1] = 1; }
                    else
                    {       if(thevector[bottomRightNode] == 2) { dispMurHoriz(); matrice[line-1][col-1] = 1;}
                            else if(thevector[bottomLeftNode] == 8) { dispMurHoriz(); matrice[line-1][col-1] = 1; }
                            else { dispBlankH(); matrice[line-1][col-1] = 0;}
                    }
                }
            }
           else // disp vert walls
            {
        int leftsquareNbr = ((line/2)-1)*N+((col-1)/2);
        int bottomNode = (line==2*M)?0:(leftsquareNbr%N)+(N-1)*(((line-1+1)/2)-1);
        int topNode = (line==2)?0:(leftsquareNbr%N)+(N-1)*(((line-2)/2)-1);
                    if(col%2 == 0) { dispBlank(); matrice[line-1][col-1] = 0; }
                else
                {
                    if(col == 1 || col == (N*2+1)) { cout << " "; logfile << " "; matrice[line-1][col-1]=1;}
                    else
                    {       if(thevector[bottomNode] == 4) { dispMurVerti(); matrice[line-1][col-1] = 1; }
                            else if(thevector[topNode] == 16) { dispMurVerti(); matrice[line-1][col-1] = 1; }
                            else { cout << " "; logfile << " "; matrice[line-1][col-1] = 0; }
                    }
                }
            }
    }
  }



    /** Going to count 1 */
    nbr1 -= N*2; // delete top and bottom line from already incremented variable
    cout << endl << "Nbr walls = " << nbr1 << endl; logfile << endl << "Nbr walls = " << nbr1 << endl;


    /** Going to count zeros */
    nbrZeros=0; // init
    for(int i=0; i<=2*N; i++)
    {
        for(int j=0; j<=2*M; j++)
        {
            if(matrice[j][i] == 0) nbrZeros++;
        }
    }

    /** Going to search path */
    lowestLevel = 0;
    searchPath(1,1,0,9,0,2*N-1, 2*M-1);
    cout << endl << "||AZ|| = " << lowestLevel << endl; logfile << endl << "||AZ|| = " << lowestLevel << endl;

    /** Going to search if closed spaces exists thanks to fillin algo */
    fillin(1,1,0,3); // it will nbrZeros-- each time a square if zombified

    if(nbrZeros > 0)
    {
        invalidCounter++;
        cout << endl << "INVALID: closed space" << endl; logfile << endl << "INVALID: closed space" << endl;
    }
    else if(nbr1 != (N-1)*(M-1))
    {
        invalidCounter++;
        cout << endl << "INVALID: superposed walls" << endl; logfile << endl << "INVALID: superposed walls" << endl;
    }
    else // only if valid
        pathsLength.push_back(lowestLevel);

}

void setCompo(int colonne, int number) { composition.insert(composition.begin()+colonne, number); }

void dispAndCalc()
{
    cout << endl << endl;
    logfile << endl << endl;
    for(int i=1;i<=n;i++)
    {
        cout << composition[i] << " ";
        logfile << composition[i] << " ";
    }

    fillAndCalc(composition); // print maze and make all calculations
    counter++;

    cout << endl;
    logfile << endl;
}


void genCompo() // recursive
{
    colonne++;
    if(colonne==n+1)
    {
       dispAndCalc();
    }
    else
    {
      for(int j=1;j<=p;j++)
        {
                setCompo(colonne, pow(2, j));
                genCompo();
        }
    }
     colonne--;
}

void dispShortestPathsStats()
{
    /** Calc shortest paths average length*/
    sort(pathsLength.begin(), pathsLength.end());

    int counterOccurences = 1;
    float avg = 0;
    for(int i=0;i<=pathsLength.size()-1; i++)
    {
        avg = avg + pathsLength[i];
        if(i<pathsLength.size()-1 && pathsLength[i] == pathsLength[i+1]) counterOccurences++;
        else
        {
             cout << "Path: " << pathsLength[i] << " squares * " << counterOccurences << " times" << endl;
            logfile << "Path: " << pathsLength[i] << " squares * " << counterOccurences << " times" << endl;
            if(counterOccurences>1)
            {
                counterOccurences = 1;
            }
        }
    }

    avg = avg / pathsLength.size();
    cout << endl << "Average shortest path = " << avg << endl; logfile << endl << "Average shortest path = " << avg << endl;
}

void conclusion()
{
    cout << endl << "Total founds: " << counter-invalidCounter << " (theor. possibilities without conditions: " << pow(p,n) << " = " << p << "^" << n << ")" << endl;
    cout << endl << "Total invalids: " << invalidCounter << endl << endl;
    logfile << endl << "Total founds: " << counter-invalidCounter << " (theor. possibilities without conditions: " << pow(p,n) << " = " << p << "^" << n << ")" << endl;
    logfile << endl << "Total invalids: " << invalidCounter << endl << endl;

    cout << endl << "Results saved in '"<< N << " x " << M << ".txt'" << endl;
}


int main(int argc, char *argv[])
{
                cout << "mazes | Program to generate every mazes of a given n*m size" << endl << endl;
                cout << "# ------------------------------------------------ #" << endl;
                cout << "# Developed by Louison Dumont                      #" << endl;
                cout << "# http://louison.me/generate-all-mazes-of-size-nm/ #" << endl;
                cout << "# (2014-05)                                        #" << endl;
                cout << "# Feel free to contact me if you have any question #" << endl;
                cout << "# ------------------------------------------------ #" << endl;
                cout << endl;


        /** Questions */
                cout << "N = ";  cin >> N;
                cout << "M = "; cin >> M;
                n = (N-1)*(M-1);
                composition.assign(n, 0); // init for storing each mazes node chain
                std::vector<int> sub(N*2+1);
                matrice.assign(M*2+1, sub); // used to store current maze, now have enough space

        /** Custom maze display */
                char toDisp;
                cout << "Draw a custom maze? (y/n) : "; cin >> toDisp;

                if(toDisp == 'y')
                {
                     std::vector<int> toDisplay;
                     toDisplay.push_back(0);
                     int lett;

                    for(int i=1; i<=n; i++)
                    {
                        lett = 0;
                        while(lett == 0)
                        {
                            cout << endl << "Enter element number " << i << " (2 or 4 or 8 or 16): ";
                            cin >> lett;
                        }
                        toDisplay.push_back(lett);
                    }

                    fillAndCalc(toDisplay);

                    cout << endl << endl << "Press a key to quit...";
                    getch(); //holds screen until a key is pressed

                    return 0;
                }


    /** Logging init */
    char name[100];
    sprintf(name, "%d x %d.txt", N, M);
    logfile.open(name);
    /** ------------ */


        cout << endl << endl << "Generated Mazes:" << endl;
        genCompo(); // main function

        dispShortestPathsStats();

        conclusion();


    cout << endl << endl << "Press a key to quit...";
    getch(); //holds screen until a key is pressed

    return 0;
}
