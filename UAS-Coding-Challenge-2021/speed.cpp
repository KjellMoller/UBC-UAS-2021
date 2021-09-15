#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
using namespace std;

//  FINE

void parseCoordinates(string coordinates, double arr[2]){
string temp1 = "";
bool space = false;
  for(int i=0; i<coordinates.length(); i++){
    if(isdigit(coordinates[i]) || coordinates[i] == '.' || coordinates[i] == '-')
      temp1 += coordinates[i];
    else if(space == false){
      arr[0] = stod(temp1);
      temp1 = "";
      space = true;
    }
    else{
      arr[1] = stod(temp1);
      break;
    }
  }
}


//  FINE      Variables for calculations
float a = 6378.137;
double n = .00167922;
double A = (a/(1+n))*(1+pow(n,2)/4+pow(n,4)/64+pow(n,6)/256+pow(n,8)/1024+pow(n,10)/4096);
double temp;
// FINE   
double α[3] = {(n/2)-(2/3*pow(n,2)+(5/16*pow(n,3))),(13/48*pow(n,2))+(3/5*pow(n,3)),(61/240*pow(n,3))};
double β[3] = {(n/2)-(2/3*pow(n,2))+(37/96*pow(n,3)),(1/48*pow(n,2))+(1/15*pow(n,3)),(61/240*pow(n,3))};
double δ[3] = {(2*n)-(2/3*pow(n,3))-(2*pow(n,3)),(7/3*pow(n,2))-(8/5*pow(n,3)),(56/15*pow(n,3))};

void latToUTM(double input[2]){

  //BUG

  double t = sinh((atanh(sin(input[0])))-(2*pow(n,1/2)/(1+n)*atanh(2*pow(n,1/2)/(1+n)*sin(input[0]))));
  //cout << << endl;
  cout << sinh(atanh(sin(input[0]))-(2*pow(n,1/2)/(1+n)*atanh(2*pow(n,1/2)/(1+n)*sin(input[0])))) << endl;

  //  FINE
  double ξ = atan(t/cos(input[1]-123));
  double η = atanh(sin(input[1]-123)/(pow(1+pow(t,2),1/2)));


  
  input[0] = 0.9996*A;
  temp = η;
  for(int j = 1; j < 4; j++){
    temp += (α[j-1]*cos(2*j*ξ)*sinh(2*j*η));
  }
  input[0] *= temp;
  input[0] += 500000;

  temp = ξ;
  input[1] = 0.9996*A;
  for(int j = 1; j < 4; j++){
    temp += (α[j-1]*sin(2*j*ξ)*cosh(2*j*η));
  }
  input[1] *= (temp);
  
}


string getSpeed (string currentCoordinates, string lastCoordinates) {
    double current[2];
    double last[2];
    parseCoordinates(currentCoordinates, current);
    parseCoordinates(lastCoordinates, last);
    //cout << current[0] << " " << current[1] << endl;

    latToUTM(current);
    //cout << current[0] << " " << current[1] << endl;
    latToUTM(last);
    
    double velocity = abs((current[1]-last[1])/(current[0]-last[0]));
    string speed = to_string(velocity) + "m/s";
    return speed;
}

int main () {
  string coordinates;
  string lastCoordinates;
  ifstream waypoints;

  waypoints.open("missionwaypoints.txt");
 // cout << sinh(atanh(sin(49.2594))-((2*pow(n,1/2)/(1+n))*atanh((2*pow(n,1/2)/(1+n))*sin(49.2594)))) << endl;


  if (waypoints.is_open())
  {
    while (getline (waypoints,coordinates))
    {
       
        //cout << endl << "lol" << endl;
        cout << getSpeed(coordinates, lastCoordinates) << endl;
        //lastCoordinates = coordinates;

    //Sleep thread for 1 second before outputting next speed.
       usleep(1000000);
    }

    //Close file
    waypoints.close();
  }

  else cout << "Unable to open file"; 

  return 0;
}